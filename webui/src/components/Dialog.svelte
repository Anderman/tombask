<script>
  import { onMount, onDestroy } from 'svelte';

  let {
    open = $bindable(false),
    type = 'confirm',
    title = '',
    message = '',
    onok = () => {},
    oncancel = () => {},
    onclose = () => {}
  } = $props();

  function handleKey(e) {
    if (open && e.key === 'Escape') {
      close();
    }
  }

  onMount(() => window.addEventListener('keydown', handleKey));
  onDestroy(() => window.removeEventListener('keydown', handleKey));

  function handleBackdropClick(e) {
    if (e.target === e.currentTarget) {
      close();
    }
  }

  function close() {
    onclose();
    open = false;
  }

  function ok() {
    onok();
    open = false;
  }

  function cancel() {
    oncancel();
    open = false;
  }
</script>

{#if open}
  <!-- svelte-ignore a11y_click_events_have_key_events -->
  <!-- svelte-ignore a11y_no_static_element_interactions -->
  <div
    class="fixed inset-0 z-50 flex items-center justify-center bg-black/50 backdrop-blur-sm p-4 cursor-pointer"
    onclick={handleBackdropClick}
    role="dialog"
    aria-modal="true"
    tabindex="-1"
  >
    <div class="bg-[var(--bg-secondary)] rounded-[var(--radius-lg)] shadow-[var(--shadow-lg)] p-6 max-w-sm w-full border border-[var(--border-color)]">
      {#if title}
        <h2 class="text-xl font-semibold mb-3 {type === 'error' ? 'text-[var(--color-danger)]' : 'text-[var(--text-primary)]'}">{title}</h2>
      {/if}
      <div class="mb-6 text-[var(--text-secondary)] text-center">{message}</div>
      <div class="flex gap-3 justify-center">
        {#if type === 'error'}
          <button
            class="px-5 py-2.5 rounded-[var(--radius-md)] font-medium transition-all duration-150 bg-[var(--color-danger)] text-white hover:bg-red-700 shadow-sm cursor-pointer"
            onclick={ok}
          >
            Close
          </button>
        {:else}
          <button
            class="px-5 py-2.5 rounded-[var(--radius-md)] font-medium transition-all duration-150 bg-[var(--bg-tertiary)] text-[var(--text-secondary)] hover:bg-[var(--bg-hover)] hover:text-[var(--text-primary)] cursor-pointer"
            onclick={cancel}
          >
            Cancel
          </button>
          <button
            class="px-5 py-2.5 rounded-[var(--radius-md)] font-medium transition-all duration-150 bg-[var(--color-accent)] text-white hover:opacity-90 shadow-sm cursor-pointer"
            onclick={ok}
          >
            OK
          </button>
        {/if}
      </div>
    </div>
  </div>
{/if}
