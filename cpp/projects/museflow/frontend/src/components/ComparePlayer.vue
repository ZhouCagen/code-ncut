<script setup lang="ts">
import { computed, nextTick, onBeforeUnmount, ref, watch } from 'vue'
import type { Version } from '../types'
import { audioUrl } from '../api/client'
import { switchPosition } from '../types/versions'
const props = defineProps<{ a: Version; b: Version }>()
const left = ref<HTMLAudioElement | null>(null), right = ref<HTMLAudioElement | null>(null)
const selected = ref<'a' | 'b'>('a'), loop = ref(false), error = ref(''), switching = ref(false)
let epoch = 0
const current = computed(() => selected.value === 'a' ? left.value : right.value)
function stop() { left.value?.pause(); right.value?.pause() }
function pauseOthers(player: HTMLAudioElement | null) { document.querySelectorAll('audio').forEach(audio => { if (audio !== player) audio.pause() }) }
async function select(side: 'a' | 'b') {
  if (switching.value) return
  const previous = current.value, position = previous?.currentTime ?? 0, playing = previous && !previous.paused
  const next = side === 'a' ? left.value : right.value
  const ticket = ++epoch
  stop(); selected.value = side; error.value = ''; switching.value = true
  try {
    if (!next) return
    if (next.readyState < 1) await new Promise<void>((resolve, reject) => {
      const clean = () => { next.removeEventListener('loadedmetadata', ready); next.removeEventListener('error', failed); clearTimeout(timer) }
      const ready = () => { clean(); resolve() }, failed = () => { clean(); reject(new Error('音频加载失败')) }
      const timer = setTimeout(() => { clean(); reject(new Error('音频加载超时')) }, 10000)
      next.addEventListener('loadedmetadata', ready); next.addEventListener('error', failed)
    })
    if (ticket !== epoch) return
    next.currentTime = switchPosition(position, next.duration)
    if (playing) { pauseOthers(next); await next.play() }
  } catch (e) { if (ticket === epoch) error.value = (e as Error).message }
  finally { if (ticket === epoch) switching.value = false }
}
async function play() { try { if (current.value) { pauseOthers(current.value); await current.value.play(); error.value = '' } } catch { error.value = '无法播放所选音频，请检查本地音频文件' } }
watch(() => [props.a.id, props.b.id], async () => { epoch++; stop(); selected.value = 'a'; switching.value = false; error.value = ''; await nextTick(); left.value?.load(); right.value?.load() })
onBeforeUnmount(() => { epoch++; stop() })
</script>
<template><section class="card compare-player"><div class="section-heading"><div><span class="eyebrow">LISTEN SIDE BY SIDE</span><h2>A/B 试听</h2></div><label class="checkbox"><input v-model="loop" type="checkbox" />循环播放当前版本</label></div><p class="muted">切换保留绝对播放位置；若新版本较短，则定位到其结尾附近。仅播放当前一侧。</p><div class="ab-switch"><button class="button" :class="{ primary: selected === 'a' }" :aria-pressed="selected === 'a'" :disabled="switching" @click="select('a')">切换到 A</button><button class="button" :class="{ primary: selected === 'b' }" :aria-pressed="selected === 'b'" :disabled="switching" @click="select('b')">切换到 B</button><button class="button" @click="play" :disabled="switching">播放当前版本</button><button class="text-button" @click="stop">暂停</button></div><div class="ab-tracks"><div><strong>A · {{ a.title }} <small>{{ a.id.slice(0, 8) }}</small></strong><audio ref="left" controls preload="auto" :src="audioUrl(a.generationId)" :loop="loop" :aria-label="'对比 A 音频'" @play="selected = 'a'; pauseOthers(left)" @error="error = 'A 音频加载失败'" /></div><div><strong>B · {{ b.title }} <small>{{ b.id.slice(0, 8) }}</small></strong><audio ref="right" controls preload="auto" :src="audioUrl(b.generationId)" :loop="loop" :aria-label="'对比 B 音频'" @play="selected = 'b'; pauseOthers(right)" @error="error = 'B 音频加载失败'" /></div></div><p v-if="error" role="alert" class="error">{{ error }}</p><p v-if="a.provider === 'mock' || b.provider === 'mock'" class="small-note">模拟版本使用同一原始测试音频；参数差异不会改变原始演奏，严格时长处理可能不同。</p></section></template>
