<script setup lang="ts">
import Layout from '@/components/layout/Layout.vue'
import ExpBar from '@/components/ui/ExpBar.vue'
import { useBlogStore } from '@/stores/blog'

const store = useBlogStore()

const skills = [
  { name: 'Vue.js', level: 90 },
  { name: 'TypeScript', level: 85 },
  { name: 'React', level: 75 },
  { name: 'Node.js', level: 80 },
  { name: 'CSS/Tailwind', level: 88 },
  { name: 'Python', level: 70 }
]

const timeline = [
  { year: '2024', title: '开始开发像素风格博客', description: '创建AdoruWorld项目' },
  { year: '2023', title: '深入学习前端技术', description: 'Vue 3, TypeScript, Tailwind CSS' },
  { year: '2022', title: '开始编程之旅', description: '学习Web开发基础' }
]
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
        <div class="w-32 h-32 mx-auto mb-6 rounded-full border-4 border-pokemon-black overflow-hidden float-animation bg-gold">
          <img
            :src="store.authorInfo.avatar"
            :alt="store.authorInfo.name"
            class="w-full h-full object-cover"
          />
        </div>
        <h1 class="pixel-text text-2xl text-pokemon-black mb-4 drop-shadow-sm">{{ store.authorInfo.name }}</h1>
        <p class="text-pokemon-dark-gray max-w-xl mx-auto">{{ store.authorInfo.bio }}</p>
      </div>
    </section>

    <section class="py-12">
      <div class="max-w-4xl mx-auto px-4">
        <div class="grid grid-cols-1 md:grid-cols-2 gap-8 mb-12">
          <div class="pixel-card p-6">
            <h2 class="pixel-text text-sm text-pokemon-black mb-4 flex items-center gap-2">
              <span>👤</span> 关于我
            </h2>
            <div class="space-y-3 text-sm text-pokemon-dark-gray">
              <p>
                你好！我是 {{ store.authorInfo.name }}，一个热爱编程和游戏的开发者。
              </p>
              <p>
                我喜欢探索新技术，同时也对像素艺术和游戏开发充满热情。
                这个博客是我用 Vue 3 和 Tailwind CSS 构建的口袋妖怪风格个人空间。
              </p>
              <p>
                在这里，我会分享技术笔记、生活随笔、项目记录以及游戏心得。
              </p>
            </div>
          </div>

          <div class="pixel-card p-6">
            <h2 class="pixel-text text-sm text-pokemon-black mb-4 flex items-center gap-2">
              <span>📍</span> 联系方式
            </h2>
            <div class="space-y-3">
              <div class="flex items-center gap-3">
                <div class="w-10 h-10 bg-sky-light rounded flex items-center justify-center">
                  <svg class="w-5 h-5 text-sky-dark" fill="currentColor" viewBox="0 0 24 24">
                    <path d="M12 0c-6.626 0-12 5.373-12 12 0 5.302 3.438 9.8 8.207 11.387.599.111.793-.261.793-.577v-2.234c-3.338.726-4.033-1.416-4.033-1.416-.546-1.387-1.333-1.756-1.333-1.756-1.089-.745.083-.729.083-.729 1.205.084 1.839 1.237 1.839 1.237 1.07 1.834 2.807 1.304 3.492.997.107-.775.418-1.305.762-1.604-2.665-.305-5.467-1.334-5.467-5.931 0-1.311.469-2.381 1.236-3.221-.124-.303-.535-1.524.117-3.176 0 0 1.008-.322 3.301 1.23.957-.266 1.983-.399 3.003-.404 1.02.005 2.047.138 3.006.404 2.291-1.552 3.297-1.23 3.297-1.23.653 1.653.242 2.874.118 3.176.77.84 1.235 1.911 1.235 3.221 0 4.609-2.807 5.624-5.479 5.921.43.372.823 1.102.823 2.222v3.293c0 .319.192.694.801.576 4.765-1.589 8.199-6.086 8.199-11.386 0-6.627-5.373-12-12-12z"/>
                  </svg>
                </div>
                <div>
                  <div class="text-xs text-pokemon-dark-gray">GitHub</div>
                  <a :href="store.authorInfo.social.github" target="_blank" class="text-sm text-sky-dark hover:underline">
                    {{ store.authorInfo.name }}
                  </a>
                </div>
              </div>
              <div class="flex items-center gap-3">
                <div class="w-10 h-10 bg-sky-light rounded flex items-center justify-center">
                  <svg class="w-5 h-5 text-sky-dark" fill="currentColor" viewBox="0 0 24 24">
                    <path d="M18.244 2.25h3.308l-7.227 8.26 8.502 11.24H16.17l-5.214-6.817L4.99 21.75H1.68l7.73-8.835L1.254 2.25H8.08l4.713 6.231zm-1.161 17.52h1.833L7.084 4.126H5.117z"/>
                  </svg>
                </div>
                <div>
                  <div class="text-xs text-pokemon-dark-gray">Twitter</div>
                  <a :href="store.authorInfo.social.twitter" target="_blank" class="text-sm text-sky-dark hover:underline">
                    @{{ store.authorInfo.name }}
                  </a>
                </div>
              </div>
              <div class="flex items-center gap-3">
                <div class="w-10 h-10 bg-sky-light rounded flex items-center justify-center">
                  <svg class="w-5 h-5 text-sky-dark" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                    <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" 
                          d="M3 8l7.89 5.26a2 2 0 002.22 0L21 8M5 19h14a2 2 0 002-2V7a2 2 0 00-2-2H5a2 2 0 00-2 2v10a2 2 0 002 2z" />
                  </svg>
                </div>
                <div>
                  <div class="text-xs text-pokemon-dark-gray">Email</div>
                  <a :href="`mailto:${store.authorInfo.social.email}`" class="text-sm text-sky-dark hover:underline">
                    {{ store.authorInfo.social.email }}
                  </a>
                </div>
              </div>
            </div>
          </div>
        </div>

        <div class="pixel-card p-6 mb-12">
          <h2 class="pixel-text text-sm text-pokemon-black mb-6 flex items-center gap-2">
            <span>⚔️</span> 技能等级
          </h2>
          <div class="grid grid-cols-1 md:grid-cols-2 gap-6">
            <div v-for="skill in skills" :key="skill.name">
              <div class="flex justify-between items-center mb-2">
                <span class="text-sm text-pokemon-black">{{ skill.name }}</span>
                <span class="pixel-text text-xs text-gold-dark">Lv.{{ Math.round(skill.level / 10) }}</span>
              </div>
              <ExpBar :percentage="skill.level" />
            </div>
          </div>
        </div>

        <div class="pixel-card p-6">
          <h2 class="pixel-text text-sm text-pokemon-black mb-6 flex items-center gap-2">
            <span>📜</span> 时间线
          </h2>
          <div class="relative">
            <div class="absolute left-4 top-0 bottom-0 w-1 bg-gold/50"></div>
            <div class="space-y-6">
              <div
                v-for="(item, index) in timeline"
                :key="index"
                class="relative pl-12"
              >
                <div class="absolute left-2 w-5 h-5 bg-gold rounded-full border-4 border-pokemon-black"></div>
                <div class="pixel-card p-4">
                  <div class="pixel-text text-xs text-sky-dark mb-1">{{ item.year }}</div>
                  <h3 class="text-sm text-pokemon-black font-medium mb-1">{{ item.title }}</h3>
                  <p class="text-xs text-pokemon-dark-gray">{{ item.description }}</p>
                </div>
              </div>
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
