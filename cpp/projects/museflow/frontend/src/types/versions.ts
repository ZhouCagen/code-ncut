import type { Version } from './index'
export interface Difference { label: string; a: string; b: string }
const labels: Record<string, string> = { title: '作品标题', sceneDescription: '场景描述', purpose: '用途', genre: '曲风', subGenre: '子曲风', mood: '情绪', bpm: 'BPM', key: '调性', instruments: '乐器', vocalMode: '人声模式', vocalGender: '人声音色', language: '语言', lyrics: '歌词', targetDurationSeconds: '目标时长', strictDuration: '严格时长', negativePrompt: '避免元素' }
const display = (value: unknown) => value === undefined || value === '' ? '—' : typeof value === 'string' ? value : JSON.stringify(value, null, 2)
export function versionDifferences(a: Version, b: Version): Difference[] {
  const rows: Difference[] = []
  const add = (label: string, left: unknown, right: unknown) => { if (JSON.stringify(left) !== JSON.stringify(right)) rows.push({ label, a: display(left), b: display(right) }) }
  for (const [key, label] of Object.entries(labels)) add(label, a.blueprint[key as keyof typeof a.blueprint], b.blueprint[key as keyof typeof b.blueprint])
  const structure = (v: Version) => v.blueprint.structure.map(({ id: _id, ...section }) => section)
  add('情绪时间轴', structure(a), structure(b))
  add('生成模式', a.mode, b.mode); add('音乐服务商', a.provider, b.provider); add('音乐模型', a.model, b.model); add('输出格式', a.format, b.format)
  add('模型目标时长', a.modelTargetDurationSeconds, b.modelTargetDurationSeconds)
  add('实际发送的提示词', a.generationPrompt ?? a.blueprint.finalLyriaPrompt, b.generationPrompt ?? b.blueprint.finalLyriaPrompt)
  return rows
}
export function versionTree(versions: Version[]): { version: Version; depth: number }[] {
  const ids = new Set(versions.map(v => v.id)), children = new Map<string, Version[]>(), roots: Version[] = []
  for (const version of [...versions].reverse()) {
    if (version.parentVersionId && ids.has(version.parentVersionId)) children.set(version.parentVersionId, [...(children.get(version.parentVersionId) ?? []), version])
    else roots.push(version)
  }
  const result: { version: Version; depth: number }[] = [], seen = new Set<string>(), stack = roots.reverse().map(version => ({ version, depth: 0 }))
  while (stack.length) {
    const item = stack.pop()!
    if (seen.has(item.version.id)) continue
    seen.add(item.version.id); result.push(item)
    for (const version of [...(children.get(item.version.id) ?? [])].reverse()) stack.push({ version, depth: item.depth + 1 })
  }
  // Legacy or damaged lineage must not hide records.
  for (const version of versions) if (!seen.has(version.id)) result.push({ version, depth: 0 })
  return result
}
export function switchPosition(current: number, nextDuration: number) {
  if (!Number.isFinite(current) || current < 0 || !Number.isFinite(nextDuration) || nextDuration <= 0) return 0
  return Math.min(current, Math.max(0, nextDuration - .05))
}
