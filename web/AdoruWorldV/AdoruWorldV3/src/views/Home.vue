<script setup lang="ts">
import { useBlogStore } from '@/stores/blog'
import Layout from '@/components/layout/Layout.vue'
import PostCard from '@/components/ui/PostCard.vue'
import Sidebar from '@/components/ui/Sidebar.vue'
import ExpBar from '@/components/ui/ExpBar.vue'

const store = useBlogStore()
</script>

<template>
  <Layout>
    <section class="relative py-16 md:py-24 overflow-hidden">
      <div class="absolute inset-0 sky-gradient"></div>
      
      <div class="absolute inset-0 overflow-hidden pointer-events-none">
        <div class="cloud cloud-1 absolute w-24 h-12 bg-white rounded-full"></div>
        <div class="cloud cloud-2 absolute w-32 h-16 bg-white rounded-full"></div>
        <div class="cloud cloud-3 absolute w-20 h-10 bg-white rounded-full"></div>
        <div class="cloud cloud-4 absolute w-28 h-14 bg-white rounded-full"></div>
        <div class="cloud cloud-5 absolute w-36 h-18 bg-white rounded-full"></div>
      </div>
      
      <div class="max-w-6xl mx-auto px-4 relative z-10">
        <div class="text-center">
          <div class="inline-block mb-6">
            <div class="w-24 h-24 bg-gold rounded-full border-4 border-pokemon-black mx-auto mb-4
                        flex items-center justify-center float-animation shadow-lg relative overflow-hidden">
              <div class="absolute inset-0 bg-gradient-to-b from-white/40 to-transparent"></div>
              <span class="text-pokemon-black font-bold text-4xl relative z-10">A</span>
            </div>
          </div>
          <h1 class="pixel-text text-2xl md:text-4xl text-pokemon-black mb-4 drop-shadow-sm">
            AdoruWorld
          </h1>
          <p class="text-pokemon-dark-gray text-lg md:text-xl max-w-2xl mx-auto mb-6">
            口袋妖怪风格的像素艺术个人博客
          </p>
          <p class="text-pokemon-dark-gray/80 text-sm max-w-xl mx-auto">
            探索技术、生活、游戏与创意的像素世界
          </p>
        </div>
      </div>
    </section>

    <section class="py-8 border-y-4 border-pokemon-black overflow-hidden relative">
      <div class="absolute inset-0 gold-pattern"></div>
      <div class="max-w-6xl mx-auto px-4 relative z-10">
        <div class="grid grid-cols-2 md:grid-cols-4 gap-4">
          <div class="text-center p-4 bg-white/80 rounded border-4 border-pokemon-black shadow-md relative overflow-hidden">
            <div class="absolute inset-0 bg-gradient-to-b from-white/50 to-transparent pointer-events-none"></div>
            <div class="pixel-text text-xl text-sky-dark relative z-10">{{ store.stats.totalPosts }}</div>
            <div class="text-pokemon-dark-gray text-xs mt-1 relative z-10">篇文章</div>
          </div>
          <div class="text-center p-4 bg-white/80 rounded border-4 border-pokemon-black shadow-md relative overflow-hidden">
            <div class="absolute inset-0 bg-gradient-to-b from-white/50 to-transparent pointer-events-none"></div>
            <div class="pixel-text text-xl text-sky-dark relative z-10">{{ store.stats.totalWords.toLocaleString() }}</div>
            <div class="text-pokemon-dark-gray text-xs mt-1 relative z-10">总字数</div>
          </div>
          <div class="text-center p-4 bg-white/80 rounded border-4 border-pokemon-black shadow-md relative overflow-hidden">
            <div class="absolute inset-0 bg-gradient-to-b from-white/50 to-transparent pointer-events-none"></div>
            <div class="pixel-text text-xl text-sky-dark relative z-10">{{ store.stats.totalViews.toLocaleString() }}</div>
            <div class="text-pokemon-dark-gray text-xs mt-1 relative z-10">阅读量</div>
          </div>
          <div class="text-center p-4 bg-white/80 rounded border-4 border-pokemon-black shadow-md relative overflow-hidden">
            <div class="absolute inset-0 bg-gradient-to-b from-white/50 to-transparent pointer-events-none"></div>
            <div class="pixel-text text-xl text-sky-dark relative z-10">{{ store.stats.runningDays }}</div>
            <div class="text-pokemon-dark-gray text-xs mt-1 relative z-10">运行天数</div>
          </div>
        </div>
      </div>
    </section>

    <section class="py-12">
      <div class="max-w-6xl mx-auto px-4">
        <div class="flex items-center gap-3 mb-8">
          <span class="text-2xl">⭐</span>
          <h2 class="pixel-text text-lg text-pokemon-black">精选文章</h2>
        </div>
        <div class="grid grid-cols-1 md:grid-cols-2 gap-6 mb-12">
          <router-link
            v-for="post in store.featuredPosts"
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
              :featured="post.featured"
            />
          </router-link>
        </div>

        <div class="grid grid-cols-1 lg:grid-cols-3 gap-8">
          <div class="lg:col-span-2">
            <div class="flex items-center gap-3 mb-6">
              <span class="text-2xl">📝</span>
              <h2 class="pixel-text text-lg text-pokemon-black">最新文章</h2>
            </div>
            <div class="grid grid-cols-1 md:grid-cols-2 gap-6">
              <router-link
                v-for="post in store.recentPosts"
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

            <div class="mt-8 text-center">
              <router-link
                to="/archives"
                class="pixel-btn bg-sky-dark text-white inline-block hover:bg-pokemon-blue"
              >
                查看全部文章
              </router-link>
            </div>
          </div>

          <div class="lg:col-span-1">
            <Sidebar />
          </div>
        </div>
      </div>
    </section>

    <section class="py-12 overflow-hidden relative">
      <div class="absolute inset-0 gold-pattern opacity-50"></div>
      <div class="max-w-6xl mx-auto px-4 relative z-10">
        <div class="pixel-card p-8 text-center">
          <h2 class="pixel-text text-lg text-pokemon-black mb-4">🎮 博客等级</h2>
          <div class="max-w-md mx-auto">
            <ExpBar :percentage="75" label="经验值" />
            <p class="text-sm text-pokemon-dark-gray mt-4">
              继续阅读更多文章来提升等级！
            </p>
          </div>
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

@keyframes cloud-drift-4 {
  0% { left: -180px; top: 35%; }
  100% { left: calc(100% + 180px); top: 38%; }
}

@keyframes cloud-drift-5 {
  0% { left: -220px; top: 45%; }
  100% { left: calc(100% + 220px); top: 42%; }
}

.cloud-1 {
  opacity: 0.7;
  animation: cloud-drift-1 35s linear infinite;
  animation-delay: 0s;
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

.cloud-4 {
  opacity: 0.65;
  animation: cloud-drift-4 40s linear infinite;
  animation-delay: -20s;
}

.cloud-5 {
  opacity: 0.55;
  animation: cloud-drift-5 50s linear infinite;
  animation-delay: -30s;
}
</style>
