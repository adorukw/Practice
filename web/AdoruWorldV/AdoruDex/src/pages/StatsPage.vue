<script setup>
import { computed } from 'vue'
import { useRouter } from 'vue-router'
import { useMediaStore } from '@/stores/mediaStore'
import GbaFrame from '@/components/GbaFrame.vue'

const router = useRouter()
const store = useMediaStore()

const statusDist = computed(() => {
  const result = []
  const statuses = [
    { key: 'completed', label: '已完成/已通关', bar: 'bg-dex-green' },
    { key: 'watching', label: '在看/在玩', bar: 'bg-dex-blue' },
    { key: 'plan', label: '想看/想玩', bar: 'bg-dex-gray' },
    { key: 'dropped', label: '弃了', bar: 'bg-dex-red' },
    { key: 'paused', label: '暂停', bar: 'bg-dex-amber' }
  ]
  for (const s of statuses) {
    const count = store.stats.byStatus[s.key] || 0
    if (count > 0) {
      result.push({ ...s, count, pct: ((count / store.stats.total) * 100).toFixed(1) })
    }
  }
  return result
})

const typeItems = computed(() => [
  { type: 'anime', label: '动画', icon: '📺', bar: 'bg-dex-blue' },
  { type: 'movie', label: '电影', icon: '🎬', bar: 'bg-dex-red' },
  { type: 'game', label: '游戏', icon: '🎮', bar: 'bg-dex-green' }
])

function typeCount(type) {
  return store.stats.byType[type] || 0
}

const topRated = computed(() =>
  [...store.items].filter(i => i.rating).sort((a, b) => b.rating - a.rating).slice(0, 5)
)
</script>

<template>
  <div class="animate-slide-in space-y-6">
    <GbaFrame title="统计面板">
      <div class="grid grid-cols-2 sm:grid-cols-4 gap-4 text-center">
        <div class="bg-dex-bg border-2 border-dex-dark/30 p-3 pixel-shadow-sm">
          <div class="text-2xl font-bold text-dex-blue-dark font-pokemon pixel-text">{{ store.stats.total }}</div>
          <div class="text-xs text-dex-gray-dark mt-1 pixel-text">总收录</div>
        </div>
        <div class="bg-dex-bg border-2 border-dex-dark/30 p-3 pixel-shadow-sm">
          <div class="text-2xl font-bold text-dex-amber-dark font-pokemon pixel-text">{{ store.stats.averageRating }}</div>
          <div class="text-xs text-dex-gray-dark mt-1 pixel-text">平均评分</div>
        </div>
        <div class="bg-dex-bg border-2 border-dex-dark/30 p-3 pixel-shadow-sm">
          <div class="text-2xl font-bold text-dex-green-dark font-pokemon pixel-text">{{ store.stats.byStatus.completed || 0 }}</div>
          <div class="text-xs text-dex-gray-dark mt-1 pixel-text">已完成</div>
        </div>
        <div class="bg-dex-bg border-2 border-dex-dark/30 p-3 pixel-shadow-sm">
          <div class="text-2xl font-bold text-dex-purple-dark font-pokemon pixel-text">{{ (store.stats.byStatus.watching || 0) + (store.stats.byStatus.playing || 0) }}</div>
          <div class="text-xs text-dex-gray-dark mt-1 pixel-text">进行中</div>
        </div>
      </div>
    </GbaFrame>

    <div class="grid grid-cols-1 md:grid-cols-2 gap-6">
      <GbaFrame title="按类型分布">
        <div class="space-y-3">
          <div v-for="t in typeItems" :key="t.type" class="flex items-center gap-3">
            <span class="text-lg w-8 text-center">{{ t.icon }}</span>
            <span class="text-sm font-bold w-12 text-dex-darker pixel-text">{{ t.label }}</span>
            <div class="flex-1 bg-dex-bg border-2 border-dex-dark/30 h-6 relative">
              <div
                :class="['h-full transition-all', t.bar]"
                :style="{ width: Math.max((typeCount(t.type) / store.stats.total * 100), 5) + '%' }"
              />
            </div>
            <span class="text-sm font-bold text-dex-darker w-8 text-right font-pokemon pixel-text">{{ typeCount(t.type) }}</span>
          </div>
        </div>
      </GbaFrame>

      <GbaFrame title="按状态分布">
        <div class="space-y-2.5">
          <div v-for="s in statusDist" :key="s.key" class="flex items-center gap-2">
            <span :class="['w-3 h-3 border border-dex-dark/30 shrink-0', s.bar]" />
            <span class="text-sm text-dex-darker w-20 font-pixel text-xs">{{ s.label }}</span>
            <div class="flex-1 bg-dex-bg border border-dex-dark/20 h-5 relative">
              <div
                :class="['h-full', s.bar]"
                :style="{ width: s.pct + '%' }"
              />
            </div>
            <span class="text-xs text-dex-gray-dark w-10 text-right font-pokemon pixel-text">{{ s.count }}</span>
          </div>
        </div>
      </GbaFrame>
    </div>

    <GbaFrame title="评分 TOP 5">
      <ol class="space-y-1.5">
        <li v-for="(item, idx) in topRated" :key="item.id" class="flex items-center gap-3 text-sm bg-dex-bg border border-dex-dark/15 p-2 pixel-shadow-sm">
          <span class="w-6 text-center font-pokemon font-bold" :class="idx === 0 ? 'text-dex-amber text-base' : 'text-dex-gray-dark'">
            {{ 'No.' + (idx + 1) }}
          </span>
          <span class="text-dex-amber text-xs font-pokemon">{{ '★'.repeat(item.rating) }}</span>
          <span class="text-dex-darker font-bold pixel-text truncate">{{ item.title }}</span>
          <span class="text-xs text-dex-gray-dark ml-auto font-pokemon pixel-text">{{ item.rating }}/10</span>
        </li>
      </ol>
    </GbaFrame>
  </div>
</template>
