import type { Node, Edge } from '@vue-flow/core'

export type SessionNodeType = 'user' | 'assistant' | 'thinking' | 'toolCall' | 'toolResult' | 'system'

export interface SessionNodeMeta {
  toolName?: string
  toolCallId?: string
  duration?: number
  error?: boolean
  cost?: number
  tokens?: number
  model?: string
  provider?: string
  api?: string
  stopReason?: string
  implicit?: boolean
}

export interface SessionNodeData {
  type: SessionNodeType
  label: string
  raw: any
  parentId?: string
  meta?: SessionNodeMeta
  timestamp: number
}

export type SessionNode = Node<SessionNodeData>
export type SessionEdge = Edge

export interface OpenClawMessage {
  role: 'user' | 'assistant' | 'toolResult' | 'system'
  content: ContentBlock[]
  timestamp?: number
  usage?: {
    input: number
    output: number
    totalTokens: number
    cacheRead: number
    cacheWrite: number
    cost: { total: number }
  }
  stopReason?: string
  api?: string
  provider?: string
  model?: string
  responseId?: string
  toolCallId?: string
  toolName?: string
  isError?: boolean
  __openclaw?: {
    id: string
    seq: number
  }
}

export type ContentBlock =
  | { type: 'text'; text: string }
  | { type: 'thinking'; thinking: string }
  | { type: 'toolCall'; id: string; name: string; arguments: Record<string, any>; partialArgs?: string }

export interface SessionInfo {
  sessionKey: string
  sessionId: string
  messages: OpenClawMessage[]
  thinkingLevel?: string
}

export interface ParsedSession {
  nodes: SessionNode[]
  edges: SessionEdge[]
  sessionInfo: {
    sessionKey: string
    sessionId: string
    totalCost: number
    totalTokens: number
    model: string
    provider: string
    messageCount: number
    toolCallCount: number
    errorCount: number
    startTime: number
    endTime: number
  }
}
