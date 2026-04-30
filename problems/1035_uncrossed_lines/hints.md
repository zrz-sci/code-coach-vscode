# 1035. 不相交的线

## 核心思路

**这道题本质上就是求两个数组的最长公共子序列（LCS）。** 连线不相交 ⟺ 配对的下标在两个数组中都是递增的 ⟺ 公共子序列。

## 思维链

1. **读完题第一反应**：两行数字之间连线，相同的才能连，线不能交叉。暴力做法？枚举所有可能的连线组合，检查哪些不交叉，取最大集合 → 指数级复杂度。

2. **关键观察 — "不交叉"意味着什么？** 如果我们连了 `(i1, j1)` 和 `(i2, j2)` 两条线，不交叉要求：`i1 < i2` 时必须 `j1 < j2`。也就是说，选出的配对下标在两个数组中**都保持递增**。

3. **这不就是公共子序列吗？** 从 nums1 和 nums2 中各选出一个子序列（保持原始顺序），使得两个子序列完全相同，且尽量长。这正是经典的 **最长公共子序列（LCS）** 问题！

4. **LCS 的 DP 解法**：定义 `dp[i][j]` = nums1 前 i 个和 nums2 前 j 个的 LCS 长度。如果 `nums1[i-1] == nums2[j-1]`，就能在之前的基础上多连一条线；否则取不用 i 或不用 j 的较大值。

5. **空间优化**：每行只依赖上一行，可以用一维滚动数组把空间从 O(mn) 降到 O(n)。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力递归 | 穷举所有匹配组合 | O(2^(m+n)) | O(m+n) | 能说出即可 |
| 记忆化搜索 | 自顶向下 + memo | O(mn) | O(mn) | 能说出即可 |
| 二维 DP | 经典 LCS 递推 | O(mn) | O(mn) | ⭐ 必须写出 |
| 一维 DP（空间优化） | 滚动数组 | O(mn) | O(n) | 加分项 |

## 关键提示

1. **画一画连线图**：把示例1画出来，尝试找不交叉的最大连线数。你会发现"不交叉"就是要求选出的配对下标保持顺序——这就是子序列！

2. **和 LeetCode 1143（最长公共子序列）对比**：如果你做过 1143，这道题的代码**一模一样**。区别只是 1143 输入是字符串，这里是整数数组。

3. **DP 状态定义**：`dp[i][j]` 表示 nums1[0..i-1] 和 nums2[0..j-1] 的最大连线数。注意用"前 i 个"而不是"到下标 i"，这样 `dp[0][*]` 和 `dp[*][0]` 自然为 0，不需要特殊初始化。

4. **转移方程只有两种情况**：
   - `nums1[i-1] == nums2[j-1]`：一定连这条线，`dp[i][j] = dp[i-1][j-1] + 1`
   - 不相等：不连这条线，`dp[i][j] = max(dp[i-1][j], dp[i][j-1])`

5. **空间优化的陷阱**：压缩到一维后，`dp[i-1][j-1]` 会被覆盖，需要一个临时变量保存。

## 解法详解

### 解法1: 暴力递归 — O(2^(m+n)) / O(m+n)

**思考过程**：对于 nums1 的每个位置 i 和 nums2 的每个位置 j，如果值相同可以选择连或不连，不同则只能跳过。

```cpp
class Solution {
public:
    int maxUncrossedLines(vector<int>& nums1, vector<int>& nums2) {
        return dfs(nums1, nums2, 0, 0);
    }
    
    int dfs(vector<int>& a, vector<int>& b, int i, int j) {
        if (i == a.size() || j == b.size()) return 0;
        if (a[i] == b[j]) {
            // 相同就连线，两边同时前进
            return 1 + dfs(a, b, i + 1, j + 1);
        }
        // 不相同，跳过 a[i] 或跳过 b[j]
        return max(dfs(a, b, i + 1, j), dfs(a, b, i, j + 1));
    }
};
```

**关键点**：会超时，但帮助理解递归结构。注意当 `a[i] == b[j]` 时，直接连线一定不亏（可以反证：如果不连这条线反而更优，那把这条线加上也不会更差，因为它不影响前面的选择）。

### 解法2: 记忆化搜索 — O(mn) / O(mn)

**从解法1优化**：递归中 `(i, j)` 状态会被重复计算。加一个 memo 数组就行。

```cpp
class Solution {
public:
    int maxUncrossedLines(vector<int>& nums1, vector<int>& nums2) {
        int m = nums1.size(), n = nums2.size();
        vector<vector<int>> memo(m, vector<int>(n, -1));
        return dfs(nums1, nums2, 0, 0, memo);
    }
    
    int dfs(vector<int>& a, vector<int>& b, int i, int j, 
            vector<vector<int>>& memo) {
        if (i == a.size() || j == b.size()) return 0;
        if (memo[i][j] != -1) return memo[i][j];
        
        if (a[i] == b[j]) {
            memo[i][j] = 1 + dfs(a, b, i + 1, j + 1, memo);
        } else {
            memo[i][j] = max(dfs(a, b, i + 1, j, memo), 
                            dfs(a, b, i, j + 1, memo));
        }
        return memo[i][j];
    }
};
```

### 解法3: 二维 DP（经典 LCS）— O(mn) / O(mn) ⭐ 面试首选

**从记忆化搜索翻译**：把自顶向下的递归翻译成自底向上的递推。

```
DP 填表方向 (nums1 = [1,4,2], nums2 = [1,2,4]):

        ""  1   2   4
    ""   0  0   0   0
     1   0  1   1   1
     4   0  1   1   2
     2   0  1   2   2  ← 答案 dp[3][3] = 2

填表方向：从左到右，从上到下
dp[i][j] 依赖：dp[i-1][j-1]（左上）、dp[i-1][j]（上）、dp[i][j-1]（左）
```

```cpp
class Solution {
public:
    int maxUncrossedLines(vector<int>& nums1, vector<int>& nums2) {
        int m = nums1.size(), n = nums2.size();
        // dp[i][j]: nums1前i个 与 nums2前j个 的最大连线数
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
        
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (nums1[i - 1] == nums2[j - 1]) {
                    // 值相同，连线！在去掉这两个元素的基础上 +1
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                } else {
                    // 值不同，取跳过 nums1[i-1] 或跳过 nums2[j-1] 的较大值
                    dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }
        return dp[m][n];
    }
};
```

**关键点**：这就是 LeetCode 1143 最长公共子序列的代码，一字不改。

### 解法4: 一维 DP（空间优化）— O(mn) / O(n) — 加分项

**从解法3优化**：dp[i][j] 只依赖上一行 dp[i-1][*] 和当前行左边 dp[i][j-1]。所以只需一维数组。

```cpp
class Solution {
public:
    int maxUncrossedLines(vector<int>& nums1, vector<int>& nums2) {
        int m = nums1.size(), n = nums2.size();
        vector<int> dp(n + 1, 0);
        
        for (int i = 1; i <= m; i++) {
            int prev = 0;  // 保存 dp[i-1][j-1]，即左上角的值
            for (int j = 1; j <= n; j++) {
                int temp = dp[j]; // 此时 dp[j] 还是上一行的值，即 dp[i-1][j]
                if (nums1[i - 1] == nums2[j - 1]) {
                    dp[j] = prev + 1;  // prev 就是 dp[i-1][j-1]
                } else {
                    dp[j] = max(dp[j], dp[j - 1]); // max(上, 左)
                }
                prev = temp; // 更新 prev 为 dp[i-1][j]，下次循环它就变成 dp[i-1][j-1]
            }
        }
        return dp[n];
    }
};
```

**关键点**：`prev` 变量的作用至关重要。在覆盖 `dp[j]` 之前先用 `temp` 保存旧值，然后赋给 `prev`。下一轮 j+1 时，`prev` 就是 `dp[i-1][(j+1)-1]` = `dp[i-1][j]`，正是我们需要的"左上角"。

## 解法对比

| | 暴力递归 | 记忆化 | 二维 DP | 一维 DP |
|---|---------|--------|---------|---------|
| 时间 | O(2^(m+n)) | O(mn) | O(mn) | O(mn) |
| 空间 | O(m+n) 栈 | O(mn) | O(mn) | O(n) |
| 代码难度 | 低 | 中 | 低 | 中 |
| 面试推荐 | ✗ | ✓ 可以作为思考起点 | ⭐ 最佳 | 追问时写 |

二维 DP 是面试首选：代码短、逻辑清晰、不易出错。空间优化版本在面试官追问"能否优化空间"时拿出来。

## 易错点

1. **下标偏移搞混**：
   - ✗ `dp[i][j]` 对应 `nums1[i]` 和 `nums2[j]` → 下标越界
   - ✓ `dp[i][j]` 对应 `nums1[i-1]` 和 `nums2[j-1]`，因为 dp 多了一行一列的 base case

2. **空间优化时忘记保存左上角**：
   - ✗ 直接 `dp[j] = dp[j-1] + 1` 用了已经被更新过的 `dp[j-1]`（当前行的），但相等时需要的是上一行的 `dp[j-1]`
   - ✓ 用 `prev` 变量在覆盖前保存 `dp[j]`，作为下一轮的左上角

3. **相等时贪心地以为要取 max**：
   - ✗ `dp[i][j] = max(dp[i-1][j-1] + 1, dp[i-1][j], dp[i][j-1])`
   - ✓ `dp[i][j] = dp[i-1][j-1] + 1`（相等时直接连线一定不亏，因为 `dp[i-1][j-1] + 1 >= max(dp[i-1][j], dp[i][j-1])` 恒成立）

## 面试追问

**Q1: 这道题和最长公共子序列（LCS）有什么关系？**
→ 完全等价。"不交叉的连线"意味着选出的匹配对的下标在两个数组中都递增，这就是公共子序列的定义。代码一模一样。

**Q2: 能否把空间从 O(mn) 优化到 O(min(m,n))？**
→ 可以。用一维滚动数组，且选较短的数组作为 dp 维度（如果 m > n 就交换）。这样空间是 O(min(m,n))。

**Q3: 如果要输出具体连了哪些线（不只是数量），怎么做？**
→ 在二维 DP 基础上回溯路径。从 `dp[m][n]` 开始：如果 `nums1[i-1] == nums2[j-1]`，记录这条连线并走到 `dp[i-1][j-1]`；否则走 `dp[i-1][j]` 和 `dp[i][j-1]` 中较大的方向。

**Q4: 如果两个数组都非常长（n = 10^5），O(n²) 会超时，有更优的做法吗？**
→ 如果值域较小或匹配对较少，可以将 LCS 转化为 LIS（最长递增子序列）问题，用 O(n log n) 的 patience sorting 来解。具体做法：把所有匹配对 (i,j) 找出来，按 i 排序后对 j 求 LIS。

## 相关题型

- **[1143. 最长公共子序列](https://leetcode.com/problems/longest-common-subsequence/)** — 本题的字符串版本，代码完全相同。先做 1143 再做本题，秒解。
- **[718. 最长重复子数组](https://leetcode.com/problems/maximum-length-of-repeated-subarray/)** — 区别：718 要求**连续**子数组，所以不匹配时 dp[i][j]=0 而不是取 max。
- **[300. 最长递增子序列](https://leetcode.com/problems/longest-increasing-subsequence/)** — LCS 可以转化为 LIS（上面 Q4 提到的优化方法），理解两者关系很重要。
- **[583. 两个字符串的删除操作](https://leetcode.com/problems/delete-operation-for-two-strings/)** — 复用 LCS：最少删除次数 = m + n - 2 * LCS长度。