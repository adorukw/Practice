<script setup>
import { computed } from 'vue'
import { useRoute, useRouter } from 'vue-router'

const route = useRoute()
const router = useRouter()

const navItems = [
  { name: '首页', route: 'home', icon: '🏠', activeOn: ['home'], accent: 'bg-dex-amber-dark border-dex-amber' },
  { name: '动画', route: 'anime', icon: '📺', activeOn: ['anime', 'anime-detail'], accent: 'bg-dex-blue border-dex-blue-dark' },
  { name: '电影', route: 'movie', icon: '🎬', activeOn: ['movie', 'movie-detail'], accent: 'bg-dex-red border-dex-red-dark' },
  { name: '游戏', route: 'game', icon: '🎮', activeOn: ['game', 'game-detail'], accent: 'bg-dex-green border-dex-green-dark' },
  { name: '统计', route: 'stats', icon: '📊', activeOn: ['stats'], accent: 'bg-dex-purple border-dex-purple-dark' }
]

function isActive(item) {
  return item.activeOn.includes(route.name)
}
</script>

<template>
  <nav class="bg-dex-dark pixel-shadow">
    <div class="max-w-5xl mx-auto flex">
      <button
        v-for="item in navItems"
        :key="item.route"
        @click="router.push({ name: item.route })"
        :class="[
          'flex-1 py-2.5 text-center font-bold text-sm tracking-wider transition-all duration-100 border-r-2 border-dex-darker/40 last:border-r-0',
          isActive(item)
            ? `${item.accent} text-white shadow-[inset_0_2px_4px_rgba(0,0,0,0.3)]`
            : 'text-dex-lighter/70 hover:bg-dex-darker/30 hover:text-dex-lighter'
        ]"
      >
        <span class="mr-1">{{ item.icon }}</span>
        <span class="hidden sm:inline font-pokemon text-xs">{{ item.name }}</span>
      </button>
    </div>
  </nav>
</template>
