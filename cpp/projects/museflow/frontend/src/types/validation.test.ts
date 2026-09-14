import { describe, expect, it } from 'vitest'
import { blueprintErrors, splitTags } from './validation'
import type { MusicBlueprint } from './index'
const sample = (): MusicBlueprint => ({
  id: 'blueprint', title: '森林', sceneDescription: '旅人走进森林', purpose: 'game', genre: '氛围',
  mood: ['静谧'], bpm: 88, instruments: ['钢琴'], vocalMode: 'instrumental',
  targetDurationSeconds: 60, strictDuration: false, finalLyriaPrompt: '',
  structure: [
    { id: 'one', name: '序章', startSeconds: 0, endSeconds: 20, emotion: '平静', intensity: 3, instruments: [], description: '' },
    { id: 'two', name: '展开', startSeconds: 20, endSeconds: 60, emotion: '希望', intensity: 6, instruments: [], description: '' },
  ],
})
describe('音乐设计稿表单校验', () => {
  it('接受相邻、不重叠的段落', () => expect(blueprintErrors(sample())).toEqual([]))
  it('允许时间轴按任意编辑顺序排列', () => { const b = sample(); b.structure.reverse(); expect(blueprintErrors(b)).toEqual([]) })
  it('检测重叠', () => { const b = sample(); b.structure[1]!.startSeconds = 19; expect(blueprintErrors(b).join()).toContain('重叠') })
  it('检测错误起止时间', () => { const b = sample(); b.structure[0]!.endSeconds = 0; expect(blueprintErrors(b).join()).toContain('开始时间') })
  it('检测超出目标时长', () => { const b = sample(); b.targetDurationSeconds = 30; expect(blueprintErrors(b).join()).toContain('超出目标') })
  it('检测小数强度和错误 BPM', () => { const b = sample(); b.bpm = 400; b.structure[0]!.intensity = 2.5; expect(blueprintErrors(b)).toHaveLength(2) })
  it('检测空时间轴', () => { const b = sample(); b.structure = []; expect(blueprintErrors(b).join()).toContain('1～32') })
  it('拆分中英文乐器分隔符', () => expect(splitTags('钢琴、 弦乐，鼓, 合成器\n')).toEqual(['钢琴', '弦乐', '鼓', '合成器']))
})
