<template>
    <el-container class="sidebar-container" :style="{ width: isCollapse ? '64px' : '250px' }">
        <el-menu :collapse="isCollapse" :collapse-transition="false" class="el-sidebar" default-active="1">
            <el-container class="flex-center">
                <h5 class="title">{{ isCollapse ? "" : "VOVGIS" }}</h5>
                <el-icon class="collapse-icon" @click="toggleCollapse">
                    <component :is="isCollapse ? Expand : Fold" />
                </el-icon>
            </el-container>

            <router-link to="/openlayers" class="custom-link">
                <el-menu-item index="1">
                    <el-icon>
                        <Compass />
                    </el-icon>
                    <span>二维地图</span>
                </el-menu-item>
            </router-link>

            <router-link to="/cesium" class="custom-link">
                <el-menu-item index="2">
                    <el-icon>
                        <Football />
                    </el-icon>
                    <span>三维地图</span>
                </el-menu-item>
            </router-link>

            <router-link to="/echarts" class="custom-link">
                <el-menu-item index="3">
                    <el-icon>
                        <DataAnalysis />
                    </el-icon>
                    <span>数据分析</span>
                </el-menu-item>
            </router-link>

            <router-link to="/dataManagement" class="custom-link">
                <el-menu-item index="4">
                    <el-icon>
                        <Document />
                    </el-icon>
                    <span>数据管理</span>
                </el-menu-item>
            </router-link>

            <router-link to="/userManagement" class="custom-link">
                <el-menu-item index="5">
                    <el-icon>
                        <Document />
                    </el-icon>
                    <span>用户管理</span>
                </el-menu-item>
            </router-link>

            <router-link to="/help" class="custom-link">
                <el-menu-item index="6">
                    <el-icon>
                        <QuestionFilled />
                    </el-icon>
                    <span>帮助</span>
                </el-menu-item>
            </router-link>

        </el-menu>
    </el-container>
</template>

<script lang="js" setup>
import { ref } from "vue"
import {
    Compass,
    Football,
    DataAnalysis,
    Document,
    QuestionFilled,
    Expand,
    Fold
} from "@element-plus/icons-vue"

const isCollapse = ref(false)

const toggleCollapse = () => {
    isCollapse.value = !isCollapse.value
}
</script>

<style scoped>
.sidebar-container {
    transition: width 0.7s ease;
    height: 98vh;
    background-color: white;
    border: 2px solid gainsboro;
    box-shadow:
        5px 0 15px -5px rgba(0, 0, 0, 0.1),
        inset -2px 0 2px -1px rgba(255, 255, 255, 0.8),
        inset 2px 0 2px -1px rgba(0, 0, 0, 0.05);
    position: relative;
    z-index: 2;
}

.sidebar-container::after {
    content: "";
    position: absolute;
    right: -2px;
    top: 0;
    bottom: 0;
    width: 2px;
    background: linear-gradient(to right,
            rgba(255, 255, 255, 0.8) 0%,
            rgba(220, 220, 220, 0.6) 50%,
            rgba(255, 255, 255, 0.8) 100%);
}

.el-sidebar {
    border-right: none !important;
    height: 100%;
    width: 100%;
}

.flex-center {
    display: flex;
    justify-content: space-between;
    align-items: center;
    padding: 0 20px;
    height: 80px;
    border-bottom: 1px solid lavender;
}

.title {
    color: darkslategray;
    margin: 0;
    font-size: 24px;
    white-space: nowrap;
    font-weight: bold;
}

.collapse-icon {
    color: #87CEEB;
    cursor: pointer;
    font-size: 30px;
}

.el-menu-item {
    color: slategray !important;
    height: 60px !important;
    line-height: 60px !important;
    border-bottom: 1px solid beige;
}

.el-menu-item:hover {
    background-color: lavender !important;
}

.el-menu-item.is-active {
    background-color: #87CEEB !important;
    color: white !important;
}

.el-icon {
    font-size: 25px;
    margin-right: 5px;
    color: #87CEEB;
}

.custom-link {
    text-decoration: none;
    outline: none;
    color: inherit !important;
    --router-link-active-exact: none;
}

.el-menu--collapse .el-menu-item span {
    display: none !important;
}

.el-menu--collapse .el-menu-item .el-icon {
    margin-right: 0 !important;
}
</style>