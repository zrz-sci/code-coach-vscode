# 186. 反转字符串中的单词 II (Medium)

## 核心思路

原地反转单词顺序。经典的 **两次翻转法**：先翻转整个数组，再逐个翻转每个单词。

## 思维链

1. "the sky is blue" → "blue is sky the"
2. 单词顺序反了，但每个单词内部字母顺序不变
3. 如果整体翻转："eulb si yks eht"
4. 再把每个单词翻转回来："blue is sky the" → 完成！
5. 要求原地 O(1) 空间 → 两次翻转法完美满足

## 解法概览

| 解法 | 思路 | 时间复杂度 | 空间复杂度 | 推荐度 |
|------|------|-----------|-----------|--------|
| 两次翻转法 | 整体翻转 + 逐词翻转 | O(n) | O(1) | ⭐⭐⭐ |

## 关键提示

1. `reverse(s.begin(), s.end())` 翻转整个数组
2. 遍历找空格，确定每个单词的边界 [start, end)
3. 对最后一个单词要特殊处理（后面没有空格）
4. 双指针翻转子数组：`reverse(s.begin()+start, s.begin()+end)`

## 解法详解

### 解法一：两次翻转法（推荐）
```cpp
class Solution {
public:
    void reverseWords(vector<char>& s) {
        // 第一步：翻转整个数组
        reverse(s.begin(), s.end());
        // 第二步：逐个翻转每个单词
        int n = s.size(), start = 0;
        for (int i = 0; i <= n; i++) {
            if (i == n || s[i] == ' ') {
                reverse(s.begin() + start, s.begin() + i);
                start = i + 1;
            }
        }
    }
};
```

## 易错点

- ✗ 只翻转整个数组，忘记逐词翻转 → 每个单词内部字母也是反的
  → ✓ 必须两步：整体翻转 + 逐词翻转

- ✗ 遍历到 `i < n` 而非 `i <= n`，漏掉最后一个单词
  → ✓ 循环条件 `i <= n`，用 `i == n` 触发最后一个单词的翻转

- ✗ start 指针没有在每个空格后更新
  → ✓ 每次遇到空格或末尾时翻转 [start, i)，然后 start = i + 1

## 面试追问

**Q1：能否先逐词翻转再整体翻转？**
→ 可以！顺序不影响最终结果。两种顺序都能得到正确答案。

**Q2：与 151. Reverse Words in a String 有什么区别？**
→ 151 题输入是 string，可能有前后空格和多余空格，需要额外处理。186 题输入是 char 数组，保证无多余空格，更纯粹地考察翻转技巧。

**Q3：如果允许 O(n) 额外空间，有更简单的方法吗？**
→ 可以用栈：把单词依次压栈，再弹出即为反序。或者直接 split + reverse + join。

## 相关题型

- [151. 反转字符串中的单词](https://leetcode.com/problems/reverse-words-in-a-string/) - 有多余空格的版本
- [344. 反转字符串](https://leetcode.com/problems/reverse-string/) - 基础翻转
- [557. 反转字符串中的单词 III](https://leetcode.com/problems/reverse-words-in-a-string-iii/) - 只翻转单词内部
- [189. 轮转数组](https://leetcode.com/problems/rotate-array/) - 同样用两次翻转
