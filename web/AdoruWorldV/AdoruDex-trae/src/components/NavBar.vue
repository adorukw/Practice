<script setup>
import { ref } from 'vue'
import { useRouter } from 'vue-router'
import { useCollectionStore } from '../stores/collection'

const store = useCollectionStore()
const router = useRouter()
const searchOpen = ref(false)
const searchInput = ref('')
const mobileMenuOpen = ref(false)

function handleSearch() {
  store.setSearchQuery(searchInput.value)
  if (searchInput.value) {
    router.push({ name: 'Home' })
  }
}

function clearSearch() {
  searchInput.value = ''
  store.setSearchQuery('')
  searchOpen.value = false
}
</script>

<template>
  <nav class="sticky top-0 z-50 bg-pokemon-blue dark:bg-pokemon-darker pixel-border border-t-0 border-l-0 border-r-0">
    <div class="max-w-6xl mx-auto px-4 py-3 flex items-center justify-between">
      <router-link to="/" class="flex items-center gap-3 group">
        <div class="w-10 h-10 relative">
          <div class="w-10 h-10 rounded-full bg-pokemon-red border-3 border-pokemon-border relative overflow-hidden">
            <div class="absolute top-1/2 left-0 right-0 h-[3px] bg-pokemon-border"></div>
            <div class="absolute top-1/2 left-1/2 -translate-x-1/2 -translate-y-1/2 w-3 h-3 rounded-full bg-pokemon-light border-2 border-pokemon-border"></div>
            <div class="absolute top-0 left-0 right-0 h-1/2 bg-pokemon-red"></div>
            <div class="absolute bottom-0 left-0 right-0 h-1/2 bg-white dark:bg-pokemon-light"></div>
          </div>
        </div>
        <span class="font-pixel text-[12px] text-pokemon-yellow pixel-text-shadow tracking-wider group-hover:text-white transition-colors">
          AdoruDex
        </span>
      </router-link>

      <div class="hidden md:flex items-center gap-1">
        <router-link
          v-for="cat in store.allCategories"
          :key="cat.slug"
          :to="`/category/${cat.slug}`"
          class="font-pixel text-[8px] px-3 py-2 text-white hover:bg-pokemon-red transition-colors tracking-wider"
          active-class="bg-pokemon-red!"
        >
          {{ cat.icon }} {{ cat.name }}
        </router-link>
      </div>

      <div class="flex items-center gap-2">
        <button
          class="w-9 h-9 pixel-border-thin bg-pokemon-light dark:bg-pokemon-dark flex items-center justify-center cursor-pointer hover:bg-pokemon-yellow transition-colors"
          @click="searchOpen = !searchOpen"
        >
          <span class="text-[12px]">🔍</span>
        </button>

        <button
          class="w-9 h-9 pixel-border-thin bg-pokemon-light dark:bg-pokemon-dark flex items-center justify-center cursor-pointer hover:bg-pokemon-yellow transition-colors"
          @click="store.toggleDarkMode()"
        >
          <span class="text-[12px]">{{ store.isDarkMode ? '☀️' : '🌙' }}</span>
        </button>

        <button
          class="md:hidden w-9 h-9 pixel-border-thin bg-pokemon-light dark:bg-pokemon-dark flex items-center justify-center cursor-pointer hover:bg-pokemon-yellow transition-colors"
          @click="mobileMenuOpen = !mobileMenuOpen"
        >
          <span class="text-[12px]">{{ mobileMenuOpen ? '✕' : '☰' }}</span>
        </button>
      </div>
    </div>

    <div v-if="searchOpen" class="px-4 pb-3">
      <div class="pixel-border-thin bg-pokemon-light dark:bg-pokemon-dark flex items-center">
        <span class="pl-3 text-[10px]">🔍</span>
        <input
          v-model="searchInput"
          type="text"
          placeholder="搜索作品..."
          class="flex-1 bg-transparent px-3 py-2 font-pixel text-[9px] text-pokemon-dark dark:text-pokemon-light outline-none placeholder:text-pokemon-darkgray"
          @keyup.enter="handleSearch"
        />
        <button
          v-if="searchInput"
          class="pr-3 text-[10px] cursor-pointer hover:text-pokemon-red"
          @click="clearSearch"
        >
          ✕
        </button>
      </div>
    </div>

    <div v-if="mobileMenuOpen" class="md:hidden px-4 pb-3">
      <div class="pixel-border bg-pokemon-light dark:bg-pokemon-dark p-2">
        <router-link
          v-for="cat in store.allCategories"
          :key="cat.slug"
          :to="`/category/${cat.slug}`"
          class="block font-pixel text-[9px] px-3 py-2 text-pokemon-dark dark:text-pokemon-light hover:bg-pokemon-blue hover:text-white transition-colors"
          @click="mobileMenuOpen = false"
        >
          {{ cat.icon }} {{ cat.name }}
        </router-link>
      </div>
    </div>
  </nav>
</template>
