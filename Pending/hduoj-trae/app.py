import streamlit as st
import streamlit.components.v1 as components
import pandas as pd
from pathlib import Path

import config
from utils import (
    load_raw_problems,
    load_analyzed_problems,
    save_analyzed_problems,
    merge_problem_with_analysis,
    get_all_tags,
    filter_problems_by_tags,
    get_statistics,
    AnalyzedProblem
)
from analyzer import ProblemAnalyzer
from graph_builder import KnowledgeGraph

st.set_page_config(**config.PAGE_CONFIG)

st.markdown("""
<style>
    .main-header {
        font-size: 2.5rem;
        font-weight: bold;
        text-align: center;
        margin-bottom: 1rem;
    }
    .sub-header {
        font-size: 1.5rem;
        font-weight: bold;
        margin-top: 1rem;
        margin-bottom: 0.5rem;
    }
    .metric-card {
        background-color: #f0f2f6;
        border-radius: 10px;
        padding: 1rem;
        margin: 0.5rem 0;
    }
    .tag-badge {
        display: inline-block;
        padding: 0.25rem 0.5rem;
        margin: 0.1rem;
        border-radius: 5px;
        font-size: 0.85rem;
    }
    .algorithm-tag {
        background-color: #4ECDC4;
        color: white;
    }
    .data-structure-tag {
        background-color: #45B7D1;
        color: white;
    }
    .concept-tag {
        background-color: #96CEB4;
        color: white;
    }
</style>
""", unsafe_allow_html=True)

if "llm_provider" not in st.session_state:
    st.session_state.llm_provider = config.LLM_PROVIDER

if "analyzer" not in st.session_state:
    st.session_state.analyzer = ProblemAnalyzer(provider=st.session_state.llm_provider)

if "analyzed_problems" not in st.session_state:
    st.session_state.analyzed_problems = load_analyzed_problems()

if "knowledge_graph" not in st.session_state:
    st.session_state.knowledge_graph = KnowledgeGraph()


def render_home_page():
    st.markdown('<p class="main-header">🎓 杭电OJ算法知识图谱</p>', unsafe_allow_html=True)
    
    st.markdown("---")
    
    st.markdown("### 📖 项目简介")
    st.markdown("""
    本系统是一个基于AI的算法题目分析平台，能够自动分析杭电OJ题目，提取算法标签、数据结构和核心概念，
    并构建可视化的知识图谱，帮助用户更好地理解和学习算法知识。
    """)
    
    st.markdown("### 🛠️ 技术栈")
    col1, col2, col3 = st.columns(3)
    with col1:
        st.markdown("- **前端界面**: Streamlit")
        st.markdown("- **AI集成**: LangChain + Ollama")
    with col2:
        st.markdown("- **数据处理**: Pandas")
        st.markdown("- **图谱构建**: NetworkX")
    with col3:
        st.markdown("- **可视化**: PyVis")
        st.markdown("- **配置管理**: Python标准库")
    
    st.markdown("### 📋 使用方法")
    
    st.markdown("#### 1️⃣ 准备数据")
    st.markdown("""
    - 将原始题目数据放在 `data/problems.json` 文件中
    - 数据格式为JSON数组，每个题目包含 `problem_id`, `title`, `problem_description` 等字段
    """)
    
    st.markdown("#### 2️⃣ 启动Ollama服务")
    st.markdown("""
    - 确保已安装Ollama并下载了模型: `ollama pull qwen2.5:4b`
    - 启动Ollama服务: `ollama serve`
    """)
    
    st.markdown("#### 3️⃣ 分析题目")
    st.markdown("""
    - 进入「分析数据」页面
    - 选择起始位置和批量大小
    - 点击「开始分析」按钮
    - 系统会自动跳过已分析的题目
    """)
    
    st.markdown("#### 4️⃣ 查看结果")
    st.markdown("""
    - **题目列表**: 查看所有题目及其分析结果
    - **标签筛选**: 按算法标签筛选题目
    - **知识图谱**: 可视化查看题目之间的关系
    """)
    
    st.markdown("### 📌 注意事项")
    st.warning("""
    - 分析题目前请确保Ollama服务已启动
    - 分析结果会自动保存，下次打开应用时会自动加载
    - 建议批量分析时控制数量，避免一次性分析过多题目
    """)


def render_analysis_page():
    st.markdown('<p class="sub-header">📊 数据分析</p>', unsafe_allow_html=True)
    
    raw_problems = load_raw_problems()
    
    if not raw_problems:
        st.error("未找到原始题目数据！请将 `problems.json` 文件放入 `data` 文件夹中。")
        return
    
    st.markdown("#### 🤖 LLM配置")
    
    provider_options = {
        "Ollama (本地)": "ollama",
        "阿里云通义千问": "dashscope",
        "DeepSeek": "deepseek",
        "OpenAI": "openai"
    }
    
    selected_provider_display = st.selectbox(
        "选择LLM提供商",
        list(provider_options.keys()),
        index=list(provider_options.values()).index(st.session_state.llm_provider)
    )
    
    selected_provider = provider_options[selected_provider_display]
    
    if selected_provider != st.session_state.llm_provider:
        st.session_state.llm_provider = selected_provider
        st.session_state.analyzer = ProblemAnalyzer(provider=selected_provider)
    
    provider_config = config.LLM_CONFIG.get(selected_provider, {})
    
    if selected_provider == "ollama":
        st.info(f"模型: {provider_config.get('model', '')} | 地址: {provider_config.get('base_url', '')}")
    else:
        api_key = provider_config.get("api_key", "")
        if api_key:
            masked_key = api_key[:8] + "****" + api_key[-4:] if len(api_key) > 12 else "****"
            st.info(f"模型: {provider_config.get('model', '')} | API Key: {masked_key}")
        else:
            st.warning(f"⚠️ {selected_provider.upper()}_API_KEY 未设置，请配置环境变量")
            with st.expander("📋 配置说明"):
                st.markdown(f"""
                **方式一：设置环境变量**
                ```bash
                export {selected_provider.upper()}_API_KEY=your_api_key
                ```
                
                **方式二：创建 .env 文件**
                ```
                {selected_provider.upper()}_API_KEY=your_api_key
                ```
                
                **各平台获取API Key：**
                - 阿里云通义千问: https://dashscope.console.aliyun.com/
                - DeepSeek: https://platform.deepseek.com/
                - OpenAI: https://platform.openai.com/
                """)
    
    st.markdown("---")
    
    is_connected, message = st.session_state.analyzer.check_connection()
    if is_connected:
        st.success(f"✅ {message}")
    else:
        st.error(f"❌ {message}")
    
    st.markdown("---")
    
    analyzed_problems = st.session_state.analyzed_problems
    merged_problems = merge_problem_with_analysis(raw_problems, analyzed_problems)
    stats = get_statistics(merged_problems)
    
    col1, col2, col3, col4 = st.columns(4)
    with col1:
        st.metric("总题目数", stats["total"])
    with col2:
        st.metric("已分析", stats["analyzed"])
    with col3:
        st.metric("未分析", stats["unanalyzed"])
    with col4:
        st.metric("分析进度", f"{stats['analysis_rate']:.1f}%")
    
    st.markdown("---")
    
    st.markdown("#### 难度分布")
    diff_col1, diff_col2, diff_col3, diff_col4, diff_col5 = st.columns(5)
    with diff_col1:
        st.metric("入门", stats["difficulty_distribution"]["入门"])
    with diff_col2:
        st.metric("简单", stats["difficulty_distribution"]["简单"])
    with diff_col3:
        st.metric("中等", stats["difficulty_distribution"]["中等"])
    with diff_col4:
        st.metric("困难", stats["difficulty_distribution"]["困难"])
    with diff_col5:
        st.metric("挑战", stats["difficulty_distribution"]["挑战"])
    
    st.markdown("---")
    
    st.markdown("#### 批量分析")
    
    col1, col2 = st.columns(2)
    with col1:
        start_index = st.number_input(
            "起始位置",
            min_value=0,
            max_value=len(raw_problems) - 1,
            value=0,
            step=1
        )
    with col2:
        batch_size = st.number_input(
            "批量大小",
            min_value=1,
            max_value=500,
            value=5,
            step=1
        )
    
    skip_analyzed = st.checkbox("跳过已分析的题目", value=True)
    
    if st.button("🚀 开始分析", type="primary"):
        if not is_connected:
            st.error("请先配置正确的API Key或启动Ollama服务！")
            return
        
        progress_bar = st.progress(0)
        status_text = st.empty()
        
        def progress_callback(index, problem_id, status):
            progress = (index - start_index + 1) / batch_size
            progress_bar.progress(min(progress, 1.0))
            if status == "analyzing":
                status_text.text(f"正在分析题目 {problem_id}...")
            elif status == "completed":
                status_text.text(f"题目 {problem_id} 分析完成")
            elif status == "skipped":
                status_text.text(f"跳过已分析的题目 {problem_id}")
            elif status == "failed":
                status_text.text(f"题目 {problem_id} 分析失败")
        
        st.session_state.analyzed_problems = st.session_state.analyzer.analyze_batch(
            problems=raw_problems,
            start_index=start_index,
            batch_size=batch_size,
            skip_analyzed=skip_analyzed,
            analyzed_problems=st.session_state.analyzed_problems,
            progress_callback=progress_callback
        )
        
        save_analyzed_problems(st.session_state.analyzed_problems)
        
        progress_bar.progress(1.0)
        status_text.text("分析完成！")
        st.success("批量分析完成，结果已保存！")
        st.rerun()


def render_problems_page():
    st.markdown('<p class="sub-header">📝 题目列表</p>', unsafe_allow_html=True)
    
    raw_problems = load_raw_problems()
    
    if not raw_problems:
        st.error("未找到原始题目数据！")
        return
    
    analyzed_problems = st.session_state.analyzed_problems
    merged_problems = merge_problem_with_analysis(raw_problems, analyzed_problems)
    
    search_term = st.text_input("🔍 搜索题目", placeholder="输入题目ID或标题...")
    
    if search_term:
        filtered_problems = [
            ap for ap in merged_problems
            if search_term.lower() in str(ap.problem.problem_id).lower()
            or search_term.lower() in ap.problem.title.lower()
        ]
    else:
        filtered_problems = merged_problems
    
    for ap in filtered_problems:
        with st.expander(
            f"{'✅' if ap.is_analyzed() else '⏳'} P{ap.problem.problem_id} - {ap.problem.title}",
            expanded=False
        ):
            col1, col2 = st.columns([3, 1])
            
            with col1:
                st.markdown(f"**题目ID**: {ap.problem.problem_id}")
                st.markdown(f"**标题**: {ap.problem.title}")
                st.markdown(f"**作者**: {ap.problem.author}")
                st.markdown(f"**来源**: {ap.problem.source}")
                st.markdown(f"**时间限制**: {ap.problem.time_limit}")
                st.markdown(f"**提交数**: {ap.problem.total_submissions}")
                st.markdown(f"**通过数**: {ap.problem.accepted_submissions}")
                st.markdown(f"**通过率**: {ap.problem.get_acceptance_rate():.2f}%")
                
                st.markdown("---")
                st.markdown(f"**题目描述**:\n{ap.problem.problem_description}")
                st.markdown(f"**输入格式**:\n{ap.problem.input}")
                st.markdown(f"**输出格式**:\n{ap.problem.output}")
                st.markdown(f"**样例输入**:\n```\n{ap.problem.sample_input}```")
                st.markdown(f"**样例输出**:\n```\n{ap.problem.sample_output}```")
            
            with col2:
                st.link_button(
                    "🔗 查看原题",
                    ap.problem.get_hduoj_url(),
                    use_container_width=True
                )
                
                if ap.is_analyzed():
                    st.markdown("#### 分析结果")
                    
                    diff_color = config.DIFFICULTY_COLORS.get(
                        ap.analysis.difficulty,
                        "#6c757d"
                    )
                    st.markdown(
                        f'<span style="background-color:{diff_color};padding:0.25rem 0.5rem;border-radius:5px;color:white;">{ap.analysis.difficulty}</span>',
                        unsafe_allow_html=True
                    )
                    
                    st.markdown("**算法标签**:")
                    for alg in ap.analysis.algorithms:
                        st.markdown(
                            f'<span class="tag-badge algorithm-tag">{alg}</span>',
                            unsafe_allow_html=True
                        )
                    
                    st.markdown("**数据结构**:")
                    for ds in ap.analysis.data_structures:
                        st.markdown(
                            f'<span class="tag-badge data-structure-tag">{ds}</span>',
                            unsafe_allow_html=True
                        )
                    
                    st.markdown("**核心概念**:")
                    for concept in ap.analysis.concepts:
                        st.markdown(
                            f'<span class="tag-badge concept-tag">{concept}</span>',
                            unsafe_allow_html=True
                        )
                    
                    st.markdown(f"**分析**: {ap.analysis.analysis}")
                    
                    if st.button("🗑️ 清除分析结果", key=f"clear_{ap.problem.problem_id}"):
                        if ap.problem.problem_id in st.session_state.analyzed_problems:
                            del st.session_state.analyzed_problems[ap.problem.problem_id]
                            save_analyzed_problems(st.session_state.analyzed_problems)
                            st.success("分析结果已清除！")
                            st.rerun()
                else:
                    st.markdown("#### 尚未分析")
                    
                    is_connected, _ = st.session_state.analyzer.check_connection()
                    
                    if st.button(
                        "🔬 分析此题目",
                        key=f"analyze_{ap.problem.problem_id}",
                        disabled=not is_connected
                    ):
                        with st.spinner("正在分析..."):
                            result = st.session_state.analyzer.analyze_problem(ap.problem)
                            if result:
                                st.session_state.analyzed_problems[ap.problem.problem_id] = AnalyzedProblem(
                                    problem=ap.problem,
                                    analysis=result
                                )
                                save_analyzed_problems(st.session_state.analyzed_problems)
                                st.success("分析完成！")
                                st.rerun()
                            else:
                                st.error("分析失败，请检查LLM服务是否正常")


def render_filter_page():
    st.markdown('<p class="sub-header">🏷️ 标签筛选</p>', unsafe_allow_html=True)
    
    raw_problems = load_raw_problems()
    
    if not raw_problems:
        st.error("未找到原始题目数据！")
        return
    
    analyzed_problems = st.session_state.analyzed_problems
    merged_problems = merge_problem_with_analysis(raw_problems, analyzed_problems)
    
    analyzed_only = [ap for ap in merged_problems if ap.is_analyzed()]
    
    if not analyzed_only:
        st.warning("暂无已分析的题目，请先分析题目！")
        return
    
    all_tags = get_all_tags(analyzed_only)
    
    col1, col2, col3 = st.columns(3)
    
    with col1:
        st.markdown("#### 算法标签")
        selected_algorithms = st.multiselect(
            "选择算法",
            all_tags["algorithms"],
            key="filter_algorithms"
        )
    
    with col2:
        st.markdown("#### 数据结构")
        selected_data_structures = st.multiselect(
            "选择数据结构",
            all_tags["data_structures"],
            key="filter_data_structures"
        )
    
    with col3:
        st.markdown("#### 核心概念")
        selected_concepts = st.multiselect(
            "选择概念",
            all_tags["concepts"],
            key="filter_concepts"
        )
    
    st.markdown("---")
    
    if selected_algorithms or selected_data_structures or selected_concepts:
        filtered = filter_problems_by_tags(
            analyzed_only,
            algorithms=selected_algorithms if selected_algorithms else None,
            data_structures=selected_data_structures if selected_data_structures else None,
            concepts=selected_concepts if selected_concepts else None
        )
        
        st.markdown(f"#### 筛选结果 ({len(filtered)} 道题目)")
        
        for ap in filtered:
            with st.container():
                col1, col2 = st.columns([4, 1])
                with col1:
                    st.markdown(
                        f"**P{ap.problem.problem_id}** - {ap.problem.title}"
                    )
                    st.markdown(
                        f"难度: {ap.analysis.difficulty} | 通过率: {ap.problem.get_acceptance_rate():.2f}%"
                    )
                with col2:
                    st.link_button(
                        "查看原题",
                        ap.problem.get_hduoj_url(),
                        use_container_width=True
                    )
                st.markdown("---")
    else:
        st.info("请选择至少一个标签进行筛选")
        
        st.markdown("#### 所有已分析题目")
        for ap in analyzed_only:
            col1, col2 = st.columns([4, 1])
            with col1:
                st.markdown(
                    f"**P{ap.problem.problem_id}** - {ap.problem.title}"
                )
                st.markdown(
                    f"难度: {ap.analysis.difficulty} | 通过率: {ap.problem.get_acceptance_rate():.2f}%"
                )
            with col2:
                st.link_button(
                    "查看原题",
                    ap.problem.get_hduoj_url(),
                    use_container_width=True
                )


def render_graph_page():
    st.markdown('<p class="sub-header">🕸️ 知识图谱</p>', unsafe_allow_html=True)
    
    raw_problems = load_raw_problems()
    
    if not raw_problems:
        st.error("未找到原始题目数据！")
        return
    
    analyzed_problems = st.session_state.analyzed_problems
    merged_problems = merge_problem_with_analysis(raw_problems, analyzed_problems)
    
    analyzed_only = [ap for ap in merged_problems if ap.is_analyzed()]
    
    if not analyzed_only:
        st.warning("暂无已分析的题目，请先分析题目！")
        return
    
    kg = st.session_state.knowledge_graph
    kg.build_graph(analyzed_only)
    
    stats = kg.get_statistics()
    
    col1, col2, col3, col4, col5 = st.columns(5)
    with col1:
        st.metric("题目节点", stats["problem"])
    with col2:
        st.metric("算法节点", stats["algorithm"])
    with col3:
        st.metric("数据结构节点", stats["data_structure"])
    with col4:
        st.metric("概念节点", stats["concept"])
    with col5:
        st.metric("关系边", stats["total_edges"])
    
    st.markdown("---")
    
    st.markdown("#### 图例说明")
    legend_col1, legend_col2, legend_col3, legend_col4 = st.columns(4)
    with legend_col1:
        st.markdown(
            f'<span style="background-color:{config.GRAPH_CONFIG["node_colors"]["problem"]};padding:0.25rem 0.5rem;border-radius:5px;color:white;">题目节点</span>',
            unsafe_allow_html=True
        )
    with legend_col2:
        st.markdown(
            f'<span style="background-color:{config.GRAPH_CONFIG["node_colors"]["algorithm"]};padding:0.25rem 0.5rem;border-radius:5px;color:white;">算法节点</span>',
            unsafe_allow_html=True
        )
    with legend_col3:
        st.markdown(
            f'<span style="background-color:{config.GRAPH_CONFIG["node_colors"]["data_structure"]};padding:0.25rem 0.5rem;border-radius:5px;color:white;">数据结构节点</span>',
            unsafe_allow_html=True
        )
    with legend_col4:
        st.markdown(
            f'<span style="background-color:{config.GRAPH_CONFIG["node_colors"]["concept"]};padding:0.25rem 0.5rem;border-radius:5px;color:white;">概念节点</span>',
            unsafe_allow_html=True
        )
    
    st.markdown("---")
    
    with st.spinner("正在生成知识图谱..."):
        graph_file = kg.visualize()
    
    st.markdown("#### 知识图谱可视化")
    st.info("💡 提示: 可以拖拽节点、滚轮缩放，点击题目节点可跳转到原题")
    
    with open(graph_file, "r", encoding="utf-8") as f:
        html_content = f.read()
    
    components.html(html_content, height=750, scrolling=True)
    
    st.markdown("---")
    
    st.markdown("#### 相关题目推荐")
    problem_id = st.number_input(
        "输入题目ID查看相关题目",
        min_value=1000,
        max_value=9999,
        value=2000,
        step=1
    )
    
    if st.button("查找相关题目"):
        related = kg.get_related_problems(problem_id)
        if related:
            st.markdown(f"**与 P{problem_id} 相关的题目**:")
            for rel in related:
                col1, col2 = st.columns([3, 1])
                with col1:
                    st.markdown(f"- {rel['label']}")
                with col2:
                    st.link_button("查看", rel["url"], use_container_width=True)
        else:
            st.info(f"未找到与 P{problem_id} 相关的题目，可能该题目尚未分析")


def main():
    st.sidebar.markdown("# 🎓 杭电OJ算法知识图谱")
    st.sidebar.markdown("---")
    
    page = st.sidebar.radio(
        "导航",
        ["🏠 首页", "📊 分析数据", "📝 题目列表", "🏷️ 标签筛选", "🕸️ 知识图谱"],
        label_visibility="collapsed"
    )
    
    st.sidebar.markdown("---")
    current_provider = st.session_state.llm_provider
    provider_config = config.LLM_CONFIG.get(current_provider, {})
    st.sidebar.markdown(f"**提供商**: {current_provider.upper()}")
    st.sidebar.markdown(f"**模型**: {provider_config.get('model', 'N/A')}")
    
    if page == "🏠 首页":
        render_home_page()
    elif page == "📊 分析数据":
        render_analysis_page()
    elif page == "📝 题目列表":
        render_problems_page()
    elif page == "🏷️ 标签筛选":
        render_filter_page()
    elif page == "🕸️ 知识图谱":
        render_graph_page()


if __name__ == "__main__":
    main()
