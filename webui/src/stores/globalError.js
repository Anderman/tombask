import { writable } from 'svelte/store';

export const globalError = writable('');

export function setGlobalError(err) {
  if (err && err.stack) {
    // Print stacktrace naar console
    console.error('Global error stack:', err.stack);
    globalError.set((err.message ? err.message + '\n' : '') + err.stack);
  } else {
    globalError.set(typeof err === 'string' ? err : (err?.message ?? String(err)));
  }
}

export function clearGlobalError() {
  globalError.set('');
}
