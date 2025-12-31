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
    const { dataBottom, dataTop } = await getData();
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
              unit: 'minute',
              displayFormats: {
                minute: 'HH:mm:ss',
                second: 'HH:mm:ss',
              },
              tooltipFormat: 'HH:mm:ss',
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

    const maxPoints = 1500;
    const allTimestamps = [...new Set([...topEntries.map(e => e.t), ...bottomEntries.map(e => e.t)].sort((a, b) => a - b))];
    const sampledTimestamps = downsampleTimestamps(allTimestamps, maxPoints);

    const dataBottom = alignToTimestamps(bottomEntries, sampledTimestamps);
    const dataTop = alignToTimestamps(topEntries, sampledTimestamps);

    return { dataBottom, dataTop };
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
