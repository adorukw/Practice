import { defineStore } from 'pinia'
import { ref, computed } from 'vue'
import type { BlogPost, Category, Tag } from '@/types'
import { posts, categories, tags, siteStats, author } from '@/data/mock'

export const useBlogStore = defineStore('blog', () => {
  const allPosts = ref<BlogPost[]>(posts)
  const allCategories = ref<Category[]>(categories)
  const allTags = ref<Tag[]>(tags)
  const currentPost = ref<BlogPost | null>(null)
  const searchQuery = ref('')
  const selectedCategory = ref<string | null>(null)
  const selectedTag = ref<string | null>(null)
  const isDarkMode = ref(false)

  const featuredPosts = computed(() => 
    allPosts.value.filter(post => post.featured)
  )

  const recentPosts = computed(() => 
    [...allPosts.value]
      .sort((a, b) => new Date(b.created_at).getTime() - new Date(a.created_at).getTime())
      .slice(0, 5)
  )

  const filteredPosts = computed(() => {
    let result = allPosts.value

    if (searchQuery.value) {
      const query = searchQuery.value.toLowerCase()
      result = result.filter(post =>
        post.title.toLowerCase().includes(query) ||
        post.description.toLowerCase().includes(query) ||
        post.tags.some(tag => tag.toLowerCase().includes(query))
      )
    }

    if (selectedCategory.value) {
      result = result.filter(post =>
        post.category.includes(selectedCategory.value!)
      )
    }

    if (selectedTag.value) {
      result = result.filter(post =>
        post.tags.includes(selectedTag.value!)
      )
    }

    return result
  })

  const stats = ref(siteStats)
  const authorInfo = ref(author)

  function getPostBySlug(slug: string): BlogPost | undefined {
    return allPosts.value.find(post => post.slug === slug)
  }

  function getPostsByCategory(categoryName: string): BlogPost[] {
    return allPosts.value.filter(post => post.category.includes(categoryName))
  }

  function getPostsByTag(tagName: string): BlogPost[] {
    return allPosts.value.filter(post => post.tags.includes(tagName))
  }

  function setSearchQuery(query: string) {
    searchQuery.value = query
  }

  function setCategory(category: string | null) {
    selectedCategory.value = category
  }

  function setTag(tag: string | null) {
    selectedTag.value = tag
  }

  function toggleDarkMode() {
    isDarkMode.value = !isDarkMode.value
    document.documentElement.classList.toggle('dark', isDarkMode.value)
  }

  return {
    allPosts,
    allCategories,
    allTags,
    currentPost,
    searchQuery,
    selectedCategory,
    selectedTag,
    isDarkMode,
    featuredPosts,
    recentPosts,
    filteredPosts,
    stats,
    authorInfo,
    getPostBySlug,
    getPostsByCategory,
    getPostsByTag,
    setSearchQuery,
    setCategory,
    setTag,
    toggleDarkMode
  }
})
