<script setup lang="ts">
import { ref, onMounted } from 'vue'
import Layout from '@/components/layout/Layout.vue'

const randomPokemon = ref('🐱')
const pokemons = ['🐱', '🐶', '🦊', '🐼', '🐨', '🦁', '🐯', '🐸', '🐵', '🦄']

onMounted(() => {
  randomPokemon.value = pokemons[Math.floor(Math.random() * pokemons.length)]
})
</script>

<template>
  <Layout>
    <div class="min-h-[70vh] flex items-center justify-center py-12 relative overflow-hidden">
      <div class="absolute inset-0 sky-gradient"></div>
      
      <div class="absolute inset-0 overflow-hidden pointer-events-none">
        <div class="cloud absolute top-4 left-[5%] w-20 h-10 bg-white rounded-full opacity-60"></div>
        <div class="cloud absolute top-8 left-[25%] w-24 h-12 bg-white rounded-full opacity-55"></div>
        <div class="cloud absolute top-2 left-[50%] w-18 h-9 bg-white rounded-full opacity-60"></div>
        <div class="cloud absolute top-10 left-[75%] w-22 h-11 bg-white rounded-full opacity-55"></div>
      </div>
      
      <div class="text-center relative z-10">
        <div class="mb-8">
          <div class="text-8xl mb-4 float-animation">{{ randomPokemon }}</div>
          <div class="dialog-box inline-block">
            <p class="pixel-text text-base text-pokemon-black">
              野生的 404 出现了！
            </p>
          </div>
        </div>

        <h1 class="pixel-text text-4xl text-pokemon-black mb-4 drop-shadow-sm">404</h1>
        <p class="text-pokemon-dark-gray text-lg mb-2">页面未找到</p>
        <p class="text-pokemon-gray text-sm mb-8">
          看起来你遇到了一个野生错误，这个页面可能已经逃跑或不存在。
        </p>

        <div class="flex flex-wrap justify-center gap-4">
          <router-link to="/" class="pixel-btn bg-gold text-pokemon-black hover:bg-gold-dark">
            返回首页
          </router-link>
          <router-link to="/archives" class="pixel-btn bg-sky-dark text-white hover:bg-pokemon-blue">
            浏览文章
          </router-link>
        </div>

        <div class="mt-12 pixel-card p-6 max-w-md mx-auto">
          <h3 class="pixel-text text-base text-pokemon-black mb-4">💡 你可以尝试：</h3>
          <ul class="text-sm text-pokemon-dark-gray text-left space-y-2">
            <li class="flex items-center gap-2">
              <span class="text-sky-dark">▸</span>
              检查网址是否正确
            </li>
            <li class="flex items-center gap-2">
              <span class="text-sky-dark">▸</span>
              返回首页重新开始
            </li>
            <li class="flex items-center gap-2">
              <span class="text-sky-dark">▸</span>
              使用搜索功能查找内容
            </li>
            <li class="flex items-center gap-2">
              <span class="text-sky-dark">▸</span>
              浏览分类或归档页面
            </li>
          </ul>
        </div>
      </div>
    </div>
  </Layout>
</template>

<style scoped>
.cloud {
  filter: blur(2px);
}
</style>
