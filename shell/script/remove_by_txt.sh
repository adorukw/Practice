#!/bin/bash

if [ $# -ne 2 ]; then
  echo "用法: $0 <目录路径> <文本文件>"
  exit 1
fi

dir="$1"
txt_file="$2"

if [ ! -d "$dir" ]; then
  echo "错误：目录 $dir 不存在"
  exit 1
fi

if [ ! -f "$txt_file" ]; then
  echo "错误：文本文件 $txt_file 不存在"
  exit 1
fi

while IFS= read -r filename; do
  # 关键修复：增加 tr -d '\r' 删除回车符
  clean_name=$(echo "$filename" | sed 's/^[ \t]*//;s/[ \t]*$//' | tr -d '\r')
  
  [ -z "$clean_name" ] && continue

  target_file="${dir%/}/$clean_name"

  if [ -e "$target_file" ]; then
    echo "删除文件: $target_file"
    rm -f -- "$target_file"
  else
    echo "文件不存在（跳过）: $target_file"
  fi
done < "$txt_file"

echo "操作完成"
