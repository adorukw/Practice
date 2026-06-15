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

import UserNode from '../components/nodes/UserNode.vue'
import AssistantNode from '../components/nodes/AssistantNode.vue'
import ThinkingNode from '../components/nodes/ThinkingNode.vue'
import ToolCallNode from '../components/nodes/ToolCallNode.vue'
import ToolResultNode from '../components/nodes/ToolResultNode.vue'
import Sidebar from '../components/Sidebar.vue'
import DetailPanel from '../components/DetailPanel.vue'
import { useSessionStore } from '../store/session'
import { getSessionById, getDefaultSessionId } from '../data/index'

const store = useSessionStore()
const route = useRoute()

const { onNodeClick, fitView, setViewport, getNode } = useVueFlow()

const nodeTypes = {
  userNode: markRaw(UserNode),
  assistantNode: markRaw(AssistantNode),
  thinkingNode: markRaw(ThinkingNode),
  toolCallNode: markRaw(ToolCallNode),
  toolResultNode: markRaw(ToolResultNode),
} as any

onNodeClick(({ node }) => {
  store.selectNode(node.id)
})

const flowNodes = computed(() => store.nodes)
const flowEdges = computed(() => store.edges)
const sidebarCollapsed = ref(false)

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

function loadCurrentSession() {
  const sessionId = (route.query.sessionId as string) || getDefaultSessionId()
  const entry = getSessionById(sessionId)
  if (entry) {
    store.loadSession(entry.sessionData)

  }
}

onMounted(() => {
  loadCurrentSession()
})

watch(() => route.query.sessionId, () => {
  loadCurrentSession()
})
</script>

<template>
  <div class="w-full h-full flex overflow-hidden relative">
    <Sidebar v-model:collapsed="sidebarCollapsed" :focus-node="focusNode" />

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

      <DetailPanel />
    </div>
  </div>
</template>
