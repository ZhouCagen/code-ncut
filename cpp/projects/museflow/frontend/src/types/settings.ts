export interface ModelSettings {
  mode: 'mock' | 'mixed' | 'live'
  text: {
    provider: 'mock' | 'deepseek'; baseUrl: string; endpoint: string; model: string
    connectTimeoutSeconds: number; requestTimeoutSeconds: number
    hasApiKey: boolean; keySource: 'none' | 'session' | 'environment'
    status: 'untested' | 'connected' | 'error'; message: string
  }
  music: {
    provider: 'mock' | 'lyria' | 'songapi' | 'elevenlabs'; songapiModel: string; songapiSandbox: boolean; songapiKeySource: string; elevenlabsKeySource: string; lyriaKeySource: string; model: 'lyria-3-clip-preview' | 'lyria-3.5'
    outputFormat: 'mp3' | 'wav'; previewFirst: boolean; activeOutputFormat: 'wav' | 'mp3'
    hasApiKey: boolean; keySource: 'none' | 'session' | 'environment'; status: string; message: string
  }
  capabilities: { realTextProvider: boolean; realMusicProvider: boolean; strictDuration: boolean; branching: boolean }
}
export interface TextJob<T> { id: string; status: 'queued' | 'running' | 'completed' | 'failed'; result?: T; error?: string; code?: string }
