# 导入我们安装好的 requests 库
import requests

# 1. 设置你想要抓取的网页地址（URL）
url = 'https://www.baidu.com'

# 2. 使用 requests 的 get 方法，向目标地址发送一个 HTTP GET 请求
#    这相当于你在浏览器地址栏输入网址并按下回车
response = requests.get(url)

# 3. 检查请求是否成功
#    状态码 200 表示请求成功，其他常见代码有 404（页面未找到）、500（服务器错误）等
print('状态码:', response.status_code)
if response.status_code == 200:
    print('请求成功！')
else:
    print('请求可能遇到了问题。')

# 4. 打印获取到的网页源代码
#    `.text` 属性包含了服务器返回的网页HTML文本内容
print('网页源代码的前1000个字符：')
print(response.text[:1000])  # 这里只打印前1000字符，避免控制台输出过长
