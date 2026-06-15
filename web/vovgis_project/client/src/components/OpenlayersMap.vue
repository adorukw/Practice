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
                <el-button type="primary" size="large" @click="RotateMap(45)">↻</el-button><br>
                <el-button type="primary" size="large" @click="RotateMap(-45)">↺</el-button><br>
                <el-button type="primary" size="large" @click="DrawFeature('Point')">●</el-button><br>
                <el-button type="primary" size="large" @click="DrawFeature('LineString')">➤</el-button><br>
                <el-button type="primary" size="large" @click="DrawFeature('Polygon')">▢</el-button><br>
                <el-button type="primary" size="large" @click="DrawFeature('Circle')">◯</el-button><br>
                <el-button type="primary" size="large" @click="DrawFeature('Stop')">✖️</el-button><br>
            </el-container>
            <div ref="featureInfoDom" class="feature-info" style="display: none;"></div>
            <div ref="popupInfoDom" class="ol-popup">
                <a  class="ol-popup-closer" @click="CloserPopup"></a>
                <div ref="popupInfoContentDom">
                    <code>{{ popupHdms }}</code>
                    <p>像元值:{{ cellValue }}</p>
                    <el-button @click="PopupToDestination(popupCoord)">点击进入</el-button>
                </div>
            </div>
            <div class="legend-container" v-show="isTifShow && dataOption.name && legendCanvasDom">
                <h5>{{ legendTitle[dataOption.subType] }}</h5>
                <canvas ref="legendCanvasDom"></canvas>
            </div>
            <div ref="echartsDom" class="echarts-dom" v-show="isTifShow && dataOption.name && echartsDom && false"> </div>
        </el-main>
    </el-container>
</template>

<script lang="js" setup>
import { ref, watch, onMounted } from "vue"
import axios from "axios"
import { Feature, Map, Overlay, View } from "ol"
import { Tile as TileLayer, Vector as VectorLayer, WebGLTile as WebGLTileLayer, Image as ImageLayer } from "ol/layer"
import { XYZ as XYZSource, Vector as VectorSource, GeoTIFF as GeoTIFFSource, TileWMS as TileWMSSource, ImageStatic as ImageStaticSource, XYZ } from "ol/source" // 导入不同类型的源
import { Style, Fill, Stroke, Circle } from "ol/style"
import { GeoJSON } from "ol/format"
import { fromLonLat, toLonLat, transformExtent } from "ol/proj"
import { toRadians } from "ol/math"
import { toStringHDMS } from "ol/coordinate"
import { defaults as defaultControls, ScaleLine } from "ol/control"
import * as GeoTIFF from "geotiff"
import { Draw } from "ol/interaction"
import * as echarts from 'echarts'

//#region  顶部导航栏、数据筛选
const BASE_URL = "http://adolkworld.top:3000"

const isGeojsonShow = ref(false)
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
//#endregion

//#region Openlayers地图
const MY_API_KEY = "14d683ff9d8989c5c7c5ebd386ac6577"
const CITY_BORDER = [118.694400592884, 27.4311978834973, 120.438447620705, 28.9415066498581]
const [left, bottom, right, top] = CITY_BORDER
const CITY_CENTER = [(left + right) / 2, (bottom + top) / 2]

const baseLayers = {
    roadLayer: new TileLayer({
        source: new XYZSource({
            url: 'https://webrd0{1-4}.is.autonavi.com/appmaptile?lang=zh_cn&size=1&scale=1&style=8&x={x}&y={y}&z={z}',

        })
    }),
    satelliteLayer: new TileLayer({
        source: new XYZSource({
            url: 'http://t6.tianditu.com/DataServer?T=img_w&x={x}&y={y}&l={z}&tk=' + MY_API_KEY,
        })
    }),
    textLayer: new TileLayer({
        source: new XYZSource({
            url: 'http://t6.tianditu.com/DataServer?T=cva_w&x={x}&y={y}&l={z}&tk=' + MY_API_KEY,
        })
    }),
}

const view = new View({
    center: fromLonLat(CITY_CENTER),
    zoom: 9,
    projection: 'EPSG:3857',
    rotation: 0,
})

const mapDom = ref()
let map
onMounted(() => {
    map = new Map({
        target: mapDom.value,
        layers: [baseLayers.roadLayer, baseLayers.satelliteLayer, baseLayers.textLayer],
        view: view,
        controls: defaultControls({
            zoom: false,
            rotate: false,
            attribution: false
        }).extend([
            new ScaleLine()
        ])
    })
})

function ResetView() {
    view.animate({
        center: fromLonLat(CITY_CENTER),
        zoom: 9,
        duration: 1500,
    })
}

function SwitchLayer() {
    baseLayers.satelliteLayer.setVisible(!baseLayers.satelliteLayer.getVisible())
}

function LocatingUser() {
    if (navigator.geolocation) {
        navigator.geolocation.getCurrentPosition((position) => {
            const latitude = position.coords.latitude
            const longitude = position.coords.longitude
            const newCenter = fromLonLat([longitude, latitude])
            view.animate({
                center: newCenter,
                zoom: 18,
                duration: 1000
            })
        }, (error) => {
            console.error('获取用户位置错误', error)
        })
    } else {
        console.error('这个浏览器不支持定位')
    }
}

function RotateMap(R) {
    const currentRotation = view.getRotation()
    const newRotation = currentRotation + toRadians(R)
    view.animate({
        rotation: newRotation,
        duration: 500
    })
}

let isDrawing = false

const featureStyles = {
    Point: new Style({
        image: new Circle({
            radius: 10,
            fill: new Fill({
                color: 'red'
            })
        })
    }),
    LineString: new Style({
        image: new Circle({
            radius: 10,
            fill: new Fill({
                color: 'red'
            })
        }),
        stroke: new Stroke({
            color: 'yellow',
            width: 2
        })
    }),
    Polygon: new Style({
        image: new Circle({
            radius: 10,
            fill: new Fill({
                color: 'red'
            })
        }),
        stroke: new Stroke({
            color: 'yellow',
            width: 2
        }),
        fill: new Fill({
            color: 'blue'
        })
    }),
    Circle: new Style({
        image: new Circle({
            radius: 10,
            fill: new Fill({
                color: 'red'
            })
        }),
        stroke: new Stroke({
            color: 'yellow',
            width: 2
        }),
        fill: new Fill({
            color: 'blue'
        })
    })
}
let drawInteraction = null
let drawFeatureLayer = null
function DrawFeature(featureType) {
    if (!map) {
        return
    }

    if (featureType === 'Stop') {
        if (drawInteraction) {
            map.removeInteraction(drawInteraction)
            drawInteraction = null
        }
        isDrawing = false
        return
    }

    if (isDrawing && drawInteraction) {
        map.removeInteraction(drawInteraction)
    }

    if (!drawFeatureLayer) {
        drawFeatureLayer = new VectorLayer({
            source: new VectorSource(),
            style: (feature) => featureStyles[feature.getGeometry()?.getType()] || null
        })
        map.addLayer(drawFeatureLayer)
    }
    drawInteraction = new Draw({
        source: drawFeatureLayer.getSource(),
        type: featureType,
        style: featureStyles[featureType]
    })
    drawInteraction.on('drawend', () => {
        map.removeInteraction(drawInteraction)
        drawInteraction = null
        isDrawing = false
    })
    map.addInteraction(drawInteraction)
    isDrawing = true

}

let geojsonLevel = "lishui_xian"
const geojsonLayer = new VectorLayer({
    source: new VectorSource({
        format: new GeoJSON(),
        url: BASE_URL + `/data/shp/${geojsonLevel}/${geojsonLevel}.geojson`,
    })
})
geojsonLayer.setZIndex(2)
watch(isGeojsonShow, (newValue, oldValue) => {
    if (newValue) {
        map.addLayer(geojsonLayer)
    }
    else {
        map.removeLayer(geojsonLayer)
    }
})

const hightlightStyle = new Style({
    fill: new Fill({
        color: 'rgba(255, 255, 0, 0.6)',
    }),
    stroke: new Stroke({
        color: 'rgba(255, 0, 0, 1)',
        width: 2,
    })
})
let prevHightlightFeature = null
const featureInfoDom = ref()
let featureInfoOverlay = null
function PointerMoveHandler(event) {
    if (isDrawing) {
        return
    }
    let isHovering = false
    map.forEachFeatureAtPixel(event.pixel, (feature) => {
        isHovering = true

        if (!prevHightlightFeature || prevHightlightFeature !== feature) {
            (feature).setStyle(hightlightStyle)

            const properties = feature.getProperties()

            const name = properties.Name || properties.TOWN || '未知名称'
            const code = properties.code || '未知代码'
            const grade = properties.grade || '未知等级'
            const layer = properties.layer || '未知图层'
            const infoString = `Name: ${name}, Layer: ${layer}, Code: ${code}, Grade: ${grade}`

            const extent = view.calculateExtent(map.getSize())
            const [left, bottom, right, top] = extent
            const offset = [(right - left) / 2, (top - bottom) / 2]
            const center = [(left + right) / 2, (top + bottom) / 2]
            if (featureInfoOverlay) {
                featureInfoOverlay.setPosition([center[0] - offset[0], center[1] + offset[1]])
            }

            if (featureInfoDom.value) {
                featureInfoDom.value.innerHTML = infoString
                featureInfoDom.value.style.display = 'block'
            }
        }

        if (prevHightlightFeature && prevHightlightFeature !== feature) {
            prevHightlightFeature.setStyle(null)
        }
        prevHightlightFeature = feature
    })

    if (!isHovering) {
        if (featureInfoDom.value) {
            featureInfoDom.value.style.display = 'none'
        }
        if (prevHightlightFeature) {
            prevHightlightFeature.setStyle(null)
            prevHightlightFeature = null
        }
    }
}
onMounted(() => {
    featureInfoOverlay = new Overlay({
        element: featureInfoDom.value,
        autoPan: false
    })
    map.addOverlay(featureInfoOverlay)
    map.on('pointermove', PointerMoveHandler)
})

function UpdateGeojsonLayer() {
    const zoom = view.getZoom() ?? 0
    if (zoom >= 12) {
        geojsonLevel = 'lishui_jiedao'
        geojsonLayer.setSource(
            new VectorSource({
                format: new GeoJSON(),
                url: BASE_URL + `/data/shp/${geojsonLevel}/${geojsonLevel}.geojson`
            })
        )
    }
    else {
        geojsonLevel = 'lishui_xian'
        geojsonLayer.setSource(
            new VectorSource({
                format: new GeoJSON(),
                url: BASE_URL + `/data/shp/${geojsonLevel}/${geojsonLevel}.geojson`
            })
        )
    }
}
onMounted(() => {
    map.getView().on('change:resolution', () => {
        UpdateGeojsonLayer()
    })
})


let tifLayer = null

const colorMaps = {
    GPP: [],
    NPP: [],
    NEP: [],
    TER: [],
    土壤PH: [],
    土壤有效含水量AWC: [],
    土壤质地TEXTURE: [],
    Pre: [],
    Rad: [],
    Rhu: [],
    Tavg: [],
    Tmax: [],
    Tmin: [],
    森林覆盖率: [],
    森林类型: [],
    森林年龄: [],
    EVI: [],
    FPAR: [],
    LAI: [],
    GDP: [],
    POP: [],
    RP: [],
    TIF: [],
}


const colorStops = {
    GPP: [
        { value: 5, color: [236, 3, 5] },
        { value: 85, color: [251, 177, 0] },
        { value: 155, color: [243, 246, 1] },
        { value: 205, color: [184, 248, 3] },
        { value: 255, color: [9, 243, 5] }
    ],
    NPP: [
        { value: 5, color: [236, 3, 5] },
        { value: 85, color: [251, 177, 0] },
        { value: 155, color: [243, 246, 1] },
        { value: 205, color: [184, 248, 3] },
        { value: 255, color: [9, 243, 5] }
    ],
    NEP: [
        { value: 5, color: [236, 3, 5] },
        { value: 85, color: [251, 177, 0] },
        { value: 155, color: [243, 246, 1] },
        { value: 205, color: [184, 248, 3] },
        { value: 255, color: [9, 243, 5] }
    ],
    TER: [
        { value: 5, color: [236, 3, 5] },
        { value: 85, color: [251, 177, 0] },
        { value: 155, color: [243, 246, 1] },
        { value: 205, color: [184, 248, 3] },
        { value: 255, color: [9, 243, 5] }
    ],
    土壤PH: [
        { value: 5, color: [0, 114, 250] },
        { value: 85, color: [174, 0, 255] },
        { value: 155, color: [84, 255, 2] },
        { value: 205, color: [255, 178, 0] },
        { value: 255, color: [126, 1, 0] }
    ],
    土壤有效含水量AWC: [
        { value: 5, color: [252, 247, 174] },
        { value: 85, color: [209, 194, 136] },
        { value: 155, color: [171, 148, 99] },
        { value: 205, color: [135, 106, 68] },
        { value: 255, color: [99, 66, 39] }
    ],
    土壤质地TEXTURE: [
        { value: 5, color: [104, 18, 11] },
        { value: 85, color: [155, 101, 32] },
        { value: 155, color: [206, 201, 108] },
        { value: 205, color: [98, 131, 50] },
        { value: 255, color: [15, 71, 7] }
    ],
    Pre: [
        { value: 5, color: [32, 204, 16] },
        { value: 85, color: [49, 224, 143] },
        { value: 155, color: [33, 237, 210] },
        { value: 205, color: [50, 148, 240] },
        { value: 255, color: [2, 32, 227] }
    ],
    Rad: [
        { value: 5, color: [2, 32, 227] },
        { value: 85, color: [160, 194, 155] },
        { value: 155, color: [250, 250, 100] },
        { value: 205, color: [250, 141, 52] },
        { value: 255, color: [232, 16, 20] }
    ],
    Rhu: [
        { value: 5, color: [32, 204, 16] },
        { value: 85, color: [49, 224, 143] },
        { value: 155, color: [33, 237, 210] },
        { value: 205, color: [50, 148, 240] },
        { value: 255, color: [2, 32, 227] }
    ],
    Tavg: [
        { value: 5, color: [2, 32, 227] },
        { value: 85, color: [160, 194, 155] },
        { value: 155, color: [250, 250, 100] },
        { value: 205, color: [250, 141, 52] },
        { value: 255, color: [232, 16, 20] }
    ],
    Tmax: [
        { value: 5, color: [2, 32, 227] },
        { value: 85, color: [160, 194, 155] },
        { value: 155, color: [250, 250, 100] },
        { value: 205, color: [250, 141, 52] },
        { value: 255, color: [232, 16, 20] }
    ],
    Tmin: [
        { value: 5, color: [2, 32, 227] },
        { value: 85, color: [160, 194, 155] },
        { value: 155, color: [250, 250, 100] },
        { value: 205, color: [250, 141, 52] },
        { value: 255, color: [232, 16, 20] }
    ],
    森林覆盖率: [
        { value: 5, color: [252, 252, 128] },
        { value: 85, color: [104, 230, 38] },
        { value: 155, color: [61, 193, 82] },
        { value: 205, color: [17, 153, 170] },
        { value: 255, color: [15, 17, 118] }
    ],
    森林类型: [
        { value: 5, color: [252, 252, 128] },
        { value: 85, color: [104, 230, 38] },
        { value: 155, color: [61, 193, 82] },
        { value: 205, color: [17, 153, 170] },
        { value: 255, color: [15, 17, 118] }
    ],
    森林年龄: [
        { value: 5, color: [252, 252, 128] },
        { value: 85, color: [104, 230, 38] },
        { value: 155, color: [61, 193, 82] },
        { value: 205, color: [17, 153, 170] },
        { value: 255, color: [15, 17, 118] }
    ],
    EVI: [
        { value: 5, color: [58, 166, 1] },
        { value: 85, color: [174, 224, 1] },
        { value: 155, color: [250, 255, 3] },
        { value: 205, color: [255, 169, 2] },
        { value: 255, color: [254, 0, 0] }
    ],
    FPAR: [
        { value: 5, color: [58, 166, 1] },
        { value: 85, color: [174, 224, 1] },
        { value: 155, color: [250, 255, 3] },
        { value: 205, color: [255, 169, 2] },
        { value: 255, color: [254, 0, 0] }
    ],
    LAI: [
        { value: 5, color: [58, 166, 1] },
        { value: 85, color: [174, 224, 1] },
        { value: 155, color: [250, 255, 3] },
        { value: 205, color: [255, 169, 2] },
        { value: 255, color: [254, 0, 0] }
    ],
    GDP: [
        { value: 5, color: [255, 230, 197] },
        { value: 85, color: [255, 198, 141] },
        { value: 155, color: [255, 176, 106] },
        { value: 205, color: [251, 157, 75] },
        { value: 255, color: [243, 128, 28] }
    ],
    POP: [
        { value: 5, color: [93, 171, 208] },
        { value: 85, color: [114, 171, 175] },
        { value: 155, color: [237, 241, 108] },
        { value: 205, color: [251, 238, 94] },
        { value: 255, color: [245, 100, 39] }
    ],
    RP: [
        { value: 5, color: [40, 146, 199] },
        { value: 85, color: [160, 194, 155] },
        { value: 155, color: [250, 250, 100] },
        { value: 205, color: [250, 141, 52] },
        { value: 255, color: [232, 16, 20] }
    ],
    TIF: [
        { value: 5, color: [236, 3, 5] },
        { value: 85, color: [251, 177, 0] },
        { value: 155, color: [243, 246, 1] },
        { value: 205, color: [184, 248, 3] },
        { value: 255, color: [9, 243, 5] }
    ]
}
function CreateColorMap(colorStops, colorMap) {
    for (let i = 0; i < colorStops.length - 1; i++) {
        const start = colorStops[i]
        const end = colorStops[i + 1]
        const step = (end.value - start.value) / 5
        for (let j = 0; j < step; j++) {
            const currentValue = start.value + j * 5
            const ratio = j / (step - 1)

            const r = Math.round(start.color[0] + (end.color[0] - start.color[0]) * ratio)
            const g = Math.round(start.color[1] + (end.color[1] - start.color[1]) * ratio)
            const b = Math.round(start.color[2] + (end.color[2] - start.color[2]) * ratio)
            colorMap.push(currentValue, [r, g, b])
        }
    }
}
function CreateColorMaps() {
    for (const key in colorStops) {
        CreateColorMap(colorStops[key], colorMaps[key])
    }
}
CreateColorMaps()

const tifDataMatrix = []
let cellSize = 0
async function ReadTifToMatrix() {
    tifDataMatrix.length = 0
    if (isTifShow.value && dataOption.value.name) {
        const tifUrl = BASE_URL + `/data/${dataOption.value.mainType}/${dataOption.value.subType}/${dataOption.value.name}.tif`
        const res = await axios.get(tifUrl, { responseType: "arraybuffer" })
        const arrayBuffer = res.data

        const tiff = await GeoTIFF.fromArrayBuffer(arrayBuffer)
        const image = await tiff.getImage()
        const tifData = await image.readRasters()
        const matrix = tifData[0]

        const width = image.getWidth()
        const height = image.getHeight()
        cellSize = image.fileDirectory.ModelPixelScale[0]

        const uniqueValues = new Set()


        for (let i = 0; i < height; i++) {
            const row = []
            for (let j = 0; j < width; j++) {
                row.push(matrix[i * width + j])
                uniqueValues.add(matrix[i * width + j])
            }
            tifDataMatrix.push(row)
        }
    }
}
function UpdateTifLayer() {
    if (tifLayer) {
        map.removeLayer(tifLayer)
        tifLayer = null
    }

    if (isTifShow.value && dataOption.value.name) {
        const tifUrl = BASE_URL + `/show/${dataOption.value.mainType}/${dataOption.value.subType}/${dataOption.value.name}.tif`
        axios.get(tifUrl, { responseType: "blob" }).then(
            res => {
                const tifData = res.data
                const tifSource = new GeoTIFFSource({
                    normalize: false,
                    sources: [{
                        blob: tifData
                    }],
                    projection: 'EPSG:4326'
                })
                let tifStyle = null
                if (dataOption.value.subType === "森林类型") {
                    tifStyle = {
                        color: [
                            'match',
                            ['band', 1],
                            0, 'rgb(0,0,0,0)',
                            43, 'rgb(197,0,255)',
                            85, 'rgb(0,0,254)',
                            128, 'rgb(255,255,193)',
                            170, 'rgb(170,205,101)',
                            213, 'rgb(38,115,0)',
                            255, 'rgb(254,0,0)',
                            'rgb(0,0,0,0)'
                        ]
                    }
                }
                else {
                    tifStyle = {
                        color: [
                            'interpolate',
                            ['linear'],
                            ['band', 1],
                            0, 'rgb(0,0,0,0)',
                            ...colorMaps[dataOption.value.subType]
                        ]
                    }
                }
                tifLayer = new WebGLTileLayer({
                    source: tifSource,
                    style: tifStyle
                })
                map.addLayer(tifLayer)
            }
        )

    }
}

const legendCanvasDom = ref()
let allDataInfo = {}

function GetAllDataInfo() {
    const url = `${BASE_URL}/getAllDataInfo`
    axios.get(url).then(res => {
        allDataInfo = res.data
    }).catch(err => {
        console.log('请求错误：', err)
    })
}
GetAllDataInfo()

const legendTitle = ref({
    GPP: "GPP（单位：μmol/m^2/s）",
    NPP: "NPP（单位：μmol/m^2/s）",
    NEP: "NEP（单位：μmol/m^2/s）",
    土壤PH: "土壤PH（单位：pH）",
    土壤有效含水量AWC: "土壤有效含水量AWC（单位：%）",
    土壤质地TEXTURE: "土壤质地TEXTURE（单位：-）",
    Pre: "降水量Pre（单位：mm）",
    Rad: "太阳辐射Rad（单位：W/m^2）",
    Rhu: "相对湿度Rhu（单位：%）",
    Tavg: "平均温度Tavg（单位：°C）",
    Tmax: "最高气温Tmax（单位：°C）",
    Tmin: "最低气温Tmin（单位：°C）",
    森林覆盖率: "森林覆盖率（单位：%）",
    森林类型: "森林类型（单位：-）",
    森林年龄: "森林年龄（单位：-）",
    EVI: "EVI（单位：-）",
    FPAR: "FPAR（单位：-）",
    LAI: "LAI（单位：-）",
    GDP: "GDP（单位：USD）",
    POP: "POP（单位：人/平方公里）",
    RP: "RP（单位：-）",
    TIF: "TIF（单位：-）"
})

function CreateLegend() {
    if (dataOption.value.name) {
        if (dataOption.value.subType === "森林类型") {
            const canvas = legendCanvasDom.value
            canvas.width = 300
            canvas.height = 180

            const colorMap = {
                竹林: 'rgb(197,0,255)',
                水域: 'rgb(0,0,254)',
                耕地: 'rgb(255,255,193)',
                阔叶林: 'rgb(170,205,101)',
                针叶林: 'rgb(38,115,0)',
                竹林: 'rgb(254,0,0)'
            }

            const ctx = canvas.getContext('2d')
            ctx.font = '14px Arial'
            ctx.fillStyle = 'black'
            let index = 0
            for (const key in colorMap) {
                ctx.fillStyle = colorMap[key]
                ctx.fillRect(120, 150 - index * 30, canvas.width - 60, 20)
                ctx.fillText(key, 0, 150 - index * 30 + 10)
                index++
            }
        }
        else {
            const colorMap = colorMaps[dataOption.value.subType]
            const stops = []
            const colors = []

            colorMap.forEach((value, index) => {
                if (index % 2 === 0) {
                    stops.push(value)
                }
                else {
                    colors.push(value)
                }
            })
            if (legendCanvasDom.value) {
                const canvas = legendCanvasDom.value
                canvas.width = 300
                canvas.height = 180
                const ctx = canvas.getContext('2d')

                const gradients = Array.from({ length: 5 }, () =>
                    ctx.createLinearGradient(120, 0, canvas.width - 60, 0)
                )

                let index = 0, gradientIndex = 0, colorIndex = 0

                for (const stop of stops) {
                    if (stop < 55) {
                        gradientIndex = 0
                    }
                    else if (stop >= 55 && stop < 105) {
                        gradientIndex = 1
                    }
                    else if (stop >= 105 && stop < 155) {
                        gradientIndex = 2
                    }
                    else if (stop >= 155 && stop < 205) {
                        gradientIndex = 3
                    }
                    else {
                        gradientIndex = 4
                    }
                    const color = colors[colorIndex]
                    colorIndex++
                    gradients[gradientIndex].addColorStop(index / 10, `rgb(${color[0]}, ${color[1]}, ${color[2]})`)
                    index++
                    index = index % 10
                }

                gradients.forEach((gradient, index) => {
                    ctx.fillStyle = gradient
                    ctx.fillRect(120, 150 - index * 30, canvas.width - 60, 20)
                })

                // const firstKey = Object.keys(allDataInfo[dataOption.value.subType])[0]
                const tifInfo = allDataInfo[dataOption.value.subType][dataOption.value.name]
                const min = tifInfo.min
                const max = tifInfo.max
                const range = max - min
                const steps = [
                    [min, 55 * range / 255 + min],
                    [55 * range / 255 + min, 105 * range / 255 + min],
                    [105 * range / 255 + min, 155 * range / 255 + min],
                    [155 * range / 255 + min, 205 * range / 255 + min],
                    [205 * range / 255 + min, max]
                ]

                steps.forEach((step, index) => {
                    ctx.font = '14px Arial'
                    ctx.fillStyle = 'black'
                    ctx.fillText(Math.round(step[0].toFixed(2)) + '~' + Math.round(step[1].toFixed(2)), 0, 150 - index * 30 + 10)
                })
            }
        }
    }
}

watch([isTifShow, () => dataOption.value.name], () => {
    UpdateTifLayer()
    ReadTifToMatrix()
    CreateLegend()
}, { immediate: true })

const popupInfoDom = ref()
const popupInfoContentDom = ref()
const popupCoord = ref([0, 0])
const popupHdms = ref('')
const cellValue = ref("此处无像元数据")
let popupInfoOverlay = null
function IsWithinArea(pointCoord, areaBorder) {
    const [left, bottom, right, top] = areaBorder;
    const [lng, lat] = pointCoord;
    return lng >= left && lng <= right && lat >= bottom && lat <= top;
}
function CoordToIndex(pointCoord, areaBorder, cellSize) {
    const [left, bottom, right, top] = areaBorder
    const [lng, lat] = pointCoord
    const i = Math.floor((lng - left) / cellSize)
    const j = Math.floor((top - lat) / cellSize)
    return [i, j]
}
function ClickMapHandler(event) {
    if (isDrawing) {
        return
    }
    popupCoord.value = event.coordinate

    const isWithinArea = IsWithinArea(toLonLat(popupCoord.value), CITY_BORDER)
    if (isWithinArea && tifDataMatrix.length > 0) {
        const [i, j] = CoordToIndex(toLonLat(popupCoord.value), CITY_BORDER, cellSize)
        cellValue.value = tifDataMatrix[j][i]
    }
    else {
        cellValue.value = "此处无像元数据"
    }

    popupHdms.value = toStringHDMS(toLonLat(popupCoord.value))

    if (popupInfoOverlay) {
        popupInfoOverlay.setPosition(popupCoord.value)
    }

}
onMounted(() => {
    popupInfoOverlay = new Overlay({
        element: popupInfoDom.value,
        autoPan: {
            animation: { duration: 250 }
        }
    })
    map.addOverlay(popupInfoOverlay)
    map.on('click', ClickMapHandler)
})
function CloserPopup(event) {
    if (popupInfoOverlay) {
        popupInfoOverlay.setPosition(undefined)
    }
    event.currentTarget.blur()
    return false
}
function PopupToDestination(coord) {
    if (coord[0] <= 180 && coord[0] > -180) {
        view.animate({
            center: fromLonLat(coord),
            zoom: 18,
            duration: 1500,
        })
    }
    else {
        view.animate({
            center: coord,
            zoom: 18,
            duration: 1500,
        })
    }
}

const animatingYear = ref()
const isAnimating = ref(false)
let animationInterval

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

const echartsDom = ref(null)
let echartsInstance

onMounted(() => {
    echartsInstance = echarts.init(echartsDom.value)
})

function DrawEcharts() {
    if (dataOption.value.subType) {
        echartsInstance.clear()

        const dataInfo = allDataInfo[dataOption.value.subType]
        const dataName = Object.keys(dataInfo)
        const dataValue = Object.values(dataInfo)
        const meanArray = Array.from(dataValue).map(item => item.mean)
        const minMean = Math.min(...meanArray)
        const maxMean = Math.max(...meanArray)
        const rangeMean = maxMean - minMean
        const yAxisinterval = Math.round(rangeMean / 5)

        let text = dataOption.value.subType + '数据均值变化'

        const option = {
            title: {
                text: text
            },
            tooltip: {
                trigger: 'axis'
            },
            xAxis: {
                name: '数据',
                type: 'category',
                data: dataName
            },
            yAxis: {
                type: 'value',
                name: dataOption.value.subType + '均值',
                interval: yAxisinterval
            },
            series: [{
                name: 'NEP 均值',
                type: 'line',
                data: meanArray,
                smooth: true
            }]
        }

        echartsInstance.setOption(option)
    }
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

.feature-info {
    position: absolute;
    top: 20px;
    left: 100px;
    background-color: rgba(255, 255, 255, 0.9);
    border: 1px solid #ccc;
    border-radius: 5px;
    padding: 10px;
    z-index: 1000;
}

.ol-popup {
    position: absolute;
    background-color: white;
    box-shadow: 0 1px 4px rgba(0, 0, 0, 0.2);
    padding: 15px;
    border-radius: 10px;
    border: 1px solid #cccccc;
    bottom: 12px;
    left: -50px;
    min-width: 280px;
}

.ol-popup:after,
.ol-popup:before {
    top: 100%;
    border: solid transparent;
    content: " ";
    height: 0;
    width: 0;
    position: absolute;
    pointer-events: none;
}

.ol-popup:after {
    border-top-color: white;
    border-width: 10px;
    left: 48px;
    margin-left: -10px;
}

.ol-popup:before {
    border-top-color: hsl(0, 0%, 80%);
    border-width: 11px;
    left: 48px;
    margin-left: -11px;
}

.ol-popup-closer {
    text-decoration: none;
    position: absolute;
    top: 2px;
    right: 8px;
}

.ol-popup-closer:after {
    content: "✖";
}

.legend-container {
    position: absolute;
    top: 250px;
    right: 100px;
    background-color: white;
    padding: 5px;
    box-shadow: 0 2px 10px rgba(0, 0, 0, 0.1);
    z-index: 1000;
}

.echarts-dom {
    width: 600px;
    height: 400px;
    background-color: white;
    position: absolute;
    right: 10px;
    bottom: 15px;
    z-index: 1000;
}
</style>