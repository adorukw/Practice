<script setup lang="ts">
import { computed } from 'vue'
import Layout from '@/components/layout/Layout.vue'
import { useBlogStore } from '@/stores/blog'

const store = useBlogStore()

const groupedPosts = computed(() => {
  const groups: { [key: string]: { year: number; month: number; posts: typeof store.allPosts }[] } = {}
  
  const sortedPosts = [...store.allPosts].sort((a, b) => 
    new Date(b.createdAt).getTime() - new Date(a.createdAt).getTime()
  )
  
  sortedPosts.forEach(post => {
    const date = new Date(post.createdAt)
    const year = date.getFullYear()
    const month = date.getMonth() + 1
    
    if (!groups[year]) {
      groups[year] = []
    }
    
    let monthGroup = groups[year].find(g => g.month === month)
    if (!monthGroup) {
      monthGroup = { year, month, posts: [] }
      groups[year].push(monthGroup)
    }
    
    monthGroup.posts.push(post)
  })
  
  return Object.entries(groups)
    .sort((a, b) => Number(b[0]) - Number(a[0]))
    .map(([year, months]) => ({
      year: Number(year),
      months: months.sort((a, b) => b.month - a.month)
    }))
})

const monthNames = ['一月', '二月', '三月', '四月', '五月', '六月', '七月', '八月', '九月', '十月', '十一月', '十二月']
</script>

<template>
  <Layout>
    <section class="relative py-16 overflow-hidden">
      <div class="absolute inset-0 sky-gradient"></div>
      
      <div class="absolute inset-0 overflow-hidden pointer-events-none">
        <div class="cloud absolute top-4 left-[8%] w-20 h-10 bg-white rounded-full opacity-60"></div>
        <div class="cloud absolute top-8 left-[55%] w-24 h-12 bg-white rounded-full opacity-55"></div>
        <div class="cloud absolute top-2 left-[80%] w-18 h-9 bg-white rounded-full opacity-60"></div>
      </div>
      
      <div class="max-w-6xl mx-auto px-4 text-center relative z-10">
        <h1 class="pixel-text text-2xl text-pokemon-black mb-4 drop-shadow-sm">📚 归档</h1>
        <p class="text-pokemon-dark-gray">按时间浏览所有文章</p>
      </div>
    </section>

    <section class="py-12">
      <div class="max-w-4xl mx-auto px-4">
        <div v-for="yearGroup in groupedPosts" :key="yearGroup.year" class="mb-12">
          <div class="flex items-center gap-4 mb-6">
            <div class="pixel-card px-4 py-2 bg-gold">
              <span class="pixel-text text-lg text-pokemon-black">{{ yearGroup.year }}</span>
            </div>
            <div class="flex-1 h-1 bg-pokemon-black/20"></div>
          </div>
          
          <div v-for="monthGroup in yearGroup.months" :key="`${yearGroup.year}-${monthGroup.month}`" class="mb-8">
            <h3 class="pixel-text text-sm text-pokemon-dark-gray mb-4 flex items-center gap-2">
              <span class="text-sky-dark">◆</span>
              {{ monthNames[monthGroup.month - 1] }}
              <span class="text-xs text-pokemon-gray">({{ monthGroup.posts.length }}篇)</span>
            </h3>
            
            <div class="space-y-4 ml-4 border-l-4 border-gold pl-6">
              <router-link
                v-for="post in monthGroup.posts"
                :key="post.id"
                :to="`/post/${post.slug}`"
                class="block pixel-card p-4 hover:translate-x-2 transition-transform"
              >
                <div class="flex flex-wrap items-start justify-between gap-2">
                  <div class="flex-1">
                    <h4 class="text-pokemon-black font-medium hover:text-sky-dark transition-colors">
                      {{ post.title }}
                    </h4>
                    <div class="flex flex-wrap gap-2 mt-2">
                      <span
                        v-for="tag in (post.tagNames || []).slice(0, 3)"
                        :key="tag"
                        class="text-xs px-2 py-0.5 bg-sky-light/50 rounded text-pokemon-dark-gray"
                      >
                        #{{ tag }}
                      </span>
                    </div>
                  </div>
                  <div class="text-right text-xs text-pokemon-dark-gray">
                    <div>{{ post.createdAt?.slice(0,10) }}</div>
                    <div class="mt-1">{{ post.reading_time }} 分钟</div>
                  </div>
                </div>
              </router-link>
            </div>
          </div>
        </div>
      </div>
    </section>
  </Layout>
</template>

<style scoped>
.cloud {
  filter: blur(2px);
}
</style>
