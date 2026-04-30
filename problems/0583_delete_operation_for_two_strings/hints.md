# 583. 两个字符串的删除操作

## 核心思路

**本质：找出两个字符串的最长公共子序列(LCS)，剩下的字符就是需要删除的。**

删除操作的最小步数 = `len(word1) + len(word2) - 2 * LCS长度`。当然，也可以直接定义 DP 状态为"使两个前缀相同的最小删除步数"，不绕道 LCS。

## 思维链

1. **读完题第一反应**：两个字符串各自删一些字符，最终变一样。最终保留下来的那个"一样的东西"是什么？→ 它必须是 word1 的子序列，同时也是 word2 的子序列，即**公共子序列**。
2. **目标转化**：要删除最少的字符 → 保留最多的字符 → 找到**最长公共子序列(LCS)**。
3. **暴力怎么做**：枚举 word1 的所有子序列（2^m 个），检查是否也是 word2 的子序列，找最长的。指数级复杂度，不可行。
4. **为什么能 DP**：比较 word1[i] 和 word2[j] 时，如果相等就同时保留，如果不等就至少删一个。这天然有**最优子结构**和**重叠子问题**。
5. **两种 DP 建模方式**：
   - **方式A**：先求 LCS 长度，再算删除步数（经典 LCS）
   - **方式B**：直接定义 dp[i][j] = 使 word1[0..i-1] 和 word2[0..j-1] 相同的最小删除数
6. **空间优化**：dp[i][j] 只依赖 dp[i-1][j-1]、dp[i-1][j]、dp[i][j-1]，可以用一维滚动数组。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力递归 | 递归尝试每个字符删或不删 | O(2^(m+n)) | O(m+n) | 能说出即可 |
| LCS 转化 + DP | 先求 LCS 长度，答案 = m+n-2*LCS | O(mn) | O(mn) | ⭐ 必须写出 |
| 直接 DP（最小删除数） | dp[i][j] 直接定义为删除步数 | O(mn) | O(mn) | ⭐ 必须写出 |
| 空间优化 DP | 滚动数组降至一维 | O(mn) | O(min(m,n)) | 加分项 |

## 关键提示

1. **转化视角**：删除最少 ↔ 保留最多。保留下来的公共部分就是 LCS。
2. **DP 状态设计**：无论哪种方式，都是二维 `dp[i][j]`，i 对应 word1 的前 i 个字符，j 对应 word2 的前 j 个字符。
3. **转移的关键分支**：`word1[i-1] == word2[j-1]` 时怎么转移？不等时怎么转移？
4. **和 Edit Distance(72题) 的关系**：本题只允许"删除"操作，编辑距离允许增删改。本题是编辑距离的简化版。
5. **初始化别忘了**：`dp[i][0]` 和 `dp[0][j]` 分别代表一个串为空时的情况。

```
// DP 填表方向示意 (word1="sea", word2="eat"):
//
//     ""  e  a  t
// ""   0  1  2  3     ← dp[0][j] = j (word1为空，需删word2的j个字符)
//  s   1  2  3  4
//  e   2  1  2  3     ← word1[1]='e' == word2[0]='e', dp[2][1]=dp[1][0]+0? 不对
//  a   3  2  1  2        用直接DP: 匹配时 dp[i][j]=dp[i-1][j-1], 不匹配时 min(dp[i-1][j],dp[i][j-1])+1
//
// 答案 dp[3][3] = 2
```

## 解法详解

### 解法1: 暴力递归 — O(2^(m+n)) / O(m+n)

**思考过程**：最直觉的方式——递归比较两个字符串。如果当前字符相同，同时前进；如果不同，要么从 word1 删一个，要么从 word2 删一个，取最小值。

```cpp
class Solution {
public:
    int minDistance(string word1, string word2) {
        return dfs(word1, word2, 0, 0);
    }
    
    int dfs(string& w1, string& w2, int i, int j) {
        // base case: 某个串用完了，剩余的另一个串全部要删
        if (i == w1.size()) return w2.size() - j;
        if (j == w2.size()) return w1.size() - i;
        
        if (w1[i] == w2[j]) {
            // 字符匹配，不需要删除，同时前进
            return dfs(w1, w2, i + 1, j + 1);
        } else {
            // 不匹配：删 word1[i] 或删 word2[j]，各花费1步
            return 1 + min(dfs(w1, w2, i + 1, j),
                          dfs(w1, w2, i, j + 1));
        }
    }
};
```

**关键点**：这个暴力解会超时，但它清晰展示了递归结构。注意大量重叠子问题：`dfs(2,3)` 可能从 `dfs(1,3)` 和 `dfs(2,2)` 同时递归到。加个 memo 就是解法2。

---

### 解法2: 记忆化搜索（自顶向下DP）— O(mn) / O(mn)

**从解法1优化**：解法1的瓶颈是重复计算。`dfs(i,j)` 的结果只取决于 (i,j)，总共 m×n 种状态，用 memo 缓存即可。

```cpp
class Solution {
public:
    int minDistance(string word1, string word2) {
        int m = word1.size(), n = word2.size();
        vector<vector<int>> memo(m, vector<int>(n, -1));
        return dfs(word1, word2, 0, 0, memo);
    }
    
    int dfs(string& w1, string& w2, int i, int j, vector<vector<int>>& memo) {
        if (i == w1.size()) return w2.size() - j;
        if (j == w2.size()) return w1.size() - i;
        
        if (memo[i][j] != -1) return memo[i][j];
        
        if (w1[i] == w2[j]) {
            memo[i][j] = dfs(w1, w2, i + 1, j + 1, memo);
        } else {
            memo[i][j] = 1 + min(dfs(w1, w2, i + 1, j, memo),
                                  dfs(w1, w2, i, j + 1, memo));
        }
        return memo[i][j];
    }
};
```

---

### 解法3: 直接 DP（递推，自底向上）— O(mn) / O(mn) ⭐ 面试首选

**把记忆化搜索翻译成递推**。定义 `dp[i][j]` = 使 word1 的前 i 个字符和 word2 的前 j 个字符变相同所需的最小删除步数。

```
// 状态转移:
//   word1[i-1] == word2[j-1]: dp[i][j] = dp[i-1][j-1]     (匹配，不用删)
//   word1[i-1] != word2[j-1]: dp[i][j] = min(dp[i-1][j], dp[i][j-1]) + 1
//                                             ↑删word1[i-1]  ↑删word2[j-1]
//
// 填表方向: 从左到右，从上到下
//
//     ""  e  a  t
// ""   0  1  2  3
//  s   1  2  3  4
//  e   2  1  2  3
//  a   3  2  1  2  ← 答案
//
// dp[i][j] 依赖: dp[i-1][j-1](左上), dp[i-1][j](上), dp[i][j-1](左)
```

```cpp
class Solution {
public:
    int minDistance(string word1, string word2) {
        int m = word1.size(), n = word2.size();
        // dp[i][j]: word1前i个 和 word2前j个 变相同的最小删除数
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
        
        // 初始化: word1为空时，需要删掉word2的全部j个字符
        for (int j = 0; j <= n; j++) dp[0][j] = j;
        // word2为空时，需要删掉word1的全部i个字符
        for (int i = 0; i <= m; i++) dp[i][0] = i;
        
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (word1[i - 1] == word2[j - 1]) {
                    // 字符匹配：这两个字符都保留，不增加删除次数
                    dp[i][j] = dp[i - 1][j - 1];
                } else {
                    // 不匹配：删word1[i-1]或删word2[j-1]，取最小
                    dp[i][j] = min(dp[i - 1][j], dp[i][j - 1]) + 1;
                }
            }
        }
        return dp[m][n];
    }
};
```

**关键点**：初始化 `dp[i][0]=i` 和 `dp[0][j]=j` 是因为另一个串为空时，当前串的所有字符都要删掉。

---

### 解法4: LCS 转化 — O(mn) / O(mn)

**另一种思考角度**：保留的部分 = 最长公共子序列。先求 LCS 长度 L，答案 = `m + n - 2*L`。

```cpp
class Solution {
public:
    int minDistance(string word1, string word2) {
        int m = word1.size(), n = word2.size();
        // lcs[i][j]: word1前i个 和 word2前j个 的LCS长度
        vector<vector<int>> lcs(m + 1, vector<int>(n + 1, 0));
        
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (word1[i - 1] == word2[j - 1]) {
                    lcs[i][j] = lcs[i - 1][j - 1] + 1;
                } else {
                    lcs[i][j] = max(lcs[i - 1][j], lcs[i][j - 1]);
                }
            }
        }
        // 两个串总长 - 保留的公共部分×2 = 需要删除的总步数
        return m + n - 2 * lcs[m][n];
    }
};
```

---

### 解法5: 空间优化（滚动数组）— O(mn) / O(min(m,n))

**dp[i][j] 只依赖上一行 dp[i-1][...] 和当前行 dp[i][j-1]**，可以压成一维。注意匹配时需要 `dp[i-1][j-1]`（左上角），需要额外变量保存。

```cpp
class Solution {
public:
    int minDistance(string word1, string word2) {
        int m = word1.size(), n = word2.size();
        // 让较短的串作为列，节省空间
        if (m < n) return minDistance(word2, word1);
        
        vector<int> dp(n + 1);
        // 初始化第0行: dp[j] = j
        for (int j = 0; j <= n; j++) dp[j] = j;
        
        for (int i = 1; i <= m; i++) {
            int prev = dp[0];  // prev 保存 dp[i-1][j-1]，即"左上角"
            dp[0] = i;         // dp[i][0] = i
            for (int j = 1; j <= n; j++) {
                int temp = dp[j];  // 保存当前值（更新前），下一轮做左上角
                if (word1[i - 1] == word2[j - 1]) {
                    dp[j] = prev;  // 对应 dp[i-1][j-1]
                } else {
                    dp[j] = min(dp[j],       // dp[i-1][j] (上)
                                dp[j - 1])    // dp[i][j-1] (左)
                            + 1;
                }
                prev = temp;
            }
        }
        return dp[n];
    }
};
```

## 解法对比

| | 直接DP(解法3) | LCS转化(解法4) | 空间优化(解法5) |
|---|---|---|---|
| **DP含义** | 最小删除步数 | LCS长度 | 同解法3，压缩空间 |
| **转移** | 匹配时不+1，不匹配时min+1 | 匹配时+1，不匹配时max | 同解法3 |
| **答案** | dp[m][n] 就是答案 | m+n-2*lcs[m][n] | dp[n] |
| **面试推荐** | ⭐ 最直接 | ⭐ 思路清晰（如果你熟悉LCS） | 加分项 |
| **空间** | O(mn) | O(mn) | O(min(m,n)) |

**选择建议**：面试时两种都能讲。如果面试官问"这题和LCS什么关系"，用解法4；如果问"直接DP怎么做"，用解法3。能额外给出解法5说明你对DP空间优化有深入理解。

## 易错点

1. **下标偏移1**：
   - ✗ `word1[i] == word2[j]` 配合 `dp` 从 1 开始
   - ✓ `word1[i-1] == word2[j-1]`，因为 dp 的第0行/列代表空串

2. **初始化遗漏**：
   - ✗ 只初始化 `dp[0][j]=0`
   - ✓ `dp[0][j]=j`，`dp[i][0]=i`（空串匹配需要删掉所有字符）

3. **匹配时的转移写反**：
   - 直接DP：匹配时 `dp[i][j] = dp[i-1][j-1]`（不加1，因为不需要删除）
   - LCS：匹配时 `lcs[i][j] = lcs[i-1][j-1] + 1`（加1，因为LCS长度增加）
   - 两种写法方向相反，容易混淆

4. **空间优化时忘记保存左上角**：
   - ✗ 直接 `dp[j] = prev` 但 prev 没有在每轮更新
   - ✓ 每轮先 `temp = dp[j]`，更新完后 `prev = temp`

## 面试追问

**Q1: 暴力解的复杂度是多少？为什么不可行？**
→ 暴力枚举所有删除方案是指数级 O(2^(m+n))。即使只枚举子序列也是 O(2^m × n) 或 O(2^n × m)。DP 利用最优子结构把状态数降到 O(mn)。

**Q2: 这道题和 LCS（1143题）有什么关系？**
→ 答案 = m + n - 2 × LCS(word1, word2)。删除最少 ↔ 保留最多的公共部分。本质上是同一个问题的两面。

**Q3: 能否 O(n) 空间完成？**
→ 可以。dp[i][j] 只依赖三个位置（左上、上、左），用一维滚动数组 + 一个 prev 变量即可。见解法5。

**Q4: 如果操作改为可以"删除"和"插入"（即编辑距离，但没有替换），答案会变吗？**
→ 不会变。在一个串中删除一个字符，等价于在另一个串中插入一个字符。所以"只允许删除"和"允许删除+插入（不允许替换）"得到的最小操作数相同，都是 `m + n - 2 * LCS`。

## 相关题型

- **1143. 最长公共子序列(LCS)** — 本题的核心子问题。学会本题的解法4后，1143 就是直接复用 LCS 的 DP 模板。
- **72. 编辑距离** — 本题的升级版。编辑距离增加了"替换"操作，DP 转移多一个分支：不匹配时还可以 `dp[i-1][j-1]+1`（替换）。本题只有 `dp[i-1][j]+1` 和 `dp[i][j-1]+1`。
- **712. 两个字符串的最小 ASCII 删除和** — 本题的变体。从"最小删除步数"变为"最小删除 ASCII 值之和"，只需把转移中的 `+1` 改为 `+对应字符的ASCII值`。
- **1092. 最短公共超序列** — 反过来：不是删到相同，而是合并成最短的包含两者的序列。也基于 LCS。