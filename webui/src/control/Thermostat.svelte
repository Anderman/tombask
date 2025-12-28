<!-- svelte-ignore a11y_click_events_have_key_events -->
<!-- svelte-ignore a11y_no_static_element_interactions -->
<script>
  export let power;
  export let topTemp;
  export let setPointTemp;
  export let min = 40;
  export let max = 70;
  export let step = 1;
  import { createEventDispatcher } from 'svelte';
  const dispatch = createEventDispatcher();

  const r = 120;
  const cx = 160;
  const cy = 160;
  const arcLen = 270;
  const arcStart = 225;
  const arcEnd = arcStart + arcLen;
  const circumference = 2 * Math.PI * r * (arcLen / 360);

  function describeArc(cx, cy, r, startAngle, endAngle) {
    const start = polarToCartesian(cx, cy, r, startAngle);
    const end = polarToCartesian(cx, cy, r, endAngle);
    const largeArcFlag = endAngle - startAngle <= 180 ? '0' : '1';
    return ['M', start.x, start.y, 'A', r, r, 0, largeArcFlag, 1, end.x, end.y].join(' ');
  }
  function polarToCartesian(cx, cy, r, angle) {
    const rad = ((angle - 90) * Math.PI) / 180.0;
    return {
      x: cx + r * Math.cos(rad),
      y: cy + r * Math.sin(rad),
    };
  }
  $: progressSetpoint = (setPointTemp - min) / (max - min);
  $: progress = (topTemp - min) / (max - min);
  $: offset = circumference * progress;
  $: handleAngle = arcStart + arcLen * progressSetpoint;
  $: handle = polarToCartesian(cx, cy, r, handleAngle);

  function setTempFromClick(e) {
    const svg = e.currentTarget;
    const rect = svg.getBoundingClientRect();
    const x = e.clientX - rect.left;
    const y = e.clientY - rect.top;
    const vb = svg.viewBox.baseVal;
    const scaleX = vb.width / rect.width;
    const scaleY = vb.height / rect.height;
    const sx = x * scaleX;
    const sy = y * scaleY;
    const dx = sx - cx;
    const dy = sy - cy;
    let angle = (Math.atan2(dy, dx) * 180) / Math.PI + 90;
    if (angle < 0) angle += 360;
    let rel = (angle - arcStart + 360) % 360;
    if (rel > arcLen) return;
    const pct = rel / arcLen;
    const val = min + pct * (max - min);
    dispatch('setTarget', Math.round(val));
  }
  function handlePlus() {
    if (setPointTemp + step <= max) dispatch('setTarget', Math.round((setPointTemp + step) * 2) / 2);
  }
  function handleMin() {
    if (setPointTemp - step >= min) dispatch('setTarget', Math.round((setPointTemp - step) * 2) / 2);
  }
</script>

<div class="flex flex-col items-center my-6">
  <svg
    id="slider"
    viewBox="0 0 320 320"
    class="w-72 h-72 select-none outline-none focus:outline-none focus:ring-0"
    tabindex="-1"
    on:click={setTempFromClick}
    style="touch-action: none; user-select: none; cursor: pointer;"
  >
    <g id="container" transform="translate(0 0)">
      <path d={describeArc(cx, cy, r, arcStart, arcEnd)} stroke="#bdbdbd" stroke-width="18" stroke-opacity="0.3" fill="none"></path>
      <path
        d={describeArc(cx, cy, r, arcStart, arcEnd)}
        stroke="var(--boost-color)"
        stroke-width="18"
        fill="none"
        stroke-linecap="round"
        stroke-dasharray={circumference}
        stroke-dashoffset={circumference - offset}
      ></path>
      <circle cx={handle.x} cy={handle.y} r="14" fill="#2563eb" stroke="#fff" stroke-width="4" style="filter: drop-shadow(0 2px 6px #2563eb44);"></circle>
      <text x="160" y="155" text-anchor="middle" font-size="3.5rem" fill="var(--boost-color)" font-weight="bold">{topTemp}<tspan font-size="1.2rem" dy="-1.2rem">°C</tspan></text>
      <text x="160" y="190" text-anchor="middle" font-size="1.2rem" fill="var(--text2)">{setPointTemp}<tspan font-size="0.5rem" dy="-0.5rem">°C</tspan></text>
      <text x="160" y="230" text-anchor="middle" font-size="1.2rem" fill="#888">{power}W</text>
    </g>
  </svg>
  <div class="relative z-10 flex flex-col items-center -mt-22"></div>
  <div class="flex gap-6 mt-4">
    <button
      class="w-10 h-10 rounded-full border-2 bg-transparent hover:bg-blue-50 transition shadow-none text-2xl font-bold pb-2"
      style="border-color: var(--text2); color: var(--text2);"
      on:click={handleMin}>-</button
    >
    <button
      class="w-10 h-10 rounded-full border-2 bg-transparent hover:bg-blue-50 transition shadow-none text-2xl font-bold pb-2"
      style="border-color: var(--text2); color: var(--text2);"
      on:click={handlePlus}>+</button
    >
  </div>
</div>
