import { defineStore } from 'pinia'
import { ref, computed } from 'vue'
import { api } from '@/api'

export const useBlogStore = defineStore('blog', () => {
  const allPosts = ref<any[]>([])
  const allCategories = ref<any[]>([])
  const allTags = ref<any[]>([])
  const allProjects = ref<any[]>([])
  const allDexEntries = ref<any[]>([])
  const searchQuery = ref('')
  const selectedCategory = ref<string | null>(null)
  const selectedTag = ref<string | null>(null)
  const stats = ref({ totalPosts: 0, totalWords: 0, totalViews: 0, runningDays: 0 })
  const authorInfo = ref({
    name: 'AdoruKw',
    avatar: '',
    bio: '',
    location: '',
    social: { github: '', twitter: '', email: '' }
  })
  const loading = ref(false)

  const featuredPosts = computed(() => allPosts.value.filter(p => p.featured))
  const recentPosts = computed(() => [...allPosts.value].sort((a, b) => new Date(b.createdAt).getTime() - new Date(a.createdAt).getTime()).slice(0, 5))

  const filteredPosts = computed(() => {
    let result = allPosts.value
    if (searchQuery.value) {
      const q = searchQuery.value.toLowerCase()
      result = result.filter(p => p.title.toLowerCase().includes(q) || p.description.toLowerCase().includes(q) || (p.tagNames || []).some((t: string) => t.toLowerCase().includes(q)))
    }
    if (selectedCategory.value) {
      result = result.filter(p => (p.category || []).includes(selectedCategory.value))
    }
    if (selectedTag.value) {
      result = result.filter(p => (p.tagNames || []).includes(selectedTag.value))
    }
    return result
  })

  async function fetchPosts() {
    loading.value = true
    try {
      allPosts.value = await api.posts.list({ published: 'true' })
    } finally { loading.value = false }
  }

  async function fetchCategories() {
    allCategories.value = await api.categories.list()
  }

  async function fetchTags() {
    allTags.value = await api.tags.list()
  }

  async function fetchProjects() {
    allProjects.value = await api.projects.list()
  }

  async function fetchDexEntries() {
    allDexEntries.value = await api.dex.list()
  }

  async function fetchStats() {
    stats.value = await api.site.stats()
  }

  async function fetchAuthor() {
    authorInfo.value = await api.site.author()
  }

  async function fetchAll() {
    await Promise.all([fetchPosts(), fetchCategories(), fetchTags(), fetchProjects(), fetchDexEntries(), fetchStats(), fetchAuthor()])
  }

  function setSearchQuery(q: string) { searchQuery.value = q }
  function setCategory(c: string | null) { selectedCategory.value = c }
  function setTag(t: string | null) { selectedTag.value = t }

  return {
    allPosts, allCategories, allTags, allProjects, allDexEntries,
    searchQuery, selectedCategory, selectedTag, stats, authorInfo, loading,
    featuredPosts, recentPosts, filteredPosts,
    fetchPosts, fetchCategories, fetchTags, fetchProjects, fetchDexEntries, fetchStats, fetchAuthor, fetchAll,
    setSearchQuery, setCategory, setTag
  }
})
