<script setup lang="ts">
import { useRouter } from 'vue-router'
import { useBlogStore } from '@/stores/blog'
import TagBadge from './TagBadge.vue'

const router = useRouter()
const store = useBlogStore()

const handleTagClick = (tagName: string) => {
  store.setTag(tagName)
  router.push('/search')
}

const handleCategoryClick = (categoryName: string) => {
  store.setCategory(categoryName)
  router.push('/search')
}
</script>

<template>
  <aside class="space-y-6">
    <div class="pixel-card p-5">
      <div class="flex items-center gap-4 mb-4">
        <div class="relative">
          <img
            :src="store.authorInfo.avatar"
            :alt="store.authorInfo.name"
            class="w-16 h-16 rounded-full border-4 border-pokemon-black shadow-md"
          />
          <div class="absolute inset-0 rounded-full bg-gradient-to-b from-white/40 to-transparent pointer-events-none"></div>
        </div>
        <div>
          <h3 class="pixel-text text-base text-pokemon-black">{{ store.authorInfo.name }}</h3>
          <p class="text-xs text-pokemon-dark-gray">{{ store.authorInfo.location }}</p>
        </div>
      </div>
      <p class="text-sm text-pokemon-dark-gray leading-relaxed">
        {{ store.authorInfo.bio }}
      </p>
    </div>

    <div class="pixel-card p-5">
      <h3 class="pixel-text text-base text-pokemon-black mb-4 flex items-center gap-2">
        <span class="text-sky-dark">📁</span> 分类
      </h3>
      <div class="space-y-2">
        <div
          v-for="category in store.allCategories"
          :key="category.id"
          class="flex items-center justify-between p-2 border-2 border-transparent hover:border-pokemon-black hover:bg-gold-light/50 cursor-pointer transition-all duration-100 rounded"
          @click="handleCategoryClick(category.name)"
        >
          <div class="flex items-center gap-2">
            <span>{{ category.icon }}</span>
            <span class="text-sm">{{ category.name }}</span>
          </div>
          <span class="text-xs text-pokemon-dark-gray bg-pokemon-gray/50 px-2 py-1 border-2 border-pokemon-black shadow-sm">
            {{ category.count }}
          </span>
        </div>
      </div>
    </div>

    <div class="pixel-card p-5">
      <h3 class="pixel-text text-base text-pokemon-black mb-4 flex items-center gap-2">
        <span class="text-gold-dark">🏷️</span> 热门标签
      </h3>
      <div class="flex flex-wrap gap-2">
        <TagBadge
          v-for="tag in store.allTags.slice(0, 10)"
          :key="tag.id"
          :name="tag.name"
          :color="tag.color"
          :count="tag.count"
          @click="handleTagClick(tag.name)"
        />
      </div>
    </div>

    <div class="pixel-card p-5">
      <h3 class="pixel-text text-base text-pokemon-black mb-4 flex items-center gap-2">
        <span class="text-pokemon-red">🔥</span> 最新文章
      </h3>
      <div class="space-y-3">
        <router-link
          v-for="post in store.recentPosts.slice(0, 5)"
          :key="post.id"
          :to="`/post/${post.slug}`"
          class="block p-2 border-2 border-transparent hover:border-pokemon-black hover:bg-sky-light/30 transition-all duration-100 rounded"
        >
          <h4 class="text-sm text-pokemon-black line-clamp-2 hover:text-sky-dark">
            {{ post.title }}
          </h4>
          <p class="text-xs text-pokemon-dark-gray mt-1">{{ post.createdAt?.slice(0,10) }}</p>
        </router-link>
      </div>
    </div>
  </aside>
</template>
