import { createRouter, createWebHistory } from 'vue-router'
import HomeView from '../views/HomeView.vue'

const routes = [
  {
    path: '/',
    name: 'Home',
    component: HomeView,
  },
  {
    path: '/category/:slug',
    name: 'Category',
    component: () => import('../views/CategoryView.vue'),
  },
  {
    path: '/work/:id',
    name: 'Detail',
    component: () => import('../views/DetailView.vue'),
  },
]

const router = createRouter({
  history: createWebHistory(),
  routes,
  scrollBehavior() {
    return { top: 0 }
  },
})

export default router
