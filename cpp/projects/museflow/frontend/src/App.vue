<script setup lang="ts">
import { onMounted } from 'vue'
import { useRoute } from 'vue-router'
import { useSettings } from './stores/settings'
import AppIcon from './components/AppIcon.vue'
const route = useRoute()
const settings = useSettings()
onMounted(settings.load)
const nav = [ ['studio', '创作台'], ['projects', '音乐项目'], ['library', '作品库'], ['compare', '版本对比'], ['inspiration', '灵感素材'] ]
</script>
<template>
  <div class="app-shell">
    <aside class="sidebar">
      <RouterLink to="/studio" class="brand"><span class="brand-mark">m</span><span>MuseFlow<small>让故事有自己的声音</small></span></RouterLink>
      <div class="workspace-label">个人工作空间 <span>LOCAL</span></div>
      <nav aria-label="主导航">
        <RouterLink v-for="[id, label] in nav" :key="id" :to="`/${id}`"><AppIcon :name="id!" />{{ label }}<span v-if="id === 'inspiration'" class="nav-soon">待开发</span></RouterLink>
      </nav>
      <div class="sidebar-bottom">
        <nav><RouterLink to="/settings"><AppIcon name="settings" />模型设置</RouterLink><RouterLink to="/about"><AppIcon name="about" />关于</RouterLink></nav>
        <div class="local-card"><i :class="{ offline: !settings.connected }"></i><div>{{ settings.connected ? '本地服务已连接' : '本地服务未连接' }}<small>设计稿与音频保存在本机</small></div></div>
        <p class="sidebar-version">MuseFlow · v0.4.0</p>
      </div>
    </aside>
    <div class="main-shell">
      <header class="topbar"><div>工作空间 <span>/</span> <strong>{{ route.meta.title }}</strong></div><span class="badge mock">{{ settings.modeLabel }}</span></header>
      <main><RouterView /></main>
    </div>
  </div>
</template>
