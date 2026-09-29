<template>
    <el-container class="map-container">
        <el-header class="map-header">
            <h5 style="margin-right:10px;">丽水市森林可持续生产力<br>可视化平台系统</h5>
            <el-container class="map-menu">
                <el-radio-group v-model="isGeojsonShow" size="small" style="display:grid;padding:0px;margin:0px;">
                    <el-radio :value="true" style="color:white;">显示矢量数据层</el-radio>
                    <br>
                    <el-radio :value="false" style="color:white;">隐藏矢量数据层</el-radio>
                </el-radio-group>

                <el-radio-group v-model="isTifShow" size="small" style="display:grid;padding:0px;margin:0px;">
                    <el-radio :value="true" style="color:white;">显示栅格数据层</el-radio>
                    <br>
                    <el-radio :value="false" style="color:white;">隐藏栅格数据层</el-radio>
                </el-radio-group>

                <el-select v-model="dataOption.mainType" placeholder="选择主数据类型" style="width: 200px;margin-left: 10px;">
                    <el-option v-for="item in mainTypeOptions" :key="item.value" :label="item.label"
                        :value="item.value" />
                </el-select>

                <el-select v-model="dataOption.subType" placeholder="选择子数据类型" style="width: 200px;margin-left: 10px;"
                    :disabled="!dataOption.mainType" :loading="!subTypeOptions.length">
                    <el-option v-for="item in subTypeOptions" :key="item.value" :label="item.label"
                        :value="item.value" />
                </el-select>

                <el-select v-model="dataOption.name" placeholder="选择数据" style="width: 200px;margin-left: 10px;"
                    :disabled="!dataOption.subType" :loading="!nameOptions.length">
                    <el-option v-for="item in nameOptions" :key="item.value" :label="item.label" :value="item.value" />
                </el-select>

                <el-date-picker v-model="animatingYear" type="yearrange" style="margin: 0px" range-separator="To"
                    start-placeholder="起始年份" end-placeholder="结束年份" />

                <el-button type="primary" @click="isAnimating ? StopAnimation() : StartAnimation()" class="header-btn"
                    style="margin-left: 20px">
                    {{ isAnimating ? '停止' : '开始' }}
                </el-button>
            </el-container>
        </el-header>
        <el-main class="map-main">
            <div ref="mapDom" class="map"></div>
            <el-container class="custom-btn-container">
                <el-button type="primary" size="large" @click="ResetView">⤒</el-button><br>
                <el-button type="primary" size="large" @click="SwitchLayer">⇌</el-button><br>
                <el-button type="primary" size="large" @click="LocatingUser">⊙</el-button><br>
            </el-container>
        </el-main>
    </el-container>

</template>

<script setup>
import { ref, watch, onMounted } from "vue"
import axios from "axios"
import * as Cesium from 'cesium'
// import 'cesium/Build/Cesium/Widgets/widgets.css'

//#region  顶部导航栏、数据筛选

const isGeojsonShow = ref(true)
const isTifShow = ref(false)

const dataOption = ref({
    mainType: '',
    subType: '',
    name: ''
})


const mainTypeOptions = [
    { value: '碳循环', label: '碳循环' },
    { value: '土壤', label: '土壤' },
    { value: '气象', label: '气象' },
    { value: '森林结构', label: '森林结构' },
    { value: '冠层数据', label: '冠层数据' },
    { value: '社会经济', label: '社会经济' },
    { value: '打分情况', label: '打分情况' }
]

const subTypeOptionsKey = {
    碳循环: [
        { value: 'GPP', label: 'GPP' },
        { value: 'NEP', label: 'NEP' },
        { value: 'NPP', label: 'NPP' },
        { value: 'TER', label: 'TER' }
    ],
    土壤: [
        { value: '土壤PH', label: '土壤PH' },
        { value: '土壤有效含水量AWC', label: '土壤有效含水量AWC' },
        { value: '土壤质地TEXTURE', label: '土壤质地TEXTURE' }
    ],
    气象: [
        { value: 'Pre', label: 'Pre' },
        { value: 'Rad', label: 'Rad' },
        { value: 'Rhu', label: 'Rhu' },
        { value: 'Tavg', label: 'Tavg' },
        { value: 'Tmax', label: 'Tmax' },
        { value: 'Tmin', label: 'Tmin' }
    ],
    森林结构: [
        { value: '森林覆盖率', label: '森林覆盖率' },
        { value: '森林类型', label: '森林类型' },
        { value: '森林年龄', label: '森林年龄' }
    ],
    冠层数据: [
        { value: 'EVI', label: 'EVI' },
        { value: 'FPAR', label: 'FPAR' },
        { value: 'LAI', label: 'LAI' }
    ],
    社会经济: [
        { value: 'GDP', label: 'GDP' },
        { value: 'POP', label: 'POP' },
        { value: 'RP', label: 'RP' }
    ],
    打分情况: [
        { value: 'TIF', label: 'TIF' }
    ]

}
let subTypeOptions = ref([])



watch(() => dataOption.value.mainType, (newValue, oldValue) => {
    dataOption.value.subType = ''
    if (newValue in subTypeOptionsKey) {
        subTypeOptions.value = subTypeOptionsKey[newValue]
    } else {
        subTypeOptions.value = []
    }
})

const nameOptions = ref([])

function GetNameOptions() {
    if (dataOption.value.subType && dataOption.value.mainType) {
        const url = `${BASE_URL}/list/${dataOption.value.mainType}/${dataOption.value.subType}`
        axios.get(url).then(
            res => {
                nameOptions.value = res.data
            }
        ).catch(
            err => console.log(err)
        )
    }
    else {
        nameOptions.value = []
    }
}

watch(() => dataOption.value.subType, (newValue, oldValue) => {
    dataOption.value.name = ''
    if (newValue) {
        GetNameOptions()
    } else {
        nameOptions.value = []
    }
})

const animatingYear = ref()
const isAnimating = ref(false)
let animationInterval

watch(animatingYear, () => {
    console.log(animatingYear.value)
})

function StartAnimation() {
    if (!isAnimating.value && animatingYear.value) {
        isAnimating.value = true
        isTifShow.value = true

        let currentYear = String(animatingYear.value[0]).split(" ")[3]

        animationInterval = setInterval(() => {
            dataOption.value.name = currentYear

            currentYear = String(parseInt(currentYear) + 1)
            if (currentYear > String(animatingYear.value[1]).split(" ")[3]) {
                currentYear = String(animatingYear.value[0]).split(" ")[3]
            }
        }, 2000)
    }
}
function StopAnimation() {
    if (animationInterval) {
        clearInterval(animationInterval)
        animationInterval = null
        isAnimating.value = false
    }
}
//#endregion

//#region  地图初始化
Cesium.Ion.defaultAccessToken = 'eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJqdGkiOiI3ZGU5YjA1Yy1kYmQ2LTRkOTEtYWU5MS00NWY4NWY1YTg1YzAiLCJpZCI6MjY2MDQwLCJpYXQiOjE3MzU3MDcxMTJ9.lryxTyJyULmUZ3n2_s4Jo1SBejPuZaTr0cwafYAwhAk'
const MY_API_KEY = "14d683ff9d8989c5c7c5ebd386ac6577"
const CITY_BORDER = [118.694400592884, 27.4311978834973, 120.438447620705, 28.9415066498581]
const [left, bottom, right, top] = CITY_BORDER
const CITY_CENTER = [(left + right) / 2, (bottom + top) / 2]
const mapDom = ref(null)
const BASE_URL = "http://adolkworld.top:3000"

let viewer = null

const tiandituImgProvider = new Cesium.UrlTemplateImageryProvider({
    url: `http://t6.tianditu.com/DataServer?T=img_w&x={x}&y={y}&l={z}&tk=${MY_API_KEY}`,
    minimumLevel: 1,
    maximumLevel: 18,
    credit: '天地图',
    tilingScheme: new Cesium.WebMercatorTilingScheme(),
    tileWidth: 256,
    tileHeight: 256
})

const tiandituCvaProvider = new Cesium.UrlTemplateImageryProvider({
    url: `http://t6.tianditu.com/DataServer?T=cva_w&x={x}&y={y}&l={z}&tk=${MY_API_KEY}`,
    minimumLevel: 1,
    maximumLevel: 18,
    credit: '天地图注记',
    tilingScheme: new Cesium.WebMercatorTilingScheme(),
    tileWidth: 256,
    tileHeight: 256
})
onMounted(() => {
    viewer = new Cesium.Viewer(mapDom.value, {
        terrain: Cesium.Terrain.fromWorldTerrain(),
        animation: false,
        baseLayerPicker: true,
        fullscreenButton: false,
        infoBox: false,
    })

    // viewer.imageryLayers.remove(viewer.imageryLayers.get(0))
    // viewer.imageryLayers.addImageryProvider(tiandituImgProvider)
    // viewer.imageryLayers.addImageryProvider(tiandituCvaProvider)
})

function SwitchLayer() {
    viewer.imageryLayers.get(1).show = !viewer.imageryLayers.get(1).show
}

function LocatingUser() {
    if (navigator.geolocation) {
        navigator.geolocation.getCurrentPosition((position) => {
            const latitude = position.coords.latitude
            const longitude = position.coords.longitude
            const newCenter = [longitude, latitude]
            viewer.camera.flyTo({
                destination: Cesium.Cartesian3.fromDegrees(newCenter[0], newCenter[1], 1000),
                orientation: {
                    heading: Cesium.Math.toRadians(0),
                    pitch: Cesium.Math.toRadians(-45),
                    roll: 0
                }
            })
        }, (error) => {
            console.error('获取用户位置错误', error)
        })
    } else {
        console.error('这个浏览器不支持定位')
    }
}

let geojsonLevel = 'lishui_xian'
let geojsonSource = null
async function LoadGeojsonData() {
    // const geojsonUrl = BASE_URL + `/data/shp/${geojsonLevel}/${geojsonLevel}.geojson`
    const geojsonUrl = 'https://geojson.cn/api/china/100000.json'
    geojsonSource = await Cesium.GeoJsonDataSource.load(geojsonUrl, {
        stroke: Cesium.Color.SKYBLUE,
        fill: Cesium.Color.WHITE.withAlpha(0.4),
        strokeWidth: 2,
        clampToGround: false,
    })
    viewer.dataSources.add(geojsonSource)
    const entities = geojsonSource.entities.values


    for (let i = 0; i < entities.length; i++) {
        const entity = entities[i]

        entity.description = `Name:1232`

        // 添加点击事件
        viewer.screenSpaceEventHandler.setInputAction(function (click) {
            const pickedObject = viewer.scene.pick(click.position)
            if (Cesium.defined(pickedObject) && pickedObject.id === entity) {
                alert('Clicked on ' + entity.name)
            }
        }, Cesium.ScreenSpaceEventType.LEFT_CLICK)
    }

    isGeojsonShow.value = true
    viewer.flyTo(geojsonSource)
}
onMounted(() => {
    LoadGeojsonData()
})
watch(isGeojsonShow, (newValue, oldValue) => {
    if (newValue) {
        viewer.dataSources.add(geojsonSource)
    } else {
        viewer.dataSources.remove(geojsonSource)
    }
})

function ResetView() {
    viewer.dataSources.add(geojsonSource)
    isGeojsonShow.value = true
    viewer.flyTo(geojsonSource)
}

//#endregion
</script>

<style scoped>
.map-container {
    margin: 0;
    height: 98vh;
    width: 100%;
}

.map-header {
    background-color: #87CEEB;
    padding: 20px;
    display: flex;
    align-items: center;
    box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);
    font-size: 1.5em;
    color: white;
    border-bottom: 2px solid #dcdcdc;
    height: 90px;
}

.map-menu {
    display: flex;
    align-items: center;
    margin-left: auto;
}

.map-main {
    margin: 0px;
    padding: 0px;
    width: 100%;
    height: 100%;
    overflow: hidden;
}

.map {
    width: 100%;
    height: 100%;
}

.custom-btn-container {
    position: relative;
    flex-direction: column;
    width: 48px;
    bottom: 90%;
    left: 2%;
}

.custom-btn-container button {
    margin-bottom: 0px;
    font-size: x-large;
    height: 48px;
    width: 48px;
    background-color: #87CEEB;
    color: white;
}

.custom-btn-container button:hover {
    background-color: lavender;
    color: gray;
}
</style>