<script setup lang="ts">
import { useRouter } from 'vue-router'
import Layout from '@/components/layout/Layout.vue'
import { useBlogStore } from '@/stores/blog'

const router = useRouter()
const store = useBlogStore()

const handleCategoryClick = (categoryName: string) => {
  store.setCategory(categoryName)
  router.push(`/category/${categoryName}`)
}
</script>

<template>
  <Layout>
    <section class="relative py-16 overflow-hidden">
      <div class="absolute inset-0 sky-gradient"></div>
      
      <div class="absolute inset-0 overflow-hidden pointer-events-none">
        <div class="cloud absolute top-4 left-[10%] w-20 h-10 bg-white rounded-full opacity-60"></div>
        <div class="cloud absolute top-8 left-[60%] w-24 h-12 bg-white rounded-full opacity-55"></div>
      </div>
      
      <div class="max-w-6xl mx-auto px-4 text-center relative z-10">
        <h1 class="pixel-text text-2xl text-pokemon-black mb-4 drop-shadow-sm">📁 分类</h1>
        <p class="text-pokemon-dark-gray">按分类浏览所有文章</p>
      </div>
    </section>

    <section class="py-12">
      <div class="max-w-6xl mx-auto px-4">
        <div class="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6">
          <div
            v-for="category in store.allCategories"
            :key="category.id"
            class="pixel-card p-6 cursor-pointer group"
            @click="handleCategoryClick(category.name)"
          >
            <div class="flex items-center gap-4 mb-4">
              <div
                class="w-14 h-14 rounded-lg border-4 border-pokemon-black flex items-center justify-center text-2xl bg-white"
              >
                {{ category.icon }}
              </div>
              <div>
                <h3 class="pixel-text text-sm text-pokemon-black group-hover:text-sky-dark transition-colors">
                  {{ category.name }}
                </h3>
                <p class="text-xs text-pokemon-dark-gray">{{ category.count }} 篇文章</p>
              </div>
            </div>
            <p class="text-sm text-pokemon-dark-gray">{{ category.description }}</p>
            <div class="mt-4 flex items-center text-sky-dark text-sm group-hover:translate-x-2 transition-transform">
              <span>查看文章</span>
              <svg class="w-4 h-4 ml-1" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M9 5l7 7-7 7" />
              </svg>
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
