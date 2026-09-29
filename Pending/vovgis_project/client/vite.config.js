import { fileURLToPath, URL } from 'node:url'

import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'
import vueDevTools from 'vite-plugin-vue-devtools'
import { viteStaticCopy } from 'vite-plugin-static-copy'


import cesium from 'vite-plugin-cesium'

// https://vite.dev/config/
export default defineConfig({
  base: "./",
  publicPath: "./",
  define: {
    CESIUM_BASE_URL: JSON.stringify('./cesium')
  },
  plugins: [
    vue(),
    vueDevTools(),
    cesium(),
    viteStaticCopy({
      targets: [
        {
          // Cesium依赖路径
          src: './node_modules/cesium/Build/Cesium/',
          // 注②
          dest: './'
        }
      ]
    })
  ],
  resolve: {
    alias: {
      '@': fileURLToPath(new URL('./src', import.meta.url))
    },
  },
})
