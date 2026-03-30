import path from 'node:path';
import { copyFileWithTimestamp, formatBuildId, pathExists, releaseRoot, repoRoot, runCommand, writeBuildInfo } from './release-lib.mjs';

async function main() {
  const buildId = formatBuildId();
  const firmwareEnv = process.env.PIO_FIRMWARE_ENV || 'esp32s3mini';

  await writeBuildInfo(buildId);

  console.log(`[release] build id: ${buildId}`);

  await runCommand('pnpm', ['--dir', 'webui', 'build']);
  await runCommand('pio', ['run', '-e', firmwareEnv]);
  await runCommand('pio', ['run', '-e', firmwareEnv, '-t', 'buildfs']);

  const srcFirmware = path.join(repoRoot, '.pio', 'build', firmwareEnv, 'firmware.bin');
  const srcSpiffs = path.join(repoRoot, '.pio', 'build', firmwareEnv, 'spiffs.bin');

  if (!(await pathExists(srcFirmware))) {
    throw new Error(`Firmware artifact not found: ${srcFirmware}`);
  }

  if (!(await pathExists(srcSpiffs))) {
    throw new Error(`SPIFFS artifact not found: ${srcSpiffs}`);
  }

  const dstFirmware = path.join(releaseRoot, 'firmware');
  const dstSpiffs = path.join(releaseRoot, 'spiffs');
  const filename = `${buildId}.bin`;

  const firmwareOutPath = await copyFileWithTimestamp(srcFirmware, dstFirmware, filename);
  const spiffsOutPath = await copyFileWithTimestamp(srcSpiffs, dstSpiffs, filename);

  console.log(`[release] firmware: ${firmwareOutPath}`);
  console.log(`[release] spiffs:   ${spiffsOutPath}`);
}

main().catch((error) => {
  console.error(`[release] ${error.message}`);
  process.exit(1);
});
