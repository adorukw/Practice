<script setup lang="ts">
import { onMounted } from 'vue'
import { useBlogStore } from '@/stores/blog'

const store = useBlogStore()

onMounted(() => {
  store.fetchAll()
})
</script>

<template>
  <router-view />
</template>
