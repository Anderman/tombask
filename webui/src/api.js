export function apiUrl(path) {
  if (path.startsWith('http://') || path.startsWith('https://')) {
    return path;
  }
  if (import.meta.env && import.meta.env.DEV) {
    return `http://192.168.2.88${path}`;
  }
  return path;
}

export async function apiGet(uri) {
  const res = await fetch(apiUrl(uri), { headers: { Accept: 'application/json' } });
  if (!res.ok) {
    const text = await res.text();
    throw new Error(text || `Failed to get (${res.status})`);
  }
  const data = await res.json();
  return data;
}

export async function apiPost(uri, payload) {
  const res = await fetch(apiUrl(uri), {
    method: 'POST',
    headers: { 'Content-Type': 'application/json', Accept: 'application/json' },
    body: JSON.stringify(payload),
  });
  if (!res.ok) {
    const text = await res.text();
    throw new Error(text || `Failed to save (${res.status})`);
  }
}
