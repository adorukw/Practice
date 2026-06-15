import type {
  TrajectoryEvent,
  TrajectoryNode,
  TrajectoryEdge,
  TrajectoryNodeData,
  ParsedTrajectory,
} from '../types/trajectory'

const NODE_HEIGHT_GAP = 120
const CHILD_OFFSET_X = 360

function truncateLabel(text: string, maxLen = 50): string {
  if (!text) return ''
  const clean = text.replace(/\n/g, ' ').trim()
  return clean.length > maxLen ? clean.slice(0, maxLen) + '...' : clean
}

function extractTextFromContent(content?: any[]): string {
  if (!content || !Array.isArray(content)) return ''
  return content
    .filter((b) => b.type === 'text' && 'text' in b)
    .map((b) => (b as { type: 'text'; text: string }).text)
    .join('\n')
}

function extractThinkingFromContent(content?: any[]): string {
  if (!content || !Array.isArray(content)) return ''
  const thinkingBlocks = content.filter((b) => b.type === 'thinking' && 'thinking' in b)
  return thinkingBlocks
    .map((b) => (b as { type: 'thinking'; thinking: string }).thinking)
    .join('\n')
}

function generateToolCallLabel(toolName: string, args?: Record<string, any>): string {
  if (!args || Object.keys(args).length === 0) return `${toolName}()`

  const keyHints: Record<string, string[]> = {
    read: ['path'],
    write: ['path', 'content'],
    exec: ['command'],
    web_search: ['query'],
    web_fetch: ['url'],
    search: ['query'],
    process: ['input', 'pdf_path'],
    update_plan: ['plan'],
    sessions_spawn: ['message'],
    sessions_send: ['message'],
  }

  const priorityKeys = keyHints[toolName] || []

  for (const key of priorityKeys) {
    const val = args[key]
    if (val !== undefined && val !== null && val !== '') {
      const str = String(val).replace(/\n/g, ' ').trim()
      if (key === 'command') {
        const cmd = str.split(' ').slice(0, 4).join(' ')
        return `${toolName}: ${cmd}${str.split(' ').length > 4 ? ' ...' : ''}`
      }
      if (key === 'path') {
        const short = str.split('/').pop() || str
        return `${toolName}: ${short}`
      }
      if (key === 'query') {
        return `${toolName}: "${truncateLabel(str, 30)}"`
      }
      if (key === 'url') {
        try {
          const u = new URL(str)
          return `${toolName}: ${u.hostname}${u.pathname.slice(0, 20)}`
        } catch {
          return `${toolName}: ${truncateLabel(str, 30)}`
        }
      }
      if (key === 'content') {
        return `${toolName}: 写入内容 (${str.length} 字符)`
      }
      if (key === 'message') {
        return `${toolName}: ${truncateLabel(str, 30)}`
      }
      return `${toolName}: ${truncateLabel(str, 30)}`
    }
  }

  const firstKey = Object.keys(args)[0]
  const firstVal = args[firstKey]
  if (firstVal !== undefined && firstVal !== null) {
    return `${toolName}: ${truncateLabel(String(firstVal), 30)}`
  }

  return `${toolName}()`
}

function generateToolResultLabel(toolName: string, content: string, isError: boolean): string {
  if (!content || content.trim() === '') {
    return isError ? '执行出错' : '执行完成（无输出）'
  }

  if (isError) {
    const errMatch = content.match(/"error"\s*:\s*"([^"]+)"/)
    if (errMatch) return `错误: ${truncateLabel(errMatch[1], 40)}`
    return `错误: ${truncateLabel(content, 40)}`
  }

  if (content.startsWith('---\n') || content.startsWith('--- ')) {
    const nameMatch = content.match(/^---\nname:\s*(\S+)/m)
    if (nameMatch) return `读取技能: ${nameMatch[1]}`
    return `读取配置文件`
  }

  if (content.startsWith('{')) {
    try {
      const obj = JSON.parse(content)
      if (obj.status === 'error') return `错误: ${truncateLabel(obj.error || 'unknown', 40)}`
      if (obj.title) return truncateLabel(obj.title, 40)
      if (obj.name) return truncateLabel(obj.name, 40)
    } catch { /* not valid json */ }
  }

  if (toolName === 'exec') {
    const lines = content.split('\n').filter((l) => l.trim())
    if (lines.length <= 3) return truncateLabel(content, 40)
    return truncateLabel(lines[0], 40)
  }

  if (toolName === 'web_search' || toolName === 'web_fetch') {
    const titleMatch = content.match(/Title:\s*(.+)/)
    if (titleMatch) return truncateLabel(titleMatch[1], 40)
  }

  if (toolName === 'read') {
    return `读取文件 (${content.length} 字符)`
  }

  if (toolName === 'write') {
    return `写入完成 (${content.length} 字符)`
  }

  return truncateLabel(content, 40)
}

export function parseTrajectoryEvents(events: TrajectoryEvent[]): ParsedTrajectory {
  const nodes: TrajectoryNode[] = []
  const edges: TrajectoryEdge[] = []

  let yCounter = 0
  let totalCost = 0
  let totalTokens = 0
  let toolCallCount = 0
  let errorCount = 0
  let startTime = Infinity
  let endTime = 0
  let lastModel = ''
  let lastProvider = ''
  let sessionId = ''
  let sessionKey = ''
  let traceId = ''

  const toolCallIdToNodeId = new Map<string, string>()
  const entryIdToNodeId = new Map<string, string>()
  const seqToNodeId = new Map<number, string>()
  const lastAssistantEntryId = new Map<string, string>()

  const filteredEvents = events.filter((e) =>
    ['user.message', 'assistant.message', 'tool.call', 'tool.result'].includes(e.type)
  )

  for (let i = 0; i < filteredEvents.length; i++) {
    const event = filteredEvents[i]
    const seq = event.seq
    const entryId = event.entryId
    const parentEntryId = event.parentEntryId
    const ts = new Date(event.ts).getTime()

    sessionId = event.sessionId || sessionId
    sessionKey = event.sessionKey || sessionKey
    traceId = event.traceId || traceId

    if (event.provider) lastProvider = event.provider
    if (event.modelId) lastModel = event.modelId

    startTime = Math.min(startTime, ts)
    endTime = Math.max(endTime, ts)

    if (event.type === 'user.message') {
      const msg = event.data.message
      if (!msg) continue

      const text = extractTextFromContent(msg.content)
      const nodeId = `user-${entryId}`
      const x = 0
      const y = yCounter * NODE_HEIGHT_GAP

      if (msg.usage) {
        totalCost += msg.usage.cost?.total ?? 0
        totalTokens += msg.usage.totalTokens ?? 0
      }

      nodes.push({
        id: nodeId,
        type: 'trajUserNode',
        position: { x, y },
        data: {
          type: 'user',
          label: truncateLabel(text),
          raw: event,
          meta: {
            entryId,
            parentEntryId: parentEntryId || undefined,
          },
          timestamp: ts,
        },
      })

      entryIdToNodeId.set(entryId, nodeId)
      seqToNodeId.set(seq, nodeId)

      if (parentEntryId) {
        const parentNodeId = entryIdToNodeId.get(parentEntryId)
        if (parentNodeId) {
          edges.push({
            id: `parent-${parentEntryId}-to-${entryId}`,
            source: parentNodeId,
            target: nodeId,
            type: 'smoothstep',
            style: { stroke: '#9ca3af', strokeWidth: 1.5 },
          })
        }
      }

      yCounter++
    }

    if (event.type === 'assistant.message') {
      const msg = event.data.message
      if (!msg) continue

      const assistantNodeId = `assistant-${entryId}`
      const assistantX = 0
      const assistantY = yCounter * NODE_HEIGHT_GAP

      if (msg.usage) {
        totalCost += msg.usage.cost?.total ?? 0
        totalTokens += msg.usage.totalTokens ?? 0
      }
      if (msg.model) lastModel = msg.model
      if (msg.provider) lastProvider = msg.provider

      const content = msg.content || []
      const textBlocks = content.filter((b: any) => b.type === 'text')
      const thinkingBlocks = content.filter((b: any) => b.type === 'thinking')
      const toolCallBlocks = content.filter((b: any) => b.type === 'toolCall')

      const assistantLabel = extractTextFromContent(textBlocks) || `Assistant (${msg.model || lastModel})`

      nodes.push({
        id: assistantNodeId,
        type: 'trajAssistantNode',
        position: { x: assistantX, y: assistantY },
        data: {
          type: 'assistant',
          label: truncateLabel(assistantLabel),
          raw: event,
          meta: {
            model: msg.model || lastModel,
            provider: msg.provider || lastProvider,
            cost: msg.usage?.cost?.total,
            tokens: msg.usage?.totalTokens,
            stopReason: msg.stopReason,
            entryId,
            parentEntryId: parentEntryId || undefined,
          },
          timestamp: ts,
        },
      })

      entryIdToNodeId.set(entryId, assistantNodeId)
      seqToNodeId.set(seq, assistantNodeId)
      lastAssistantEntryId.set(entryId, assistantNodeId)

      if (parentEntryId) {
        const parentNodeId = entryIdToNodeId.get(parentEntryId)
        if (parentNodeId) {
          edges.push({
            id: `parent-${parentEntryId}-to-${entryId}`,
            source: parentNodeId,
            target: assistantNodeId,
            type: 'smoothstep',
            style: { stroke: '#9ca3af', strokeWidth: 1.5 },
          })
        }
      }

      yCounter++

      thinkingBlocks.forEach((block: any, blockIndex: number) => {
        const thinking = block.thinking || ''
        const blockNodeId = `thinking-${entryId}-block-${blockIndex}`
        const bx = CHILD_OFFSET_X
        const by = yCounter * NODE_HEIGHT_GAP

        nodes.push({
          id: blockNodeId,
          type: 'trajThinkingNode',
          position: { x: bx, y: by },
          data: {
            type: 'thinking',
            label: truncateLabel(thinking),
            raw: event,
            parentId: assistantNodeId,
            meta: {
              entryId,
              parentEntryId: entryId,
              blockIndex,
            },
            timestamp: ts,
          },
        })

        edges.push({
          id: `assistant-${assistantNodeId}-to-thinking-${blockNodeId}`,
          source: assistantNodeId,
          target: blockNodeId,
          type: 'smoothstep',
          style: { stroke: '#9ca3af', strokeWidth: 1, strokeDasharray: '5 5' },
        })

        yCounter++
      })

      toolCallBlocks.forEach((block: any, blockIndex: number) => {
        toolCallCount++
        const toolName = block.name
        const toolCallId = block.id
        const blockNodeId = `toolcall-${entryId}-block-${blockIndex}`
        toolCallIdToNodeId.set(toolCallId, blockNodeId)

        const bx = CHILD_OFFSET_X
        const by = yCounter * NODE_HEIGHT_GAP

        const callArgs = block.arguments || {}

        nodes.push({
          id: blockNodeId,
          type: 'trajToolCallNode',
          position: { x: bx, y: by },
          data: {
            type: 'toolCall',
            label: generateToolCallLabel(toolName, callArgs),
            raw: event,
            parentId: assistantNodeId,
            meta: {
              toolName,
              toolCallId,
              model: msg.model || lastModel,
              cost: msg.usage?.cost?.total,
              tokens: msg.usage?.totalTokens,
              entryId,
              parentEntryId: entryId,
              blockIndex,
            },
            timestamp: ts,
          },
        })

        edges.push({
          id: `assistant-${assistantNodeId}-to-toolcall-${blockNodeId}`,
          source: assistantNodeId,
          target: blockNodeId,
          type: 'smoothstep',
          style: { stroke: '#f59e0b', strokeWidth: 2 },
          animated: true,
        })

        yCounter++
      })
    }

    if (event.type === 'tool.call') {
      const toolCallId = event.data.toolCallId || ''
      const toolName = event.data.name || 'unknown'
      const args = event.data.arguments || {}
      const assistantEntryId = event.data.assistantEntryId || ''
      const blockIndex = event.data.blockIndex ?? 0

      if (toolCallIdToNodeId.has(toolCallId)) continue

      toolCallCount++
      const tcNodeId = `toolcall-${entryId}`
      toolCallIdToNodeId.set(toolCallId, tcNodeId)

      const tcX = CHILD_OFFSET_X
      const tcY = yCounter * NODE_HEIGHT_GAP

      let parentAssistantNodeId = assistantEntryId
        ? lastAssistantEntryId.get(assistantEntryId)
        : undefined

      if (!parentAssistantNodeId) {
        for (let j = i - 1; j >= 0; j--) {
          const prevEvent = filteredEvents[j]
          if (prevEvent.type === 'assistant.message') {
            parentAssistantNodeId = entryIdToNodeId.get(prevEvent.entryId)
            break
          }
        }
      }

      nodes.push({
        id: tcNodeId,
        type: 'trajToolCallNode',
        position: { x: tcX, y: tcY },
        data: {
          type: 'toolCall',
          label: generateToolCallLabel(toolName, args),
          raw: event,
          parentId: parentAssistantNodeId,
          meta: {
            toolName,
            toolCallId,
            entryId,
            parentEntryId: parentEntryId || undefined,
            blockIndex,
          },
          timestamp: ts,
        },
      })

      entryIdToNodeId.set(entryId, tcNodeId)
      seqToNodeId.set(seq, tcNodeId)

      if (parentAssistantNodeId) {
        edges.push({
          id: `assistant-${parentAssistantNodeId}-to-toolcall-${tcNodeId}`,
          source: parentAssistantNodeId,
          target: tcNodeId,
          type: 'smoothstep',
          style: { stroke: '#f59e0b', strokeWidth: 2 },
          animated: true,
        })
      }

      yCounter++
    }

    if (event.type === 'tool.result') {
      const msg = event.data.message
      if (!msg) continue

      const toolCallId = msg.toolCallId || event.data.toolCallId || ''
      const toolName = msg.toolName || event.data.name || 'unknown'
      const isError = msg.isError ?? false
      if (isError) errorCount++

      const resultText = extractTextFromContent(msg.content)
      const sourceNodeId = toolCallIdToNodeId.get(toolCallId)
      const resultNodeId = `toolresult-${entryId}`

      const resultX = CHILD_OFFSET_X * 2
      const resultY = yCounter * NODE_HEIGHT_GAP

      nodes.push({
        id: resultNodeId,
        type: 'trajToolResultNode',
        position: { x: resultX, y: resultY },
        data: {
          type: 'toolResult',
          label: generateToolResultLabel(toolName, resultText, isError),
          raw: event,
          meta: {
            toolName,
            toolCallId,
            error: isError,
            entryId,
            parentEntryId: parentEntryId || undefined,
            durationMs: msg.details?.durationMs,
            exitCode: msg.details?.exitCode,
          },
          timestamp: ts,
        },
      })

      entryIdToNodeId.set(entryId, resultNodeId)
      seqToNodeId.set(seq, resultNodeId)

      if (sourceNodeId) {
        edges.push({
          id: `toolcall-${sourceNodeId}-to-result-${resultNodeId}`,
          source: sourceNodeId,
          target: resultNodeId,
          type: 'smoothstep',
          style: isError
            ? { stroke: '#ef4444', strokeWidth: 2, strokeDasharray: '5 5' }
            : { stroke: '#10b981', strokeWidth: 2 },
          animated: !isError,
        })
      } else {
        let parentAssistantNodeId: string | undefined
        for (let j = i - 1; j >= 0; j--) {
          const prevEvent = filteredEvents[j]
          if (prevEvent.type === 'assistant.message') {
            parentAssistantNodeId = entryIdToNodeId.get(prevEvent.entryId)
            break
          }
        }

        if (parentAssistantNodeId) {
          edges.push({
            id: `assistant-${parentAssistantNodeId}-to-result-${resultNodeId}`,
            source: parentAssistantNodeId,
            target: resultNodeId,
            type: 'smoothstep',
            style: isError
              ? { stroke: '#ef4444', strokeWidth: 2, strokeDasharray: '5 5' }
              : { stroke: '#10b981', strokeWidth: 2 },
            animated: !isError,
          })
        }
      }

      if (i + 1 < filteredEvents.length) {
        const nextEvent = filteredEvents[i + 1]
        if (nextEvent.type === 'assistant.message') {
          const nextEntryId = nextEvent.entryId
          if (!entryIdToNodeId.has(nextEntryId)) {
            const nextNodeId = `assistant-${nextEntryId}`
            edges.push({
              id: `result-${resultNodeId}-to-next-assistant-${nextNodeId}`,
              source: resultNodeId,
              target: nextNodeId,
              type: 'smoothstep',
              style: { stroke: '#8b5cf6', strokeWidth: 1.5 },
            })
          }
        }
      }

      yCounter++
    }
  }

  return {
    nodes,
    edges,
    trajectoryInfo: {
      sessionId,
      sessionKey,
      traceId,
      model: lastModel,
      provider: lastProvider,
      eventCount: filteredEvents.length,
      toolCallCount,
      errorCount,
      startTime: startTime === Infinity ? 0 : startTime,
      endTime,
      totalCost,
      totalTokens,
    },
  }
}
