import { describe, expect, it } from 'vitest'
import { versionDifferences, versionTree, switchPosition } from './versions'
import type { Version } from './index'
const version = (id: string, parentVersionId: string | null = null): Version => ({ id, parentVersionId, projectId: 'project', generationId: id, title: '测试', mode: 'full', provider: 'mock', format: 'wav', actualDurationSeconds: 8, modelTargetDurationSeconds: 60, strictDurationApplied: false, rating: 0, notes: '', createdAt: '', blueprint: { id, title: '测试', sceneDescription: '森林', purpose: 'game', genre: 'Ambient', bpm: 88, mood: [], instruments: [], vocalMode: 'instrumental', targetDurationSeconds: 60, strictDuration: false, finalLyriaPrompt: 'forest', structure: [{ id, name: 'intro', startSeconds: 0, endSeconds: 60, intensity: 5, emotion: 'calm', instruments: [], description: '' }] } })
describe('version comparison', () => {
  it('ignores random IDs and metadata while comparing musical content', () => { const a = version('a'), b = version('b'); b.rating = 5; b.notes = 'great'; expect(versionDifferences(a, b)).toEqual([]) })
  it('compares actual generation prompt, BPM and timeline', () => { const a = version('a'), b = version('b'); b.blueprint.bpm = 112; b.blueprint.structure[0]!.intensity = 9; b.generationPrompt = 'preview'; expect(versionDifferences(a, b).map(x => x.label)).toEqual(['BPM', '情绪时间轴', '实际发送的提示词']) })
  it('orders a branched tree without hiding a missing-parent record', () => { expect(versionTree([version('orphan', 'missing'), version('sibling', 'root'), version('grand', 'child'), version('child', 'root'), version('root')]).map(x => [x.version.id, x.depth])).toEqual([['root', 0], ['child', 1], ['grand', 2], ['sibling', 1], ['orphan', 0]]) })
  it('does not loop or hide records with damaged cyclic lineage', () => { expect(versionTree([version('a', 'b'), version('b', 'a')])).toHaveLength(2) })
  it('keeps absolute playback time and clamps to a shorter track', () => { expect(switchPosition(2, 8)).toBe(2); expect(switchPosition(10, 5)).toBe(4.95); expect(switchPosition(NaN, 5)).toBe(0); expect(switchPosition(1, 0)).toBe(0) })
})
