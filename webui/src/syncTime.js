import { apiPost, apiGet } from './api.js';

export async function syncTimeZone() {
  try {
    const timezone = Intl.DateTimeFormat().resolvedOptions().timeZone;
    await apiPost('/api/settimezone', { timezone });
  } catch (e) {
    console.warn('Time sync failed:', e.message);
  }
}
