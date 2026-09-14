<script setup lang="ts">
import { useStudio } from '../stores/studio'
import { useSettings } from '../stores/settings'
import { purposes, taskLabels } from '../types'
import { splitTags } from '../types/validation'
import TimelineEditor from '../components/TimelineEditor.vue'
import AudioPlayer from '../components/AudioPlayer.vue'
import AppIcon from '../components/AppIcon.vue'
const studio = useStudio()
const settings = useSettings()
function newDraft() {
  if (studio.blueprint && !window.confirm('新建会清空当前编辑区，未保存修改会丢失。继续吗？')) return
  studio.reset()
}
</script>
<template>
  <div class="page-heading"><div><span class="eyebrow">CREATE YOUR SOUNDSCAPE</span><h1>从一个场景，开始一段音乐</h1><p>写下你的故事，为每一个情绪找到声音。</p></div><button class="button" @click="newDraft" :disabled="studio.busy || studio.active"><AppIcon name="plus" />新建创作</button></div>
  <div class="mock-notice"><span class="badge mock">{{ settings.musicIsMock ? '音乐模拟' : settings.musicLabel }}</span><span>{{ settings.textIsMock ? '当前设计稿来自固定模板。' : '创作助手调用已配置的文本模型，可能产生费用。' }} {{ settings.musicIsMock ? '音乐使用内置 8 秒测试音频；严格时长可裁剪或补齐。' : '音乐请求将发送到所选服务商，可能产生费用；生成结果受模型与账户权限影响。' }}</span></div>
  <div v-if="settings.data?.music.provider === 'songapi' && settings.data.music.songapiSandbox" class="mock-notice">当前是 SongAPI 沙箱 Key：生成的是静音示例。试听真实音乐需要 sk_live_ 开头的 Key。</div>
  <div v-if="studio.parentVersionId" class="mock-notice"><strong>分支草稿 · 父版本 {{ studio.parentVersionId.slice(0, 8) }}</strong><span>本次生成会创建子版本；旧音频保持不变。</span><button class="text-button" @click="studio.parentVersionId = ''" :disabled="studio.busy || studio.active">改为独立版本</button></div>
  <div v-if="studio.error" class="error message" role="alert">{{ studio.error }}</div>
  <div v-if="studio.notice" class="success message" role="status">{{ studio.notice }}</div>
  <div v-if="studio.textProgress" class="mock-notice" role="status">{{ studio.textProgress }} 本地服务仍可响应；校验失败最多重试一次。</div>
  <div class="studio-layout">
    <div class="editor-column">
      <fieldset class="editor-fields" :disabled="studio.busy">
      <section class="card">
        <div class="section-heading"><div><span class="eyebrow">01 / THE SCENE</span><h2>你想讲述怎样的故事？</h2></div><span class="subtle-tag">创作起点</span></div>
        <label class="sr-only" for="scene">场景描述</label>
        <textarea id="scene" v-model="studio.scene" class="scene-input" rows="5" maxlength="4000" placeholder="描述画面、人物和情绪变化，也可以写下你想听见的乐器……" @input="studio.blueprint && (studio.blueprint.sceneDescription = ($event.target as HTMLTextAreaElement).value)" />
        <div class="scene-footer"><div class="purpose-control"><label for="purpose">用在</label><select id="purpose" v-model="studio.purpose" @change="studio.blueprint && (studio.blueprint.purpose = studio.purpose)"><option v-for="(label, value) in purposes" :key="value" :value="value">{{ label }}</option></select></div><button class="button primary" @click="studio.generateBlueprint" :disabled="studio.busy || studio.active || !studio.scene.trim()"><AppIcon name="sparkle" />{{ studio.busy ? '处理中…' : 'AI 生成音乐设计稿' }}</button></div>
      </section>
      <template v-if="studio.blueprint">
        <section class="card revision-card"><span class="eyebrow">DIRECT YOUR MUSIC</span><h2>用一句话调整设计稿</h2><label>修改要求<textarea v-model="studio.revisionInstruction" rows="2" maxlength="2000" placeholder="例如：让中段更有紧张感，加入低音弦乐；改为中文人声并补充歌词。" /></label><div class="scene-footer"><span class="muted">{{ settings.textIsMock ? '模拟模式仅记录要求，不执行智能改写。' : '保留未要求修改的参数，重新生成设计稿与提示词。' }}</span><button class="button" @click="studio.reviseBlueprint" :disabled="studio.active || !!studio.errors.length || !studio.revisionInstruction.trim()">AI 修改设计稿</button></div><p class="small-note">修改设计稿不会改变旧音频。确认后重新生成，会保存为独立音乐版本。</p></section>
        <section class="card">
          <div class="section-heading"><div><span class="eyebrow">02 / MUSIC BLUEPRINT</span><h2>把声音调整成你的想象</h2></div><span class="subtle-tag">可自由编辑</span></div>
          <label>作品标题<input v-model="studio.blueprint.title" maxlength="100" /></label>
          <div class="form-grid two">
            <label>曲风<input v-model="studio.blueprint.genre" maxlength="60" /></label>
            <label>子曲风<input v-model="studio.blueprint.subGenre" maxlength="60" /></label>
            <label>情绪<input :value="studio.blueprint.mood.join('、')" @change="studio.blueprint.mood = splitTags(($event.target as HTMLInputElement).value)" /></label>
            <label>乐器<input :value="studio.blueprint.instruments.join('、')" @change="studio.blueprint.instruments = splitTags(($event.target as HTMLInputElement).value)" /></label>
          </div>
          <div class="form-grid three">
            <label>BPM<input v-model.number="studio.blueprint.bpm" type="number" min="40" max="240" /></label>
            <label>调性<input v-model="studio.blueprint.key" maxlength="30" /></label>
            <label>目标时长（秒）<input v-model.number="studio.blueprint.targetDurationSeconds" type="number" min="5" max="600" /></label>
          </div>
          <div class="duration-hint"><label class="checkbox"><input v-model="studio.blueprint.strictDuration" type="checkbox" />严格时长</label><span>开启后使用 FFmpeg 裁剪 / 补静音并淡出，以 WAV 精确保存。试听处理到 30 秒，完整歌曲按目标时长处理。</span></div>
          <div class="form-grid three">
            <label>人声模式<select v-model="studio.blueprint.vocalMode"><option value="instrumental">纯音乐</option><option value="vocal">有人声</option></select></label>
            <label>人声音色<input v-model="studio.blueprint.vocalGender" :disabled="studio.blueprint.vocalMode === 'instrumental'" placeholder="例如：轻柔女声" maxlength="30" /></label>
            <label>歌词语言<input v-model="studio.blueprint.language" :disabled="studio.blueprint.vocalMode === 'instrumental'" maxlength="30" /></label>
          </div>
          <label v-if="studio.blueprint.vocalMode === 'vocal'">歌词<textarea v-model="studio.blueprint.lyrics" rows="5" maxlength="6000" placeholder="写下歌词。模拟音频不包含合成人声。" /></label>
          <label>避免出现的元素<input v-model="studio.blueprint.negativePrompt" maxlength="1200" /></label>
        </section>
        <TimelineEditor v-model="studio.blueprint" />
        <div v-if="studio.errors.length" class="error message" role="alert"><strong>请先修正设计稿</strong><ul><li v-for="error in studio.errors" :key="error">{{ error }}</li></ul></div>
        <section class="card prompt-card">
          <div class="section-heading"><div><span class="eyebrow">PROMPT PREVIEW</span><h2>音乐提示词预览</h2></div><button class="button small" @click="studio.refreshPrompt" :disabled="studio.busy || !!studio.errors.length">校验并刷新</button></div>
          <p class="muted">最近提示词来源：{{ studio.blueprint.promptSource === 'deepseek' ? '创作助手' : '本地规则' }}。未修改参数时保留 AI 提示词；手动编辑后，在保存或校验时按参数刷新。需要 AI 重新编写可使用上方改稿。</p>
          <details><summary>查看最近一次保存 / 校验的提示词</summary><pre>{{ studio.blueprint.finalLyriaPrompt }}</pre></details>
        </section>
      </template>
      <section v-else class="empty-blueprint"><AppIcon name="studio" /><h2>故事准备好了，音乐就有了起点</h2><p>生成设计稿后，在这里调整曲风、乐器与情绪时间轴。</p><div class="workflow"><span>描述场景</span><AppIcon name="arrow" /><span>编辑设计稿</span><AppIcon name="arrow" /><span>生成并保存</span></div></section>
      </fieldset>
    </div>
    <aside class="render-column">
      <section class="card render-card">
        <span class="eyebrow">04 / BRING IT TO LIFE</span><h2>听见你的场景</h2><p class="muted">先试听，再探索更多可能。</p>
        <div class="render-summary"><span>音乐生成服务 <b>{{ settings.musicLabel }}</b></span><span>输出格式 <b>{{ studio.blueprint?.strictDuration ? 'WAV · 严格时长' : (settings.data?.music.outputFormat ?? 'wav').toUpperCase() }}</b></span><span>预计音乐 API 费用 <b :class="{ free: settings.musicIsMock }">{{ settings.musicIsMock ? '¥0.00' : '未估算 · 按服务商计费' }}</b></span></div>
        <p class="small-note">{{ settings.musicIsMock ? '模拟模式不消耗音乐额度，原始音频固定为 8 秒。' : settings.data?.music.provider === 'elevenlabs' ? 'ElevenLabs 试听与完整歌曲均使用 music_v1，按目标时长生成；歌词与段落通过提示词引导，失败不自动重试。' : settings.data?.music.provider === 'songapi' ? 'SongAPI 试听与完整歌曲均按一次生成计费，保存第一个版本。目标时长不保证精确，可开启严格时长；失败不自动重新生成。' : '试听使用 lyria-3-clip-preview；完整歌曲使用 lyria-3.5。真实生成不自动重试，避免重复计费。' }} 试听会将设计稿时间轴按比例缩放到 30 秒。</p>
        <button class="button full-width" :class="{ primary: settings.data?.music.previewFirst || (settings.data?.music.provider === 'lyria' && settings.data.music.model === 'lyria-3-clip-preview') }" :disabled="!studio.blueprint || studio.busy || studio.active || !!studio.errors.length" @click="studio.generateMusic('preview')"><AppIcon name="library" />生成 30 秒试听{{ settings.musicIsMock ? ' · 模拟' : '' }}</button>
        <button class="button full-width" :class="{ primary: !settings.data?.music.previewFirst && (settings.data?.music.provider !== 'lyria' || settings.data.music.model === 'lyria-3.5') }" :disabled="!studio.blueprint || studio.busy || studio.active || !!studio.errors.length" @click="studio.generateMusic('full')">生成完整歌曲{{ settings.musicIsMock ? ' · 模拟' : '' }}</button>
        <button class="text-button full-width" :disabled="!studio.blueprint || studio.busy || !!studio.errors.length" @click="studio.save">保存音乐设计稿</button>
        <p class="small-note centered">生成前自动保存设计稿，完成后自动保存作品。</p>
        <div v-if="studio.task" class="task-panel" aria-live="polite"><div><strong>{{ studio.task.provider === 'mock' && studio.task.status === 'generating' ? '模拟音乐生成' : taskLabels[studio.task.status] }}</strong><span>{{ studio.task.progress }}%</span></div><progress max="100" :value="studio.task.progress" /><small>任务 {{ studio.task.id.slice(0, 8) }} · {{ studio.task.mode === 'preview' ? '试听' : '完整歌曲' }}</small></div>
      </section>
      <section v-if="studio.task?.status === 'completed'" class="card result-card"><span class="eyebrow">LATEST TAKE</span><h2>这一次的声音</h2><AudioPlayer :generation-id="studio.task.id" :title="studio.task.blueprint.title" :target="studio.task.modelTargetDurationSeconds" :actual="studio.task.actualDurationSeconds ?? 0" :format="studio.task.format" :provider="studio.task.provider" :source-duration="studio.task.sourceDurationSeconds" :strict="studio.task.strictDurationApplied" /><details v-if="studio.task.songText?.length"><summary>{{ studio.task.provider === 'songapi' ? 'SongAPI 返回的歌词' : 'Lyria 返回的歌词与歌曲结构' }}</summary><pre>{{ studio.task.songText.join('\n\n') }}</pre></details><RouterLink class="text-link" to="/library">在作品库查看所有生成记录 →</RouterLink></section>
      <div class="aside-note"><AppIcon name="projects" /><div><strong>留住每一次灵感</strong><p>每次生成都创建独立作品，保存当时的参数与提示词。音频不会覆盖。</p></div></div>
    </aside>
  </div>
</template>
