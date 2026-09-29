<script setup>
import { computed } from 'vue'
import { useMediaStore } from '@/stores/mediaStore'

const props = defineProps({
  item: { type: Object, required: true }
})

defineEmits(['click'])

const store = useMediaStore()

const dexColorMap = {
  anime: { border: 'border-dex-blue-dark', accent: 'text-dex-blue', bg: 'bg-dex-blue', dot: 'bg-dex-blue' },
  movie: { border: 'border-dex-red-dark', accent: 'text-dex-red', bg: 'bg-dex-red', dot: 'bg-dex-red' },
  game: { border: 'border-dex-green-dark', accent: 'text-dex-green-dark', bg: 'bg-dex-green', dot: 'bg-dex-green' }
}

const statusBadgeMap = {
  completed: { bg: 'bg-dex-green text-white', dot: 'bg-white' },
  watching: { bg: 'bg-dex-blue text-white', dot: 'bg-white' },
  playing: { bg: 'bg-dex-blue text-white', dot: 'bg-white' },
  plan: { bg: 'bg-dex-gray text-white', dot: 'bg-white' },
  dropped: { bg: 'bg-dex-red text-white', dot: 'bg-white' },
  paused: { bg: 'bg-dex-amber text-dex-darker', dot: 'bg-dex-darker' }
}

const dexColors = computed(() => dexColorMap[props.item.type] || dexColorMap.anime)
const badgeStyle = computed(() => statusBadgeMap[props.item.status] || statusBadgeMap.plan)

function getStars(rating) {
  if (!rating) return '☆☆☆☆☆'
  const filled = Math.round(rating / 2)
  return '★'.repeat(filled) + '☆'.repeat(5 - filled)
}

function getStatusLabel() {
  return store.statusMap[props.item.type]?.[props.item.status] || props.item.status
}
</script>

<template>
  <button
    @click="$emit('click', item)"
    :class="[
      'w-full text-left cursor-pointer group',
      'bg-dex-bg border-2',
      dexColors.border,
      'pixel-shadow-sm',
      'hover:pixel-shadow hover:-translate-y-0.5 transition-all duration-100'
    ]"
  >
    <div class="flex">
      <div class="w-14 shrink-0 flex flex-col border-r-2 border-dashed border-dex-dark/20 items-center justify-center py-2 gap-0.5 bg-dex-window/50">
        <span class="text-xs font-pokemon pixel-text" :class="dexColors.accent">No.</span>
        <span class="text-sm font-pokemon pixel-text font-bold text-dex-darker">{{ String(item.id).padStart(3, '0') }}</span>
      </div>

      <div class="flex-1 min-w-0 p-3">
        <div class="flex items-start justify-between gap-2">
          <div class="min-w-0">
            <h3 class="font-bold text-sm text-dex-darker pixel-text truncate">{{ item.title }}</h3>
            <p v-if="item.titleEn" class="text-xs text-dex-gray-dark pixel-text mt-0.5 truncate opacity-70">{{ item.titleEn }}</p>
          </div>
          <span
            :class="['text-xs px-2 py-0.5 font-bold pixel-text shrink-0 border border-black/20', badgeStyle.bg]"
          >
            <span class="inline-block w-1.5 h-1.5 mr-1 rounded-none" :class="badgeStyle.dot" />
            {{ getStatusLabel() }}
          </span>
        </div>

        <div class="mt-2 flex items-center gap-2 text-xs">
          <span class="pixel-text text-dex-amber text-xs">{{ getStars(item.rating) }}</span>
          <span class="pixel-text text-dex-darker font-bold">{{ item.rating }}/10</span>
          <span v-if="item.year" class="text-dex-gray-dark/60 ml-auto font-pixel">{{ item.year }}</span>
        </div>

        <div v-if="item.tags && item.tags.length > 0" class="flex flex-wrap gap-1 mt-2">
          <span
            v-for="tag in item.tags"
            :key="tag"
            class="text-xs px-1.5 py-0 bg-dex-window border border-dex-dark/20 text-dex-gray-dark font-pixel"
          >
            {{ tag }}
          </span>
        </div>

        <div v-if="item.platform" class="mt-1.5">
          <span class="text-xs bg-dex-purple-dark text-white px-1.5 py-0 font-pixel border border-black/20">
            {{ item.platform }}
          </span>
        </div>
      </div>
    </div>
  </button>
</template>
