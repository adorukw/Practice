with open('pronnameW.txt', 'r',encoding='utf-8') as f1, open('pronname.txt', 'r',encoding='utf-8') as f2:
    # 读取文件内容并转换为集合
    set1 = set(line.strip() for line in f1)
    set2 = set(line.strip() for line in f2)
    
    # 找出交集（重复行）
    duplicates = set1 & set2

# 将重复行写入新文件
with open('duplicates.txt', 'w') as out:
    for line in duplicates:
        out.write(line + '\n')