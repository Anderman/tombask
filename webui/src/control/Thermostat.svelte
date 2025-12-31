<script>
  let {
    power,
    topTemp,
    setPointTemp,
    min = 40,
    max = 70,
    step = 1,
    onsetTarget = () => {}
  } = $props();

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

  let progressSetpoint = $derived((setPointTemp - min) / (max - min));
  let progress = $derived((topTemp - min) / (max - min));
  let offset = $derived(circumference * progress);
  let handleAngle = $derived(arcStart + arcLen * progressSetpoint);
  let handle = $derived(polarToCartesian(cx, cy, r, handleAngle));

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
    onsetTarget(Math.round(val));
  }

  function handlePlus() {
    if (setPointTemp + step <= max) onsetTarget(Math.round((setPointTemp + step) * 2) / 2);
  }

  function handleMin() {
    if (setPointTemp - step >= min) onsetTarget(Math.round((setPointTemp - step) * 2) / 2);
  }
</script>

<div class="flex flex-col items-center py-4">
  <!-- svelte-ignore a11y_click_events_have_key_events -->
  <!-- svelte-ignore a11y_no_static_element_interactions -->
  <svg
    id="slider"
    viewBox="0 0 320 320"
    class="w-72 h-72 select-none outline-none focus:outline-none focus:ring-0 cursor-pointer touch-none"
    tabindex="-1"
    onclick={setTempFromClick}
    role="slider"
    aria-valuenow={topTemp}
    aria-valuemin={min}
    aria-valuemax={max}
  >
    <g id="container">
      <path d={describeArc(cx, cy, r, arcStart, arcEnd)} stroke="#e2e8f0" stroke-width="20" fill="none" class="dark:stroke-slate-600"></path>
      <path
        d={describeArc(cx, cy, r, arcStart, arcEnd)}
        stroke="var(--color-accent)"
        stroke-width="20"
        fill="none"
        stroke-linecap="round"
        stroke-dasharray={circumference}
        stroke-dashoffset={circumference - offset}
        class="transition-all duration-300"
      ></path>
      <circle cx={handle.x} cy={handle.y} r="16" fill="#3b82f6" stroke="#ffffff" stroke-width="4" class="drop-shadow-lg"></circle>
      <text x="160" y="150" text-anchor="middle" font-size="3.5rem" fill="var(--color-accent)" font-weight-bold>{topTemp.toFixed(1)}<tspan font-size="1.2rem" dy="-1.2rem" fill="var(--text-secondary)">°C</tspan></text>
      <text x="160" y="190" text-anchor="middle" font-size="1.2rem" fill="var(--text-secondary)">Set: {setPointTemp}<tspan font-size="0.5rem" dy="-0.5rem">°C</tspan></text>
      <text x="160" y="230" text-anchor="middle" font-size="1.1rem" fill="var(--text-muted)">{power}W</text>
    </g>
  </svg>

  <div class="flex gap-8 mt-6">
    <button
      class="w-12 h-12 rounded-full border-2 bg-[var(--bg-tertiary)] hover:bg-[var(--bg-hover)] transition flex items-center justify-center text-2xl font-bold text-[var(--text-secondary)] hover:text-[var(--color-primary)] hover:border-[var(--color-primary)] cursor-pointer"
      onclick={handleMin}>-</button
    >
    <button
      class="w-12 h-12 rounded-full border-2 bg-[var(--bg-tertiary)] hover:bg-[var(--bg-hover)] transition flex items-center justify-center text-2xl font-bold text-[var(--text-secondary)] hover:text-[var(--color-primary)] hover:border-[var(--color-primary)] cursor-pointer"
      onclick={handlePlus}>+</button
    >
  </div>
</div>
