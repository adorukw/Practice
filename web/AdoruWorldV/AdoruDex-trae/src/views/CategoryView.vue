<script setup>
import { ref, computed } from 'vue'
import { useRoute, useRouter } from 'vue-router'
import { useCollectionStore } from '../stores/collection'
import PixelCard from '../components/PixelCard.vue'
import PixelButton from '../components/PixelButton.vue'
import PixelDialog from '../components/PixelDialog.vue'

const route = useRoute()
const router = useRouter()
const store = useCollectionStore()

const category = computed(() => store.getCategoryBySlug(route.params.slug))
const works = computed(() => store.getWorksByCategory(route.params.slug))
const viewMode = ref('grid')

const statusFilter = ref('all')
const statusOptions = [
  { value: 'all', label: '全部' },
  { value: 'playing', label: '在玩' },
  { value: 'watching', label: '在看' },
  { value: 'listening', label: '在听' },
  { value: 'reading', label: '在读' },
  { value: 'want', label: '想看/玩' },
  { value: 'completed', label: '已完成' },
]

const filteredWorks = computed(() => {
  if (statusFilter.value === 'all') return works.value
  return works.value.filter(w => {
    if (statusFilter.value === 'want') return w.status === 'want' || w.status === 'wantwatch' || w.status === 'wantplay' || w.status === 'wantlisten' || w.status === 'wantread'
    return w.status === statusFilter.value
  })
})
</script>

<template>
  <div class="pixel-bg-pattern min-h-screen">
    <div v-if="category" class="max-w-6xl mx-auto px-4 py-8">
      <div class="mb-8">
        <button
          class="font-pixel text-[8px] text-pokemon-blue dark:text-pokemon-yellow hover:text-pokemon-red cursor-pointer mb-4"
          @click="router.back()"
        >
          ◀ 返回
        </button>

        <div class="pixel-border bg-pokemon-card dark:bg-pokemon-dark p-6">
          <div class="flex items-center gap-4 mb-4">
            <span class="text-4xl">{{ category.icon }}</span>
            <div>
              <h1 class="font-pixel text-[14px] md:text-[18px] text-pokemon-dark dark:text-pokemon-light pixel-text-shadow">
                {{ category.name }}
              </h1>
              <p class="font-pixel text-[8px] text-pokemon-darkgray mt-1">{{ category.nameEn }} · {{ works.length }} 件作品</p>
            </div>
          </div>
          <p class="font-pixel text-[8px] text-pokemon-darkgray dark:text-pokemon-gray leading-relaxed">
            {{ category.description }}
          </p>
        </div>
      </div>

      <div class="flex flex-wrap items-center justify-between gap-4 mb-6">
        <div class="flex items-center gap-2">
          <div class="flex pixel-border-thin overflow-hidden">
            <button
              :class="[
                'font-pixel text-[8px] px-3 py-1.5 cursor-pointer transition-colors',
                viewMode === 'grid' ? 'bg-pokemon-blue text-white' : 'bg-pokemon-light dark:bg-pokemon-dark text-pokemon-dark dark:text-pokemon-light hover:bg-pokemon-blue hover:text-white',
              ]"
              @click="viewMode = 'grid'"
            >
              ▦ 网格
            </button>
            <button
              :class="[
                'font-pixel text-[8px] px-3 py-1.5 cursor-pointer transition-colors',
                viewMode === 'list' ? 'bg-pokemon-blue text-white' : 'bg-pokemon-light dark:bg-pokemon-dark text-pokemon-dark dark:text-pokemon-light hover:bg-pokemon-blue hover:text-white',
              ]"
              @click="viewMode = 'list'"
            >
              ☰ 列表
            </button>
          </div>
        </div>

        <div class="flex items-center gap-2 flex-wrap">
          <span class="font-pixel text-[7px] text-pokemon-darkgray">状态:</span>
          <button
            v-for="opt in statusOptions"
            :key="opt.value"
            :class="[
              'font-pixel text-[7px] px-2 py-1 cursor-pointer transition-colors',
              statusFilter === opt.value
                ? 'bg-pokemon-blue text-white pixel-border-thin'
                : 'bg-pokemon-light dark:bg-pokemon-dark text-pokemon-dark dark:text-pokemon-light hover:bg-pokemon-yellow',
            ]"
            @click="statusFilter = opt.value"
          >
            {{ opt.label }}
          </button>
        </div>
      </div>

      <div v-if="filteredWorks.length === 0" class="text-center py-16">
        <PixelDialog>
          <p class="text-center mb-2">没有找到符合条件的作品...</p>
          <p class="text-center">试试切换其他筛选条件吧！</p>
        </PixelDialog>
      </div>

      <div
        v-else
        :class="[
          viewMode === 'grid'
            ? 'grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-3 xl:grid-cols-4 gap-6'
            : 'flex flex-col gap-4',
        ]"
      >
        <template v-if="viewMode === 'grid'">
          <PixelCard
            v-for="work in filteredWorks"
            :key="work.id"
            :work="work"
          />
        </template>
        <template v-else>
          <router-link
            v-for="work in filteredWorks"
            :key="work.id"
            :to="`/work/${work.id}`"
            class="pixel-border bg-pokemon-card dark:bg-pokemon-dark p-4 flex items-center gap-4 hover:bg-pokemon-highlight dark:hover:bg-pokemon-dark/80 transition-colors"
          >
            <div class="w-12 h-12 bg-pokemon-bg dark:bg-pokemon-darker flex items-center justify-center pixel-border-thin shrink-0">
              <img :src="work.coverImage" :alt="work.title" class="w-8 h-8 object-contain" loading="lazy" />
            </div>
            <div class="flex-1 min-w-0">
              <h3 class="font-pixel text-[9px] text-pokemon-dark dark:text-pokemon-light mb-1 truncate">{{ work.title }}</h3>
              <div class="flex flex-wrap gap-1">
                <span v-for="tag in work.tags" :key="tag" class="text-[6px] font-pixel px-1.5 py-0.5 bg-pokemon-highlight dark:bg-pokemon-blue/30 text-pokemon-dark dark:text-pokemon-yellow">
                  {{ tag }}
                </span>
              </div>
            </div>
            <div class="font-pixel text-[7px] text-pokemon-darkgray shrink-0">{{ work.date }}</div>
          </router-link>
        </template>
      </div>
    </div>

    <div v-else class="max-w-6xl mx-auto px-4 py-16 text-center">
      <div class="pixel-border bg-pokemon-card dark:bg-pokemon-dark p-8 inline-block">
        <p class="text-6xl mb-4">🔮</p>
        <h2 class="font-pixel text-[12px] text-pokemon-dark dark:text-pokemon-light mb-2">
          野生的404出现了！
        </h2>
        <p class="font-pixel text-[8px] text-pokemon-darkgray mb-4">
          找不到这个分类...
        </p>
        <PixelButton label="返回首页" variant="primary" to="/" />
      </div>
    </div>
  </div>
</template>
