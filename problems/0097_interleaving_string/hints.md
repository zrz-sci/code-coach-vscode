# 97. 交错字符串

## 核心思路

本质上是一个**双指针路径问题**：你站在一个 `(m+1) x (n+1)` 的网格的左上角 `(0,0)`，每一步可以向右走（消耗 s1 的一个字符）或向下走（消耗 s2 的一个字符），问是否存在一条路径到达右下角 `(m,n)`，使得路径上消耗的字符恰好拼出 s3。

## 思维链

1. **读完题第一反应**：s3 的每个字符要么来自 s1，要么来自 s2，且相对顺序不变。最朴素的想法是递归尝试：当前 s3 的字符如果匹配 s1 的下一个字符就尝试取 s1，如果匹配 s2 的下一个字符就尝试取 s2。

2. **暴力递归的瓶颈**：两种选择都可能匹配时会分叉，最坏情况是指数级 `O(2^(m+n))`。但仔细看——递归状态其实只由 `(i, j)` 决定（i = s1 已用了多少，j = s2 已用了多少，s3 的位置自动确定为 `i+j`），大量状态被重复计算。

3. **发现重叠子问题 → DP**：既然状态只有 `(i, j)` 两个维度，总共 `O(m*n)` 个状态，加个 memo 就行。或者直接用二维 DP 表。

4. **定义 DP 状态**：`dp[i][j]` = s1 的前 i 个字符和 s2 的前 j 个字符能否交错组成 s3 的前 `i+j` 个字符。

5. **状态转移**：`dp[i][j]` 为 true，当且仅当：
   - 从上方转移：`dp[i-1][j]` 为 true **且** `s1[i-1] == s3[i+j-1]`
   - 或从左方转移：`dp[i][j-1]` 为 true **且** `s2[j-1] == s3[i+j-1]`

6. **空间优化（Follow-up）**：`dp[i][j]` 只依赖 `dp[i-1][j]`（上一行同列）和 `dp[i][j-1]`（同行前一列），可以压缩成一维数组 `dp[j]`。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力递归 | DFS 尝试每步取 s1 或 s2 | O(2^(m+n)) | O(m+n) | 能说出即可 |
| 记忆化搜索 | 递归 + memo | O(m·n) | O(m·n) | 能说出即可 |
| 二维 DP | 自底向上填表 | O(m·n) | O(m·n) | ⭐ 必须写出 |
| 一维 DP（空间优化） | 滚动数组 | O(m·n) | O(n) | 加分项 |

## 关键提示

1. **长度校验**：如果 `len(s1) + len(s2) != len(s3)`，直接返回 false。这个剪枝能排除大量无效情况。

2. **状态的唯一性**：给定 s1 用了 i 个字符、s2 用了 j 个字符，s3 的位置 `k = i + j` 是自动确定的——所以状态只需要两个维度，不需要三个。

3. **网格路径类比**：把问题想象成在 `(m+1) x (n+1)` 的网格上从 `(0,0)` 走到 `(m,n)`，每步只能向右或向下。这样就和经典的路径 DP 一模一样了：

```
        s2: d  b  b  c  a
     j:  0  1  2  3  4  5
  i: 0  [T] →  →  →  →  →
s1:     ↓
a  1    ↓
a  2    ↓
b  3    ↓
c  4    ↓
c  5    ↓              [?]  ← 目标
```

4. **DP 初始化**：第一行 `dp[0][j]` 表示只用 s2 的前 j 个字符拼 s3 的前 j 个字符；第一列 `dp[i][0]` 表示只用 s1 的前 i 个字符拼 s3 的前 i 个字符。一旦某个位置不匹配，后面全是 false。

5. **空间优化的关键**：`dp[i][j]` 依赖正上方 `dp[i-1][j]` 和正左方 `dp[i][j-1]`。一维滚动时，`dp[j]` 在更新前就是上一行的值（正上方），`dp[j-1]` 是本行刚更新的值（正左方），完美对应。

## 解法详解

### 解法1: 暴力递归 — O(2^(m+n)) / O(m+n)

**思考过程**：最直接的想法——递归尝试。当前要匹配 s3[i+j]，如果它等于 s1[i]，就尝试从 s1 取；如果它等于 s2[j]，就尝试从 s2 取。两者可以都尝试。

```cpp
class Solution {
public:
    bool isInterleave(string s1, string s2, string s3) {
        if (s1.size() + s2.size() != s3.size()) return false;
        return dfs(s1, s2, s3, 0, 0);
    }
    
    bool dfs(string& s1, string& s2, string& s3, int i, int j) {
        int k = i + j;
        // 全部匹配完成
        if (k == s3.size()) return true;
        
        // 尝试从 s1 取
        if (i < s1.size() && s1[i] == s3[k] && dfs(s1, s2, s3, i + 1, j))
            return true;
        // 尝试从 s2 取
        if (j < s2.size() && s2[j] == s3[k] && dfs(s1, s2, s3, i, j + 1))
            return true;
        
        return false;
    }
};
```

**关键点**：会 TLE。当 s1 和 s2 有大量相同字符时（如 `s1="aaa..."`, `s2="aaa..."`），分叉极多，指数爆炸。

---

### 解法2: 记忆化搜索 — O(m·n) / O(m·n)

**从解法1优化**：解法1 的瓶颈是大量重复计算。注意到状态只由 `(i, j)` 决定，总共 `O(m·n)` 种状态，加一个 memo 数组记录已经算过的状态即可。

```cpp
class Solution {
public:
    bool isInterleave(string s1, string s2, string s3) {
        int m = s1.size(), n = s2.size();
        if (m + n != (int)s3.size()) return false;
        
        // memo[i][j]: 0=未访问, 1=true, -1=false
        vector<vector<int>> memo(m + 1, vector<int>(n + 1, 0));
        return dfs(s1, s2, s3, 0, 0, memo);
    }
    
    bool dfs(string& s1, string& s2, string& s3, int i, int j, 
             vector<vector<int>>& memo) {
        if (i + j == (int)s3.size()) return true;
        if (memo[i][j] != 0) return memo[i][j] == 1;
        
        bool res = false;
        int k = i + j;
        if (i < (int)s1.size() && s1[i] == s3[k])
            res = dfs(s1, s2, s3, i + 1, j, memo);
        if (!res && j < (int)s2.size() && s2[j] == s3[k])
            res = dfs(s1, s2, s3, i, j + 1, memo);
        
        memo[i][j] = res ? 1 : -1;
        return res;
    }
};
```

**关键点**：memo 用三值而不是两值（0/1），因为要区分"还没算过"和"算过结果为 false"。

---

### 解法3: 二维 DP — O(m·n) / O(m·n) ⭐ 面试首选

**从记忆化翻译成递推**：把自顶向下的递归改成自底向上的填表。

```
// dp 填表示意 (s1="aabcc", s2="dbbca", s3="aadbbcbcac"):
//
//       ""  d     b     b     c     a       ← s2
//  ""  [ T   F     F     F     F     F ]
//   a  [ T   F     F     F     F     F ]
//   a  [ T   T     T     T     T     F ]
//   b  [ F   T     T     F     T     F ]
//   c  [ F   F     T     T     T     T ]
//   c  [ F   F     F     T     F     T ]  ← 右下角 = true ✓
//   ↑
//  s1
//
// 填表方向: 从上到下, 每行从左到右
// dp[i][j] 依赖: dp[i-1][j] (正上方) 和 dp[i][j-1] (正左方)
```

```cpp
class Solution {
public:
    bool isInterleave(string s1, string s2, string s3) {
        int m = s1.size(), n = s2.size();
        if (m + n != (int)s3.size()) return false;
        
        // dp[i][j] = s1前i个 + s2前j个 能否交错组成 s3前(i+j)个
        vector<vector<bool>> dp(m + 1, vector<bool>(n + 1, false));
        dp[0][0] = true;
        
        // 第一列: 只用 s1
        for (int i = 1; i <= m; i++)
            dp[i][0] = dp[i-1][0] && (s1[i-1] == s3[i-1]);
        
        // 第一行: 只用 s2
        for (int j = 1; j <= n; j++)
            dp[0][j] = dp[0][j-1] && (s2[j-1] == s3[j-1]);
        
        // 填表
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                // s3[i+j-1] 这个字符要么来自 s1[i-1]，要么来自 s2[j-1]
                dp[i][j] = (dp[i-1][j] && s1[i-1] == s3[i+j-1])  // 从上方来
                        || (dp[i][j-1] && s2[j-1] == s3[i+j-1]);  // 从左方来
            }
        }
        
        return dp[m][n];
    }
};
```

**关键点**：
- `s3[i+j-1]` 中的 `-1` 是因为字符串 0-indexed 而 dp 是 1-indexed
- 第一行和第一列的初始化具有"链式"特点：一旦断了（不匹配），后面全是 false

---

### 解法4: 一维 DP（空间优化）— O(m·n) / O(n) 🎯 Follow-up

**从解法3优化**：`dp[i][j]` 只依赖 `dp[i-1][j]`（上一行同列）和 `dp[i][j-1]`（本行前一列）。经典的可以压缩成一维数组的模式。

```
// 空间优化原理:
//
// 二维版:  dp[i][j] = dp[i-1][j] || dp[i][j-1]
//                        ↑ 上一行       ↑ 本行左边
//
// 一维版:  dp[j] (更新前) = 上一行的 dp[i-1][j]  ← 天然就是!
//          dp[j-1] (更新后) = 本行的 dp[i][j-1]  ← 刚更新过!
//
// 所以从左到右遍历 j，dp[j] 自动对应正确的依赖关系
```

```cpp
class Solution {
public:
    bool isInterleave(string s1, string s2, string s3) {
        int m = s1.size(), n = s2.size();
        if (m + n != (int)s3.size()) return false;
        
        // dp[j] = 用 s1 前 i 个 + s2 前 j 个能否交错组成 s3 前 i+j 个
        vector<bool> dp(n + 1, false);
        dp[0] = true;
        
        // 初始化第一行 (i=0, 只用 s2)
        for (int j = 1; j <= n; j++)
            dp[j] = dp[j-1] && (s2[j-1] == s3[j-1]);
        
        for (int i = 1; i <= m; i++) {
            // dp[0] 对应只用 s1，需要逐行更新
            dp[0] = dp[0] && (s1[i-1] == s3[i-1]);
            
            for (int j = 1; j <= n; j++) {
                // dp[j] 更新前 = dp[i-1][j]（上方）
                // dp[j-1] 已更新 = dp[i][j-1]（左方）
                dp[j] = (dp[j] && s1[i-1] == s3[i+j-1])       // 从上方
                      || (dp[j-1] && s2[j-1] == s3[i+j-1]);    // 从左方
            }
        }
        
        return dp[n];
    }
};
```

**关键点**：别忘了每一行的开头 `dp[0]` 也需要更新——它对应 `dp[i][0]`，即只用 s1 的前 i 个字符。

## 解法对比

| | 暴力递归 | 记忆化搜索 | 二维 DP | 一维 DP |
|---|---|---|---|---|
| 时间 | O(2^(m+n)) | O(m·n) | O(m·n) | O(m·n) |
| 空间 | O(m+n) 栈 | O(m·n) | O(m·n) | O(n) |
| 思维难度 | 最低 | 低 | 中 | 中 |
| 面试推荐 | 说思路即可 | 可以作为推导过程 | **首选** | Follow-up 加分 |

**选择建议**：面试中先说暴力思路（30秒），然后解释为什么有重叠子问题（30秒），直接写二维 DP（3分钟），被追问空间优化时给出一维 DP。

## 易错点

1. **忘记长度校验**
   - ✗ 直接开始 DP，当 `s1.size() + s2.size() != s3.size()` 时 dp 表越界或给出错误结果
   - ✓ 第一行就判断 `if (m + n != s3.size()) return false;`

2. **s3 的索引算错**
   - ✗ `s3[i+j]` —— dp 的 `(i,j)` 表示"前 i 个"和"前 j 个"，s3 要取第 `i+j` 个字符，0-indexed 是 `s3[i+j-1]`
   - ✓ `s3[i+j-1]`

3. **一维 DP 忘记更新 dp[0]**
   - ✗ 只在初始化时设 `dp[0] = true`，后续不更新
   - ✓ 每轮外层循环开始时 `dp[0] = dp[0] && (s1[i-1] == s3[i-1])`，因为 `dp[i][0]` 依赖 `dp[i-1][0]`

4. **记忆化搜索 memo 用 bool 数组**
   - ✗ `vector<vector<bool>> memo` —— 无法区分"未访问"和"结果为 false"
   - ✓ 用 `vector<vector<int>>` 三值标记（0=未访问, 1=true, -1=false）

5. **第一行/第一列初始化不是独立的**
   - ✗ `dp[i][0] = (s1[i-1] == s3[i-1])` —— 只看当前字符，忽略了前面必须全部匹配
   - ✓ `dp[i][0] = dp[i-1][0] && (s1[i-1] == s3[i-1])` —— 链式依赖

## 面试追问

**Q1（基础理解）**：暴力递归的时间复杂度为什么是指数级？能举例说明吗？
> 当 s1="aa", s2="aa", s3="aaaa" 时，每一步 s1[i] 和 s2[j] 都等于 s3[k]，两个分支都要走，形成完全二叉树，节点数是 O(2^(m+n))。

**Q2（优化核心）**：为什么状态只需要 `(i, j)` 两个维度？s3 的位置不需要作为状态吗？
> 因为 `k = i + j` 是由 i 和 j 唯一确定的。我们从 s1 取了 i 个、从 s2 取了 j 个，s3 自然就匹配到了第 `i+j` 个位置。所以 k 是冗余的。

**Q3（空间优化 — Follow-up）**：能否只用 O(min(m, n)) 空间？
> 可以。一维 DP 滚动的是内层循环的那个维度。让较短的字符串作为 s2（内层），则空间 O(min(m,n))。如果 s1 比 s2 短，交换 s1 和 s2 即可（交错的定义是对称的）。

**Q4（变体）**：如果要求输出所有可能的交错方式（不只是判断 true/false），怎么做？
> 改用回溯 + 记忆化。不能简单用 DP 填表了，因为要记录路径。在 DFS 过程中记录每步选的是 s1 还是 s2，到达终点时收集一种方案。剪枝仍然可以用 memo（memo[i][j]=false 的直接跳过）。

## 相关题型

- **62. 不同路径** — 本题的 DP 网格模型和 62 题完全一样（从左上到右下，只能向右或向下），区别在于本题的"能否走"有字符匹配的约束
- **72. 编辑距离** — 同样是二维字符串 DP，dp[i][j] 依赖上方、左方、左上方，空间优化思路一致
- **115. 不同的子序列** — 也是两个字符串的二维 DP，但状态转移不同：115 是"s 的子序列匹配 t"，本题是"s1 和 s2 交错组成 s3"
- **10. 正则表达式匹配 / 44. 通配符匹配** — 同属"两个字符串的二维 DP"家族，复用相同的填表框架