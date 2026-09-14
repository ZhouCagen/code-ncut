import { computed, ref, watch } from 'vue'
import { defineStore } from 'pinia'
import { api, textRequest } from '../api/client'
import { blueprintErrors } from '../types/validation'
import type { GenerationTask, MusicBlueprint, Project, Purpose } from '../types'

export const useStudio = defineStore('studio', () => {
  const scene = ref('一位旅人走进雨后的森林，发现废弃神殿里还有微弱的光。音乐从安静、好奇，逐渐走向温暖的希望。')
  const purpose = ref<Purpose>('game')
  const blueprint = ref<MusicBlueprint | null>(null)
  const projectId = ref(''), projectName = ref(''), parentVersionId = ref(''), savedDraft = ref('')
  const task = ref<GenerationTask | null>(null)
  const busy = ref(false), error = ref(''), notice = ref('')
  const textProgress = ref(''), revisionInstruction = ref('')
  let timer: ReturnType<typeof setTimeout> | undefined
  let pollingEpoch = 0
  const active = computed(() => !!task.value && !['completed', 'failed'].includes(task.value.status))
  const draftSnapshot = () => JSON.stringify({ blueprint: blueprint.value, parentVersionId: parentVersionId.value, projectName: projectName.value })
  const dirty = computed(() => !!blueprint.value && draftSnapshot() !== savedDraft.value)
  watch(dirty, value => { (window as Window & { __museflowUnsaved?: boolean }).__museflowUnsaved = value }, { immediate: true })
  function canReplace() { return !busy.value && (!dirty.value || window.confirm('当前设计稿有未保存修改，继续将替换编辑区。确定继续？')) }
  const errors = computed(() => blueprint.value ? blueprintErrors(blueprint.value) : [])
  async function perform(action: () => Promise<void>) {
    busy.value = true; error.value = ''; notice.value = ''
    try { await action() } catch (e) { error.value = e instanceof Error ? e.message : '操作失败' }
    finally { busy.value = false; textProgress.value = '' }
  }
  async function generateBlueprint() {
    await perform(async () => {
      const result = await textRequest<{ blueprint: MusicBlueprint; provider: string }>('/blueprints/generate', { userInput: scene.value, purpose: purpose.value }, status => { textProgress.value = status })
      blueprint.value = result.blueprint
      scene.value = result.blueprint.sceneDescription; purpose.value = result.blueprint.purpose
      notice.value = result.provider === 'mock' ? '已创建模拟设计稿：这是固定模板，可自由编辑。' : '创作助手已生成设计稿、情绪时间轴和音乐提示词，并通过 Schema 校验。'
    })
  }
  async function reviseBlueprint() {
    if (!blueprint.value || !revisionInstruction.value.trim() || active.value || busy.value) return
    await perform(async () => {
      const result = await textRequest<{ blueprint: MusicBlueprint; provider: string; message?: string }>('/blueprints/revise', { blueprint: blueprint.value, instruction: revisionInstruction.value }, status => { textProgress.value = status })
      blueprint.value = result.blueprint; scene.value = result.blueprint.sceneDescription; purpose.value = result.blueprint.purpose
      notice.value = result.message ?? '设计稿已根据要求更新，尚未生成新音频。确认参数后可创建新的音乐版本。'
      revisionInstruction.value = ''
    })
  }
  async function persist() {
    if (!blueprint.value) throw new Error('请先生成音乐设计稿')
    if (errors.value.length) throw new Error(errors.value.join('；'))
    const project = await api<Project>(`/projects${projectId.value ? `/${projectId.value}` : ''}`, projectId.value ? 'PUT' : 'POST', { name: projectName.value || blueprint.value.title, blueprint: blueprint.value, draftParentVersionId: parentVersionId.value || null })
    projectId.value = project.id; projectName.value = project.name; blueprint.value = project.blueprint; parentVersionId.value = project.draftParentVersionId ?? ''; savedDraft.value = draftSnapshot()
    return project
  }
  async function save() { await perform(async () => { await persist(); notice.value = '设计稿已保存到本机 SQLite 数据库' }) }
  async function refreshPrompt() {
    await perform(async () => {
      const result = await api<{ blueprint: MusicBlueprint }>('/blueprints/validate', 'POST', { blueprint: blueprint.value })
      blueprint.value = result.blueprint; notice.value = '已校验设计稿；参数有变化时自动刷新提示词。本次校验不调用外部模型。'
    })
  }
  function stopPolling() { pollingEpoch++; clearTimeout(timer) }
  async function poll(id: string, epoch: number) {
    try {
      const result = await api<GenerationTask>(`/generations/${id}`)
      if (epoch !== pollingEpoch) return
      task.value = result; error.value = ''
      if (result.status === 'failed') error.value = result.error
      else if (result.status !== 'completed') timer = setTimeout(() => { void poll(id, epoch) }, 400)
      else notice.value = `${result.provider === 'mock' ? '模拟作品' : result.provider === 'elevenlabs' ? 'ElevenLabs 作品' : result.provider === 'songapi' ? 'SongAPI 作品' : 'Lyria 作品'}已保存。重复生成会创建新版本，历史音频会保留。`
    } catch (e) {
      if (epoch !== pollingEpoch) return
      error.value = e instanceof Error ? e.message : '获取任务状态失败'
      timer = setTimeout(() => { void poll(id, epoch) }, 2500)
    }
  }
  async function generateMusic(mode: 'preview' | 'full') {
    if (active.value || busy.value) return
    await perform(async () => {
      const previous = task.value
      task.value = null
      try {
        await persist()
        task.value = await api<GenerationTask>('/generations', 'POST', { projectId: projectId.value, blueprint: blueprint.value, mode, parentVersionId: parentVersionId.value })
        stopPolling(); void poll(task.value.id, pollingEpoch)
      } catch (error) {
        task.value = previous
        throw error
      }
    })
  }
  async function openProject(id: string) {
    if (!canReplace()) return false
    await perform(async () => {
      const project = await api<Project>(`/projects/${id}`)
      stopPolling(); projectId.value = project.id; projectName.value = project.name; blueprint.value = project.blueprint; parentVersionId.value = project.draftParentVersionId ?? ''; savedDraft.value = draftSnapshot()
      scene.value = project.sceneDescription; purpose.value = project.blueprint.purpose
      task.value = project.tasks?.find(t => !['completed', 'failed'].includes(t.status)) ?? project.tasks?.find(t => !t.audioDeleted) ?? null
      if (active.value && task.value) void poll(task.value.id, pollingEpoch)
    })
    return !error.value
  }
  async function branch(versionId: string) {
    if (active.value || !canReplace()) return false
    await perform(async () => {
      const result = await api<{ project: Project }>(`/versions/${versionId}/branch`, 'POST', {})
      const project = result.project
      stopPolling(); projectId.value = project.id; projectName.value = project.name; blueprint.value = project.blueprint
      scene.value = project.sceneDescription; purpose.value = project.blueprint.purpose
      parentVersionId.value = versionId; task.value = null; savedDraft.value = draftSnapshot()
      notice.value = '已从所选版本创建分支草稿。修改设计稿后生成，会保留父版本与旧音频。'
    })
    return !error.value
  }
  function forgetVersion(id: string, generationId: string) {
    if (parentVersionId.value === id) parentVersionId.value = ''
    if (task.value?.id === generationId) task.value = null
  }
  function reset() {
    stopPolling(); blueprint.value = null; projectId.value = ''; projectName.value = ''; parentVersionId.value = ''; savedDraft.value = ''; task.value = null; error.value = ''; notice.value = ''
  }
  return { scene, purpose, blueprint, projectId, projectName, parentVersionId, dirty, branch, forgetVersion, task, busy, error, notice, active, errors, textProgress, revisionInstruction, generateBlueprint, reviseBlueprint, save, refreshPrompt, generateMusic, openProject, reset }
})
