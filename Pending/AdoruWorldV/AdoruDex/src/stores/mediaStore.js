import { defineStore } from 'pinia'
import { ref, computed } from 'vue'

export const useMediaStore = defineStore('media', () => {
  const items = ref([
    {
      id: 1,
      type: 'anime',
      title: '孤独摇滚！',
      titleEn: 'Bocchi the Rock!',
      cover: '',
      rating: 9,
      status: 'completed',
      episodes: 12,
      year: 2022,
      date: '2024-03-15',
      notes: '让人感同身受的社恐日常，音乐部分也很出色',
      tags: ['日常', '音乐', '搞笑']
    },
    {
      id: 2,
      type: 'anime',
      title: '葬送的芙莉莲',
      titleEn: 'Sousou no Frieren',
      cover: '',
      rating: 10,
      status: 'watching',
      episodes: 28,
      progress: 20,
      year: 2023,
      date: '2024-06-01',
      notes: '时间与记忆的叙事诗，制作精良',
      tags: ['奇幻', '冒险', '治愈']
    },
    {
      id: 3,
      type: 'movie',
      title: '奥本海默',
      titleEn: 'Oppenheimer',
      cover: '',
      rating: 8,
      status: 'completed',
      year: 2023,
      date: '2024-02-20',
      notes: '诺兰的传记杰作，默菲演技炸裂',
      tags: ['传记', '历史', '剧情']
    },
    {
      id: 4,
      type: 'game',
      title: '塞尔达传说 王国之泪',
      titleEn: 'Tears of the Kingdom',
      cover: '',
      rating: 10,
      status: 'playing',
      platform: 'Nintendo Switch',
      year: 2023,
      date: '2024-01-10',
      notes: '究极手和通天术改变了开放世界的玩法范式',
      tags: ['开放世界', '冒险', '解谜']
    },
    {
      id: 5,
      type: 'game',
      title: '艾尔登法环',
      titleEn: 'Elden Ring',
      cover: '',
      rating: 9,
      status: 'completed',
      platform: 'PC',
      year: 2022,
      date: '2023-08-15',
      notes: '交界地的壮丽史诗，难度略有下降但体验一流',
      tags: ['魂系', '开放世界', '奇幻']
    },
    {
      id: 6,
      type: 'anime',
      title: '间谍过家家',
      titleEn: 'Spy x Family',
      cover: '',
      rating: 8,
      status: 'completed',
      episodes: 25,
      year: 2022,
      date: '2023-11-20',
      notes: '安妮亚太可爱了，家庭温馨与谍战动作完美融合',
      tags: ['喜剧', '动作', '日常']
    },
    {
      id: 7,
      type: 'movie',
      title: '蜘蛛侠：纵横宇宙',
      titleEn: 'Spider-Man: Across the Spider-Verse',
      cover: '',
      rating: 9,
      status: 'completed',
      year: 2023,
      date: '2024-04-05',
      notes: '视觉风格的又一次突破，每个宇宙都是艺术品',
      tags: ['动画', '超级英雄', '科幻']
    },
    {
      id: 8,
      type: 'anime',
      title: '跃动青春',
      titleEn: 'Skip and Loafer',
      cover: '',
      rating: 8,
      status: 'completed',
      episodes: 12,
      year: 2023,
      date: '2024-05-10',
      notes: '青春群像剧的教科书，每个人都闪闪发光',
      tags: ['校园', '青春', '日常']
    }
  ])

  const statusMap = {
    anime: { completed: '已看完', watching: '在看', plan: '想看', dropped: '弃了' },
    movie: { completed: '已看完', watching: '在看', plan: '想看', dropped: '弃了' },
    game: { completed: '已通关', playing: '在玩', plan: '想玩', dropped: '弃了', paused: '暂停' }
  }

  const typeLabels = { anime: '动画', movie: '电影', game: '游戏' }

  const getItemsByType = computed(() => (type) => items.value.filter(i => i.type === type))

  const stats = computed(() => {
    const total = items.value.length
    const byType = {}
    const byStatus = {}
    let totalRating = 0
    let ratingCount = 0

    for (const item of items.value) {
      byType[item.type] = (byType[item.type] || 0) + 1
      byStatus[item.status] = (byStatus[item.status] || 0) + 1
      if (item.rating) {
        totalRating += item.rating
        ratingCount++
      }
    }

    return {
      total,
      byType,
      byStatus,
      averageRating: ratingCount ? (totalRating / ratingCount).toFixed(1) : 0
    }
  })

  function addItem(item) {
    const newItem = {
      ...item,
      id: Date.now(),
      date: new Date().toISOString().split('T')[0]
    }
    items.value.push(newItem)
    return newItem
  }

  function updateItem(id, data) {
    const index = items.value.findIndex(i => i.id === id)
    if (index !== -1) {
      items.value[index] = { ...items.value[index], ...data }
    }
  }

  function removeItem(id) {
    items.value = items.value.filter(i => i.id !== id)
  }

  function getItemById(id) {
    return items.value.find(i => i.id === Number(id))
  }

  return {
    items,
    statusMap,
    typeLabels,
    getItemsByType,
    stats,
    addItem,
    updateItem,
    removeItem,
    getItemById
  }
})
