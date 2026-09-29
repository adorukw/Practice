<template>
    <el-container class="management-container">
        <el-header class="management-header">
            <h5 style="margin-right:10px;">丽水市森林可持续生产力<br>可视化平台系统</h5>
            <el-container class="management-menu">
                <el-select v-model="dataOption.mainType" placeholder="选择主数据类型" clearable
                    style="width: 200px;margin-left: 10px;">
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

                <el-upload class="upload-demo" action="http://localhost:3000/upload" :data="uploadData"
                    :before-upload="beforeUpload" :on-success="handleSuccess" :on-error="handleError"
                    :disabled="!canUpload" :show-file-list="false" style="margin-left: 20px;">
                    <el-button type="success" :disabled="!canUpload" :loading="uploadLoading" v-if="role === 'admin'">
                        上传TIF文件
                    </el-button>
                </el-upload>
            </el-container>
        </el-header>
        <el-main class="management-main">
            <el-table :data="tableData" stripe style="width: 100%" v-loading="loading" height="calc(98vh - 150px)">
                <el-table-column prop="category" label="主分类" />
                <el-table-column prop="subcategory" label="子分类" />
                <el-table-column prop="data_name" label="数据名称" />
                <el-table-column label="统计信息">
                    <el-table-column prop="min" label="最小值" />
                    <el-table-column prop="max" label="最大值" />
                    <el-table-column prop="mean" label="平均值" />
                    <el-table-column prop="std" label="标准差" />
                </el-table-column>
                <el-table-column prop="created_at" label="更新时间">
                    <template #default="{ row }">
                        {{ FormatTime(row.created_at) }}
                    </template>
                </el-table-column>
                <el-table-column label="操作" fixed="right">
                    <template #default="{ row }">
                        <el-button type="primary" size="small" @click="DownloadHandle(row)">
                            下载
                        </el-button>
                        <el-button type="danger" size="small" @click="DeleteHandle(row)"
                            :loading="deleteLoading[row.data_name]" v-if="role === 'admin'">
                            删除
                        </el-button>
                    </template>
                </el-table-column>
            </el-table>
            <el-pagination v-model:current-page="currentPage" v-model:page-size="pageSize"
                :page-sizes="[10, 20, 30, 50]" :total="total" layout="total, sizes, prev, pager, next, jumper"
                @current-change="handleCurrentChange" @size-change="handleSizeChange" style="margin-top: 20px;" />
        </el-main>
    </el-container>
</template>

<script setup>
import { ref, watch, onMounted, computed } from "vue"
import axios from "axios"
import dayjs from "dayjs"
import { ElMessage } from 'element-plus'

const BASE_URL = "http://adolkworld.top:3000"
const role = ref(localStorage.getItem('role') || 'guest')
console.log(role.value)

//#region  顶部导航栏、数据筛选
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

//#region  表格数据展示

const GetFilteredData = async () => {
    try {
        loading.value = true
        const params = {
            mainType: dataOption.value.mainType || undefined,
            subType: dataOption.value.subType || undefined,
            name: dataOption.value.name || undefined
        }

        const res = await axios.get(`${BASE_URL}/getFilteredDataInfo`, { params })
        tableData.value = TransformTableData(res.data)
    } catch (err) {
        console.error(err)
        ElMessage.error('数据加载失败')
    } finally {
        loading.value = false
    }
}

const tableData = ref([])
const loading = ref(false)

const FormatTime = (timestamp) => {
    return dayjs(timestamp).format('YYYY-MM-DD HH:mm')
}

function TransformTableData(apiData) {
    return Object.entries(apiData).flatMap(([dataType, items]) => {
        const [category, subcategory] = dataType.split('/')
        return Object.entries(items).map(([dataName, values]) => ({
            category,
            subcategory,
            data_name: dataName,
            ...values
        }))
    })
}

const DownloadHandle = async (row) => {
    try {
        const filePath = `${row.category}/${row.subcategory}/${row.data_name}.tif`
        const response = await axios.get(`${BASE_URL}/data/${filePath}`, { responseType: 'blob' })

        const url = window.URL.createObjectURL(new Blob([response.data]))
        const link = document.createElement('a')
        link.href = url
        link.setAttribute('download', `${row.data_name}.tif`)
        document.body.appendChild(link)
        link.click()
        document.body.removeChild(link)

        ElMessage.success('开始下载文件')
    } catch (error) {
        console.error('下载失败:', error)
        ElMessage.error('文件下载失败')
    }
}

watch(
    () => ({ ...dataOption.value }),
    () => GetFilteredData(),
    { deep: true, immediate: true }
)

const deleteLoading = ref({})

const DeleteHandle = async (row) => {
    try {
        deleteLoading.value[row.data_name] = true

        await axios.delete(`${BASE_URL}/delete`, {
            params: {
                category: row.category,
                subcategory: row.subcategory,
                data_name: row.data_name
            }
        })

        tableData.value = tableData.value.filter(item =>
            !(item.category === row.category &&
                item.subcategory === row.subcategory &&
                item.data_name === row.data_name)
        )

        ElMessage.success('删除成功')
    } catch (error) {
        console.error('删除失败:', error)
        ElMessage.error('删除失败')
    } finally {
        deleteLoading.value[row.data_name] = false
    }
}

const uploadData = ref({})
const uploadLoading = ref(false)


const canUpload = computed(() => {
    return dataOption.value.mainType && dataOption.value.subType
})

const beforeUpload = (file) => {
    uploadLoading.value = true
    uploadData.value = {
        category: dataOption.value.mainType,
        subcategory: dataOption.value.subType
    }
    return true
}

const handleSuccess = () => {
    ElMessage.success('上传成功')
    GetFilteredData()
    uploadLoading.value = false
}

const handleError = (err) => {
    console.error('上传失败:', err)
    ElMessage.error('文件上传失败')
    uploadLoading.value = false
}

//#endregion

</script>

<style scoped>
.management-container {
    margin: 0;
    height: 98vh;
    width: 100%;
}

.management-header {
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

.management-menu {
    display: flex;
    align-items: center;
    margin-left: auto;
}

.management-main {
    padding: 20px;
}

.el-table {
    margin-top: 20px;
    box-shadow: 0 2px 12px 0 rgba(0, 0, 0, .1);
}

:deep(.el-table__cell) {
    text-align: center;
}
</style>