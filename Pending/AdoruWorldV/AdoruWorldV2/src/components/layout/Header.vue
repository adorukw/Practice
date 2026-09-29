<script setup lang="ts">
import { ref } from 'vue'
import { useRouter, useRoute } from 'vue-router'
import { useBlogStore } from '@/stores/blog'
import { navItems } from '@/data/mock'

const router = useRouter()
const route = useRoute()
const store = useBlogStore()

const isMenuOpen = ref(false)
const searchInput = ref('')

const isActive = (path: string) => {
  return route.path === path
}

const handleSearch = () => {
  if (searchInput.value.trim()) {
    store.setSearchQuery(searchInput.value)
    router.push('/search')
  }
}

const toggleMenu = () => {
  isMenuOpen.value = !isMenuOpen.value
}
</script>

<template>
  <header class="sticky top-0 z-50 border-b-4 border-pokemon-black overflow-hidden">
    <div class="absolute inset-0 sky-gradient"></div>
    
    <div class="absolute inset-0 overflow-hidden pointer-events-none">
      <div class="cloud cloud-1 absolute top-1 w-20 h-10 bg-white rounded-full opacity-70"></div>
      <div class="cloud cloud-2 absolute top-3 w-28 h-14 bg-white rounded-full opacity-60"></div>
      <div class="cloud cloud-3 absolute top-0 w-16 h-8 bg-white rounded-full opacity-75"></div>
      <div class="cloud cloud-4 absolute top-5 w-24 h-12 bg-white rounded-full opacity-65"></div>
      <div class="cloud cloud-5 absolute top-2 w-32 h-16 bg-white rounded-full opacity-55"></div>
    </div>
    
    <div class="max-w-6xl mx-auto px-4 relative z-10">
      <div class="flex items-center justify-between h-16">
        <router-link to="/" class="flex items-center gap-3 group">
          <div class="w-10 h-10 bg-gold rounded-full border-4 border-pokemon-black flex items-center justify-center
                      group-hover:scale-110 transition-transform shadow-md relative overflow-hidden">
            <div class="absolute inset-0 bg-gradient-to-b from-white/40 to-transparent"></div>
            <span class="text-pokemon-black font-bold text-lg relative z-10">A</span>
          </div>
          <span class="pixel-text text-gold-dark text-sm hidden sm:block drop-shadow-sm">
            AdoruWorld
          </span>
        </router-link>

        <nav class="hidden md:flex items-center gap-1">
          <router-link
            v-for="item in navItems"
            :key="item.path"
            :to="item.path"
            class="px-4 py-2 text-pokemon-black pixel-text text-xs transition-all duration-100
                   hover:bg-gold hover:shadow-md rounded border-2 border-transparent
                   hover:border-pokemon-black relative overflow-hidden"
            :class="{ 'bg-gold border-pokemon-black shadow-md': isActive(item.path) }"
          >
            <span class="relative z-10">{{ item.name }}</span>
            <div class="absolute inset-0 bg-gradient-to-b from-white/30 to-transparent pointer-events-none"></div>
          </router-link>
        </nav>

        <div class="flex items-center gap-3">
          <div class="relative hidden sm:block">
            <input
              v-model="searchInput"
              type="text"
              placeholder="搜索..."
              class="pixel-input w-40 text-sm py-2 pr-10 bg-white/90"
              @keyup.enter="handleSearch"
            />
            <button
              @click="handleSearch"
              class="absolute right-2 top-1/2 -translate-y-1/2 text-pokemon-dark-gray hover:text-sky-dark transition-colors"
            >
              <svg class="w-5 h-5" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" 
                      d="M21 21l-6-6m2-5a7 7 0 11-14 0 7 7 0 0114 0z" />
              </svg>
            </button>
          </div>

          <button
            @click="toggleMenu"
            class="md:hidden p-2 text-pokemon-black hover:bg-gold rounded transition-colors border-2 border-transparent hover:border-pokemon-black"
          >
            <svg v-if="!isMenuOpen" class="w-6 h-6" fill="none" stroke="currentColor" viewBox="0 0 24 24">
              <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M4 6h16M4 12h16M4 18h16" />
            </svg>
            <svg v-else class="w-6 h-6" fill="none" stroke="currentColor" viewBox="0 0 24 24">
              <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M6 18L18 6M6 6l12 12" />
            </svg>
          </button>
        </div>
      </div>

      <div
        v-if="isMenuOpen"
        class="md:hidden border-t-2 border-sky-dark/30 py-4 bg-white/20 backdrop-blur-sm rounded-b-lg"
      >
        <nav class="flex flex-col gap-2">
          <router-link
            v-for="item in navItems"
            :key="item.path"
            :to="item.path"
            class="px-4 py-3 text-pokemon-black pixel-text text-xs transition-all duration-100
                   hover:bg-gold rounded"
            :class="{ 'bg-gold': isActive(item.path) }"
            @click="isMenuOpen = false"
          >
            {{ item.name }}
          </router-link>
        </nav>
        <div class="mt-4 relative">
          <input
            v-model="searchInput"
            type="text"
            placeholder="搜索..."
            class="pixel-input w-full text-sm py-2 pr-10 bg-white/90"
            @keyup.enter="handleSearch"
          />
          <button
            @click="handleSearch"
            class="absolute right-2 top-1/2 -translate-y-1/2 text-pokemon-dark-gray hover:text-sky-dark transition-colors"
          >
            <svg class="w-5 h-5" fill="none" stroke="currentColor" viewBox="0 0 24 24">
              <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" 
                    d="M21 21l-6-6m2-5a7 7 0 11-14 0 7 7 0 0114 0z" />
            </svg>
          </button>
        </div>
      </div>
    </div>
  </header>
</template>

<style scoped>
.cloud {
  filter: blur(2px);
  box-shadow: 
    0 0 20px rgba(255, 255, 255, 0.5),
    inset 0 -5px 10px rgba(200, 220, 240, 0.3);
}

.cloud::before,
.cloud::after {
  content: '';
  position: absolute;
  background: white;
  border-radius: 50%;
}

.cloud::before {
  width: 60%;
  height: 80%;
  top: 20%;
  left: -20%;
}

.cloud::after {
  width: 50%;
  height: 70%;
  top: 30%;
  right: -15%;
}

@keyframes cloud-drift-1 {
  0% { left: -120px; }
  100% { left: calc(100% + 120px); }
}

@keyframes cloud-drift-2 {
  0% { left: -180px; }
  100% { left: calc(100% + 180px); }
}

@keyframes cloud-drift-3 {
  0% { left: -100px; }
  100% { left: calc(100% + 100px); }
}

@keyframes cloud-drift-4 {
  0% { left: -150px; }
  100% { left: calc(100% + 150px); }
}

@keyframes cloud-drift-5 {
  0% { left: -200px; }
  100% { left: calc(100% + 200px); }
}

.cloud-1 {
  animation: cloud-drift-1 25s linear infinite;
  animation-delay: 0s;
}

.cloud-2 {
  animation: cloud-drift-2 35s linear infinite;
  animation-delay: -10s;
}

.cloud-3 {
  animation: cloud-drift-3 20s linear infinite;
  animation-delay: -5s;
}

.cloud-4 {
  animation: cloud-drift-4 30s linear infinite;
  animation-delay: -15s;
}

.cloud-5 {
  animation: cloud-drift-5 40s linear infinite;
  animation-delay: -20s;
}
</style>
