import requests
from bs4 import BeautifulSoup

# 1. 获取网页源代码（复习上一讲内容）
url = 'https://httpbin.org/html'  # 一个提供示例HTML的测试网站
response = requests.get(url)
html_content = response.text

# 2. 创建BeautifulSoup对象，用于解析
#    第一个参数是待解析的HTML文本，第二个参数是解析器，这里使用Python内置的'html.parser'
soup = BeautifulSoup(html_content, 'html.parser')

# 3. 使用 find() 方法提取“第一个”匹配的元素
#    这里我们提取第一个 <h1> 标签
main_title = soup.find('h1')
print("找到的h1标签对象:", main_title)
print("h1标签内的文本:", main_title.text)  # 使用 .text 属性获取标签内的纯文本
print("---")

# 4. 使用 find_all() 方法提取“所有”匹配的元素
#    这里我们提取所有的 <p> (段落) 标签
all_paragraphs = soup.find_all('p')
print(f"一共找到了 {len(all_paragraphs)} 个段落(p)标签。")
for i, p in enumerate(all_paragraphs):  # enumerate用于获取索引
    print(f"第{i+1}个段落：{p.text}")
