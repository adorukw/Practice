<script setup lang="ts">
import { computed } from 'vue'
import dayjs from 'dayjs'
import { useTrajectoryStore } from '../../store/trajectory'
import type { TrajectoryNodeType } from '../../types/trajectory'

const store = useTrajectoryStore()

const collapsed = defineModel<boolean>('collapsed', { default: false })

const props = defineProps<{
  focusNode?: (nodeId: string) => void
}>()

const filterOptions: { label: string; value: TrajectoryNodeType | 'all' }[] = [
  { label: '全部', value: 'all' },
  { label: '用户', value: 'user' },
  { label: '助手', value: 'assistant' },
  { label: '思考', value: 'thinking' },
  { label: '工具调用', value: 'toolCall' },
  { label: '工具结果', value: 'toolResult' },
]

const typeTagMap: Record<string, { text: string; cls: string }> = {
  user: { text: '用', cls: 'bg-blue-100 text-blue-700' },
  assistant: { text: '助', cls: 'bg-purple-100 text-purple-700' },
  thinking: { text: '思', cls: 'bg-gray-100 text-gray-600' },
  toolCall: { text: '调', cls: 'bg-amber-100 text-amber-700' },
  toolResult: { text: '果', cls: 'bg-emerald-100 text-emerald-700' },
}

const displayNodes = computed(() => {
  let result = [...store.nodes]
    .filter((n) => n.data?.timestamp)
    .sort((a, b) => (a.data?.timestamp ?? 0) - (b.data?.timestamp ?? 0))

  if (store.filterType !== 'all') {
    result = result.filter((n: any) => n.data?.type === store.filterType)
  }

  if (store.searchQuery) {
    const q = store.searchQuery.toLowerCase()
    result = result.filter(
      (n: any) =>
        n.data?.label?.toLowerCase().includes(q) ||
        n.data?.meta?.toolName?.toLowerCase().includes(q)
    )
  }

  return result
})

function handleNodeClick(nodeId: string) {
  store.selectNode(nodeId)
  props.focusNode?.(nodeId)
}
</script>

<template>
  <aside
    class="bg-white border-r border-gray-200 flex flex-col shrink-0 overflow-hidden transition-all duration-200"
    :class="collapsed ? 'w-10' : 'w-72'"
  >
    <div class="flex items-center justify-between px-3 py-2 border-b border-gray-100">
      <span v-if="!collapsed" class="text-xs font-semibold text-gray-400 uppercase tracking-wider">筛选</span>
      <button
        class="w-6 h-6 flex items-center justify-center rounded text-gray-400 hover:text-gray-600 hover:bg-gray-100 transition-colors text-xs"
        @click="collapsed = !collapsed"
      >
        {{ collapsed ? '>' : '<' }}
      </button>
    </div>

    <template v-if="!collapsed">
      <div class="p-4 border-b border-gray-100">
        <div class="flex flex-wrap gap-2">
          <button
            v-for="opt in filterOptions"
            :key="opt.value"
            class="text-sm px-4 py-2 rounded-lg border transition-all duration-200 font-medium"
            :class="store.filterType === opt.value
              ? 'bg-gray-900 text-white border-gray-900 shadow-sm'
              : 'bg-gray-50 text-gray-600 border-gray-200 hover:bg-gray-100 hover:border-gray-300'"
            @click="store.setFilterType(opt.value)"
          >
            {{ opt.label }}
          </button>
        </div>
      </div>

      <div class="p-4 border-b border-gray-100">
        <input
          type="text"
          placeholder="搜索节点..."
          class="w-full text-sm border border-gray-200 rounded-lg px-3 py-2 focus:outline-none focus:ring-2 focus:ring-gray-300 focus:border-transparent"
          :value="store.searchQuery"
          @input="store.setSearchQuery(($event.target as HTMLInputElement).value)"
        />
      </div>

      <div class="flex-1 overflow-y-auto p-4">
        <h3 class="text-xs font-semibold text-gray-400 uppercase tracking-wider mb-3">事件时间线</h3>
        <div class="space-y-1.5">
          <button
            v-for="node in displayNodes"
            :key="node.id"
            class="w-full text-left px-3 py-2.5 rounded-lg text-sm transition-all duration-200 flex items-start gap-2.5"
            :class="{
              'bg-gray-100 ring-1 ring-gray-300': store.selectedNodeId === node.id,
              'hover:bg-gray-50': store.selectedNodeId !== node.id,
            }"
            @click="handleNodeClick(node.id)"
          >
            <span
              class="shrink-0 mt-0.5 w-6 h-6 flex items-center justify-center rounded text-[10px] font-bold"
              :class="typeTagMap[node.data?.type || '']?.cls || 'bg-gray-100 text-gray-500'"
            >
              {{ typeTagMap[node.data?.type || '']?.text || '?' }}
            </span>
            <div class="min-w-0 flex-1">
              <div v-if="store.getSummary(node.id)" class="text-gray-700 text-sm leading-snug truncate">{{ store.getSummary(node.id).split('\n')[0] }}</div>
              <div v-else class="text-gray-700 text-sm leading-snug truncate">{{ node.data?.label }}</div>
              <div class="text-xs text-gray-400 mt-0.5">
                {{ dayjs(node.data?.timestamp).format('HH:mm:ss') }}
                <span v-if="node.data?.meta?.toolName" class="ml-1.5 text-amber-500 font-medium">{{ node.data.meta.toolName }}</span>
              </div>
            </div>
          </button>
        </div>
      </div>
    </template>
  </aside>
</template>
