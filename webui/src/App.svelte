<script>
  import TempChart from './TempChart.svelte';
  import Logger from './Logger.svelte';
  import Control from './control/Control.svelte';
  import MenuButton from './MenuButton.svelte';
  import { globalError, clearGlobalError, setGlobalError } from './stores/globalError.js';
  import Dialog from './components/Dialog.svelte';
  import { onMount } from 'svelte';
  import { syncTimeZone } from './syncTime.js';
  import MqttForm from './mqtt/MqttForm.svelte';

  let menuOpen = $state(false);
  let activeMenu = $state(decodeURIComponent(window.location.hash.replace('#', '')) || 'home');
  let darkMode = $state(window.matchMedia('(prefers-color-scheme: dark)').matches);
  let isDesktop = $state(window.innerWidth >= 1024);
  let errorDialogOpen = $state(false);

  $effect(() => {
    document.documentElement.setAttribute('data-theme', darkMode ? 'dark' : 'light');
  });

  function toggleTheme() {
    darkMode = !darkMode;
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

  $effect(() => {
    const handler = () => {
      activeMenu = decodeURIComponent(window.location.hash.replace('#', '')) || 'home';
    };
    window.addEventListener('hashchange', handler);
    return () => window.removeEventListener('hashchange', handler);
  });

  $effect(() => {
    const resizeHandler = () => {
      isDesktop = window.innerWidth >= 1024;
      if (isDesktop) menuOpen = false;
    };
    window.addEventListener('resize', resizeHandler);
    return () => window.removeEventListener('resize', resizeHandler);
  });

  $effect(() => {
    if ($globalError) {
      errorDialogOpen = true;
    }
  });

  onMount(() => {
    syncTimeZone();

    window.addEventListener('error', (e) => {
      setGlobalError(e?.message ?? 'Unknown error');
    });
    window.addEventListener('unhandledrejection', (e) => {
      const msg = e?.reason?.message ?? e?.reason ?? 'Promise error';
      if (!msg.includes('Failed to fetch') && !msg.includes('CORS')) {
        setGlobalError(msg);
      }
    });
  });
</script>

<div class="min-h-screen bg-[var(--bg-primary)] text-[var(--text-primary)] transition-colors duration-200">
  <Dialog
    bind:open={errorDialogOpen}
    type="error"
    title="Error"
    message={$globalError}
    onok={clearGlobalError}
  />

  <header class="sticky top-0 z-40 bg-[var(--bg-secondary)] border-b border-[var(--border-color)] shadow-sm lg:hidden">
    <div class="max-w-5xl mx-auto px-4 py-3 flex items-center gap-3">
      <button class="p-2 rounded-[var(--radius-md)] hover:bg-[var(--bg-hover)] transition focus:outline-none focus:ring-2 focus:ring-[var(--color-primary)] cursor-pointer" aria-label="Menu" onclick={openMenu}>
        <svg class="w-6 h-6 text-[var(--text-primary)]" fill="none" stroke="currentColor" viewBox="0 0 24 24">
          <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M4 6h16M4 12h16M4 18h16"/>
        </svg>
      </button>
      <h1 class="text-lg font-semibold capitalize text-[var(--text-primary)]">{activeMenu}</h1>
      <button class="ml-auto p-2 rounded-[var(--radius-md)] hover:bg-[var(--bg-hover)] transition focus:outline-none focus:ring-2 focus:ring-[var(--color-primary)] cursor-pointer" onclick={toggleTheme} aria-label="Toggle dark/light mode">
        {#if darkMode}
          <svg class="w-5 h-5 text-amber-400" fill="none" stroke="currentColor" viewBox="0 0 24 24">
            <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M12 3v1m0 16v1m9-9h-1M4 12H3m15.364 6.364l-.707-.707M6.343 6.343l-.707-.707m12.728 0l-.707.707M6.343 17.657l-.707.707M16 12a4 4 0 11-8 0 4 4 0 018 0z"/>
          </svg>
        {:else}
          <svg class="w-5 h-5 text-slate-600" fill="none" stroke="currentColor" viewBox="0 0 24 24">
            <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M20.354 15.354A9 9 0 018.646 3.646 9.003 9.003 0 0012 21a9.003 9.003 0 008.354-5.646z"/>
          </svg>
        {/if}
      </button>
    </div>
  </header>

  <div class="flex">
    <nav class="hidden lg:flex flex-col w-64 bg-[var(--bg-secondary)] border-r border-[var(--border-color)] min-h-screen sticky top-0">
      <div class="flex items-center justify-between px-4 py-4 border-b border-[var(--border-color)]">
        <span class="font-semibold text-lg text-[var(--text-primary)]">Tombask</span>
        <button class="p-2 rounded-[var(--radius-md)] hover:bg-[var(--bg-hover)] transition focus:outline-none focus:ring-2 focus:ring-[var(--color-primary)] cursor-pointer" onclick={toggleTheme} aria-label="Toggle dark/light mode">
          {#if darkMode}
            <svg class="w-5 h-5 text-amber-400" fill="none" stroke="currentColor" viewBox="0 0 24 24">
              <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M12 3v1m0 16v1m9-9h-1M4 12H3m15.364 6.364l-.707-.707M6.343 6.343l-.707-.707m12.728 0l-.707.707M6.343 17.657l-.707.707M16 12a4 4 0 11-8 0 4 4 0 018 0z"/>
            </svg>
          {:else}
            <svg class="w-5 h-5 text-slate-600" fill="none" stroke="currentColor" viewBox="0 0 24 24">
              <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M20.354 15.354A9 9 0 018.646 3.646 9.003 9.003 0 0012 21a9.003 9.003 0 008.354-5.646z"/>
            </svg>
          {/if}
        </button>
      </div>
      <ul class="py-2 flex-1">
        <li>
          <MenuButton name="home" label="Home" icon="home" {activeMenu} {selectMenu} />
        </li>
        <li>
          <MenuButton name="graph" label="Graph" icon="chart" {activeMenu} {selectMenu} />
        </li>
        <li>
          <MenuButton name="mqtt" label="MQTT Settings" icon="mqtt" {activeMenu} {selectMenu} />
        </li>
        <li>
          <MenuButton name="logger" label="Logger" icon="logger" {activeMenu} {selectMenu} />
        </li>
        <li>
          <MenuButton name="wifi" label="Wifi Settings" href={`http://${window.location.hostname}:8080/`} icon="wifi" />
        </li>
      </ul>
    </nav>

    <!-- svelte-ignore a11y_click_events_have_key_events -->
    <!-- svelte-ignore a11y_no_static_element_interactions -->
    <div class="fixed inset-0 z-40 bg-black/50 backdrop-blur-sm lg:hidden transition-opacity duration-200 cursor-pointer" class:opacity-0={!menuOpen} class:pointer-events-none={!menuOpen} onclick={closeMenu} role="button" tabindex="0" onkeydown={(e) => e.key === 'Enter' || e.key === ' ' ? closeMenu() : {}}></div>

    <nav class="fixed inset-0 bg-[var(--bg-secondary)] z-50 transform transition-transform duration-200 ease-out lg:hidden flex flex-col" class:translate-x-0={menuOpen} class:translate-x-[-100%]={!menuOpen}>
      <div class="flex items-center justify-between px-4 py-4 border-b border-[var(--border-color)]">
        <span class="font-semibold text-lg text-[var(--text-primary)]">Menu</span>
        <button class="p-2 rounded-[var(--radius-md)] hover:bg-[var(--bg-hover)] transition focus:outline-none focus:ring-2 focus:ring-[var(--color-primary)] cursor-pointer" aria-label="Close" onclick={closeMenu}>
          <svg class="w-6 h-6" fill="none" stroke="currentColor" viewBox="0 0 24 24">
            <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M6 18L18 6M6 6l12 12"/>
          </svg>
        </button>
      </div>
      <ul class="flex-1 py-4">
        <li>
          <MenuButton name="home" label="Home" icon="home" {activeMenu} {selectMenu} />
        </li>
        <li>
          <MenuButton name="graph" label="Graph" icon="chart" {activeMenu} {selectMenu} />
        </li>
        <li>
          <MenuButton name="mqtt" label="MQTT Settings" icon="mqtt" {activeMenu} {selectMenu} />
        </li>
        <li>
          <MenuButton name="logger" label="Logger" icon="logger" {activeMenu} {selectMenu} />
        </li>
        <li>
          <MenuButton name="wifi" label="Wifi Settings" href={`http://${window.location.hostname}:8080/`} icon="wifi" />
        </li>
      </ul>
      <div class="px-4 py-4 border-t border-[var(--border-color)]">
        <button class="w-full flex items-center justify-center gap-2 px-4 py-3 rounded-[var(--radius-md)] bg-[var(--bg-tertiary)] text-[var(--text-secondary)] hover:bg-[var(--bg-hover)] transition cursor-pointer" onclick={toggleTheme}>
          {#if darkMode}
            <svg class="w-5 h-5 text-amber-400" fill="none" stroke="currentColor" viewBox="0 0 24 24">
              <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M12 3v1m0 16v1m9-9h-1M4 12H3m15.364 6.364l-.707-.707M6.343 6.343l-.707-.707m12.728 0l-.707.707M6.343 17.657l-.707.707M16 12a4 4 0 11-8 0 4 4 0 018 0z"/>
            </svg>
            <span>Light Mode</span>
          {:else}
            <svg class="w-5 h-5 text-slate-600" fill="none" stroke="currentColor" viewBox="0 0 24 24">
              <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M20.354 15.354A9 9 0 018.646 3.646 9.003 9.003 0 0012 21a9.003 9.003 0 008.354-5.646z"/>
            </svg>
            <span>Dark Mode</span>
          {/if}
        </button>
      </div>
    </nav>

    <main class="flex-1 min-w-0 lg:p-6">
      {#if activeMenu === 'home'}
        <Control />
      {/if}
      {#if activeMenu === 'mqtt'}
        <MqttForm />
      {/if}
      {#if activeMenu === 'graph'}
        <TempChart />
      {/if}
      {#if activeMenu === 'logger'}
        <Logger />
      {/if}
    </main>
  </div>
</div>
