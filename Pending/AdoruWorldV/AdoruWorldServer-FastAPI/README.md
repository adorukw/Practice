# AdoruWorld Server - FastAPI

基于 Python + FastAPI + SQLite 的个人站点后端服务。

## 环境准备

### 创建 Conda 环境

```bash
conda create -n AdoruWorldServer python=3.11 -y
```

### 激活环境

```bash
conda activate AdoruWorldServer
```

### 安装依赖

```bash
pip install -r requirements.txt
```

## 启动服务

### 开发模式启动

```bash
uvicorn app.main:app --host 0.0.0.0 --port 8000 --reload
```

### 生产模式启动

```bash
uvicorn app.main:app --host 0.0.0.0 --port 8000
```

启动后访问：

- API 根路径：`http://localhost:8000/`
- Swagger 文档：`http://localhost:8000/docs`
- ReDoc 文档：`http://localhost:8000/redoc`

## 项目结构

```
AdoruWorldServer-FastAPI/
├── app/
│   ├── __init__.py
│   ├── main.py              # FastAPI 应用入口
│   ├── config.py            # 配置
│   ├── database.py          # 异步数据库连接
│   ├── utils.py             # 工具函数
│   ├── models/              # SQLAlchemy ORM 模型
│   ├── schemas/             # Pydantic schemas
│   ├── routers/             # API 路由
│   └── crud/                # 数据库 CRUD 操作
└── requirements.txt
```

## API 端点

所有 API 前缀为 `/api/v1`。

| 模块         | 端点                   | 方法             | 说明                 |
| ---------- | -------------------- | -------------- | ------------------ |
| Posts      | `/posts`             | GET/POST       | 文章列表/创建            |
| <br />     | `/posts/{id}`        | GET/PUT/DELETE | 文章 CRUD            |
| <br />     | `/posts/slug/{slug}` | GET            | 按 slug 获取（自动增加浏览量） |
| <br />     | `/posts/archives`    | GET            | 按年月归档              |
| Categories | `/categories`        | GET/POST       | 分类列表/创建            |
| <br />     | `/categories/{id}`   | GET/PUT/DELETE | 分类 CRUD            |
| Tags       | `/tags`              | GET/POST       | 标签列表/创建            |
| <br />     | `/tags/{id}`         | GET/PUT/DELETE | 标签 CRUD            |
| Projects   | `/projects`          | GET/POST       | 项目列表/创建            |
| <br />     | `/projects/{id}`     | GET/PUT/DELETE | 项目 CRUD            |
| Dex        | `/dex`               | GET/POST       | 收藏条目列表/创建          |
| <br />     | `/dex/{id}`          | GET/PUT/DELETE | 条目 CRUD            |
| <br />     | `/dex/categories`    | GET            | 分类信息               |
| <br />     | `/dex/statuses`      | GET            | 状态信息               |
| <br />     | `/dex/stats`         | GET            | 统计                 |
| Author     | `/author`            | GET/POST/PUT   | 作者信息               |
| Nav        | `/nav`               | GET/POST       | 导航项列表/创建           |
| <br />     | `/nav/{id}`          | PUT/DELETE     | 导航项更新/删除           |
| Stats      | `/stats`             | GET            | 站点统计               |

## 快速测试示例

```bash
# 创建分类
curl -X POST http://localhost:8000/api/v1/categories \
  -H "Content-Type: application/json" \
  -d '{"name":"技术","slug":"tech","color":"#2196F3"}'

# 创建标签
curl -X POST http://localhost:8000/api/v1/tags \
  -H "Content-Type: application/json" \
  -d '{"name":"Python","slug":"python","color":"#3776AB"}'

# 创建文章
curl -X POST http://localhost:8000/api/v1/posts \
  -H "Content-Type: application/json" \
  -d '{"slug":"hello-world","title":"Hello World","content":"My first post!","published":true,"reading_time":1,"word_count":6}'

# 创建项目
curl -X POST http://localhost:8000/api/v1/projects \
  -H "Content-Type: application/json" \
  -d '{"name":"AdoruWorld","tech":["Vue","FastAPI"],"status":"in-progress"}'

# 创建 Dex 条目
curl -X POST http://localhost:8000/api/v1/dex \
  -H "Content-Type: application/json" \
  -d '{"slug":"steins-gate","title":"Steins;Gate","cover_image":"/images/steins-gate.jpg","category":"anime","status":"completed","rating":9.5}'

# 创建作者
curl -X POST http://localhost:8000/api/v1/author \
  -H "Content-Type: application/json" \
  -d '{"name":"AdoruKw","avatar":"/avatar.jpg","bio":"A developer","location":"China"}'

# 查看站点统计
curl http://localhost:8000/api/v1/stats
```

