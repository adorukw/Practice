import type { DexEntry, DexCategoryInfo, DexStatusInfo, DexStats, DexCategory, DexStatus } from '@/types'

export const dexCategories: DexCategoryInfo[] = [
  { id: 'anime', name: '动画', icon: '📺', color: '#FF6B6B', bgColor: '#FFE8E8' },
  { id: 'movie', name: '电影', icon: '🎬', color: '#4ECDC4', bgColor: '#E8FFFE' },
  { id: 'tv', name: '电视剧', icon: '📡', color: '#45B7D1', bgColor: '#E8F7FC' },
  { id: 'game', name: '游戏', icon: '🎮', color: '#96CEB4', bgColor: '#F0FFF4' },
  { id: 'book', name: '书籍', icon: '📚', color: '#DDA0DD', bgColor: '#FFF0FF' },
  { id: 'music', name: '音乐', icon: '🎵', color: '#FFB347', bgColor: '#FFF8E8' }
]

export const dexStatuses: DexStatusInfo[] = [
  { id: 'completed', name: '已完成', icon: '✅', color: '#4CAF50' },
  { id: 'watching', name: '在看', icon: '👀', color: '#2196F3' },
  { id: 'playing', name: '在玩', icon: '🎯', color: '#9C27B0' },
  { id: 'reading', name: '在读', icon: '📖', color: '#795548' },
  { id: 'listening', name: '在听', icon: '🎧', color: '#FF9800' },
  { id: 'dropped', name: '搁置', icon: '💤', color: '#9E9E9E' },
  { id: 'planned', name: '计划', icon: '📋', color: '#607D8B' }
]

export const dexEntries: DexEntry[] = [
  {
    id: '1',
    slug: 'pokemon-horizons',
    title: '宝可梦 地平线',
    originalTitle: 'ポケットモンスター',
    cover: 'https://picsum.photos/seed/pokemon-anime/300/400',
    category: 'anime',
    status: 'watching',
    rating: 9,
    progress: 45,
    total: 100,
    startDate: '2023-04-14',
    tags: ['冒险', '奇幻', '热血'],
    comment: '新一代宝可梦动画，莉可和罗伊的冒险故事非常精彩！',
    creator: 'OLM',
    year: 2023,
    genre: ['冒险', '奇幻']
  },
  {
    id: '2',
    slug: 'spirited-away',
    title: '千与千寻',
    originalTitle: '千と千尋の神隠し',
    cover: 'https://picsum.photos/seed/spirited-away/300/400',
    category: 'movie',
    status: 'completed',
    rating: 10,
    progress: 1,
    total: 1,
    startDate: '2020-06-15',
    finishDate: '2020-06-15',
    tags: ['奇幻', '冒险', '治愈'],
    comment: '宫崎骏的巅峰之作，每次看都有新的感悟。',
    creator: '宫崎骏',
    year: 2001,
    genre: ['奇幻', '冒险']
  },
  {
    id: '3',
    slug: 'breaking-bad',
    title: '绝命毒师',
    originalTitle: 'Breaking Bad',
    cover: 'https://picsum.photos/seed/breaking-bad/300/400',
    category: 'tv',
    status: 'completed',
    rating: 10,
    progress: 62,
    total: 62,
    startDate: '2022-01-10',
    finishDate: '2022-03-20',
    tags: ['犯罪', '剧情', '悬疑'],
    comment: '神剧！人物塑造和剧情发展都堪称完美。',
    creator: 'Vince Gilligan',
    year: 2008,
    genre: ['犯罪', '剧情']
  },
  {
    id: '4',
    slug: 'pokemon-scarlet-violet',
    title: '宝可梦 朱/紫',
    originalTitle: 'ポケットモンスター・スカーレット・バイオレット',
    cover: 'https://picsum.photos/seed/pokemon-sv/300/400',
    category: 'game',
    status: 'completed',
    rating: 8,
    progress: 1,
    total: 1,
    startDate: '2022-11-18',
    finishDate: '2023-02-28',
    tags: ['RPG', '冒险', '收集'],
    comment: '开放世界宝可梦，虽然技术问题多但游戏性很棒！',
    creator: 'Game Freak',
    year: 2022,
    platform: 'Nintendo Switch',
    genre: ['RPG', '冒险']
  },
  {
    id: '5',
    slug: 'harry-potter-series',
    title: '哈利·波特系列',
    originalTitle: 'Harry Potter',
    cover: 'https://picsum.photos/seed/harry-potter/300/400',
    category: 'book',
    status: 'completed',
    rating: 10,
    progress: 7,
    total: 7,
    startDate: '2015-06-01',
    finishDate: '2016-08-15',
    tags: ['奇幻', '冒险', '成长'],
    comment: '陪伴我成长的经典，魔法世界永远令人向往。',
    creator: 'J.K. Rowling',
    year: 1997,
    genre: ['奇幻', '冒险']
  },
  {
    id: '6',
    slug: 'radiohead-ok-computer',
    title: 'OK Computer',
    cover: 'https://picsum.photos/seed/ok-computer/300/400',
    category: 'music',
    status: 'completed',
    rating: 10,
    progress: 1,
    total: 1,
    startDate: '2021-03-10',
    tags: ['摇滚', '另类', '实验'],
    comment: 'Radiohead的巅峰之作，每一首歌都是艺术品。',
    creator: 'Radiohead',
    year: 1997,
    genre: ['另类摇滚', '艺术摇滚']
  },
  {
    id: '7',
    slug: 'elden-ring',
    title: '艾尔登法环',
    originalTitle: 'ELDEN RING',
    cover: 'https://picsum.photos/seed/elden-ring/300/400',
    category: 'game',
    status: 'completed',
    rating: 10,
    progress: 1,
    total: 1,
    startDate: '2022-02-25',
    finishDate: '2022-05-15',
    tags: ['动作RPG', '开放世界', '魂系'],
    comment: '宫崎英高与乔治·R·R·马丁的完美合作，年度最佳！',
    creator: 'FromSoftware',
    year: 2022,
    platform: 'PC / PS5 / Xbox',
    genre: ['动作RPG', '开放世界']
  },
  {
    id: '8',
    slug: 'your-name',
    title: '你的名字。',
    originalTitle: '君の名は。',
    cover: 'https://picsum.photos/seed/your-name/300/400',
    category: 'movie',
    status: 'completed',
    rating: 9,
    progress: 1,
    total: 1,
    startDate: '2016-12-02',
    finishDate: '2016-12-02',
    tags: ['爱情', '奇幻', '动画'],
    comment: '新海诚的视觉盛宴，故事也很动人。',
    creator: '新海诚',
    year: 2016,
    genre: ['爱情', '奇幻']
  },
  {
    id: '9',
    slug: 'steins-gate',
    title: '命运石之门',
    originalTitle: 'STEINS;GATE',
    cover: 'https://picsum.photos/seed/steins-gate/300/400',
    category: 'anime',
    status: 'completed',
    rating: 10,
    progress: 24,
    total: 24,
    startDate: '2021-07-01',
    finishDate: '2021-07-15',
    tags: ['科幻', '时间旅行', '悬疑'],
    comment: '神作！前期铺垫后期爆发，剧情设计太精妙了。',
    creator: 'WHITE FOX',
    year: 2011,
    genre: ['科幻', '悬疑']
  },
  {
    id: '10',
    slug: 'zelda-totk',
    title: '塞尔达传说 王国之泪',
    originalTitle: 'ゼルダの伝説 ティアーズ オブ ザ キングダム',
    cover: 'https://picsum.photos/seed/zelda-totk/300/400',
    category: 'game',
    status: 'playing',
    rating: 0,
    progress: 60,
    total: 100,
    startDate: '2023-05-12',
    tags: ['动作冒险', '开放世界', '解谜'],
    comment: '旷野之息的续作，建造系统太有趣了！',
    creator: 'Nintendo',
    year: 2023,
    platform: 'Nintendo Switch',
    genre: ['动作冒险', '开放世界']
  },
  {
    id: '11',
    slug: 'dune-part2',
    title: '沙丘2',
    originalTitle: 'Dune: Part Two',
    cover: 'https://picsum.photos/seed/dune2/300/400',
    category: 'movie',
    status: 'completed',
    rating: 9,
    progress: 1,
    total: 1,
    startDate: '2024-03-01',
    finishDate: '2024-03-01',
    tags: ['科幻', '史诗', '冒险'],
    comment: '维伦纽瓦的史诗巨作，视觉效果震撼！',
    creator: 'Denis Villeneuve',
    year: 2024,
    genre: ['科幻', '史诗']
  },
  {
    id: '12',
    slug: 'pink-floyd-dark-side',
    title: 'The Dark Side of the Moon',
    cover: 'https://picsum.photos/seed/dark-side/300/400',
    category: 'music',
    status: 'completed',
    rating: 10,
    progress: 1,
    total: 1,
    startDate: '2020-05-20',
    tags: ['摇滚', '前卫', '经典'],
    comment: '摇滚史上最伟大的专辑之一，必须完整聆听。',
    creator: 'Pink Floyd',
    year: 1973,
    genre: ['前卫摇滚']
  },
  {
    id: '13',
    slug: 'attack-on-titan',
    title: '进击的巨人',
    originalTitle: '進撃の巨人',
    cover: 'https://picsum.photos/seed/aot/300/400',
    category: 'anime',
    status: 'completed',
    rating: 10,
    progress: 87,
    total: 87,
    startDate: '2020-01-01',
    finishDate: '2023-11-04',
    tags: ['动作', '黑暗', '剧情'],
    comment: '史诗级作品，结局虽然争议但整体是神作。',
    creator: 'MAPPA / Wit Studio',
    year: 2013,
    genre: ['动作', '黑暗奇幻']
  },
  {
    id: '14',
    slug: 'three-body-problem',
    title: '三体',
    cover: 'https://picsum.photos/seed/three-body/300/400',
    category: 'book',
    status: 'completed',
    rating: 10,
    progress: 3,
    total: 3,
    startDate: '2019-04-01',
    finishDate: '2019-06-15',
    tags: ['科幻', '硬科幻', '史诗'],
    comment: '中国科幻的巅峰之作，想象力令人震撼。',
    creator: '刘慈欣',
    year: 2008,
    genre: ['科幻']
  },
  {
    id: '15',
    slug: 'persona-5-royal',
    title: '女神异闻录5 皇家版',
    originalTitle: 'ペルソナ5 ザ・ロイヤル',
    cover: 'https://picsum.photos/seed/p5r/300/400',
    category: 'game',
    status: 'completed',
    rating: 10,
    progress: 1,
    total: 1,
    startDate: '2022-08-01',
    finishDate: '2022-10-15',
    tags: ['RPG', '回合制', '校园'],
    comment: 'P5天下第一！音乐、美术、剧情都是顶级。',
    creator: 'ATLUS',
    year: 2020,
    platform: 'PC / PS4 / Switch / Xbox',
    genre: ['RPG', '回合制']
  }
]

export const dexStats: DexStats = {
  total: dexEntries.length,
  byCategory: {
    anime: dexEntries.filter(e => e.category === 'anime').length,
    movie: dexEntries.filter(e => e.category === 'movie').length,
    tv: dexEntries.filter(e => e.category === 'tv').length,
    game: dexEntries.filter(e => e.category === 'game').length,
    book: dexEntries.filter(e => e.category === 'book').length,
    music: dexEntries.filter(e => e.category === 'music').length
  },
  byStatus: {
    completed: dexEntries.filter(e => e.status === 'completed').length,
    watching: dexEntries.filter(e => e.status === 'watching').length,
    playing: dexEntries.filter(e => e.status === 'playing').length,
    reading: dexEntries.filter(e => e.status === 'reading').length,
    listening: dexEntries.filter(e => e.status === 'listening').length,
    dropped: dexEntries.filter(e => e.status === 'dropped').length,
    planned: dexEntries.filter(e => e.status === 'planned').length
  },
  averageRating: Number((dexEntries.filter(e => e.rating > 0).reduce((a, b) => a + b.rating, 0) / dexEntries.filter(e => e.rating > 0).length).toFixed(1))
}

export function getDexEntryBySlug(slug: string): DexEntry | undefined {
  return dexEntries.find(entry => entry.slug === slug)
}

export function getDexEntriesByCategory(category: DexCategory): DexEntry[] {
  return dexEntries.filter(entry => entry.category === category)
}

export function getDexEntriesByStatus(status: DexStatus): DexEntry[] {
  return dexEntries.filter(entry => entry.status === status)
}

export function getCategoryInfo(category: DexCategory): DexCategoryInfo | undefined {
  return dexCategories.find(c => c.id === category)
}

export function getStatusInfo(status: DexStatus): DexStatusInfo | undefined {
  return dexStatuses.find(s => s.id === status)
}
