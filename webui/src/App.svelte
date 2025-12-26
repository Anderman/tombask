<script>
  /**
   * API contract:
    *  GET  /api/config -> { deviceId, mqttHost, mqttPort }
   *  POST /api/config (JSON) -> same shape. Server may ignore empty wifiPassword.
   */

  let error = '';
  let ok = '';
  let loading = false;
  let saving = false;

  let form = {
    deviceId: 1,
    mqttHost: '',
    mqttPort: 1883,
    mqttUser: '',
    mqttPass: '',
  };

  let logsEnabled = false;
  let logLines = [];
  let es;
  let logContainer;

  $: if (logContainer && logLines.length) {
    setTimeout(() => {
      logContainer.scrollTop = logContainer.scrollHeight;
    }, 0);
  }

  async function load() {
    error = '';
    ok = '';
    loading = true;
    try {
      const res = await fetch('/api/config', { headers: { 'Accept': 'application/json' } });
      if (!res.ok) throw new Error(`Failed to load config (${res.status})`);
      const data = await res.json();
      form = {
        deviceId: Number(data.deviceId ?? 1),
        mqttHost: data.mqttHost ?? '',
        mqttPort: Number(data.mqttPort ?? 1883),
        mqttUser: data.mqttUser ?? '',
        mqttPass: '',
      };
    } catch (e) {
      error = e?.message ?? String(e);
    } finally {
      loading = false;
    }
  }

  async function save() {
    error = '';
    ok = '';
    saving = true;
    try {
      const payload = {
        deviceId: Number(form.deviceId),
        mqttHost: form.mqttHost,
        mqttPort: Number(form.mqttPort),
        mqttUser: form.mqttUser,
      };

      // Only send password if user typed one (empty means "keep current")
      if (String(form.mqttPass ?? '').length > 0) {
        payload.mqttPass = form.mqttPass;
      }

      const res = await fetch('/api/config', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json', 'Accept': 'application/json' },
        body: JSON.stringify(payload)
      });
      if (!res.ok) {
        const text = await res.text();
        throw new Error(text || `Failed to save (${res.status})`);
      }
      ok = 'Configuration saved successfully';
      setTimeout(() => { ok = ''; }, 3000);
      form.mqttPass = '';
    } catch (e) {
      error = e?.message ?? String(e);
    } finally {
      saving = false;
    }
  }

  load();

  function startLogs() {
    error = ''; // Clear any previous errors
    logLines = [];
    es = new EventSource('/api/logs');
    logsEnabled = true;
    es.onmessage = (ev) => {
      const line = String(ev.data);
      logLines = [...logLines, line].slice(-200);
    };
    es.onerror = () => {
      error = 'Lost connection to log stream';
      stopLogs();
    };
  }

  function stopLogs() {
    if (es) es.close();
    logsEnabled = false;
  }

  function clearLogs() {
    logLines = [];
  }

  function copyLogs() {
    const text = logLines.join('\n');
    navigator.clipboard.writeText(text).then(() => {
      ok = 'Logs copied to clipboard';
      setTimeout(() => { ok = ''; }, 2000);
    }).catch(() => {
      error = 'Failed to copy logs';
    });
  }
</script>

<main>
  <header>
    <h1>🔧 Tombask Config</h1>
    <p class="subtitle">ESP32 Heat Pump Controller</p>
  </header>

  {#if error}
    <div class="alert alert-error">
      <span class="alert-icon">⚠️</span>
      <span class="alert-text">{error}</span>
    </div>
  {/if}
  {#if ok}
    <div class="alert alert-success">
      <span class="alert-icon">✓</span>
      <span class="alert-text">{ok}</span>
    </div>
  {/if}

  <section class="card">
    <h2>Device Configuration</h2>
    <form on:submit|preventDefault={save}>
      <label>
        <span class="label-text">Device ID <small>(1-255)</small></span>
        <input 
          type="number" 
          min="1" 
          max="255" 
          step="1" 
          bind:value={form.deviceId}
          disabled={saving || loading}
        />
      </label>

      <label>
        <span class="label-text">MQTT Host</span>
        <input 
          bind:value={form.mqttHost} 
          placeholder="192.168.1.10" 
          autocomplete="off"
          disabled={saving || loading}
        />
      </label>

      <label>
        <span class="label-text">MQTT Port</span>
        <input 
          type="number" 
          min="1" 
          max="65535" 
          step="1" 
          bind:value={form.mqttPort}
          disabled={saving || loading}
        />
      </label>

      <label>
        <span class="label-text">MQTT Username</span>
        <input
          bind:value={form.mqttUser}
          placeholder="(optional)"
          autocomplete="username"
          disabled={saving || loading}
        />
      </label>

      <label>
        <span class="label-text">MQTT Password</span>
        <input
          type="password"
          bind:value={form.mqttPass}
          placeholder="Leave blank to keep"
          autocomplete="current-password"
          disabled={saving || loading}
        />
      </label>

      <div class="button-row">
        <button 
          type="button" 
          class="btn btn-secondary" 
          on:click={load}
          disabled={loading || saving}
        >
          {loading ? '⟳ Loading...' : '↻ Reload'}
        </button>
        <button 
          type="submit" 
          class="btn btn-primary"
          disabled={loading || saving}
        >
          {saving ? '⟳ Saving...' : '💾 Save'}
        </button>
      </div>
    </form>
  </section>

  <section class="card logs-section">
    <div class="logs-header">
      <div class="logs-title">
        <h2>Debug Logs</h2>
        {#if logsEnabled}
          <span class="status-badge status-active">● Live</span>
        {:else}
          <span class="status-badge status-inactive">○ Stopped</span>
        {/if}
        {#if logLines.length > 0}
          <span class="log-count">{logLines.length} lines</span>
        {/if}
      </div>
      <div class="button-row">
        {#if logsEnabled}
          <button type="button" class="btn btn-small btn-secondary" on:click={stopLogs}>⏸ Stop</button>
          {#if logLines.length > 0}
            <button type="button" class="btn btn-small btn-secondary" on:click={clearLogs}>🗑 Clear</button>
            <button type="button" class="btn btn-small btn-secondary" on:click={copyLogs}>📋 Copy</button>
          {/if}
        {:else}
          <button type="button" class="btn btn-small btn-primary" on:click={startLogs}>▶ Start Logs</button>
        {/if}
      </div>
    </div>

    {#if logsEnabled && logLines.length === 0}
      <div class="logs-empty">
        <div class="spinner"></div>
        <p>Waiting for log data...</p>
      </div>
    {/if}

    {#if logLines.length > 0}
      <pre class="logbox" bind:this={logContainer}>{logLines.join('\n')}</pre>
    {/if}
  </section>
</main>

<style>
  :global(body) {
    margin: 0;
    font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, 'Helvetica Neue', Arial, sans-serif;
    background: linear-gradient(135deg, #0f172a 0%, #1e293b 100%);
    color: #e2e8f0;
    line-height: 1.6;
  }

  main {
    max-width: 600px;
    margin: 0 auto;
    padding: 16px;
    min-height: 100vh;
    display: flex;
    flex-direction: column;
    gap: 20px;
    box-sizing: border-box;
  }

  @media (min-width: 768px) {
    main {
      padding: 32px 24px;
      gap: 24px;
    }
  }

  @media (min-width: 1024px) {
    main {
      max-width: 900px;
    }
  }

  @media (min-width: 1280px) {
    main {
      max-width: 1100px;
    }
  }

  header {
    text-align: center;
    padding: 8px 0;
  }

  h1 {
    font-size: 28px;
    font-weight: 700;
    margin: 0 0 4px;
    color: #f1f5f9;
    letter-spacing: -0.5px;
  }

  @media (min-width: 768px) {
    h1 {
      font-size: 32px;
    }
  }

  .subtitle {
    margin: 0;
    font-size: 14px;
    color: #94a3b8;
    font-weight: 500;
  }

  h2 {
    font-size: 18px;
    font-weight: 600;
    margin: 0 0 16px;
    color: #f8fafc;
  }

  .card {
    background: rgba(30, 41, 59, 0.8);
    backdrop-filter: blur(10px);
    border: 1px solid rgba(148, 163, 184, 0.1);
    border-radius: 16px;
    padding: 20px;
    box-shadow: 0 4px 6px -1px rgba(0, 0, 0, 0.3), 0 2px 4px -1px rgba(0, 0, 0, 0.2);
  }

  @media (min-width: 768px) {
    .card {
      padding: 24px;
    }
  }

  form {
    display: grid;
    gap: 16px;
  }

  label {
    display: flex;
    flex-direction: column;
    gap: 6px;
  }

  .label-text {
    font-size: 13px;
    font-weight: 500;
    color: #cbd5e1;
    display: flex;
    align-items: baseline;
    gap: 6px;
  }

  .label-text small {
    font-size: 11px;
    opacity: 0.7;
    font-weight: 400;
  }

  input {
    width: 100%;
    box-sizing: border-box;
    padding: 12px 14px;
    border-radius: 10px;
    border: 1px solid rgba(148, 163, 184, 0.2);
    background: rgba(15, 23, 42, 0.6);
    color: #f1f5f9;
    font-size: 15px;
    outline: none;
    transition: all 0.2s ease;
  }

  input:focus {
    border-color: rgba(96, 165, 250, 0.6);
    background: rgba(15, 23, 42, 0.8);
    box-shadow: 0 0 0 3px rgba(96, 165, 250, 0.1);
  }

  input:disabled {
    opacity: 0.5;
    cursor: not-allowed;
  }

  input:focus-visible {
    outline: none;
  }

  input::placeholder {
    color: #64748b;
  }

  /* Better number input appearance */
  input[type="number"]::-webkit-inner-spin-button,
  input[type="number"]::-webkit-outer-spin-button {
    opacity: 1;
  }

  .button-row {
    display: flex;
    gap: 10px;
    margin-top: 8px;
    flex-wrap: wrap;
  }

  .btn {
    flex: 1;
    min-width: 120px;
    padding: 12px 20px;
    border-radius: 10px;
    border: none;
    font-size: 14px;
    font-weight: 600;
    cursor: pointer;
    transition: all 0.2s ease;
    display: inline-flex;
    align-items: center;
    justify-content: center;
    gap: 6px;
    white-space: nowrap;
  }

  .btn-primary {
    background: linear-gradient(135deg, #3b82f6 0%, #2563eb 100%);
    color: white;
    box-shadow: 0 2px 4px rgba(37, 99, 235, 0.3);
  }

  .btn-primary:hover:not(:disabled) {
    background: linear-gradient(135deg, #2563eb 0%, #1d4ed8 100%);
    box-shadow: 0 4px 8px rgba(37, 99, 235, 0.4);
    transform: translateY(-1px);
  }

  .btn-primary:active:not(:disabled) {
    transform: translateY(0);
  }

  .btn-secondary {
    background: rgba(51, 65, 85, 0.8);
    color: #cbd5e1;
    border: 1px solid rgba(148, 163, 184, 0.2);
  }

  .btn-secondary:hover:not(:disabled) {
    background: rgba(71, 85, 105, 0.9);
    color: #f1f5f9;
  }

  .btn-small {
    min-width: auto;
    padding: 8px 14px;
    font-size: 13px;
  }

  .btn:disabled {
    opacity: 0.5;
    cursor: not-allowed;
    transform: none !important;
  }

  .btn:focus-visible {
    outline: 2px solid rgba(96, 165, 250, 0.5);
    outline-offset: 2px;
  }

  .alert {
    padding: 12px 16px;
    border-radius: 10px;
    display: flex;
    align-items: center;
    gap: 10px;
    font-size: 14px;
    animation: slideIn 0.3s ease;
  }

  @keyframes slideIn {
    from {
      opacity: 0;
      transform: translateY(-10px);
    }
    to {
      opacity: 1;
      transform: translateY(0);
    }
  }

  .alert-error {
    background: rgba(239, 68, 68, 0.15);
    border: 1px solid rgba(239, 68, 68, 0.3);
    color: #fca5a5;
  }

  .alert-success {
    background: rgba(34, 197, 94, 0.15);
    border: 1px solid rgba(34, 197, 94, 0.3);
    color: #86efac;
  }

  .alert-icon {
    font-size: 18px;
    flex-shrink: 0;
  }

  .alert-text {
    flex: 1;
  }

  .logs-section {
    flex: 1;
    display: flex;
    flex-direction: column;
    min-height: 300px;
  }

  @media (min-width: 768px) {
    .logs-section {
      min-height: 400px;
    }
  }

  @media (min-width: 1024px) {
    .logs-section {
      min-height: 500px;
    }
  }

  .logs-header {
    display: flex;
    justify-content: space-between;
    align-items: center;
    margin-bottom: 12px;
    flex-wrap: wrap;
    gap: 12px;
  }

  .logs-title {
    display: flex;
    align-items: center;
    gap: 10px;
    flex-wrap: wrap;
  }

  .logs-header h2 {
    margin: 0;
  }

  .status-badge {
    font-size: 12px;
    font-weight: 600;
    padding: 4px 10px;
    border-radius: 12px;
    display: inline-flex;
    align-items: center;
    gap: 4px;
  }

  .status-active {
    background: rgba(34, 197, 94, 0.15);
    color: #86efac;
    border: 1px solid rgba(34, 197, 94, 0.3);
  }

  .status-inactive {
    background: rgba(100, 116, 139, 0.15);
    color: #94a3b8;
    border: 1px solid rgba(100, 116, 139, 0.3);
  }

  .log-count {
    font-size: 12px;
    color: #64748b;
    font-weight: 500;
  }

  .logs-empty {
    text-align: center;
    padding: 60px 20px;
    color: #64748b;
    font-size: 14px;
    display: flex;
    flex-direction: column;
    align-items: center;
    gap: 16px;
  }

  .logs-empty p {
    margin: 0;
  }

  .spinner {
    width: 32px;
    height: 32px;
    border: 3px solid rgba(148, 163, 184, 0.2);
    border-top-color: #60a5fa;
    border-radius: 50%;
    animation: spin 0.8s linear infinite;
  }

  @keyframes spin {
    to { transform: rotate(360deg); }
  }

  .logbox {
    flex: 1;
    margin: 0;
    padding: 14px;
    border-radius: 10px;
    background: rgba(15, 23, 42, 0.8);
    border: 1px solid rgba(148, 163, 184, 0.15);
    color: #94a3b8;
    font-family: 'SF Mono', 'Monaco', 'Consolas', monospace;
    font-size: 12px;
    line-height: 1.5;
    overflow: auto;
    white-space: pre-wrap;
    word-wrap: break-word;
    max-height: 300px;
  }

  @media (min-width: 768px) {
    .logbox {
      font-size: 13px;
      max-height: 500px;
      padding: 16px;
    }
  }

  @media (min-width: 1024px) {
    .logbox {
      max-height: 600px;
      font-size: 13px;
    }
  }

  @media (min-width: 1280px) {
    .logbox {
      max-height: 700px;
    }
  }

  .logbox::-webkit-scrollbar {
    width: 8px;
    height: 8px;
  }

  .logbox::-webkit-scrollbar-track {
    background: rgba(15, 23, 42, 0.5);
    border-radius: 4px;
  }

  .logbox::-webkit-scrollbar-thumb {
    background: rgba(148, 163, 184, 0.3);
    border-radius: 4px;
  }

  .logbox::-webkit-scrollbar-thumb:hover {
    background: rgba(148, 163, 184, 0.5);
  }
</style>
