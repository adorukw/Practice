<script setup>
import { useRouter } from 'vue-router'
import { useCollectionStore } from '../stores/collection'
import PixelButton from '../components/PixelButton.vue'
import PixelCard from '../components/PixelCard.vue'
import PixelDialog from '../components/PixelDialog.vue'

const store = useCollectionStore()
const router = useRouter()
</script>

<template>
  <div class="pixel-bg-pattern min-h-screen">
    <section class="relative overflow-hidden bg-gradient-to-b from-pokemon-blue via-pokemon-blue to-pokemon-bg dark:from-pokemon-darker dark:via-pokemon-darker dark:to-pokemon-darker py-16 md:py-24">
      <div class="absolute inset-0 opacity-10">
        <div class="absolute top-10 left-10 w-32 h-32 border-4 border-white/20 rotate-45"></div>
        <div class="absolute top-20 right-20 w-20 h-20 border-4 border-white/20 rotate-12"></div>
        <div class="absolute bottom-10 left-1/3 w-24 h-24 border-4 border-white/20 -rotate-12"></div>
      </div>

      <div class="max-w-6xl mx-auto px-4 relative z-10">
        <div class="flex flex-col items-center text-center">
          <div class="animate-float mb-8">
            <div class="w-24 h-24 md:w-32 md:h-32 rounded-full bg-pokemon-red border-4 border-pokemon-border relative overflow-hidden pixel-shadow">
              <div class="absolute top-0 left-0 right-0 h-1/2 bg-pokemon-red"></div>
              <div class="absolute bottom-0 left-0 right-0 h-1/2 bg-white dark:bg-pokemon-light"></div>
              <div class="absolute top-1/2 left-0 right-0 h-[4px] bg-pokemon-border -translate-y-1/2"></div>
              <div class="absolute top-1/2 left-1/2 -translate-x-1/2 -translate-y-1/2 w-8 h-8 md:w-10 md:h-10 rounded-full bg-pokemon-light border-4 border-pokemon-border"></div>
              <div class="absolute top-1/2 left-1/2 -translate-x-1/2 -translate-y-1/2 w-4 h-4 md:w-5 md:h-5 rounded-full bg-white border-2 border-pokemon-border"></div>
            </div>
          </div>

          <h1 class="font-pixel text-[16px] md:text-[24px] lg:text-[32px] text-pokemon-yellow pixel-text-shadow mb-6 leading-relaxed">
            AdoruDex
          </h1>
          <p class="font-pixel text-[8px] md:text-[10px] text-white/90 mb-2 max-w-xl leading-relaxed">
            口袋图鉴档案馆
          </p>
          <p class="font-pixel text-[7px] md:text-[8px] text-white/70 mb-8 max-w-lg leading-relaxed">
            记录每一次感动，收藏每一份热爱。<br/>像口袋妖怪图鉴一样，收集你的人生作品。
          </p>

          <div class="flex flex-wrap gap-3 justify-center">
            <PixelButton label="开始探索" variant="yellow" size="lg" to="/" />
            <PixelButton label="查看图鉴" variant="outline" size="lg" to="/" />
          </div>
        </div>
      </div>
    </section>

    <section class="max-w-6xl mx-auto px-4 py-12">
      <div class="flex items-center gap-3 mb-8">
        <div class="w-2 h-8 bg-pokemon-red"></div>
        <h2 class="font-pixel text-[12px] md:text-[14px] text-pokemon-dark dark:text-pokemon-light pixel-text-shadow">
          分类导航
        </h2>
      </div>

      <div class="grid grid-cols-2 md:grid-cols-3 lg:grid-cols-5 gap-4">
        <router-link
          v-for="cat in store.allCategories"
          :key="cat.slug"
          :to="`/category/${cat.slug}`"
          class="pixel-border bg-pokemon-card dark:bg-pokemon-dark p-4 text-center transition-all duration-200 hover:translate-y-[-4px] hover:shadow-[6px_6px_0px_0px_var(--color-pokemon-shadow)] dark:hover:shadow-[6px_6px_0px_0px_#111] group"
        >
          <div class="text-3xl mb-3 group-hover:scale-110 transition-transform">{{ cat.icon }}</div>
          <div class="font-pixel text-[9px] text-pokemon-dark dark:text-pokemon-light mb-1">{{ cat.name }}</div>
          <div class="font-pixel text-[7px] text-pokemon-darkgray">{{ cat.nameEn }}</div>
          <div class="font-pixel text-[6px] text-pokemon-darkgray mt-2">
            {{ store.getWorksByCategory(cat.slug).length }} 件
          </div>
        </router-link>
      </div>
    </section>

    <section class="max-w-6xl mx-auto px-4 py-12">
      <div class="flex items-center justify-between mb-8">
        <div class="flex items-center gap-3">
          <div class="w-2 h-8 bg-pokemon-yellow"></div>
          <h2 class="font-pixel text-[12px] md:text-[14px] text-pokemon-dark dark:text-pokemon-light pixel-text-shadow">
            ★ 精选作品
          </h2>
        </div>
      </div>

      <div class="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-4 gap-6">
        <PixelCard
          v-for="work in store.featuredWorks"
          :key="work.id"
          :work="work"
        />
      </div>
    </section>

    <section class="max-w-6xl mx-auto px-4 py-12">
      <div class="flex items-center justify-between mb-8">
        <div class="flex items-center gap-3">
          <div class="w-2 h-8 bg-pokemon-green"></div>
          <h2 class="font-pixel text-[12px] md:text-[14px] text-pokemon-dark dark:text-pokemon-light pixel-text-shadow">
            最新更新
          </h2>
        </div>
      </div>

      <div class="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-3 gap-6">
        <PixelCard
          v-for="work in store.latestWorks"
          :key="work.id"
          :work="work"
        />
      </div>
    </section>

    <section class="max-w-6xl mx-auto px-4 py-12">
      <PixelDialog title="关于 AdoruDex">
        <p class="mb-3">
          欢迎来到 AdoruDex 口袋图鉴档案馆！
        </p>
        <p class="mb-3">
          这里是一个记录和展示个人收藏的数字空间，灵感来自GBA时代的口袋妖怪图鉴。
        </p>
        <p>
          每一件作品都像一只口袋妖怪，等待着被发现和收录。让我们一起，收集它们全部！
        </p>
      </PixelDialog>
    </section>

    <section class="max-w-6xl mx-auto px-4 py-12">
      <div class="pixel-border bg-pokemon-blue dark:bg-pokemon-dark p-6 md:p-8 text-center">
        <h2 class="font-pixel text-[10px] md:text-[12px] text-pokemon-yellow pixel-text-shadow mb-4">
          准备好开始你的收藏之旅了吗？
        </h2>
        <p class="font-pixel text-[7px] md:text-[8px] text-white/80 mb-6">
          探索不同分类，发现更多精彩作品
        </p>
        <div class="flex flex-wrap gap-3 justify-center">
          <PixelButton
            v-for="cat in store.allCategories.slice(0, 3)"
            :key="cat.slug"
            :label="cat.name"
            :to="`/category/${cat.slug}`"
            variant="yellow"
            size="md"
          />
        </div>
      </div>
    </section>
  </div>
</template>
