<script setup>
import { computed } from 'vue'
import { useRouter } from 'vue-router'
import { useMediaStore } from '@/stores/mediaStore'
import GbaFrame from '@/components/GbaFrame.vue'
import MediaCard from '@/components/MediaCard.vue'

const props = defineProps({
  mediaType: { type: String, required: true }
})

const router = useRouter()
const store = useMediaStore()

const filteredItems = computed(() => store.getItemsByType(props.mediaType))
const typeLabel = computed(() => store.typeLabels[props.mediaType] || props.mediaType)

const statusOptions = computed(() => {
  const map = store.statusMap[props.mediaType] || {}
  return Object.entries(map).map(([key, label]) => ({ key, label }))
})

function countByStatus(status) {
  return filteredItems.value.filter(i => i.status === status).length
}

function navigateToDetail(item) {
  router.push({ name: `${props.mediaType}-detail`, params: { id: item.id } })
}
</script>

<template>
  <div class="animate-slide-in space-y-4">
    <GbaFrame :title="`${typeLabel}档案馆`">
      <div class="flex flex-wrap gap-2 items-center">
        <span class="text-xs text-dex-gray-dark pixel-text">◆ 筛选：</span>
        <span
          v-for="opt in statusOptions"
          :key="opt.key"
          class="text-xs px-2 py-0.5 bg-dex-bg border-2 border-dex-dark/30 font-bold pixel-text text-dex-darker"
        >
          {{ opt.label }} · {{ countByStatus(opt.key) }}
        </span>
      </div>
    </GbaFrame>

    <div v-if="filteredItems.length > 0" class="space-y-2">
      <MediaCard
        v-for="item in filteredItems"
        :key="item.id"
        :item="item"
        @click="navigateToDetail"
      />
    </div>

    <div v-else class="text-center py-12 bg-dex-window border-2 border-dex-dark">
      <p class="text-4xl mb-3">📭</p>
      <p class="text-sm text-dex-gray-dark pixel-text">暂无{{ typeLabel }}记录</p>
      <p class="text-xs text-dex-gray-dark/60 mt-1 pixel-text">按 START 开始添加</p>
    </div>
  </div>
</template>
