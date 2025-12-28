<script>
  import { onMount } from 'svelte';
  import { apiUrl } from './api';

  let logLines = [];
  let eventSource;
  let error = '';
  let autoScroll = true;

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
    eventSource.onerror = (e) => {
      error = 'Verbinding met logger verbroken';
    };
    return () => eventSource && eventSource.close();
  });
</script>

<div style="width:calc(100vw - 16px);max-width:calc(1700px);">
  <label style="display:block;margin-bottom:4px;">
    <input type="checkbox" bind:checked={autoScroll}>
    Auto-scroll
  </label>
  {#if error}
    <div class="text-red-600 mb-2">{error}</div>
  {/if}
  <pre id="logview" class="bg-black text-green-400 rounded p-2 overflow-y-auto flex-1" style="max-height:calc(100vh - 112px); line-height:0.6em;">
    {#each logLines as line}
      {line}
    {/each}
  </pre>
</div>

<style>
  #logview {
    font-family: monospace;
  }
</style>
