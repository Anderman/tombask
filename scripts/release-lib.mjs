import fs from 'node:fs/promises';
import path from 'node:path';
import { fileURLToPath } from 'node:url';
import { spawn } from 'node:child_process';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

export const repoRoot = path.resolve(__dirname, '..');
export const buildDir = path.join(repoRoot, '.build');
export const buildInfoPath = path.join(buildDir, 'build-info.txt');
export const generatedHeaderPath = path.join(repoRoot, 'src', 'build_info.generated.h');
export const releaseRoot = path.join(repoRoot, 'release');

function pad(value) {
  return String(value).padStart(2, '0');
}

export function formatBuildId(date = new Date()) {
  return [
    date.getUTCFullYear(),
    pad(date.getUTCMonth() + 1),
    pad(date.getUTCDate()),
    'T',
    pad(date.getUTCHours()),
    pad(date.getUTCMinutes()),
    pad(date.getUTCSeconds()),
    'Z',
  ].join('');
}

export async function ensureDir(dirPath) {
  await fs.mkdir(dirPath, { recursive: true });
}

export async function writeBuildInfo(buildId) {
  await ensureDir(buildDir);

  await fs.writeFile(buildInfoPath, buildId, 'utf8');
  await fs.writeFile(generatedHeaderPath, ['#pragma once', '', `#define TOMBASK_BUILD_ID \"${buildId}\"`, ''].join('\n'), 'utf8');

  return { buildId };
}

export async function copyFileWithTimestamp(sourcePath, destinationDir, fileName) {
  await ensureDir(destinationDir);
  const destinationPath = path.join(destinationDir, fileName);
  await fs.copyFile(sourcePath, destinationPath);
  return destinationPath;
}

export async function pathExists(targetPath) {
  try {
    await fs.access(targetPath);
    return true;
  } catch {
    return false;
  }
}

export async function runCommand(command, args, options = {}) {
  const env = { ...process.env, ...(options.env ?? {}) };

  if (process.platform === 'win32') {
    delete env.MSYSTEM;
    delete env.MSYSTEM_PREFIX;
    delete env.MSYS;
    delete env.MSYS2_PATH_TYPE;
    delete env.CHERE_INVOKING;
    delete env.SHELL;
  }

  await new Promise((resolve, reject) => {
    const spawnOptions = {
      cwd: options.cwd ?? repoRoot,
      stdio: 'inherit',
      shell: false,
      env,
    };

    const child =
      process.platform === 'win32' ? spawn('cmd.exe', ['/d', '/s', '/c', command, ...args], spawnOptions) : spawn(command, args, spawnOptions);

    child.on('exit', (code) => {
      if (code === 0) {
        resolve();
        return;
      }
      reject(new Error(`${command} ${args.join(' ')} failed with exit code ${code ?? 1}`));
    });

    child.on('error', reject);
  });
}
