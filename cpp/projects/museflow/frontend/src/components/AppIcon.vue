<script setup lang="ts">
defineProps<{ name: string }>()
const paths: Record<string, string> = {
  studio: 'M4 19V9m5 10V5m5 14V11m5 8V3',
  projects: 'M3 7V5h6l2 2h10v13H3V7Z',
  library: 'M9 18V5l11-2v13M9 7l11-2M9 18c0 4-6 4-6 1s6-4 6-1m11-2c0 4-6 4-6 1s6-4 6-1',
  compare: 'M6 3v18M18 3v18M3 8h6m6 8h6M6 8l12 8',
  inspiration: 'M9 18h6m-5 3h4M8 14a7 7 0 1 1 8 0l-1 2H9l-1-2Z',
  settings: 'M4 7h16M4 17h16M8 4v6m8 4v6',
  about: 'M12 8h.01M12 11v6M22 12a10 10 0 1 1-20 0 10 10 0 0 1 20 0',
  plus: 'M12 5v14M5 12h14',
  arrow: 'M5 12h14m-5-5 5 5-5 5',
  sparkle: 'm12 3 2.5 6.5L21 12l-6.5 2.5L12 21l-2.5-6.5L3 12l6.5-2.5L12 3Z',
}
</script>
<template><svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.65" stroke-linecap="round" stroke-linejoin="round" aria-hidden="true"><path :d="paths[name] || paths.studio" /></svg></template>
