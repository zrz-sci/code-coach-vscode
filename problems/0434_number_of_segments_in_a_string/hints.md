# 434. Number of Segments in a String

## 核心思路
统计字符串中由非空格字符组成的连续片段数量。关键在于检测 **"段的开头"**：当前字符非空格且前一个字符是空格（或当前是第一个字符）。

## 思维链
1. 一个"段"的开始条件：当前字符非空格，且（前一个是空格 或 当前是位置 0）
2. 遍历一次字符串，每遇到一个段的开头就 count++
3. 无需真正分割字符串，一次遍历 O(n) 完成

## 解法概览表

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| ⭐ 一次遍历检测段开头 | O(n) | O(1) | 最优解 |
| istringstream 分词 | O(n) | O(n) | 利用标准库 |

## 关键提示
1. **段开头判断**：`s[i] != ' ' && (i == 0 || s[i-1] == ' ')`
2. **边界情况**：空字符串返回 0；全空格字符串返回 0
3. **不要用 split**：面试中通常期望手动实现而非调库

## 解法详解

### 解法一：一次遍历检测段开头 ⭐

**思路**：遍历每个字符，当遇到非空格且前一个字符是空格（或 i==0）时，计数 +1。

```cpp
int countSegments(string s) {
    int count = 0;
    for (int i = 0; i < s.size(); i++) {
        if (s[i] != ' ' && (i == 0 || s[i-1] == ' ')) {
            count++;
        }
    }
    return count;
}
```

**复杂度**：时间 O(n)，空间 O(1)

### 解法二：istringstream 分词

```cpp
int countSegments(string s) {
    istringstream iss(s);
    string word;
    int count = 0;
    while (iss >> word) count++;
    return count;
}
```

## 易错点
- ✗ 用 `s.split(" ")` 然后数非空元素 → 连续空格会产生空字符串 → ✓ 手动遍历更可靠
- ✗ 忘记处理空字符串或全空格 → ✓ 条件判断自然处理了这些情况
- ✗ 用 `s[i] != ' ' && s[i-1] == ' '` 但 i=0 时越界 → ✓ 加 `i == 0` 条件

## 面试追问

**Q1: 如果空格不只是 ' '，还包括 \t \n 等空白字符怎么办？**
用 `isspace(s[i])` 代替 `s[i] != ' '`，可以处理所有空白字符。

**Q2: 如何同时返回每个段的内容？**
记录段开始位置，遇到段结束时用 `substr` 提取。或用双指针标记每个段的起止。

**Q3: 如果字符串非常长（10^9 级别），有什么注意事项？**
算法本身是 O(n) 已最优。注意 string 的内存分配，可以用流式读取避免一次性加载。

## 相关题型
- [58. Length of Last Word](https://leetcode.com/problems/length-of-last-word/) - 最后一个单词长度
- [151. Reverse Words in a String](https://leetcode.com/problems/reverse-words-in-a-string/) - 翻转单词
- [557. Reverse Words in a String III](https://leetcode.com/problems/reverse-words-in-a-string-iii/) - 单词内翻转
