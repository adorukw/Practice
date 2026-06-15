import { createRouter, createWebHistory } from 'vue-router'

const router = createRouter({
  history: createWebHistory(),
  routes: [
    {
      path: '/',
      redirect: '/session',
    },
    {
      path: '/session',
      name: 'session',
      component: () => import('../views/SessionView.vue'),
    },
    {
      path: '/trajectory',
      name: 'trajectory',
      component: () => import('../views/TrajectoryView.vue'),
    },
  ],
})

export default router
