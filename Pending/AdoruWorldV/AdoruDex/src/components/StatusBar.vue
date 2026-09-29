<script setup>
import { useMediaStore } from '@/stores/mediaStore'

const store = useMediaStore()
</script>

<template>
  <footer class="bg-dex-darker text-dex-lighter border-t-4 border-dex-dark mt-auto">
    <div class="max-w-5xl mx-auto px-4 py-2 flex items-center justify-between text-xs">
      <div class="flex items-center gap-4 pixel-text">
        <span class="text-dex-amber">📟</span>
        <span>AdoruDex v1.0</span>
        <span class="hidden sm:inline text-dex-light/40">|</span>
        <span class="hidden sm:inline text-dex-light/50">收录 {{ store.items.length }} 条记录</span>
      </div>
      <div class="flex items-center gap-3 pixel-text">
        <span class="text-dex-amber">★</span>
        <span>均分 {{ store.stats.averageRating }}</span>
        <span class="text-dex-light/40">|</span>
        <span class="animate-blink text-dex-amber">🔴 ONLINE</span>
      </div>
    </div>
  </footer>
</template>
