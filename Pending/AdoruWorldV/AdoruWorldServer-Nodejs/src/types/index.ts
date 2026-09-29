export interface Post {
  id: string;
  slug: string;
  title: string;
  description: string | null;
  content: string;
  coverImage: string | null;
  createdAt: string;
  updatedAt: string;
  published: boolean;
  wordCount: number;
  views: number;
  featured: boolean;
}

export interface PostCategory {
  id: string;
  name: string;
  slug: string;
  description: string | null;
  count: number;
  icon: string | null;
  color: string | null;
}

export interface PostTag {
  id: string;
  name: string;
  slug: string;
  color: string | null;
  count: number;
}

export interface ArchiveItem {
  id: string;
  year: number;
  month: number;
}

export interface SiteStats {
  id: string;
  totalPosts: number;
  totalWords: number;
  totalViews: number;
  runningDays: number;
  updatedAt: string;
}

export interface Author {
  id: string;
  name: string;
  avatar: string;
  bio: string;
  location: string;
  social: Record<string, string> | null;
}

export type ProjectStatus = 'COMPLETED' | 'IN_PROGRESS' | 'ARCHIVED';

export interface Project {
  id: string;
  name: string;
  description: string | null;
  link: string | null;
  github: string | null;
  image: string | null;
  status: ProjectStatus;
}

export interface ProjectTech {
  id: string;
  name: string;
  projectId: string;
}

export type DexCategory = 'ANIME' | 'MOVIE' | 'TV' | 'GAME' | 'BOOK' | 'MUSIC' | 'OTHER';
export type DexStatus = 'COMPLETED' | 'WATCHING' | 'PLAYING' | 'READING' | 'LISTENING' | 'DROPPED' | 'PLANNED';

export interface DexEntry {
  id: string;
  slug: string;
  title: string;
  originalTitle: string | null;
  coverImage: string;
  category: DexCategory;
  status: DexStatus;
  rating: number;
  startDate: string | null;
  finishDate: string | null;
  comments: string | null;
  creator: string | null;
  year: number | null;
}

export interface DexEntryGenre {
  id: string;
  name: string;
  dexEntryId: string;
}

export interface DexStats {
  id: string;
  total: number;
  byCategory: Record<string, number>;
  byStatus: Record<string, number>;
  averageRating: number;
  updatedAt: string;
}

export interface PostCategoryRelation {
  postId: string;
  categoryId: string;
}

export interface PostTagRelation {
  postId: string;
  tagId: string;
}

export interface ArchiveItemPostRelation {
  archiveItemId: string;
  postId: string;
}
