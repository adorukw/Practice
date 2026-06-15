import eventsCae6 from '../data/trajectory_info/cae67eba-54b7-47b4-b925-853c72ea8cda/events.jsonl?raw'
import events2ee5 from '../data/trajectory_info/2ee5add5-cbf8-4963-8996-d145346eb427/events.jsonl?raw'
import metadataCae6 from '../data/trajectory_info/cae67eba-54b7-47b4-b925-853c72ea8cda/metadata.json'
import metadata2ee5 from '../data/trajectory_info/2ee5add5-cbf8-4963-8996-d145346eb427/metadata.json'
import type { TrajectoryEvent } from '../types/trajectory'

export interface TrajectoryEntry {
  sessionId: string
  description: string
  eventsRaw: string
  metadata: any
}

const trajectories: TrajectoryEntry[] = [
  {
    sessionId: 'cae67eba-54b7-47b4-b925-853c72ea8cda',
    description: '明确提示词，让openclaw分析三篇论文',
    eventsRaw: eventsCae6,
    metadata: metadataCae6,
  },
  {
    sessionId: '2ee5add5-cbf8-4963-8996-d145346eb427',
    description: '分析轨迹文件',
    eventsRaw: events2ee5,
    metadata: metadata2ee5,
  },
]

export function getTrajectories(): TrajectoryEntry[] {
  return trajectories
}

export function getTrajectoryById(sessionId: string): TrajectoryEntry | undefined {
  return trajectories.find((t) => t.sessionId === sessionId)
}

export function getDefaultTrajectoryId(): string {
  return trajectories[0]?.sessionId || ''
}

export function parseEventsJsonl(raw: string): TrajectoryEvent[] {
  return raw
    .split('\n')
    .filter((line) => line.trim())
    .map((line) => {
      try {
        return JSON.parse(line) as TrajectoryEvent
      } catch {
        return null
      }
    })
    .filter((e): e is TrajectoryEvent => e !== null)
}
