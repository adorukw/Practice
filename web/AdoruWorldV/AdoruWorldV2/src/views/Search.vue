<script setup lang="ts">
import { computed } from 'vue'
import { useRouter } from 'vue-router'
import Layout from '@/components/layout/Layout.vue'
import PostCard from '@/components/ui/PostCard.vue'
import { useBlogStore } from '@/stores/blog'

const router = useRouter()
const store = useBlogStore()

const searchQuery = computed(() => store.searchQuery)

const results = computed(() => store.filteredPosts)

const clearFilters = () => {
  store.setSearchQuery('')
  store.setCategory(null)
  store.setTag(null)
  router.push('/')
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
        <h1 class="pixel-text text-2xl text-pokemon-black mb-4 drop-shadow-sm">🔍 搜索</h1>
        <p class="text-pokemon-dark-gray">查找你感兴趣的内容</p>
      </div>
    </section>

    <section class="py-12">
      <div class="max-w-6xl mx-auto px-4">
        <div class="mb-8">
          <div class="flex flex-wrap gap-2 items-center">
            <span class="text-sm text-pokemon-dark-gray">搜索条件：</span>
            <span
              v-if="searchQuery"
              class="tag bg-sky text-white"
            >
              关键词: {{ searchQuery }}
              <button @click="store.setSearchQuery('')" class="ml-2 hover:text-gold-light">×</button>
            </span>
            <span
              v-if="store.selectedCategory"
              class="tag bg-gold text-pokemon-black"
            >
              分类: {{ store.selectedCategory }}
              <button @click="store.setCategory(null)" class="ml-2 hover:text-sky-dark">×</button>
            </span>
            <span
              v-if="store.selectedTag"
              class="tag bg-sky-light text-pokemon-black"
            >
              标签: {{ store.selectedTag }}
              <button @click="store.setTag(null)" class="ml-2 hover:text-sky-dark">×</button>
            </span>
            <button
              v-if="searchQuery || store.selectedCategory || store.selectedTag"
              @click="clearFilters"
              class="text-xs text-sky-dark hover:underline"
            >
              清除全部
            </button>
          </div>
        </div>

        <div v-if="results.length">
          <p class="text-sm text-pokemon-dark-gray mb-6">
            找到 {{ results.length }} 篇相关文章
          </p>
          <div class="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6">
            <router-link
              v-for="post in results"
              :key="post.id"
              :to="`/post/${post.slug}`"
              class="block"
            >
              <PostCard
                :title="post.title"
                :cover-image="post.coverImage"
                :tags="post.tagNames"
                :category="post.category"
                :date="post.createdAt?.slice(0,10)"
                :reading-time="post.reading_time"
                :views="post.views"
              />
            </router-link>
          </div>
        </div>

        <div v-else class="text-center py-16">
          <div class="text-6xl mb-4">🔍</div>
          <h2 class="pixel-text text-lg text-pokemon-black mb-4">没有找到相关内容</h2>
          <p class="text-pokemon-dark-gray mb-6">试试其他关键词或浏览全部分类</p>
          <div class="flex justify-center gap-4">
            <router-link to="/categories" class="pixel-btn bg-sky text-white hover:bg-sky-dark">
              浏览分类
            </router-link>
            <router-link to="/archives" class="pixel-btn bg-gold text-pokemon-black hover:bg-gold-dark">
              浏览归档
            </router-link>
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
