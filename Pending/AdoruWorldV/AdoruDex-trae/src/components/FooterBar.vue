<script setup>
</script>

<template>
  <footer class="bg-pokemon-blue dark:bg-pokemon-darker pixel-border border-b-0 border-l-0 border-r-0 mt-12">
    <div class="max-w-6xl mx-auto px-4 py-8">
      <div class="flex flex-col md:flex-row items-center justify-between gap-6">
        <div class="flex items-center gap-3">
          <div class="w-8 h-8 rounded-full bg-pokemon-red border-2 border-pokemon-border relative overflow-hidden">
            <div class="absolute top-1/2 left-0 right-0 h-[2px] bg-pokemon-border"></div>
            <div class="absolute top-1/2 left-1/2 -translate-x-1/2 -translate-y-1/2 w-2 h-2 rounded-full bg-pokemon-light border border-pokemon-border"></div>
          </div>
          <span class="font-pixel text-[10px] text-pokemon-yellow">AdoruDex</span>
        </div>

        <div class="flex items-center gap-4">
          <a href="#" class="w-8 h-8 pixel-border-thin bg-pokemon-light dark:bg-pokemon-dark flex items-center justify-center hover:bg-pokemon-yellow transition-colors text-[10px]">
            🐦
          </a>
          <a href="#" class="w-8 h-8 pixel-border-thin bg-pokemon-light dark:bg-pokemon-dark flex items-center justify-center hover:bg-pokemon-yellow transition-colors text-[10px]">
            📺
          </a>
          <a href="#" class="w-8 h-8 pixel-border-thin bg-pokemon-light dark:bg-pokemon-dark flex items-center justify-center hover:bg-pokemon-yellow transition-colors text-[10px]">
            💻
          </a>
        </div>

        <div class="font-pixel text-[7px] text-pokemon-gray text-center md:text-right leading-relaxed">
          <p>© 2026 AdoruDex Archive</p>
          <p class="mt-1">Gotta Collect 'Em All!</p>
        </div>
      </div>

      <div class="mt-6 pt-4 border-t-2 border-pokemon-border/30 text-center">
        <button
          class="font-pixel text-[8px] text-pokemon-yellow hover:text-white transition-colors cursor-pointer"
          @click="window.scrollTo({ top: 0, behavior: 'smooth' })"
        >
          ▲ 返回顶部
        </button>
      </div>
    </div>
  </footer>
</template>
