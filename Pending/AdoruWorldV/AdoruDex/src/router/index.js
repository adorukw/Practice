import { createRouter, createWebHistory } from 'vue-router'

const routes = [
  {
    path: '/',
    name: 'home',
    component: () => import('@/pages/HomePage.vue')
  },
  {
    path: '/anime',
    name: 'anime',
    component: () => import('@/pages/MediaListPage.vue'),
    props: { mediaType: 'anime' }
  },
  {
    path: '/anime/:id',
    name: 'anime-detail',
    component: () => import('@/pages/MediaDetailPage.vue'),
    props: true
  },
  {
    path: '/movie',
    name: 'movie',
    component: () => import('@/pages/MediaListPage.vue'),
    props: { mediaType: 'movie' }
  },
  {
    path: '/movie/:id',
    name: 'movie-detail',
    component: () => import('@/pages/MediaDetailPage.vue'),
    props: true
  },
  {
    path: '/game',
    name: 'game',
    component: () => import('@/pages/MediaListPage.vue'),
    props: { mediaType: 'game' }
  },
  {
    path: '/game/:id',
    name: 'game-detail',
    component: () => import('@/pages/MediaDetailPage.vue'),
    props: true
  },
  {
    path: '/stats',
    name: 'stats',
    component: () => import('@/pages/StatsPage.vue')
  }
]

const router = createRouter({
  history: createWebHistory(),
  routes
})

export default router
