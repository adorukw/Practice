const API_BASE = '/api'

async function request<T>(url: string, options?: RequestInit): Promise<T> {
  const res = await fetch(`${API_BASE}${url}`, {
    headers: { 'Content-Type': 'application/json' },
    ...options
  })
  if (!res.ok) {
    const error = await res.json().catch(() => ({ error: res.statusText }))
    throw new Error(error.error || 'Request failed')
  }
  return res.json()
}

export async function uploadFile(file: File): Promise<{ url: string; filename: string }> {
  const formData = new FormData()
  formData.append('file', file)
  const res = await fetch(`${API_BASE}/upload`, { method: 'POST', body: formData })
  if (!res.ok) {
    const error = await res.json().catch(() => ({ error: res.statusText }))
    throw new Error(error.error || 'Upload failed')
  }
  return res.json()
}

export const api = {
  posts: {
    list: (params?: Record<string, string>) => {
      const query = params ? '?' + new URLSearchParams(params).toString() : ''
      return request<any[]>(`/posts${query}`)
    },
    get: (slug: string) => request<any>(`/posts/${slug}`),
    create: (data: any) => request<any>('/posts', { method: 'POST', body: JSON.stringify(data) }),
    update: (id: string, data: any) => request<any>(`/posts/${id}`, { method: 'PUT', body: JSON.stringify(data) }),
    delete: (id: string) => request<{ success: boolean }>(`/posts/${id}`, { method: 'DELETE' })
  },
  categories: {
    list: () => request<any[]>('/categories'),
    get: (slug: string) => request<any>(`/categories/${slug}`),
    create: (data: any) => request<any>('/categories', { method: 'POST', body: JSON.stringify(data) }),
    update: (id: string, data: any) => request<any>(`/categories/${id}`, { method: 'PUT', body: JSON.stringify(data) }),
    delete: (id: string) => request<{ success: boolean }>(`/categories/${id}`, { method: 'DELETE' })
  },
  tags: {
    list: () => request<any[]>('/tags'),
    create: (data: any) => request<any>('/tags', { method: 'POST', body: JSON.stringify(data) }),
    update: (id: string, data: any) => request<any>(`/tags/${id}`, { method: 'PUT', body: JSON.stringify(data) }),
    delete: (id: string) => request<{ success: boolean }>(`/tags/${id}`, { method: 'DELETE' })
  },
  projects: {
    list: () => request<any[]>('/projects'),
    get: (id: string) => request<any>(`/projects/${id}`),
    create: (data: any) => request<any>('/projects', { method: 'POST', body: JSON.stringify(data) }),
    update: (id: string, data: any) => request<any>(`/projects/${id}`, { method: 'PUT', body: JSON.stringify(data) }),
    delete: (id: string) => request<{ success: boolean }>(`/projects/${id}`, { method: 'DELETE' })
  },
  dex: {
    list: (params?: Record<string, string>) => {
      const query = params ? '?' + new URLSearchParams(params).toString() : ''
      return request<any[]>(`/dex${query}`)
    },
    get: (slug: string) => request<any>(`/dex/${slug}`),
    create: (data: any) => request<any>('/dex', { method: 'POST', body: JSON.stringify(data) }),
    update: (id: string, data: any) => request<any>(`/dex/${id}`, { method: 'PUT', body: JSON.stringify(data) }),
    delete: (id: string) => request<{ success: boolean }>(`/dex/${id}`, { method: 'DELETE' })
  },
  site: {
    stats: () => request<any>('/site/stats'),
    author: () => request<any>('/site/author'),
    updateAuthor: (data: any) => request<any>('/site/author', { method: 'PUT', body: JSON.stringify(data) })
  }
}
