<!-- svelte-ignore a11y_click_events_have_key_events -->
  <!-- svelte-ignore a11y_no_static_element_interactions -->
<script>
  export let currentTemp = 20;
  export let targetTemp = 21;
  export let min = 10;
  export let max = 30;
  export let step = 0.5;
  import { createEventDispatcher } from 'svelte';
  const dispatch = createEventDispatcher();

  // Arc/circle parameters
  const r = 120;
  const cx = 160;
  const cy = 160;
  const arcLen = 270; // 270° arc (like HA)
  const arcStart = 225; // Start at 135° (bottom left)
  const arcEnd = arcStart + arcLen; // End at 45° (bottom right)
  const circumference = 2 * Math.PI * r * (arcLen / 360);

  // SVG arc path (fixed)
  function describeArc(cx, cy, r, startAngle, endAngle) {
    const start = polarToCartesian(cx, cy, r, startAngle);
    const end = polarToCartesian(cx, cy, r, endAngle);
    const largeArcFlag = endAngle - startAngle <= 180 ? "0" : "1";
    return [
      "M", start.x, start.y,
      "A", r, r, 0, largeArcFlag, 1, end.x, end.y
    ].join(" ");
  }
  function polarToCartesian(cx, cy, r, angle) {
    const rad = (angle - 90) * Math.PI / 180.0;
    return {
      x: cx + r * Math.cos(rad),
      y: cy + r * Math.sin(rad)
    };
  }
  // Progress (0..1)
  $: progress = (targetTemp - min) / (max - min);
  $: dash = circumference * progress;
  // Fill should start at arcStart (bottom) and run clockwise
  $: offset = circumference * (progress);
  // Handle position
  $: handleAngle = arcStart + arcLen * progress;
  $: handle = polarToCartesian(cx, cy, r, handleAngle);

  function setTempFromClick(e) {
    const rect = e.target.getBoundingClientRect();
    const x = e.clientX - rect.left;
    const y = e.clientY - rect.top;
    // Fix: svg can be scaled, so correct for viewBox
    const svg = e.target.closest('svg');
    const vb = svg.viewBox.baseVal;
    const scaleX = vb.width / rect.width;
    const scaleY = vb.height / rect.height;
    const sx = x * scaleX;
    const sy = y * scaleY;
    const dx = sx - cx;
    const dy = sy - cy;
    let angle = Math.atan2(dy, dx) * 180 / Math.PI + 90;
    if (angle < 0) angle += 360;
    let rel = (angle - arcStart + 360) % 360;
    if (rel > arcLen) return;
    const pct = rel / arcLen;
    const val = min + pct * (max - min);
    dispatch('setTarget', Math.round(val * 2) / 2);
  }
  function handlePlus() {
    if (targetTemp + step <= max) dispatch('setTarget', Math.round((targetTemp + step) * 2) / 2);
  }
  function handleMin() {
    if (targetTemp - step >= min) dispatch('setTarget', Math.round((targetTemp - step) * 2) / 2);
  }
</script>

<div class="flex flex-col items-center my-6">
  <svg
    id="slider"
    viewBox="0 0 320 320"
    class="w-72 h-72 select-none outline-none focus:outline-none focus:ring-0 cursor-pointer touch-none"
    tabindex="-1"
    onclick={setTempFromClick}
    style="touch-action: none; user-select: none;"
  >
    <g id="container" transform="translate(0 0)">
      <!-- Arc background -->
      <path d={describeArc(cx, cy, r, arcStart, arcEnd)} stroke="#bdbdbd" stroke-opacity="0.3" stroke-width="18" fill="none" />
      <!-- Arc progress -->
      <path
        d={describeArc(cx, cy, r, arcStart, arcEnd)}
        stroke="var(--color-accent)"
        stroke-width="18"
        fill="none"
        stroke-linecap="round"
        stroke-dasharray={circumference}
        stroke-dashoffset={circumference-offset}
      />
      <!-- Handle -->
      <circle
        cx={handle.x}
        cy={handle.y}
        r="14"
        fill="var(--color-primary)"
        stroke="#fff"
        stroke-width="4"
        style="filter: drop-shadow(0 2px 6px #2563eb44);"
      />
      <!-- Current temp large in the center -->
      <text x="160" y="155" text-anchor="middle" font-size="3.5rem" fill="var(--color-primary)" font-weight="bold">{currentTemp}°C</text>
      <!-- Target temp smaller below -->
      <text x="160" y="190" text-anchor="middle" font-size="1.2rem" fill="var(--text-muted)">Target: {targetTemp}°C</text>
    </g>
  </svg>
  <div class="relative z-10 flex flex-col items-center -mt-24">
    <!-- Remove this old display -->
    <!-- <div class="text-5xl font-extrabold text-blue-900">{targetTemp}°C</div>
    <div class="text-base text-blue-700 mt-1">Streefwaarde</div> -->
  </div>
  <div class="flex gap-6 mt-4">
    <button class="w-10 h-10 rounded-full border-2 font-bold text-2xl bg-transparent transition shadow-none cursor-pointer" style="border-color: var(--color-primary); color: var(--color-primary);" onclick={handleMin}>-</button>
    <button class="w-10 h-10 rounded-full border-2 font-bold text-2xl bg-transparent transition shadow-none cursor-pointer" style="border-color: var(--color-primary); color: var(--color-primary);" onclick={handlePlus}>+</button>
  </div>
</div>

