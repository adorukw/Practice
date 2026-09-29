<script setup>
import { computed, ref } from 'vue'
import { useRoute, useRouter } from 'vue-router'
import { useCollectionStore } from '../stores/collection'
import PixelButton from '../components/PixelButton.vue'
import PixelDialog from '../components/PixelDialog.vue'

const route = useRoute()
const router = useRouter()
const store = useCollectionStore()

const work = computed(() => store.getWorkById(route.params.id))
const category = computed(() => work.value ? store.getCategoryBySlug(work.value.category) : null)

const currentImageIndex = ref(0)

const allImages = computed(() => {
  if (!work.value) return []
  return [work.value.coverImage, ...work.value.images]
})

const statusLabels = {
  playing: '在玩', played: '已玩', want: '想玩',
  watched: '已看', watching: '在看', wantwatch: '想看',
  listened: '已听', listening: '在听', wantlisten: '想听',
  read: '已读', reading: '在读', wantread: '想读',
  completed: '已完成', 'in-progress': '进行中',
}

const statusColors = {
  playing: 'bg-pokemon-green', played: 'bg-pokemon-darkgray', want: 'bg-pokemon-yellow text-pokemon-dark',
  watched: 'bg-pokemon-darkgray', watching: 'bg-pokemon-green', wantwatch: 'bg-pokemon-yellow text-pokemon-dark',
  listened: 'bg-pokemon-darkgray', listening: 'bg-pokemon-green', wantlisten: 'bg-pokemon-yellow text-pokemon-dark',
  read: 'bg-pokemon-darkgray', reading: 'bg-pokemon-green', wantread: 'bg-pokemon-yellow text-pokemon-dark',
  completed: 'bg-pokemon-blue', 'in-progress': 'bg-pokemon-green',
}

const prevWork = computed(() => {
  if (!work.value) return null
  const idx = store.allWorks.findIndex(w => w.id === work.value.id)
  return idx > 0 ? store.allWorks[idx - 1] : null
})

const nextWork = computed(() => {
  if (!work.value) return null
  const idx = store.allWorks.findIndex(w => w.id === work.value.id)
  return idx < store.allWorks.length - 1 ? store.allWorks[idx + 1] : null
})

function prevImage() {
  currentImageIndex.value = (currentImageIndex.value - 1 + allImages.value.length) % allImages.value.length
}

function nextImage() {
  currentImageIndex.value = (currentImageIndex.value + 1) % allImages.value.length
}
</script>

<template>
  <div class="pixel-bg-pattern min-h-screen">
    <div v-if="work" class="max-w-6xl mx-auto px-4 py-8">
      <button
        class="font-pixel text-[8px] text-pokemon-blue dark:text-pokemon-yellow hover:text-pokemon-red cursor-pointer mb-4"
        @click="router.back()"
      >
        ◀ 返回
      </button>

      <div class="grid grid-cols-1 lg:grid-cols-2 gap-8 mb-8">
        <div>
          <div class="pixel-border bg-pokemon-card dark:bg-pokemon-dark p-4">
            <div class="aspect-square bg-pokemon-bg dark:bg-pokemon-darker flex items-center justify-center relative pixel-border-thin">
              <img
                :src="allImages[currentImageIndex]"
                :alt="work.title"
                class="max-w-[60%] max-h-[60%] object-contain"
              />
              <div
                v-if="allImages.length > 1"
                class="absolute bottom-3 left-0 right-0 flex items-center justify-center gap-3"
              >
                <button
                  class="w-8 h-8 pixel-border-thin bg-pokemon-card dark:bg-pokemon-dark flex items-center justify-center cursor-pointer hover:bg-pokemon-yellow transition-colors text-[10px]"
                  @click="prevImage"
                >
                  ◀
                </button>
                <span class="font-pixel text-[7px] text-pokemon-dark dark:text-pokemon-light">
                  {{ currentImageIndex + 1 }} / {{ allImages.length }}
                </span>
                <button
                  class="w-8 h-8 pixel-border-thin bg-pokemon-card dark:bg-pokemon-dark flex items-center justify-center cursor-pointer hover:bg-pokemon-yellow transition-colors text-[10px]"
                  @click="nextImage"
                >
                  ▶
                </button>
              </div>
            </div>

            <div v-if="allImages.length > 1" class="flex gap-2 mt-3 overflow-x-auto pb-2">
              <button
                v-for="(img, idx) in allImages"
                :key="idx"
                :class="[
                  'w-14 h-14 shrink-0 bg-pokemon-bg dark:bg-pokemon-darker flex items-center justify-center cursor-pointer transition-all',
                  currentImageIndex === idx ? 'pixel-border' : 'pixel-border-thin opacity-60 hover:opacity-100',
                ]"
                @click="currentImageIndex = idx"
              >
                <img :src="img" :alt="`缩略图 ${idx + 1}`" class="w-8 h-8 object-contain" />
              </button>
            </div>
          </div>
        </div>

        <div class="flex flex-col gap-4">
          <div class="pixel-border bg-pokemon-card dark:bg-pokemon-dark p-5">
            <div class="flex items-start justify-between gap-3 mb-4">
              <div>
                <div class="font-pixel text-[7px] text-pokemon-accent mb-2">
                  #{{ work.id }} · {{ category?.icon }} {{ category?.name }}
                </div>
                <h1 class="font-pixel text-[12px] md:text-[14px] text-pokemon-dark dark:text-pokemon-light pixel-text-shadow leading-relaxed">
                  {{ work.title }}
                </h1>
              </div>
              <div
                v-if="work.status && statusLabels[work.status]"
                :class="[
                  'shrink-0 px-3 py-1 text-[7px] font-pixel text-white pixel-border-thin',
                  statusColors[work.status] || 'bg-pokemon-darkgray',
                ]"
              >
                {{ statusLabels[work.status] }}
              </div>
            </div>

            <div class="flex flex-wrap gap-2 mb-4">
              <span
                v-for="tag in work.tags"
                :key="tag"
                class="text-[7px] font-pixel px-2 py-1 bg-pokemon-highlight dark:bg-pokemon-blue/30 text-pokemon-dark dark:text-pokemon-yellow pixel-border-thin"
              >
                {{ tag }}
              </span>
            </div>

            <div class="flex items-center gap-1 mb-4">
              <span v-for="i in 5" :key="i" class="text-[14px]">
                {{ i <= work.rating ? '⭐' : '☆' }}
              </span>
            </div>

            <div class="font-pixel text-[7px] text-pokemon-darkgray mb-4">
              📅 {{ work.date }}
            </div>
          </div>

          <PixelDialog title="作品介绍">
            <p class="leading-relaxed">{{ work.description }}</p>
          </PixelDialog>

          <div class="pixel-border bg-pokemon-card dark:bg-pokemon-dark p-4">
            <h3 class="font-pixel text-[9px] text-pokemon-dark dark:text-pokemon-light mb-3">
              ▶ 标记状态
            </h3>
            <div class="flex flex-wrap gap-2">
              <PixelButton
                v-for="statusOpt in [
                  { value: 'want', label: '想看' },
                  { value: 'watching', label: '在看' },
                  { value: 'watched', label: '已看' },
                ]"
                :key="statusOpt.value"
                :label="statusOpt.label"
                :variant="work.status === statusOpt.value ? 'red' : 'outline'"
                size="sm"
              />
            </div>
          </div>
        </div>
      </div>

      <div class="pixel-border bg-pokemon-card dark:bg-pokemon-dark p-4 flex items-center justify-between">
        <div>
          <router-link
            v-if="prevWork"
            :to="`/work/${prevWork.id}`"
            class="font-pixel text-[8px] text-pokemon-blue dark:text-pokemon-yellow hover:text-pokemon-red transition-colors"
          >
            ◀ {{ prevWork.title }}
          </router-link>
          <span v-else class="font-pixel text-[8px] text-pokemon-darkgray">已是最前</span>
        </div>
        <div>
          <router-link
            v-if="nextWork"
            :to="`/work/${nextWork.id}`"
            class="font-pixel text-[8px] text-pokemon-blue dark:text-pokemon-yellow hover:text-pokemon-red transition-colors"
          >
            {{ nextWork.title }} ▶
          </router-link>
          <span v-else class="font-pixel text-[8px] text-pokemon-darkgray">已是最后</span>
        </div>
      </div>
    </div>

    <div v-else class="max-w-6xl mx-auto px-4 py-16 text-center">
      <div class="pixel-border bg-pokemon-card dark:bg-pokemon-dark p-8 inline-block">
        <p class="text-6xl mb-4">👻</p>
        <h2 class="font-pixel text-[12px] text-pokemon-dark dark:text-pokemon-light mb-2">
          野生的404出现了！
        </h2>
        <p class="font-pixel text-[8px] text-pokemon-darkgray mb-4">
          这件作品似乎逃跑了...
        </p>
        <PixelButton label="返回首页" variant="primary" to="/" />
      </div>
    </div>
  </div>
</template>
