import { createApp } from 'vue'
import { createPinia } from 'pinia'
import { createRouter, createWebHistory } from 'vue-router'
import App from './App.vue'
import StudioView from './views/StudioView.vue'
import ProjectsView from './views/ProjectsView.vue'
import LibraryView from './views/LibraryView.vue'
import SettingsView from './views/SettingsView.vue'
import CompareView from './views/CompareView.vue'
import RoadmapView from './views/RoadmapView.vue'
import AboutView from './views/AboutView.vue'
import './styles/main.css'

const router = createRouter({
  history: createWebHistory(), routes: [
    { path: '/', redirect: '/studio' },
    { path: '/studio', component: StudioView, meta: { title: '创作台' } },
    { path: '/projects', component: ProjectsView, meta: { title: '音乐项目' } },
    { path: '/library', component: LibraryView, meta: { title: '作品库' } },
    { path: '/settings', component: SettingsView, meta: { title: '模型设置' } },
    { path: '/compare', component: CompareView, meta: { title: '版本对比' } },
    { path: '/inspiration', component: RoadmapView, meta: { title: '灵感素材' } },
    { path: '/about', component: AboutView, meta: { title: '关于' } },
    { path: '/:pathMatch(.*)*', redirect: '/studio' },
  ],
})
createApp(App).use(createPinia()).use(router).mount('#app')

window.addEventListener('beforeunload', event => { if ((window as Window & { __museflowUnsaved?: boolean }).__museflowUnsaved) { event.preventDefault(); event.returnValue = '' } })
