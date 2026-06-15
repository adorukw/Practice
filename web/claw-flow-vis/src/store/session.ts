import { defineStore } from 'pinia'
import { ref, computed } from 'vue'
import type { SessionEdge, SessionInfo, ParsedSession, SessionNodeType } from '../types'
import { parseSession } from '../parser'
import { summarizeContent, type SummarizeRequest } from '../services/summarizer'
import config from '../config.json'

const STORAGE_KEY_PREFIX = 'openclaw-summary-'

function getStorageKey(sessionId: string): string {
  return `${STORAGE_KEY_PREFIX}${sessionId}`
}

function loadCachedSummaries(sessionId: string): Record<string, string> {
  try {
    const raw = localStorage.getItem(getStorageKey(sessionId))
    if (raw) return JSON.parse(raw)
  } catch {}
  return {}
}

function saveCachedSummaries(sessionId: string, data: Record<string, string>) {
  try {
    localStorage.setItem(getStorageKey(sessionId), JSON.stringify(data))
  } catch {}
}

export const useSessionStore = defineStore('session', () => {
  const nodes = ref<any[]>([])
  const edges = ref<SessionEdge[]>([])
  const selectedNodeId = ref<string | null>(null)
  const sessionMeta = ref<ParsedSession['sessionInfo'] | null>(null)
  const filterType = ref<SessionNodeType | 'all'>('all')
  const searchQuery = ref('')
  const rawSession = ref<SessionInfo | null>(null)
  const summaries = ref<Record<string, string>>({})
  const isSummarizing = ref(false)
  const summarizeProgress = ref({ done: 0, total: 0 })

  const selectedNode = computed(() => {
    if (!selectedNodeId.value) return null
    return nodes.value.find((n: any) => n.id === selectedNodeId.value) ?? null
  })

  const filteredNodes = computed(() => {
    let result: any[] = nodes.value
    if (filterType.value !== 'all') {
      result = result.filter((n: any) => n.data?.type === filterType.value)
    }
    if (searchQuery.value) {
      const q = searchQuery.value.toLowerCase()
      result = result.filter(
        (n: any) =>
          n.data?.label?.toLowerCase().includes(q) ||
          n.data?.meta?.toolName?.toLowerCase().includes(q)
      )
    }
    return result
  })

  const filteredNodeIds = computed(() => new Set(filteredNodes.value.map((n: any) => n.id)))

  const toolCallNodes = computed(() => nodes.value.filter((n: any) => n.data?.type === 'toolCall'))
  const errorNodes = computed(() => nodes.value.filter((n: any) => n.data?.meta?.error))
  const userNodes = computed(() => nodes.value.filter((n: any) => n.data?.type === 'user'))

  function loadSession(session: SessionInfo) {
    rawSession.value = session
    const parsed = parseSession(session)
    nodes.value = parsed.nodes
    edges.value = parsed.edges
    sessionMeta.value = parsed.sessionInfo
    selectedNodeId.value = null

    const cached = loadCachedSummaries(parsed.sessionInfo.sessionId)
    summaries.value = cached

    if (config.deepseekApiKey && config.autoSummarize) {
      runSummarize()
    }
  }

  function selectNode(nodeId: string | null) {
    selectedNodeId.value = nodeId
  }

  function setFilterType(type: SessionNodeType | 'all') {
    filterType.value = type
  }

  function setSearchQuery(query: string) {
    searchQuery.value = query
  }

  function clearSelection() {
    selectedNodeId.value = null
  }

  function getSummary(nodeId: string): string {
    return summaries.value[nodeId] || ''
  }

  function persistSummaries() {
    if (sessionMeta.value) {
      saveCachedSummaries(sessionMeta.value.sessionId, summaries.value)
    }
  }

  async function runSummarize() {
    const requests: { nodeId: string; req: SummarizeRequest }[] = []

    for (const node of nodes.value) {
      const data = node.data
      if (!data) continue

      if (summaries.value[node.id]) continue

      if (data.type === 'thinking') {
        const thinking = data.raw?.thinking || data.label
        requests.push({
          nodeId: node.id,
          req: {
            toolName: 'thinking',
            content: thinking,
            type: 'thinking',
          },
        })
      } else if (data.type === 'toolCall') {
        const args = data.raw?.arguments
        const argsStr = args ? JSON.stringify(args) : ''
        requests.push({
          nodeId: node.id,
          req: {
            toolName: data.meta?.toolName || 'unknown',
            content: argsStr || data.label,
            type: 'toolCall',
          },
        })
      } else if (data.type === 'toolResult') {
        const content = data.raw?.content
        let text = ''
        if (Array.isArray(content)) {
          text = content.map((b: any) => b.text || '').join('\n')
        } else {
          text = data.label
        }
        requests.push({
          nodeId: node.id,
          req: {
            toolName: data.meta?.toolName || 'unknown',
            content: text,
            type: 'toolResult',
            isError: data.meta?.error,
          },
        })
      }
    }

    if (requests.length === 0) return

    isSummarizing.value = true
    summarizeProgress.value = { done: 0, total: requests.length }

    try {
      const batchSize = 5
      for (let i = 0; i < requests.length; i += batchSize) {
        const batch = requests.slice(i, i + batchSize)
        const results = await Promise.all(
          batch.map(async (r) => {
            const summary = await summarizeContent(r.req)
            return { nodeId: r.nodeId, summary }
          })
        )
        for (const r of results) {
          if (r.summary) {
            summaries.value[r.nodeId] = r.summary
          }
        }
        persistSummaries()
        summarizeProgress.value = { done: Math.min(i + batchSize, requests.length), total: requests.length }
        if (i + batchSize < requests.length) {
          await new Promise((r) => setTimeout(r, 300))
        }
      }
    } finally {
      isSummarizing.value = false
      persistSummaries()
    }
  }

  return {
    nodes,
    edges,
    selectedNodeId,
    selectedNode,
    sessionMeta,
    rawSession,
    filterType,
    searchQuery,
    filteredNodes,
    filteredNodeIds,
    toolCallNodes,
    errorNodes,
    userNodes,
    summaries,
    isSummarizing,
    summarizeProgress,
    loadSession,
    selectNode,
    setFilterType,
    setSearchQuery,
    clearSelection,
    getSummary,
    runSummarize,
  }
})
