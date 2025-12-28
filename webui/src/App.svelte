<!-- svelte-ignore a11y_click_events_have_key_events -->
<!-- svelte-ignore a11y_no_static_element_interactions -->
<script>
  import TempChart from './TempChart.svelte';
  import Logger from './Logger.svelte';
  import Control from './control/Control.svelte';
  import MenuButton from './MenuButton.svelte';
  import { globalError, clearGlobalError, setGlobalError } from './stores/globalError.js';
  import { onMount } from 'svelte';
  import { syncTimeZone } from './syncTime.js';
  import MqttForm from './mqtt/MqttForm.svelte';

  onMount(() => {
    syncTimeZone();
    window.addEventListener('error', (e) => {
      setGlobalError(e?.message ?? 'Onbekende fout');
    });
    window.addEventListener('unhandledrejection', (e) => {
      setGlobalError(e?.reason?.message ?? e?.reason ?? 'Promise fout');
    });
  });

  let menuOpen = false;
  let activeMenu = decodeURIComponent(window.location.hash.replace('#', '')) || '🏠 home';
  let darkMode = window.matchMedia('(prefers-color-scheme: dark)').matches;
  document.documentElement.setAttribute('data-theme', darkMode ? 'dark' : 'light');

  function toggleTheme() {
    darkMode = !darkMode;
    document.documentElement.setAttribute('data-theme', darkMode ? 'dark' : 'light');
  }

  function openMenu() {
    menuOpen = true;
  }
  function closeMenu() {
    menuOpen = false;
  }
  function selectMenu(menu) {
    activeMenu = menu;
    window.location.hash = encodeURIComponent(menu);
    closeMenu();
  }

  window.addEventListener('hashchange', () => {
    activeMenu = decodeURIComponent(window.location.hash.replace('#', '')) || '🏠 home';
  });
</script>

<main class="min-h-screen flex flex-col items-center px-2 py-4" style="background: var(--card-bg); color: var(--text-color);">
  {#if $globalError}
    <div class="fixed inset-0 z-50 flex items-center justify-center bg-black bg-opacity-40">
      <div class="bg-white dark:bg-gray-900 rounded shadow-lg p-6 max-w-md w-full text-center border border-red-600">
        <h2 class="text-xl font-bold text-red-700 mb-2">Error</h2>
        <div class="mb-4">{$globalError}</div>
        <button class="bg-red-700 text-white px-4 py-2 rounded hover:bg-red-800" on:click={clearGlobalError}>Close</button>
      </div>
    </div>
  {/if}
  <header class="flex items-center gap-2 w-full max-w-md mb-4">
    <button class="flex flex-col gap-1 p-2 mr-2 focus:outline-none" aria-label="Menu" on:click={openMenu}>
      <span class="block w-7 h-1 rounded" style="background: var(--icon-color)"></span>
      <span class="block w-7 h-1 rounded" style="background: var(--icon-color)"></span>
      <span class="block w-7 h-1 rounded" style="background: var(--icon-color)"></span>
    </button>
    <h1 class="text-2xl font-bold" style="color: var(--header-color)">{activeMenu}</h1>
    <button class="ml-auto px-3 py-1 rounded border border-gray-400 text-sm" on:click={toggleTheme} aria-label="Toggle dark/light mode">
      {darkMode ? '🌙 Dark' : '☀️ Light'}
    </button>
  </header>

  <div class="fixed inset-0 z-20 transition-opacity duration-200" style="background: var(--overlay-bg);" class:hidden={!menuOpen} on:click={closeMenu}></div>
  <nav
    class="fixed top-0 left-0 h-full w-64 shadow-lg z-30 transform transition-transform duration-200 flex flex-col"
    style="background: var(--menu-bg); color: var(--menu-text);"
    class:-translate-x-full={!menuOpen}
    class:translate-x-0={menuOpen}
  >
    <div class="flex items-center justify-between px-4 py-3 border-b font-semibold text-lg" style="border-color: var(--menu-border);">
      Menu
      <button class="text-2xl font-bold focus:outline-none" style="color: var(--accent-color);" aria-label="Close" on:click={closeMenu}>&times;</button>
    </div>
    <ul class="flex-1">
      <li>
        <MenuButton name="🏠 home" activeMenu={activeMenu} selectMenu={selectMenu} />
      </li>
      <li>
        <MenuButton name="📈 Graph" activeMenu={activeMenu} selectMenu={selectMenu} />
      </li>
      <li>
        <MenuButton name="📨 Mqtt Settings" activeMenu={activeMenu} selectMenu={selectMenu} />
      </li>
      <li>
        <MenuButton name="📝 Logger" activeMenu={activeMenu} selectMenu={selectMenu} />
      </li>
      <li>
        <MenuButton name="📶 Wifi Settings" href={`http://${window.location.hostname}:8080/`} />
      </li>
    </ul>
  </nav>
  {#if activeMenu === '🏠 home'}
    <Control />
  {/if}
  {#if activeMenu === '📨 Mqtt Settings'}
    <MqttForm />
  {/if}
  {#if activeMenu === '📈 Graph'}
    <TempChart />
  {/if}
  {#if activeMenu === '📝 Logger'}
    <Logger />
  {/if}
</main>
