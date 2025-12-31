<script>
  import { apiGet, apiPost } from '../api';
  import Thermostat from './Thermostat.svelte';
  import { onMount } from 'svelte';
  import Dialog from '../components/Dialog.svelte';

  let confirmBoost = $state(false);
  let legionellaTemp = $state(0);
  let setPointTemp = $state(0);
  let topTemp = $state(0);
  let bottomTemp = $state(0);
  let power = $state(0);
  let fanOn = $state(false);
  let boostOn = $state(false);
  let paused = $state(false);

  onMount(() => {
    async function pollStatus() {
      if (!paused) {
        const data = await apiGet('/api/status');
        legionellaTemp = data.LegionellaTemp ?? legionellaTemp;
        setPointTemp = data.SetpointTemp ?? setPointTemp;
        topTemp = data.TopTemp ?? topTemp;
        bottomTemp = data.BottomTemp ?? bottomTemp;
        power = data.Power ?? power;
        fanOn = !!data.Fan;
        boostOn = !!data.Boost;
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
      <h2 class="text-lg font-semibold text-[var(--text-primary)]">Temperature Sensors</h2>
      <p class="text-sm text-[var(--text-muted)] mt-1">Current readings from all sensors</p>
    </div>
    <div class="p-6">
      <div class="grid grid-cols-2 sm:grid-cols-4 gap-4">
        <div class="text-center p-4 rounded-[var(--radius-md)] bg-[var(--bg-tertiary)]">
          <div class="text-sm text-[var(--text-muted)] mb-1">Top</div>
          <div class="text-2xl font-bold text-[var(--text-primary)]">{topTemp.toFixed(1)}°C</div>
        </div>
        <div class="text-center p-4 rounded-[var(--radius-md)] bg-[var(--bg-tertiary)]">
          <div class="text-sm text-[var(--text-muted)] mb-1">Bottom</div>
          <div class="text-2xl font-bold text-[var(--text-primary)]">{bottomTemp.toFixed(1)}°C</div>
        </div>
        <div class="text-center p-4 rounded-[var(--radius-md)] bg-[var(--bg-tertiary)]">
          <div class="text-sm text-[var(--text-muted)] mb-1">Legionella</div>
          <div class="text-2xl font-bold text-[var(--text-primary)]">{legionellaTemp.toFixed(1)}°C</div>
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
</div>
