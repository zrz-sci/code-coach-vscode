# 718. 最长重复子数组

## 核心思路

这道题本质上是求两个数组的**最长公共子数组**（连续），和"最长公共子序列(LCS)"非常相似，但关键区别在于子数组必须**连续**，这导致 DP 转移方程和 LCS 不同——不匹配时直接归零，不能继承之前的结果。

## 思维链

1. **读完题第一反应**：枚举 nums1 的所有子数组，看是否出现在 nums2 中。子数组总共 O(n²) 个，检查是否出现需要 O(m) 比较，暴力 O(n²m) 甚至 O(n³)。
2. **暴力解的瓶颈**：对于每一对起点 (i, j)，都要逐个字符比较看能匹配多长，大量重复计算。
3. **观察子问题结构**：如果 nums1[i] == nums2[j]，那么从 (i, j) 开始的最长公共子数组长度 = 从 (i+1, j+1) 开始的最长公共子数组长度 + 1。这就是 DP！
4. **定义 DP 状态**：dp[i][j] = 以 nums1[i-1] 和 nums2[j-1] 结尾的最长公共子数组长度。注意是"结尾"，不是"从此开始"——因为子数组必须连续，不匹配就断了。
5. **进一步优化**：dp[i] 只依赖 dp[i-1] 那一行，可以用一维滚动数组优化空间到 O(min(m,n))。
6. **更高级方法**：二分答案 + 滚动哈希，将问题转化为"是否存在长度为 k 的公共子数组"，二分 k 的值，用 Rabin-Karp 哈希 O(n+m) 判断，总体 O((n+m)log(min(n,m)))。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力枚举 | 枚举所有起点对，逐个比较 | O(n²m) | O(1) | 能说出即可 |
| 二维 DP | dp[i][j] 表示以 nums1[i-1], nums2[j-1] 结尾的最长公共长度 | O(nm) | O(nm) | ⭐ 必须写出 |
| 一维滚动 DP | 空间优化，只保留上一行 | O(nm) | O(min(n,m)) | ⭐ 必须写出 |
| 二分 + 滚动哈希 | 二分答案长度 k，Rabin-Karp 判断 | O((n+m)log(min(n,m))) | O(n+m) | 加分项 |

## 关键提示

1. **子数组 vs 子序列**：子数组必须连续！这意味着一旦 nums1[i] ≠ nums2[j]，连续性就断了，dp 归零。对比 LCS 的转移：不匹配时取 max(dp[i-1][j], dp[i][j-1])，而本题不匹配时 dp[i][j] = 0。
2. **答案不是 dp 表右下角**：和 LCS 不同，答案是 dp 表中的**最大值**（因为最长公共子数组可以在任意位置结束），不是 dp[m][n]。
3. **空间优化的陷阱**：如果用一维数组滚动，必须**从右往左**遍历 j，否则会用到本行已更新的值覆盖上一行的值。
4. **DP 填表示意**：
```
        ""  3  2  1  4  7
    ""   0  0  0  0  0  0
     1   0  0  0  1  0  0
     2   0  0  1  0  0  0
     3   0  1  0  0  0  0
     2   0  0  2  0  0  0
     1   0  0  0  3  0  0
                  ↑ 最大值=3, 对应 [3,2,1]
```
5. **二分 + 哈希的思路**：如果存在长度为 k 的公共子数组，那么必然存在长度为 k-1 的公共子数组（取前 k-1 个即可）。这种单调性允许二分。

## 解法详解

### 解法1: 暴力枚举 — O(n²m) / O(1)

**思考过程**：最直觉的做法——枚举 nums1 中每个位置 i 和 nums2 中每个位置 j 作为起点，然后逐个比较看能连续匹配多长。

```cpp
class Solution {
public:
    int findLength(vector<int>& nums1, vector<int>& nums2) {
        int m = nums1.size(), n = nums2.size(), ans = 0;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                int len = 0;
                // 从 (i, j) 开始逐个比较
                while (i + len < m && j + len < n 
                       && nums1[i + len] == nums2[j + len]) {
                    len++;
                }
                ans = max(ans, len);
            }
        }
        return ans;
    }
};
```

**关键点**：这个解法没什么技巧，但帮助理解题意。瓶颈在于大量重复比较——如果 (i,j) 匹配了 len 个，那么 (i+1,j+1) 的匹配信息其实已经包含在里面了。

### 解法2: 二维 DP — O(nm) / O(nm) ⭐ 面试首选

**从暴力优化**：暴力解中 (i,j) 的匹配长度和 (i+1,j+1) 的匹配长度有重叠。定义 dp[i][j] = 以 nums1[i-1] 和 nums2[j-1] **结尾**的最长公共子数组长度，转移方程：

- 若 nums1[i-1] == nums2[j-1]：dp[i][j] = dp[i-1][j-1] + 1
- 若 nums1[i-1] ≠ nums2[j-1]：dp[i][j] = 0（连续性断了！）

```cpp
class Solution {
public:
    int findLength(vector<int>& nums1, vector<int>& nums2) {
        int m = nums1.size(), n = nums2.size(), ans = 0;
        // dp[i][j]: 以 nums1[i-1] 和 nums2[j-1] 结尾的最长公共子数组长度
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
        
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (nums1[i - 1] == nums2[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                    ans = max(ans, dp[i][j]);
                }
                // 不等时 dp[i][j] 保持初始值 0，无需显式赋值
            }
        }
        return ans;
    }
};
```

**关键点**：
- 答案是 dp 表中的**最大值**，不是 dp[m][n]！
- dp 的索引偏移 1 是为了让 dp[0][...] 和 dp[...][0] 自然作为边界（长度为 0）

### 解法3: 一维滚动 DP — O(nm) / O(min(n,m)) ⭐ 面试首选

**从解法2优化**：dp[i][j] 只依赖 dp[i-1][j-1]（左上角），所以只需一行数组。但必须**从右往左**遍历，防止覆盖。

```cpp
class Solution {
public:
    int findLength(vector<int>& nums1, vector<int>& nums2) {
        int m = nums1.size(), n = nums2.size(), ans = 0;
        // 让较短的数组作为 dp 的维度，节省空间
        if (m < n) { swap(nums1, nums2); swap(m, n); }
        
        vector<int> dp(n + 1, 0);
        for (int i = 1; i <= m; i++) {
            // 从右往左遍历，因为 dp[j] 依赖的是上一行的 dp[j-1]
            // 如果从左往右，dp[j-1] 已经被本行覆盖了
            for (int j = n; j >= 1; j--) {
                if (nums1[i - 1] == nums2[j - 1]) {
                    dp[j] = dp[j - 1] + 1;
                    ans = max(ans, dp[j]);
                } else {
                    dp[j] = 0;  // 必须显式清零！
                }
            }
        }
        return ans;
    }
};
```

**关键点**：
- 不等时必须 `dp[j] = 0`！二维版本中 dp 初始化全是 0，但一维滚动时 dp[j] 可能保留了上一轮的非零值。
- 遍历方向必须从右往左，和 0-1 背包的空间优化原理相同。

### 解法4: 二分 + 滚动哈希 — O((n+m)log(min(n,m))) / O(n+m)

**全新思路**：如果存在长度为 k 的公共子数组，那么也存在长度 ≤ k 的。答案具有单调性，可以二分答案长度。对于每个候选长度 k，用 Rabin-Karp 滚动哈希 O(n+m) 判断是否存在。

```cpp
class Solution {
public:
    int findLength(vector<int>& nums1, vector<int>& nums2) {
        int m = nums1.size(), n = nums2.size();
        // 二分答案：最长公共子数组的长度
        int lo = 0, hi = min(m, n), ans = 0;
        
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (hasCommonSubarray(nums1, nums2, mid)) {
                ans = mid;
                lo = mid + 1;  // 试试更长的
            } else {
                hi = mid - 1;
            }
        }
        return ans;
    }
    
private:
    // 判断是否存在长度为 k 的公共子数组
    bool hasCommonSubarray(vector<int>& a, vector<int>& b, int k) {
        if (k == 0) return true;
        
        long long MOD = 1e9 + 7, BASE = 113;
        long long power = 1;  // BASE^(k-1) % MOD
        for (int i = 0; i < k - 1; i++)
            power = power * BASE % MOD;
        
        // 计算 a 中所有长度为 k 的子数组的哈希值
        unordered_set<long long> hashSet;
        long long h = 0;
        for (int i = 0; i < (int)a.size(); i++) {
            h = (h * BASE + a[i]) % MOD;
            if (i >= k)  // 滑出窗口最左边的元素
                h = (h - a[i - k] * power % MOD * BASE % MOD + MOD * 2) % MOD;
            if (i >= k - 1) hashSet.insert(h);
        }
        
        // 计算 b 中所有长度为 k 的子数组的哈希值，看是否有匹配
        h = 0;
        for (int i = 0; i < (int)b.size(); i++) {
            h = (h * BASE + b[i]) % MOD;
            if (i >= k)
                h = (h - b[i - k] * power % MOD * BASE % MOD + MOD * 2) % MOD;
            if (i >= k - 1 && hashSet.count(h)) {
                // 哈希冲突可能导致误判，严格来说应该二次验证
                return true;
            }
        }
        return false;
    }
};
```

**关键点**：滚动哈希可能有冲突，面试中提到这一点并说明可以用双哈希或二次验证解决，会是加分项。

## 解法对比

| | 暴力 | 二维 DP | 一维滚动 DP | 二分+哈希 |
|---|---|---|---|---|
| 时间 | O(n²m) | O(nm) | O(nm) | O((n+m)log(min)) |
| 空间 | O(1) | O(nm) | O(min(n,m)) | O(n+m) |
| 代码难度 | 简单 | 简单 | 中等 | 较难 |
| 面试推荐 | 展示思路 | ⭐首选 | ⭐追问空间优化 | 加分 |

**面试建议**：先写二维 DP，面试官追问空间优化时给出一维滚动。如果面试官继续追问更快的方法，提到二分+哈希的思路。

## 易错点

1. **答案取错位置**：
   - ✗ `return dp[m][n];` — 这是 LCS（最长公共子序列）的写法
   - ✓ 在填表过程中用 `ans = max(ans, dp[i][j])` 维护全局最大值

2. **一维滚动时忘记清零**：
   - ✗ 不等时跳过不管 → dp[j] 保留了上一行的旧值，导致错误延续
   - ✓ `dp[j] = 0;` 必须显式写

3. **一维滚动遍历方向错误**：
   - ✗ `for (int j = 1; j <= n; j++)` 从左往右 → dp[j-1] 已被本行覆盖
   - ✓ `for (int j = n; j >= 1; j--)` 从右往左

4. **与 LCS 转移方程混淆**：
   - ✗ 不等时 `dp[i][j] = max(dp[i-1][j], dp[i][j-1])` — 这是子序列，不是子数组
   - ✓ 不等时 `dp[i][j] = 0` — 连续性断了

## 面试追问

**Q1: 这道题和最长公共子序列(LCS)有什么区别？**
A: 子数组必须连续，子序列可以不连续。体现在 DP 上：不匹配时子数组 dp 归零（连续性断了），子序列取 max(上、左)（可以跳过不匹配的元素）。答案位置也不同：子数组是 dp 表最大值，子序列是 dp[m][n]。

**Q2: 能否优化空间到 O(min(n,m))？**
A: 可以，用一维滚动数组。由于 dp[i][j] 只依赖 dp[i-1][j-1]，和 0-1 背包一样从右往左遍历即可。注意不等时必须显式清零。

**Q3: 如果数组很长（10^5 级别），O(nm) 会 TLE，怎么办？**
A: 二分答案 + 滚动哈希。观察到"是否存在长度为 k 的公共子数组"对 k 具有单调性，用二分搜索答案长度 k，对每个 k 用 Rabin-Karp O(n+m) 判断，总体 O((n+m)log(min(n,m)))。

## 相关题型

- **1143. 最长公共子序列 (LCS)** — 对比学习：转移方程不同（不匹配时取 max vs 归零），答案位置不同（dp[m][n] vs dp 表最大值）
- **516. 最长回文子序列** — 也是二维 DP，但自身和自身反转比较
- **53. 最大子数组和** — 同样是"连续子数组"问题，dp 状态定义类似（以某位结尾）
- **300. 最长递增子序列** — 子序列问题的经典 DP，对比连续 vs 不连续的状态转移差异