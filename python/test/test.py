import requests
import time
from bs4 import BeautifulSoup
import pandas as pd
import os

def ExtractReplies(htmlContent):
    """提取回复数据并返回结构化DataFrame"""
    soup = BeautifulSoup(htmlContent, 'html.parser')
    ol = soup.find('ol', {'class': 'list-unstyled'})
    
    data = []
    if ol:
        for li in ol.find_all('li', {'class': 'mb-1'}):
            # 提取贴吧名（第一个<a>标签）
            barLinks = li.find_all('a')
            barName = barLinks[0].text.strip() if barLinks else "未知贴吧"
            
            # 提取帖子标题（第二个<a>标签）
            postTitle = barLinks[1].text.strip() if len(barLinks) > 1 else "无标题"
            
            # 提取回复内容（:之后的文本）
            textParts = []
            for element in li.contents:
                if isinstance(element, str):
                    textParts.append(element.strip())
                elif element.name == 'br':
                    textParts.append('\n')
            
            rawContent = ''.join(textParts)
            replyContent = rawContent.split('：', 1)[-1].strip() if '：' in rawContent else rawContent
            
            # 提取时间
            timeSpan = li.find('span', {'class': 'text-muted'})
            timeStr = timeSpan.text.strip() if timeSpan else "未知时间"
            
            data.append([replyContent, barName, postTitle, timeStr])
    
    return pd.DataFrame(data, columns=['回复内容', '所在贴吧', '回复的帖子标题', '回复时间'])

# 主抓取函数
def ScrapeAndParsePages(baseUrl, startPage, endPage, outputCsv="all_replies.csv"):
    headers = {
        "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36",
        "Referer": "https://www.82cat.com/tieba/reply/",
        "Accept-Language": "zh-CN,zh;q=0.9,en;q=0.8",
        "Connection": "keep-alive"
    }
    
    # 创建存储目录
    os.makedirs("html_pages", exist_ok=True)
    
    # 初始化结果DataFrame
    allData = pd.DataFrame()
    
    for page in range(startPage, endPage + 1):
        url = f"{baseUrl}{page}"
        print(f"正在抓取第 {page} 页: {url}")
        
        try:
            # 1. 抓取页面
            response = requests.get(url, headers=headers, timeout=15)
            response.raise_for_status()
            
            # 2. 保存HTML文件
            html_path = f"html_pages/page_{page}.html"
            with open(html_path, "w", encoding="utf-8") as f:
                f.write(response.text)
            
            # 3. 解析内容
            page_data = ExtractReplies(response.text)
            
            if not page_data.empty:
                # 添加页码信息
                page_data.insert(0, '页码', page)
                # 添加到总数据
                allData = pd.concat([allData, page_data], ignore_index=True)
                print(f"  解析成功: 获取到 {len(page_data)} 条回复")
            else:
                print("  警告: 未解析到回复数据")
            
            # 4. 实时保存结果
            if page % 10 == 0 or page == endPage:
                allData.to_csv(outputCsv, index=False, encoding='utf_8_sig')
                print(f"  已保存检查点数据到 {outputCsv}")
                
        except requests.exceptions.RequestException as e:
            print(f"  抓取失败: {str(e)}")
        except Exception as e:
            print(f"  处理异常: {str(e)}")
        
        # 礼貌性延迟，避免被封IP
        time.sleep(1.5 + page % 3)  # 动态延迟1.5-4.5秒
    
    print(f"全部页面抓取完成！共获取 {len(allData)} 条回复")
    return allData

# 使用示例
if __name__ == "__main__":
    baseUrl = "https://www.82cat.com/tieba/reply/文化大盗亻委奴/"
    result = ScrapeAndParsePages(
        baseUrl=baseUrl,
        startPage=1,
        endPage=155,  # 根据实际调整页数
        outputCsv="文化大盗亻委奴_贴吧回复.csv"
    )
    
    # 最终保存结果
    result.to_csv("文化大盗亻委奴_贴吧回复.csv", index=False, encoding='utf_8_sig')
    print("数据已保存为CSV文件")