<script setup lang="ts">
import { ref, watch } from 'vue'
import type { Version } from '../types'
import { api } from '../api/client'
const props = defineProps<{ version: Version }>()
const emit = defineEmits<{ saved: [version: Version] }>()
const rating = ref(0), notes = ref(''), busy = ref(false), error = ref(''), saved = ref(false)
watch(() => props.version.id, () => { rating.value = props.version.rating; notes.value = props.version.notes; saved.value = false }, { immediate: true })
async function save() {
  busy.value = true; error.value = ''; saved.value = false
  try { const result = await api<Version>(`/versions/${props.version.id}/metadata`, 'PUT', { rating: rating.value, notes: notes.value }); emit('saved', result); saved.value = true }
  catch (e) { error.value = (e as Error).message } finally { busy.value = false }
}
</script>
<template><details><summary>评分与备注{{ version.rating ? ` · ${version.rating} / 5` : '' }}</summary><form @submit.prevent="save" class="metadata-form"><label>版本评分<select v-model.number="rating"><option :value="0">未评分</option><option v-for="score in 5" :key="score" :value="score">{{ score }} / 5</option></select></label><label>版本备注<textarea v-model="notes" rows="2" maxlength="2000" placeholder="例如：弦乐更贴合结尾，但鼓点稍强。" /></label><button class="button small" :disabled="busy">保存评价</button><span v-if="saved" role="status">评价已保存</span><p v-if="error" class="error" role="alert">{{ error }}</p></form></details></template>
