<script>
  import zoomPlugin from 'chartjs-plugin-zoom';
  import { onMount, onDestroy } from 'svelte';
  import Chart from 'chart.js/auto';
  import 'chartjs-adapter-date-fns';
  import { apiGet } from './api.js';

  let chart;
  let chartEl;
  let loading = true;
  let error = '';

  onMount(() => {
    fetchAndRender();
  });
  async function fetchAndRender() {
    loading = true;
    error = '';
    const { dataBottom, dataTop, sampledTimestamps } = await getData();

    const hasRange = sampledTimestamps?.length >= 2;
    const minT = hasRange ? sampledTimestamps[0] : null;
    const maxT = hasRange ? sampledTimestamps[sampledTimestamps.length - 1] : null;
    const spansMultipleDays =
      hasRange &&
      new Date(minT * 1000).toDateString() !== new Date(maxT * 1000).toDateString();

    const tickTimeFmt = spansMultipleDays ? 'dd-MM HH:mm' : 'HH:mm:ss';
    const tooltipFmt = spansMultipleDays ? 'dd-MM-yyyy HH:mm:ss' : 'HH:mm:ss';

    if (chart) chart.destroy();
    Chart.register(zoomPlugin);
    chart = new Chart(chartEl, {
      type: 'line',
      data: {
        datasets: [
          {
            label: 'Bottom temperature (°C)',
            data: dataBottom,
            borderColor: 'rgb(75, 192, 192)',
            pointRadius: 0,
            stepped: 'before',
          },
          {
            label: 'Top temperature (°C)',
            data: dataTop,
            borderColor: 'rgb(255, 99, 132)',
            pointRadius: 0,
            stepped: 'before',
          },
        ],
      },
      options: {
        maintainAspectRatio: false,
        animation: false,
        plugins: {
          legend: { display: true },
          tooltip: {
            intersect: false,
            mode: 'index',
          },
          zoom: {
            pan: {
              enabled: true,
              mode: 'x',
            },
            zoom: {
              wheel: { enabled: true, modifierKey: 'ctrl' },
              pinch: { enabled: true },
              mode: 'x',
            },
            limits: {
              x: { min: 'original', max: 'original' },
            },
          },
        },
        scales: {
          y: {
            min: 10,
            max: 70,
             title: { display: true, text: 'Celsius' },
          },
          x: {
            type: 'time',
            time: {
              minUnit: 'second',
              displayFormats: {
                second: tickTimeFmt,
                minute: tickTimeFmt,
                hour: spansMultipleDays ? 'dd-MM HH:mm' : 'HH:mm',
                day: 'dd-MM',
                month: 'MMM yyyy',
              },
              tooltipFormat: tooltipFmt,
            },
             title: { display: true, text: 'Time' },
            ticks: {
              autoSkip: true,
              maxTicksLimit: 10,
            },
          },
        },
      },
    });
  }
  async function getData() {
    const json = await apiGet('/api/templogs');
    let topEntries = (json.top || []).sort((a, b) => a.t - b.t);
    let bottomEntries = (json.bottom || []).sort((a, b) => a.t - b.t);

    // Align start: begin at the later of the two start times
    const topStart = topEntries.length ? topEntries[0].t : null;
    const bottomStart = bottomEntries.length ? bottomEntries[0].t : null;
    const commonStart =
      topStart != null && bottomStart != null ? Math.max(topStart, bottomStart) : null;

    if (commonStart != null) {
      topEntries = topEntries.filter(e => e.t >= commonStart);
      bottomEntries = bottomEntries.filter(e => e.t >= commonStart);
    }

    const maxPoints = 1500;
    let allTimestamps = [
      ...new Set(
        [...topEntries.map(e => e.t), ...bottomEntries.map(e => e.t)].sort((a, b) => a - b),
      ),
    ];

    if (commonStart != null) {
      allTimestamps = allTimestamps.filter(t => t >= commonStart);
    }

    const sampledTimestamps = downsampleTimestamps(allTimestamps, maxPoints);

    const dataBottom = alignToTimestamps(bottomEntries, sampledTimestamps);
    const dataTop = alignToTimestamps(topEntries, sampledTimestamps);

    return { dataBottom, dataTop, sampledTimestamps };
  }

  function downsampleTimestamps(timestamps, maxPoints) {
    if (timestamps.length <= maxPoints) return timestamps;

    const step = Math.ceil(timestamps.length / maxPoints);
    const sampled = [];
    for (let i = 0; i < timestamps.length; i += step) {
      sampled.push(timestamps[i]);
    }
    return sampled;
  }

  function alignToTimestamps(entries, timestamps) {
    const entryMap = new Map(entries.map(e => [e.t, e.v]));
    let lastValue = null;
    return timestamps.map(t => {
      if (entryMap.has(t)) lastValue = entryMap.get(t);
      return { x: new Date(t * 1000), y: lastValue };
    });
  }
</script>

<div class="w-full h-[calc(100vh-6rem)]">
  <canvas bind:this={chartEl}></canvas>
</div>
