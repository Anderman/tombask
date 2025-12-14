# Tombask Web UI

Minimal Svelte (Vite) web UI to edit device config on the ESP32.

## Dev

From `webui/`:
- `pnpm install`
- `pnpm run dev`

## Build

- `pnpm run build`

Output goes to `webui/dist/` with stable filenames:
- `dist/index.html`
- `dist/assets/app.js`
- `dist/assets/app.css`

## Expected device API

- `GET /api/config` -> JSON:
  ```json
  {
    "deviceId": "tombask-kitchen",
    "mqttHost": "192.168.2.4",
    "mqttPort": 1883,
    "wifiSsid": "MyWiFi",
    "wifiPassword": ""
  }
  ```
- `POST /api/config` (JSON) -> returns 200 OK (optionally JSON).

Recommended: device does not return the stored wifi password; only updates it when a non-empty value is posted.
