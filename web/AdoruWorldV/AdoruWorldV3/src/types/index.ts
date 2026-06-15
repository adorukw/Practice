export interface BlogPost {
  id: string
  slug: string
  title: string
  description: string
  content: string
  coverImage?: string
  created_at: string
  updated_at: string
  published: boolean
  category: string[]
  tags: string[]
  reading_time: number
  word_count: number
  views: number
  featured: boolean
}

export interface Category {
  id: string
  name: string
  slug: string
  description?: string
  count: number
  icon?: string
  color?: string
}

export interface Tag {
  id: string
  name: string
  slug: string
  color?: string
  count: number
}

export interface ArchiveItem {
  year: number
  month: number
  count: number
  posts: BlogPost[]
}

export interface SiteStats {
  totalPosts: number
  totalWords: number
  totalViews: number
  runningDays: number
}

export interface Author {
  name: string
  avatar: string
  bio: string
  location: string
  social: {
    github?: string
    twitter?: string
    email?: string
  }
}

export interface Project {
  id: string
  name: string
  description: string
  tech: string[]
  link?: string
  github?: string
  image?: string
  status: 'completed' | 'in-progress' | 'archived'
}

export interface NavItem {
  name: string
  path: string
  icon?: string
}

export type DexCategory = 'anime' | 'movie' | 'tv' | 'game' | 'book' | 'music'

export type DexStatus = 'completed' | 'watching' | 'playing' | 'reading' | 'listening' | 'dropped' | 'planned'

export interface DexEntry {
  id: string
  slug: string
  title: string
  originalTitle?: string
  cover: string
  category: DexCategory
  status: DexStatus
  rating: number
  progress: number
  total: number
  startDate?: string
  finishDate?: string
  tags: string[]
  comment?: string
  creator?: string
  year?: number
  platform?: string
  genre?: string[]
}

export interface DexCategoryInfo {
  id: DexCategory
  name: string
  icon: string
  color: string
  bgColor: string
}

export interface DexStatusInfo {
  id: DexStatus
  name: string
  icon: string
  color: string
}

export interface DexStats {
  total: number
  byCategory: Record<DexCategory, number>
  byStatus: Record<DexStatus, number>
  averageRating: number
}
