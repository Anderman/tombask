<!-- svelte-ignore a11y_label_has_associated_control -->
<script>
  import { onMount } from 'svelte';
  import { apiGet, apiPost } from '../api.js';
  import FormInput from '../components/FormInput.svelte';

  let form = {};

  async function load() {
    const data = await apiGet('/api/config');
    form = {
      deviceId: data.deviceId ?? 1,
      mqttHost: data.mqttHost ?? '',
      mqttPort: data.mqttPort ?? 1883,
      mqttUser: data.mqttUser ?? '',
      mqttPass: '',
    };
  }

  function handleSubmit(e) {
    e.preventDefault();
    const payload = {
      deviceId: Number(form.deviceId),
      mqttHost: form.mqttHost,
      mqttPort: Number(form.mqttPort),
      mqttUser: form.mqttUser,
      mqttPass: form.mqttPass,
    };

    apiPost('/api/config', payload);
    form.mqttPass = '';
  }

  onMount(load);
</script>

<form class="space-y-4 w-full max-w-sm mx-auto px-2 sm:px-0" on:submit={handleSubmit}>
  <FormInput label="Device ID" type="number" bind:bindValue={form.deviceId} min="1" required autocomplete="off" />
  <FormInput label="MQTT Host" type="text" bind:bindValue={form.mqttHost} required />
  <FormInput label="MQTT Port" type="number" bind:bindValue={form.mqttPort} min="1" max="65535" required autocomplete="off" />
  <FormInput label="MQTT User" type="text" bind:bindValue={form.mqttUser} autocomplete="username" />
  <FormInput label="MQTT Password" type="password" bind:bindValue={form.mqttPass} autocomplete="current-password" />
  <button type="submit" class="bg-blue-700 text-white px-4 py-3 rounded hover:bg-blue-800 transition w-full text-lg">Save</button>
</form>
