import { createRouter, createWebHashHistory } from 'vue-router'

import OpenlayersMap from '../components/OpenlayersMap.vue'
import CesiumMap from '../components/CesiumMap.vue'
import EchartsMap from '../components/EchartsMap.vue'
import DataManagement from '../components/DataManagement.vue'
import Help from '../components/Help.vue'
import Login from '../components/Login.vue'
import UserManagement from '../components/UserManagement.vue'
import { path } from 'd3'

const routes = [
  {
    path: '/openlayers',
    name: 'OpenlayersMap',
    component: OpenlayersMap
  },
  {
    path: '/cesium',
    name: 'CesiumMap',
    component: CesiumMap
  },
  {
    path: '/echarts',
    name: 'EchartsMap',
    component: EchartsMap
  },
  {
    path: '/dataManagement',
    name: 'DataManagement',
    component: DataManagement
  },
  {
    path: '/help',
    name: 'Help',
    component: Help
  },
  {
    path: '/login',
    name: 'Login',
    component: Login,
    meta: {
      requiresAuth: false
    }
  },
  {
    path: '/userManagement',
    name: 'UserManagement',
    component: UserManagement,
    meta: {
      requiresAuth: true
    }
  },
  {
    path: '/',
    redirect: '/login'
  }
]

const router = createRouter({
  history: createWebHashHistory(),
  routes
})

router.beforeEach((to, from, next) => {
  const role = localStorage.getItem('role') || 'guest'
  const isLoggedIn = !!localStorage.getItem('token')
  
  if (to.meta.requiresAdmin) {
    if (!isLoggedIn) {
      return next('/login')
    }
    if (role !== 'admin') {
      return next('/openlayers') // 或显示无权限页面
    }
  }
  next()
})

export default router