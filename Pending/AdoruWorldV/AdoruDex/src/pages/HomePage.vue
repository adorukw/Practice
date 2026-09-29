<script setup>
import { computed } from 'vue'
import { useRouter } from 'vue-router'
import { useMediaStore } from '@/stores/mediaStore'
import GbaFrame from '@/components/GbaFrame.vue'
import MediaCard from '@/components/MediaCard.vue'

const router = useRouter()
const store = useMediaStore()

const sortedByDate = computed(() =>
  [...store.items].sort((a, b) => new Date(b.date) - new Date(a.date)).slice(0, 5)
)

const sortedByRating = computed(() =>
  [...store.items].filter(i => i.rating).sort((a, b) => b.rating - a.rating).slice(0, 5)
)

const navShortcuts = [
  { type: 'anime', label: '动画', icon: '📺', bg: 'bg-dex-blue', border: 'border-dex-blue-dark', route: 'anime' },
  { type: 'movie', label: '电影', icon: '🎬', bg: 'bg-dex-red', border: 'border-dex-red-dark', route: 'movie' },
  { type: 'game', label: '游戏', icon: '🎮', bg: 'bg-dex-green', border: 'border-dex-green-dark', route: 'game' }
]

function typeCount(type) {
  return store.stats.byType[type] || 0
}

function navigateToItem(item) {
  router.push({ name: `${item.type}-detail`, params: { id: item.id } })
}
</script>

<template>
  <div class="animate-slide-in space-y-6">
    <GbaFrame title="欢迎来到 AdoruDex 数字档案馆">
      <div class="space-y-3">
        <p class="text-sm leading-relaxed text-dex-gray-dark pixel-text">
          ▸ 这里是你私人的数字档案中心，像训练师记录图鉴一样，收藏你看过的每一部动画、电影和玩过的游戏。
        </p>
        <p class="text-sm leading-relaxed text-dex-gray-dark pixel-text">
          ▸ 每种类型的档案都有独立的图鉴编号，记录你的评分、状态与笔记。
        </p>
        <div class="flex gap-3 mt-4">
          <div
            v-for="shortcut in navShortcuts"
            :key="shortcut.type"
            @click="router.push({ name: shortcut.route })"
            :class="[
              'flex-1 py-3 text-center font-bold text-sm cursor-pointer border-2 text-white transition-all',
              'hover:-translate-y-0.5 pixel-shadow-sm',
              shortcut.bg, shortcut.border
            ]"
          >
            <div class="text-xl mb-1">{{ shortcut.icon }}</div>
            <div class="pixel-text text-xs">{{ shortcut.label }}</div>
            <div class="text-xs mt-0.5 opacity-80 font-pixel">×{{ typeCount(shortcut.type) }}</div>
          </div>
        </div>
      </div>
    </GbaFrame>

    <div class="grid grid-cols-1 md:grid-cols-2 gap-6">
      <GbaFrame title="最近收录">
        <div class="space-y-2">
          <MediaCard
            v-for="item in sortedByDate"
            :key="item.id"
            :item="item"
            @click="navigateToItem"
          />
          <p v-if="sortedByDate.length === 0" class="text-sm text-dex-gray-dark text-center py-4 pixel-text">
            —— 还没有收录任何记录，去探索吧！ ——
          </p>
        </div>
      </GbaFrame>

      <GbaFrame title="最高评分">
        <div class="space-y-2">
          <MediaCard
            v-for="item in sortedByRating"
            :key="item.id"
            :item="item"
            @click="navigateToItem"
          />
          <p v-if="sortedByRating.length === 0" class="text-sm text-dex-gray-dark text-center py-4 pixel-text">
            —— 还没有评分记录 ——
          </p>
        </div>
      </GbaFrame>
    </div>

    <GbaFrame title="总览数据">
      <div class="grid grid-cols-2 sm:grid-cols-4 gap-4 text-center">
        <div class="bg-dex-bg border-2 border-dex-dark/30 p-3 pixel-shadow-sm">
          <div class="text-2xl font-bold text-dex-blue-dark font-pokemon pixel-text">{{ store.stats.total }}</div>
          <div class="text-xs text-dex-gray-dark mt-1 pixel-text">◆ 总收录</div>
        </div>
        <div class="bg-dex-bg border-2 border-dex-dark/30 p-3 pixel-shadow-sm">
          <div class="text-2xl font-bold text-dex-amber-dark font-pokemon pixel-text">{{ store.stats.averageRating }}</div>
          <div class="text-xs text-dex-gray-dark mt-1 pixel-text">◆ 平均评分</div>
        </div>
        <div class="bg-dex-bg border-2 border-dex-dark/30 p-3 pixel-shadow-sm">
          <div class="text-2xl font-bold text-dex-green-dark font-pokemon pixel-text">{{ store.stats.byStatus.completed || 0 }}</div>
          <div class="text-xs text-dex-gray-dark mt-1 pixel-text">◆ 已完成</div>
        </div>
        <div class="bg-dex-bg border-2 border-dex-dark/30 p-3 pixel-shadow-sm">
          <div class="text-2xl font-bold text-dex-purple-dark font-pokemon pixel-text">{{ (store.stats.byStatus.watching || 0) + (store.stats.byStatus.playing || 0) }}</div>
          <div class="text-xs text-dex-gray-dark mt-1 pixel-text">◆ 进行中</div>
        </div>
      </div>
    </GbaFrame>
  </div>
</template>
