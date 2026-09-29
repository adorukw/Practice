<script setup lang="ts">
defineProps<{
  title?: string
  coverImage?: string
  tags?: string[]
  category?: string[]
  date?: string
  readingTime?: number
  views?: number
  featured?: boolean
}>()
</script>

<template>
  <article class="pixel-card overflow-hidden group cursor-pointer">
    <div class="relative overflow-hidden" v-if="coverImage">
      <img
        :src="coverImage"
        :alt="title"
        class="w-full h-48 object-cover transition-transform duration-300 group-hover:scale-105"
        style="image-rendering: auto;"
      />
      <div class="absolute inset-0 border-4 border-pokemon-black/20 pointer-events-none"></div>
      <div
        v-if="featured"
        class="absolute top-3 right-3 bg-pokemon-red text-white pixel-text text-xs px-3 py-1 border-3 border-pokemon-black shadow-md relative overflow-hidden"
      >
        <div class="absolute inset-0 bg-gradient-to-b from-white/30 to-transparent"></div>
        <span class="relative z-10">精选</span>
      </div>
    </div>
    <div class="p-5 relative">
      <div class="absolute top-0 left-0 right-0 h-1 bg-gradient-to-r from-transparent via-pokemon-black/10 to-transparent"></div>
      
      <div class="flex flex-wrap gap-2 mb-3" v-if="category?.length">
        <span
          v-for="cat in category"
          :key="cat"
          class="tag bg-sky text-white"
        >
          {{ cat }}
        </span>
      </div>
      <h3 class="pixel-text text-sm text-pokemon-black mb-2 line-clamp-2 group-hover:text-sky-dark transition-colors">
        {{ title }}
      </h3>
      <div class="flex items-center gap-4 text-xs text-pokemon-dark-gray mb-3">
        <span v-if="date" class="flex items-center gap-1">
          <svg class="w-4 h-4" fill="none" stroke="currentColor" viewBox="0 0 24 24">
            <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" 
                  d="M8 7V3m8 4V3m-9 8h10M5 21h14a2 2 0 002-2V7a2 2 0 00-2-2H5a2 2 0 00-2 2v12a2 2 0 002 2z" />
          </svg>
          {{ date }}
        </span>
        <span v-if="readingTime" class="flex items-center gap-1">
          <svg class="w-4 h-4" fill="none" stroke="currentColor" viewBox="0 0 24 24">
            <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" 
                  d="M12 8v4l3 3m6-3a9 9 0 11-18 0 9 9 0 0118 0z" />
          </svg>
          {{ readingTime }} 分钟
        </span>
        <span v-if="views" class="flex items-center gap-1">
          <svg class="w-4 h-4" fill="none" stroke="currentColor" viewBox="0 0 24 24">
            <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" 
                  d="M15 12a3 3 0 11-6 0 3 3 0 016 0z" />
            <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" 
                  d="M2.458 12C3.732 7.943 7.523 5 12 5c4.478 0 8.268 2.943 9.542 7-1.274 4.057-5.064 7-9.542 7-4.477 0-8.268-2.943-9.542-7z" />
          </svg>
          {{ views }}
        </span>
      </div>
      <div class="flex flex-wrap gap-2" v-if="tags?.length">
        <span
          v-for="tag in tags.slice(0, 3)"
          :key="tag"
          class="text-xs px-2 py-1 bg-gold-light/80 border-2 border-pokemon-black text-pokemon-dark-gray shadow-sm"
        >
          #{{ tag }}
        </span>
        <span
          v-if="tags.length > 3"
          class="text-xs px-2 py-1 bg-pokemon-gray/50 border-2 border-pokemon-black text-pokemon-dark-gray shadow-sm"
        >
          +{{ tags.length - 3 }}
        </span>
      </div>
    </div>
  </article>
</template>
