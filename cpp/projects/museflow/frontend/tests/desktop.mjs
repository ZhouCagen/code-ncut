import { chromium, expect } from '@playwright/test'
const browser = await chromium.connectOverCDP('http://127.0.0.1:18083', { timeout: 20000, noDefaults: true })
try {
  const context = browser.contexts()[0]
  let page
  for (let attempt = 0; attempt < 100; attempt++) {
    page = context.pages().find(p => p.url().startsWith('http://127.0.0.1:18080/'))
    if (page) break
    await new Promise(resolve => setTimeout(resolve, 100))
  }
  if (!page) throw new Error('Qt desktop did not load the local interface')
  const agent = await page.evaluate(() => navigator.userAgent)
  if (!agent.includes('QtWebEngine')) throw new Error('Test did not connect to Qt WebEngine')
  await expect(page.getByRole('heading', { name: '从一个场景，开始一段音乐' })).toBeVisible()
  await page.evaluate(async () => { await fetch('/api/settings', { method: 'PUT', headers: { 'Content-Type': 'application/json' }, body: JSON.stringify({ music: { provider: 'mock', outputFormat: 'mp3' } }) }) })
  await page.getByRole('button', { name: 'AI 生成音乐设计稿' }).click()
  await page.getByLabel('作品标题').fill('桌面窗口测试')
  await page.getByRole('button', { name: '生成 30 秒试听 · 模拟' }).click()
  await expect(page.getByText('作品已保存', { exact: true })).toBeVisible({ timeout: 15000 })
  await expect(page.getByRole('link', { name: '↓ 下载 MP3' })).toBeVisible()
  const audio = page.locator('audio')
  await audio.evaluate(async player => { await player.play() })
  await expect.poll(() => audio.evaluate(player => player.currentTime)).toBeGreaterThan(0)
  await audio.evaluate(player => player.pause())
  await page.getByRole('link', { name: '作品库', exact: true }).click()
  await page.getByRole('button', { name: '从此版本创建分支' }).click()
  await page.getByLabel('BPM', { exact: true }).fill('120')
  await page.getByRole('button', { name: '生成完整歌曲 · 模拟' }).click()
  await expect(page.getByText('作品已保存', { exact: true })).toBeVisible({ timeout: 15000 })
  await page.getByRole('link', { name: '版本对比', exact: true }).click()
  await expect(page.locator('.version-tree li')).toHaveCount(2)
  await expect(page.locator('.diff-table')).toContainText('120')
  await page.getByLabel('循环播放当前版本').check()
  await page.getByRole('button', { name: '播放当前版本', exact: true }).click()
  await page.getByRole('button', { name: '切换到 B' }).click()
  await expect.poll(() => page.getByLabel('对比 B 音频').evaluate(player => !player.paused)).toBe(true)
  await expect.poll(() => page.getByLabel('对比 A 音频').evaluate(player => player.paused)).toBe(true)
  await page.getByRole('button', { name: '暂停', exact: true }).click()
  await page.screenshot({ path: 'test-results/desktop-qt-compare.png', fullPage: true })
  const blocked = await page.evaluate(async () => { try { await fetch('https://example.com/'); return false } catch { return true } })
  if (!blocked) throw new Error('Desktop allowed an external renderer request')
  console.log('PASS: real Qt WebEngine window, create/generate/play, branching/A-B, blocked external requests')
} finally { await browser.close() }
