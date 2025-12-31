<script>
  import { apiGet, apiPost } from '../api';
  import Thermostat from './Thermostat.svelte';
  import { onMount } from 'svelte';
  import Dialog from '../components/Dialog.svelte';

  let confirmBoost = $state(false);
  let confirmLegionella = $state(false);
  let legionellaTemp = $state(0);
  let legionellaTargetTemp = $state(60);
  let setPointTemp = $state(0);
  let topTemp = $state(0);
  let bottomTemp = $state(0);
  let power = $state(0);
  let fanOn = $state(false);
  let boostOn = $state(false);
  let legionellaOn = $state(false);
  let legionellaHour = $state(2);
  let paused = $state(false);

  onMount(() => {
    async function pollStatus() {
      if (!paused) {
        const data = await apiGet('/api/status');
        legionellaTemp = data.LegionellaTemp ?? legionellaTemp;
        legionellaTargetTemp = data.LegionellaTargetTemp ?? data.LegionellaTemp ?? legionellaTargetTemp;
        setPointTemp = data.SetpointTemp ?? setPointTemp;
        topTemp = data.TopTemp ?? topTemp;
        bottomTemp = data.BottomTemp ?? bottomTemp;
        power = data.Power ?? power;
        fanOn = !!data.Fan;
        boostOn = !!data.Boost;
        legionellaOn = !!data.LegionellaOn;
        legionellaHour = data.LegionellaHour ?? legionellaHour;
      }
    }
    pollStatus();
    const interval = setInterval(pollStatus, 1000);
    return () => clearInterval(interval);
  });

  async function setTargetTemp(val) {
    paused = true;
    await apiPost('/api/setpoint', { SetpointTemp: val });
    paused = false;
  }

  async function toggleFan() {
    paused = true;
    fanOn = !fanOn;
    await apiPost('/api/fan', { Fan: fanOn });
    paused = false;
  }

  async function toggleBoost() {
    if (!boostOn) {
      confirmBoost = true;
    } else {
      paused = true;
      boostOn = false;
      await apiPost('/api/boost', { Boost: false });
      paused = false;
    }
  }

  async function confirmBoostOk() {
    confirmBoost = false;
    paused = true;
    boostOn = true;
    await apiPost('/api/boost', { Boost: true });
    paused = false;
  }

  function confirmBoostCancel() {
    confirmBoost = false;
  }

  async function toggleLegionella() {
    if (!legionellaOn) {
      confirmLegionella = true;
    } else {
      paused = true;
      legionellaOn = false;
      await apiPost('/api/legionellaOn', { LegionellaOn: false });
      paused = false;
    }
  }

  async function confirmLegionellaOk() {
    confirmLegionella = false;
    paused = true;
    legionellaOn = true;
    await apiPost('/api/legionellaOn', { LegionellaOn: true });
    paused = false;
  }

  function confirmLegionellaCancel() {
    confirmLegionella = false;
  }

  async function setLegionellaHour(val) {
    paused = true;
    legionellaHour = val;
    await apiPost('/api/legionellaHour', { LegionellaHour: val });
    paused = false;
  }

  async function setLegionellaTemp(val) {
    paused = true;
    legionellaTargetTemp = val;
    await apiPost('/api/legionellaTemp', { LegionellaTemp: val });
    paused = false;
  }
</script>

<div class="space-y-6">
  <div class="bg-[var(--bg-secondary)] rounded-[var(--radius-lg)] border border-[var(--border-color)] shadow-sm overflow-hidden">
    <div class="px-6 py-4 border-b border-[var(--border-color)]">
      <h2 class="text-lg font-semibold text-[var(--text-primary)]">Thermostat Control</h2>
      <p class="text-sm text-[var(--text-muted)] mt-1">Adjust your desired temperature</p>
    </div>
    <div class="p-6">
      <Thermostat {topTemp} {setPointTemp} {power} onsetTarget={(val) => setTargetTemp(val)} />
    </div>
  </div>

  <div class="bg-[var(--bg-secondary)] rounded-[var(--radius-lg)] border border-[var(--border-color)] shadow-sm overflow-hidden">
    <div class="px-6 py-4 border-b border-[var(--border-color)]">
      <h2 class="text-lg font-semibold text-[var(--text-primary)]">Quick Controls</h2>
      <p class="text-sm text-[var(--text-muted)] mt-1">Toggle fan and boost modes</p>
    </div>
    <div class="p-6">
      <div class="grid grid-cols-2 gap-4">
        <button
          class="flex items-center justify-center gap-2 px-5 py-4 rounded-[var(--radius-md)] border-2 font-semibold transition-all duration-150 cursor-pointer
          {fanOn
            ? 'bg-[var(--color-accent)] border-[var(--color-accent)] text-white shadow-md'
            : 'border-[var(--border-color)] text-[var(--text-secondary)] hover:border-[var(--color-accent)] hover:text-[var(--color-accent)]'}"
          aria-pressed={fanOn}
          onclick={toggleFan}
        >
          <svg class="w-5 h-5" fill="none" stroke="currentColor" viewBox="0 0 24 24">
            <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M14 10l-2 1m0 0l-2-1m2 1v2.5M20 7l-2 1m2-1l-2-1m2 1v2.5M14 4l-2-1-2 1M4 7l2-1M4 7l2 1M4 7v2.5M12 21l-2-1m2 1l2-1m-2 1v-2.5M6 18l-2-1v-2.5M18 18l2-1v-2.5"/>
          </svg>
          Fan {fanOn ? 'On' : 'Off'}
        </button>
        <button
          class="flex items-center justify-center gap-2 px-5 py-4 rounded-[var(--radius-md)] border-2 font-semibold transition-all duration-150 cursor-pointer
          {boostOn
            ? 'bg-[var(--color-accent)] border-[var(--color-accent)] text-white shadow-md'
            : 'border-[var(--border-color)] text-[var(--text-secondary)] hover:border-[var(--color-accent)] hover:text-[var(--color-accent)]'}"
          aria-pressed={boostOn}
          onclick={toggleBoost}
        >
          <svg class="w-5 h-5" fill="none" stroke="currentColor" viewBox="0 0 24 24">
            <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M13 10V3L4 14h7v7l9-11h-7z"/>
          </svg>
          Boost {boostOn ? 'On' : 'Off'}
        </button>
      </div>
    </div>
  </div>

  <div class="bg-[var(--bg-secondary)] rounded-[var(--radius-lg)] border border-[var(--border-color)] shadow-sm overflow-hidden">
    <div class="px-6 py-4 border-b border-[var(--border-color)]">
      <h2 class="text-lg font-semibold text-[var(--text-primary)]">Legionella Prevention</h2>
      <p class="text-sm text-[var(--text-muted)] mt-1">Weekly thermal disinfection settings</p>
    </div>
    <div class="p-6 space-y-4">
      <div class="flex flex-col sm:flex-row sm:items-center justify-between gap-4">
        <div class="flex items-center gap-3">
          <button
            class="relative inline-flex h-7 w-12 items-center rounded-full transition-colors cursor-pointer
            {legionellaOn ? 'bg-[var(--color-accent)]' : 'bg-[var(--bg-tertiary)]'}"
            onclick={toggleLegionella}
            role="switch"
            aria-checked={legionellaOn}
            aria-label="Toggle legionella prevention"
          >
            <span
              class="inline-block h-5 w-5 transform rounded-full bg-white transition-transform shadow-sm
              {legionellaOn ? 'translate-x-6' : 'translate-x-1'}"
            ></span>
          </button>
          <span class="font-medium text-[var(--text-primary)]">
            {legionellaOn ? 'Enabled' : 'Disabled'}
          </span>
        </div>
        <div class="flex items-center gap-3">
          <span class="text-sm text-[var(--text-secondary)]">Weekly hour:</span>
          <div class="flex items-center gap-2">
            <button
              class="w-9 h-9 rounded-[var(--radius-md)] border border-[var(--border-color)] bg-[var(--bg-tertiary)] hover:bg-[var(--bg-hover)] flex items-center justify-center text-[var(--text-primary)] cursor-pointer disabled:opacity-50"
              onclick={() => legionellaHour > 0 && setLegionellaHour(legionellaHour - 1)}
              disabled={legionellaHour <= 0}
            >
              -
            </button>
            <span class="w-12 text-center font-semibold text-[var(--text-primary)]">{legionellaHour}:00</span>
            <button
              class="w-9 h-9 rounded-[var(--radius-md)] border border-[var(--border-color)] bg-[var(--bg-tertiary)] hover:bg-[var(--bg-hover)] flex items-center justify-center text-[var(--text-primary)] cursor-pointer disabled:opacity-50"
              onclick={() => legionellaHour < 23 && setLegionellaHour(legionellaHour + 1)}
              disabled={legionellaHour >= 23}
            >
              +
            </button>
          </div>
        </div>
      </div>
      <div class="flex items-center justify-between gap-4">
        <div class="flex items-center gap-3">
          <span class="text-sm text-[var(--text-secondary)]">Target temp:</span>
          <div class="flex items-center gap-2">
            <button
              class="w-9 h-9 rounded-[var(--radius-md)] border border-[var(--border-color)] bg-[var(--bg-tertiary)] hover:bg-[var(--bg-hover)] flex items-center justify-center text-[var(--text-primary)] cursor-pointer disabled:opacity-50"
              onclick={() => legionellaTargetTemp > 50 && setLegionellaTemp(legionellaTargetTemp - 1)}
              disabled={legionellaTargetTemp <= 50}
            >
              -
            </button>
            <span class="w-12 text-center font-semibold text-[var(--text-primary)]">{legionellaTargetTemp}°C</span>
            <button
              class="w-9 h-9 rounded-[var(--radius-md)] border border-[var(--border-color)] bg-[var(--bg-tertiary)] hover:bg-[var(--bg-hover)] flex items-center justify-center text-[var(--text-primary)] cursor-pointer disabled:opacity-50"
              onclick={() => legionellaTargetTemp < 80 && setLegionellaTemp(legionellaTargetTemp + 1)}
              disabled={legionellaTargetTemp >= 80}
            >
              +
            </button>
          </div>
        </div>
        <div class="text-sm text-[var(--text-muted)]">
          Current: {legionellaTemp.toFixed(1)}°C
        </div>
      </div>
    </div>
  </div>

  <div class="bg-[var(--bg-secondary)] rounded-[var(--radius-lg)] border border-[var(--border-color)] shadow-sm overflow-hidden">
    <div class="px-6 py-4 border-b border-[var(--border-color)]">
      <h2 class="text-lg font-semibold text-[var(--text-primary)]">Temperature Sensors</h2>
      <p class="text-sm text-[var(--text-muted)] mt-1">Current readings from all sensors</p>
    </div>
    <div class="p-6">
      <div class="grid grid-cols-2 sm:grid-cols-3 gap-4">
        <div class="text-center p-4 rounded-[var(--radius-md)] bg-[var(--bg-tertiary)]">
          <div class="text-sm text-[var(--text-muted)] mb-1">Top</div>
          <div class="text-2xl font-bold text-[var(--text-primary)]">{topTemp.toFixed(1)}°C</div>
        </div>
        <div class="text-center p-4 rounded-[var(--radius-md)] bg-[var(--bg-tertiary)]">
          <div class="text-sm text-[var(--text-muted)] mb-1">Bottom</div>
          <div class="text-2xl font-bold text-[var(--text-primary)]">{bottomTemp.toFixed(1)}°C</div>
        </div>
        <div class="text-center p-4 rounded-[var(--radius-md)] bg-[var(--bg-tertiary)]">
          <div class="text-sm text-[var(--text-muted)] mb-1">Power</div>
          <div class="text-2xl font-bold text-[var(--color-accent)]">{power}W</div>
        </div>
      </div>
    </div>
  </div>

  <Dialog
    bind:open={confirmBoost}
    type="confirm"
    message="Are you sure you want to enable Boost? This will require an additional 2000 watts of power. Please ensure your installation can handle this load."
    onok={confirmBoostOk}
    oncancel={confirmBoostCancel}
  />

  <Dialog
    bind:open={confirmLegionella}
    type="confirm"
    message="Are you sure you want to enable Legionella prevention? This will heat the tank to 60°C for disinfection. Make sure no hot water is being used during this time."
    onok={confirmLegionellaOk}
    oncancel={confirmLegionellaCancel}
  />
</div>
