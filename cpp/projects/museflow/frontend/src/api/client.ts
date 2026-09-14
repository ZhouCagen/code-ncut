import type { TextJob } from '../types/settings'
export async function api<T>(path: string, method = 'GET', body?: unknown): Promise<T> {
  let response: Response
  try {
    response = await fetch(`/api${path}`, {
      method, headers: body === undefined ? {} : { 'Content-Type': 'application/json' },
      body: body === undefined ? undefined : JSON.stringify(body),
      signal: AbortSignal.timeout(15000),
    })
  } catch { throw new Error('无法连接本地后端，请确认 127.0.0.1:18080 正在运行') }
  const data = await response.json().catch(() => null)
  if (!response.ok) throw new Error(data?.error ?? `本地请求失败（${response.status}）`)
  if (data === null) throw new Error('本地服务返回了无效数据')
  return data as T
}
export const audioUrl = (id: string, download = false) => `/api/generations/${encodeURIComponent(id)}/audio${download ? '?download=1' : ''}`

export async function textRequest<T>(path: string, body: unknown, onStatus?: (status: string) => void): Promise<T> {
  const response = await api<T | { job: TextJob<T> }>(path, 'POST', body)
  if (!(response && typeof response === 'object' && 'job' in response)) return response as T
  let job = response.job
  for (;;) {
    onStatus?.(job.status === 'queued' ? '等待创作助手处理…' : '创作助手正在生成并校验 JSON…')
    if (job.status === 'failed') throw new Error(job.error ?? '文本任务失败')
    if (job.status === 'completed') {
      if (!job.result) throw new Error('文本任务缺少结果')
      return job.result
    }
    await new Promise(resolve => setTimeout(resolve, 400))
    job = await api<TextJob<T>>(`/text-jobs/${encodeURIComponent(job.id)}`)
  }
}
