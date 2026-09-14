import { computed, ref } from 'vue'
import { defineStore } from 'pinia'
import { api } from '../api/client'
import type { ModelSettings } from '../types/settings'
export const useSettings = defineStore('settings', () => {
  // Only public settings enter Pinia. Password inputs are never stored here.
  const data = ref<ModelSettings | null>(null), error = ref(''), connected = ref(false)
  const textIsMock = computed(() => data.value?.text.provider === 'mock')
  const musicIsMock = computed(() => data.value?.music.provider === 'mock')
  const musicLabel = computed(() => data.value?.music.provider === 'elevenlabs' ? 'ElevenLabs Music' : data.value?.music.provider === 'songapi' ? 'SongAPI · Suno' : musicIsMock.value ? 'Mock Music' : 'Google Lyria')
  const modeLabel = computed(() => !data.value ? '正在读取模型状态…' : musicIsMock.value ? (textIsMock.value ? '模拟模式 · 不产生 API 费用' : 'DeepSeek 创作 · 音乐模拟') : `${textIsMock.value ? '设计稿模拟' : 'DeepSeek 创作'} · ${musicLabel.value} 音乐生成`)
  async function load() {
    try { data.value = await api<ModelSettings>('/settings'); connected.value = true; error.value = '' }
    catch (e) { connected.value = false; error.value = (e as Error).message }
  }
  async function save(patch: unknown) {
    data.value = await api<ModelSettings>('/settings', 'PUT', patch); connected.value = true; error.value = ''
  }
  return { data, error, connected, textIsMock, musicIsMock, musicLabel, modeLabel, load, save }
})
