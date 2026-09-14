<script setup lang="ts">
import { computed, onMounted, ref } from 'vue'
import { useRouter } from 'vue-router'
import { api } from '../api/client'
import { useStudio } from '../stores/studio'
import type { Project } from '../types'
import { purposes, taskLabels } from '../types'
const query = ref(''), history = ref<Project | null>(null), editing = ref(''), newName = ref('')
const projects = ref<Project[]>([]), error = ref(''), loading = ref(true)
const router = useRouter(), studio = useStudio()
const filtered = computed(() => projects.value.filter(p => `${p.name} ${p.sceneDescription}`.toLowerCase().includes(query.value.toLowerCase())))
async function rename(project: Project) {
  try { const saved = await api<Project>(`/projects/${project.id}`, 'PUT', { name: newName.value, blueprint: project.blueprint }); if (studio.projectId === project.id) studio.projectName = saved.name; editing.value = ''; await load() } catch (e) { error.value = (e as Error).message }
}
async function showHistory(id: string) { try { history.value = await api<Project>(`/projects/${id}`) } catch (e) { error.value = (e as Error).message } }
async function load() {
  loading.value = true
  try { projects.value = await api<Project[]>('/projects') } catch (e) { error.value = (e as Error).message }
  finally { loading.value = false }
}
async function open(id: string) { const opened = await studio.openProject(id); if (opened) await router.push('/studio'); else error.value = studio.error }
async function remove(project: Project) {
  if (!window.confirm(`删除“${project.name}”？这会同时删除该项目的设计稿、任务和全部音频，无法恢复。`)) return
  try { const result = await api<{ audioCleanupPending: boolean }>(`/projects/${project.id}`, 'DELETE'); if (studio.projectId === project.id) studio.reset(); await load(); error.value = result.audioCleanupPending ? '数据库记录已删除，但部分音频清理失败，请检查目录权限' : '' }
  catch (e) { error.value = (e as Error).message }
}
onMounted(load)
</script>
<template>
  <div class="page-heading"><div><span class="eyebrow">YOUR WORKSPACE</span><h1>音乐项目</h1><p>接着上一次的灵感，继续创作。</p></div><RouterLink class="button primary" to="/studio">前往创作台 →</RouterLink></div>
  <label class="search-label">搜索项目<input v-model="query" type="search" placeholder="项目名称或场景描述" /></label>
  <section v-if="history" class="card history-card"><div class="section-heading"><h2>{{ history.name }} · 生成记录</h2><button class="text-button" @click="history = null">收起记录</button></div><p v-if="!history.tasks?.length" class="muted">暂无生成任务</p><ul class="history-list"><li v-for="task in history.tasks" :key="task.id"><strong>{{ taskLabels[task.status] }}{{ task.audioDeleted ? ' · 音频已删除' : '' }}</strong><span>{{ task.mode === 'preview' ? '试听' : '完整歌曲' }} · {{ task.provider }} · {{ new Date(task.createdAt).toLocaleString('zh-CN') }}</span><p v-if="task.error" class="error">{{ task.error }}</p><small>任务 {{ task.id.slice(0, 8) }}{{ task.parentVersionId ? ' · 父版本 ' + task.parentVersionId.slice(0, 8) : '' }}</small></li></ul><button class="button small" @click="showHistory(history.id)">刷新记录</button></section>
  <p v-if="error" class="error message" role="alert">{{ error }}</p>
  <p v-if="loading" class="muted">正在读取本地项目…</p>
  <div v-else-if="!projects.length" class="card empty-state"><h2>你的第一个故事，从这里开始</h2><p>在创作台生成设计稿并保存，项目就会出现在这里。</p><RouterLink class="button" to="/studio">开始创作</RouterLink></div>
  <div v-else class="project-grid">
    <article v-for="project in filtered" :key="project.id" class="card project-card"><div class="project-top"><span class="project-icon">♫</span><span class="subtle-tag">{{ purposes[project.blueprint.purpose] }}</span></div><form v-if="editing === project.id" @submit.prevent="rename(project)"><label>项目新名称<input v-model="newName" maxlength="100" /></label><button class="button small" type="submit">保存名称</button><button class="text-button" type="button" @click="editing = ''">取消</button></form><h2 v-else>{{ project.name }}</h2><p class="project-description">{{ project.sceneDescription }}</p><div class="tag-row"><span>{{ project.blueprint.genre }}</span><span>{{ project.blueprint.bpm }} BPM</span><span>{{ project.blueprint.targetDurationSeconds }} 秒目标</span></div><small class="muted">更新于 {{ new Date(project.updatedAt).toLocaleString('zh-CN') }}</small><div class="project-actions"><button class="button small" @click="open(project.id)" :disabled="studio.busy">打开设计稿 →</button><button class="text-button danger" @click="remove(project)">删除项目</button></div><div class="version-actions"><button class="text-button" @click="editing = project.id; newName = project.name">重命名</button><button class="text-button" @click="showHistory(project.id)">生成记录</button><RouterLink class="text-link" :to="{ path: '/compare', query: { project: project.id } }">版本树与对比</RouterLink><a class="text-link" :href="`/api/projects/${project.id}/export`" download>导出设计与记录</a></div></article>
  </div>
</template>
