export function apiUrl(path) {
  if (path.startsWith('http://') || path.startsWith('https://')) {
    return path;
  }
  const hostname = window.location.hostname;
  if (hostname === 'localhost' || hostname === '127.0.0.1') {
    return `http://192.168.2.88${path}`;
  }
  return path;
}

export async function apiGet(uri) {
  const url = apiUrl(uri);
  const res = await fetch(url, { headers: { Accept: 'application/json' } });
  if (!res.ok) {
    const text = await res.text();
    throw new Error(text || `Failed to get (${res.status})`);
  }
  const data = await res.json();
  return data;
}

export async function apiPost(uri, payload) {
  const url = apiUrl(uri);
  const res = await fetch(url, {
    method: 'POST',
    headers: { 'Content-Type': 'application/json', Accept: 'application/json' },
    body: JSON.stringify(payload),
  });
  if (!res.ok) {
    const text = await res.text();
    throw new Error(text || `Failed to save (${res.status})`);
  }
}
