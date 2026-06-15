import os
from pathlib import Path

BASE_DIR = Path(__file__).parent

DATA_DIR = BASE_DIR / "data"
RAW_DATA_FILE = DATA_DIR / "problems.json"
ANALYZED_DATA_FILE = DATA_DIR / "analyzed_problems.json"
GRAPH_DATA_FILE = DATA_DIR / "knowledge_graph.json"

DATA_DIR.mkdir(parents=True, exist_ok=True)

LLM_PROVIDER = os.getenv("LLM_PROVIDER", "deepseek")

OLLAMA_BASE_URL = os.getenv("OLLAMA_BASE_URL", "http://localhost:11434")
OLLAMA_MODEL = os.getenv("OLLAMA_MODEL", "qwen3.5:4b")

OPENAI_API_KEY = os.getenv("OPENAI_API_KEY", "")
OPENAI_BASE_URL = os.getenv("OPENAI_BASE_URL", "https://api.openai.com/v1")
OPENAI_MODEL = os.getenv("OPENAI_MODEL", "gpt-3.5-turbo")

DASHSCOPE_API_KEY = os.getenv("DASHSCOPE_API_KEY", "")
DASHSCOPE_MODEL = os.getenv("DASHSCOPE_MODEL", "qwen-turbo")

DEEPSEEK_API_KEY = os.getenv("DEEPSEEK_API_KEY", "")
DEEPSEEK_BASE_URL = os.getenv("DEEPSEEK_BASE_URL", "https://api.deepseek.com")
DEEPSEEK_MODEL = os.getenv("DEEPSEEK_MODEL", "deepseek-chat")

LLM_CONFIG = {
    "ollama": {
        "base_url": OLLAMA_BASE_URL,
        "model": OLLAMA_MODEL
    },
    "openai": {
        "api_key": OPENAI_API_KEY,
        "base_url": OPENAI_BASE_URL,
        "model": OPENAI_MODEL
    },
    "dashscope": {
        "api_key": DASHSCOPE_API_KEY,
        "model": DASHSCOPE_MODEL
    },
    "deepseek": {
        "api_key": DEEPSEEK_API_KEY,
        "base_url": DEEPSEEK_BASE_URL,
        "model": DEEPSEEK_MODEL
    }
}

HDUOJ_PROBLEM_URL = "https://acm.hdu.edu.cn/showproblem.php?pid={problem_id}"

ANALYSIS_PROMPT_TEMPLATE = """你是一位经验丰富的算法竞赛教练，专门分析算法竞赛题目。请分析以下题目，提取其涉及的算法、数据结构和核心概念。

## 题目信息

**题目ID**: {problem_id}
**题目名称**: {title}
**题目描述**: {problem_description}
**输入格式**: {input}
**输出格式**: {output}
**样例输入**: {sample_input}
**样例输出**: {sample_output}
**题目来源**: {source}

## 分析要求

请根据题目信息，分析并返回以下内容（以JSON格式输出）：

1. **algorithms**: 题目涉及的主要算法标签（数组形式），例如：
   - 基础算法：模拟、枚举、递归、分治、贪心、动态规划、回溯、搜索
   - 排序算法：冒泡排序、选择排序、插入排序、快速排序、归并排序、堆排序、计数排序、基数排序
   - 搜索算法：二分查找、深度优先搜索、广度优先搜索、双向搜索
   - 图论算法：最短路径、最小生成树、拓扑排序、连通分量、网络流、二分图匹配
   - 数学算法：数论、组合数学、博弈论、概率论、高精度计算
   - 字符串算法：字符串匹配、KMP、Trie树、后缀数组
   - 计算几何：点线面关系、凸包、多边形

2. **data_structures**: 题目涉及的数据结构（数组形式），例如：
   - 数组、链表、栈、队列、树、图、堆、哈希表、并查集、线段树、树状数组

3. **concepts**: 题目涉及的核心概念（数组形式），例如：
   - 时间复杂度优化、空间复杂度优化、边界条件处理、输入输出优化、精度处理

4. **difficulty**: 题目难度（字符串），可选值：
   - 入门：适合初学者，基础语法练习
   - 简单：需要掌握基本算法思想
   - 中等：需要综合运用多种算法
   - 困难：需要深入理解和优化
   - 挑战：需要创新性思维

5. **analysis**: 简要的题目分析（字符串，100字以内）

## 输出格式

请严格按照以下JSON格式输出，不要添加任何其他内容：

```json
{{
    "algorithms": ["算法1", "算法2"],
    "data_structures": ["数据结构1", "数据结构2"],
    "concepts": ["概念1", "概念2"],
    "difficulty": "难度等级",
    "analysis": "题目分析说明"
}}
```

请确保：
1. 只输出JSON格式的内容，不要包含其他解释性文字
2. 数组中的标签要具体、准确
3. 如果题目很简单，可能只涉及一个算法或数据结构
4. 难度评估要客观，参考通过率等信息
"""

PAGE_CONFIG = {
    "page_title": "杭电OJ算法知识图谱",
    "page_icon": "🎓",
    "layout": "wide",
    "initial_sidebar_state": "expanded"
}

GRAPH_CONFIG = {
    "height": "750px",
    "width": "100%",
    "bgcolor": "#222222",
    "font_color": "white",
    "node_colors": {
        "problem": "#FF6B6B",
        "algorithm": "#4ECDC4",
        "data_structure": "#45B7D1",
        "concept": "#96CEB4"
    }
}

DIFFICULTY_COLORS = {
    "入门": "#28a745",
    "简单": "#17a2b8",
    "中等": "#ffc107",
    "困难": "#fd7e14",
    "挑战": "#dc3545"
}
