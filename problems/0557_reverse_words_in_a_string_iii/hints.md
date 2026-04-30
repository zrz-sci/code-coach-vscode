# 557. Reverse Words in a String III - 反转字符串中的单词 III

## 核心思路
按空格分割单词，逐个反转每个单词内的字符，保持单词顺序不变。

## 思维链
1. 如何定位每个单词的边界？ → 用双指针找空格分隔
2. 如何原地反转单词？ → 对每个单词区间做 reverse
3. 需要额外空间吗？ → 可以 O(1) 原地操作

## 解法概览表

| 解法 | 时间 | 空间 | 特点 |
|------|------|------|------|
| ⭐ 双指针原地反转 | O(n) | O(1) | 最优，面试首选 |
| istringstream 分割 | O(n) | O(n) | 代码简洁，但需额外空间 |

## 关键提示
1. 双指针 `i` 和 `j` 分别标记单词的起止位置
2. 遇到空格或末尾时，对 `[i, j-1]` 执行 reverse
3. 可以直接用 `std::reverse(s.begin()+i, s.begin()+j)`

## 解法详解

### 解法1: 双指针原地反转 ⭐
```cpp
class Solution {
public:
    string reverseWords(string s) {
        int n = s.size();
        int i = 0;
        for (int j = 0; j <= n; j++) {
            if (j == n || s[j] == ' ') {
                reverse(s.begin() + i, s.begin() + j);
                i = j + 1;
            }
        }
        return s;
    }
};
```

### 解法2: istringstream 分割
```cpp
class Solution {
public:
    string reverseWords(string s) {
        istringstream iss(s);
        string word, result;
        while (iss >> word) {
            reverse(word.begin(), word.end());
            if (!result.empty()) result += ' ';
            result += word;
        }
        return result;
    }
};
```

## 易错点
- ✗ 遗漏最后一个单词（末尾无空格） → ✓ 循环条件 `j <= n`，在 `j == n` 时也触发反转
- ✗ 把整个字符串反转了 → ✓ 只反转每个单词内部字符，单词顺序不变
- ✗ reverse 区间写错 → ✓ `reverse(begin+i, begin+j)` 是左闭右开区间

## 面试追问

**Q1: 如果要求反转单词顺序（而非单词内部），怎么做？**
先整体反转，再逐个单词反转（即 LC 151）。

**Q2: 如果字符串有多个连续空格怎么处理？**
双指针法天然兼容：遇到空格跳过即可，`i = j + 1` 会跳到下一个非空格字符。

**Q3: 能否不用 STL reverse，手写反转？**
可以，用两个指针从两端向中间交换：`while (l < r) swap(s[l++], s[r--]);`

## 相关题型
- [151. Reverse Words in a String](https://leetcode.com/problems/reverse-words-in-a-string/) - 反转单词顺序
- [186. Reverse Words in a String II](https://leetcode.com/problems/reverse-words-in-a-string-ii/) - 原地反转
- [344. Reverse String](https://leetcode.com/problems/reverse-string/) - 反转整个字符串
