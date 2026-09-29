import requests
from bs4 import BeautifulSoup
import json
import time
import os


def fetch_hdu_problem(problem_id, output_file="problems.txt"):
    """
    爬取HDU OJ指定题号的问题信息，并追加到文件中

    Args:
        problem_id: 题目ID（如2023）
        output_file: 输出文件名，默认为"problems.txt"
    """
    # 1. 构建URL
    BASE_URL = f"https://acm.hdu.edu.cn/showproblem.php?pid={problem_id}"
    headers = {
        "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/114.0.0.0 Safari/537.36"
    }

    print(f"正在爬取题目 {problem_id}...")

    # 2. 发送请求
    try:
        response = requests.get(BASE_URL, headers=headers, timeout=10)
        response.raise_for_status()  # 如果请求失败，抛出异常
    except requests.RequestException as e:
        print(f"爬取题目 {problem_id} 失败: {e}")
        return None

    # 3. 解析HTML
    soup = BeautifulSoup(response.text, 'html.parser')

    # 4. 提取题目信息
    problem_info = {"problem_id": problem_id}

    # 提取标题
    title_tag = soup.find('h1')
    if title_tag:
        problem_info['title'] = title_tag.get_text(strip=True)

    # 提取时间限制、内存限制、提交/通过数
    limit_span = soup.find('span', style=lambda s: s and 'color:green' in s)
    if limit_span:
        lines = [line.strip() for line in limit_span.get_text(
            strip=True, separator='\n').split('\n') if line.strip()]
        for line in lines:
            if 'Time Limit:' in line:
                problem_info['time_limit'] = line.split(
                    'Time Limit:')[-1].strip()
            elif 'Memory Limit:' in line:
                problem_info['memory_limit'] = line.split(
                    'Memory Limit:')[-1].strip()
            elif 'Total Submission(s):' in line:
                parts = line.split('Accepted Submission(s):')
                problem_info['total_submissions'] = parts[0].split(
                    'Total Submission(s):')[-1].strip()
                problem_info['accepted_submissions'] = parts[1].strip()

    # 提取问题描述、输入、输出、样例等
    sections = ['Problem Description', 'Input', 'Output',
                'Sample Input', 'Sample Output', 'Author', 'Source']
    for section in sections:
        title_div = soup.find('div', class_='panel_title',
                              string=lambda t: t and section in t)
        if title_div:
            content_div = title_div.find_next_sibling(
                'div', class_='panel_content')
            if content_div:
                if 'Sample' in section:
                    pre_tag = content_div.find('pre')
                    problem_info[section.lower().replace(' ', '_')] = pre_tag.get_text(
                    ) if pre_tag else content_div.get_text(strip=True)
                else:
                    problem_info[section.lower().replace(
                        ' ', '_')] = content_div.get_text(strip=True)

    # 5. 将题目信息追加到文件中
    try:
        with open(output_file, 'a', encoding='utf-8') as f:
            # 将字典转换为JSON格式写入文件
            json.dump(problem_info, f, ensure_ascii=False, indent=2)
            f.write('\n,\n')  # 添加逗号和换行，便于后续解析

        print(f"题目 {problem_id} 已成功保存到 {output_file}")

        # 在控制台显示简要信息
        print(f"\n=== 题目 {problem_id} 简要信息 ===")
        print(f"标题: {problem_info.get('title', 'N/A')}")
        print(f"时间限制: {problem_info.get('time_limit', 'N/A')}")
        print(f"内存限制: {problem_info.get('memory_limit', 'N/A')}")
        print(
            f"提交/通过: {problem_info.get('total_submissions', 'N/A')}/{problem_info.get('accepted_submissions', 'N/A')}")

        return problem_info

    except IOError as e:
        print(f"写入文件 {output_file} 失败: {e}")
        return None


def batch_fetch_problems(start_id, end_id, output_file="problems.txt"):
    """
    批量爬取指定范围内的题目

    Args:
        start_id: 起始题号
        end_id: 结束题号
        output_file: 输出文件名
    """
    for pid in range(start_id, end_id + 1):
        fetch_hdu_problem(pid, output_file)


def read_problems_from_file(filename="problems.txt"):
    """
    从文件中读取已爬取的题目信息

    Args:
        filename: 文件名

    Returns:
        题目信息列表
    """
    if not os.path.exists(filename):
        print(f"文件 {filename} 不存在")
        return []

    try:
        with open(filename, 'r', encoding='utf-8') as f:
            content = f.read()
            # 将文件内容解析为JSON列表
            # 注意：由于我们写入时每个题目后面有",\n"，需要特殊处理
            json_str = '[' + content.rstrip(',\n') + ']'
            problems = json.loads(json_str)
            return problems
    except Exception as e:
        print(f"读取文件 {filename} 失败: {e}")
        return []


def search_problems(filename="problems.txt", keyword=None, min_accepted=None):
    """
    搜索已爬取的题目

    Args:
        filename: 文件名
        keyword: 搜索关键字
        min_accepted: 最小通过人数
    """
    problems = read_problems_from_file(filename)

    if not problems:
        print("没有找到题目信息")
        return

    results = []
    for problem in problems:
        match = True

        if keyword:
            # 在标题和描述中搜索关键字
            title = problem.get('title', '').lower()
            description = problem.get('problem_description', '').lower()
            if keyword.lower() not in title and keyword.lower() not in description:
                match = False

        if min_accepted and match:
            try:
                accepted = int(problem.get(
                    'accepted_submissions', '0').replace(',', ''))
                if accepted < min_accepted:
                    match = False
            except ValueError:
                match = False

        if match:
            results.append(problem)

    print(f"\n找到 {len(results)} 个匹配的题目:")
    for i, problem in enumerate(results, 1):
        print(f"{i}. 题目ID: {problem.get('problem_id', 'N/A')}")
        print(f"   标题: {problem.get('title', 'N/A')}")
        print(
            f"   通过率: {problem.get('accepted_submissions', 'N/A')}/{problem.get('total_submissions', 'N/A')}")
        print()


# 主程序示例
if __name__ == "__main__":
    # # 示例1: 爬取单个题目
    # problem_info = fetch_hdu_problem(2023)

    # if problem_info:
    #     # 示例2: 批量爬取题目
    #     # 注意: 批量爬取时请设置合理的间隔，避免对服务器造成压力
    #     # batch_fetch_problems(1000, 1010, "hdu_problems.txt")

    #     # 示例3: 读取并搜索题目
    #     search_problems(keyword="平均成绩")

    # 示例4: 爬取多个指定题目
    for pid in range(1000,1100):
        fetch_hdu_problem(pid, "selected_problems.txt")
        time.sleep(3)  # 添加延迟，避免请求过快
