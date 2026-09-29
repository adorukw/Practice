<script setup lang="ts">
defineProps<{
  percentage: number
  label?: string
}>()
</script>

<template>
  <div class="w-full">
    <div class="flex justify-between items-center mb-2" v-if="label">
      <span class="pixel-text text-xs text-pokemon-dark-gray">{{ label }}</span>
      <span class="pixel-text text-xs text-pokemon-yellow">{{ percentage }}%</span>
    </div>
    <div class="exp-bar">
      <div
        class="exp-bar-fill"
        :style="{ width: `${percentage}%` }"
      />
    </div>
  </div>
</template>
