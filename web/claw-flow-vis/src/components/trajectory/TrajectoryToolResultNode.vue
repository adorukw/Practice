<script setup lang="ts">
import { computed, ref } from 'vue'
import { Handle, Position } from '@vue-flow/core'
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
const expanded = ref(false)
const isError = computed(() => props.data.meta?.error ?? false)
const toolName = computed(() => props.data.meta?.toolName || '')
const aiSummary = computed(() => store.getSummary(props.id))

const resultText = computed(() => {
  const content = props.data.raw?.data?.message?.content
  if (Array.isArray(content)) {
    const textBlock = content.find((b: any) => b.type === 'text')
    return textBlock?.text || ''
  }
  return ''
})
</script>

<template>
  <div
    class="px-4 py-3 rounded-lg border-2 transition-all duration-200"
    :class="{
      'bg-emerald-50 border-emerald-300 shadow-sm': !isError && !isSelected,
      'bg-emerald-50 border-emerald-500 ring-2 ring-emerald-500/50 shadow-md': !isError && isSelected,
      'bg-red-50 border-red-300 shadow-sm': isError && !isSelected,
      'bg-red-50 border-red-500 ring-2 ring-red-500/50 shadow-md': isError && isSelected,
    }"
    style="width: 300px"
    @click="store.selectNode(id)"
  >
    <Handle type="target" :position="Position.Top" :class="isError ? '!bg-red-500' : '!bg-emerald-500'" class="!w-2 !h-2" />
    <div class="flex items-center gap-2 mb-2">
      <span
        class="w-6 h-6 flex items-center justify-center rounded text-white text-[10px] font-bold"
        :class="isError ? 'bg-red-500' : 'bg-emerald-500'"
      >
        {{ isError ? '错误' : '结果' }}
      </span>
      <span
        class="text-xs font-semibold tracking-wider"
        :class="isError ? 'text-red-600' : 'text-emerald-600'"
      >
        {{ isError ? '执行错误' : '执行结果' }}
      </span>
      <span v-if="toolName" class="text-xs text-gray-500 bg-gray-100 px-2 py-0.5 rounded font-mono">{{ toolName }}</span>
      <span v-if="data.meta?.durationMs" class="text-xs text-gray-400 font-mono">{{ data.meta.durationMs }}ms</span>
      <button
        class="ml-auto text-xs text-gray-400 hover:text-gray-600 px-2 py-0.5 rounded hover:bg-gray-100 transition-colors"
        @click.stop="expanded = !expanded"
      >
        {{ expanded ? '收起' : '展开' }}
      </button>
    </div>
    <div v-if="aiSummary" class="text-sm text-gray-700 leading-snug mb-1.5 font-medium border-l-2 pl-2" :class="isError ? 'border-red-400' : 'border-emerald-400'">
      {{ aiSummary }}
    </div>
    <div v-else class="text-sm text-gray-500 overflow-hidden" style="word-break: break-word; display: -webkit-box; -webkit-line-clamp: 2; -webkit-box-orient: vertical;">{{ data.label }}</div>
    <div v-if="expanded" class="text-sm text-gray-600 leading-relaxed whitespace-pre-wrap break-words max-h-[240px] overflow-y-auto bg-white/50 rounded p-3 mt-2">
      {{ resultText || data.label }}
    </div>
    <Handle type="source" :position="Position.Bottom" :class="isError ? '!bg-red-500' : '!bg-emerald-500'" class="!w-2 !h-2" />
  </div>
</template>
