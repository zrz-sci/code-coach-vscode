# 195. 第十行 (Tenth Line)

**难度**: Easy | **标签**: Shell

## 核心思路

从文本文件中提取第10行 —— 本质是考察 Shell 文本处理工具（sed、awk、head+tail）的基本用法。

## 思维链

1. 文件逐行读取，只需要第10行
2. 可以用行号直接定位（sed/awk），也可以用管道截取范围（head+tail）
3. 需要考虑边界情况：文件不足10行时不输出

## 解法概览

| 解法 | 思路 | 推荐 |
|------|------|------|
| sed | `sed -n '10p'` 直接打印第10行 | ⭐ |
| awk | `awk 'NR==10'` 按行号过滤 | ⭐ |
| head + tail | 先取前10行再取最后1行 | |
| read 循环 | bash 内置 read 逐行计数 | |

## 关键提示

1. `sed -n` 的 `-n` 表示静默模式，只打印匹配行
2. `awk` 的 `NR` 是内置变量，代表当前行号
3. `head -n 10 | tail -n 1` 需注意文件不足10行时 tail 仍会输出最后一行（错误）
4. 使用 `wc -l` 可以预检查行数

## 解法详解

### 解法一：sed（推荐）

```bash
sed -n '10p' file.txt
```

**原理**：`-n` 关闭自动打印，`10p` 仅打印第10行。文件不足10行时自动无输出。

### 解法二：awk

```bash
awk 'NR==10' file.txt
```

**原理**：`NR` (Number of Records) 匹配到第10行时执行默认的 print 操作。

### 解法三：head + tail（需额外处理边界）

```bash
# 简单版（文件不足10行时可能有误）
head -n 10 file.txt | tail -n 1

# 安全版
line_count=$(wc -l < file.txt)
if [ "$line_count" -ge 10 ]; then
    head -n 10 file.txt | tail -n 1
fi
```

### 解法四：bash read 循环

```bash
cnt=0
while IFS= read -r line; do
    cnt=$((cnt + 1))
    if [ $cnt -eq 10 ]; then
        echo "$line"
        break
    fi
done < file.txt
```

## 易错点

1. ✗ `head -n 10 file.txt | tail -n 1` 当文件只有5行时输出第5行
   ✓ 需要先检查行数或使用 sed/awk（自动处理）

2. ✗ `awk '{if(NR=10) print}'` 误用 `=`（赋值）
   ✓ `awk '{if(NR==10) print}'` 或 `awk 'NR==10'`

3. ✗ 忘记 `sed` 的 `-n` 标志导致所有行都输出
   ✓ `sed -n '10p'`

## 面试追问

**Q1**: sed 和 awk 的区别是什么？各自适合什么场景？

**Q2**: 如果要打印第10到第20行，如何修改？
→ `sed -n '10,20p'` 或 `awk 'NR>=10 && NR<=20'`

**Q3**: 如果文件非常大（几十GB），哪种方式最高效？
→ `sed '10q;d'` 读到第10行后立即退出，避免扫描全文件

## 相关题型

- 192: Word Frequency（Shell + awk/sort/uniq）
- 193: Valid Phone Numbers（Shell + grep/sed 正则）
- 194: Transpose File（Shell + awk 矩阵转置）
