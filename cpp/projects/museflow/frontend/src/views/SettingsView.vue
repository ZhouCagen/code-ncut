<script setup lang="ts">
import { computed, onBeforeUnmount, onMounted, reactive, ref } from 'vue'
import { textRequest } from '../api/client'
import { useSettings } from '../stores/settings'
import { useStudio } from '../stores/studio'
import { useRouter } from 'vue-router'
import { api } from '../api/client'
import type { MusicBlueprint, Project } from '../types'
const studio = useStudio(), router = useRouter()
const settings = useSettings()
const textForm = reactive({ provider: 'mock', baseUrl: 'https://api.deepseek.com', model: 'deepseek-v4-flash', connectTimeoutSeconds: 10, requestTimeoutSeconds: 90 })
const musicForm = reactive({ provider: 'mock', model: 'lyria-3-clip-preview', songapiModel: 'v5', outputFormat: 'wav', previewFirst: true })
const textKey = ref(''), musicKey = ref(''), error = ref(''), notice = ref(''), progress = ref(''), busy = ref(false)
const ready = ref(false)
const isElevenLabs = computed(() => musicForm.provider === 'elevenlabs')
const isSongApi = computed(() => musicForm.provider === 'songapi')
const musicKeyLabel = computed(() => isElevenLabs.value ? 'ElevenLabs API Key' : isSongApi.value ? 'SongAPI Key' : 'Google Gemini API Key')
const musicEnvironment = computed(() => isElevenLabs.value ? 'ELEVENLABS_API_KEY' : isSongApi.value ? 'SONGAPI_KEY' : 'GEMINI_API_KEY')
const musicKeySource = computed(() => isElevenLabs.value ? settings.data?.music.elevenlabsKeySource : isSongApi.value ? settings.data?.music.songapiKeySource : settings.data?.music.lyriaKeySource)
function sync() {
  const current = settings.data
  if (!current) return
  Object.keys(textForm).forEach(key => { (textForm as Record<string, unknown>)[key] = current.text[key as keyof typeof current.text] })
  Object.keys(musicForm).forEach(key => { (musicForm as Record<string, unknown>)[key] = current.music[key as keyof typeof current.music] })
}
onMounted(async () => { await settings.load(); sync(); ready.value = true })
onBeforeUnmount(() => { textKey.value = ''; musicKey.value = '' })
async function perform(action: () => Promise<void>) {
  busy.value = true; error.value = ''; notice.value = ''; progress.value = ''
  try { await action() } catch (e) { error.value = (e as Error).message }
  finally { busy.value = false; progress.value = ''; textKey.value = ''; musicKey.value = '' }
}
async function saveText(test = false) {
  await perform(async () => {
    await settings.save({ text: { ...textForm, ...(textKey.value ? { apiKey: textKey.value } : {}) } })
    textKey.value = ''
    if (test) {
      try {
        const result = await textRequest<{ ok: boolean; message: string }>('/providers/text/test', {}, value => { progress.value = value })
        notice.value = result.message
      } finally { await settings.load() }
    } else notice.value = '创作助手设置已保存。界面输入的 Key 仅保存在后端进程内存。'
  })
}
async function saveMusic() {
  await perform(async () => {
    await settings.save({ music: { ...musicForm, ...(musicKey.value ? { apiKey: musicKey.value } : {}) } })
    notice.value = '音乐模型设置已保存。Key 仅保存在本次后端会话，重启软件后需要重新填写；点击“保存并测试密钥”可检查连接。'
  })
}
async function keyAction(kind: 'text' | 'music', action: 'clearApiKey' | 'useEnvironmentKey') {
  await perform(async () => { await settings.save({ [kind]: { ...(kind === 'music' ? { ...musicForm } : {}), [action]: true } }); notice.value = action === 'clearApiKey' ? '已清除会话 Key，并在本次会话停用对应环境变量 Key。' : '已恢复使用对应环境变量 Key（若已设置）。' })
}
async function testMusic() {
  await perform(async () => {
    await settings.save({ music: { ...musicForm, ...(musicKey.value ? { apiKey: musicKey.value } : {}) } }); musicKey.value = ''
    try { const result = await textRequest<{ message: string }>('/providers/music/test', {}, value => { progress.value = value }); notice.value = result.message }
    finally { await settings.load() }
  })
}
async function generateTestMusic() {
  await perform(async () => {
    if (studio.busy || studio.active) throw new Error('请等待当前创作任务完成后再生成测试音乐')
    await settings.save({ music: { ...musicForm, ...(musicKey.value ? { apiKey: musicKey.value } : {}) } }); musicKey.value = ''
    // A local blueprint fixture avoids an unintended paid text-model request.
    const blueprint: MusicBlueprint = { id: crypto.randomUUID(), title: '音乐服务测试', sceneDescription: '温暖、轻柔的原声吉他纯音乐', purpose: 'other', genre: 'Acoustic', mood: ['温暖'], bpm: 90, key: 'C major', instruments: ['吉他'], vocalMode: 'instrumental', targetDurationSeconds: 30, strictDuration: false, structure: [{ id: crypto.randomUUID(), name: '试听', startSeconds: 0, endSeconds: 30, emotion: '温暖', intensity: 4, instruments: ['吉他'], description: '轻柔的旋律，自然结束' }], finalLyriaPrompt: '' }
    const project = await api<Project>('/projects', 'POST', { name: blueprint.title, blueprint })
    if (!await studio.openProject(project.id)) return; await studio.generateMusic('preview'); await router.push('/studio')
  })
}
const keySource = (source?: string) => source === 'session' ? '后端会话内存' : source === 'environment' ? '后端环境变量' : '尚未配置'
</script>
<template>
  <div class="page-heading"><div><span class="eyebrow">MODEL CONNECTIONS</span><h1>模型设置</h1><p>创作助手与音乐生成服务独立配置，所有模型请求由本地后端发出。</p></div></div>
  <p v-if="error || settings.error" class="error message" role="alert">{{ error || settings.error }}</p>
  <p v-if="notice" class="success message" role="status">{{ notice }}</p>
  <p v-if="progress" class="mock-notice" role="status">{{ progress }} 可以继续访问其他页面，音乐任务不受影响。</p>
  <div class="settings-grid">
    <section class="card">
      <span class="eyebrow">TEXT PROVIDER</span><h2>创作助手模型</h2><p class="muted">理解场景、编写歌词、设计情绪时间轴，并生成音乐提示词。</p>
      <fieldset :disabled="busy || !ready || !settings.data" class="settings-fields">
        <label>创作助手服务商<select v-model="textForm.provider" aria-label="创作助手服务商"><option value="mock">Mock · 免费模拟</option><option value="deepseek">DeepSeek / Chat Completions 兼容服务</option></select></label>
        <label>API 地址<input v-model="textForm.baseUrl" aria-label="API 地址" type="url" autocomplete="off" spellcheck="false" maxlength="2048" placeholder="https://api.deepseek.com" /><small>支持基础地址、/v1 地址或完整 /chat/completions 地址。</small></label>
        <label>模型名称<input v-model="textForm.model" aria-label="模型名称" autocomplete="off" spellcheck="false" maxlength="160" list="text-models" /><datalist id="text-models"><option value="deepseek-v4-flash" /><option value="deepseek-v4-pro" /></datalist></label>
        <label>DeepSeek API Key<input v-model="textKey" aria-label="DeepSeek API Key" type="password" autocomplete="off" spellcheck="false" maxlength="8192" placeholder="留空保留当前 Key；保存后清空输入框" /><small>当前来源：{{ keySource(settings.data?.text.keySource) }}。Key 不回传浏览器、不写入磁盘。</small></label>
        <div class="key-actions"><button class="text-button" @click="keyAction('text', 'clearApiKey')">清除创作助手 Key</button><button class="text-button" @click="keyAction('text', 'useEnvironmentKey')">使用 DEEPSEEK_API_KEY</button></div>
        <details><summary>网络超时</summary><div class="form-grid two"><label>连接超时（秒）<input v-model.number="textForm.connectTimeoutSeconds" type="number" min="1" max="30" /></label><label>响应超时（秒）<input v-model.number="textForm.requestTimeoutSeconds" type="number" min="2" max="180" /></label></div></details>
        <div class="settings-actions"><button class="button primary" @click="saveText(false)">保存创作助手设置</button><button class="button" @click="saveText(true)">{{ textForm.provider === 'mock' ? '测试模拟创作助手' : '保存并测试连接' }}</button></div>
      </fieldset>
      <p class="small-note">选择真实服务后，测试连接和生成设计稿会发送文本请求，可能产生费用，以服务商计费为准。测试按钮会先保存当前输入。</p>
      <p class="status-text">当前连接状态：{{ settings.data?.text.status === 'connected' ? '已连接' : settings.data?.text.status === 'error' ? '连接失败' : '未测试' }}</p>
      <p v-if="settings.data?.text.message && settings.data.text.message !== notice" class="muted">{{ settings.data.text.message }}</p>
    </section>
    <section class="card">
      <span class="eyebrow">MUSIC PROVIDER</span><h2>音乐生成模型</h2><p class="muted">支持 ElevenLabs Music、SongAPI（Suno）、Google Lyria 和免费模拟。</p>
      <div class="mock-notice"><span class="badge mock">{{ settings.musicIsMock ? '模拟模式' : settings.musicLabel }}</span><span>{{ settings.musicIsMock ? '当前使用内置 8 秒测试音频，不消耗音乐额度。' : '已选择真实音乐服务，生成音乐可能产生费用。' }}</span></div>
      <fieldset :disabled="busy || !ready || !settings.data" class="settings-fields">
        <label>音乐生成服务商<select v-model="musicForm.provider" @change="musicKey = ''; notice = ''; error = ''" aria-label="音乐生成服务商"><option value="mock">Mock · 免费模拟</option><option value="elevenlabs">ElevenLabs Music</option><option value="songapi">SongAPI · Suno</option><option value="lyria">Google Lyria</option></select></label>
        <label>{{ musicKeyLabel }}<input v-model="musicKey" :aria-label="musicKeyLabel" type="password" autocomplete="off" spellcheck="false" maxlength="8192" placeholder="留空保留当前 Key；保存后清空输入框" /><small>当前来源：{{ keySource(musicKeySource) }}</small></label>
        <div class="key-actions"><button class="text-button" @click="keyAction('music', 'clearApiKey')">清除音乐模型 Key</button><button class="text-button" @click="keyAction('music', 'useEnvironmentKey')">使用 {{ musicEnvironment }}</button></div>
        <template v-if="isSongApi">
          <label>SongAPI 地址<input value="https://api.songapi.dev" readonly aria-label="SongAPI 地址" /></label>
          <label>SongAPI 模型<select v-model="musicForm.songapiModel" aria-label="SongAPI 模型"><option v-for="model in ['v5', 'v5.5', 'v4.5-plus', 'v4.5-all', 'v4.5', 'v4', 'v3.5']" :key="model" :value="model">{{ model }}</option></select></label>
          <p class="small-note">填写 songapi.dev 控制台的 Key。sk_live_ 开头用于真实音乐；sk_test_ 开头只返回静音测试音频。Key 不写入磁盘，重启后需重新填写或使用环境变量。</p>
        </template>
        <template v-else-if="isElevenLabs">
          <label>ElevenLabs 地址<input value="https://api.elevenlabs.io" readonly aria-label="ElevenLabs 地址" /></label>
          <label>ElevenLabs 音乐模型<input value="music_v1" readonly aria-label="ElevenLabs 音乐模型" /></label>
          <p class="small-note">填写 ElevenLabs 控制台的 API Key，并启用 Music 权限。Key 仅保存在本次后端会话，重启后需重新填写或使用 ELEVENLABS_API_KEY。</p>
        </template>
        <label v-else>音乐模型<select v-model="musicForm.model" aria-label="音乐模型"><option value="lyria-3-clip-preview">lyria-3-clip-preview · 30 秒试听</option><option value="lyria-3.5">lyria-3.5 · 完整歌曲</option></select></label>
        <label>默认输出格式<select v-model="musicForm.outputFormat" aria-label="默认输出格式"><option value="wav">WAV</option><option value="mp3">MP3</option></select><small>普通模式按此格式输出；严格时长使用 WAV，避免 MP3 编码填充影响精度。</small></label>
        <label class="checkbox"><input v-model="musicForm.previewFirst" type="checkbox" />默认试听优先</label>
        <div class="settings-actions"><button class="button primary" @click="saveMusic">保存音乐模型设置</button><button class="button" @click="testMusic">{{ musicForm.provider === 'mock' ? '测试模拟音乐服务' : '保存并测试密钥' }}</button></div>
      </fieldset>
      <button class="button" :disabled="busy || !ready || !settings.data" @click="generateTestMusic">生成测试音乐{{ musicForm.provider === 'mock' ? ' · 模拟' : ' · 可能收费' }}</button>
      <p class="status-text">当前连接状态：{{ settings.data?.music.status === 'connected' ? '已连接' : settings.data?.music.status === 'error' ? '连接失败' : '未测试' }}</p><p class="muted">{{ settings.data?.music.message }}</p>
      <p v-if="isSongApi" class="small-note">测试密钥只查询一条生成记录，不创建音乐。正式 Key 的每次生成（包括测试音乐、30 秒试听）均按 SongAPI 正常扣费；保存第一个版本，第二个请在 SongAPI 控制台查看。目标时长为提示要求，开启“严格时长”后由本机裁剪或补齐。</p>
      <p v-else-if="isElevenLabs" class="small-note">测试密钥调用音乐编排接口，不生成音频、不消耗音乐积分。生成测试音乐为 30 秒；试听和完整歌曲使用 music_v1，实际生成会消耗额度。提示词含歌词与时间轴最多 4100 字符。</p>
      <p v-else class="small-note">测试密钥仅查询所选模型信息，不生成音乐。生成测试音乐会创建独立项目，并使用 30 秒试听模型；真实模式按服务商计费。</p>
    </section>
  </div>
</template>
