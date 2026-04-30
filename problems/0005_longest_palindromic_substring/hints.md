# 5. 最长回文子串

## 核心思路

本质上在问：在字符串 `s` 中，找到一个最长的连续子串，使得它正读和反读一样。核心难点是如何高效地判断和搜索所有可能的回文子串。

## 思维链

1. **读完题第一反应**：枚举所有子串，逐个判断是否回文，记录最长的。子串有 O(n²) 个，判断回文 O(n)，总共 O(n³)。

2. **暴力解的瓶颈在哪？** 两个瓶颈：
   - 枚举子串 O(n²) 似乎避不开
   - 但每次从头判断回文 O(n) 有没有办法优化？

3. **观察回文的结构特征**：回文串是**关于中心对称**的。如果我们已经知道以某个位置为中心的回文有多长，能不能直接"向外扩展"？答案是可以的 — 从中心出发，左右同时扩展，只要两边字符相等就继续。

4. **中心扩展法**：枚举每个可能的中心（单字符中心 + 双字符中心，共 2n-1 个），从每个中心向外扩展。每次扩展 O(n)，总共 O(n²)，比暴力快了一个量级。

5. **能不能更快？** 可以用 DP 把"子串是否回文"的信息存下来，避免重复计算。或者用 Manacher 算法做到 O(n)，但面试中很少要求。

6. **面试最佳策略**：中心扩展法代码最简洁、思路最清晰，是面试首选。DP 解法展示你理解"利用已知信息"的能力。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力枚举 | 枚举所有子串，逐个判断回文 | O(n³) | O(1) | 能说出即可 |
| 动态规划 | dp[i][j] 表示 s[i..j] 是否回文 | O(n²) | O(n²) | 能说出即可 |
| 中心扩展 | 枚举 2n-1 个中心，向外扩展 | O(n²) | O(1) | ⭐ 必须写出 |
| Manacher | 利用已知回文信息跳过重复计算 | O(n) | O(n) | 加分项 |

## 关键提示

1. **回文的中心有两种**：单字符中心（如 "aba" 以 'b' 为中心）和双字符中心（如 "abba" 以 "bb" 为中心）。所以长度为 n 的字符串有 `2n - 1` 个可能的中心，不是 n 个。

2. **DP 的状态转移**：`s[i..j]` 是回文 ⟺ `s[i] == s[j]` 且 `s[i+1..j-1]` 是回文。想想遍历顺序：你需要先知道短子串的结果，才能推出长子串。

3. **中心扩展为什么是 O(n²) 而不是 O(n³)**：虽然每个中心最坏扩展 O(n)，但我们不需要从头验证整个子串，而是增量地"往外扩一步"。

4. **边界条件**：长度为 1 的子串一定是回文；长度为 2 的子串只需检查两个字符是否相等。

5. **返回的是子串本身，不是长度**：记得记录起始位置和长度，最后用 `substr` 提取。

## 解法详解

### 解法1: 暴力枚举 — O(n³) / O(1)

**思考过程**：最直接的想法 — 枚举所有可能的子串 `s[i..j]`，检查每个是否回文，记录最长的。

```cpp
class Solution {
public:
    string longestPalindrome(string s) {
        int n = s.size();
        int start = 0, maxLen = 1;
        
        for (int i = 0; i < n; i++) {
            for (int j = i; j < n; j++) {
                // 检查 s[i..j] 是否回文
                if (isPalindrome(s, i, j) && j - i + 1 > maxLen) {
                    start = i;
                    maxLen = j - i + 1;
                }
            }
        }
        return s.substr(start, maxLen);
    }
    
private:
    bool isPalindrome(const string& s, int left, int right) {
        while (left < right) {
            if (s[left] != s[right]) return false;
            left++;
            right--;
        }
        return true;
    }
};
```

**关键点**：O(n²) 个子串 × O(n) 判断 = O(n³)。n=1000 时约 10⁹ 次操作，会超时。瓶颈在于：对每个子串都是"从头判断"，没有利用之前的计算结果。

---

### 解法2: 动态规划 — O(n²) / O(n²)

**从解法1优化**：暴力法中，判断 `s[i..j]` 是否回文时，没有利用 `s[i+1..j-1]` 的结果。DP 可以把这些结果存下来。

**状态定义**：`dp[i][j]` = `true` 表示子串 `s[i..j]` 是回文。

**状态转移**：
```
dp[i][j] = (s[i] == s[j]) && dp[i+1][j-1]
```

**初始化与遍历方向**：
- 长度为 1：`dp[i][i] = true`
- 长度为 2：`dp[i][i+1] = (s[i] == s[i+1])`
- 长度 ≥ 3：按长度从小到大遍历，确保 `dp[i+1][j-1]` 已经算过

```
// DP 填表方向示意 (s = "babad"):
//
//   b a b a d
// b T . . . .     T = true, F = false, . = 尚未填
// a   T . . .     
// b     T . .     填表顺序: 先填对角线(长度1)
// a       T .              → 再填次对角线(长度2)
// d         T              → 再填长度3, 4, 5...
//
// 填完后:
//   b a b a d
// b T F T F F     dp[0][2]=T → "bab" 是回文
// a   T F T F     dp[1][3]=T → "aba" 是回文
// b     T F F     
// a       T F
// d         T
// 最长回文: "bab" 或 "aba"，长度 3
```

```cpp
class Solution {
public:
    string longestPalindrome(string s) {
        int n = s.size();
        if (n < 2) return s;
        
        // dp[i][j] 表示 s[i..j] 是否为回文
        vector<vector<bool>> dp(n, vector<bool>(n, false));
        
        int start = 0, maxLen = 1;
        
        // 所有长度为1的子串都是回文
        for (int i = 0; i < n; i++) {
            dp[i][i] = true;
        }
        
        // 按子串长度从小到大填表（关键：保证 dp[i+1][j-1] 已计算）
        for (int len = 2; len <= n; len++) {
            for (int i = 0; i <= n - len; i++) {
                int j = i + len - 1;
                
                if (s[i] != s[j]) {
                    dp[i][j] = false;
                } else {
                    // 长度为2时 s[i]==s[j] 就够了（中间没有字符）
                    // 长度≥3时还需要 dp[i+1][j-1] 也是回文
                    dp[i][j] = (len == 2) || dp[i + 1][j - 1];
                }
                
                if (dp[i][j] && len > maxLen) {
                    start = i;
                    maxLen = len;
                }
            }
        }
        
        return s.substr(start, maxLen);
    }
};
```

**关键点**：
- 遍历顺序必须按长度从小到大（或 i 从大到小），否则依赖的子问题还没算过
- `len == 2` 的特判不能漏，因为此时 `i+1 > j-1`，`dp[i+1][j-1]` 虽然数组中能访问但语义不对

---

### 解法3: 中心扩展法 — O(n²) / O(1) ⭐ 面试首选

**换个角度思考**：与其枚举子串的两个端点，不如枚举回文的**中心**，然后向两边扩展。

```
// 中心扩展示意 (s = "babad"):
//
// 中心在 s[0]='b':  b → 无法扩展 → "b"
// 中心在 s[0..1]='ba': b≠a → 不回文
// 中心在 s[1]='a':  a → bab(s[0]==s[2]) → 无法继续 → "bab" ✓
// 中心在 s[1..2]='ab': a≠b → 不回文  
// 中心在 s[2]='b':  b → aba(s[1]==s[3]) → 无法继续 → "aba" ✓
// ...以此类推
//
// 回文中心有两种:
//   奇数长度: 以单个字符为中心  →  n 个中心
//   偶数长度: 以两个字符的间隙为中心  →  n-1 个中心
//   共 2n-1 个中心
```

```cpp
class Solution {
public:
    string longestPalindrome(string s) {
        int n = s.size();
        if (n < 2) return s;
        
        int start = 0, maxLen = 1;
        
        for (int i = 0; i < n; i++) {
            // 以 s[i] 为中心扩展（奇数长度回文）
            int len1 = expandAroundCenter(s, i, i);
            // 以 s[i] 和 s[i+1] 之间为中心扩展（偶数长度回文）
            int len2 = expandAroundCenter(s, i, i + 1);
            
            int len = max(len1, len2);
            if (len > maxLen) {
                // 从中心和长度反推起始位置:
                // 奇数: 中心i, 长度len → 起始 = i - (len-1)/2
                // 偶数: 中心i和i+1, 长度len → 起始 = i - (len-2)/2 = i - len/2 + 1
                // 两种情况统一: start = i - (len - 1) / 2
                start = i - (len - 1) / 2;
                maxLen = len;
            }
        }
        
        return s.substr(start, maxLen);
    }
    
private:
    // 从 s[left] 和 s[right] 开始，向两边扩展，返回最长回文长度
    int expandAroundCenter(const string& s, int left, int right) {
        while (left >= 0 && right < (int)s.size() && s[left] == s[right]) {
            left--;
            right++;
        }
        // 循环结束时 left 和 right 已经越过回文边界一步
        // 回文实际范围是 [left+1, right-1]，长度 = right - left - 1
        return right - left - 1;
    }
};
```

**为什么这是面试首选**：
- 代码简洁，思路直观（"从中心向外扩"）
- O(1) 空间，不需要额外数组
- 时间 O(n²) 对 n≤1000 完全够用
- 容易讲清楚，不容易出 bug

---

### 解法4: Manacher 算法 — O(n) / O(n)（加分项）

**核心思想**：利用已经计算过的回文信息来跳过重复的扩展。Manacher 维护一个"最右回文边界"，当新的中心在这个边界内时，可以利用对称性直接得到一个初始半径，避免从头扩展。

**预处理**：在字符之间插入特殊字符（如 `#`），把偶数长度回文统一为奇数长度处理。

```
// 预处理: "babad" → "^#b#a#b#a#d#$"
//   ^ 和 $ 是哨兵，避免越界
//   # 统一了奇偶长度
//
// Manacher 核心:
//   p[i] = 以 i 为中心的回文半径
//   维护 center 和 right = center + p[center]（当前最右回文的中心和右边界）
//   
//   对于新位置 i:
//     如果 i < right，利用对称点 mirror = 2*center - i
//       p[i] = min(p[mirror], right - i)  ← 跳过已知部分
//     然后尝试继续扩展
//     如果扩展后 i + p[i] > right，更新 center 和 right
```

```cpp
class Solution {
public:
    string longestPalindrome(string s) {
        // 预处理: 插入 # 统一奇偶
        string t = "^#";
        for (char c : s) {
            t += c;
            t += '#';
        }
        t += '$';
        
        int n = t.size();
        vector<int> p(n, 0);  // p[i] = 以 t[i] 为中心的回文半径
        
        int center = 0, right = 0;  // 当前最右回文的中心和右边界
        
        for (int i = 1; i < n - 1; i++) {
            int mirror = 2 * center - i;  // i 关于 center 的对称点
            
            if (i < right) {
                // 利用对称性：p[i] 至少和 p[mirror] 一样大
                // 但不能超过 right - i（超出部分还没验证）
                p[i] = min(p[mirror], right - i);
            }
            
            // 尝试继续扩展
            while (t[i + p[i] + 1] == t[i - p[i] - 1]) {
                p[i]++;
            }
            
            // 如果扩展后超过了当前最右边界，更新 center 和 right
            if (i + p[i] > right) {
                center = i;
                right = i + p[i];
            }
        }
        
        // 找 p 数组中的最大值
        int maxLen = 0, centerIdx = 0;
        for (int i = 1; i < n - 1; i++) {
            if (p[i] > maxLen) {
                maxLen = p[i];
                centerIdx = i;
            }
        }
        
        // 映射回原字符串: 预处理后的索引 centerIdx 对应原串起始位置
        int start = (centerIdx - maxLen) / 2;
        return s.substr(start, maxLen);
    }
};
```

**关键点**：Manacher 的精妙在于 `p[i] = min(p[mirror], right - i)` 这一行，它利用回文的对称性跳过已知部分，使总扩展次数为 O(n)。面试中一般不要求写 Manacher，但能讲清思路是加分项。

## 解法对比

| | 暴力枚举 | 动态规划 | 中心扩展 | Manacher |
|---|---|---|---|---|
| 时间 | O(n³) | O(n²) | O(n²) | O(n) |
| 空间 | O(1) | O(n²) | O(1) | O(n) |
| 代码复杂度 | 简单 | 中等 | 简单 | 复杂 |
| 面试推荐度 | ❌ | ✓ | ⭐⭐⭐ | 加分 |
| 核心思想 | 穷举 | 记忆化子问题 | 利用回文对称性 | 利用已知回文跳跃 |

**什么时候选哪个？**
- **面试**：优先写中心扩展（简洁 + O(1) 空间），然后口头说明 DP 思路
- **需要 O(n)**：Manacher，但面试官很少要求
- **理解 DP 框架**：DP 解法是"区间 DP"的入门，值得掌握思路

## 易错点

1. **中心扩展忘记偶数长度回文**
   - ✗ 只扩展 `expand(i, i)`
   - ✓ 必须同时扩展 `expand(i, i)` 和 `expand(i, i+1)`
   - 因为 "abba" 这种偶数长度回文没有单字符中心

2. **中心扩展的起始位置反推公式写错**
   - ✗ `start = i - len / 2`
   - ✓ `start = i - (len - 1) / 2`
   - 原因：奇数长度时中心在正中间，偶数长度时中心偏左。`(len-1)/2` 整数除法同时处理两种情况

3. **DP 遍历顺序错误**
   - ✗ 按 `i` 从小到大、`j` 从小到大遍历
   - ✓ 按**子串长度**从小到大遍历，或 `i` 从大到小遍历
   - 因为 `dp[i][j]` 依赖 `dp[i+1][j-1]`，如果 `i` 从小到大，`i+1` 行还没算

4. **DP 中 `len==2` 的特判遗漏**
   - ✗ `dp[i][j] = (s[i]==s[j]) && dp[i+1][j-1]` 对所有长度
   - ✓ 长度为 2 时 `i+1 > j-1`，此时 `dp[i+1][j-1]` 语义上是空串（应为 true），需要特判

5. **Manacher 预处理时忘加边界哨兵**
   - ✗ `t = "#b#a#b#a#d#"` → while 循环可能越界
   - ✓ `t = "^#b#a#b#a#d#$"` → `^` 和 `$` 保证永远不匹配，自然停止

## 面试追问

**Q1: 暴力法的时间复杂度是多少？瓶颈在哪？**
→ O(n³)。O(n²) 个子串，每个判断回文 O(n)。瓶颈在于每次判断回文都是独立的，没有利用之前的结果。

**Q2: 你用 DP 优化了判断回文的过程，能否进一步优化空间？**
→ DP 需要 O(n²) 空间存表。可以改用中心扩展法，O(1) 空间、同样 O(n²) 时间。中心扩展不需要存所有子问题的结果，因为每个中心的扩展是独立的。

**Q3: 如果要求 O(n) 时间复杂度呢？**
→ Manacher 算法。核心思想是维护一个"最右回文边界"，利用回文的对称性跳过重复扩展。每个字符最多被扩展常数次，总时间 O(n)。

**Q4: 如果不是找最长回文子串，而是找最长回文子序列（LeetCode 516）呢？**
→ 完全不同的问题。子序列不要求连续，不能用中心扩展。要用区间 DP：`dp[i][j]` = `s[i..j]` 中最长回文子序列的长度。转移方程：如果 `s[i]==s[j]`，`dp[i][j] = dp[i+1][j-1]+2`；否则 `dp[i][j] = max(dp[i+1][j], dp[i][j-1])`。

## 相关题型

- **LeetCode 647 回文子串** — 复用中心扩展框架，区别：本题求最长一个，647求总数量（把 `max` 改成 `count++`）
- **LeetCode 516 最长回文子序列** — 区间 DP 框架，和本题 DP 解法的状态定义类似（都是 `dp[i][j]`），但转移方程不同（子序列不要求连续）
- **LeetCode 131 分割回文串** — 复用本题的回文判断（可预处理 DP 表），然后回溯枚举所有分割方式
- **LeetCode 214 最短回文串** — 可用 Manacher 或 KMP 求解，本质是找以 `s[0]` 开头的最长回文前缀