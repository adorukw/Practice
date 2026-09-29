<script setup>
import { useRouter } from 'vue-router'
import { useCollectionStore } from '../stores/collection'

const store = useCollectionStore()
const router = useRouter()

const props = defineProps({
  work: { type: Object, required: true },
})

const statusLabels = {
  playing: '在玩',
  played: '已玩',
  want: '想玩',
  watched: '已看',
  watching: '在看',
  wantwatch: '想看',
  listened: '已听',
  listening: '在听',
  wantlisten: '想听',
  read: '已读',
  reading: '在读',
  wantread: '想读',
  completed: '已完成',
  'in-progress': '进行中',
}

const statusColors = {
  playing: 'bg-pokemon-green',
  played: 'bg-pokemon-darkgray',
  want: 'bg-pokemon-yellow text-pokemon-dark',
  watched: 'bg-pokemon-darkgray',
  watching: 'bg-pokemon-green',
  wantwatch: 'bg-pokemon-yellow text-pokemon-dark',
  listened: 'bg-pokemon-darkgray',
  listening: 'bg-pokemon-green',
  wantlisten: 'bg-pokemon-yellow text-pokemon-dark',
  read: 'bg-pokemon-darkgray',
  reading: 'bg-pokemon-green',
  wantread: 'bg-pokemon-yellow text-pokemon-dark',
  completed: 'bg-pokemon-blue',
  'in-progress': 'bg-pokemon-green',
}

function goToDetail() {
  router.push(`/work/${props.work.id}`)
}
</script>

<template>
  <div
    class="pixel-border bg-pokemon-card dark:bg-pokemon-dark cursor-pointer transition-all duration-200 hover:translate-y-[-4px] hover:shadow-[6px_6px_0px_0px_var(--color-pokemon-shadow)] dark:hover:shadow-[6px_6px_0px_0px_#111] group"
    @click="goToDetail"
  >
    <div class="relative overflow-hidden">
      <div class="aspect-[4/3] bg-pokemon-bg dark:bg-pokemon-darker flex items-center justify-center p-4">
        <img
          :src="work.coverImage"
          :alt="work.title"
          class="w-20 h-20 object-contain group-hover:scale-110 transition-transform duration-300"
          loading="lazy"
        />
      </div>
      <div
        v-if="work.status && statusLabels[work.status]"
        :class="[
          'absolute top-2 right-2 px-2 py-0.5 text-[7px] font-pixel text-white pixel-border-thin',
          statusColors[work.status] || 'bg-pokemon-darkgray',
        ]"
      >
        {{ statusLabels[work.status] }}
      </div>
      <div class="absolute top-2 left-2 px-2 py-0.5 text-[7px] font-pixel text-white bg-pokemon-accent pixel-border-thin">
        #{{ work.id }}
      </div>
    </div>
    <div class="p-3">
      <h3 class="font-pixel text-[9px] text-pokemon-dark dark:text-pokemon-light mb-2 line-clamp-2 leading-relaxed">
        {{ work.title }}
      </h3>
      <div class="flex flex-wrap gap-1">
        <span
          v-for="tag in work.tags.slice(0, 2)"
          :key="tag"
          class="text-[7px] font-pixel px-1.5 py-0.5 bg-pokemon-highlight dark:bg-pokemon-blue/30 text-pokemon-dark dark:text-pokemon-yellow"
        >
          {{ tag }}
        </span>
      </div>
    </div>
  </div>
</template>
