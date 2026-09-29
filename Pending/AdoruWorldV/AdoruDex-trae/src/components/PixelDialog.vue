<script setup>
defineProps({
  title: { type: String, default: '' },
})
</script>

<template>
  <div class="dialog-box">
    <h3 v-if="title" class="font-pixel text-[10px] text-pokemon-dark dark:text-pokemon-yellow mb-3">
      ▶ {{ title }}
    </h3>
    <div class="font-pixel text-[9px] leading-relaxed text-pokemon-text dark:text-pokemon-gray">
      <slot />
    </div>
  </div>
</template>
