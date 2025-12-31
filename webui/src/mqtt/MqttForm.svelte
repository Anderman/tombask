<script>
  import { onMount } from 'svelte';
  import { apiGet, apiPost } from '../api.js';
  import FormInput from '../components/FormInput.svelte';

  let form = $state({
    deviceId: 1,
    mqttHost: '',
    mqttPort: 1883,
    mqttUser: '',
    mqttPass: '',
  });
  let saving = $state(false);
  let saved = $state(false);
  let error = $state('');

  async function load() {
    try {
      const data = await apiGet('/api/config');
      form.deviceId = data.deviceId ?? 1;
      form.mqttHost = data.mqttHost ?? '';
      form.mqttPort = data.mqttPort ?? 1883;
      form.mqttUser = data.mqttUser ?? '';
      form.mqttPass = '';
    } catch (e) {
      error = e.message;
    }
  }

  async function handleSubmit(e) {
    e.preventDefault();
    saving = true;
    saved = false;
    error = '';
    try {
      const payload = {
        deviceId: Number(form.deviceId),
        mqttHost: form.mqttHost,
        mqttPort: Number(form.mqttPort),
        mqttUser: form.mqttUser,
        mqttPass: form.mqttPass,
      };
      await apiPost('/api/config', payload);
      form.mqttPass = '';
      saved = true;
      setTimeout(() => saved = false, 3000);
    } catch (e) {
      error = e.message;
    } finally {
      saving = false;
    }
  }

  onMount(load);
</script>

<div class="max-w-xl mx-auto">
  <div class="bg-[var(--bg-secondary)] rounded-[var(--radius-lg)] border border-[var(--border-color)] shadow-sm overflow-hidden">
    <div class="px-6 py-4 border-b border-[var(--border-color)]">
      <h2 class="text-lg font-semibold text-[var(--text-primary)]">MQTT Configuration</h2>
      <p class="text-sm text-[var(--text-muted)] mt-1">Configure your MQTT broker settings</p>
    </div>
    <div class="p-6">
      <form class="space-y-5" onsubmit={handleSubmit}>
        <FormInput label="Device ID" type="number" bind:value={form.deviceId} min="1" required autocomplete="off" id="deviceId" />
        <FormInput label="MQTT Host" type="text" bind:value={form.mqttHost} required placeholder="e.g., mqtt.example.com" id="mqttHost" />
        <FormInput label="MQTT Port" type="number" bind:value={form.mqttPort} min="1" max="65535" required autocomplete="off" id="mqttPort" />
        <FormInput label="MQTT User" type="text" bind:value={form.mqttUser} autocomplete="username" id="mqttUser" />
        <FormInput label="MQTT Password" type="password" bind:value={form.mqttPass} autocomplete="current-password" id="mqttPass" />
        <div class="pt-2">
          <button
            type="submit"
            class="w-full bg-[var(--color-primary)] text-white px-6 py-3 rounded-[var(--radius-md)] hover:bg-[var(--color-primary-hover)] transition font-medium shadow-sm disabled:opacity-50 disabled:cursor-not-allowed flex items-center justify-center gap-2 cursor-pointer"
            disabled={saving}
          >
            {#if saving}
              <svg class="animate-spin h-5 w-5" fill="none" viewBox="0 0 24 24">
                <circle class="opacity-25" cx="12" cy="12" r="10" stroke="currentColor" stroke-width="4"></circle>
                <path class="opacity-75" fill="currentColor" d="M4 12a8 8 0 018-8V0C5.373 0 0 5.373 0 12h4zm2 5.291A7.962 7.962 0 014 12H0c0 3.042 1.135 5.824 3 7.938l3-2.647z"></path>
              </svg>
              Saving...
            {:else}
              Save Configuration
            {/if}
          </button>
        </div>
        {#if saved}
          <div class="text-center text-[var(--color-success)] text-sm font-medium">Configuration saved successfully</div>
        {/if}
        {#if error}
          <div class="text-center text-[var(--color-danger)] text-sm font-medium">{error}</div>
        {/if}
      </form>
    </div>
  </div>
</div>
