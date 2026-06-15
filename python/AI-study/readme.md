# AI Study

## 踩坑记录
### 1. seaborn设置样式会覆盖matplotlib设置样式，导致中文显示异常
解决方法：在设置seaborn样式之后单独设置中文字体或者使用matplotlib设置中文字体，如：
```python
plt.rcParams['font.sans-serif'] = ['WenQuanYi Zen Hei']
plt.rcParams['axes.unicode_minus'] = False
sns.set_style('whitegrid')
sns.set_theme(font=["WenQuanYi Zen Hei"])
sns.set_style({'axes.unicode_minus': False}) 
```

