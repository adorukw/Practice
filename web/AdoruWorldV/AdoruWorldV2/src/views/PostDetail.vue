<script setup lang="ts">
import { ref, computed, onMounted, watch } from 'vue'
import { useRoute, useRouter } from 'vue-router'
import { marked } from 'marked'
import hljs from 'highlight.js'
import 'highlight.js/styles/atom-one-dark.css'
import Layout from '@/components/layout/Layout.vue'
import PostCard from '@/components/ui/PostCard.vue'
import { useBlogStore } from '@/stores/blog'

const route = useRoute()
const router = useRouter()
const store = useBlogStore()

const readingProgress = ref(0)
const showToc = ref(false)

const post = computed(() => store.allPosts.find(p => p.slug === route.params.slug))

const relatedPosts = computed(() => {
  if (!post.value) return []
  return store.allPosts
    .filter(p => 
      p.id !== post.value?.id && 
      (p.tagNames || []).some((tag: string) => (post.value?.tagNames || []).includes(tag))
    )
    .slice(0, 3)
})

const renderedContent = computed(() => {
  if (!post.value) return ''
  
  marked.setOptions({
    highlight: (code, lang) => {
      if (lang && hljs.getLanguage(lang)) {
        return hljs.highlight(code, { language: lang }).value
      }
      return hljs.highlightAuto(code).value
    },
    breaks: true,
    gfm: true
  })
  
  return marked(post.value.content)
})

const handleScroll = () => {
  const scrollTop = window.scrollY
  const docHeight = document.documentElement.scrollHeight - window.innerHeight
  readingProgress.value = Math.min(100, Math.round((scrollTop / docHeight) * 100))
}

const scrollToTop = () => {
  window.scrollTo({ top: 0, behavior: 'smooth' })
}

onMounted(() => {
  window.addEventListener('scroll', handleScroll)
})

watch(() => route.params.slug, () => {
  window.scrollTo(0, 0)
  readingProgress.value = 0
})
</script>

<template>
  <Layout>
    <template v-if="post">
      <div class="fixed top-0 left-0 right-0 h-1 bg-pokemon-gray z-50">
        <div
          class="h-full bg-gradient-to-r from-sky to-gold transition-all duration-150"
          :style="{ width: `${readingProgress}%` }"
        />
      </div>

      <article>
        <header class="relative py-16 overflow-hidden">
          <div class="absolute inset-0 sky-gradient"></div>
          
          <div class="absolute inset-0 overflow-hidden pointer-events-none">
            <div class="cloud absolute top-2 left-[5%] w-20 h-10 bg-white rounded-full opacity-60"></div>
            <div class="cloud absolute top-6 left-[15%] w-16 h-8 bg-white rounded-full opacity-55"></div>
            <div class="cloud absolute top-4 left-[70%] w-24 h-12 bg-white rounded-full opacity-60"></div>
            <div class="cloud absolute top-10 left-[80%] w-18 h-9 bg-white rounded-full opacity-55"></div>
          </div>
          
          <div class="max-w-4xl mx-auto px-4 relative z-10">
            <div class="flex flex-wrap gap-2 mb-4">
              <span
                v-for="cat in post.category"
                :key="cat"
                class="tag bg-gold text-pokemon-black"
              >
                {{ cat }}
              </span>
            </div>
            <h1 class="pixel-text text-xl md:text-2xl text-pokemon-black mb-6 leading-relaxed drop-shadow-sm">
              {{ post.title }}
            </h1>
            <div class="flex flex-wrap items-center gap-4 text-pokemon-dark-gray text-sm">
              <span class="flex items-center gap-1">
                <svg class="w-4 h-4" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                  <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" 
                        d="M8 7V3m8 4V3m-9 8h10M5 21h14a2 2 0 002-2V7a2 2 0 00-2-2H5a2 2 0 00-2 2v12a2 2 0 002 2z" />
                </svg>
                {{ post.createdAt?.slice(0,10) }}
              </span>
              <span class="flex items-center gap-1">
                <svg class="w-4 h-4" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                  <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" 
                        d="M12 8v4l3 3m6-3a9 9 0 11-18 0 9 9 0 0118 0z" />
                </svg>
                {{ post.reading_time }} 分钟阅读
              </span>
              <span class="flex items-center gap-1">
                <svg class="w-4 h-4" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                  <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" 
                        d="M15 12a3 3 0 11-6 0 3 3 0 016 0z" />
                  <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" 
                        d="M2.458 12C3.732 7.943 7.523 5 12 5c4.478 0 8.268 2.943 9.542 7-1.274 4.057-5.064 7-9.542 7-4.477 0-8.268-2.943-9.542-7z" />
                </svg>
                {{ post.views }} 次阅读
              </span>
            </div>
          </div>
        </header>

        <div v-if="post.coverImage" class="max-w-4xl mx-auto px-4 -mt-8 relative z-20">
          <img
            :src="post.coverImage"
            :alt="post.title"
            class="w-full h-64 md:h-96 object-cover pixel-border rounded-lg"
          />
        </div>

        <div class="max-w-4xl mx-auto px-4 py-12">
          <div class="flex flex-wrap gap-2 mb-8">
            <span
              v-for="tag in post.tagNames"
              :key="tag"
              class="tag bg-sky-light text-pokemon-black hover:bg-sky hover:text-white cursor-pointer transition-colors"
            >
              #{{ tag }}
            </span>
          </div>

          <div
            class="prose prose-lg max-w-none article-content"
            v-html="renderedContent"
          />

          <div class="mt-12 pt-8 border-t-4 border-pokemon-black">
            <div class="dialog-box mb-8">
              <p class="pixel-text text-sm text-pokemon-black">
                感谢阅读！如果这篇文章对你有帮助，欢迎分享给更多人。
              </p>
            </div>

            <div class="flex flex-wrap gap-4 justify-center">
              <button
                @click="scrollToTop"
                class="pixel-btn bg-sky text-white hover:bg-sky-dark"
              >
                返回顶部
              </button>
              <router-link
                to="/"
                class="pixel-btn bg-gold text-pokemon-black hover:bg-gold-dark"
              >
                返回首页
              </router-link>
            </div>
          </div>
        </div>

        <section v-if="relatedPosts.length" class="py-12 overflow-hidden relative">
          <div class="absolute inset-0 gold-pattern opacity-30"></div>
          <div class="max-w-6xl mx-auto px-4 relative z-10">
            <h2 class="pixel-text text-lg text-pokemon-black mb-6 flex items-center gap-3">
              <span>📖</span> 相关文章
            </h2>
            <div class="grid grid-cols-1 md:grid-cols-3 gap-6">
              <router-link
                v-for="relatedPost in relatedPosts"
                :key="relatedPost.id"
                :to="`/post/${relatedPost.slug}`"
                class="block"
              >
                <PostCard
                  :title="relatedPost.title"
                  :cover-image="relatedPost.coverImage"
                  :tags="relatedPost.tags"
                  :date="relatedPost.createdAt?.slice(0,10)"
                  :reading-time="relatedPost.reading_time"
                />
              </router-link>
            </div>
          </div>
        </section>
      </article>
    </template>

    <template v-else>
      <div class="min-h-[60vh] flex items-center justify-center">
        <div class="text-center">
          <div class="text-6xl mb-4">🔍</div>
          <h2 class="pixel-text text-xl text-pokemon-black mb-4">文章未找到</h2>
          <p class="text-pokemon-dark-gray mb-6">看起来这篇文章不存在或已被删除。</p>
          <router-link to="/" class="pixel-btn bg-sky text-white">
            返回首页
          </router-link>
        </div>
      </div>
    </template>
  </Layout>
</template>

<style scoped>
.cloud {
  filter: blur(2px);
}

.article-content :deep(h1) {
  font-family: var(--font-pixel);
  font-size: 1.5rem;
  color: var(--color-pokemon-black);
  margin: 2rem 0 1rem;
  padding-bottom: 0.5rem;
  border-bottom: 3px solid var(--color-pokemon-black);
}

.article-content :deep(h2) {
  font-family: var(--font-pixel);
  font-size: 1.25rem;
  color: var(--color-pokemon-black);
  margin: 1.5rem 0 0.75rem;
}

.article-content :deep(h3) {
  font-family: var(--font-pixel);
  font-size: 1rem;
  color: var(--color-sky-dark);
  margin: 1.25rem 0 0.5rem;
}

.article-content :deep(p) {
  line-height: 1.8;
  margin-bottom: 1rem;
}

.article-content :deep(pre) {
  background: #1e1e1e;
  border: 4px solid var(--color-pokemon-black);
  border-radius: 4px;
  padding: 1rem;
  overflow-x: auto;
  margin: 1rem 0;
}

.article-content :deep(code) {
  font-family: 'Fira Code', monospace;
  font-size: 0.875rem;
}

.article-content :deep(ul), .article-content :deep(ol) {
  margin: 1rem 0;
  padding-left: 1.5rem;
}

.article-content :deep(li) {
  margin: 0.5rem 0;
}

.article-content :deep(a) {
  color: var(--color-sky-dark);
  text-decoration: underline;
}

.article-content :deep(a:hover) {
  color: var(--color-gold-dark);
}

.article-content :deep(img) {
  max-width: 100%;
  border: 4px solid var(--color-pokemon-black);
  border-radius: 4px;
  margin: 1rem 0;
}

.article-content :deep(table) {
  width: 100%;
  border-collapse: collapse;
  margin: 1rem 0;
}

.article-content :deep(th), .article-content :deep(td) {
  border: 2px solid var(--color-pokemon-black);
  padding: 0.75rem;
  text-align: left;
}

.article-content :deep(th) {
  background: var(--color-gold-light);
  font-family: var(--font-pixel);
  font-size: 0.75rem;
}
</style>
