<script>
  import { onMount, onDestroy } from 'svelte';
  import { createEventDispatcher } from 'svelte';
  export let open = false;
  export let message = 'Weet je het zeker?';

  function handleKey(e) {
    if (open && e.key === 'Escape') {
      cancel();
    }
  }
  onMount(() => window.addEventListener('keydown', handleKey));
  onDestroy(() => window.removeEventListener('keydown', handleKey));
  const dispatch = createEventDispatcher();
  function ok() {
    dispatch('ok');
    open = false;
  }
  function cancel() {
    dispatch('cancel');
    open = false;
  }
</script>

{#if open}
  <div class="fixed inset-0 z-50 flex items-center justify-center" style="background: var(--modal-backdrop, rgba(0,0,0,0.4));">
    <div class="rounded shadow-lg p-6 max-w-xs w-full text-center border" style="background: var(--card-bg); color: var(--text-color); border-color: var(--border-color);">
      <div class="mb-4">{message}</div>
      <div class="flex gap-4 justify-center">
        <button class="ok-btn" on:click={ok}>OK</button>
        <button class="cancel-btn" on:click={cancel}>Cancel</button>
      </div>
    </div>
  </div>
{/if}

<style>
  .ok-btn {
    background: var(--accent-color, #2563eb);
    color: var(--button-text, #fff);
    padding: 0.5rem 1.25rem;
    border-radius: 0.375rem;
    border: none;
    font-weight: 500;
    transition: background 0.2s;
    cursor: pointer;
  }
  .ok-btn:hover {
    background: var(--accent-color-hover, #1d4ed8);
  }
  .cancel-btn {
    background: var(--button-bg, #e5e7eb);
    color: var(--button-text, #1f2937);
    padding: 0.5rem 1.25rem;
    border-radius: 0.375rem;
    border: none;
    font-weight: 500;
    transition: background 0.2s;
    cursor: pointer;
  }
  .cancel-btn:hover {
    background: var(--button-bg-hover, #d1d5db);
  }
</style>
