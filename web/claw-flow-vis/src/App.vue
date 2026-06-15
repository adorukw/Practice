<script setup lang="ts">
import NavBar from './components/NavBar.vue'
</script>

<template>
  <div class="w-full h-screen flex flex-col bg-gray-50">
    <NavBar />
    <div class="flex-1 overflow-hidden">
      <router-view />
    </div>
  </div>
</template>
