<script>
  import { onMount } from 'svelte';
  import { apiUrl } from './api';

  let logLines = $state([]);
  let eventSource;
  let error = $state('');
  let autoScroll = $state(true);
  let filterController = $state(false);

  const loggerApi = apiUrl('/api/logs');
  onMount(() => {
    eventSource = new EventSource(loggerApi);
    eventSource.onmessage = (e) => {
      error = '';
      logLines = [...logLines, e.data];
      if (autoScroll) {
        setTimeout(() => {
          const el = document.getElementById('logview');
          if (el) el.scrollTop = el.scrollHeight;
        });
      }
    };
    eventSource.onerror = () => {
      error = 'Connection to logger lost';
    };
    return () => eventSource && eventSource.close();
  });

  function isControllerLine(line) {
    return /^7E 7E F0 01 [0-9A-Fa-f]{2} [0-9A-Fa-f]{2} [0-9A-Fa-f]{2} [0-9A-Fa-f]{2} 02/.test(line);
  }

  let filteredLines = $derived(filterController ? logLines.filter(isControllerLine) : logLines);
</script>

<div style="width:calc(100vw - 16px);max-width:1700px;">
  <div style="display:flex;gap:1rem;align-items:center;margin-bottom:8px;">
    <label style="color: var(--text-secondary); cursor: pointer;">
      <input type="checkbox" bind:checked={autoScroll} style="cursor: pointer;">
      Auto-scroll
    </label>
    <label style="color: var(--text-secondary); cursor: pointer;">
      <input type="checkbox" bind:checked={filterController} style="cursor: pointer;">
      Wired Controller Only (7E 7E F0 01...)
    </label>
    <span style="color: var(--text-muted); font-size: 0.875rem;">({filteredLines.length} / {logLines.length} lines)</span>
  </div>
  {#if error}
    <div class="mb-2" style="color: var(--color-danger);">{error}</div>
  {/if}
  <pre id="logview" class="overflow-y-auto flex-1" style="max-height:calc(100vh - 112px); line-height:0.6em; background: var(--bg-primary); color: var(--color-success); border-radius: var(--radius-md); padding: 0.5rem;">
    {#each filteredLines as line}
      {line}
    {/each}
  </pre>
</div>

<style>
  #logview {
    font-family: monospace;
  }
</style>
