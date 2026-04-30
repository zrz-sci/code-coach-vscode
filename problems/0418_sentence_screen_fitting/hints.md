# 418. Sentence Screen Fitting (屏幕可显示句子的数量)

## 核心思路
将句子拼成一个用空格分隔的循环字符串 `s = "hello world "`，然后模拟每行能放多少字符。关键优化：用**字符串游标法**在 O(rows * cols / len) 内完成，或用**记忆化**对每个起始单词索引缓存该行结束后的状态。

## 思维链
1. 暴力：逐行逐词模拟放置 → 可能 TLE（rows 和 cols 最大 2*10^4）
2. 优化一：把句子拼成 `s = "w1 w2 ... wn "`，游标在 s 上滑动
3. 每行加 cols 个字符后，如果落在空格上直接换行，否则回退到上一个空格
4. 优化二：对每个起始单词位置 i，预计算从该位置开始一行后到达哪个单词、完成几轮句子
5. 因为起始位置只有 `sentence.length` 种，记忆化后只需 O(rows) 查表

## 解法概览表

| 解法 | 时间 | 空间 | 特点 |
|------|------|------|------|
| ⭐ 字符串游标法 | O(rows * (cols/len + 1)) | O(len) | 巧妙简洁 |
| 记忆化 DP | O(n + rows) | O(n) | 每个起始位置只算一次 |

## 关键提示
1. 拼接字符串 `s = "hello world "`，注意末尾也有空格
2. 游标法核心：`start += cols`，若 `s[start % len] == ' '` 则 `start++`，否则回退
3. 回退时 `while (start > 0 && s[(start-1) % len] != ' ') start--`
4. 最终 `start / len` 就是完成的句子轮数

## 解法详解

### 解法一：字符串游标法（推荐）
```cpp
class Solution {
public:
    int wordsTyping(vector<string>& sentence, int rows, int cols) {
        string s;
        for (auto& w : sentence) s += w + " ";
        int len = s.size(), start = 0;
        
        for (int i = 0; i < rows; i++) {
            start += cols;
            if (s[start % len] == ' ') {
                start++; // 刚好落在空格上，跳过
            } else {
                // 回退到上一个空格之后
                while (start > 0 && s[(start - 1) % len] != ' ') {
                    start--;
                }
            }
        }
        return start / len;
    }
};
```

### 解法二：记忆化 DP
```cpp
class Solution {
public:
    int wordsTyping(vector<string>& sentence, int rows, int cols) {
        int n = sentence.size();
        // dp[i] = {从第i个单词开始一行后，下一行起始单词索引, 本行完成了几轮句子}
        vector<pair<int,int>> dp(n);
        
        for (int i = 0; i < n; i++) {
            int cur = i, count = 0, remain = cols;
            while (remain >= (int)sentence[cur].size()) {
                remain -= sentence[cur].size();
                remain--; // 空格
                cur++;
                if (cur == n) { cur = 0; count++; }
            }
            dp[i] = {cur, count};
        }
        
        int total = 0, cur = 0;
        for (int i = 0; i < rows; i++) {
            total += dp[cur].second;
            cur = dp[cur].first;
        }
        return total;
    }
};
```

## 易错点
- ✗ 忘记句子末尾加空格 → ✓ `s = "hello world "` 末尾必须有空格
- ✗ 回退时用 `s[start % len]` 判断 → ✓ 应该看 `s[(start-1) % len]` 是否为空格
- ✗ 单词长度 > cols 时死循环 → ✓ 题目保证 `sentence[i].length <= 10 <= cols`，但仍需注意
- ✗ 记忆化 DP 中 remain 没减去空格 → ✓ 每放一个词后 `remain--` 扣掉空格

## 面试追问
**Q1: 为什么字符串游标法是正确的？**
A: 把句子看成无限循环的字符流，每行消耗 cols 个字符位置（加上可能的回退）。`start` 是全局消耗的字符数，`start / len` 就是完成的轮数。

**Q2: 时间复杂度分析？**
A: 游标法每行最多回退 max_word_length 次（最长10），所以是 O(rows * 10) = O(rows)。记忆化法预处理 O(n * cols)，查询 O(rows)。

**Q3: 如果句子非常长（10^5 个词），哪种方法更好？**
A: 游标法的循环字符串会很长但只做取模运算，仍然 O(rows)。记忆化法预处理 O(n * cols) 可能更慢。总体游标法更优。

## 相关题型
- [68. Text Justification](https://leetcode.com/problems/text-justification/) - 文本排版
- [1592. Rearrange Spaces Between Words](https://leetcode.com/problems/rearrange-spaces-between-words/) - 字符串排列
