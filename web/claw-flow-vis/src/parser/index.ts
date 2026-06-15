import type { SessionNode, SessionEdge, SessionInfo, ParsedSession, OpenClawMessage, ContentBlock } from '../types'

const NODE_HEIGHT_GAP = 120
const CHILD_OFFSET_X = 360

function truncateLabel(text: string, maxLen = 50): string {
  if (!text) return ''
  const clean = text.replace(/\n/g, ' ').trim()
  return clean.length > maxLen ? clean.slice(0, maxLen) + '...' : clean
}

function getNodeId(msg: OpenClawMessage, blockIndex?: string | number): string {
  const base = msg.__openclaw?.id || `msg-${Date.now()}-${Math.random()}`
  return blockIndex !== undefined ? `${base}-block-${blockIndex}` : base
}

function getSeq(msg: OpenClawMessage): number {
  return msg.__openclaw?.seq ?? 0
}

function extractTextFromContent(content?: ContentBlock[]): string {
  if (!content || !Array.isArray(content)) return ''
  return content
    .filter((b) => b.type === 'text' && 'text' in b)
    .map((b) => (b as { type: 'text'; text: string }).text)
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

  if (content.startsWith('{') && content.includes('"status"')) {
    const statusMatch = content.match(/"status"\s*:\s*"([^"]+)"/)
    if (statusMatch) return `状态: ${statusMatch[1]}`
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
    const firstLine = content.split('\n')[0]
    if (firstLine.startsWith('---')) {
      const nameMatch = content.match(/^---\nname:\s*(\S+)/m)
      if (nameMatch) return `读取: ${nameMatch[1]}`
    }
    return `读取文件 (${content.length} 字符)`
  }

  if (toolName === 'write') {
    return `写入完成 (${content.length} 字符)`
  }

  if (toolName === 'update_plan') {
    return '更新执行计划'
  }

  return truncateLabel(content, 40)
}

export function parseSession(session: SessionInfo): ParsedSession {
  const nodes: SessionNode[] = []
  const edges: SessionEdge[] = []

  let yCounter = 0
  let totalCost = 0
  let totalTokens = 0
  let toolCallCount = 0
  let errorCount = 0
  let startTime = Infinity
  let endTime = 0
  let lastModel = ''
  let lastProvider = ''

  const toolCallIdToNodeId = new Map<string, string>()
  const seqToNodeId = new Map<number, string>()

  const messages = session.messages || []

  const lastAssistantSeqWithToolUse = new Map<number, { seq: number; nodeId: string }>()

  for (let i = 0; i < messages.length; i++) {
    const msg = messages[i]
    const seq = getSeq(msg)

    if (msg.timestamp) {
      startTime = Math.min(startTime, msg.timestamp)
      endTime = Math.max(endTime, msg.timestamp)
    }
    if (msg.usage) {
      totalCost += msg.usage.cost?.total ?? 0
      totalTokens += msg.usage.totalTokens ?? 0
    }
    if (msg.model) lastModel = msg.model
    if (msg.provider) lastProvider = msg.provider

    if (msg.role === 'user') {
      const text = extractTextFromContent(msg.content)
      const nodeId = getNodeId(msg)
      const x = 0
      const y = yCounter * NODE_HEIGHT_GAP

      nodes.push({
        id: nodeId,
        type: 'userNode',
        position: { x, y },
        data: {
          type: 'user',
          label: truncateLabel(text),
          raw: msg,
          meta: { tokens: msg.usage?.totalTokens },
          timestamp: msg.timestamp ?? 0,
        },
      })

      seqToNodeId.set(seq, nodeId)
      if (seq > 1) {
        const prevNodeId = seqToNodeId.get(seq - 1)
        if (prevNodeId) {
          edges.push({
            id: `seq-${seq - 1}-to-${seq}`,
            source: prevNodeId,
            target: nodeId,
            type: 'smoothstep',
            style: { stroke: '#9ca3af', strokeWidth: 1.5 },
          })
        }
      }

      yCounter++
    }

    if (msg.role === 'assistant') {
      const assistantNodeId = getNodeId(msg)
      const assistantX = 0
      const assistantY = yCounter * NODE_HEIGHT_GAP

      const textBlocks: ContentBlock[] = msg.content || []
      const thinkingBlocks = textBlocks.filter((b) => b.type === 'thinking')
      const toolCallBlocks = textBlocks.filter((b) => b.type === 'toolCall')
      const textBlocks2 = textBlocks.filter((b) => b.type === 'text')
      const hasToolCall = toolCallBlocks.length > 0
      const hasThinking = thinkingBlocks.length > 0
      const isImplicitToolUse = !hasToolCall && msg.stopReason === 'toolUse'

      const assistantLabel = extractTextFromContent(textBlocks2) || `Assistant (${msg.model || 'unknown'})`

      if (!hasThinking && !hasToolCall && !isImplicitToolUse) {
        nodes.push({
          id: assistantNodeId,
          type: 'assistantNode',
          position: { x: assistantX, y: assistantY },
          data: {
            type: 'assistant',
            label: truncateLabel(assistantLabel),
            raw: msg,
            meta: {
              model: msg.model,
              provider: msg.provider,
              cost: msg.usage?.cost?.total,
              tokens: msg.usage?.totalTokens,
              stopReason: msg.stopReason,
            },
            timestamp: msg.timestamp ?? 0,
          },
        })

        seqToNodeId.set(seq, assistantNodeId)
        if (seq > 1) {
          const prevNodeId = seqToNodeId.get(seq - 1)
          if (prevNodeId) {
            edges.push({
              id: `seq-${seq - 1}-to-${seq}`,
              source: prevNodeId,
              target: assistantNodeId,
              type: 'smoothstep',
              style: { stroke: '#9ca3af', strokeWidth: 1.5 },
            })
          }
        }

        yCounter++
      } else {
        nodes.push({
          id: assistantNodeId,
          type: 'assistantNode',
          position: { x: assistantX, y: assistantY },
          data: {
            type: 'assistant',
            label: truncateLabel(assistantLabel),
            raw: msg,
            meta: {
              model: msg.model,
              provider: msg.provider,
              cost: msg.usage?.cost?.total,
              tokens: msg.usage?.totalTokens,
              stopReason: msg.stopReason,
            },
            timestamp: msg.timestamp ?? 0,
          },
        })

        seqToNodeId.set(seq, assistantNodeId)
        if (seq > 1) {
          const prevNodeId = seqToNodeId.get(seq - 1)
          if (prevNodeId) {
            edges.push({
              id: `seq-${seq - 1}-to-${seq}`,
              source: prevNodeId,
              target: assistantNodeId,
              type: 'smoothstep',
              style: { stroke: '#9ca3af', strokeWidth: 1.5 },
            })
          }
        }

        yCounter++

        thinkingBlocks.forEach((block, blockIndex) => {
          const thinking = 'thinking' in block ? block.thinking : ''
          const blockNodeId = getNodeId(msg, `thinking-${blockIndex}`)
          const bx = CHILD_OFFSET_X
          const by = yCounter * NODE_HEIGHT_GAP

          nodes.push({
            id: blockNodeId,
            type: 'thinkingNode',
            position: { x: bx, y: by },
            data: {
              type: 'thinking',
              label: truncateLabel(thinking),
              raw: { thinking, parentMsg: msg },
              parentId: assistantNodeId,
              meta: {},
              timestamp: msg.timestamp ?? 0,
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

        toolCallBlocks.forEach((block, blockIndex) => {
          toolCallCount++
          const toolName = block.name
          const toolCallId = block.id
          const blockNodeId = getNodeId(msg, `toolcall-${blockIndex}`)
          toolCallIdToNodeId.set(toolCallId, blockNodeId)

          const bx = CHILD_OFFSET_X
          const by = yCounter * NODE_HEIGHT_GAP

          const callArgs = block.arguments || {}

          nodes.push({
            id: blockNodeId,
            type: 'toolCallNode',
            position: { x: bx, y: by },
            data: {
              type: 'toolCall',
              label: generateToolCallLabel(toolName, callArgs),
              raw: { ...block, parentMsg: msg },
              parentId: assistantNodeId,
              meta: {
                toolName,
                toolCallId,
                model: msg.model,
                cost: msg.usage?.cost?.total,
                tokens: msg.usage?.totalTokens,
              },
              timestamp: msg.timestamp ?? 0,
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

        if (isImplicitToolUse) {
          lastAssistantSeqWithToolUse.set(seq, { seq, nodeId: assistantNodeId })
        }
      }
    }

    if (msg.role === 'toolResult') {
      const toolCallId = msg.toolCallId || ''
      const toolName = msg.toolName || 'unknown'
      const isError = msg.isError ?? false
      if (isError) errorCount++

      const resultText = extractTextFromContent(msg.content)
      const sourceNodeId = toolCallIdToNodeId.get(toolCallId)
      const nodeId = getNodeId(msg)

      if (sourceNodeId) {
        const x = CHILD_OFFSET_X * 2
        const y = yCounter * NODE_HEIGHT_GAP

        nodes.push({
          id: nodeId,
          type: 'toolResultNode',
          position: { x, y },
          data: {
            type: 'toolResult',
            label: generateToolResultLabel(toolName, resultText, isError),
            raw: msg,
            meta: { toolName, toolCallId, error: isError },
            timestamp: msg.timestamp ?? 0,
          },
        })

        edges.push({
          id: `toolcall-${sourceNodeId}-to-result-${nodeId}`,
          source: sourceNodeId,
          target: nodeId,
          type: 'smoothstep',
          style: isError
            ? { stroke: '#ef4444', strokeWidth: 2, strokeDasharray: '5 5' }
            : { stroke: '#10b981', strokeWidth: 2 },
          animated: !isError,
        })

        seqToNodeId.set(seq, nodeId)
        yCounter++
      } else {
        toolCallCount++
        const virtualToolCallNodeId = `virtual-tc-${toolCallId}`

        let parentAssistantNodeId: string | undefined
        for (const [asstSeq, info] of [...lastAssistantSeqWithToolUse].reverse()) {
          if (asstSeq < seq) {
            parentAssistantNodeId = info.nodeId
            break
          }
        }

        if (!parentAssistantNodeId) {
          for (let j = i - 1; j >= 0; j--) {
            const prevMsg = messages[j]
            if (prevMsg.role === 'assistant') {
              parentAssistantNodeId = getNodeId(prevMsg)
              break
            }
          }
        }

        const tcX = CHILD_OFFSET_X
        const tcY = yCounter * NODE_HEIGHT_GAP

        nodes.push({
          id: virtualToolCallNodeId,
          type: 'toolCallNode',
          position: { x: tcX, y: tcY },
          data: {
            type: 'toolCall',
            label: `${toolName}()`,
            raw: { id: toolCallId, name: toolName, arguments: {}, implicit: true },
            parentId: parentAssistantNodeId,
            meta: { toolName, toolCallId, implicit: true },
            timestamp: msg.timestamp ?? 0,
          },
        })

        toolCallIdToNodeId.set(toolCallId, virtualToolCallNodeId)

        if (parentAssistantNodeId) {
          edges.push({
            id: `assistant-${parentAssistantNodeId}-to-virtual-tc-${virtualToolCallNodeId}`,
            source: parentAssistantNodeId,
            target: virtualToolCallNodeId,
            type: 'smoothstep',
            style: { stroke: '#f59e0b', strokeWidth: 2 },
            animated: true,
          })
        }

        yCounter++

        const resultX = CHILD_OFFSET_X * 2
        const resultY = yCounter * NODE_HEIGHT_GAP

        nodes.push({
          id: nodeId,
          type: 'toolResultNode',
          position: { x: resultX, y: resultY },
          data: {
            type: 'toolResult',
            label: generateToolResultLabel(toolName, resultText, isError),
            raw: msg,
            meta: { toolName, toolCallId, error: isError },
            timestamp: msg.timestamp ?? 0,
          },
        })

        edges.push({
          id: `virtual-tc-${virtualToolCallNodeId}-to-result-${nodeId}`,
          source: virtualToolCallNodeId,
          target: nodeId,
          type: 'smoothstep',
          style: isError
            ? { stroke: '#ef4444', strokeWidth: 2, strokeDasharray: '5 5' }
            : { stroke: '#10b981', strokeWidth: 2 },
          animated: !isError,
        })

        seqToNodeId.set(seq, nodeId)
        yCounter++
      }

      if (i + 1 < messages.length) {
        const nextMsg = messages[i + 1]
        if (nextMsg.role === 'assistant') {
          const nextSeq = getSeq(nextMsg)
          if (!seqToNodeId.has(nextSeq)) {
            const nextNodeId = getNodeId(nextMsg)
            edges.push({
              id: `result-${nodeId}-to-next-assistant-${nextNodeId}`,
              source: nodeId,
              target: nextNodeId,
              type: 'smoothstep',
              style: { stroke: '#8b5cf6', strokeWidth: 1.5 },
            })
          }
        }
      }
    }
  }

  return {
    nodes,
    edges,
    sessionInfo: {
      sessionKey: session.sessionKey,
      sessionId: session.sessionId,
      totalCost,
      totalTokens,
      model: lastModel,
      provider: lastProvider,
      messageCount: messages.length,
      toolCallCount,
      errorCount,
      startTime: startTime === Infinity ? 0 : startTime,
      endTime,
    },
  }
}
