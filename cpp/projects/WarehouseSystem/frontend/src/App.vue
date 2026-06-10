<template>
  <div v-if="!user.id" class="login-container">
    <el-card class="login-card">
      <template #header>
        <div class="login-header">
          <h2>📦 仓储系统登录</h2>
        </div>
      </template>
      <el-form :model="loginForm">
        <el-form-item>
          <el-input v-model="loginForm.username" placeholder="账号 (admin)" :prefix-icon="User" size="large" />
        </el-form-item>
        <el-form-item>
          <el-input v-model="loginForm.password" type="password" placeholder="密码 (123456)" :prefix-icon="Lock" size="large" show-password />
        </el-form-item>
        <el-button type="primary" style="width: 100%; margin-top: 10px;" size="large" @click="handleLogin" :loading="loading">
          立即登录
        </el-button>
      </el-form>
    </el-card>
  </div>

  <el-container v-else class="layout-container">
    <el-aside width="220px" class="aside-menu">
      <div class="logo">
        <el-icon><Box /></el-icon> <span>WMS 仓储管理</span>
      </div>
      <el-menu
        active-text-color="#ffd04b"
        background-color="#304156"
        text-color="#fff"
        :default-active="activeMenu"
        class="el-menu-vertical"
        @select="(index) => activeMenu = index"
      >
        <el-menu-item index="dashboard">
          <el-icon><Odometer /></el-icon>
          <span>工作台 (Dashboard)</span>
        </el-menu-item>
        <el-menu-item index="packages">
          <el-icon><Van /></el-icon>
          <span>快递包裹管理</span>
        </el-menu-item>
        <el-menu-item index="users">
          <el-icon><User /></el-icon>
          <span>员工账号管理</span>
        </el-menu-item>
        <el-menu-item index="settings">
          <el-icon><Setting /></el-icon>
          <span>系统设置</span>
        </el-menu-item>
      </el-menu>
    </el-aside>

    <el-container>
      <el-header class="header">
        <div class="header-left">
          <el-icon size="20" style="margin-right: 10px; cursor: pointer"><Expand /></el-icon>
          <el-breadcrumb separator="/">
            <el-breadcrumb-item>首页</el-breadcrumb-item>
            <el-breadcrumb-item>{{ menuNames[activeMenu] }}</el-breadcrumb-item>
          </el-breadcrumb>
        </div>
        <div class="header-right">
          <el-avatar :size="32" :src="user.avatar" />
          <span class="username">{{ user.nickname }}</span>
          <el-divider direction="vertical" />
          <el-button link type="danger" @click="logout">退出</el-button>
        </div>
      </el-header>

      <el-main class="main-content">
        
        <div v-if="activeMenu === 'packages'">
          <div class="page-header">
            <h3>📦 包裹列表</h3>
            <div>
               <el-input v-model="searchText" placeholder="搜索单号..." style="width: 200px; margin-right: 10px;" :prefix-icon="Search" />
               <el-button type="primary" @click="openDialog()">+ 新增包裹</el-button>
               <el-button type="success" @click="fetchData">刷新</el-button>
            </div>
          </div>

          <el-table :data="filteredData" border stripe style="width: 100%; border-radius: 8px; overflow: hidden; box-shadow: 0 2px 12px 0 rgba(0,0,0,0.05);">
            <el-table-column prop="Id" label="ID" width="60" />
            <el-table-column prop="TrackingNumber" label="快递单号" width="150" />
            <el-table-column prop="Sender" label="发件人" />
            <el-table-column prop="Receiver" label="收件人" />
            <el-table-column prop="Location" label="当前位置" />
            <el-table-column prop="Status" label="状态">
              <template #default="scope">
                <el-tag :type="getStatusType(scope.row.Status)" effect="dark">{{ scope.row.Status }}</el-tag>
              </template>
            </el-table-column>
            <el-table-column label="操作" width="180">
              <template #default="scope">
                <el-button size="small" type="primary" plain @click="handleEdit(scope.row)">编辑</el-button>
                <el-button size="small" type="danger" plain @click="handleDelete(scope.row.Id)">删除</el-button>
              </template>
            </el-table-column>
          </el-table>
        </div>

        <div v-else-if="activeMenu === 'dashboard'" style="text-align: center; padding-top: 50px;">
          <el-row :gutter="20">
            <el-col :span="8"><el-card shadow="hover"><h2>1024</h2><p>今日入库</p></el-card></el-col>
            <el-col :span="8"><el-card shadow="hover"><h2>128</h2><p>待发货</p></el-card></el-col>
            <el-col :span="8"><el-card shadow="hover"><h2>99%</h2><p>准时送达率</p></el-card></el-col>
          </el-row>
          <div style="margin-top: 50px; color: #999;">
             <h1>👋 欢迎回来，{{ user.nickname }}</h1>
             <p>今天是 2025年12月31日，准备好开始工作了吗？</p>
          </div>
        </div>

        <div v-else class="empty-placeholder">
          <el-empty description="该模块正在开发中..." />
        </div>

      </el-main>
    </el-container>
    
    <el-dialog v-model="dialogVisible" :title="isEdit ? '更新物流' : '新增包裹'" width="400px">
      <el-form :model="form" label-width="70px">
        <el-form-item label="单号" v-if="!isEdit"><el-input v-model="form.trackingNumber" /></el-form-item>
        <el-form-item label="发件人" v-if="!isEdit"><el-input v-model="form.sender" /></el-form-item>
        <el-form-item label="收件人" v-if="!isEdit"><el-input v-model="form.receiver" /></el-form-item>
        <el-form-item label="位置"><el-input v-model="form.location" /></el-form-item>
        <el-form-item label="状态" v-if="isEdit">
          <el-select v-model="form.status" style="width: 100%">
            <el-option label="待发货" value="Pending" />
            <el-option label="运输中" value="In Transit" />
            <el-option label="已送达" value="Delivered" />
          </el-select>
        </el-form-item>
      </el-form>
      <template #footer>
        <span class="dialog-footer">
          <el-button @click="dialogVisible = false">取消</el-button>
          <el-button type="primary" @click="submitForm">确定</el-button>
        </span>
      </template>
    </el-dialog>

  </el-container>
</template>

<script setup>
import { ref, reactive, onMounted, computed } from 'vue'
import axios from 'axios'
import { ElMessage, ElMessageBox } from 'element-plus'
import { User, Lock, Box, Odometer, Van, Setting, Expand, Search } from '@element-plus/icons-vue'

const API_BASE = 'http://localhost:3000/api'

// --- 状态管理 ---
const user = ref({ id: null, nickname: '', avatar: '' }) // 用户信息
const activeMenu = ref('dashboard') // 当前选中的菜单
const menuNames = { 'dashboard': '工作台', 'packages': '包裹管理', 'users': '员工管理', 'settings': '系统设置' }
const loading = ref(false)

// --- 登录逻辑 ---
const loginForm = reactive({ username: '', password: '' })
const handleLogin = async () => {
  if(!loginForm.username || !loginForm.password) return ElMessage.warning('请输入账号密码')
  loading.value = true
  try {
    const res = await axios.post(`${API_BASE}/login`, loginForm)
    if (res.data.success) {
      user.value = { 
        id: res.data.user.Id, 
        nickname: res.data.user.Nickname, 
        avatar: res.data.user.Avatar 
      }
      ElMessage.success('登录成功')
      fetchData() // 登录后加载数据
    }
  } catch (error) {
    ElMessage.error('账号或密码错误 (试下 admin / 123456)')
  } finally {
    loading.value = false
  }
}
const logout = () => {
  user.value = { id: null }
  loginForm.username = ''
  loginForm.password = ''
  ElMessage.info('已退出登录')
}

// --- 包裹管理逻辑 ---
const tableData = ref([])
const searchText = ref('')
const dialogVisible = ref(false)
const isEdit = ref(false)
const form = reactive({ id: null, trackingNumber: '', sender: '', receiver: '', location: '', status: '' })

// 搜索过滤
const filteredData = computed(() => {
  if (!searchText.value) return tableData.value
  return tableData.value.filter(item => 
    item.TrackingNumber.toLowerCase().includes(searchText.value.toLowerCase())
  )
})

const fetchData = async () => {
  try {
    const res = await axios.get(`${API_BASE}/packages`)
    tableData.value = res.data
  } catch (error) { console.error(error) }
}

const getStatusType = (status) => {
  if (status === 'Delivered') return 'success'
  if (status === 'In Transit') return 'primary'
  return 'warning'
}

const openDialog = () => {
  isEdit.value = false
  Object.assign(form, { trackingNumber: '', sender: '', receiver: '', location: '', status: 'Pending' })
  dialogVisible.value = true
}

const handleEdit = (row) => {
  isEdit.value = true
  Object.assign(form, { id: row.Id, location: row.Location, status: row.Status })
  dialogVisible.value = true
}

const submitForm = async () => {
  try {
    if (isEdit.value) {
      await axios.put(`${API_BASE}/packages/${form.id}`, { status: form.status, location: form.location })
    } else {
      await axios.post(`${API_BASE}/packages`, form)
    }
    ElMessage.success('操作成功')
    dialogVisible.value = false
    fetchData()
  } catch (error) { ElMessage.error('操作失败') }
}

const handleDelete = (id) => {
  ElMessageBox.confirm('确定删除?', '警告', { type: 'warning' }).then(async () => {
    await axios.delete(`${API_BASE}/packages/${id}`)
    ElMessage.success('已删除')
    fetchData()
  })
}
</script>

<style>
/* 全局样式重置 */
body { margin: 0; padding: 0; font-family: 'Helvetica Neue', Helvetica, 'PingFang SC', 'Hiragino Sans GB', 'Microsoft YaHei', '微软雅黑', Arial, sans-serif; background-color: #f0f2f5; }

/* 登录页样式 */
.login-container { height: 100vh; display: flex; justify-content: center; align-items: center; background: linear-gradient(135deg, #2c3e50, #4ca1af); }
.login-card { width: 400px; border-radius: 10px; }
.login-header h2 { text-align: center; color: #333; margin: 0; }

/* 布局样式 */
.layout-container { height: 100vh; }
.aside-menu { background-color: #304156; color: white; display: flex; flex-direction: column; transition: width 0.3s; }
.logo { height: 60px; display: flex; align-items: center; justify-content: center; font-size: 18px; font-weight: bold; background-color: #2b3a4d; }
.logo .el-icon { margin-right: 8px; font-size: 24px; color: #409EFF; }
.el-menu-vertical { border-right: none !important; }

.header { background-color: #fff; border-bottom: 1px solid #dcdfe6; display: flex; align-items: center; justify-content: space-between; padding: 0 20px; box-shadow: 0 1px 4px rgba(0,21,41,.08); }
.header-left { display: flex; align-items: center; }
.header-right { display: flex; align-items: center; font-size: 14px; }
.username { margin: 0 10px; font-weight: 500; }

.main-content { padding: 20px; background-color: #f0f2f5; }
.page-header { display: flex; justify-content: space-between; align-items: center; margin-bottom: 20px; background: #fff; padding: 15px 20px; border-radius: 8px; }
.page-header h3 { margin: 0; }
.empty-placeholder { display: flex; justify-content: center; align-items: center; height: 80%; background: #fff; border-radius: 8px; }
</style>