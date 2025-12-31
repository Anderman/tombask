<script>
  let {
    name,
    label,
    activeMenu,
    selectMenu,
    href = '',
    icon = ''
  } = $props();

  const iconPaths = {
    home: 'M3 12l2-2m0 0l7-7 7 7M5 10v10a1 1 0 001 1h3m10-11l2 2m-2-2v10a1 1 0 01-1 1h-3m-6 0a1 1 0 001-1v-4a1 1 0 011-1h2a1 1 0 011 1v4a1 1 0 001 1m-6 0h6',
    chart: 'M9 19v-6a2 2 0 00-2-2H5a2 2 0 00-2 2v6a2 2 0 002 2h2a2 2 0 002-2zm0 0V9a2 2 0 012-2h2a2 2 0 012 2v10m-6 0a2 2 0 002 2h2a2 2 0 002-2m0 0V5a2 2 0 012-2h2a2 2 0 012 2v14a2 2 0 01-2 2h-2a2 2 0 01-2-2z',
    mqtt: 'M8.111 16.404a5.5 5.5 0 017.778 0M12 20h.01m-7.08-7.071c3.904-3.905 10.236-3.905 14.141 0M1.394 9.393c5.857-5.857 15.355-5.857 21.213 0',
    logger: 'M9 12h6m-6 4h6m2 5H7a2 2 0 01-2-2V5a2 2 0 012-2h5.586a1 1 0 01.707.293l5.414 5.414a1 1 0 01.293.707V19a2 2 0 01-2 2z',
    wifi: 'M8.111 16.404a5.5 5.5 0 017.778 0M12 20h.01m-7.08-7.071c3.904-3.905 10.236-3.905 14.141 0M1.394 9.393c5.857-5.857 15.355-5.857 21.213 0'
  };

  let isActive = $derived(activeMenu === name);
  let btnClass = $derived(`
    w-full flex items-center gap-3 px-3 py-2 text-left transition-all duration-150 rounded-[var(--radius-md)] cursor-pointer
    ${isActive
      ? 'bg-[var(--bg-tertiary)] text-[var(--color-primary)] font-semibold'
      : 'text-[var(--text-secondary)] hover:bg-[var(--bg-hover)] hover:text-[var(--text-primary)]'}
  `);
</script>

{#if href}
  <a href={href} target="_blank" rel="noopener noreferrer" class="{btnClass} cursor-pointer">
    <svg class="w-5 h-5 flex-shrink-0" fill="none" stroke="currentColor" viewBox="0 0 24 24">
      <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d={iconPaths[icon] || iconPaths.home}/>
    </svg>
    <span class="font-medium">{label}</span>
  </a>
{:else}
  <button class={btnClass} onclick={() => selectMenu(name)}>
    <svg class="w-5 h-5 flex-shrink-0" fill="none" stroke="currentColor" viewBox="0 0 24 24">
      <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d={iconPaths[icon] || iconPaths.home}/>
    </svg>
    <span class="font-medium">{label}</span>
  </button>
{/if}
