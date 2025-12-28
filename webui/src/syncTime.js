import { apiPost, apiGet } from './api.js';

// Haal tijd van publieke tijdserver en stuur naar ESP32
export async function syncTimeZone() {
  const timezone = Intl.DateTimeFormat().resolvedOptions().timeZone;
  // Stuur naar ESP32 via api.js
  await apiPost('/api/settimezone', { timezone });
}
