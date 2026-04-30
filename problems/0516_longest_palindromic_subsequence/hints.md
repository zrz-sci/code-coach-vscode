# 516. 最长回文子序列

## 核心思路

这道题本质上是一个**区间 DP** 问题：对于字符串的任意子区间 `s[i..j]`，根据两端字符是否相等，可以递归地将问题缩小为更短的子区间，最终求出整个字符串的最长回文子序列长度。

另一个等价视角：**最长回文子序列 = s 和 reverse(s) 的最长公共子序列 (LCS)**。

## 思维链

1. **读完题第一反应**：子序列（不连续）比子串（连续）更难处理，不能简单用滑动窗口。暴力做法是枚举所有 $2^n$ 个子序列，逐一检查是否回文 → 指数级，不可接受。

2. **寻找递归结构**：回文的特征是"两端对称"。如果 `s[i] == s[j]`，这两个字符可以"包住"内部的回文子序列；如果 `s[i] != s[j]`，至少有一端不在最优解中，尝试去掉左端或右端取较大值。这就是一个区间缩小的递归过程。

3. **发现重叠子问题**：递归中 `(i, j)` 会被反复计算 → 加 memo 就是记忆化搜索，或者翻译成递推就是区间 DP。

4. **另一个视角**：回文 = 正着读和反着读一样。最长回文子序列 = s 与 reverse(s) 的 LCS。如果你已经会做 LCS（LeetCode 1143），这道题可以直接复用。

5. **空间优化**：无论是区间 DP 还是 LCS，二维表都可以优化到一维滚动数组。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力递归 | 枚举区间两端递归 | O(2^n) | O(n) 栈 | 能说出即可 |
| 记忆化搜索 | 暴力递归 + memo | O(n²) | O(n²) | ⭐ 必须写出 |
| 区间 DP（递推） | 自底向上填表 | O(n²) | O(n²) | ⭐ 必须写出 |
| LCS 转化 | s 和 reverse(s) 的 LCS | O(n²) | O(n²) → O(n) | 加分项 |
| 空间优化区间 DP | 一维滚动数组 | O(n²) | O(n) | 加分项 |

## 关键提示

1. **区间两端的决策**：对于 `s[i..j]`，如果 `s[i] == s[j]`，答案 = 内部区间答案 + 2；否则答案 = max(去掉左端, 去掉右端)。这是最核心的递推关系。

2. **填表方向**：`dp[i][j]` 依赖 `dp[i+1][j-1]`、`dp[i+1][j]`、`dp[i][j-1]`，所以 `i` 要从大到小遍历（或按区间长度从小到大）。

3. **base case**：单个字符就是长度为 1 的回文，即 `dp[i][i] = 1`。

4. **LCS 等价**：如果你把 `s` 反转得到 `t`，那么 `s` 和 `t` 的最长公共子序列长度就是答案。想想为什么？因为从 `s` 中选出的子序列，如果它是回文，那么它正读反读一样，必然也是 `t` 中的一个子序列。

5. **空间优化的关键**：区间 DP 中 `dp[i][j]` 只依赖 `i+1` 行和 `i` 行，可以压缩到一维；但需要一个临时变量保存被覆盖的值（即 `dp[i+1][j-1]`）。

## 解法详解

### 解法1: 暴力递归 — O(2^n) / O(n)

**思考过程**：最自然的递归——考虑区间 `[i, j]`，根据两端字符做决策。没有记忆化，存在大量重复计算。

```
决策树示意 (s = "bbbab"):
                     solve(0,4)
                s[0]='b' == s[4]='b' → 2 + solve(1,3)
                                          s[1]='b' != s[3]='a'
                                         /                    \
                                  solve(2,3)              solve(1,2)
                              s[2]='b'!=s[3]='a'      s[1]='b'!=s[2]='b'?
                               /           \           实际 s[1]='b'==s[2]='b'
                          solve(3,3)   solve(2,2)      → 2 + solve(2,1)=0
                            =1           =1            = 2
                                                ...
```

```cpp
class Solution {
public:
    int longestPalindromeSubseq(string s) {
        return solve(s, 0, s.size() - 1);
    }
private:
    int solve(const string& s, int i, int j) {
        if (i > j) return 0;          // 空区间
        if (i == j) return 1;         // 单个字符
        if (s[i] == s[j])
            return 2 + solve(s, i + 1, j - 1);
        else
            return max(solve(s, i + 1, j), solve(s, i, j - 1));
    }
};
```

**关键点**：这个解法会 TLE，但它清晰展示了递归结构，是后续所有优化的基础。

---

### 解法2: 记忆化搜索 — O(n²) / O(n²) ⭐ 面试首选

**从解法1优化**：解法1的瓶颈是重叠子问题——同一个 `(i, j)` 被反复计算。加一个 `memo[i][j]` 缓存结果即可。

```cpp
class Solution {
public:
    int longestPalindromeSubseq(string s) {
        int n = s.size();
        vector<vector<int>> memo(n, vector<int>(n, -1));
        return solve(s, 0, n - 1, memo);
    }
private:
    int solve(const string& s, int i, int j, vector<vector<int>>& memo) {
        if (i > j) return 0;
        if (i == j) return 1;
        if (memo[i][j] != -1) return memo[i][j];
        
        if (s[i] == s[j])
            memo[i][j] = 2 + solve(s, i + 1, j - 1, memo);
        else
            memo[i][j] = max(solve(s, i + 1, j), solve(s, i, j - 1, memo));
        
        return memo[i][j];
    }
};
```

**关键点**：`memo` 初始化为 -1 表示未计算。状态总数 O(n²)，每个状态 O(1) 计算，总时间 O(n²)。

---

### 解法3: 区间 DP（递推） — O(n²) / O(n²) ⭐ 面试首选

**从记忆化翻译成递推**：将自顶向下的递归翻译成自底向上的填表。

**状态定义**：`dp[i][j]` = 字符串 `s[i..j]` 中的最长回文子序列长度。

**状态转移**：
- 若 `s[i] == s[j]`：`dp[i][j] = dp[i+1][j-1] + 2`
- 若 `s[i] != s[j]`：`dp[i][j] = max(dp[i+1][j], dp[i][j-1])`

**填表方向和依赖关系**：

```
dp[i][j] 依赖于:
  dp[i+1][j-1]  (左下方)
  dp[i+1][j]    (正下方)
  dp[i][j-1]    (左方)

所以 i 从下往上遍历，j 从左往右遍历。

以 s = "bbbab" 为例，dp 表填充结果:
       j: 0  1  2  3  4
   i: 0 [ 1  2  3  3  4 ]
      1 [ 0  1  2  2  3 ]
      2 [ 0  0  1  1  3 ]
      3 [ 0  0  0  1  1 ]
      4 [ 0  0  0  0  1 ]
      
答案 = dp[0][4] = 4 (即 "bbbb")
```

```cpp
class Solution {
public:
    int longestPalindromeSubseq(string s) {
        int n = s.size();
        // dp[i][j] = s[i..j] 的最长回文子序列长度
        vector<vector<int>> dp(n, vector<int>(n, 0));
        
        // base case: 单个字符回文长度为 1
        for (int i = 0; i < n; i++)
            dp[i][i] = 1;
        
        // i 从下往上，j 从 i+1 往右
        for (int i = n - 2; i >= 0; i--) {
            for (int j = i + 1; j < n; j++) {
                if (s[i] == s[j])
                    dp[i][j] = dp[i + 1][j - 1] + 2;
                else
                    dp[i][j] = max(dp[i + 1][j], dp[i][j - 1]);
            }
        }
        
        return dp[0][n - 1];
    }
};
```

**关键点**：遍历顺序至关重要。`i` 从 `n-2` 到 `0`（因为需要 `i+1` 已经算好），`j` 从 `i+1` 到 `n-1`（因为需要 `j-1` 已经算好）。

---

### 解法4: 空间优化区间 DP — O(n²) / O(n)

**从解法3优化**：`dp[i][j]` 只依赖第 `i+1` 行和第 `i` 行。压缩为一维数组 `dp[j]`。

关键难点：更新 `dp[j]` 时，`dp[i+1][j-1]` 已经被当前行的 `dp[j-1]` 覆盖了，需要用一个变量 `prev` 保存。

```cpp
class Solution {
public:
    int longestPalindromeSubseq(string s) {
        int n = s.size();
        vector<int> dp(n, 0);
        
        for (int i = n - 1; i >= 0; i--) {
            dp[i] = 1;  // dp[i][i] = 1
            int prev = 0; // 保存 dp[i+1][j-1]（被覆盖前的值）
            for (int j = i + 1; j < n; j++) {
                int temp = dp[j]; // 保存旧的 dp[i+1][j]，下一轮变成 dp[i+1][j-1]
                if (s[i] == s[j])
                    dp[j] = prev + 2;
                else
                    dp[j] = max(dp[j], dp[j - 1]); // max(dp[i+1][j], dp[i][j-1])
                prev = temp;
            }
        }
        
        return dp[n - 1];
    }
};
```

---

### 解法5: LCS 转化 — O(n²) / O(n)

**等价转化**：`s` 的最长回文子序列 = `s` 和 `reverse(s)` 的最长公共子序列 (LCS)。

**为什么？** 从 `s` 中选一个子序列，它正读反读一样（回文），那它既是 `s` 的子序列，也是 `reverse(s)` 的子序列，即公共子序列。反之亦然。

如果已经掌握 LeetCode 1143（LCS），这道题可以直接复用。

```cpp
class Solution {
public:
    int longestPalindromeSubseq(string s) {
        string t(s.rbegin(), s.rend());
        int n = s.size();
        // 一维空间优化的 LCS
        vector<int> dp(n + 1, 0);
        for (int i = 1; i <= n; i++) {
            int prev = 0;
            for (int j = 1; j <= n; j++) {
                int temp = dp[j];
                if (s[i - 1] == t[j - 1])
                    dp[j] = prev + 1;
                else
                    dp[j] = max(dp[j], dp[j - 1]);
                prev = temp;
            }
        }
        return dp[n];
    }
};
```

## 解法对比

| | 区间 DP (解法3) | 空间优化 (解法4) | LCS 转化 (解法5) |
|---|---|---|---|
| 时间 | O(n²) | O(n²) | O(n²) |
| 空间 | O(n²) | O(n) | O(n) |
| 思维难度 | 中等 | 较高（prev 变量） | 简单（如果会 LCS） |
| 面试推荐 | 首选，容易讲清楚 | 追问空间优化时给出 | 展示联想能力 |

**选择建议**：面试中先写解法3（清晰易懂），被追问空间优化时给出解法4。如果面试官问"还有别的思路吗"，给出解法5展示联想能力。

## 易错点

1. **遍历方向写反**
   - ✗ `for (int i = 0; i < n; i++)` — i 从上往下，此时 `dp[i+1][...]` 还没算
   - ✓ `for (int i = n - 2; i >= 0; i--)` — i 从下往上，保证依赖已就绪

2. **空间优化时忘记保存 prev**
   - ✗ `dp[j] = dp[j-1] + 2` — `dp[j-1]` 已经是当前行的值，不是 `dp[i+1][j-1]`
   - ✓ 用 `prev` 变量保存上一轮的 `dp[j-1]`

3. **base case 遗漏**
   - ✗ 忘记设置 `dp[i][i] = 1`，所有单字符区间默认是 0
   - ✓ 初始化所有 `dp[i][i] = 1`

4. **空间优化中 `dp[i] = 1` 的位置**
   - ✗ 放在内层循环里，每次 j 迭代都重置
   - ✓ 放在外层循环里，每个 i 只设置一次

## 面试追问

**Q1: 暴力递归为什么是指数级？能画出重叠子问题吗？**

> `solve(i, j)` 会分裂成 `solve(i+1, j)` 和 `solve(i, j-1)`，两者都会调用 `solve(i+1, j-1)`。状态空间是 O(n²) 但不加 memo 会指数级重复计算。

**Q2: 能否把空间从 O(n²) 优化到 O(n)？**

> 可以。`dp[i][j]` 只依赖第 `i+1` 行和第 `i` 行，用一维数组 + prev 变量即可。见解法4。

**Q3: 如果要输出最长回文子序列本身（不仅是长度），怎么做？**

> 在填完 dp 表后，从 `dp[0][n-1]` 开始回溯：如果 `s[i]==s[j]`，把 `s[i]` 加到结果两端，然后走到 `(i+1, j-1)`；否则走到 `dp[i+1][j]` 和 `dp[i][j-1]` 中较大的那个。时间 O(n) 回溯。

**Q4: 这道题和 LeetCode 5（最长回文子串）有什么区别？**

> 子串要求连续，子序列不要求连续。子串版本可以用中心扩展 O(n²) 或 Manacher O(n)。子序列版本必须用 DP，因为不连续的选择需要记录状态。

## 相关题型

- **LeetCode 5 最长回文子串** — 子串（连续）版本，可用中心扩展或 Manacher，和本题的区别是连续 vs 不连续
- **LeetCode 1143 最长公共子序列 (LCS)** — 本题的解法5直接复用 LCS 模板，只需把第二个字符串设为 reverse(s)
- **LeetCode 647 回文子串计数** — 同样是区间 DP 框架，但求的是数量而非最长长度
- **LeetCode 1216 验证回文串 III** — 问能否删除最多 k 个字符使之回文，等价于 `n - LPS(s) <= k`，直接复用本题的结果
- **LeetCode 72 编辑距离** — 同为二维 DP + 空间优化到一维的经典题，`prev` 变量技巧完全相同