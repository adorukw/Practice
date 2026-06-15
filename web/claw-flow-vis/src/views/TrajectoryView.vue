<script setup lang="ts">
import { computed, onMounted, markRaw, ref, watch } from 'vue'
import { useRoute } from 'vue-router'
import { VueFlow, useVueFlow } from '@vue-flow/core'
import { Background } from '@vue-flow/background'
import { Controls } from '@vue-flow/controls'
import { MiniMap } from '@vue-flow/minimap'
import '@vue-flow/core/dist/style.css'
import '@vue-flow/core/dist/theme-default.css'
import '@vue-flow/controls/dist/style.css'
import '@vue-flow/minimap/dist/style.css'

import TrajectoryUserNode from '../components/trajectory/TrajectoryUserNode.vue'
import TrajectoryAssistantNode from '../components/trajectory/TrajectoryAssistantNode.vue'
import TrajectoryThinkingNode from '../components/trajectory/TrajectoryThinkingNode.vue'
import TrajectoryToolCallNode from '../components/trajectory/TrajectoryToolCallNode.vue'
import TrajectoryToolResultNode from '../components/trajectory/TrajectoryToolResultNode.vue'
import TrajectorySidebar from '../components/trajectory/TrajectorySidebar.vue'
import TrajectoryDetailPanel from '../components/trajectory/TrajectoryDetailPanel.vue'
import MetadataPanel from '../components/trajectory/MetadataPanel.vue'
import { useTrajectoryStore } from '../store/trajectory'
import { getTrajectoryById, getDefaultTrajectoryId, parseEventsJsonl } from '../data/trajectory'

const store = useTrajectoryStore()
const route = useRoute()

const { onNodeClick, setViewport, getNode } = useVueFlow()

const nodeTypes = {
  trajUserNode: markRaw(TrajectoryUserNode),
  trajAssistantNode: markRaw(TrajectoryAssistantNode),
  trajThinkingNode: markRaw(TrajectoryThinkingNode),
  trajToolCallNode: markRaw(TrajectoryToolCallNode),
  trajToolResultNode: markRaw(TrajectoryToolResultNode),
} as any

onNodeClick(({ node }) => {
  store.selectNode(node.id)
})

const flowNodes = computed(() => store.nodes)
const flowEdges = computed(() => store.edges)
const sidebarCollapsed = ref(false)
const metadataPanelCollapsed = ref(false)

function focusNode(nodeId: string) {
  const node = getNode.value(nodeId)
  if (node) {
    setViewport({
      x: -node.position.x + 400,
      y: -node.position.y + 200,
      zoom: 1.2,
    })
  }
}

function loadCurrentTrajectory() {
  const sessionId = (route.query.sessionId as string) || getDefaultTrajectoryId()
  const entry = getTrajectoryById(sessionId)
  if (entry) {
    const events = parseEventsJsonl(entry.eventsRaw)
    store.loadTrajectory(events, entry.metadata)
  }
}

onMounted(() => {
  loadCurrentTrajectory()
})

watch(() => route.query.sessionId, () => {
  loadCurrentTrajectory()
})
</script>

<template>
  <div class="w-full h-full flex overflow-hidden relative">
    <TrajectorySidebar v-model:collapsed="sidebarCollapsed" :focus-node="focusNode" />

    <div class="flex-1 relative" style="min-height: 0">
      <VueFlow
        :nodes="flowNodes"
        :edges="flowEdges"
        :node-types="nodeTypes"
        :default-viewport="{ zoom: 1, x: 300, y: 0 }"
        :min-zoom="0.1"
        :max-zoom="2"
        style="width: 100%; height: 100%"
      >
        <Background pattern-color="#e5e7eb" :gap="20" />
        <Controls position="top-right" />
        <MiniMap
          position="bottom-right"
          :pannable="true"
          :zoomable="true"
          :node-color="(n: any) => {
            const type = n.data?.type
            if (type === 'user') return '#3b82f6'
            if (type === 'assistant') return '#8b5cf6'
            if (type === 'thinking') return '#6b7280'
            if (type === 'toolCall') return '#f59e0b'
            if (type === 'toolResult') return '#10b981'
            return '#e5e7eb'
          }"
        />
      </VueFlow>

      <TrajectoryDetailPanel />
    </div>

    <MetadataPanel v-model:collapsed="metadataPanelCollapsed" />
  </div>
</template>
