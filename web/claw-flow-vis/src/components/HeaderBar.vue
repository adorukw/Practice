<script setup lang="ts">
import { computed } from 'vue'
import dayjs from 'dayjs'
import { useSessionStore } from '../store/session'

const store = useSessionStore()
const meta = computed(() => store.sessionMeta)

const duration = computed(() => {
  if (!meta.value) return ''
  const ms = meta.value.endTime - meta.value.startTime
  if (ms < 1000) return `${ms}ms`
  if (ms < 60000) return `${(ms / 1000).toFixed(1)}s`
  return `${(ms / 60000).toFixed(1)}min`
})

const timeRange = computed(() => {
  if (!meta.value) return ''
  return `${dayjs(meta.value.startTime).format('HH:mm:ss')} → ${dayjs(meta.value.endTime).format('HH:mm:ss')}`
})
</script>

<template>
  <header v-if="meta" class="h-14 bg-white border-b border-gray-200 flex items-center px-5 gap-5 shrink-0">
    <div class="flex items-center gap-2.5">
      <span class="text-2xl">🐾</span>
      <span class="font-bold text-gray-800 text-base">OpenClaw Flow</span>
    </div>
    <div class="h-6 w-px bg-gray-200" />
    <div class="flex items-center gap-4 text-sm text-gray-500">
      <span class="flex items-center gap-1.5" title="Session ID">
        <span class="text-base">🆔</span>
        <span class="font-mono text-gray-700 text-sm">{{ meta.sessionId?.slice(0, 8) }}...</span>
      </span>
      <span class="flex items-center gap-1.5" title="Model">
        <span class="text-base">🧠</span>
        <span class="text-gray-700 text-sm">{{ meta.model }}</span>
      </span>
      <span class="flex items-center gap-1.5" title="Provider">
        <span class="text-base">🏢</span>
        <span class="text-gray-700 text-sm">{{ meta.provider }}</span>
      </span>
      <span class="flex items-center gap-1.5" title="Total Cost">
        <span class="text-base">💰</span>
        <span class="text-gray-700 text-sm">${{ Number(meta.totalCost).toFixed(4) }}</span>
      </span>
      <span class="flex items-center gap-1.5" title="Total Tokens">
        <span class="text-base">🔤</span>
        <span class="text-gray-700 text-sm">{{ meta.totalTokens.toLocaleString() }}</span>
      </span>
      <span class="flex items-center gap-1.5" title="Duration">
        <span class="text-base">⏱️</span>
        <span class="text-gray-700 text-sm">{{ duration }}</span>
      </span>
      <span class="flex items-center gap-1.5" title="Time Range">
        <span class="text-base">📅</span>
        <span class="text-gray-700 text-sm">{{ timeRange }}</span>
      </span>
    </div>
    <div class="ml-auto flex items-center gap-2.5 text-sm">
      <span class="bg-blue-50 text-blue-600 px-3 py-1 rounded-full font-medium">{{ meta.messageCount }} msgs</span>
      <span class="bg-orange-50 text-orange-600 px-3 py-1 rounded-full font-medium">{{ meta.toolCallCount }} tools</span>
      <span v-if="meta.errorCount > 0" class="bg-red-50 text-red-600 px-3 py-1 rounded-full font-medium">{{ meta.errorCount }} errors</span>
    </div>
  </header>
</template>
