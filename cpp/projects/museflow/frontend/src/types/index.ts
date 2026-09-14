export type Purpose = 'game' | 'video' | 'animation' | 'story' | 'song' | 'other'
export interface MusicSection {
  id: string; name: string; startSeconds: number; endSeconds: number
  emotion: string; intensity: number; instruments: string[]; description: string
}
export interface MusicBlueprint {
  id: string; title: string; sceneDescription: string; purpose: Purpose
  genre: string; subGenre?: string; mood: string[]; bpm?: number; key?: string
  instruments: string[]; vocalMode: 'instrumental' | 'vocal'; vocalGender?: string
  language?: string; lyrics?: string; targetDurationSeconds: number; strictDuration: boolean
  structure: MusicSection[]; negativePrompt?: string; finalLyriaPrompt: string
  promptSource?: 'local' | 'deepseek'; promptFingerprint?: string
}
export type TaskState = 'queued' | 'preparing' | 'generating' | 'post_processing' | 'completed' | 'failed'
export interface GenerationTask {
  id: string; projectId: string; blueprint: MusicBlueprint; mode: 'preview' | 'full'
  status: TaskState; progress: number; error: string; provider: 'mock' | 'lyria' | 'songapi' | 'elevenlabs'
  format?: 'wav' | 'mp3'; strictDurationApplied?: boolean; sourceDurationSeconds?: number; songText?: string[]; songStructure?: unknown[];
  actualDurationSeconds?: number; modelTargetDurationSeconds: number
  parentVersionId?: string | null; audioDeleted?: boolean; versionId?: string
  createdAt: string; updatedAt: string
}
export interface Project {
  id: string; name: string; sceneDescription: string; blueprint: MusicBlueprint
  createdAt: string; updatedAt: string; tasks?: GenerationTask[]; draftParentVersionId?: string | null
}
export interface Version {
  id: string; projectId: string; generationId: string; parentVersionId: string | null
  title: string; blueprint: MusicBlueprint; mode: 'preview' | 'full'; provider: 'mock' | 'lyria' | 'songapi' | 'elevenlabs'
  model?: string; format: 'wav' | 'mp3'; generationPrompt?: string; songText?: string[]; songStructure?: unknown[]; sourceDurationSeconds?: number; actualDurationSeconds: number; modelTargetDurationSeconds: number
  strictDurationApplied: boolean; rating: number; notes: string; createdAt: string
}
export const purposes: Record<Purpose, string> = { game: '游戏', video: '视频', animation: '动画', story: '故事', song: '歌曲', other: '其他' }
export const taskLabels: Record<TaskState, string> = {
  queued: '等待生成', preparing: '准备设计稿', generating: '音乐生成中',
  post_processing: '检测、处理并保存音频', completed: '作品已保存', failed: '生成失败',
}
