<script setup>
import { computed } from 'vue'
import { useRoute, useRouter } from 'vue-router'
import { useMediaStore } from '@/stores/mediaStore'
import GbaFrame from '@/components/GbaFrame.vue'

const route = useRoute()
const router = useRouter()
const store = useMediaStore()

const item = computed(() => store.getItemById(route.params.id))

const typeLabel = computed(() => {
  return item.value ? store.typeLabels[item.value.type] : ''
})

function getRatingStars(rating) {
  if (!rating) return '☆☆☆☆☆☆☆☆☆☆'
  return '★'.repeat(rating) + '☆'.repeat(10 - rating)
}

function getStatusLabel() {
  if (!item.value) return ''
  return store.statusMap[item.value.type]?.[item.value.status] || item.value.status
}

function statusStyle(status) {
  const map = {
    completed: 'text-dex-green bg-dex-green/10',
    watching: 'text-dex-blue bg-dex-blue/10',
    playing: 'text-dex-blue bg-dex-blue/10',
    plan: 'text-dex-gray bg-dex-gray/10',
    dropped: 'text-dex-red bg-dex-red/10',
    paused: 'text-dex-amber bg-dex-amber/10'
  }
  return map[status] || 'text-dex-gray bg-dex-gray/10'
}

function goBack() {
  router.back()
}

const dexImageMap = {
  anime: '📺',
  movie: '🎬',
  game: '🎮'
}
</script>

<template>
  <div class="animate-slide-in space-y-4">
    <button
      @click="goBack"
      class="text-sm text-dex-blue font-bold flex items-center gap-1 mb-2 pixel-text hover:text-dex-blue-dark transition-colors"
    >
      ◀ 返回列表
    </button>

    <div v-if="item">
      <GbaFrame :title="`No.${String(item.id).padStart(3, '0')}  ${typeLabel}图鉴`" variant="dex">
        <div class="space-y-5">
          <div class="flex items-start gap-4">
            <div class="w-24 h-24 shrink-0 bg-dex-bg border-4 border-dex-dark pixel-shadow-sm flex items-center justify-center text-5xl relative">
              <span>{{ dexImageMap[item.type] }}</span>
              <span class="absolute bottom-0 right-0 bg-dex-dark text-dex-amber text-xs px-1.5 py-0 font-pokemon">
                {{ String(item.id).padStart(3, '0') }}
              </span>
            </div>
            <div class="flex-1 min-w-0">
              <h2 class="text-xl font-bold text-dex-darker font-pokemon pixel-text">{{ item.title }}</h2>
              <p v-if="item.titleEn" class="text-sm text-dex-gray-dark pixel-text mt-0.5 opacity-70">{{ item.titleEn }}</p>
              <div class="flex flex-wrap items-center gap-3 mt-2.5">
                <span :class="['text-xs font-bold px-2 py-0.5 border border-black/10 pixel-text', statusStyle(item.status)]">
                  {{ getStatusLabel() }}
                </span>
                <span class="text-xs text-dex-gray-dark font-pixel">{{ item.year }}</span>
                <span v-if="item.platform" class="text-xs bg-dex-purple-dark text-white px-2 py-0.5 font-pixel border border-black/20">
                  {{ item.platform }}
                </span>
              </div>
            </div>
          </div>

          <div class="border-t-2 border-dex-dark/15 pt-4 space-y-4">
            <div class="bg-dex-bg border-2 border-dex-dark/20 p-3 pixel-shadow-sm">
              <div class="text-xs font-bold text-dex-gray-dark pixel-text mb-1.5">◆ 评分</div>
              <div class="text-dex-amber text-lg font-pokemon">
                {{ getRatingStars(item.rating) }}
              </div>
              <div class="text-sm text-dex-darker font-bold mt-0.5 pixel-text">{{ item.rating }}/10</div>
            </div>

            <div v-if="item.tags && item.tags.length > 0" class="bg-dex-window/50 p-3 border-2 border-dex-dark/10">
              <div class="text-xs font-bold text-dex-gray-dark pixel-text mb-2">◆ 标签</div>
              <div class="flex flex-wrap gap-1.5">
                <span
                  v-for="tag in item.tags"
                  :key="tag"
                  class="text-xs px-2 py-0.5 bg-dex-bg border-2 border-dex-dark/20 text-dex-darker font-pixel"
                >
                  #{{ tag }}
                </span>
              </div>
            </div>

            <div v-if="item.notes" class="bg-dex-bg border-2 border-dex-dark/20 p-3 pixel-shadow-sm">
              <div class="text-xs font-bold text-dex-gray-dark pixel-text mb-1.5">◆ 笔记</div>
              <p class="text-sm text-dex-darker leading-relaxed pixel-text">
                {{ item.notes }}
              </p>
            </div>

            <div class="flex flex-wrap gap-x-6 gap-y-1 text-xs text-dex-gray-dark pixel-text">
              <div>
                <span class="font-bold">收录日期：</span>{{ item.date }}
              </div>
              <div v-if="item.episodes">
                <span class="font-bold">总集数：</span>{{ item.episodes }}
              </div>
              <div v-if="item.progress">
                <span class="font-bold">进度：</span>{{ item.progress }}/{{ item.episodes }}
              </div>
            </div>
          </div>
        </div>
      </GbaFrame>
    </div>

    <div v-else class="text-center py-12 bg-dex-window border-4 border-dex-dark pixel-shadow-sm">
      <p class="text-4xl mb-3">❓</p>
      <p class="text-sm text-dex-gray-dark pixel-text">未找到该条目</p>
      <button @click="goBack" class="mt-3 text-sm text-dex-blue font-bold pixel-text hover:text-dex-blue-dark transition-colors">
        ◀ 返回
      </button>
    </div>
  </div>
</template>
