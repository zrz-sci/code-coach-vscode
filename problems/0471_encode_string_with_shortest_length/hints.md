# 471. 编码最短长度的字符串

## 核心思路
使用区间DP，`dp[i][j]`表示子串`s[i..j]`的最短编码，通过"拼接取余"技巧发现重复模式，结合分割点枚举实现最优编码。

## 思维链

1. **问题分析**: 编码规则是`k[encoded_string]`，可以嵌套，需要找最短编码
2. **区间DP框架**: 定义dp[i][j]为s[i..j]的最短编码，从短子串递推到长子串
3. **发现重复模式**: 对子串t，将t+t拼接后从位置1查找t，若pos < len(t)，则t由长度pos的前缀重复len/pos次
4. **整体编码**: 若发现重复，编码为`k[dp[i][i+pos-1]]`，注意内部子串递归使用已编码的dp值
5. **分割优化**: 枚举分割点k，`dp[i][j] = dp[i][k] + dp[k+1][j]`取最短
6. **长度阈值**: 子串长度 <= 4时不可能编码更短（`k[x]`至少占4字符），直接用原串

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 区间DP+拼接取余 | dp[i][j]=min(整体编码, 分割) | O(n^3) | O(n^2) | ⭐ |

## 关键提示

1. **拼接取余技巧** (核心):
```
t = "abcabc"  (len=6)
t + t = "abcabcabcabc"
在 t+t 中从位置1查找 t:
  pos = 3   (< len=6)

含义: t 由 "abc"(长度3) 重复 6/3=2 次
编码: 2[abc]

t = "abcd"  (len=4)
t + t = "abcdabcd"
在 t+t 中从位置1查找 t:
  pos = 4   (= len=4)

含义: t 不能分解为重复模式
```

2. **DP转移图示**:
```
dp[i][j] 的三种来源:

1) 原串本身: s[i..j]  (不编码)
2) 整体编码: k[dp[i][i+pos-1]]  (若有重复模式)
3) 分割:     dp[i][mid] + dp[mid+1][j]  (枚举分割点)

取三者中最短的
```

3. **嵌套编码示例**:
```
s = "abbbabbbcabbbabbbc"

第一层: "abbbabbbc" 出现2次
第二层: "abbbabbbc" 中 "abbb" 出现2次

最终: 2[2[abbb]c]

DP过程:
  dp["abbb"] = "abbb"  (长度4，编码不更短)
  dp["abbbabbb"] = "2[abbb]"
  dp["abbbabbbc"] = "2[abbb]c"
  dp["abbbabbbcabbbabbbc"] = "2[2[abbb]c]"
```

4. **长度阈值4的原因**: 编码`k[x]`中，k至少1位，加上`[]`共3个额外字符。`2[a]`长4字符 = `"aa"`长2字符的编码并不更短（4>2），要到`5[a]`(4字符) vs `"aaaaa"`(5字符)才划算

5. **子串查找的时间**: `(t+t).find(t, 1)` 用KMP可以O(n)，实际用string::find一般也够快

## 解法详解

### 解法1: 区间DP + 拼接取余

```cpp
class Solution {
public:
    string encode(string s) {
        int n = s.size();
        vector<vector<string>> dp(n, vector<string>(n));

        // 枚举子串长度，从短到长
        for (int len = 1; len <= n; len++) {
            for (int i = 0; i + len - 1 < n; i++) {
                int j = i + len - 1;
                string sub = s.substr(i, len);
                dp[i][j] = sub;  // 默认不编码

                if (len <= 4) continue;  // 太短不可能编码更短

                // 尝试发现重复模式: 拼接取余技巧
                string doubled = sub + sub;
                int pos = doubled.find(sub, 1);
                if (pos < (int)sub.size()) {
                    // sub 由长度pos的前缀重复 len/pos 次
                    string candidate = to_string(len / pos)
                                     + "[" + dp[i][i + pos - 1] + "]";
                    if (candidate.size() < dp[i][j].size()) {
                        dp[i][j] = candidate;
                    }
                }

                // 尝试分割: 枚举所有分割点
                for (int k = i; k < j; k++) {
                    string combined = dp[i][k] + dp[k + 1][j];
                    if (combined.size() < dp[i][j].size()) {
                        dp[i][j] = combined;
                    }
                }
            }
        }
        return dp[0][n - 1];
    }
};
```

### 解法2: 带记忆化的递归写法

```cpp
class Solution {
public:
    string encode(string s) {
        int n = s.size();
        vector<vector<string>> memo(n, vector<string>(n));
        return solve(s, 0, n - 1, memo);
    }

    string solve(const string& s, int i, int j,
                 vector<vector<string>>& memo) {
        if (!memo[i][j].empty()) return memo[i][j];

        string sub = s.substr(i, j - i + 1);
        int len = j - i + 1;
        memo[i][j] = sub;  // 默认不编码

        if (len <= 4) return memo[i][j];

        // 检查重复模式
        string doubled = sub + sub;
        int pos = doubled.find(sub, 1);
        if (pos < len) {
            string candidate = to_string(len / pos)
                             + "[" + solve(s, i, i + pos - 1, memo) + "]";
            if (candidate.size() < memo[i][j].size()) {
                memo[i][j] = candidate;
            }
        }

        // 分割
        for (int k = i; k < j; k++) {
            string combined = solve(s, i, k, memo)
                            + solve(s, k + 1, j, memo);
            if (combined.size() < memo[i][j].size()) {
                memo[i][j] = combined;
            }
        }
        return memo[i][j];
    }
};
```

## 易错点

1. **拼接取余的find起始位置**:
```cpp
// ✗ 错误: 从位置0开始找，会立即找到自己
int pos = doubled.find(sub, 0);  // pos=0，无意义
// ✓ 正确: 从位置1开始找
int pos = doubled.find(sub, 1);
```

2. **编码时用原串而非dp值**:
```cpp
// ✗ 错误: 内部子串没有递归编码
string candidate = to_string(k) + "[" + s.substr(i, pos) + "]";
// ✓ 正确: 使用dp值(可能已被进一步编码)
string candidate = to_string(k) + "[" + dp[i][i + pos - 1] + "]";
```

3. **长度阈值设置错误**:
```cpp
// ✗ 错误: 长度 <= 3 时跳过，但 "aaaa" -> "4[a]" 长度相同不划算
if (len <= 3) continue;
// ✓ 正确: 长度 <= 4 时跳过，"5[a]"(4字符) 才比 "aaaaa"(5字符) 短
if (len <= 4) continue;
```

4. **分割和整体编码的顺序**:
```cpp
// ✗ 潜在问题: 先分割再整体编码，可能被分割结果覆盖整体最优解
// ✓ 正确: 三者都尝试，取最短的即可（顺序不影响正确性，但要确保都比较了）
```

## 面试追问

**Q1: 为什么拼接取余技巧能发现重复模式？**
> 如果字符串t是由周期p重复k次构成的（t = p^k），那么在t+t中从位置1开始找t，会在位置len(p)处找到。因为t+t = p^(2k)，而t = p^k可以从任何p的起始位置开始匹配。

**Q2: 时间复杂度O(n^3)怎么来的？**
> 枚举所有子串O(n^2)，每个子串枚举分割点O(n)，加上string::find O(n)。总共O(n^3)。空间O(n^2)存储dp表中的字符串。

**Q3: 能否处理解码（给定编码还原原串）？**
> 是的，LC 394 "Decode String" 就是解码问题，用栈或递归处理嵌套的`k[...]`模式。

## 相关题型

| 题号 | 题目 | 关系 |
|------|------|------|
| 394 | Decode String | 编码的逆过程(解码) |
| 459 | Repeated Substring Pattern | 判断整个串是否为重复模式 |
| 516 | Longest Palindromic Subsequence | 区间DP模板题 |
| 312 | Burst Balloons | 区间DP经典题 |
