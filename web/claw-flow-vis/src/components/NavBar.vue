<script setup lang="ts">
import { ref, computed, watch } from 'vue'
import { useRoute, useRouter } from 'vue-router'
import { getSessions, getDefaultSessionId } from '../data/index'
import { getTrajectories, getDefaultTrajectoryId } from '../data/trajectory'

const route = useRoute()
const router = useRouter()
const sessions = getSessions()
const trajectories = getTrajectories()

const currentRoute = computed(() => route.name as string)

const selectedSessionId = ref(getDefaultSessionId())
const selectedTrajectoryId = ref(getDefaultTrajectoryId())

const currentDropdownItems = computed(() => {
  if (currentRoute.value === 'trajectory') {
    return trajectories.map((t) => ({ id: t.sessionId, description: t.description }))
  }
  return sessions.map((s) => ({ id: s.sessionId, description: s.description }))
})

const selectedId = computed({
  get: () => currentRoute.value === 'trajectory' ? selectedTrajectoryId.value : selectedSessionId.value,
  set: (val: string) => {
    if (currentRoute.value === 'trajectory') {
      selectedTrajectoryId.value = val
    } else {
      selectedSessionId.value = val
    }
  },
})

function navigateTo(name: string) {
  const queryId = name === 'trajectory' ? selectedTrajectoryId.value : selectedSessionId.value
  router.push({ name, query: { sessionId: queryId } })
}

function onDropdownChange() {
  const queryId = currentRoute.value === 'trajectory' ? selectedTrajectoryId.value : selectedSessionId.value
  router.push({ name: currentRoute.value, query: { sessionId: queryId } })
}

watch(() => route.query.sessionId, (newId) => {
  if (typeof newId === 'string') {
    if (currentRoute.value === 'trajectory') {
      if (trajectories.some((t) => t.sessionId === newId)) {
        selectedTrajectoryId.value = newId
      }
    } else {
      if (sessions.some((s) => s.sessionId === newId)) {
        selectedSessionId.value = newId
      }
    }
  }
}, { immediate: true })
</script>

<template>
  <nav class="h-16 bg-white border-b border-gray-200 flex items-center px-8 shrink-0 gap-10">
    <div class="flex items-center gap-3 shrink-0">
      <span class="font-bold text-gray-900 text-3xl tracking-tight">Claw Flow Vis</span>
    </div>

    <div class="flex items-center gap-5">
      <button
        class="px-5 py-2.5 text-sm font-semibold rounded-xl transition-all duration-200 transform active:scale-95 select-none"
        :class="currentRoute === 'session'
          ? 'bg-gray-900 text-white shadow-lg shadow-gray-200 active:bg-black'
          : 'text-gray-500 hover:bg-gray-100 hover:text-gray-800 active:bg-gray-200'"
        @click="navigateTo('session')"
      >
        Session 分析
      </button>
      <button
        class="px-5 py-2.5 text-sm font-semibold rounded-xl transition-all duration-200 transform active:scale-95 select-none"
        :class="currentRoute === 'trajectory'
          ? 'bg-gray-900 text-white shadow-lg shadow-gray-200 active:bg-black'
          : 'text-gray-500 hover:bg-gray-100 hover:text-gray-800 active:bg-gray-200'"
        @click="navigateTo('trajectory')"
      >
        Trajectory 分析
      </button>
    </div>

    <div class="relative transition-all duration-200 active:scale-[0.98] shrink-0">
      <select
        v-model="selectedId"
        @change="onDropdownChange"
        class="text-sm border border-gray-200 rounded-xl px-3 py-2 bg-white focus:outline-none focus:ring-2 focus:ring-gray-900 focus:border-transparent max-w-[260px] hover:border-gray-400 hover:bg-gray-50 transition-all cursor-pointer shadow-sm appearance-none pr-10 truncate"
      >
        <option v-for="item in currentDropdownItems" :key="item.id" :value="item.id">
          {{ item.description }}
        </option>
      </select>
      <div class="pointer-events-none absolute inset-y-0 right-0 flex items-center px-3 text-gray-500">
        <svg class="fill-current h-4 w-4" xmlns="http://www.w3.org/2000/svg" viewBox="0 0 20 20">
          <path d="M9.293 12.95l.707.707L15.657 8l-1.414-1.414L10 10.828 5.757 6.586 4.343 8z"/>
        </svg>
      </div>
    </div>
  </nav>
</template>
