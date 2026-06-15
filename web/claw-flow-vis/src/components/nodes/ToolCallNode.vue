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

const toolName = computed(() => props.data.meta?.toolName || 'unknown')
const aiSummary = computed(() => store.getSummary(props.id))

const args = computed(() => {
  const raw = props.data.raw
  if (raw?.arguments) {
    try {
      return typeof raw.arguments === 'string' ? JSON.parse(raw.arguments) : raw.arguments
    } catch {
      return raw.arguments
    }
  }
  return null
})
</script>

<template>
  <div
    class="toolcall-node px-4 py-3 rounded-lg border-2 transition-all duration-200"
    :class="{
      'bg-toolcall-bg border-toolcall-border shadow-sm': !isSelected,
      'bg-toolcall-bg border-toolcall ring-2 ring-toolcall/50 shadow-md': isSelected,
    }"
    style="width: 300px"
    @click="store.selectNode(id)"
  >
    <Handle type="target" :position="Position.Top" class="!bg-toolcall !w-2 !h-2" />
    <div class="flex items-center gap-2 mb-2">
      <span class="w-6 h-6 flex items-center justify-center rounded bg-toolcall text-white text-[10px] font-bold">调用</span>
      <span class="text-sm font-bold text-toolcall font-mono">{{ toolName }}()</span>
      <span v-if="data.meta?.implicit" class="text-[10px] text-gray-400 bg-gray-100 px-1.5 py-0.5 rounded">隐式</span>
      <button
        class="ml-auto text-xs text-gray-400 hover:text-gray-600 px-2 py-0.5 rounded hover:bg-gray-100 transition-colors"
        @click.stop="expanded = !expanded"
      >
        {{ expanded ? '收起' : '参数' }}
      </button>
    </div>
    <div v-if="aiSummary" class="text-sm text-gray-700 leading-snug mb-1.5 font-medium border-l-2 border-toolcall pl-2">
      {{ aiSummary }}
    </div>
    <div v-else class="text-sm text-gray-600 leading-snug overflow-hidden" style="word-break: break-word; display: -webkit-box; -webkit-line-clamp: 2; -webkit-box-orient: vertical;">
      {{ data.label }}
    </div>
    <div v-if="expanded && args" class="text-xs text-gray-600 bg-white/60 rounded p-3 mt-2 max-h-[200px] overflow-y-auto whitespace-pre-wrap break-all font-mono leading-relaxed">
      {{ JSON.stringify(args, null, 2) }}
    </div>
    <Handle type="source" :position="Position.Bottom" class="!bg-toolcall !w-2 !h-2" />
  </div>
</template>
