<script>
  let {
    label = '',
    type = 'text',
    value = $bindable(),
    autocomplete = '',
    min,
    max,
    required = false,
    placeholder = '',
    id = ''
  } = $props();
</script>

<div class="flex flex-col sm:flex-row items-stretch gap-2">
  <label class="font-medium text-[var(--text-secondary)] sm:w-40 flex items-center text-sm" for={id}>{label}</label>
  <input
    {id}
    class="flex-1 rounded-[var(--radius-md)] border border-[var(--border-color)] bg-[var(--bg-tertiary)] px-4 py-3 text-[var(--text-primary)] placeholder-[var(--text-muted)] focus:outline-none focus:ring-2 focus:ring-[var(--color-primary)] focus:border-transparent transition-all duration-150"
    {type}
    bind:value={value}
    {autocomplete}
    {min}
    {max}
    {required}
    {placeholder}
  />
</div>
