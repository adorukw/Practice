<script setup lang="ts">
import { ref, computed, onMounted } from 'vue'
import { api, uploadFile } from '@/api'

const activeTab = ref('posts')
const loading = ref(false)
const uploading = ref(false)

const posts = ref<any[]>([])
const categories = ref<any[]>([])
const tags = ref<any[]>([])
const projects = ref<any[]>([])
const dexEntries = ref<any[]>([])

const editingItem = ref<any>(null)
const showEditor = ref(false)
const editorMode = ref<'create' | 'edit'>('create')

const tabs = [
  { id: 'posts', name: '文章', icon: '📝' },
  { id: 'categories', name: '分类', icon: '📁' },
  { id: 'tags', name: '标签', icon: '🏷️' },
  { id: 'projects', name: '项目', icon: '🚀' },
  { id: 'dex', name: '图鉴', icon: '📖' }
]

const currentItems = computed(() => {
  switch (activeTab.value) {
    case 'posts': return posts.value
    case 'categories': return categories.value
    case 'tags': return tags.value
    case 'projects': return projects.value
    case 'dex': return dexEntries.value
    default: return []
  }
})

async function fetchData() {
  loading.value = true
  try {
    const [p, c, t, pr, d] = await Promise.all([
      api.posts.list(),
      api.categories.list(),
      api.tags.list(),
      api.projects.list(),
      api.dex.list()
    ])
    posts.value = p
    categories.value = c
    tags.value = t
    projects.value = pr
    dexEntries.value = d
  } finally { loading.value = false }
}

function openCreate() {
  editorMode.value = 'create'
  editingItem.value = getEmptyItem()
  showEditor.value = true
}

function openEdit(item: any) {
  editorMode.value = 'edit'
  const data = { ...item }
  if (activeTab.value === 'posts') {
    data.categoryNames = Array.isArray(data.category) ? data.category.join(',') : (data.categoryNames || '')
    data.tagNames = Array.isArray(data.tagNames) ? data.tagNames.join(',') : (data.tagNames || '')
  }
  if (activeTab.value === 'projects') {
    data.tech = Array.isArray(data.tech) ? data.tech.join(',') : (data.tech || '')
  }
  if (activeTab.value === 'dex') {
    data.tags = Array.isArray(data.tags) ? data.tags.join(',') : (data.tags || '')
    data.genre = Array.isArray(data.genre) ? data.genre.join(',') : (data.genre || '')
  }
  editingItem.value = data
  showEditor.value = true
}

function getEmptyItem() {
  switch (activeTab.value) {
    case 'posts': return { title: '', slug: '', description: '', content: '', coverImage: '', published: false, featured: false, categoryNames: [], tagNames: [] }
    case 'categories': return { name: '', slug: '', description: '', icon: '', color: '' }
    case 'tags': return { name: '', slug: '', color: '' }
    case 'projects': return { name: '', description: '', tech: [], link: '', github: '', image: '', status: 'in-progress' }
    case 'dex': return { title: '', slug: '', originalTitle: '', cover: '', category: 'anime', status: 'planned', rating: 0, progress: 0, total: 1, tags: [], comment: '', creator: '', year: new Date().getFullYear(), platform: '', genre: [] }
    default: return {}
  }
}

async function saveItem() {
  loading.value = true
  try {
    const data = { ...editingItem.value }
    
    if (activeTab.value === 'posts') {
      data.categoryNames = typeof data.categoryNames === 'string' ? data.categoryNames.split(',').map((s: string) => s.trim()).filter(Boolean) : data.categoryNames
      data.tagNames = typeof data.tagNames === 'string' ? data.tagNames.split(',').map((s: string) => s.trim()).filter(Boolean) : data.tagNames
    }
    if (activeTab.value === 'projects') {
      data.tech = typeof data.tech === 'string' ? data.tech.split(',').map((s: string) => s.trim()).filter(Boolean) : data.tech
    }
    if (activeTab.value === 'dex') {
      data.tags = typeof data.tags === 'string' ? data.tags.split(',').map((s: string) => s.trim()).filter(Boolean) : data.tags
      data.genre = typeof data.genre === 'string' ? data.genre.split(',').map((s: string) => s.trim()).filter(Boolean) : data.genre
    }
    
    if (editorMode.value === 'create') {
      switch (activeTab.value) {
        case 'posts': await api.posts.create(data); break
        case 'categories': await api.categories.create(data); break
        case 'tags': await api.tags.create(data); break
        case 'projects': await api.projects.create(data); break
        case 'dex': await api.dex.create(data); break
      }
    } else {
      switch (activeTab.value) {
        case 'posts': await api.posts.update(data.id, data); break
        case 'categories': await api.categories.update(data.id, data); break
        case 'tags': await api.tags.update(data.id, data); break
        case 'projects': await api.projects.update(data.id, data); break
        case 'dex': await api.dex.update(data.id, data); break
      }
    }
    showEditor.value = false
    await fetchData()
  } catch (e: any) {
    alert('保存失败: ' + e.message)
  } finally { loading.value = false }
}

async function deleteItem(item: any) {
  if (!confirm('确定要删除吗？')) return
  loading.value = true
  try {
    switch (activeTab.value) {
      case 'posts': await api.posts.delete(item.id); break
      case 'categories': await api.categories.delete(item.id); break
      case 'tags': await api.tags.delete(item.id); break
      case 'projects': await api.projects.delete(item.id); break
      case 'dex': await api.dex.delete(item.id); break
    }
    await fetchData()
  } catch (e: any) {
    alert('删除失败: ' + e.message)
  } finally { loading.value = false }
}

function generateSlug() {
  if (editingItem.value.title && !editingItem.value.slug) {
    editingItem.value.slug = editingItem.value.title.toLowerCase()
      .replace(/[\s]+/g, '-')
      .replace(/[^\w\-\u4e00-\u9fa5]/g, '')
  }
}

const dexCategoryOptions = [
  { value: 'anime', label: '📺 动画' },
  { value: 'movie', label: '🎬 电影' },
  { value: 'tv', label: '📡 电视剧' },
  { value: 'game', label: '🎮 游戏' },
  { value: 'book', label: '📚 书籍' },
  { value: 'music', label: '🎵 音乐' }
]

const dexStatusOptions = [
  { value: 'completed', label: '✅ 已完成' },
  { value: 'watching', label: '👀 在看' },
  { value: 'playing', label: '🎯 在玩' },
  { value: 'reading', label: '📖 在读' },
  { value: 'listening', label: '🎧 在听' },
  { value: 'dropped', label: '💤 搁置' },
  { value: 'planned', label: '📋 计划' }
]

const projectStatusOptions = [
  { value: 'in-progress', label: '进行中' },
  { value: 'completed', label: '已完成' },
  { value: 'archived', label: '已归档' }
]

function handleUpload(event: Event, field: string) {
  const input = event.target as HTMLInputElement
  if (!input.files?.length) return
  const file = input.files[0]
  uploading.value = true
  uploadFile(file)
    .then(res => { editingItem.value[field] = res.url })
    .catch(e => alert('上传失败: ' + e.message))
    .finally(() => { uploading.value = false; input.value = '' })
}

function getImagePreview(field: string): string | null {
  const val = editingItem.value?.[field]
  if (!val) return null
  if (val.startsWith('/uploads/')) return val
  if (val.startsWith('http')) return val
  return null
}

onMounted(fetchData)
</script>

<template>
  <div class="min-h-screen bg-pokemon-gray/30">
    <div class="bg-sky border-b-4 border-pokemon-black px-4 py-3">
      <div class="max-w-7xl mx-auto flex items-center justify-between">
        <div class="flex items-center gap-4">
          <router-link to="/" class="pixel-text text-xs text-pokemon-black hover:text-gold-dark">← 返回前台</router-link>
          <h1 class="pixel-text text-sm text-pokemon-black">⚙️ 管理后台</h1>
        </div>
      </div>
    </div>

    <div class="max-w-7xl mx-auto px-4 py-6">
      <div class="flex gap-2 mb-6 flex-wrap">
        <button
          v-for="tab in tabs"
          :key="tab.id"
          @click="activeTab = tab.id"
          class="pixel-btn text-xs py-2 px-4"
          :class="activeTab === tab.id ? 'bg-gold text-pokemon-black' : 'bg-white text-pokemon-black'"
        >
          {{ tab.icon }} {{ tab.name }}
          <span class="ml-1 text-pokemon-dark-gray">({{ activeTab === tab.id ? currentItems.length : '' }})</span>
        </button>
      </div>

      <div class="flex justify-between items-center mb-4">
        <h2 class="pixel-text text-sm text-pokemon-black">
          {{ tabs.find(t => t.id === activeTab)?.name }}列表
        </h2>
        <button @click="openCreate" class="pixel-btn bg-gold text-pokemon-black text-xs py-2 px-4">
          + 新增
        </button>
      </div>

      <div class="pixel-card overflow-hidden">
        <table class="w-full text-sm">
          <thead>
            <tr class="bg-gold-light border-b-4 border-pokemon-black">
              <th class="pixel-text text-xs text-left p-3">名称</th>
              <th v-if="activeTab === 'posts'" class="pixel-text text-xs text-left p-3 hidden md:table-cell">状态</th>
              <th v-if="activeTab === 'posts'" class="pixel-text text-xs text-left p-3 hidden md:table-cell">浏览</th>
              <th v-if="activeTab === 'dex'" class="pixel-text text-xs text-left p-3 hidden md:table-cell">分类</th>
              <th v-if="activeTab === 'dex'" class="pixel-text text-xs text-left p-3 hidden md:table-cell">评分</th>
              <th v-if="activeTab === 'projects'" class="pixel-text text-xs text-left p-3 hidden md:table-cell">状态</th>
              <th class="pixel-text text-xs text-right p-3">操作</th>
            </tr>
          </thead>
          <tbody>
            <tr v-for="item in currentItems" :key="item.id" class="border-b-2 border-pokemon-black/10 hover:bg-gold-light/30">
              <td class="p-3">
                <div class="font-medium">{{ item.title || item.name }}</div>
                <div v-if="item.description" class="text-xs text-pokemon-dark-gray mt-1 line-clamp-1">{{ item.description }}</div>
              </td>
              <td v-if="activeTab === 'posts'" class="p-3 hidden md:table-cell">
                <span class="text-xs px-2 py-1 border-2 border-pokemon-black rounded" :class="item.published ? 'bg-grass text-pokemon-black' : 'bg-pokemon-gray text-pokemon-dark-gray'">
                  {{ item.published ? '已发布' : '草稿' }}
                </span>
              </td>
              <td v-if="activeTab === 'posts'" class="p-3 hidden md:table-cell text-pokemon-dark-gray">{{ item.views }}</td>
              <td v-if="activeTab === 'dex'" class="p-3 hidden md:table-cell">
                <span class="text-xs">{{ dexCategoryOptions.find(o => o.value === item.category)?.label || item.category }}</span>
              </td>
              <td v-if="activeTab === 'dex'" class="p-3 hidden md:table-cell text-gold-dark">{{ item.rating > 0 ? item.rating + '/10' : '-' }}</td>
              <td v-if="activeTab === 'projects'" class="p-3 hidden md:table-cell">
                <span class="text-xs">{{ projectStatusOptions.find(o => o.value === item.status)?.label || item.status }}</span>
              </td>
              <td class="p-3 text-right">
                <button @click="openEdit(item)" class="text-sky-dark hover:underline text-xs mr-3">编辑</button>
                <button @click="deleteItem(item)" class="text-pokemon-red hover:underline text-xs">删除</button>
              </td>
            </tr>
            <tr v-if="currentItems.length === 0">
              <td colspan="5" class="p-8 text-center text-pokemon-dark-gray">暂无数据</td>
            </tr>
          </tbody>
        </table>
      </div>
    </div>

    <div v-if="showEditor" class="fixed inset-0 bg-black/50 z-50 flex items-center justify-center p-4" @click.self="showEditor = false">
      <div class="pixel-card p-6 w-full max-w-2xl max-h-[90vh] overflow-y-auto">
        <div class="flex justify-between items-center mb-6">
          <h2 class="pixel-text text-sm text-pokemon-black">
            {{ editorMode === 'create' ? '新增' : '编辑' }}{{ tabs.find(t => t.id === activeTab)?.name }}
          </h2>
          <button @click="showEditor = false" class="text-pokemon-dark-gray hover:text-pokemon-black text-xl">&times;</button>
        </div>

        <div class="space-y-4">
          <div v-if="activeTab === 'posts'">
            <div class="grid grid-cols-1 md:grid-cols-2 gap-4">
              <div>
                <label class="pixel-text text-xs text-pokemon-black block mb-1">标题</label>
                <input v-model="editingItem.title" @blur="generateSlug" class="pixel-input w-full text-sm" />
              </div>
              <div>
                <label class="pixel-text text-xs text-pokemon-black block mb-1">Slug</label>
                <input v-model="editingItem.slug" class="pixel-input w-full text-sm" />
              </div>
            </div>
            <div>
              <label class="pixel-text text-xs text-pokemon-black block mb-1">描述</label>
              <input v-model="editingItem.description" class="pixel-input w-full text-sm" />
            </div>
            <div>
              <label class="pixel-text text-xs text-pokemon-black block mb-1">封面图</label>
              <div class="flex gap-2">
                <input v-model="editingItem.coverImage" class="pixel-input flex-1 text-sm" placeholder="图片URL或点击上传" />
                <label class="pixel-btn bg-sky text-white text-xs py-2 px-4 cursor-pointer" :class="{ 'opacity-50': uploading }">
                  {{ uploading ? '上传中...' : '📁 上传' }}
                  <input type="file" accept="image/*" class="hidden" @change="(e: Event) => handleUpload(e, 'coverImage')" />
                </label>
              </div>
              <img v-if="getImagePreview('coverImage')" :src="getImagePreview('coverImage')!" class="mt-2 w-32 h-20 object-cover rounded border-4 border-pokemon-black" />
            </div>
            <div>
              <label class="pixel-text text-xs text-pokemon-black block mb-1">内容 (Markdown)</label>
              <textarea v-model="editingItem.content" class="pixel-input w-full text-sm" rows="12"></textarea>
            </div>
            <div>
              <label class="pixel-text text-xs text-pokemon-black block mb-1">分类 (逗号分隔)</label>
              <input v-model="editingItem.categoryNames" class="pixel-input w-full text-sm" placeholder="技术笔记,项目记录" />
            </div>
            <div>
              <label class="pixel-text text-xs text-pokemon-black block mb-1">标签 (逗号分隔)</label>
              <input v-model="editingItem.tagNames" class="pixel-input w-full text-sm" placeholder="Vue,TypeScript" />
            </div>
            <div class="flex gap-4">
              <label class="flex items-center gap-2 cursor-pointer">
                <input type="checkbox" v-model="editingItem.published" class="w-4 h-4" />
                <span class="text-sm">发布</span>
              </label>
              <label class="flex items-center gap-2 cursor-pointer">
                <input type="checkbox" v-model="editingItem.featured" class="w-4 h-4" />
                <span class="text-sm">精选</span>
              </label>
            </div>
          </div>

          <div v-if="activeTab === 'categories'">
            <div class="grid grid-cols-1 md:grid-cols-2 gap-4">
              <div>
                <label class="pixel-text text-xs text-pokemon-black block mb-1">名称</label>
                <input v-model="editingItem.name" class="pixel-input w-full text-sm" />
              </div>
              <div>
                <label class="pixel-text text-xs text-pokemon-black block mb-1">Slug</label>
                <input v-model="editingItem.slug" class="pixel-input w-full text-sm" />
              </div>
            </div>
            <div>
              <label class="pixel-text text-xs text-pokemon-black block mb-1">描述</label>
              <input v-model="editingItem.description" class="pixel-input w-full text-sm" />
            </div>
            <div class="grid grid-cols-1 md:grid-cols-2 gap-4">
              <div>
                <label class="pixel-text text-xs text-pokemon-black block mb-1">图标</label>
                <input v-model="editingItem.icon" class="pixel-input w-full text-sm" placeholder="💻" />
              </div>
              <div>
                <label class="pixel-text text-xs text-pokemon-black block mb-1">颜色</label>
                <input v-model="editingItem.color" class="pixel-input w-full text-sm" placeholder="#3C5AA6" />
              </div>
            </div>
          </div>

          <div v-if="activeTab === 'tags'">
            <div class="grid grid-cols-1 md:grid-cols-2 gap-4">
              <div>
                <label class="pixel-text text-xs text-pokemon-black block mb-1">名称</label>
                <input v-model="editingItem.name" class="pixel-input w-full text-sm" />
              </div>
              <div>
                <label class="pixel-text text-xs text-pokemon-black block mb-1">Slug</label>
                <input v-model="editingItem.slug" class="pixel-input w-full text-sm" />
              </div>
            </div>
            <div>
              <label class="pixel-text text-xs text-pokemon-black block mb-1">颜色</label>
              <input v-model="editingItem.color" class="pixel-input w-full text-sm" placeholder="#42b883" />
            </div>
          </div>

          <div v-if="activeTab === 'projects'">
            <div>
              <label class="pixel-text text-xs text-pokemon-black block mb-1">名称</label>
              <input v-model="editingItem.name" class="pixel-input w-full text-sm" />
            </div>
            <div>
              <label class="pixel-text text-xs text-pokemon-black block mb-1">描述</label>
              <input v-model="editingItem.description" class="pixel-input w-full text-sm" />
            </div>
            <div>
              <label class="pixel-text text-xs text-pokemon-black block mb-1">技术栈 (逗号分隔)</label>
              <input v-model="editingItem.tech" class="pixel-input w-full text-sm" placeholder="Vue 3,TypeScript" />
            </div>
            <div class="grid grid-cols-1 md:grid-cols-2 gap-4">
              <div>
                <label class="pixel-text text-xs text-pokemon-black block mb-1">链接</label>
                <input v-model="editingItem.link" class="pixel-input w-full text-sm" />
              </div>
              <div>
                <label class="pixel-text text-xs text-pokemon-black block mb-1">GitHub</label>
                <input v-model="editingItem.github" class="pixel-input w-full text-sm" />
              </div>
            </div>
            <div>
              <label class="pixel-text text-xs text-pokemon-black block mb-1">封面图</label>
              <div class="flex gap-2">
                <input v-model="editingItem.image" class="pixel-input flex-1 text-sm" placeholder="图片URL或点击上传" />
                <label class="pixel-btn bg-sky text-white text-xs py-2 px-4 cursor-pointer" :class="{ 'opacity-50': uploading }">
                  {{ uploading ? '上传中...' : '📁 上传' }}
                  <input type="file" accept="image/*" class="hidden" @change="(e: Event) => handleUpload(e, 'image')" />
                </label>
              </div>
              <img v-if="getImagePreview('image')" :src="getImagePreview('image')!" class="mt-2 w-32 h-20 object-cover rounded border-4 border-pokemon-black" />
            </div>
            <div>
              <label class="pixel-text text-xs text-pokemon-black block mb-1">状态</label>
              <select v-model="editingItem.status" class="pixel-input w-full text-sm">
                <option v-for="opt in projectStatusOptions" :key="opt.value" :value="opt.value">{{ opt.label }}</option>
              </select>
            </div>
          </div>

          <div v-if="activeTab === 'dex'">
            <div class="grid grid-cols-1 md:grid-cols-2 gap-4">
              <div>
                <label class="pixel-text text-xs text-pokemon-black block mb-1">标题</label>
                <input v-model="editingItem.title" class="pixel-input w-full text-sm" />
              </div>
              <div>
                <label class="pixel-text text-xs text-pokemon-black block mb-1">Slug</label>
                <input v-model="editingItem.slug" class="pixel-input w-full text-sm" />
              </div>
            </div>
            <div>
              <label class="pixel-text text-xs text-pokemon-black block mb-1">原标题</label>
              <input v-model="editingItem.originalTitle" class="pixel-input w-full text-sm" />
            </div>
            <div>
              <label class="pixel-text text-xs text-pokemon-black block mb-1">封面</label>
              <div class="flex gap-2">
                <input v-model="editingItem.cover" class="pixel-input flex-1 text-sm" placeholder="图片URL或点击上传" />
                <label class="pixel-btn bg-sky text-white text-xs py-2 px-4 cursor-pointer" :class="{ 'opacity-50': uploading }">
                  {{ uploading ? '上传中...' : '📁 上传' }}
                  <input type="file" accept="image/*" class="hidden" @change="(e: Event) => handleUpload(e, 'cover')" />
                </label>
              </div>
              <img v-if="getImagePreview('cover')" :src="getImagePreview('cover')!" class="mt-2 w-24 h-32 object-cover rounded border-4 border-pokemon-black" />
            </div>
            <div class="grid grid-cols-1 md:grid-cols-2 gap-4">
              <div>
                <label class="pixel-text text-xs text-pokemon-black block mb-1">分类</label>
                <select v-model="editingItem.category" class="pixel-input w-full text-sm">
                  <option v-for="opt in dexCategoryOptions" :key="opt.value" :value="opt.value">{{ opt.label }}</option>
                </select>
              </div>
              <div>
                <label class="pixel-text text-xs text-pokemon-black block mb-1">状态</label>
                <select v-model="editingItem.status" class="pixel-input w-full text-sm">
                  <option v-for="opt in dexStatusOptions" :key="opt.value" :value="opt.value">{{ opt.label }}</option>
                </select>
              </div>
            </div>
            <div class="grid grid-cols-2 md:grid-cols-4 gap-4">
              <div>
                <label class="pixel-text text-xs text-pokemon-black block mb-1">评分</label>
                <input v-model.number="editingItem.rating" type="number" min="0" max="10" class="pixel-input w-full text-sm" />
              </div>
              <div>
                <label class="pixel-text text-xs text-pokemon-black block mb-1">进度</label>
                <input v-model.number="editingItem.progress" type="number" class="pixel-input w-full text-sm" />
              </div>
              <div>
                <label class="pixel-text text-xs text-pokemon-black block mb-1">总数</label>
                <input v-model.number="editingItem.total" type="number" class="pixel-input w-full text-sm" />
              </div>
              <div>
                <label class="pixel-text text-xs text-pokemon-black block mb-1">年份</label>
                <input v-model.number="editingItem.year" type="number" class="pixel-input w-full text-sm" />
              </div>
            </div>
            <div class="grid grid-cols-1 md:grid-cols-2 gap-4">
              <div>
                <label class="pixel-text text-xs text-pokemon-black block mb-1">创作者</label>
                <input v-model="editingItem.creator" class="pixel-input w-full text-sm" />
              </div>
              <div>
                <label class="pixel-text text-xs text-pokemon-black block mb-1">平台</label>
                <input v-model="editingItem.platform" class="pixel-input w-full text-sm" />
              </div>
            </div>
            <div>
              <label class="pixel-text text-xs text-pokemon-black block mb-1">标签 (逗号分隔)</label>
              <input v-model="editingItem.tags" class="pixel-input w-full text-sm" />
            </div>
            <div>
              <label class="pixel-text text-xs text-pokemon-black block mb-1">类型 (逗号分隔)</label>
              <input v-model="editingItem.genre" class="pixel-input w-full text-sm" />
            </div>
            <div>
              <label class="pixel-text text-xs text-pokemon-black block mb-1">短评</label>
              <textarea v-model="editingItem.comment" class="pixel-input w-full text-sm" rows="3"></textarea>
            </div>
          </div>
        </div>

        <div class="flex justify-end gap-3 mt-6 pt-4 border-t-4 border-pokemon-black">
          <button @click="showEditor = false" class="pixel-btn bg-pokemon-gray text-pokemon-black text-xs py-2 px-4">取消</button>
          <button @click="saveItem" class="pixel-btn bg-gold text-pokemon-black text-xs py-2 px-4" :disabled="loading">
            {{ loading ? '保存中...' : '保存' }}
          </button>
        </div>
      </div>
    </div>
  </div>
</template>
