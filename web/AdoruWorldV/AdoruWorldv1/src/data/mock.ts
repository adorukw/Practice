import type { BlogPost, Category, Tag, Author, Project, SiteStats, NavItem } from '@/types'

export const navItems: NavItem[] = [
  { name: '首页', path: '/' },
  { name: '图鉴', path: '/dex' },
  { name: '分类', path: '/categories' },
  { name: '归档', path: '/archives' },
  { name: '项目', path: '/projects' },
  { name: '关于', path: '/about' }
]

export const categories: Category[] = [
  { id: '1', name: '技术笔记', slug: 'tech', description: '编程、开发、技术教程', count: 12, icon: '💻', color: '#3C5AA6' },
  { id: '2', name: '生活随笔', slug: 'life', description: '日常记录、思考、见闻', count: 8, icon: '🌸', color: '#FF7300' },
  { id: '3', name: '项目记录', slug: 'projects', description: '个人项目开发日志', count: 5, icon: '🚀', color: '#FF0000' },
  { id: '4', name: '读书笔记', slug: 'reading', description: '书籍阅读心得', count: 6, icon: '📚', color: '#7B5BA6' },
  { id: '5', name: '游戏人生', slug: 'gaming', description: '游戏评测、心得', count: 4, icon: '🎮', color: '#FFDE00' },
  { id: '6', name: '创意思考', slug: 'creative', description: '设计、艺术、创意分享', count: 3, icon: '✨', color: '#9CBB0F' }
]

export const tags: Tag[] = [
  { id: '1', name: 'Vue', slug: 'vue', color: '#42b883', count: 8 },
  { id: '2', name: 'TypeScript', slug: 'typescript', color: '#3178c6', count: 6 },
  { id: '3', name: 'React', slug: 'react', color: '#61dafb', count: 4 },
  { id: '4', name: 'Node.js', slug: 'nodejs', color: '#68a063', count: 5 },
  { id: '5', name: '前端', slug: 'frontend', color: '#FF7300', count: 10 },
  { id: '6', name: '后端', slug: 'backend', color: '#3C5AA6', count: 4 },
  { id: '7', name: 'CSS', slug: 'css', color: '#264de4', count: 3 },
  { id: '8', name: '像素艺术', slug: 'pixel-art', color: '#FFDE00', count: 2 },
  { id: '9', name: '游戏开发', slug: 'game-dev', color: '#FF0000', count: 3 },
  { id: '10', name: '生活', slug: 'life', color: '#9CBB0F', count: 8 }
]

export const posts: BlogPost[] = [
  {
    id: '1',
    slug: 'vue3-pixel-blog',
    title: '使用Vue 3构建像素风格博客',
    description: '本文介绍如何使用Vue 3和Tailwind CSS构建一个口袋妖怪风格的像素艺术博客，包括技术选型、实现细节和部署方案。',
    content: `# 使用Vue 3构建像素风格博客

## 前言

作为一个口袋妖怪的忠实粉丝，我一直想要创建一个具有GBA时代风格的个人博客。在这篇文章中，我将分享如何使用Vue 3和Tailwind CSS来实现这个目标。

## 技术选型

- **框架**: Vue 3 + Composition API
- **构建工具**: Vite
- **样式方案**: Tailwind CSS + 自定义像素样式
- **状态管理**: Pinia
- **路由**: Vue Router

## 设计系统

### 配色方案

我们采用了Gen 3口袋妖怪的典型配色：

\`\`\`css
--color-pokemon-blue: #3C5AA6;
--color-pokemon-red: #FF0000;
--color-pokemon-yellow: #FFDE00;
--color-pokemon-green: #9CBB0F;
\`\`\`

### 像素边框效果

\`\`\`css
.pixel-border {
  border: 4px solid var(--color-pokemon-black);
  box-shadow: 4px 4px 0 var(--color-pokemon-dark-gray);
}
\`\`\`

## 总结

通过这个项目，我们不仅实现了一个功能完整的博客系统，还创造了一个独特的视觉体验。`,
    coverImage: 'https://picsum.photos/seed/pixel-blog/800/400',
    created_at: '2024-01-15',
    updated_at: '2024-01-20',
    published: true,
    category: ['技术笔记', '项目记录'],
    tags: ['Vue', 'TypeScript', '前端', '像素艺术'],
    reading_time: 8,
    word_count: 2500,
    views: 1234,
    featured: true
  },
  {
    id: '2',
    slug: 'typescript-advanced-patterns',
    title: 'TypeScript高级类型模式详解',
    description: '深入探讨TypeScript中的高级类型模式，包括条件类型、映射类型、模板字面量类型等高级特性的实际应用。',
    content: `# TypeScript高级类型模式详解

## 条件类型

条件类型允许我们根据类型关系进行条件判断：

\`\`\`typescript
type IsString<T> = T extends string ? true : false;

type A = IsString<string>; // true
type B = IsString<number>; // false
\`\`\`

## 映射类型

映射类型可以基于现有类型创建新类型：

\`\`\`typescript
type Readonly<T> = {
  readonly [P in keyof T]: T[P];
};
\`\`\`

## 总结

掌握这些高级类型模式，可以让你的TypeScript代码更加类型安全和可维护。`,
    coverImage: 'https://picsum.photos/seed/typescript/800/400',
    created_at: '2024-01-10',
    updated_at: '2024-01-12',
    published: true,
    category: ['技术笔记'],
    tags: ['TypeScript', '前端'],
    reading_time: 12,
    word_count: 3800,
    views: 856,
    featured: true
  },
  {
    id: '3',
    slug: 'life-in-2024',
    title: '2024年生活回顾',
    description: '回顾2024年的生活点滴，记录成长与变化，展望未来的方向。',
    content: `# 2024年生活回顾

## 年初计划

2024年初，我给自己定下了几个目标：

1. 学习一门新技能
2. 完成一个个人项目
3. 保持健康的生活方式

## 完成情况

### 技术成长

今年我深入学习了Vue 3和TypeScript，并完成了这个像素风格的博客项目。

### 生活变化

搬了新家，养了一只猫，生活变得更加充实。

## 展望2025

新的一年，希望能够继续保持学习的热情，创造更多有价值的内容。`,
    coverImage: 'https://picsum.photos/seed/life-2024/800/400',
    created_at: '2024-12-31',
    updated_at: '2024-12-31',
    published: true,
    category: ['生活随笔'],
    tags: ['生活'],
    reading_time: 5,
    word_count: 1200,
    views: 432,
    featured: false
  },
  {
    id: '4',
    slug: 'nodejs-best-practices',
    title: 'Node.js最佳实践指南',
    description: '总结Node.js开发中的最佳实践，包括项目结构、错误处理、性能优化等方面。',
    content: `# Node.js最佳实践指南

## 项目结构

推荐的项目结构：

\`\`\`
src/
├── controllers/
├── services/
├── models/
├── middlewares/
├── utils/
└── app.ts
\`\`\`

## 错误处理

使用统一的错误处理中间件：

\`\`\`typescript
app.use((err, req, res, next) => {
  console.error(err.stack);
  res.status(500).json({ error: 'Something went wrong!' });
});
\`\`\`

## 性能优化

- 使用集群模式
- 启用压缩
- 使用缓存
- 优化数据库查询`,
    coverImage: 'https://picsum.photos/seed/nodejs/800/400',
    created_at: '2024-02-05',
    updated_at: '2024-02-08',
    published: true,
    category: ['技术笔记'],
    tags: ['Node.js', '后端'],
    reading_time: 10,
    word_count: 2800,
    views: 678,
    featured: false
  },
  {
    id: '5',
    slug: 'game-dev-diary-1',
    title: '独立游戏开发日记：第一周',
    description: '记录我开发第一款独立游戏的第一周经历，从构思到原型实现。',
    content: `# 独立游戏开发日记：第一周

## 游戏概念

我决定开发一款像素风格的冒险游戏，灵感来自经典的口袋妖怪系列。

## 技术选择

- 游戏引擎: Phaser 3
- 语言: TypeScript
- 美术: Aseprite

## 本周进展

### 完成的工作

1. 搭建了基础项目框架
2. 实现了角色移动
3. 创建了第一个测试地图

### 遇到的挑战

- 地图碰撞检测的实现
- 动画帧同步问题

## 下周计划

- 添加NPC交互
- 实现战斗系统原型`,
    coverImage: 'https://picsum.photos/seed/game-dev/800/400',
    created_at: '2024-03-01',
    updated_at: '2024-03-01',
    published: true,
    category: ['项目记录', '游戏人生'],
    tags: ['游戏开发', 'TypeScript'],
    reading_time: 6,
    word_count: 1500,
    views: 345,
    featured: false
  },
  {
    id: '6',
    slug: 'reading-clean-code',
    title: '《代码整洁之道》读书笔记',
    description: '分享阅读《代码整洁之道》的心得体会，总结编写整洁代码的关键原则。',
    content: `# 《代码整洁之道》读书笔记

## 核心原则

### 有意义的命名

变量、函数、类的名称应该表达其意图：

\`\`\`typescript
// 不好的命名
const d = new Date();

// 好的命名
const currentDate = new Date();
\`\`\`

### 函数应该短小

函数应该只做一件事，做好这件事，只做这一件事。

### 注释

最好的注释是不需要注释。代码应该自我解释。

## 实践建议

1. 保持代码格式一致
2. 遵循团队规范
3. 及时重构
4. 编写测试

## 总结

整洁的代码是专业程序员的基本素养，需要持续练习和改进。`,
    coverImage: 'https://picsum.photos/seed/clean-code/800/400',
    created_at: '2024-02-20',
    updated_at: '2024-02-22',
    published: true,
    category: ['读书笔记'],
    tags: ['前端', '后端'],
    reading_time: 7,
    word_count: 1800,
    views: 567,
    featured: false
  }
]

export const author: Author = {
  name: 'AdoruKw',
  avatar: 'https://picsum.photos/seed/avatar/200/200',
  bio: '一个热爱编程和游戏的开发者，正在探索像素艺术与Web开发的结合。',
  location: '中国',
  social: {
    github: 'https://github.com',
    twitter: 'https://twitter.com',
    email: 'hello@example.com'
  }
}

export const projects: Project[] = [
  {
    id: '1',
    name: 'AdoruWorld Blog',
    description: '口袋妖怪风格的像素艺术个人博客，使用Vue 3和Tailwind CSS构建。',
    tech: ['Vue 3', 'TypeScript', 'Tailwind CSS', 'Vite'],
    link: 'https://example.com',
    github: 'https://github.com',
    image: 'https://picsum.photos/seed/project-blog/600/400',
    status: 'in-progress'
  },
  {
    id: '2',
    name: 'Pixel Adventure',
    description: '一款像素风格的冒险游戏，使用Phaser 3开发。',
    tech: ['Phaser 3', 'TypeScript', 'Aseprite'],
    github: 'https://github.com',
    image: 'https://picsum.photos/seed/project-game/600/400',
    status: 'in-progress'
  },
  {
    id: '3',
    name: 'Task Manager',
    description: '一个简洁的任务管理应用，支持拖拽排序和标签分类。',
    tech: ['React', 'Node.js', 'MongoDB'],
    link: 'https://example.com',
    github: 'https://github.com',
    image: 'https://picsum.photos/seed/project-task/600/400',
    status: 'completed'
  },
  {
    id: '4',
    name: 'Weather App',
    description: '天气预报应用，支持多城市切换和天气动画效果。',
    tech: ['Vue 3', 'OpenWeather API'],
    github: 'https://github.com',
    image: 'https://picsum.photos/seed/project-weather/600/400',
    status: 'archived'
  }
]

export const siteStats: SiteStats = {
  totalPosts: 38,
  totalWords: 125000,
  totalViews: 15680,
  runningDays: 365
}

export function getPostBySlug(slug: string): BlogPost | undefined {
  return posts.find(post => post.slug === slug)
}

export function getPostsByCategory(categorySlug: string): BlogPost[] {
  const category = categories.find(c => c.slug === categorySlug)
  if (!category) return []
  return posts.filter(post => post.category.includes(category.name))
}

export function getPostsByTag(tagSlug: string): BlogPost[] {
  const tag = tags.find(t => t.slug === tagSlug)
  if (!tag) return []
  return posts.filter(post => post.tags.includes(tag.name))
}

export function searchPosts(query: string): BlogPost[] {
  const lowerQuery = query.toLowerCase()
  return posts.filter(post => 
    post.title.toLowerCase().includes(lowerQuery) ||
    post.description.toLowerCase().includes(lowerQuery) ||
    post.tags.some(tag => tag.toLowerCase().includes(lowerQuery))
  )
}
