<script setup lang="ts">
import { computed } from 'vue'
import { useRoute } from 'vue-router'
import Layout from '@/components/layout/Layout.vue'
import { getCategoryInfo, getStatusInfo } from '@/data/dex'
import { useBlogStore } from '@/stores/blog'

const route = useRoute()
const store = useBlogStore()

const entry = computed(() => store.allDexEntries.find(e => e.slug === route.params.slug))

const entryIndex = computed(() => {
  if (!entry.value) return 0
  return store.allDexEntries.findIndex(e => e.id === entry.value!.id) + 1
})

const categoryInfo = computed(() => entry.value ? getCategoryInfo(entry.value.category) : null)
const statusInfo = computed(() => entry.value ? getStatusInfo(entry.value.status) : null)

const getRatingStars = (rating: number) => {
  if (rating === 0) return '☆☆☆☆☆☆☆☆☆☆'
  return '★'.repeat(rating) + '☆'.repeat(10 - rating)
}

const getProgressPercentage = () => {
  if (!entry.value || entry.value.total === 0) return 0
  return Math.round((entry.value.progress / entry.value.total) * 100)
}

const relatedEntries = computed(() => {
  if (!entry.value) return []
  return store.allDexEntries
    .filter(e => 
      e.id !== entry.value!.id && 
      (e.category === entry.value!.category || e.tags.some(t => entry.value!.tags.includes(t)))
    )
    .slice(0, 4)
})
</script>

<template>
  <Layout>
    <template v-if="entry">
      <section class="relative py-8 overflow-hidden">
        <div class="absolute inset-0" :style="{ backgroundColor: categoryInfo?.bgColor }"></div>
        
        <div class="max-w-4xl mx-auto px-4 relative z-10">
          <div class="flex items-center gap-2 mb-4">
            <router-link to="/dex" class="text-pokemon-dark-gray hover:text-pokemon-black transition-colors">
              <svg class="w-6 h-6" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M15 19l-7-7 7-7" />
              </svg>
            </router-link>
            <span class="pixel-text text-sm text-pokemon-dark-gray">返回图鉴</span>
          </div>
        </div>
      </section>

      <section class="py-8">
        <div class="max-w-4xl mx-auto px-4">
          <div class="grid grid-cols-1 md:grid-cols-3 gap-8">
            <div class="md:col-span-1">
              <div class="pixel-card overflow-hidden sticky top-24">
                <div class="relative aspect-[3/4]">
                  <img
                    :src="entry.cover"
                    :alt="entry.title"
                    class="w-full h-full object-cover"
                    style="image-rendering: auto;"
                  />
                  <div class="absolute inset-0 bg-gradient-to-t from-black/30 to-transparent"></div>
                  
                  <div class="absolute top-3 left-3 right-3 flex justify-between">
                    <span
                      class="text-sm px-3 py-1 border-3 border-pokemon-black text-white pixel-text"
                      :style="{ backgroundColor: categoryInfo?.color }"
                    >
                      {{ categoryInfo?.icon }} {{ categoryInfo?.name }}
                    </span>
                  </div>
                  
                  <div class="absolute bottom-3 left-3 right-3">
                    <div class="bg-white/90 backdrop-blur-sm rounded border-3 border-pokemon-black p-2">
                      <div class="pixel-text text-xs text-pokemon-dark-gray">
                        #{{ String(entryIndex).padStart(3, '0') }}
                      </div>
                    </div>
                  </div>
                </div>
              </div>
            </div>

            <div class="md:col-span-2">
              <div class="pixel-card p-6 mb-6">
                <div class="flex items-start justify-between gap-4 mb-4">
                  <div>
                    <h1 class="pixel-text text-xl text-pokemon-black mb-2">{{ entry.title }}</h1>
                    <p v-if="entry.originalTitle" class="text-pokemon-dark-gray">
                      {{ entry.originalTitle }}
                    </p>
                  </div>
                  <span
                    class="flex-shrink-0 text-sm px-4 py-2 border-4 border-pokemon-black text-white pixel-text"
                    :style="{ backgroundColor: statusInfo?.color }"
                  >
                    {{ statusInfo?.icon }} {{ statusInfo?.name }}
                  </span>
                </div>

                <div v-if="entry.rating > 0" class="mb-6">
                  <div class="flex items-center gap-3 mb-2">
                    <span class="pixel-text text-xs text-pokemon-black">评分</span>
                    <span class="pixel-text text-lg text-gold-dark">{{ entry.rating }}/10</span>
                  </div>
                  <div class="text-gold text-lg tracking-wider">
                    {{ getRatingStars(entry.rating) }}
                  </div>
                </div>

                <div v-if="entry.total > 1" class="mb-6">
                  <div class="flex items-center justify-between mb-2">
                    <span class="pixel-text text-xs text-pokemon-black">进度</span>
                    <span class="text-sm text-pokemon-dark-gray">{{ entry.progress }} / {{ entry.total }}</span>
                  </div>
                  <div class="h-4 bg-pokemon-gray rounded-full overflow-hidden border-3 border-pokemon-black">
                    <div
                      class="h-full transition-all"
                      :style="{ 
                        width: `${getProgressPercentage()}%`,
                        backgroundColor: categoryInfo?.color 
                      }"
                    />
                  </div>
                </div>

                <div class="grid grid-cols-2 gap-4 mb-6">
                  <div v-if="entry.creator" class="p-3 bg-pokemon-gray/30 rounded border-2 border-pokemon-black">
                    <div class="pixel-text text-xs text-pokemon-dark-gray mb-1">创作者</div>
                    <div class="text-sm text-pokemon-black">{{ entry.creator }}</div>
                  </div>
                  <div v-if="entry.year" class="p-3 bg-pokemon-gray/30 rounded border-2 border-pokemon-black">
                    <div class="pixel-text text-xs text-pokemon-dark-gray mb-1">年份</div>
                    <div class="text-sm text-pokemon-black">{{ entry.year }}</div>
                  </div>
                  <div v-if="entry.platform" class="p-3 bg-pokemon-gray/30 rounded border-2 border-pokemon-black">
                    <div class="pixel-text text-xs text-pokemon-dark-gray mb-1">平台</div>
                    <div class="text-sm text-pokemon-black">{{ entry.platform }}</div>
                  </div>
                  <div v-if="entry.startDate" class="p-3 bg-pokemon-gray/30 rounded border-2 border-pokemon-black">
                    <div class="pixel-text text-xs text-pokemon-dark-gray mb-1">开始日期</div>
                    <div class="text-sm text-pokemon-black">{{ entry.startDate }}</div>
                  </div>
                </div>

                <div v-if="entry.tags.length" class="mb-6">
                  <div class="pixel-text text-xs text-pokemon-black mb-3">标签</div>
                  <div class="flex flex-wrap gap-2">
                    <span
                      v-for="tag in entry.tags"
                      :key="tag"
                      class="tag bg-sky-light text-pokemon-black"
                    >
                      {{ tag }}
                    </span>
                  </div>
                </div>

                <div v-if="entry.genre?.length" class="mb-6">
                  <div class="pixel-text text-xs text-pokemon-black mb-3">类型</div>
                  <div class="flex flex-wrap gap-2">
                    <span
                      v-for="g in entry.genre"
                      :key="g"
                      class="text-sm px-3 py-1 bg-gold-light border-2 border-pokemon-black rounded"
                    >
                      {{ g }}
                    </span>
                  </div>
                </div>
              </div>

              <div v-if="entry.comment" class="pixel-card p-6 mb-6">
                <h2 class="pixel-text text-sm text-pokemon-black mb-4 flex items-center gap-2">
                  <span>💬</span> 短评
                </h2>
                <p class="text-pokemon-dark-gray leading-relaxed">{{ entry.comment }}</p>
              </div>

              <div v-if="relatedEntries.length" class="pixel-card p-6">
                <h2 class="pixel-text text-sm text-pokemon-black mb-4 flex items-center gap-2">
                  <span>🔗</span> 相关推荐
                </h2>
                <div class="grid grid-cols-2 sm:grid-cols-4 gap-4">
                  <router-link
                    v-for="related in relatedEntries"
                    :key="related.id"
                    :to="`/dex/${related.slug}`"
                    class="group"
                  >
                    <div class="aspect-[3/4] overflow-hidden rounded border-2 border-pokemon-black mb-2">
                      <img
                        :src="related.cover"
                        :alt="related.title"
                        class="w-full h-full object-cover transition-transform group-hover:scale-110"
                        style="image-rendering: auto;"
                      />
                    </div>
                    <div class="pixel-text text-xs text-pokemon-black line-clamp-1 group-hover:text-sky-dark">
                      {{ related.title }}
                    </div>
                  </router-link>
                </div>
              </div>
            </div>
          </div>
        </div>
      </section>
    </template>

    <template v-else>
      <div class="min-h-[60vh] flex items-center justify-center">
        <div class="text-center">
          <div class="text-6xl mb-4">🔍</div>
          <h2 class="pixel-text text-xl text-pokemon-black mb-4">记录未找到</h2>
          <p class="text-pokemon-dark-gray mb-6">这个条目可能不存在或已被删除。</p>
          <router-link to="/dex" class="pixel-btn bg-sky text-white">
            返回图鉴
          </router-link>
        </div>
      </div>
    </template>
  </Layout>
</template>
