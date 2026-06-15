<script setup lang="ts">
import { computed } from 'vue'
import { Handle, Position } from '@vue-flow/core'
import dayjs from 'dayjs'
import { useSessionStore } from '../../store/session'

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

const store = useSessionStore()
const isSelected = computed(() => store.selectedNodeId === props.id)
const timeStr = computed(() =>
  props.data.timestamp ? dayjs(props.data.timestamp).format('HH:mm:ss') : ''
)
</script>

<template>
  <div
    class="user-node px-5 py-3.5 rounded-lg border-2 transition-all duration-200"
    :class="{
      'bg-user-bg border-user-border shadow-sm': !isSelected,
      'bg-user-bg border-user ring-2 ring-user/50 shadow-md': isSelected,
    }"
    style="width: 280px"
    @click="store.selectNode(id)"
  >
    <Handle type="target" :position="Position.Top" class="!bg-user !w-2 !h-2" />
    <div class="flex items-center gap-2 mb-2">
      <span class="w-6 h-6 flex items-center justify-center rounded bg-user text-white text-[10px] font-bold">用户</span>
      <span class="text-xs font-semibold text-user tracking-wider">用户消息</span>
      <span v-if="timeStr" class="text-xs text-gray-400 ml-auto font-mono">{{ timeStr }}</span>
    </div>
    <div class="text-sm text-gray-700 leading-relaxed overflow-hidden" style="word-break: break-word; display: -webkit-box; -webkit-line-clamp: 3; -webkit-box-orient: vertical;">{{ data.label }}</div>
    <Handle type="source" :position="Position.Bottom" class="!bg-user !w-2 !h-2" />
  </div>
</template>
