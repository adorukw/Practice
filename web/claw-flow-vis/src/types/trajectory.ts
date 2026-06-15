import type { Node, Edge } from '@vue-flow/core'

export type TrajectoryEventType =
  | 'user.message'
  | 'assistant.message'
  | 'tool.call'
  | 'tool.result'
  | 'session.started'
  | 'session.thinking_level_change'
  | 'session.custom'
  | 'context.compiled'
  | 'prompt.submitted'
  | 'trace.metadata'

export type TrajectoryNodeType =
  | 'user'
  | 'assistant'
  | 'thinking'
  | 'toolCall'
  | 'toolResult'
  | 'system'

export interface TrajectoryEvent {
  traceSchema: string
  schemaVersion: number
  traceId: string
  source: 'transcript' | 'runtime'
  type: TrajectoryEventType | string
  ts: string
  seq: number
  sourceSeq: number
  sessionId: string
  sessionKey: string
  workspaceDir: string
  entryId: string
  parentEntryId: string | null
  runId?: string
  provider?: string
  modelId?: string
  modelApi?: string
  data: TrajectoryEventData
}

export interface TrajectoryEventData {
  message?: {
    role: string
    content: any[]
    timestamp?: number
    stopReason?: string
    usage?: {
      input: number
      output: number
      cacheRead: number
      cacheWrite: number
      totalTokens: number
      cost: {
        input: number
        output: number
        cacheRead: number
        cacheWrite: number
        total: number
      }
    }
    api?: string
    provider?: string
    model?: string
    responseId?: string
    toolCallId?: string
    toolName?: string
    isError?: boolean
    details?: {
      status?: string
      exitCode?: number
      durationMs?: number
      aggregated?: string
      cwd?: string
    }
  }
  toolCallId?: string
  name?: string
  arguments?: Record<string, any>
  assistantEntryId?: string
  blockIndex?: number
  thinkingLevel?: string
  customType?: string
  prompt?: string
  systemPrompt?: string
  [key: string]: any
}

export interface TrajectoryNodeMeta {
  toolName?: string
  toolCallId?: string
  error?: boolean
  cost?: number
  tokens?: number
  model?: string
  provider?: string
  api?: string
  stopReason?: string
  entryId?: string
  parentEntryId?: string
  blockIndex?: number
  implicit?: boolean
  durationMs?: number
  exitCode?: number
}

export interface TrajectoryNodeData {
  type: TrajectoryNodeType
  label: string
  raw: TrajectoryEvent
  parentId?: string
  meta?: TrajectoryNodeMeta
  timestamp: number
}

export type TrajectoryNode = Node<TrajectoryNodeData>
export type TrajectoryEdge = Edge

export interface ParsedTrajectory {
  nodes: TrajectoryNode[]
  edges: TrajectoryEdge[]
  trajectoryInfo: {
    sessionId: string
    sessionKey: string
    traceId: string
    model: string
    provider: string
    eventCount: number
    toolCallCount: number
    errorCount: number
    startTime: number
    endTime: number
    totalCost: number
    totalTokens: number
  }
}
