<script setup>
import { computed } from 'vue'
import { useRoute } from 'vue-router'
import NavMenu from '@/components/NavMenu.vue'
import StatusBar from '@/components/StatusBar.vue'

const route = useRoute()
const pageTitle = computed(() => {
  const map = {
    home: 'AdoruDex',
    anime: '动画档案馆',
    movie: '电影档案馆',
    game: '游戏档案馆',
    stats: '统计面板',
    'anime-detail': '动画详情',
    'movie-detail': '电影详情',
    'game-detail': '游戏详情'
  }
  return map[route.name] || 'AdoruDex'
})
</script>

<template>
  <div class="min-h-screen flex flex-col font-pixel text-dex-darker">
    <header class="bg-dex-dark text-dex-lighter pixel-shadow relative z-10">
      <div class="max-w-5xl mx-auto px-4 py-3 flex items-center justify-between">
        <div class="flex items-center gap-3">
          <span class="text-2xl select-none animate-pulse-glow">🔴</span>
          <div class="pixel-text">
            <h1 class="text-lg font-bold tracking-wider font-pokemon text-dex-amber">
              {{ pageTitle }}
            </h1>
            <p class="text-xs text-dex-light/60">
              ▼ POKéDEX ARCHIVE — 数字档案馆 v1.0
            </p>
          </div>
        </div>
        <div class="hidden sm:flex items-center gap-2 text-xs text-dex-light/50 pixel-text">
          <span class="text-dex-amber">★</span>
          <span>A 确定</span>
          <span class="ml-2 text-dex-red">●</span>
          <span>B 返回</span>
        </div>
      </div>
    </header>

    <NavMenu />

    <main class="flex-1 max-w-5xl mx-auto w-full px-4 py-6">
      <slot />
    </main>

    <StatusBar />
  </div>
</template>
