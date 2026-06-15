<script setup lang="ts">
import { computed, ref } from 'vue'
import { Handle, Position } from '@vue-flow/core'
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
const expanded = ref(false)
const aiSummary = computed(() => store.getSummary(props.id))
</script>

<template>
  <div
    class="thinking-node px-4 py-3 rounded-lg border-2 border-dashed transition-all duration-200"
    :class="{
      'bg-thinking-bg border-thinking-border': !isSelected,
      'bg-thinking-bg border-thinking ring-2 ring-thinking/50': isSelected,
    }"
    style="width: 300px"
    @click="store.selectNode(id)"
  >
    <Handle type="target" :position="Position.Top" class="!bg-thinking !w-2 !h-2" />
    <div class="flex items-center gap-2 mb-2">
      <span class="w-6 h-6 flex items-center justify-center rounded bg-thinking text-white text-[10px] font-bold">思考</span>
      <span class="text-xs font-semibold text-thinking tracking-wider">思考过程</span>
      <button
        class="ml-auto text-xs text-gray-400 hover:text-gray-600 px-2 py-0.5 rounded hover:bg-gray-100 transition-colors"
        @click.stop="expanded = !expanded"
      >
        {{ expanded ? '收起' : '展开' }}
      </button>
    </div>
    <div v-if="aiSummary" class="text-sm text-gray-700 leading-snug mb-1.5 font-medium border-l-2 border-thinking pl-2">
      {{ aiSummary }}
    </div>
    <div v-else-if="!expanded" class="text-sm text-gray-500 italic overflow-hidden" style="word-break: break-word; display: -webkit-box; -webkit-line-clamp: 2; -webkit-box-orient: vertical;">{{ data.label }}</div>
    <div v-if="expanded && !aiSummary" class="text-sm text-gray-600 leading-relaxed whitespace-pre-wrap break-words max-h-[240px] overflow-y-auto bg-white/50 rounded p-3">
      {{ data.raw?.thinking || data.label }}
    </div>
    <Handle type="source" :position="Position.Bottom" class="!bg-thinking !w-2 !h-2" />
  </div>
</template>
