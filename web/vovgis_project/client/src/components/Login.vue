<template>
    <div class="login-container">
      <div class="login-box">
        <div class="title-container">
          <img src="../assets/tree-icon.webp" class="logo" alt="logo">
          <h1 class="title">丽水市森林可持续生产力<br>可视化平台系统</h1>
        </div>
  
        <el-form :model="form" label-position="top" class="login-form">
          <el-form-item label="用户名" class="form-item">
            <el-input 
              v-model="form.username" 
              placeholder="请输入用户名"
              prefix-icon="User"
              size="large"
            />
          </el-form-item>
  
          <el-form-item label="密码" class="form-item">
            <el-input 
              type="password" 
              v-model="form.password"
              placeholder="请输入密码"
              prefix-icon="Lock"
              size="large"
              show-password
            />
          </el-form-item>

  
          <el-button 
            type="primary" 
            class="login-btn" 
            size="large"
            @click="HandleLogin"
            >立即登录</el-button>
  
          <div class="divider">
            <span class="line"></span>
            <span class="text">或</span>
            <span class="line"></span>
          </div>
  
          <el-button 
            class="guest-btn" 
            size="large"
            @click="HandleGuest"
            >游客访问</el-button>

            <!-- <el-button 
              class="register-btn" 
              size="large"
              @click="HandleRegister"
              >立即注册</el-button> -->
        </el-form>
      </div>
  
      <div class="copyright">
        © 浙江农林大学 地理信息科学 2025届毕业生王士宝 毕业设计 丽水市森林可持续生产力可视化平台系统
      </div>
    </div>
  </template>
  
  <script setup>
  import { ref } from 'vue'
  import { useRouter } from 'vue-router'
  import axios from 'axios'
  import { ElMessage } from 'element-plus'
  
  const form = ref({ username: '', password: '' })
  const router = useRouter()
  const BASE_URL = "http://adolkworld.top:3000"
  
  const HandleLogin = async () => {
    try {
      const res = await axios.post(BASE_URL +'/login', form.value)
      localStorage.setItem('token', res.data.token)
      localStorage.setItem('role', res.data.role)
      router.push('/openlayers')
    } catch (error) {
      console.log(error)
      ElMessage.error('登录失败')
    }
  }
  
  const HandleGuest = () => {
    localStorage.setItem('role', 'guest')
    router.push('/openlayers')
  }

  const HandleRegister = () => {
    router.push('/register')
  }
  </script>

<style scoped>
.login-container {
  width: 100%;
  min-height: 98vh;
  background: url('../assets/forest-bg.png') no-repeat center/cover;
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  position: relative;
}

.login-box {
  width: 480px;
  background: rgba(255, 255, 255, 0.95);
  border-radius: 12px;
  padding: 40px;
  box-shadow: 0 8px 32px rgba(0, 64, 32, 0.1);
  backdrop-filter: blur(8px);
  animation: fadeIn 0.6s ease;
}

.title-container {
  text-align: center;
  margin-bottom: 40px;
}

.logo {
  width: 80px;
  margin-bottom: 16px;
}

.title {
  font-size: 24px;
  color: #2a5936;
  line-height: 1.4;
  margin: 0;
  font-weight: 600;
  letter-spacing: 1.5px;
}

.login-form .form-item {
  margin-bottom: 28px;
}

.login-form .form-item >>> .el-form-item__label {
  font-weight: 500;
  color: #5a725f;
  margin-bottom: 8px;
}

.login-form .form-item >>> .el-input__wrapper {
  border-radius: 8px;
  padding: 2px 15px;
}

.login-btn {
  width: 100%;
  background: #2a5936;
  border: none;
  border-radius: 8px;
  font-size: 16px;
  transition: all 0.3s;
}

.login-btn:hover {
  background: #1d4530;
  transform: translateY(-2px);
}

.divider {
  display: flex;
  align-items: center;
  margin: 24px 0;
}

.divider .line {
  flex: 1;
  height: 1px;
  background: #e8e8e8;
}

.divider .text {
  color: #999;
  padding: 0 15px;
  font-size: 14px;
}

.button-group {
  display: flex;
  flex-direction: column;
  gap: 16px;
}

.secondary-buttons {
  display: flex;
  gap: 12px;
}

.register-btn {
  width: 100%;
  border-color: #c8d6cb;
  color: #5a725f;
  transition: all 0.3s;
}

.register-btn:hover {
  border-color: #2a5936;
  color: #2a5936;
}


.guest-btn {
  width: 100%;
  border-color: #c8d6cb;
  color: #5a725f;
  transition: all 0.3s;
}

.guest-btn:hover {
  border-color: #2a5936;
  color: #2a5936;
}

.copyright {
  position: absolute;
  bottom: 30px;
  color: rgba(255, 255, 255, 0.8);
  font-size: 14px;
}

@keyframes fadeIn {
  from {
    opacity: 0;
    transform: translateY(20px);
  }
  to {
    opacity: 1;
    transform: translateY(0);
  }
}
</style>