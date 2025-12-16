import { defineConfig } from 'vite';
import { svelte } from '@sveltejs/vite-plugin-svelte';

// Build output tuned for embedded serving:
// - stable filenames (no hashes)
// - everything in dist/
export default defineConfig({
  plugins: [svelte()],
  build: {
    outDir: 'dist',
    assetsDir: 'assets',
    // Debug-friendly build (keep source maps, avoid minifier surprises)
    sourcemap: true,
    minify: true,
    rollupOptions: {
      output: {
        entryFileNames: 'assets/app.js',
        chunkFileNames: 'assets/chunk.js',
        assetFileNames: (assetInfo) => {
          if (assetInfo.name && assetInfo.name.endsWith('.css')) return 'assets/app.css';
          return 'assets/[name][extname]';
        }
      }
    }
  }
});
