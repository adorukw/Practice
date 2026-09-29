<script setup lang="ts">
import { computed } from 'vue'
import Layout from '@/components/layout/Layout.vue'
import { useBlogStore } from '@/stores/blog'

const store = useBlogStore()

const groupedProjects = computed(() => ({
  'in-progress': (store.allProjects as any[]).filter(p => p.status === 'in-progress'),
  'completed': (store.allProjects as any[]).filter(p => p.status === 'completed'),
  'archived': (store.allProjects as any[]).filter(p => p.status === 'archived')
}))
</script>

<template>
  <Layout>
    <section class="relative py-16 overflow-hidden">
      <div class="absolute inset-0 sky-gradient"></div>
      
      <div class="absolute inset-0 overflow-hidden pointer-events-none">
        <div class="cloud absolute top-4 left-[12%] w-20 h-10 bg-white rounded-full opacity-60"></div>
        <div class="cloud absolute top-8 left-[65%] w-24 h-12 bg-white rounded-full opacity-55"></div>
      </div>
      
      <div class="max-w-6xl mx-auto px-4 text-center relative z-10">
        <h1 class="pixel-text text-2xl text-pokemon-black mb-4 drop-shadow-sm">🚀 项目</h1>
        <p class="text-pokemon-dark-gray">我的个人项目和作品集</p>
      </div>
    </section>

    <section class="py-12">
      <div class="max-w-6xl mx-auto px-4">
        <div v-if="groupedProjects['in-progress'].length" class="mb-12">
          <h2 class="pixel-text text-lg text-pokemon-black mb-6 flex items-center gap-3">
            <span class="text-gold-dark">⚡</span> 进行中
          </h2>
          <div class="grid grid-cols-1 md:grid-cols-2 gap-6">
            <div
              v-for="project in groupedProjects['in-progress']"
              :key="project.id"
              class="pixel-card overflow-hidden"
            >
              <img
                v-if="project.image"
                :src="project.image"
                :alt="project.name"
                class="w-full h-48 object-cover"
              />
              <div class="p-6">
                <div class="flex items-center justify-between mb-3">
                  <h3 class="pixel-text text-sm text-pokemon-black">{{ project.name }}</h3>
                  <span class="text-xs px-2 py-1 bg-gold text-pokemon-black rounded border-2 border-pokemon-black">
                    进行中
                  </span>
                </div>
                <p class="text-sm text-pokemon-dark-gray mb-4">{{ project.description }}</p>
                <div class="flex flex-wrap gap-2 mb-4">
                  <span
                    v-for="tech in project.tech"
                    :key="tech"
                    class="text-xs px-2 py-1 bg-sky-light/50 text-sky-dark rounded"
                  >
                    {{ tech }}
                  </span>
                </div>
                <div class="flex gap-3">
                  <a
                    v-if="project.link"
                    :href="project.link"
                    target="_blank"
                    class="pixel-btn bg-sky text-white text-xs py-2 px-4 hover:bg-sky-dark"
                  >
                    访问
                  </a>
                  <a
                    v-if="project.github"
                    :href="project.github"
                    target="_blank"
                    class="pixel-btn bg-pokemon-dark-gray text-white text-xs py-2 px-4"
                  >
                    GitHub
                  </a>
                </div>
              </div>
            </div>
          </div>
        </div>

        <div v-if="groupedProjects['completed'].length" class="mb-12">
          <h2 class="pixel-text text-lg text-pokemon-black mb-6 flex items-center gap-3">
            <span class="text-grass-dark">✅</span> 已完成
          </h2>
          <div class="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6">
            <div
              v-for="project in groupedProjects['completed']"
              :key="project.id"
              class="pixel-card overflow-hidden"
            >
              <img
                v-if="project.image"
                :src="project.image"
                :alt="project.name"
                class="w-full h-40 object-cover"
              />
              <div class="p-5">
                <h3 class="pixel-text text-xs text-pokemon-black mb-2">{{ project.name }}</h3>
                <p class="text-sm text-pokemon-dark-gray mb-3 line-clamp-2">{{ project.description }}</p>
                <div class="flex flex-wrap gap-1 mb-3">
                  <span
                    v-for="tech in project.tech.slice(0, 3)"
                    :key="tech"
                    class="text-xs px-2 py-0.5 bg-pokemon-gray/50 rounded text-pokemon-dark-gray"
                  >
                    {{ tech }}
                  </span>
                </div>
                <div class="flex gap-2">
                  <a
                    v-if="project.link"
                    :href="project.link"
                    target="_blank"
                    class="text-xs text-sky-dark hover:underline"
                  >
                    访问 →
                  </a>
                  <a
                    v-if="project.github"
                    :href="project.github"
                    target="_blank"
                    class="text-xs text-pokemon-dark-gray hover:underline"
                  >
                    GitHub →
                  </a>
                </div>
              </div>
            </div>
          </div>
        </div>

        <div v-if="groupedProjects['archived'].length">
          <h2 class="pixel-text text-lg text-pokemon-black mb-6 flex items-center gap-3">
            <span class="text-pokemon-gray">📦</span> 已归档
          </h2>
          <div class="grid grid-cols-1 md:grid-cols-3 gap-4">
            <div
              v-for="project in groupedProjects['archived']"
              :key="project.id"
              class="pixel-card p-4 opacity-75 hover:opacity-100 transition-opacity"
            >
              <h3 class="text-sm text-pokemon-black mb-1">{{ project.name }}</h3>
              <p class="text-xs text-pokemon-dark-gray">{{ project.description }}</p>
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
