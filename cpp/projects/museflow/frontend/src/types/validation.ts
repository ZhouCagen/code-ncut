import type { MusicBlueprint } from './index'
export const splitTags = (value: string): string[] => value.split(/[,，、\n]/).map(x => x.trim()).filter(Boolean)
export function blueprintErrors(b: MusicBlueprint): string[] {
  const errors: string[] = []
  if (!b.title.trim()) errors.push('请填写作品标题')
  if (!b.sceneDescription.trim()) errors.push('请填写场景描述')
  if (!b.genre.trim()) errors.push('请填写曲风')
  if (!Number.isInteger(b.bpm) || b.bpm! < 40 || b.bpm! > 240) errors.push('BPM 必须是 40～240 之间的整数')
  if (!Number.isFinite(b.targetDurationSeconds) || b.targetDurationSeconds < 5 || b.targetDurationSeconds > 600) errors.push('目标时长应为 5～600 秒')
  if (!b.structure.length || b.structure.length > 32) errors.push('时间轴应包含 1～32 个段落')
  const sorted = [...b.structure].sort((a, c) => a.startSeconds - c.startSeconds)
  let lastEnd = 0
  for (const s of sorted) {
    if (!s.name.trim()) errors.push('请填写段落名称')
    if (!Number.isFinite(s.startSeconds) || !Number.isFinite(s.endSeconds) || s.startSeconds < 0 || s.startSeconds >= s.endSeconds) errors.push(`${s.name}：开始时间必须小于结束时间，且不能为负数`)
    if (s.startSeconds < lastEnd) errors.push(`${s.name}：时间段重叠`)
    if (s.endSeconds > b.targetDurationSeconds) errors.push(`${s.name}：超出目标时长`)
    if (!Number.isInteger(s.intensity) || s.intensity < 1 || s.intensity > 10) errors.push(`${s.name}：强度应为 1～10 的整数`)
    lastEnd = s.endSeconds
  }
  return [...new Set(errors)]
}
