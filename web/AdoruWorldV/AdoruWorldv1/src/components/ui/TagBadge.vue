<script setup lang="ts">
import { computed } from 'vue'

const props = defineProps<{
  name: string
  color?: string
  count?: number
  active?: boolean
}>()

const tagStyle = computed(() => ({
  backgroundColor: props.active ? props.color : `${props.color}20`,
  borderColor: props.color,
  color: props.active ? '#fff' : props.color
}))
</script>

<template>
  <span
    class="tag cursor-pointer"
    :style="tagStyle"
  >
    #{{ name }}
    <span v-if="count" class="ml-1 opacity-70">({{ count }})</span>
  </span>
</template>
