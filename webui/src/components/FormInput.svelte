<!-- svelte-ignore a11y_label_has_associated_control -->
<script>
  export let label = '';
  export let type = 'text';
  export let bindValue;
  export let autocomplete = '';
  export let min;
  export let max;
  export let required = false;

  // Forward all other attributes
  export let inputProps = {};
</script>

  <div class="flex flex-col sm:flex-row items-stretch sm:items-center justify-between gap-1 sm:gap-2">
    <label class="font-medium mb-1 sm:mb-0 sm:w-40 text-left">{label}</label>
    <input class="border rounded px-3 py-3 text-base flex-1" type={type} bind:value={bindValue} autocomplete={autocomplete} min={min} max={max} required={required} {...inputProps} />
  </div>
