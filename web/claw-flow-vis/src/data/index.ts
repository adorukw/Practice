import sessionInfo581d from '../data/session_info/581d73ba-d06d-4ab1-a997-e98f56739cf3.json'
import sessionInfo6e02 from '../data/session_info/6e022e88-f346-4f39-b30f-58616db0c506.json'
import sessionInfoC129 from '../data/session_info/c12953f2-3cef-469a-9fe3-a7f67bb818ac.json'
import sessionInfoC970 from '../data/session_info/c97059f6-ed91-4bd0-9927-2e4fbb9f725a.json'
import sessionInfoCae6 from '../data/session_info/cae67eba-54b7-47b4-b925-853c72ea8cda.json'
import sessionInfo61ce from '../data/session_info/61ce2bc5-c79b-407e-adee-3ea8022127aa.json'

export interface SessionEntry {
  sessionId: string
  sessionData: any
  description: string
}

const sessions: SessionEntry[] = [
  {
    sessionId: '61ce2bc5-c79b-407e-adee-3ea8022127aa',
    sessionData: sessionInfo61ce,
    description: '未明确提示词，让openclaw分析三篇论文',
  },
  {
    sessionId: 'cae67eba-54b7-47b4-b925-853c72ea8cda',
    sessionData: sessionInfoCae6,
    description: '明确提示词，让openclaw分析三篇论文',
  },
  {
    sessionId: '581d73ba-d06d-4ab1-a997-e98f56739cf3',
    sessionData: sessionInfo581d,
    description: '写一篇《2025–2026 LLM Agent 架构演化》综述',

  },
  {
    sessionId: '6e022e88-f346-4f39-b30f-58616db0c506',
    sessionData: sessionInfo6e02,
    description: '爬取B站热搜第一条记录',

  },
  {
    sessionId: 'c12953f2-3cef-469a-9fe3-a7f67bb818ac',
    sessionData: sessionInfoC129,
    description: '2026年AI Agent框架的竞品分析报告',

  },
  {
    sessionId: 'c97059f6-ed91-4bd0-9927-2e4fbb9f725a',
    sessionData: sessionInfoC970,
    description: '爬取音频视频并分析',

  }
]

export function getSessionIds(): string[] {
  return sessions.map((s) => s.sessionId)
}

export function getSessions(): SessionEntry[] {
  return sessions
}

export function getSessionById(sessionId: string): SessionEntry | undefined {
  return sessions.find((s) => s.sessionId === sessionId)
}

export function getDefaultSessionId(): string {
  return sessions[0]?.sessionId || ''
}
