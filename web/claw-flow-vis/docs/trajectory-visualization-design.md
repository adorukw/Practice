# Trajectory 可视化方案思考

## 一、数据概览

OpenClaw trajectory 导出包含 7 个文件，核心是 `events.jsonl`（229 条事件），辅以 `metadata.json`、`artifacts.json`、`prompts.json`、`session-branch.json`、`tools.json`、`system-prompt.txt`。

与 Session 数据的关键区别：**trajectory 是事件级别的时序流**，而非消息级别的对话流。它同时记录了运行时事件（模型调用、上下文编译）和对话事件（用户消息、工具调用），粒度更细、维度更丰富。

---

## 二、核心数据模型

### 2.1 事件类型分类

| 类别 | 事件类型 | 可视化价值 |
|------|---------|-----------|
| **生命周期** | `session.started` / `session.ended` | 划分 Run 边界 |
| **运行时** | `trace.metadata` / `context.compiled` / `prompt.submitted` / `model.completed` / `trace.artifacts` | 性能分析、Token 消耗追踪 |
| **对话** | `user.message` / `assistant.message` | 对话流主线 |
| **工具** | `tool.call` / `tool.result` | 工具调用链 |
| **配置** | `session.model_change` / `session.thinking_level_change` / `session.custom` | 配置变更节点 |

### 2.2 两种关联维度

1. **时序维度**：所有事件按 `seq` 严格递增，形成全局时间线
2. **树形维度**：transcript 事件通过 `entryId` / `parentEntryId` 构成父子树

### 2.3 Run 概念

一个 Run = 一次完整的「用户触发 → 模型推理 → 工具执行 → 返回结果」循环。由 `session.started`（trigger=user/heartbeat/cron）到 `session.ended` 界定。

---

## 三、可视化维度设计

### 3.1 维度一：时间线视图（Timeline）

**核心思路**：将 229 条事件按时间排列，用甘特图/泳道图展示。

**泳道划分**：
- **用户层**：`user.message` 事件
- **模型层**：`assistant.message` + thinking/text 内容
- **工具层**：`tool.call` → `tool.result` 调用链
- **运行时层**：`session.started` / `model.completed` / `session.ended` 等
- **配置层**：`model_change` / `thinking_level_change`

**每个事件展示**：
- 时间戳（精确到毫秒）
- 事件类型图标
- 关键信息摘要（如工具名、Token 数、耗时）
- Run 边界用竖线/色块标记

### 3.2 维度二：调用链图（Call Graph）

**核心思路**：基于 `entryId` / `parentEntryId` 树形关系，构建 DAG 图。

**节点类型**：
- 用户消息节点（入口）
- 助手消息节点（含 thinking/text/toolCall 分支）
- 工具调用节点（从 assistant 分支出来）
- 工具结果节点（连回或触发下一轮 assistant）

**与 Session 视图的区别**：
- Session 视图只有 assistant → toolCall → toolResult 三层
- Trajectory 视图可以展示**多轮嵌套**：assistant → toolCall → toolResult → assistant → toolCall → ...
- 可以展示 Run 边界（哪些事件属于同一次模型调用）

### 3.3 维度三：性能分析视图（Performance）

**核心思路**：利用 runtime 事件的 Token 和时间数据，做性能可视化。

**指标**：
- 每次 Run 的总耗时（`session.started.ts` → `session.ended.ts`）
- 每次 `model.completed` 的 Token 消耗（input/output/cacheRead/total）
- 工具执行耗时（`tool.call.ts` → `tool.result.ts`）
- 缓存命中率（`cacheRead / total`）

**图表**：
- Token 消耗堆叠面积图（按 Run 划分）
- 工具执行耗时柱状图
- 缓存命中率折线图

### 3.4 维度四：上下文分析视图（Context）

**核心思路**：利用 `context.compiled` 和 `prompts.json` 分析系统提示词构成。

**展示内容**：
- 系统提示词总字符数及各部分占比（工具 Schema / 技能提示词 / 工作区文件注入）
- 注入的工作区文件列表（AGENTS.md、SOUL.md 等）及其字符数
- 每次提交的 prompt 长度变化趋势

---

## 四、页面布局方案

### 4.1 整体结构

```
┌──────────────────────────────────────────────────┐
│ NavBar [Session分析] [Trajectory分析] [会话选择▼]  │
├──────────────────────────────────────────────────┤
│ Tab: [时间线] [调用链] [性能] [上下文]             │
├──────────┬───────────────────────────────────────┤
│          │                                       │
│  侧边栏   │          主视图区域                    │
│  (事件    │   (根据 Tab 切换不同可视化)             │
│   列表)   │                                       │
│          │                                       │
├──────────┴───────────────────────────────────────┤
│ Detail Panel (点击事件查看详情)                     │
└──────────────────────────────────────────────────┘
```

### 4.2 四个 Tab 页面

#### Tab 1：时间线（默认页）

- **主区域**：横向泳道甘特图
  - Y 轴：5 个泳道（用户/模型/工具/运行时/配置）
  - X 轴：时间
  - 每个事件是一个小方块，hover 显示摘要
  - Run 边界用半透明色块标记
  - 点击事件 → 侧边栏高亮 + Detail Panel 展示详情
- **侧边栏**：事件列表（按时间排序），支持按类型过滤

#### Tab 2：调用链

- **主区域**：Vue Flow DAG 图（复用 Session 视图的技术栈）
  - 节点：user.message / assistant.message / tool.call / tool.result
  - 边：parentEntryId 关系 + toolCallId 关系
  - Run 边界用背景色块标记
  - 与 Session 视图的区别：展示 entryId 树形关系，而非消息序列
- **侧边栏**：节点列表，支持过滤

#### Tab 3：性能

- **主区域**：图表区
  - 上方：Token 消耗堆叠面积图（ECharts / Chart.js）
  - 中间：工具执行耗时柱状图
  - 下方：Run 统计表格（耗时/Token/工具数/状态）
- **侧边栏**：Run 列表，点击跳转到对应时间线位置

#### Tab 4：上下文

- **主区域**：
  - 左侧：系统提示词构成饼图
  - 右侧：注入文件列表 + 字符数
  - 下方：prompt 长度变化趋势图
- **侧边栏**：文件列表

---

## 五、数据解析策略

### 5.1 events.jsonl 解析

```
输入：events.jsonl（每行一个 JSON）
输出：结构化的事件列表 + 关联索引

解析步骤：
1. 逐行读取，解析为 Event 对象
2. 按 seq 排序
3. 构建 entryId → Event 索引
4. 构建 parentEntryId → children 索引
5. 识别 Run 边界（session.started → session.ended 配对）
6. 计算 Run 内事件的耗时
7. 关联 tool.call 和 tool.result（通过 toolCallId）
```

### 5.2 辅助文件解析

| 文件 | 解析方式 | 用途 |
|------|---------|------|
| `manifest.json` | 直接读取 | 验证数据完整性 |
| `metadata.json` | 提取 model/harness/config | 性能视图的元信息 |
| `artifacts.json` | 提取 usage/toolMetas | 性能视图的汇总数据 |
| `prompts.json` | 提取 system/submittedPrompts | 上下文视图 |
| `session-branch.json` | 提取 entries 树 | 调用链图的树形关系 |
| `tools.json` | 提取工具名+描述 | 工具调用节点的元信息 |
| `system-prompt.txt` | 直接读取 | 上下文视图 |

---

## 六、与 Session 视图的关系

| 维度 | Session 视图 | Trajectory 视图 |
|------|-------------|----------------|
| 数据源 | session_info JSON | trajectory 导出文件包 |
| 粒度 | 消息级别 | 事件级别 |
| 维度 | 对话流 + 工具调用 | 时间线 + 调用链 + 性能 + 上下文 |
| 关联 | 消息序列 | entryId 树 + Run 分组 |
| 特色 | AI 摘要 | 性能分析、Run 追踪 |

**互补关系**：Session 视图回答「对话做了什么」，Trajectory 视图回答「系统怎么执行的」。

---

## 七、技术选型建议

| 模块 | 技术 | 理由 |
|------|------|------|
| 时间线 | 自定义 Canvas/SVG 或 vis-timeline | 泳道甘特图需要精确时间轴控制 |
| 调用链 | Vue Flow（复用） | 与 Session 视图技术栈一致 |
| 性能图表 | ECharts | 功能丰富，支持堆叠面积图/柱状图 |
| 上下文 | ECharts 饼图 + 自定义列表 | 简单直观 |
| 事件解析 | 纯 TypeScript | 无额外依赖 |

---

## 八、优先级建议

1. **P0 - 时间线视图**：最直观，能展示 trajectory 的核心价值（事件时序 + Run 边界）
2. **P1 - 调用链视图**：复用 Vue Flow，开发成本低，与 Session 视图形成互补
3. **P2 - 性能视图**：需要引入 ECharts，但数据价值高（Token 追踪、耗时分析）
4. **P3 - 上下文视图**：相对独立，可后续补充

建议先实现时间线 + 调用链两个 Tab，性能和上下文作为后续迭代。
