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
            label: 'Bottom temperatuur (°C)',
            data: dataBottom,
            borderColor: 'rgb(75, 192, 192)',
            pointRadius: 0,
            stepped: 'before',
          },
          {
            label: 'Top temperatuur (°C)',
            data: dataTop,
            borderColor: 'rgb(255, 99, 132)',
            pointRadius: 0,
            stepped: 'before',
          },
        ],
      },
      options: {
        maintainAspectRatio: false,
        animation: true,
        plugins: {
          legend: { display: true },
          tooltip: {
            intersect: false,
            mode: 'index',
            // geen custom callbacks, Chart.js regelt de tijdstip-formattering
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
            title: { display: true, text: 'Graden Celsius' },
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
            title: { display: true, text: 'Tijd' },
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

    const { minEpoch, maxEpoch } = getBoundary(topEntries, bottomEntries);
    return enrichTimeSeriesData(bottomEntries, topEntries, maxEpoch, minEpoch);
  }
  function enrichTimeSeriesData(bottomEntries, topEntries, maxEpoch, minEpoch) {
    const bottomMap = new Map(bottomEntries.map((e) => [e.t, e.v]));
    const topMap = new Map(topEntries.map((e) => [e.t, e.v]));
    const allTimestamps = Array.from({ length: Math.floor((maxEpoch - minEpoch) / 1) + 1 }, (_, i) => minEpoch + i * 1);

    const dataBottom = fillSerie(bottomMap, allTimestamps);
    const dataTop = fillSerie(topMap, allTimestamps);

    return { dataBottom, dataTop };
  }

  function fillSerie(map, timestamps) {
    let last = null;
    return timestamps.map((t) => {
      if (map.has(t)) last = map.get(t);
      return { x: new Date(t * 1000), y: last };
    });
  }
  function getBoundary(topEntries, bottomEntries) {
    let allEntries = [...topEntries, ...bottomEntries];
    if (!allEntries.length) return { dataBottom: [], dataTop: [] };
    let minEpoch = Math.min(...allEntries.map((e) => e.t));
    let maxEpoch = Math.max(...allEntries.map((e) => e.t));
    return { minEpoch, maxEpoch };
  }
</script>

<div style="width:calc(100vw - 16px);height:calc(100vh - 96px);">
  <canvas bind:this={chartEl}></canvas>
</div>
