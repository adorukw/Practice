<template>
  <el-container class="help-container">
    <el-main>
      <h2 class="help-title">系统使用帮助</h2>
      <el-collapse v-model="activeNames" accordion>
        <!-- 地图操作 -->
        <el-collapse-item title="地图基础操作" name="1">
          <div class="help-section">
            <h3>🌍 地图控件：</h3>
            <p>1. <el-tag>⤒</el-tag> 重置视图：恢复初始地图中心点和缩放层级</p>
            <p>2. <el-tag>⇌</el-tag> 图层切换：在矢量底图和卫星影像之间切换</p>
            <p>3. <el-tag>⊙</el-tag> 定位功能：获取您当前的地理位置并居中显示</p>
            <p>4. 鼠标操作：
              <br>• 左键拖动：平移地图
              <br>• 右键拖动：调整地图倾斜角度
              <br>• 滚轮滑动：缩放地图层级
            </p>
          </div>
        </el-collapse-item>

        <!-- 数据操作 -->
        <el-collapse-item title="数据可视化操作" name="2">
          <div class="help-section">
            <h3>📊 数据选择：</h3>
            <el-steps direction="vertical" :active="3">
              <el-step title="选择主类型" description="从7个主数据类型中选择（如碳循环、气象等）" />
              <el-step title="选择子类型" description="根据主类型显示对应的子数据类型" />
              <el-step title="选择具体数据" description="选择具体年份或特定数据集" />
            </el-steps>

            <h3>📈 图表功能：</h3>
            <p>• 折线图：展示数据随时间变化趋势</p>
            <p>• 直方图：显示数据分布情况</p>
            <p>• 热力图：展示空间数据密度分布</p>
            <p>• 等高线：显示数据地形分布特征</p>
          </div>
        </el-collapse-item>

        <!-- 动画控制 -->
        <el-collapse-item title="时间动画控制" name="3">
          <div class="help-section">
            <h3>⏯️ 动画操作步骤：</h3>
            <el-steps :active="3" simple>
              <el-step title="1. 选择年份范围" icon="el-icon-date" />
              <el-step title="2. 点击开始按钮" icon="el-icon-video-play" />
              <el-step title="3. 自动逐年播放" icon="el-icon-refresh-right" />
            </el-steps>
            <p class="tip">提示：动画播放过程中可以随时切换数据层，停止后保留当前状态</p>
          </div>
        </el-collapse-item>

        <!-- 高级工具 -->
        <el-collapse-item title="高级分析工具" name="4">
          <div class="help-section">
            <h3>🛠️ 绘图工具：</h3>
            <p><el-tag>●</el-tag> 点要素标记：点击地图添加标记点</p>
            <p><el-tag>➤</el-tag> 绘制线段：点击确定起点，双击结束绘制</p>
            <p><el-tag>▢</el-tag> 绘制多边形：至少3个顶点，双击闭合图形</p>
            <p><el-tag>✖️</el-tag> 停止绘制：退出当前绘图模式</p>

            <h3>⚙️ 其他工具：</h3>
            <p>• 地图旋转：通过↻/↺按钮调整地图方向</p>
            <p>• 数值查询：点击地图显示当前位置的栅格数据值</p>
            <p>• 图例说明：右下角显示当前数据层的颜色映射关系</p>
          </div>
        </el-collapse-item>
      </el-collapse>
    </el-main>
  </el-container>
</template>

<script setup>
import { ref } from 'vue'

const activeNames = ref(['1'])
</script>

<style scoped>
.help-container {
  margin: 0;
  height: 98vh;
  width: 100%;
  margin: 0 auto;
  background-color: #f9f9f9;
  border-radius: 8px;
  box-shadow: 0 2px 12px rgba(0, 0, 0, 0.1);
}

.help-title {
  color: #2c3e50;
  text-align: center;
  margin-bottom: 24px;
  font-size: 24px;
}

.help-section {
  padding: 10px 20px;
}

.help-section h3 {
  color: #34495e;
  margin: 15px 0 10px;
  font-size: 16px;
}

.help-section p {
  color: #7f8c8d;
  line-height: 1.8;
  margin: 8px 0;
}

.tip {
  color: #e67e22;
  font-size: 13px;
  margin-top: 12px;
}

.el-tag {
  margin: 0 4px;
  vertical-align: middle;
}
</style>