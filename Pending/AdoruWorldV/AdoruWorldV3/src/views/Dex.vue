<script setup lang="ts">
import { ref, computed, onMounted } from 'vue'
import Layout from '@/components/layout/Layout.vue'
import { dexCategories, dexStatuses, getCategoryInfo, getStatusInfo } from '@/data/dex'
import { useBlogStore } from '@/stores/blog'
import type { DexCategory, DexStatus, DexEntry } from '@/types'

const store = useBlogStore()

const selectedCategory = ref<DexCategory | 'all'>('all')
const selectedStatus = ref<DexStatus | 'all'>('all')
const searchQuery = ref('')
const viewMode = ref<'grid' | 'list'>('grid')

const dexEntries = computed(() => store.allDexEntries)

const dexStats = computed(() => {
  const entries = store.allDexEntries
  const byCategory: any = {}
  const byStatus: any = {}
  dexCategories.forEach(c => { byCategory[c.id] = 0 })
  dexStatuses.forEach(s => { byStatus[s.id] = 0 })
  entries.forEach(e => {
    if (byCategory[e.category] !== undefined) byCategory[e.category]++
    if (byStatus[e.status] !== undefined) byStatus[e.status]++
  })
  const rated = entries.filter(e => e.rating > 0)
  const averageRating = rated.length ? Number((rated.reduce((a, b) => a + b.rating, 0) / rated.length).toFixed(1)) : 0
  return { total: entries.length, byCategory, byStatus, averageRating }
})

const filteredEntries = computed(() => {
  let result = dexEntries.value

  if (selectedCategory.value !== 'all') {
    result = result.filter(e => e.category === selectedCategory.value)
  }

  if (selectedStatus.value !== 'all') {
    result = result.filter(e => e.status === selectedStatus.value)
  }

  if (searchQuery.value) {
    const query = searchQuery.value.toLowerCase()
    result = result.filter(e =>
      e.title.toLowerCase().includes(query) ||
      e.originalTitle?.toLowerCase().includes(query) ||
      e.tags.some(t => t.toLowerCase().includes(query))
    )
  }

  return result
})

const getRatingStars = (rating: number) => {
  if (rating === 0) return '☆☆☆☆☆☆☆☆☆☆'
  return '★'.repeat(rating) + '☆'.repeat(10 - rating)
}

const getProgressPercentage = (entry: DexEntry) => {
  if (entry.total === 0) return 0
  return Math.round((entry.progress / entry.total) * 100)
}
</script>

<template>
  <Layout>
    <section class="relative py-16 overflow-hidden">
      <div class="absolute inset-0 sky-gradient"></div>
      
      <div class="absolute inset-0 overflow-hidden pointer-events-none">
        <div class="cloud cloud-1 absolute w-24 h-12 bg-white rounded-full"></div>
        <div class="cloud cloud-2 absolute w-32 h-16 bg-white rounded-full"></div>
        <div class="cloud cloud-3 absolute w-20 h-10 bg-white rounded-full"></div>
      </div>
      
      <div class="max-w-6xl mx-auto px-4 relative z-10">
        <div class="text-center">
          <div class="inline-block mb-4">
            <div class="w-20 h-20 bg-pokemon-red rounded-lg border-4 border-pokemon-black mx-auto mb-4
                        flex items-center justify-center shadow-lg relative overflow-hidden">
              <div class="absolute inset-0 bg-gradient-to-b from-white/30 to-transparent"></div>
              <span class="text-4xl relative z-10">📖</span>
            </div>
          </div>
          <h1 class="pixel-text text-2xl md:text-3xl text-pokemon-black mb-4 drop-shadow-sm">
            图鉴 Dex
          </h1>
          <p class="text-pokemon-dark-gray max-w-xl mx-auto">
            记录我看过的动画、电影、剧集，玩过的游戏，读过的书籍，听过的音乐
          </p>
        </div>
      </div>
    </section>

    <section class="py-6 border-y-4 border-pokemon-black overflow-hidden relative">
      <div class="absolute inset-0 gold-pattern"></div>
      <div class="max-w-6xl mx-auto px-4 relative z-10">
        <div class="grid grid-cols-3 md:grid-cols-6 gap-3">
          <div
            v-for="cat in dexCategories"
            :key="cat.id"
            class="text-center p-3 cursor-pointer transition-all border-4 rounded-lg"
            :class="selectedCategory === cat.id 
              ? 'border-pokemon-black shadow-md' 
              : 'border-transparent hover:border-pokemon-black/50'"
            :style="{ backgroundColor: selectedCategory === cat.id ? cat.bgColor : 'rgba(255,255,255,0.8)' }"
            @click="selectedCategory = selectedCategory === cat.id ? 'all' : cat.id"
          >
            <div class="text-2xl mb-1">{{ cat.icon }}</div>
            <div class="pixel-text text-base" :style="{ color: cat.color }">{{ cat.name }}</div>
            <div class="text-xs text-pokemon-dark-gray mt-1">{{ dexStats.byCategory[cat.id] }}</div>
          </div>
        </div>
      </div>
    </section>

    <section class="py-8">
      <div class="max-w-6xl mx-auto px-4">
        <div class="flex flex-wrap items-center justify-between gap-4 mb-6">
          <div class="flex items-center gap-4">
            <div class="relative">
              <input
                v-model="searchQuery"
                type="text"
                placeholder="搜索图鉴..."
                class="pixel-input w-48 text-sm py-2 pr-10"
              />
              <svg class="w-5 h-5 absolute right-3 top-1/2 -translate-y-1/2 text-pokemon-dark-gray" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M21 21l-6-6m2-5a7 7 0 11-14 0 7 7 0 0114 0z" />
              </svg>
            </div>
            
            <select
              v-model="selectedStatus"
              class="pixel-input text-sm py-2"
            >
              <option value="all">全部状态</option>
              <option v-for="status in dexStatuses" :key="status.id" :value="status.id">
                {{ status.icon }} {{ status.name }}
              </option>
            </select>
          </div>

          <div class="flex items-center gap-2">
            <button
              class="p-2 border-4 border-pokemon-black transition-all"
              :class="viewMode === 'grid' ? 'bg-gold' : 'bg-white hover:bg-pokemon-gray/50'"
              @click="viewMode = 'grid'"
            >
              <svg class="w-5 h-5" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M4 6a2 2 0 012-2h2a2 2 0 012 2v2a2 2 0 01-2 2H6a2 2 0 01-2-2V6zM14 6a2 2 0 012-2h2a2 2 0 012 2v2a2 2 0 01-2 2h-2a2 2 0 01-2-2V6zM4 16a2 2 0 012-2h2a2 2 0 012 2v2a2 2 0 01-2 2H6a2 2 0 01-2-2v-2zM14 16a2 2 0 012-2h2a2 2 0 012 2v2a2 2 0 01-2 2h-2a2 2 0 01-2-2v-2z" />
              </svg>
            </button>
            <button
              class="p-2 border-4 border-pokemon-black transition-all"
              :class="viewMode === 'list' ? 'bg-gold' : 'bg-white hover:bg-pokemon-gray/50'"
              @click="viewMode = 'list'"
            >
              <svg class="w-5 h-5" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M4 6h16M4 10h16M4 14h16M4 18h16" />
              </svg>
            </button>
          </div>
        </div>

        <div class="flex items-center gap-4 mb-6 p-4 bg-white/80 rounded-lg border-4 border-pokemon-black">
          <div class="flex items-center gap-2">
            <span class="pixel-text text-base text-pokemon-black">总计:</span>
            <span class="pixel-text text-lg text-sky-dark">{{ dexStats.total }}</span>
          </div>
          <div class="w-px h-6 bg-pokemon-black/20"></div>
          <div class="flex items-center gap-2">
            <span class="pixel-text text-base text-pokemon-black">平均评分:</span>
            <span class="pixel-text text-lg text-gold-dark">{{ dexStats.averageRating }}</span>
          </div>
          <div class="w-px h-6 bg-pokemon-black/20"></div>
          <div class="flex items-center gap-2">
            <span class="pixel-text text-base text-pokemon-black">已完成:</span>
            <span class="pixel-text text-lg text-grass-dark">{{ dexStats.byStatus.completed }}</span>
          </div>
        </div>

        <div v-if="viewMode === 'grid'" class="grid grid-cols-2 sm:grid-cols-3 md:grid-cols-4 lg:grid-cols-5 gap-4">
          <router-link
            v-for="(entry, index) in filteredEntries"
            :key="entry.id"
            :to="`/dex/${entry.slug}`"
            class="group"
          >
            <div class="pixel-card overflow-hidden">
              <div class="relative aspect-[3/4] overflow-hidden">
                <img
                  :src="entry.cover"
                  :alt="entry.title"
                  class="w-full h-full object-cover transition-transform duration-300 group-hover:scale-110"
                  style="image-rendering: auto;"
                />
                <div class="absolute inset-0 bg-gradient-to-t from-black/60 via-transparent to-transparent"></div>
                
                <div class="absolute top-2 left-2">
                  <span
                    class="text-xs px-2 py-1 border-2 border-pokemon-black text-white"
                    :style="{ backgroundColor: getCategoryInfo(entry.category)?.color }"
                  >
                    {{ getCategoryInfo(entry.category)?.icon }}
                  </span>
                </div>
                
                <div class="absolute top-2 right-2">
                  <span
                    class="text-xs px-2 py-1 border-2 border-pokemon-black text-white"
                    :style="{ backgroundColor: getStatusInfo(entry.status)?.color }"
                  >
                    {{ getStatusInfo(entry.status)?.icon }}
                  </span>
                </div>
                
                <div class="absolute bottom-0 left-0 right-0 p-3">
                  <div class="pixel-text text-base text-white mb-1 line-clamp-2">
                    #{{ String(index + 1).padStart(3, '0') }} {{ entry.title }}
                  </div>
                  <div v-if="entry.rating > 0" class="text-xs text-gold-light">
                    {{ getRatingStars(entry.rating) }}
                  </div>
                </div>
              </div>
            </div>
          </router-link>
        </div>

        <div v-else class="space-y-3">
          <router-link
            v-for="(entry, index) in filteredEntries"
            :key="entry.id"
            :to="`/dex/${entry.slug}`"
            class="block"
          >
            <div class="pixel-card p-4 flex gap-4 items-center">
              <div class="w-16 h-20 flex-shrink-0 overflow-hidden rounded border-2 border-pokemon-black">
                <img
                  :src="entry.cover"
                  :alt="entry.title"
                  class="w-full h-full object-cover"
                  style="image-rendering: auto;"
                />
              </div>
              
              <div class="flex-1 min-w-0">
                <div class="flex items-center gap-2 mb-1">
                  <span class="pixel-text text-base text-pokemon-dark-gray">
                    #{{ String(index + 1).padStart(3, '0') }}
                  </span>
                  <h3 class="pixel-text text-base text-pokemon-black truncate">{{ entry.title }}</h3>
                </div>
                <p v-if="entry.originalTitle" class="text-xs text-pokemon-dark-gray mb-2 truncate">
                  {{ entry.originalTitle }}
                </p>
                <div class="flex flex-wrap items-center gap-2">
                  <span
                    class="text-xs px-2 py-0.5 border-2 border-pokemon-black text-white"
                    :style="{ backgroundColor: getCategoryInfo(entry.category)?.color }"
                  >
                    {{ getCategoryInfo(entry.category)?.name }}
                  </span>
                  <span
                    class="text-xs px-2 py-0.5 border-2 border-pokemon-black text-white"
                    :style="{ backgroundColor: getStatusInfo(entry.status)?.color }"
                  >
                    {{ getStatusInfo(entry.status)?.name }}
                  </span>
                  <span v-if="entry.year" class="text-xs text-pokemon-dark-gray">{{ entry.year }}</span>
                </div>
              </div>
              
              <div class="flex-shrink-0 text-right">
                <div v-if="entry.rating > 0" class="text-gold-dark text-sm mb-1">
                  {{ getRatingStars(entry.rating) }}
                </div>
                <div v-if="entry.total > 1" class="text-xs text-pokemon-dark-gray">
                  {{ entry.progress }} / {{ entry.total }}
                </div>
                <div class="w-20 h-2 bg-pokemon-gray rounded-full overflow-hidden border border-pokemon-black mt-1">
                  <div
                    class="h-full transition-all"
                    :style="{ 
                      width: `${getProgressPercentage(entry)}%`,
                      backgroundColor: getCategoryInfo(entry.category)?.color 
                    }"
                  />
                </div>
              </div>
            </div>
          </router-link>
        </div>

        <div v-if="filteredEntries.length === 0" class="text-center py-16">
          <div class="text-6xl mb-4">🔍</div>
          <h2 class="pixel-text text-lg text-pokemon-black mb-4">没有找到记录</h2>
          <p class="text-pokemon-dark-gray">尝试更换筛选条件或搜索其他关键词</p>
        </div>
      </div>
    </section>
  </Layout>
</template>

<style scoped>
.cloud {
  filter: blur(2px);
  box-shadow: 
    0 0 30px rgba(255, 255, 255, 0.6),
    inset 0 -8px 15px rgba(200, 220, 240, 0.4);
}

.cloud::before,
.cloud::after {
  content: '';
  position: absolute;
  background: white;
  border-radius: 50%;
}

.cloud::before {
  width: 50%;
  height: 70%;
  top: 25%;
  left: -15%;
}

.cloud::after {
  width: 45%;
  height: 60%;
  top: 35%;
  right: -10%;
}

@keyframes cloud-drift-1 {
  0% { left: -150px; top: 5%; }
  100% { left: calc(100% + 150px); top: 8%; }
}

@keyframes cloud-drift-2 {
  0% { left: -200px; top: 15%; }
  100% { left: calc(100% + 200px); top: 12%; }
}

@keyframes cloud-drift-3 {
  0% { left: -120px; top: 25%; }
  100% { left: calc(100% + 120px); top: 22%; }
}

.cloud-1 {
  opacity: 0.7;
  animation: cloud-drift-1 35s linear infinite;
}

.cloud-2 {
  opacity: 0.6;
  animation: cloud-drift-2 45s linear infinite;
  animation-delay: -15s;
}

.cloud-3 {
  opacity: 0.75;
  animation: cloud-drift-3 25s linear infinite;
  animation-delay: -8s;
}
</style>
