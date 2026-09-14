import { afterEach, describe, expect, it, vi } from 'vitest'
import { textRequest } from './client'
const response = (value: unknown, status = 200) => Promise.resolve(new Response(JSON.stringify(value), { status, headers: { 'Content-Type': 'application/json' } }))
afterEach(() => { vi.unstubAllGlobals(); vi.useRealTimers() })
describe('后台文本任务客户端', () => {
  it('兼容立即返回的 Mock 结果', async () => {
    const fetch = vi.fn(() => response({ provider: 'mock' })); vi.stubGlobal('fetch', fetch)
    expect(await textRequest('/blueprints/generate', {})).toEqual({ provider: 'mock' })
    expect(fetch).toHaveBeenCalledTimes(1)
  })
  it('轮询真正的后台任务直到完成', async () => {
    vi.useFakeTimers()
    const fetch = vi.fn().mockImplementationOnce(() => response({ job: { id: 'job', status: 'queued' } }, 202))
      .mockImplementationOnce(() => response({ id: 'job', status: 'running' }))
      .mockImplementationOnce(() => response({ id: 'job', status: 'completed', result: { provider: 'deepseek' } }))
    vi.stubGlobal('fetch', fetch)
    const promise = textRequest('/blueprints/generate', {})
    await vi.advanceTimersByTimeAsync(900)
    expect(await promise).toEqual({ provider: 'deepseek' })
    expect(fetch).toHaveBeenCalledTimes(3)
  })
  it('呈现后端脱敏后的模型错误', async () => {
    vi.useFakeTimers()
    const fetch = vi.fn().mockImplementationOnce(() => response({ job: { id: 'job', status: 'queued' } }, 202))
      .mockImplementationOnce(() => response({ id: 'job', status: 'failed', error: '模型请求过于频繁', code: 'rate_limited' }))
    vi.stubGlobal('fetch', fetch)
    const expected = expect(textRequest('/blueprints/generate', {})).rejects.toThrow('模型请求过于频繁')
    await vi.advanceTimersByTimeAsync(450)
    await expected
  })
})
