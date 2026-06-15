<script setup lang="ts">
import { useBlogStore } from '@/stores/blog'

const store = useBlogStore()
const currentYear = new Date().getFullYear()
</script>

<template>
  <footer class="border-t-4 border-pokemon-black mt-auto overflow-hidden relative">
    <div class="absolute inset-0 gold-pattern"></div>
    
    <div class="grass-container absolute bottom-0 left-0 right-0 h-12 pointer-events-none overflow-hidden">
      <div class="grass-row">
        <div v-for="i in 50" :key="i" class="grass-blade" :class="`grass-${(i % 3) + 1}`"></div>
      </div>
    </div>
    
    <div class="max-w-6xl mx-auto px-4 py-8 relative z-10">
      <div class="grid grid-cols-1 md:grid-cols-3 gap-8">
        <div class="text-center md:text-left">
          <div class="flex items-center justify-center md:justify-start gap-3 mb-4">
            <div class="w-10 h-10 bg-sky rounded-full border-4 border-pokemon-black flex items-center justify-center shadow-md relative overflow-hidden">
              <div class="absolute inset-0 bg-gradient-to-b from-white/40 to-transparent"></div>
              <span class="text-white font-bold text-lg relative z-10">A</span>
            </div>
            <span class="pixel-text text-pokemon-black text-sm drop-shadow-sm">AdoruWorld</span>
          </div>
          <p class="text-pokemon-dark-gray text-sm">
            口袋妖怪风格的像素艺术个人博客
          </p>
        </div>

        <div class="text-center">
          <h3 class="pixel-text text-pokemon-black text-xs mb-4">网站统计</h3>
          <div class="grid grid-cols-2 gap-4 text-sm">
            <div class="bg-white/80 rounded p-3 border-4 border-pokemon-black shadow-md relative overflow-hidden">
              <div class="absolute inset-0 bg-gradient-to-b from-white/50 to-transparent pointer-events-none"></div>
              <div class="pixel-text text-sky-dark text-lg relative z-10">{{ store.stats.totalPosts }}</div>
              <div class="text-pokemon-dark-gray text-xs relative z-10">文章数</div>
            </div>
            <div class="bg-white/80 rounded p-3 border-4 border-pokemon-black shadow-md relative overflow-hidden">
              <div class="absolute inset-0 bg-gradient-to-b from-white/50 to-transparent pointer-events-none"></div>
              <div class="pixel-text text-sky-dark text-lg relative z-10">{{ store.stats.totalWords.toLocaleString() }}</div>
              <div class="text-pokemon-dark-gray text-xs relative z-10">总字数</div>
            </div>
            <div class="bg-white/80 rounded p-3 border-4 border-pokemon-black shadow-md relative overflow-hidden">
              <div class="absolute inset-0 bg-gradient-to-b from-white/50 to-transparent pointer-events-none"></div>
              <div class="pixel-text text-sky-dark text-lg relative z-10">{{ store.stats.totalViews.toLocaleString() }}</div>
              <div class="text-pokemon-dark-gray text-xs relative z-10">阅读量</div>
            </div>
            <div class="bg-white/80 rounded p-3 border-4 border-pokemon-black shadow-md relative overflow-hidden">
              <div class="absolute inset-0 bg-gradient-to-b from-white/50 to-transparent pointer-events-none"></div>
              <div class="pixel-text text-sky-dark text-lg relative z-10">{{ store.stats.runningDays }}</div>
              <div class="text-pokemon-dark-gray text-xs relative z-10">运行天数</div>
            </div>
          </div>
        </div>

        <div class="text-center md:text-right">
          <h3 class="pixel-text text-pokemon-black text-xs mb-4">社交媒体</h3>
          <div class="flex justify-center md:justify-end gap-3">
            <a
              :href="store.authorInfo.social.github"
              target="_blank"
              class="w-10 h-10 bg-white rounded border-4 border-pokemon-black flex items-center justify-center
                     hover:bg-sky hover:text-white hover:-translate-y-1 transition-all duration-100 shadow-md
                     relative overflow-hidden"
              title="GitHub"
            >
              <div class="absolute inset-0 bg-gradient-to-b from-white/50 to-transparent pointer-events-none"></div>
              <svg class="w-5 h-5 relative z-10" fill="currentColor" viewBox="0 0 24 24">
                <path d="M12 0c-6.626 0-12 5.373-12 12 0 5.302 3.438 9.8 8.207 11.387.599.111.793-.261.793-.577v-2.234c-3.338.726-4.033-1.416-4.033-1.416-.546-1.387-1.333-1.756-1.333-1.756-1.089-.745.083-.729.083-.729 1.205.084 1.839 1.237 1.839 1.237 1.07 1.834 2.807 1.304 3.492.997.107-.775.418-1.305.762-1.604-2.665-.305-5.467-1.334-5.467-5.931 0-1.311.469-2.381 1.236-3.221-.124-.303-.535-1.524.117-3.176 0 0 1.008-.322 3.301 1.23.957-.266 1.983-.399 3.003-.404 1.02.005 2.047.138 3.006.404 2.291-1.552 3.297-1.23 3.297-1.23.653 1.653.242 2.874.118 3.176.77.84 1.235 1.911 1.235 3.221 0 4.609-2.807 5.624-5.479 5.921.43.372.823 1.102.823 2.222v3.293c0 .319.192.694.801.576 4.765-1.589 8.199-6.086 8.199-11.386 0-6.627-5.373-12-12-12z"/>
              </svg>
            </a>
            <a
              :href="store.authorInfo.social.twitter"
              target="_blank"
              class="w-10 h-10 bg-white rounded border-4 border-pokemon-black flex items-center justify-center
                     hover:bg-sky hover:text-white hover:-translate-y-1 transition-all duration-100 shadow-md
                     relative overflow-hidden"
              title="Twitter"
            >
              <div class="absolute inset-0 bg-gradient-to-b from-white/50 to-transparent pointer-events-none"></div>
              <svg class="w-5 h-5 relative z-10" fill="currentColor" viewBox="0 0 24 24">
                <path d="M18.244 2.25h3.308l-7.227 8.26 8.502 11.24H16.17l-5.214-6.817L4.99 21.75H1.68l7.73-8.835L1.254 2.25H8.08l4.713 6.231zm-1.161 17.52h1.833L7.084 4.126H5.117z"/>
              </svg>
            </a>
            <a
              :href="`mailto:${store.authorInfo.social.email}`"
              class="w-10 h-10 bg-white rounded border-4 border-pokemon-black flex items-center justify-center
                     hover:bg-sky hover:text-white hover:-translate-y-1 transition-all duration-100 shadow-md
                     relative overflow-hidden"
              title="Email"
            >
              <div class="absolute inset-0 bg-gradient-to-b from-white/50 to-transparent pointer-events-none"></div>
              <svg class="w-5 h-5 relative z-10" fill="none" stroke="currentColor" viewBox="0 0 24 24">
                <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" 
                      d="M3 8l7.89 5.26a2 2 0 002.22 0L21 8M5 19h14a2 2 0 002-2V7a2 2 0 00-2-2H5a2 2 0 00-2 2v10a2 2 0 002 2z" />
              </svg>
            </a>
          </div>
        </div>
      </div>

      <div class="border-t-2 border-pokemon-black/30 mt-8 pt-6 text-center relative z-10">
        <p class="text-pokemon-dark-gray text-xs pixel-text">
          © {{ currentYear }} AdoruWorld. All rights reserved.
        </p>
        <p class="text-pokemon-black/60 text-xs mt-2">
          Made with ❤️ and pixel art magic
        </p>
      </div>
    </div>
  </footer>
</template>

<style scoped>
.grass-container {
  z-index: 5;
}

.grass-row {
  display: flex;
  justify-content: space-around;
  align-items: flex-end;
  height: 100%;
}

.grass-blade {
  width: 4px;
  background: linear-gradient(to top, #70A858, #90C878, #A8E090);
  border-radius: 2px 2px 0 0;
  transform-origin: bottom center;
  box-shadow: 0 0 2px rgba(112, 168, 88, 0.5);
}

.grass-1 {
  height: 20px;
  animation: grass-sway-1 2s ease-in-out infinite;
}

.grass-2 {
  height: 28px;
  animation: grass-sway-2 2.3s ease-in-out infinite;
}

.grass-3 {
  height: 24px;
  animation: grass-sway-3 1.8s ease-in-out infinite;
}

@keyframes grass-sway-1 {
  0%, 100% { transform: rotate(-4deg); }
  50% { transform: rotate(4deg); }
}

@keyframes grass-sway-2 {
  0%, 100% { transform: rotate(3deg); }
  50% { transform: rotate(-3deg); }
}

@keyframes grass-sway-3 {
  0%, 100% { transform: rotate(-2deg); }
  50% { transform: rotate(2deg); }
}

@media (max-width: 768px) {
  .grass-blade {
    width: 3px;
  }
  
  .grass-1 { height: 16px; }
  .grass-2 { height: 22px; }
  .grass-3 { height: 18px; }
}
</style>
