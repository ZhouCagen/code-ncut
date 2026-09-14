<script setup lang="ts">
import { computed } from 'vue'
import type { MusicBlueprint } from '../types'
import { splitTags } from '../types/validation'
const blueprint = defineModel<MusicBlueprint>({ required: true })
const sorted = computed(() => [...blueprint.value.structure].sort((a, b) => a.startSeconds - b.startSeconds))
function addSection() {
  const b = blueprint.value
  if (b.structure.length >= 32) return
  let start = 0, end = b.targetDurationSeconds
  for (const section of sorted.value) {
    if (section.startSeconds - start >= 1) { end = section.startSeconds; break }
    start = Math.max(start, section.endSeconds)
  }
  if (end - start < 1) {
    const longest = [...b.structure].sort((a, c) => (c.endSeconds - c.startSeconds) - (a.endSeconds - a.startSeconds))[0]
    if (!longest || longest.endSeconds <= longest.startSeconds) return
    start = Math.round((longest.startSeconds + longest.endSeconds) * 50) / 100
    end = longest.endSeconds; longest.endSeconds = start
  }
  b.structure.push({ id: crypto.randomUUID(), name: '新的段落', startSeconds: start, endSeconds: end, emotion: '平静', intensity: 5, instruments: [], description: '' })
}
</script>
<template>
  <section class="card timeline-card">
    <div class="section-heading"><div><span class="eyebrow">03 / EMOTION TIMELINE</span><h2>让情绪沿时间展开</h2></div><button class="button small" @click="addSection" :disabled="blueprint.structure.length >= 32">＋ 添加段落</button></div>
    <p class="muted">每一段都是音乐的叙事方向。可在描述中注明乐器的进入、退出和过渡。</p>
    <div class="timeline-ruler"><span>0:00</span><span>{{ blueprint.targetDurationSeconds }} 秒 · 目标</span></div>
    <div class="timeline-track" aria-label="情绪时间轴预览">
      <div v-for="(section, index) in sorted" :key="section.id" class="timeline-block" :class="`tone-${index % 3}`" :style="{ left: `${Math.max(0, section.startSeconds / blueprint.targetDurationSeconds * 100)}%`, width: `${Math.max(0, (section.endSeconds - section.startSeconds) / blueprint.targetDurationSeconds * 100)}%` }"><span>{{ section.name }}</span><small>{{ section.intensity }}/10</small></div>
    </div>
    <div v-for="(section, index) in blueprint.structure" :key="section.id" class="section-editor">
      <div class="section-number">{{ String(index + 1).padStart(2, '0') }}</div>
      <div class="section-fields">
        <div class="form-grid section-first-row">
          <label>段落名称<input v-model="section.name" maxlength="60" /></label>
          <label>开始（秒）<input v-model.number="section.startSeconds" type="number" min="0" step="0.1" /></label>
          <label>结束（秒）<input v-model.number="section.endSeconds" type="number" min="0" step="0.1" /></label>
        </div>
        <div class="form-grid three">
          <label>情绪<input v-model="section.emotion" maxlength="60" /></label>
          <label>强度 · {{ section.intensity }}/10<input v-model.number="section.intensity" type="range" min="1" max="10" /></label>
          <label>段落乐器<input :value="section.instruments.join('、')" @change="section.instruments = splitTags(($event.target as HTMLInputElement).value)" placeholder="钢琴、弦乐" /></label>
        </div>
        <label>过渡与编配描述<input v-model="section.description" placeholder="例如：弦乐渐入，鼓组在段落末尾退出" maxlength="600" /></label>
      </div>
      <button class="icon-button danger" :aria-label="`删除段落 ${index + 1}`" @click="blueprint.structure.splice(index, 1)" title="删除段落">×</button>
    </div>
  </section>
</template>
