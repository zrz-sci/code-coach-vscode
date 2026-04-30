# 192. 统计词频

## 核心思路

本质上就是一个**文本处理统计**的问题：读取文件中的所有单词，统计每个单词出现次数，按频率降序输出。Shell的管道哲学（每个工具做一件事，串联起来）在这里完美适用。

## 思维链

1. **读完题第一反应** → 需要三步：① 把文件中每个单词拆成单独一行 ② 排序+去重计数 ③ 按计数排序后输出。
2. **怎么把单词拆到每行？** → `tr -s ' ' '\n'` 或 `xargs -n1` 或 `awk '{for(i=1;i<=NF;i++) print $i}'`。
3. **怎么统计频率？** → 经典组合：`sort | uniq -c`，先排序把相同单词聚在一起，再用 uniq 计数。
4. **怎么按频率降序？** → `sort -rn`（-r 降序，-n 数值排序）。
5. **输出格式对不对？** → `uniq -c` 输出格式是 "count word"，题目要求 "word count"，需要 `awk '{print $2, $1}'` 调换顺序。

## 解法概览

| 解法 | 思路 | 工具组合 | 面试 |
|------|------|---------|------|
| tr + sort + uniq + awk | 管道流经典组合 | tr / sort / uniq -c / sort -rn / awk | ⭐ 必须写出 |
| awk 数组统计 | awk 关联数组计数 + END排序 | awk + sort | 加分项 |
| sed + sort | sed 替换空格为换行 | sed / sort / uniq | 可选 |

## 关键提示

1. **`tr -s ' ' '\n'`**：`-s` 表示 squeeze（压缩），多个连续空格只当一个处理。如果不用 `-s`，多个空格会产生空行。
2. **`sort | uniq -c`**：`uniq` 只能去除**相邻**的重复行，所以必须先 `sort`。这是 Shell 面试的常见陷阱。
3. **`sort -rn`**：`-n` 是数值排序（不是字典序），`-r` 是逆序。没有 `-n` 时 "9" 会排在 "10" 后面。
4. **空行处理**：`tr` 产生的空行可以用 `grep -v '^$'` 或在 awk 中用 `NF` 过滤（空行的 NF=0）。

## 解法详解

### 解法1: tr + sort + uniq + awk — 管道经典 ⭐

**管道数据流**:
```
文件内容 → 每词一行 → 排序 → 计数 → 按数量降序 → 调换列顺序

the day is sunny the the    tr -s ' ' '\n'     sort      uniq -c    sort -rn    awk
the sunny is is          ──────────────→ day  ──────→  3 is    ──────→  4 the  ──────→ the 4
                             day           is      2 sunny     3 is      is 3
                             is            is      2 the       2 sunny   sunny 2
                             is            sunny   4 the       1 day     day 1
                             sunny         sunny   1 day
                             the           the
                             the           the
                             the           the
                             the
```

```bash
# 解法1: 经典管道一行解
cat words.txt | tr -s ' ' '\n' | sort | uniq -c | sort -rn | awk '{print $2, $1}'
```

---

### 解法2: awk 关联数组 — 更灵活

**思路**: awk 内置关联数组（类似Python dict），遍历每个单词直接计数，最后排序输出。

```bash
# 解法2: awk 数组统计
awk '{for(i=1;i<=NF;i++) count[$i]++} END {for(w in count) print w, count[w]}' words.txt | sort -k2 -rn
```

---

### 解法3: xargs + sort + uniq

```bash
# 解法3: xargs 拆词
cat words.txt | xargs -n1 | sort | uniq -c | sort -rn | awk '{print $2, $1}'
```

## 易错点

1. **uniq 前忘记 sort**：
   - ✗ `cat words.txt | tr -s ' ' '\n' | uniq -c` → uniq 只去相邻重复，不排序的话相同单词不相邻，计数错误
   - ✓ `... | sort | uniq -c` → 先排序确保相同单词相邻

2. **多空格产生空行**：
   - ✗ `tr ' ' '\n'` → 多个连续空格变成多个空行，空行也被统计
   - ✓ `tr -s ' ' '\n'` → `-s` 压缩连续分隔符

3. **sort 不加 -n 导致字典序**：
   - ✗ `sort -r` → "9" 排在 "10" 后面（字典序比较字符 '9' > '1'）
   - ✓ `sort -rn` → 数值排序，10 > 9

4. **输出格式不对**：
   - ✗ 直接输出 `uniq -c` 的结果 → 格式是 "  4 the"（带前导空格，count在前）
   - ✓ `awk '{print $2, $1}'` → 调换为 "the 4"

## 面试追问

**Q1: 能用一行命令实现吗？**
> `cat words.txt | tr -s ' ' '\n' | sort | uniq -c | sort -rn | awk '{print $2,$1}'`。这就是 Unix 管道哲学的精华：每个工具做一件小事，组合起来完成复杂任务。

**Q2: 如果文件非常大（几十GB），怎么优化？**
> `sort` 是瓶颈（需要外部排序）。可以用 `awk` 的关联数组在内存中一次遍历计数，避免排序。或者用 `sort --parallel=N` 利用多核加速。

**Q3: 如果要处理大小写不敏感的词频呢？**
> 在管道最前面加 `tr 'A-Z' 'a-z'` 把所有字母转小写，再统计。

## 相关题型

- **193. 有效电话号码** — 同样是 Shell 文本处理，用 grep/awk/sed 做正则匹配。
- **194. 转置文件** — Shell 矩阵操作，用 awk 处理列转行。
- **195. 第十行** — 简单的行选取，`sed -n '10p'` 或 `awk 'NR==10'`。
