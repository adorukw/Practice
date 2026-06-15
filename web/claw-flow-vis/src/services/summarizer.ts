import config from '../config.json'

const API_URL = config.deepseekApiUrl || 'https://api.deepseek.com/v1/chat/completions'
const API_KEY = config.deepseekApiKey || ''
const MODEL = config.deepseekModel || 'deepseek-chat'

export interface SummarizeRequest {
  toolName: string
  content: string
  type: 'toolCall' | 'toolResult' | 'thinking'
  isError?: boolean
}

const THINKING_PROMPT = `你是一个专业的工作流分析引擎。请分析以下AI助手的思考过程(thinking)，生成结构化的分析摘要。

输出格式要求（严格遵循，不要加编号和前缀）：
[推理] 一句话概括推理方向
[判断] 做出的关键判断
[计划] 下一步计划或决策

示例：
[推理] 当前搜索方案超时，需切换策略
[判断] web_search不可用，改用web_fetch直接抓取
[计划] 先用curl获取页面，再解析数据

注意：
- 保持简洁，每行不超过20字
- 直接输出三行，不要任何额外解释
- 用中文输出
- 提取思考中的核心逻辑链`

const TOOL_CALL_PROMPT = `你是一个专业的工作流分析引擎。请分析以下工具调用(toolCall)的信息，生成结构化的分析摘要。

输出格式要求（严格遵循，不要加编号和前缀）：
[意图] 一句话描述调用目的
[操作] 具体执行的操作
[目标] 操作的对象或目标

示例：
[意图] 搜索竞品信息
[操作] 执行web_search查询
[目标] "AI Agent框架对比分析"

示例2：
[意图] 读取技能配置
[操作] 读取本地文件
[目标] ~/.openclaw/workspace/skills/brave-search/SKILL.md

注意：
- 保持简洁，每行不超过20字
- 直接输出三行，不要任何额外解释
- 用中文输出
- 如果是命令执行，提取关键命令和参数
- 如果是文件读取，提取文件路径
- 如果是搜索，提取搜索关键词`

const TOOL_RESULT_PROMPT = `你是一个专业的工作流分析引擎。请分析以下工具执行结果(toolResult)的信息，生成结构化的分析摘要。

输出格式要求（严格遵循，不要加编号和前缀）：
[状态] 成功/失败/部分成功
[结果] 一句话概括返回内容
[要点] 关键信息或数据摘要

示例：
[状态] 成功
[结果] 获取到5个AI框架的GitHub数据
[要点] AutoGPT: 160k stars, LangChain: 85k stars

示例2：
[状态] 失败
[结果] 搜索请求超时
[要点] web_search服务响应超时，需切换方案

注意：
- 保持简洁，每行不超过25字
- 直接输出三行，不要任何额外解释
- 用中文输出
- 对于错误结果，说明错误原因和建议
- 对于成功结果，提取关键数据点
- 对于空结果，说明无输出原因`

const summaryCache = new Map<string, string>()

function getCacheKey(req: SummarizeRequest): string {
  const contentPreview = req.content.slice(0, 200)
  return `${req.type}:${req.toolName}:${contentPreview}`
}

function getSystemPrompt(type: SummarizeRequest['type']): string {
  if (type === 'thinking') return THINKING_PROMPT
  if (type === 'toolCall') return TOOL_CALL_PROMPT
  return TOOL_RESULT_PROMPT
}

export async function summarizeContent(req: SummarizeRequest): Promise<string> {
  const cacheKey = getCacheKey(req)
  const cached = summaryCache.get(cacheKey)
  if (cached) return cached

  if (!API_KEY) return ''

  const systemPrompt = getSystemPrompt(req.type)

  const userMessage = `工具名: ${req.toolName}
类型: ${req.type}
${req.isError ? '状态: 出错' : ''}
内容:
${req.content.slice(0, 2000)}`

  try {
    const response = await fetch(API_URL, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
        'Authorization': `Bearer ${API_KEY}`,
      },
      body: JSON.stringify({
        model: MODEL,
        messages: [
          { role: 'system', content: systemPrompt },
          { role: 'user', content: userMessage },
        ],
        max_tokens: 150,
        temperature: 0.2,
      }),
    })

    if (!response.ok) {
      console.warn('API error:', response.status)
      return ''
    }

    const data = await response.json()
    const summary = data.choices?.[0]?.message?.content?.trim() || ''

    if (summary) {
      summaryCache.set(cacheKey, summary)
    }

    return summary
  } catch (err) {
    console.warn('API request failed:', err)
    return ''
  }
}
