<script setup lang="ts">
import { computed } from 'vue'
import { useRoute } from 'vue-router'
import Layout from '@/components/layout/Layout.vue'
import PostCard from '@/components/ui/PostCard.vue'
import { useBlogStore } from '@/stores/blog'

const route = useRoute()
const store = useBlogStore()

const categoryName = computed(() => route.params.slug as string)

const categoryInfo = computed(() => 
  store.allCategories.find((c: { name: string }) => c.name === categoryName.value)
)

const posts = computed(() => 
  store.allPosts.filter((p: { category: string[] }) => p.category?.includes(categoryName.value))
)
</script>

<template>
  <Layout>
    <section class="relative py-16 overflow-hidden">
      <div class="absolute inset-0 sky-gradient"></div>
      
      <div class="absolute inset-0 overflow-hidden pointer-events-none">
        <div class="cloud absolute top-4 left-[15%] w-20 h-10 bg-white rounded-full opacity-60"></div>
        <div class="cloud absolute top-8 left-[70%] w-24 h-12 bg-white rounded-full opacity-55"></div>
      </div>
      
      <div class="max-w-6xl mx-auto px-4 text-center relative z-10">
        <div class="text-4xl mb-4">{{ categoryInfo?.icon || '📁' }}</div>
        <h1 class="pixel-text text-2xl text-pokemon-black mb-4 drop-shadow-sm">{{ categoryName }}</h1>
        <p class="text-pokemon-dark-gray">{{ categoryInfo?.description || '该分类下的所有文章' }}</p>
      </div>
    </section>

    <section class="py-12">
      <div class="max-w-6xl mx-auto px-4">
        <div v-if="posts.length" class="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6">
          <router-link
            v-for="post in posts"
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
        <div v-else class="text-center py-12">
          <div class="text-6xl mb-4">📭</div>
          <p class="text-pokemon-dark-gray">该分类下暂无文章</p>
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
