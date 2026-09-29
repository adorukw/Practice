import { defineStore } from 'pinia'
import { ref, computed } from 'vue'

const categories = [
  { slug: 'game', name: '游戏', nameEn: 'GAME', icon: '🎮', color: '#3C5AA6', description: '探索游戏世界的无限可能' },
  { slug: 'media', name: '影音', nameEn: 'MEDIA', icon: '🎬', color: '#E74040', description: '影视与动画的精彩瞬间' },
  { slug: 'audio', name: '音乐', nameEn: 'AUDIO', icon: '🎵', color: '#FFDE00', description: '聆听心灵的旋律' },
  { slug: 'text', name: '书籍', nameEn: 'TEXT', icon: '📖', color: '#4CAF50', description: '文字中的智慧与想象' },
  { slug: 'project', name: '创造', nameEn: 'PROJECT', icon: '💡', color: '#9C27B0', description: '从灵感到现实的创造之旅' },
]

const works = [
  {
    id: '001',
    title: '塞尔达传说：王国之泪',
    slug: 'zelda-totk',
    category: 'game',
    tags: ['开放世界', '冒险', '任天堂'],
    coverImage: 'https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/pokeball.png',
    images: [
      'https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/pokemon/other/official-artwork/25.png',
      'https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/pokemon/other/official-artwork/1.png',
    ],
    description: '在海拉鲁大陆上展开全新的冒险，利用究极手和余料建造等新能力，探索天空岛屿和地底世界。',
    date: '2026-05-12',
    featured: true,
    status: 'playing',
    rating: 5,
  },
  {
    id: '002',
    title: '千与千寻',
    slug: 'spirited-away',
    category: 'media',
    tags: ['动画', '宫崎骏', '吉卜力'],
    coverImage: 'https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/great-ball.png',
    images: [
      'https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/pokemon/other/official-artwork/133.png',
    ],
    description: '少女千寻误入神灵世界，在汤屋打工的过程中逐渐成长，最终拯救了父母的故事。',
    date: '2026-04-20',
    featured: true,
    status: 'watched',
    rating: 5,
  },
  {
    id: '003',
    title: 'OK Computer',
    slug: 'ok-computer',
    category: 'audio',
    tags: ['摇滚', '另类', 'Radiohead'],
    coverImage: 'https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/ultra-ball.png',
    images: [],
    description: 'Radiohead的里程碑式专辑，融合了电子乐与摇滚，探讨了科技时代下人类的焦虑与疏离。',
    date: '2026-03-15',
    featured: true,
    status: 'listened',
    rating: 5,
  },
  {
    id: '004',
    title: '百年孤独',
    slug: 'one-hundred-years',
    category: 'text',
    tags: ['文学', '魔幻现实', '马尔克斯'],
    coverImage: 'https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/master-ball.png',
    images: [],
    description: '布恩迪亚家族七代人的传奇故事，魔幻现实主义文学的巅峰之作。',
    date: '2026-02-10',
    featured: false,
    status: 'read',
    rating: 5,
  },
  {
    id: '005',
    title: '艾尔登法环',
    slug: 'elden-ring',
    category: 'game',
    tags: ['动作RPG', '开放世界', 'FromSoftware'],
    coverImage: 'https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/pokeball.png',
    images: [
      'https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/pokemon/other/official-artwork/6.png',
    ],
    description: '在交界地展开史诗般的冒险，挑战半神们，成为艾尔登之王。',
    date: '2026-01-28',
    featured: false,
    status: 'played',
    rating: 5,
  },
  {
    id: '006',
    title: '星际穿越',
    slug: 'interstellar',
    category: 'media',
    tags: ['科幻', '诺兰', '太空'],
    coverImage: 'https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/great-ball.png',
    images: [],
    description: '在地球面临末日危机时，一群探险者穿越虫洞，寻找人类新家园的壮阔旅程。',
    date: '2026-01-15',
    featured: false,
    status: 'watched',
    rating: 4,
  },
  {
    id: '007',
    title: 'Dark Side of the Moon',
    slug: 'dark-side-moon',
    category: 'audio',
    tags: ['前卫摇滚', 'Pink Floyd', '经典'],
    coverImage: 'https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/ultra-ball.png',
    images: [],
    description: 'Pink Floyd的概念专辑杰作，探索疯狂、时间、金钱与死亡的主题。',
    date: '2025-12-20',
    featured: false,
    status: 'listened',
    rating: 5,
  },
  {
    id: '008',
    title: '三体',
    slug: 'three-body',
    category: 'text',
    tags: ['科幻', '刘慈欣', '硬科幻'],
    coverImage: 'https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/master-ball.png',
    images: [],
    description: '人类文明与三体文明的史诗对决，中国科幻文学的里程碑。',
    date: '2025-12-01',
    featured: true,
    status: 'read',
    rating: 5,
  },
  {
    id: '009',
    title: '个人博客系统',
    slug: 'personal-blog',
    category: 'project',
    tags: ['Vue', '前端', '全栈'],
    coverImage: 'https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/pokeball.png',
    images: [],
    description: '使用Vue3和Node.js构建的个人博客系统，支持Markdown写作和主题切换。',
    date: '2025-11-15',
    featured: false,
    status: 'completed',
    rating: 4,
  },
  {
    id: '010',
    title: '空洞骑士',
    slug: 'hollow-knight',
    category: 'game',
    tags: ['银河恶魔城', '独立游戏', 'Team Cherry'],
    coverImage: 'https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/pokeball.png',
    images: [
      'https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/pokemon/other/official-artwork/150.png',
    ],
    description: '在广阔的虫族王国中探索，揭开圣巢的神秘面纱。',
    date: '2025-10-20',
    featured: false,
    status: 'want',
    rating: 0,
  },
  {
    id: '011',
    title: '攻壳机动队',
    slug: 'ghost-in-shell',
    category: 'media',
    tags: ['科幻', '动画', '押井守'],
    coverImage: 'https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/great-ball.png',
    images: [],
    description: '在赛博朋克的世界中，探讨人类意识与机械躯体的哲学命题。',
    date: '2025-10-01',
    featured: false,
    status: 'want',
    rating: 0,
  },
  {
    id: '012',
    title: '像素风游戏引擎',
    slug: 'pixel-engine',
    category: 'project',
    tags: ['TypeScript', 'Canvas', '游戏开发'],
    coverImage: 'https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/items/pokeball.png',
    images: [],
    description: '基于Canvas的轻量级像素风游戏引擎，支持精灵动画和碰撞检测。',
    date: '2025-09-10',
    featured: true,
    status: 'in-progress',
    rating: 0,
  },
]

export const useCollectionStore = defineStore('collection', () => {
  const allWorks = ref(works)
  const allCategories = ref(categories)
  const searchQuery = ref('')
  const isDarkMode = ref(false)

  const featuredWorks = computed(() => allWorks.value.filter(w => w.featured))

  const latestWorks = computed(() =>
    [...allWorks.value].sort((a, b) => new Date(b.date) - new Date(a.date)).slice(0, 6)
  )

  const filteredWorks = computed(() => {
    if (!searchQuery.value) return allWorks.value
    const q = searchQuery.value.toLowerCase()
    return allWorks.value.filter(w =>
      w.title.toLowerCase().includes(q) ||
      w.tags.some(t => t.toLowerCase().includes(q)) ||
      w.description.toLowerCase().includes(q)
    )
  })

  function getWorksByCategory(slug) {
    return allWorks.value.filter(w => w.category === slug)
  }

  function getWorkById(id) {
    return allWorks.value.find(w => w.id === id)
  }

  function getCategoryBySlug(slug) {
    return allCategories.value.find(c => c.slug === slug)
  }

  function toggleDarkMode() {
    isDarkMode.value = !isDarkMode.value
    document.documentElement.classList.toggle('dark', isDarkMode.value)
  }

  function setSearchQuery(query) {
    searchQuery.value = query
  }

  return {
    allWorks,
    allCategories,
    searchQuery,
    isDarkMode,
    featuredWorks,
    latestWorks,
    filteredWorks,
    getWorksByCategory,
    getWorkById,
    getCategoryBySlug,
    toggleDarkMode,
    setSearchQuery,
  }
})
