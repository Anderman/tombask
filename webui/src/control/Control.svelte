<script>
  import { apiGet, apiPost } from '../api';
  import Thermostat from './Thermostat.svelte';
  import { onMount } from 'svelte';
  import ConfirmDialog from '../components/ConfirmDialog.svelte';
  // State en logica nu hier
    let confirmBoost = false;
  let legionellaTemp = 0;
  let setPointTemp = 0;
  let topTemp = 0;
  let bottomTemp = 0;
  let power = 0;
  let fanOn = false;
  let boostOn = false;
  let paused = false;
  
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
      // direct uitzetten
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

<section class="w-full flex flex-col items-center">
  <Thermostat topTemp={topTemp} setPointTemp={setPointTemp} power={power} on:setTarget={(e) => setTargetTemp(e.detail)} />
  <div class="flex gap-4 mt-6">
    <button
      class="px-5 py-2 rounded-full border-2 font-semibold transition"
      style="background: {fanOn ? 'var(--boost-color)' : 'transparent'}; color: {fanOn ? 'white' : 'var(--boost-color)'};border-color: var(--boost-color);"
      aria-pressed={fanOn}
      on:click={toggleFan}
    >
      {fanOn ? 'Fan On' : 'Fan Off'}
    </button>
    <button
      class="px-5 py-2 rounded-full border-2 font-semibold transition"
      style="background: {boostOn ? 'var(--boost-color)' : 'transparent'}; color: {boostOn ? 'white' : 'var(--boost-color)'}; border-color: var(--boost-color);"
      aria-pressed={boostOn}
      on:click={toggleBoost}
    >
      {boostOn ? 'Boost On' : 'Boost Off'}
    </button>
    <ConfirmDialog
      open={confirmBoost}
      message="Are you sure you want to enable Boost? This will require an additional 2000 watts of power. Please ensure your installation can handle this load."
      on:ok={confirmBoostOk}
      on:cancel={confirmBoostCancel}
    />
  </div>
</section>
