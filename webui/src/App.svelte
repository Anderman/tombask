<script>
  /**
   * API contract:
   *  GET  /api/config -> { deviceId, mqttHost, mqttPort, wifiSsid, wifiPassword: "" }
   *  POST /api/config (JSON) -> same shape. Server may ignore empty wifiPassword.
   */

  let error = '';
  let ok = '';

  let form = {
    deviceId: '',
    mqttHost: '',
    mqttPort: 1883,
  };

  let logsEnabled = false;
  let logLines = [];
  let es;

  async function load() {
    error = '';
    ok = '';
    try {
      const res = await fetch('/api/config', { headers: { 'Accept': 'application/json' } });
      if (!res.ok) throw new Error(`GET /api/config failed (${res.status})`);
      const data = await res.json();
      form = {
        deviceId: data.deviceId ?? '',
        mqttHost: data.mqttHost ?? '',
        mqttPort: Number(data.mqttPort ?? 1883),
      };
    } catch (e) {
      error = e?.message ?? String(e);
    }
  }

  async function save() {
    error = '';
    ok = '';
    try {
      const payload = {
        deviceId: form.deviceId,
        mqttHost: form.mqttHost,
        mqttPort: Number(form.mqttPort),
      };

      const res = await fetch('/api/config', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json', 'Accept': 'application/json' },
        body: JSON.stringify(payload)
      });
      if (!res.ok) throw new Error(`POST /api/config failed (${res.status})`);
      ok = 'Saved.';
      form.wifiPassword = '';
    } catch (e) {
      error = e?.message ?? String(e);
    } 
  }

  load();

  function startLogs() {
    logLines = [];
      es = new EventSource('/api/logs');
      logsEnabled = true;
      es.onmessage = (ev) => {
        const line = String(ev.data);
        logLines = [...logLines, line].slice(-200);
      };
  }

  function stopLogs() {
    if (es) es.close();
    logsEnabled = false;
  }
</script>

<main>
  <h1>Tombask Config</h1>

    <form on:submit|preventDefault={save}>
      <label>
        <span>Device ID</span>
        <input bind:value={form.deviceId} placeholder="tombask-kitchen" autocomplete="off" />
      </label>

      <label>
        <span>MQTT host</span>
        <input bind:value={form.mqttHost} placeholder="192.168.1.10" autocomplete="off" />
      </label>

      <label>
        <span>MQTT port</span>
        <input type="number" min="1" max="65535" step="1" bind:value={form.mqttPort} />
      </label>

      <div class="row">
        <button type="button" on:click={load} >Reload</button>
        <button type="submit">Save</button>
      </div>

      {#if error}
        <p class="error">{error}</p>
      {/if}
      {#if ok}
        <p class="ok">{ok}</p>
      {/if}

      <p class="hint">
        Note: the device typically does not return WiFi password; set it only when changing.
      </p>
    </form>

    <section class="logs">
      <div class="row">
        {#if logsEnabled}
          <button type="button" on:click={stopLogs}>Stop logs</button>
        {:else}
          <button type="button" on:click={startLogs}>Start logs</button>
        {/if}
      </div>

      {#if logLines.length}
        <pre class="logbox">{logLines.join('\n')}</pre>
      {/if}
    </section>
</main>

<style>
  :global(body) {
    margin: 0;
    font-family: system-ui, -apple-system, Segoe UI, Roboto, sans-serif;
    background: #0b0f17;
    color: #e6eaf2;
  }

 main {
    max-width: 520px;
    margin: 0 auto;
    padding: 20px;

    min-height: 100vh;
    display: flex;
    flex-direction: column;
    gap: 16px;
    box-sizing: border-box;
  }
  
  h1 {
    font-size: 20px;
    margin: 0 0 16px;
  }

  form {
    display: grid;
    gap: 12px;
    padding: 16px;
    border: 1px solid rgba(255, 255, 255, 0.12);
    border-radius: 12px;
    background: rgba(255, 255, 255, 0.03);
  }

  label {
    display: grid;
    gap: 6px;
  }

  label > span {
    font-size: 12px;
    opacity: 0.85;
  }

  input {
    width: 100%;
    box-sizing: border-box;
    padding: 10px 12px;
    border-radius: 10px;
    border: 1px solid rgba(255, 255, 255, 0.14);
    background: rgba(0, 0, 0, 0.35);
    color: inherit;
    outline: none;
  }

  input:focus {
    border-color: rgba(120, 170, 255, 0.8);
  }

  .row {
    display: flex;
    gap: 10px;
  }

  button {
    padding: 10px 12px;
    border-radius: 10px;
    border: 1px solid rgba(255, 255, 255, 0.14);
    background: rgba(255, 255, 255, 0.08);
    color: inherit;
    cursor: pointer;
  }

  .error {
    color: #ffb4b4;
    margin: 0;
  }

  .ok {
    color: #b7ffcb;
    margin: 0;
  }

  .hint {
    margin: 0;
    font-size: 12px;
    opacity: 0.7;
  }

 .logs {
    margin-top: 16px;

    /* breakout to full viewport width */
    width: 100vw;
    margin-left: calc(50% - 50vw);
    padding: 0 16px;
    box-sizing: border-box;
  }

  .logbox {
    margin: 10px 0 0;
    padding: 12px;
    border-radius: 10px;
    border: 1px solid rgba(255, 255, 255, 0.12);
    background: rgba(0, 0, 0, 0.25);

    width: 100%;
    max-width: none;

    max-height: 240px;
    overflow: auto;
    white-space: pre-wrap;
  }
</style>
