import fs from "node:fs/promises";
import path from "node:path";
import { fileURLToPath } from "node:url";

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

const webuiDir = path.resolve(__dirname, "..");
const dataDir = path.resolve(webuiDir, "..", "data");
const buildInfoPath = path.resolve(webuiDir, "..", ".build", "build-info.txt");

async function exists(p) {
  try {
    await fs.access(p);
    return true;
  } catch {
    return false;
  }
}

if (!(await exists(dataDir))) {
  console.error(`ERROR: data folder not found: ${dataDir}`);
  console.error("Run 'pnpm build:dist' or 'pnpm build' first.");
  process.exit(1);
}

if (!(await exists(buildInfoPath))) {
  console.error(`ERROR: build info not found: ${buildInfoPath}`);
  process.exit(1);
}

const versionFilePath = path.join(dataDir, "version.txt");
await fs.copyFile(buildInfoPath, versionFilePath);

console.log(`Copied ${buildInfoPath} -> ${versionFilePath}`);
