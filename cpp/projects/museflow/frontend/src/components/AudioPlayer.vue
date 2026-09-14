<script setup lang="ts">
import { onBeforeUnmount, ref, watch } from 'vue'
import { audioUrl } from '../api/client'
const props = defineProps<{ generationId: string; title: string; target: number; actual: number; format?: string; provider?: string; sourceDuration?: number; strict?: boolean }>()
const audio = ref<HTMLAudioElement | null>(null)
const peaks = ref<number[]>([]), progress = ref(0), error = ref('')
let controller: AbortController | undefined
let epoch = 0
watch(() => props.generationId, async id => {
  const current = ++epoch
  controller?.abort(); controller = new AbortController(); peaks.value = []; progress.value = 0; error.value = ''
  let context: AudioContext | undefined
  try {
    const response = await fetch(audioUrl(id), { signal: controller.signal })
    if (!response.ok) return
    const bytes = await response.arrayBuffer()
    context = new AudioContext()
    const buffer = await context.decodeAudioData(bytes)
    if (current !== epoch) return
    const channel = buffer.getChannelData(0)
    const step = Math.max(1, Math.floor(channel.length / 80))
    peaks.value = Array.from({ length: 80 }, (_, i) => {
      let peak = 0
      for (let j = i * step; j < Math.min(channel.length, (i + 1) * step); j++) peak = Math.max(peak, Math.abs(channel[j]!))
      return Math.max(3, peak * 90)
    })
  } catch { /* Native audio controls remain usable when waveform decoding is unavailable. */ }
  finally { if (context) await context.close().catch(() => {}) }
}, { immediate: true })
function updateProgress() { if (audio.value?.duration) progress.value = audio.value.currentTime / audio.value.duration }
function seek(event: MouseEvent) {
  if (!audio.value || !Number.isFinite(audio.value.duration)) return
  const bounds = (event.currentTarget as SVGElement).getBoundingClientRect()
  audio.value.currentTime = Math.max(0, Math.min(1, (event.clientX - bounds.left) / bounds.width)) * audio.value.duration
}
function pauseOthers() { document.querySelectorAll('audio').forEach(player => { if (player !== audio.value) player.pause() }) }
onBeforeUnmount(() => { epoch++; controller?.abort(); audio.value?.pause() })
</script>
<template>
  <div class="audio-player">
    <div class="audio-title"><div class="cover-note">♫</div><div><strong>{{ title }}</strong><small>{{ provider === 'elevenlabs' ? 'ElevenLabs 音频' : provider === 'lyria' ? 'Lyria 音频' : provider === 'songapi' ? 'SongAPI 音频' : '模拟音频' }} · {{ (format ?? 'wav').toUpperCase() }} · 本机保存</small></div></div>
    <svg v-if="peaks.length" class="waveform" viewBox="0 0 480 76" aria-label="实际音频波形，点击可跳转播放位置" @click="seek">
      <rect v-for="(peak, i) in peaks" :key="i" :x="i * 6" :y="38 - peak / 2" width="3" :height="peak" rx="1.5" :fill="i / 80 < progress ? '#6255c7' : '#cec9ed'" />
    </svg>
    <p v-else class="muted">波形加载中；也可直接使用下方播放器。</p>
    <audio ref="audio" controls preload="metadata" :src="audioUrl(generationId)" @timeupdate="updateProgress" @play="pauseOthers" @error="error = '音频加载失败，请检查本地服务及音频文件'" :aria-label="`播放 ${title}`"></audio>
    <p v-if="error" class="error" role="alert">{{ error }}</p>
    <div class="duration-row"><span>模型目标时长 <b>{{ target }} 秒</b></span><span v-if="sourceDuration !== undefined">原始音频时长 <b>{{ sourceDuration.toFixed(3) }} 秒</b></span><span>{{ strict ? '处理后实际时长' : '实际音频时长' }} <b>{{ actual.toFixed(3) }} 秒</b></span></div>
    <a class="button download-button" :href="audioUrl(generationId, true)" download>↓ 下载 {{ (format ?? 'wav').toUpperCase() }}</a>
  </div>
</template>
