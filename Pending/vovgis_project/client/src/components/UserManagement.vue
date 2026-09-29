<template>
    <el-container class="management-container">
        <el-main v-if="role !== 'admin'" class="permission-alert" style="width: 100%">
            <el-alert title="您当前是游客登录，没有操作权限" type="warning" :closable="false" show-icon />
            <el-button type="primary" class="login-btn" @click="GoToLogin">
                <el-icon>
                    <UserFilled />
                </el-icon>
                立即登录
            </el-button>
        </el-main>

        <el-main v-else>
            <el-button type="primary" @click="dialogVisible = true">添加用户</el-button>
            <el-table :data="users" style="width: 100%">
                <el-table-column prop="username" label="用户名" />
                <el-table-column prop="role" label="角色" />
                <el-table-column label="操作">
                    <template #default="{ row }">
                        <el-button v-if="role === 'admin'" type="danger" @click="DeleteUser(row.id)">
                            删除
                        </el-button>
                        <span v-else class="text-gray-400">无操作权限</span>
                    </template>
                </el-table-column>
            </el-table>

            <el-dialog v-model="dialogVisible" title="新增用户">
                <el-form :model="newUser">
                    <el-form-item label="用户名" required>
                        <el-input v-model="newUser.username" />
                    </el-form-item>
                    <el-form-item label="密码" required>
                        <el-input type="password" v-model="newUser.password" />
                    </el-form-item>
                </el-form>
                <template #footer>
                    <el-button @click="dialogVisible = false">取消</el-button>
                    <el-button type="primary" @click="addUser">确认</el-button>
                </template>
            </el-dialog>

        </el-main>
    </el-container>
</template>

<script setup>
import { UserFilled } from '@element-plus/icons-vue'
import { ref, onMounted } from 'vue'
import axios from 'axios'
import { ElMessage } from 'element-plus'
import { useRouter } from 'vue-router'
const router = useRouter()

const BASE_URL = "http://adolkworld.top:3000"

const role = ref(localStorage.getItem('role') || 'guest')
const users = ref([])
const dialogVisible = ref(false)
const newUser = ref({ username: '', password: '' })

const GoToLogin = () => {
    router.push('/login')
}

const GetUsers = async () => {
    try {
        const res = await axios.get(BASE_URL + '/users')
        console.log(res.data)
        users.value = res.data
    } catch (error) {
        ElMessage.error('获取用户列表失败')
    }
}

const addUser = async () => {
    if (!newUser.value.username || !newUser.value.password) {
        return ElMessage.warning('请填写完整信息')
    }

    try {
        await axios.post(BASE_URL + '/users', newUser.value, {
            headers: {
                Authorization: `Bearer ${localStorage.getItem('token')}`
            }
        })
        ElMessage.success('添加成功')
        dialogVisible.value = false
        newUser.value = { username: '', password: '' }
        await GetUsers()
    } catch (error) {
        ElMessage.error('添加用户失败')
    }
}

const DeleteUser = async (id) => {
    try {
        await axios.delete(BASE_URL + `/users/${id}`, {
            headers: {
                Authorization: `Bearer ${localStorage.getItem('token')}`
            }
        })
        ElMessage.success('删除成功')
        await GetUsers()
    } catch (error) {
        ElMessage.error('删除用户失败')
    }
}

onMounted(() => {
    if (role.value === 'admin') {
        GetUsers()
    }
})
</script>

<style scoped>
.permission-alert {
    width: 100%;
    margin-top: 20px;
}

.management-container {
    margin: 0;
    height: 98vh;
    width: 100%;
    margin: 0 auto;
    background-color: #f9f9f9;
    border-radius: 8px;
    box-shadow: 0 2px 12px rgba(0, 0, 0, 0.1);
    height: calc(98vh - 60px);
}
</style>