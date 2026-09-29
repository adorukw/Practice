<script setup>
defineProps({
  title: { type: String, default: '' },
  noPadding: { type: Boolean, default: false },
  variant: { type: String, default: 'default' }
})
</script>

<template>
  <div
    :class="[
      'relative pixel-shadow-sm',
      variant === 'dex'
        ? 'bg-dex-bg border-4 border-dex-dark'
        : 'bg-dex-window border-4 border-dex-dark'
    ]"
  >
    <div
      v-if="title"
      class="bg-dex-dark text-dex-lighter px-4 py-2 flex items-center gap-2 pixel-text"
    >
      <span class="w-3 h-3 bg-dex-amber animate-blink inline-block shrink-0" />
      <span class="font-bold text-sm tracking-wider font-pokemon">{{ title }}</span>
    </div>
    <div :class="[noPadding ? '' : 'p-4']">
      <slot />
    </div>
  </div>
</template>
