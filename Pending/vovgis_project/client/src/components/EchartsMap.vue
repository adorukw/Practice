<template>
    <el-container class="echarts-container">
        <el-header class="echarts-header">
            <h5 style="margin-right:10px;">丽水市森林可持续生产力<br>可视化平台系统</h5>
            <el-container class="echarts-menu">
                <el-radio-group v-model="isEchartsShow" size="small" style="display:grid;padding:0px;margin:0px;">
                    <el-radio :value="true" style="color:white;">显示图表</el-radio>
                    <br>
                    <el-radio :value="false" style="color:white;">隐藏图表</el-radio>
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
        <el-main class="echarts-main">
            <div class="echarts-tip">
                <h1>选择好数据后，查看图表</h1>
                <h2>✅ 点击上方导航栏选择数据集</h2>
                <h2>✅ 在数据表中勾选需要分析的字段</h2>
                <h2>✅ 设置时间范围或分类筛选条件</h2>
            </div>
            <div ref="histogramEchartsDom" class="echarts-dom" v-show="isEchartsShow && dataOption.name"> </div>
            <div ref="heatmapEchartsDom" class="echarts-dom" v-show="isEchartsShow && dataOption.name"> </div>
            <div ref="meanEchartsDom" class="echarts-dom" v-show="isEchartsShow && dataOption.subType"> </div>
            <div ref="maxEchartsDom" class="echarts-dom" v-show="isEchartsShow && dataOption.subType"> </div>
            <div ref="stdEchartsDom" class="echarts-dom" v-show="isEchartsShow && dataOption.subType"> </div>
        </el-main>
    </el-container>
</template>

<script setup>
import { ref, watch, onMounted } from "vue"
import axios from "axios"
import * as echarts from 'echarts'
import * as GeoTIFF from "geotiff"
import { min } from "d3"

//#region  顶部导航栏、数据筛选
const BASE_URL = "http://adolkworld.top:3000"
const isEchartsShow = ref(true)

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

function StartAnimation() {
    if (!isAnimating.value && animatingYear.value) {
        isAnimating.value = true
        isEchartsShow.value = true

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

//#region 绘制图表
const tifDataMatrix = []
async function ReadTifToMatrix() {
    tifDataMatrix.length = 0
    if (isEchartsShow.value && dataOption.value.name) {
        const tifUrl = BASE_URL + `/data/${dataOption.value.mainType}/${dataOption.value.subType}/${dataOption.value.name}.tif`
        const res = await axios.get(tifUrl, { responseType: "arraybuffer" })
        const arrayBuffer = res.data

        const tiff = await GeoTIFF.fromArrayBuffer(arrayBuffer)
        const image = await tiff.getImage()
        const tifData = await image.readRasters()
        const matrix = tifData[0]

        const width = image.getWidth()
        const height = image.getHeight()

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

let allDataInfo = {}
function GetAllDataInfo() {
    const url = `${BASE_URL}/getAllDataInfo`
    axios.get(url).then(res => {
        allDataInfo = res.data
        console.log(allDataInfo)
    }).catch(err => {
        console.log('请求错误：', err)
    })
}
GetAllDataInfo()

const meanEchartsDom = ref(null)
const maxEchartsDom = ref(null)
const stdEchartsDom = ref(null)
const histogramEchartsDom = ref(null)
const heatmapEchartsDom = ref(null)

let meanEchartsInstance = null
let maxEchartsInstance = null
let stdEchartsInstance = null
let histogramEchartsInstance = null
let heatmapEchartsInstance = null

onMounted(() => {
    meanEchartsInstance = echarts.init(meanEchartsDom.value)
    maxEchartsInstance = echarts.init(maxEchartsDom.value)
    stdEchartsInstance = echarts.init(stdEchartsDom.value)
    histogramEchartsInstance = echarts.init(histogramEchartsDom.value)
    heatmapEchartsInstance = echarts.init(heatmapEchartsDom.value)
})

function DrawYearlyEcharts() {
    if (dataOption.value.subType) {
        meanEchartsInstance.clear()
        maxEchartsInstance.clear()
        stdEchartsInstance.clear()

        const dataInfo = allDataInfo[dataOption.value.subType]
        const dataName = Object.keys(dataInfo)
        const dataValue = Object.values(dataInfo)

        const meanArray = Array.from(dataValue).map(item => item.mean)
        const minMean = Math.min(...meanArray)
        const maxMean = Math.max(...meanArray)
        const rangeMean = maxMean - minMean

        const maxArray = Array.from(dataValue).map(item => item.max)
        const minMax = Math.min(...maxArray)
        const maxMax = Math.max(...maxArray)
        const rangeMax = maxMax - minMax

        const stdArray = Array.from(dataValue).map(item => item.std)
        const minStd = Math.min(...stdArray)
        const maxStd = Math.max(...stdArray)
        const rangeStd = maxStd - minStd

        const meanYAxisinterval = Math.round(rangeMean / 5)
        const maxYAxisinterval = Math.round(rangeMax / 5)
        const stdYAxisinterval = Math.round(rangeStd / 5)

        let meanText = dataOption.value.subType + '历年数据均值变化'
        let maxText = dataOption.value.subType + '历年数据最大值变化'
        let stdText = dataOption.value.subType + '历年数据标准差变化'

        const meanOption = {
            title: {
                text: meanText
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
                interval: meanYAxisinterval
            },
            series: [{
                name: '均值',
                type: 'line',
                data: meanArray,
                smooth: true
            }]
        }
        const maxOption = {
            title: {
                text: maxText
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
                name: dataOption.value.subType + '最大值',
                interval: maxYAxisinterval
            },
            series: [{
                name: '最大值',
                type: 'line',
                data: maxArray,
                smooth: true
            }]
        }
        const stdOption = {
            title: {
                text: stdText
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
                name: dataOption.value.subType + '标准差',
                interval: stdYAxisinterval
            },
            series: [{
                name: '标准差',
                type: 'line',
                data: stdArray,
                smooth: true
            }]
        }


        meanEchartsInstance.setOption(meanOption)
        maxEchartsInstance.setOption(maxOption)
        stdEchartsInstance.setOption(stdOption)
    }
}
function DrawHistogramEcharts() {
    if (dataOption.value.name) {
        histogramEchartsInstance.clear()

        const pixelValues = tifDataMatrix.flat().filter(v => v !== -9999)

        const minVal = Math.min(...pixelValues)
        const maxVal = Math.max(...pixelValues)
        const binCount = 10
        const binSize = (maxVal - minVal) / binCount

        const bins = Array.from({ length: binCount }, (_, i) => ({
            start: minVal + i * binSize,
            end: minVal + (i + 1) * binSize,
            count: 0
        }))

        pixelValues.forEach(value => {
            const binIndex = Math.min(
                Math.floor((value - minVal) / binSize),
                binCount - 1
            )
            bins[binIndex].count++;
        })

        const xAxisData = bins.map((bin, i) =>
            `${bin.start.toFixed(2)} - ${bin.end.toFixed(2)}`
        );
        const seriesData = bins.map(bin => bin.count)

        const option = {
            title: {
                text: `${dataOption.value.subType} 数据分布直方图`,
                left: 'center'
            },
            tooltip: {
                trigger: 'axis',
                formatter: params => {
                    const bin = bins[params[0].dataIndex];
                    return `范围: ${bin.start.toFixed(2)}~${bin.end.toFixed(2)}<br/>频数: ${bin.count}`;
                }
            },
            xAxis: {
                type: 'category',
                data: xAxisData,
                name: '数值区间',
                axisLabel: {
                    rotate: 45
                }
            },
            yAxis: {
                type: 'value',
                name: '频数'
            },
            series: [{
                type: 'bar',
                data: seriesData,
                barWidth: '95%',
                itemStyle: {
                    color: '#5470c6'
                }
            }]
        }

        histogramEchartsInstance.setOption(option)
    }
}
function DrawHeatmapEcharts() {
    if (dataOption.value.name) {
        heatmapEchartsInstance.clear()

        const validData = []
        tifDataMatrix.forEach((row, y) => {
            row.forEach((value, x) => {
                if (value !== -9999) {
                    validData.push([x, y, value])
                }
            })
        })

        const values = validData.map(item => item[2]);
        const minVal = Math.min(...values);
        const maxVal = Math.max(...values);

        const option = {
            title: {
                text: `${dataOption.value.subType} 数据热力图`,
                left: 'center'
            },
            tooltip: {
                position: 'top',
                formatter: params => {
                    return `坐标: (${params.data[0]}, ${params.data[1]})<br/>数值: ${params.data[2].toFixed(2)}`
                }
            },
            grid: {
                height: '80%',
                top: '10%'
            },
            xAxis: {
                type: 'category',
                show: false
            },
            yAxis: {
                type: 'category',
                inverse: true,
                show: false
            },
            visualMap: {
                min: minVal,
                max: maxVal,
                calculable: true,
                orient: 'vertical',
                right: '3%',
                top: '20%',
                inRange: {
                    color: ['#313695', '#4575b4', '#74add1', '#abd9e9', '#e0f3f8', '#ffffbf', '#fee090', '#fdae61', '#f46d43', '#d73027', '#a50026']
                }
            },
            series: [{
                name: '热力图',
                type: 'heatmap',
                data: validData,
                itemStyle: {
                    borderWidth: 0.5,
                    borderColor: 'rgba(100,100,100,0.2)'
                },
                progressive: 400,
                emphasis: {
                    itemStyle: {
                        shadowBlur: 10,
                        shadowColor: 'rgba(0, 0, 0, 0.5)'
                    }
                }
            }]
        };

        heatmapEchartsInstance.setOption(option)
    }
}

watch([isEchartsShow, () => dataOption.value.subType, () => dataOption.value.name], () => {
    DrawYearlyEcharts()
    ReadTifToMatrix().then(() => {
        DrawHistogramEcharts()
        DrawHeatmapEcharts()
    })
})


//#endregion

</script>

<style scoped>
.echarts-container {
    margin: 0;
    height: 98vh;
    width: 100%;
}

.echarts-header {
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

.echarts-menu {
    display: flex;
    align-items: center;
    margin-left: auto;
}

.echarts-main {
    margin: 0px;
    padding: 0px;
    width: 100%;
    height: 100%;
    overflow: hidden;
    display: flex;
    flex-direction: row;
    flex-wrap: wrap;
}

.echarts-tip {
    /* 容器基础 */
    position: relative;
    width: 500px;
    height: 432px;
    margin: 0 20px 20px;
    background: linear-gradient(145deg, #ffffff 0%, #f8f9fa 100%);
    border: 1px solid;
    border-image: linear-gradient(135deg, rgba(224, 224, 224, 0.8), rgba(224, 224, 224, 0.2)) 1;
    border-radius: 12px;
    box-shadow:
        0 4px 6px -1px rgba(0, 0, 0, 0.05),
        0 10px 15px -3px rgba(0, 0, 0, 0.08),
        inset 0 -2px 8px rgba(255, 255, 255, 0.3);
    backdrop-filter: blur(4px);
    transition: all 0.3s cubic-bezier(0.4, 0, 0.2, 1);
    overflow: hidden;

    /* 添加磨砂质感 */
    &::before {
        content: '';
        position: absolute;
        top: 0;
        left: 0;
        right: 0;
        bottom: 0;
        background: repeating-linear-gradient(45deg,
                transparent,
                transparent 5px,
                rgba(255, 255, 255, 0.1) 5px,
                rgba(255, 255, 255, 0.1) 10px);
        mix-blend-mode: overlay;
        pointer-events: none;
    }

    /* 标题样式 */
    h1 {
        font-family: 'Segoe UI', system-ui, sans-serif;
        font-size: 1.5rem;
        font-weight: 600;
        color: #2d3436;
        text-align: center;
        margin: 2rem 0 1.5rem;
        letter-spacing: -0.5px;
        position: relative;

        /* 标题装饰线 */
        &::after {
            content: '';
            display: block;
            width: 60px;
            height: 3px;
            background: linear-gradient(90deg, #6c5ce7 0%, #a363d9 100%);
            border-radius: 2px;
            margin: 1rem auto 0;
        }
    }

    /* 悬停动效 */
    &:hover {
        transform: translateY(-2px);
        box-shadow:
            0 6px 12px -2px rgba(0, 0, 0, 0.08),
            0 20px 25px -5px rgba(0, 0, 0, 0.1),
            inset 0 -2px 8px rgba(255, 255, 255, 0.4);
    }

    /* 加载完成后的入场动画 */
    &.loaded {
        animation: tipEntrance 0.6s ease-out;
    }
}

@keyframes tipEntrance {
    0% {
        opacity: 0;
        transform: translateY(20px) scale(0.95);
    }

    100% {
        opacity: 1;
        transform: translateY(0) scale(1);
    }
}

.echarts-dom {
    /* 容器基础 */
    position: relative;
    width: 500px;
    height: 432px;
    margin: 0 20px 20px;
    background: linear-gradient(145deg, #ffffff 0%, #f8f9fa 100%);
    border: 1px solid;
    border-image: linear-gradient(135deg, rgba(224, 224, 224, 0.8), rgba(224, 224, 224, 0.2)) 1;
    border-radius: 12px;
    box-shadow:
        0 4px 6px -1px rgba(0, 0, 0, 0.05),
        0 10px 15px -3px rgba(0, 0, 0, 0.08),
        inset 0 -2px 8px rgba(255, 255, 255, 0.3);
    backdrop-filter: blur(4px);
    transition: all 0.3s cubic-bezier(0.4, 0, 0.2, 1);
    overflow: hidden;

    /* 添加磨砂质感 */
    &::before {
        content: '';
        position: absolute;
        top: 0;
        left: 0;
        right: 0;
        bottom: 0;
        background: repeating-linear-gradient(45deg,
                transparent,
                transparent 5px,
                rgba(255, 255, 255, 0.1) 5px,
                rgba(255, 255, 255, 0.1) 10px);
        mix-blend-mode: overlay;
        pointer-events: none;
    }

    /* 标题样式 */
    h1 {
        font-family: 'Segoe UI', system-ui, sans-serif;
        font-size: 1.5rem;
        font-weight: 600;
        color: #2d3436;
        text-align: center;
        margin: 2rem 0 1.5rem;
        letter-spacing: -0.5px;
        position: relative;

        /* 标题装饰线 */
        &::after {
            content: '';
            display: block;
            width: 60px;
            height: 3px;
            background: linear-gradient(90deg, #6c5ce7 0%, #a363d9 100%);
            border-radius: 2px;
            margin: 1rem auto 0;
        }
    }

    /* 悬停动效 */
    &:hover {
        transform: translateY(-2px);
        box-shadow:
            0 6px 12px -2px rgba(0, 0, 0, 0.08),
            0 20px 25px -5px rgba(0, 0, 0, 0.1),
            inset 0 -2px 8px rgba(255, 255, 255, 0.4);
    }

    /* 加载完成后的入场动画 */
    &.loaded {
        animation: tipEntrance 0.6s ease-out;
    }
}

@keyframes tipEntrance {
    0% {
        opacity: 0;
        transform: translateY(20px) scale(0.95);
    }

    100% {
        opacity: 1;
        transform: translateY(0) scale(1);
    }
}
</style>