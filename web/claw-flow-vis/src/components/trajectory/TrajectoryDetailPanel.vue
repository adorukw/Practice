<script setup lang="ts">
import { computed, ref } from 'vue'
import { useTrajectoryStore } from '../../store/trajectory'
import dayjs from 'dayjs'
import MarkdownIt from 'markdown-it'

const md = new MarkdownIt({ breaks: true, linkify: true })

const store = useTrajectoryStore()
const selectedNode = computed(() => store.selectedNode)
const showRawJson = ref(false)

const nodeData = computed(() => selectedNode.value?.data)

const renderedContent = computed(() => {
  const nd = nodeData.value
  if (!nd) return ''
  const raw = nd.raw
  let text = ''

  if (nd.type === 'thinking') {
    if (raw?.data?.message?.content) {
      const content = raw.data.message.content
      const thinkingBlocks = content.filter((b: any) => b.type === 'thinking')
      text = thinkingBlocks.map((b: any) => b.thinking || '').join('\n')
    }
    if (!text) text = nd.label
  } else if (nd.type === 'toolCall') {
    let args = null
    if (raw?.data?.arguments) {
      args = raw.data.arguments
    } else if (raw?.data?.message?.content) {
      const content = raw.data.message.content
      const toolCallBlocks = content.filter((b: any) => b.type === 'toolCall')
      if (toolCallBlocks.length > 0) {
        args = toolCallBlocks[0].arguments || {}
      }
    }
    text = '```json\n' + JSON.stringify(args || {}, null, 2) + '\n```'
  } else if (nd.type === 'toolResult') {
    const content = raw?.data?.message?.content
    if (Array.isArray(content)) {
      text = content.map((b: any) => b.text || '').join('\n')
    } else {
      text = nd.label
    }
  } else if (nd.type === 'user' || nd.type === 'assistant') {
    const content = raw?.data?.message?.content
    if (Array.isArray(content)) {
      text = content
        .filter((b: any) => b.type === 'text')
        .map((b: any) => b.text || '')
        .join('\n')
    } else {
      text = nd.label
    }
  }

  return md.render(text)
})

const typeLabelMap: Record<string, string> = {
  user: '用户消息',
  assistant: '助手回复',
  thinking: '思考过程',
  toolCall: '工具调用',
  toolResult: '执行结果',
}
</script>

<template>
  <Transition name="slide">
    <div
      v-if="selectedNode && nodeData"
      class="absolute bottom-0 left-0 right-0 bg-white border-t border-gray-200 shadow-lg z-10"
      style="max-height: 45vh"
    >
      <div class="flex items-center justify-between px-5 py-3 border-b border-gray-100 bg-gray-50">
        <div class="flex items-center gap-3">
          <span class="text-sm font-semibold text-gray-700">
            {{ typeLabelMap[nodeData.type] || 'Detail' }}
          </span>
          <span v-if="nodeData.meta?.toolName" class="text-sm bg-amber-100 text-amber-700 px-2.5 py-0.5 rounded font-medium">
            {{ nodeData.meta.toolName }}
          </span>
          <span class="text-sm text-gray-400 font-mono">
            {{ dayjs(nodeData.timestamp).format('YYYY-MM-DD HH:mm:ss') }}
          </span>
        </div>
        <div class="flex items-center gap-2">
          <button
            class="text-sm px-3 py-1.5 rounded-md border transition-colors font-medium"
            :class="showRawJson ? 'bg-gray-800 text-white border-gray-800' : 'bg-gray-100 text-gray-600 hover:bg-gray-200 border-gray-200'"
            @click="showRawJson = !showRawJson"
          >
            {{ showRawJson ? '渲染视图' : '原始JSON' }}
          </button>
          <button
            class="text-gray-400 hover:text-gray-600 text-sm leading-none w-7 h-7 flex items-center justify-center rounded-md hover:bg-gray-100 transition-colors"
            @click="store.clearSelection()"
          >
            x
          </button>
        </div>
      </div>

      <div class="overflow-y-auto p-5" style="max-height: calc(45vh - 52px)">
        <div v-if="!showRawJson" class="prose prose-sm max-w-none" v-html="renderedContent" />
        <pre v-else class="text-sm bg-gray-50 rounded-lg p-4 overflow-x-auto whitespace-pre-wrap break-all text-gray-700 leading-relaxed">{{ JSON.stringify(nodeData.raw, null, 2) }}</pre>

        <div v-if="nodeData.meta" class="mt-4 pt-4 border-t border-gray-100">
          <h4 class="text-sm font-semibold text-gray-500 mb-3">元信息</h4>
          <div class="grid grid-cols-3 gap-3 text-sm">
            <div v-if="nodeData.meta.model" class="bg-gray-50 rounded-lg p-3">
              <div class="text-gray-400 text-xs mb-1">模型</div>
              <div class="text-gray-700 font-mono text-sm">{{ nodeData.meta.model }}</div>
            </div>
            <div v-if="nodeData.meta.provider" class="bg-gray-50 rounded-lg p-3">
              <div class="text-gray-400 text-xs mb-1">提供商</div>
              <div class="text-gray-700 text-sm">{{ nodeData.meta.provider }}</div>
            </div>
            <div v-if="nodeData.meta.cost != null" class="bg-gray-50 rounded-lg p-3">
              <div class="text-gray-400 text-xs mb-1">花费</div>
              <div class="text-gray-700 text-sm">${{ Number(nodeData.meta.cost).toFixed(6) }}</div>
            </div>
            <div v-if="nodeData.meta.tokens" class="bg-gray-50 rounded-lg p-3">
              <div class="text-gray-400 text-xs mb-1">令牌数</div>
              <div class="text-gray-700 text-sm">{{ nodeData.meta.tokens.toLocaleString() }}</div>
            </div>
            <div v-if="nodeData.meta.stopReason" class="bg-gray-50 rounded-lg p-3">
              <div class="text-gray-400 text-xs mb-1">停止原因</div>
              <div class="text-gray-700 text-sm">{{ nodeData.meta.stopReason }}</div>
            </div>
            <div v-if="nodeData.meta.durationMs" class="bg-gray-50 rounded-lg p-3">
              <div class="text-gray-400 text-xs mb-1">耗时</div>
              <div class="text-gray-700 text-sm">{{ nodeData.meta.durationMs }}ms</div>
            </div>
            <div v-if="nodeData.meta.exitCode != null" class="bg-gray-50 rounded-lg p-3">
              <div class="text-gray-400 text-xs mb-1">退出码</div>
              <div class="text-gray-700 text-sm">{{ nodeData.meta.exitCode }}</div>
            </div>
            <div v-if="nodeData.meta.toolCallId" class="bg-gray-50 rounded-lg p-3 col-span-3">
              <div class="text-gray-400 text-xs mb-1">调用ID</div>
              <div class="text-gray-700 font-mono text-xs">{{ nodeData.meta.toolCallId }}</div>
            </div>
            <div v-if="nodeData.meta.entryId" class="bg-gray-50 rounded-lg p-3 col-span-3">
              <div class="text-gray-400 text-xs mb-1">Entry ID</div>
              <div class="text-gray-700 font-mono text-xs">{{ nodeData.meta.entryId }}</div>
            </div>
          </div>
        </div>
      </div>
    </div>
  </Transition>
</template>

<style scoped>
.slide-enter-active,
.slide-leave-active {
  transition: transform 0.3s ease;
}
.slide-enter-from,
.slide-leave-to {
  transform: translateY(100%);
}

.prose :deep(pre) {
  background: #f3f4f6;
  border-radius: 8px;
  padding: 14px;
  font-size: 13px;
  overflow-x: auto;
}

.prose :deep(code) {
  font-size: 13px;
}

.prose :deep(p) {
  font-size: 14px;
  line-height: 1.6;
}
</style>
