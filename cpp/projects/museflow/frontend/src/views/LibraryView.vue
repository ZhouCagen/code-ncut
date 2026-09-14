<script setup lang="ts">
import { computed, onMounted, ref, watch } from 'vue'
import { useRouter } from 'vue-router'
import VersionMetadata from '../components/VersionMetadata.vue'
import { api } from '../api/client'
import type { Version, Project } from '../types'
import AudioPlayer from '../components/AudioPlayer.vue'
import { useStudio } from '../stores/studio'
const studio = useStudio(), router = useRouter()
async function branch(id: string) { if (await studio.branch(id)) await router.push('/studio'); else if (studio.error) error.value = studio.error }
async function remove(version: Version) {
  if (!window.confirm(`删除“${version.title}”的这个版本及原始音频？生成任务记录会保留，此操作无法恢复。`)) return
  try { const result = await api<{ audioCleanupPending: boolean }>(`/versions/${version.id}`, 'DELETE'); studio.forgetVersion(version.id, version.generationId); await load(); if (result.audioCleanupPending) error.value = '版本已删除，部分文件清理失败，请检查数据目录权限' } catch (e) { error.value = (e as Error).message }
}
const versions = ref<Version[]>([]), projects = ref<Project[]>([]), query = ref(''), projectId = ref(''), error = ref(''), loading = ref(true)
const filtered = computed(() => versions.value.filter(v => (!projectId.value || v.projectId === projectId.value) && `${v.title} ${v.blueprint.genre} ${v.blueprint.sceneDescription}`.toLowerCase().includes(query.value.toLowerCase())))
async function load() {
  try { [versions.value, projects.value] = await Promise.all([api<Version[]>('/versions'), api<Project[]>('/projects')]); error.value = '' }
  catch (e) { error.value = (e as Error).message }
  finally { loading.value = false }
}
onMounted(load)
watch(() => studio.task?.status, status => { if (status === 'completed') void load() })
</script>
<template>
  <div class="page-heading"><div><span class="eyebrow">SAVED SOUNDS</span><h1>作品库</h1><p>每一次生成，都有迹可循。共 {{ versions.length }} 个本地作品。</p></div><span class="badge mock">本机作品</span></div>
  <div class="library-toolbar"><label class="search-label"><span class="sr-only">搜索作品</span><input v-model="query" type="search" placeholder="搜索作品、曲风或场景…" /></label><label><span class="sr-only">按项目筛选</span><select v-model="projectId"><option value="">全部项目</option><option v-for="p in projects" :key="p.id" :value="p.id">{{ p.name }}</option></select></label></div>
  <p v-if="error" class="error message" role="alert">{{ error }}</p><p v-if="loading" class="muted">正在读取作品…</p>
  <div v-else-if="!filtered.length" class="card empty-state"><h2>{{ versions.length ? '没有匹配的作品' : '还没有保存的声音' }}</h2><p>完成一次音乐生成后，作品会自动出现在这里。</p><RouterLink class="button" to="/studio">返回创作台</RouterLink></div>
  <div v-else class="library-grid"><article v-for="version in filtered" :key="version.id" :data-version-id="version.id" class="card"><div class="library-meta"><span class="subtle-tag">{{ version.mode === 'preview' ? '试听' : '完整歌曲' }} · {{ version.provider === 'mock' ? '模拟' : 'Lyria' }}</span><small>{{ new Date(version.createdAt).toLocaleString('zh-CN') }}</small></div><AudioPlayer :generation-id="version.generationId" :title="version.title" :target="version.modelTargetDurationSeconds" :actual="version.actualDurationSeconds" :format="version.format" :provider="version.provider" :source-duration="version.sourceDurationSeconds" :strict="version.strictDurationApplied" /><details><summary>查看生成时的参数与提示词</summary><div class="tag-row"><span>{{ version.blueprint.genre }}</span><span>{{ version.blueprint.bpm }} BPM</span><span>{{ version.blueprint.key }}</span></div><p class="muted">作品 ID：{{ version.id }}</p><pre>{{ version.generationPrompt ?? version.blueprint.finalLyriaPrompt }}</pre></details><details v-if="version.songText?.length"><summary>Lyria 返回的歌词与歌曲结构</summary><pre>{{ version.songText.join('\n\n') }}</pre></details><VersionMetadata :version="version" @saved="load" /><div class="version-actions"><button class="button small" :disabled="studio.busy || studio.active" @click="branch(version.id)">从此版本创建分支</button><RouterLink class="text-link" :to="{ path: '/compare', query: { project: version.projectId, a: version.id } }">加入 A/B 对比</RouterLink><button class="text-button danger" @click="remove(version)">删除作品</button></div></article></div>
</template>
