<template>
    <el-container>
        <Sidebar v-if="!isLoginPage" />
        <router-view></router-view>
    </el-container>
</template>

<script setup lang="js">
import Sidebar from "./components/Sidebar.vue"
import { computed } from 'vue'
import { useRoute } from "vue-router"

const route = useRoute()

const isLoginPage = computed(() => route.path === '/login')
</script>

<style scoped></style>
