<script>
  export let name;
  export let activeMenu;
  export let selectMenu;
  export let href = '';

  $: btnStyle = `
    background: ${activeMenu === name ? 'var(--accent-color)' : 'var(--menu-bg)'};
    color: ${activeMenu === name ? 'var(--menu-bg)' : 'var(--accent-color)'};
    border: none;
  `;
</script>

{#if href}
  <a href={href} target="_blank" rel="noopener noreferrer" class="w-full block text-left px-6 py-3 transition font-medium rounded" style={btnStyle}>
    {name}
  </a>
{:else}
  <button class="w-full text-left px-6 py-3 transition font-medium rounded" style={btnStyle} on:click={() => selectMenu(name)}>
    {name}
  </button>
{/if}
