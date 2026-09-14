import { defineConfig } from '@playwright/test'
export default defineConfig({
  testDir: './tests', workers: 1, timeout: 30000,
  use: {
    baseURL: 'http://127.0.0.1:18080', headless: true, viewport: { width: 1440, height: 1000 },
    // Use the user's installed Chrome with an isolated temporary browser profile.
    ...(process.env.MUSEFLOW_CHROME_PATH
      ? { launchOptions: { executablePath: process.env.MUSEFLOW_CHROME_PATH } }
      : { channel: 'chrome' }),
  },
  webServer: { command: 'python3 ../scripts/test_server.py', url: 'http://127.0.0.1:18080/api/health', reuseExistingServer: false, timeout: 15000, gracefulShutdown: { signal: 'SIGTERM', timeout: 6000 } },
})
