<script setup lang="ts">
import { computed } from 'vue'
import { Handle, Position } from '@vue-flow/core'
import dayjs from 'dayjs'
import { useTrajectoryStore } from '../../store/trajectory'

const props = defineProps<{
  id: string
  type: string
  data: {
    type: string
    label: string
    raw: any
    meta?: any
    timestamp: number
  }
  selected?: boolean
}>()

const store = useTrajectoryStore()
const isSelected = computed(() => store.selectedNodeId === props.id)
const timeStr = computed(() =>
  props.data.timestamp ? dayjs(props.data.timestamp).format('HH:mm:ss') : ''
)
</script>

<template>
  <div
    class="px-5 py-3.5 rounded-lg border-2 transition-all duration-200"
    :class="{
      'bg-purple-50 border-purple-300 shadow-sm': !isSelected,
      'bg-purple-50 border-purple-500 ring-2 ring-purple-500/50 shadow-md': isSelected,
    }"
    style="width: 300px"
    @click="store.selectNode(id)"
  >
    <Handle type="target" :position="Position.Top" class="!bg-purple-500 !w-2 !h-2" />
    <div class="flex items-center gap-2 mb-2">
      <span class="w-6 h-6 flex items-center justify-center rounded bg-purple-500 text-white text-[10px] font-bold">助手</span>
      <span class="text-xs font-semibold text-purple-600 tracking-wider">助手回复</span>
      <span v-if="data.meta?.model" class="text-xs text-gray-500 bg-gray-100 px-2 py-0.5 rounded font-mono">{{ data.meta.model }}</span>
      <span v-if="timeStr" class="text-xs text-gray-400 ml-auto font-mono">{{ timeStr }}</span>
    </div>
    <div class="text-sm text-gray-700 leading-relaxed overflow-hidden" style="word-break: break-word; display: -webkit-box; -webkit-line-clamp: 3; -webkit-box-orient: vertical;">{{ data.label }}</div>
    <Handle type="source" :position="Position.Bottom" class="!bg-purple-500 !w-2 !h-2" />
  </div>
</template>
