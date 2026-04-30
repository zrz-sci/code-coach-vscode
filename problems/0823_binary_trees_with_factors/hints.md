# 823. Binary Trees With Factors

## 核心思路

排序后用**动态规划**，对每个数 `arr[i]`，枚举所有可能的左右子节点因子对 `(arr[j], arr[i]/arr[j])`。`dp[val]` 表示以 `val` 为根的二叉树数量。本质是**因子分解 + DP 计数**。

## 思维链

1. **排序数组**：从小到大排序，保证处理 `arr[i]` 时，所有可能的因子都已处理完毕
2. **初始化 DP**：每个数字本身作为单节点树，`dp[arr[i]] = 1`
3. **枚举因子对**：对每个 `arr[i]`，遍历所有 `arr[j] < arr[i]`，若 `arr[i] % arr[j] == 0` 且 `arr[i]/arr[j]` 也在数组中
4. **状态转移**：`dp[arr[i]] += dp[arr[j]] * dp[arr[i]/arr[j]]`（左子树方案数 * 右子树方案数）
5. **求和取模**：累加所有 `dp[arr[i]]`，对 1e9+7 取模
6. **用 HashMap 快速查找**：用 `unordered_map` 存储数字是否在数组中以及对应的 dp 值

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| 排序 + DP + HashMap ⭐ | O(N^2) | O(N) | 最优解，因子枚举 |
| 排序 + DP + 二分查找 | O(N^2 logN) | O(N) | 用二分代替 HashMap |

## 关键提示

- **排序是前提**：必须从小到大处理，确保子节点的 dp 值已计算
- **因子对称性**：`(a, b)` 和 `(b, a)` 是两棵不同的树（左右子节点互换），所以不需要除以 2
- **取模时机**：乘法和加法都要取模，防止 long long 溢出
- **HashMap 加速**：用 `unordered_map<long long, long long>` 存储 dp 值，O(1) 查找因子

## 解法详解

### 解法一：排序 + DP + HashMap（推荐）

```cpp
class Solution {
public:
    int numFactoredBinaryTrees(vector<int>& arr) {
        const int MOD = 1e9 + 7;
        sort(arr.begin(), arr.end());
        
        unordered_map<long long, long long> dp;
        // 每个数字自身是一棵单节点树
        for (int x : arr) dp[x] = 1;
        
        for (int i = 0; i < (int)arr.size(); i++) {
            long long val = arr[i];
            for (int j = 0; j < i; j++) {
                long long left = arr[j];
                if (val % left == 0) {
                    long long right = val / left;
                    if (dp.count(right)) {
                        dp[val] = (dp[val] + dp[left] * dp[right]) % MOD;
                    }
                }
            }
        }
        
        long long result = 0;
        for (auto& [k, v] : dp) {
            result = (result + v) % MOD;
        }
        return result;
    }
};
```

### 解法二：排序 + DP + 索引映射

```cpp
class Solution {
public:
    int numFactoredBinaryTrees(vector<int>& arr) {
        const int MOD = 1e9 + 7;
        int n = arr.size();
        sort(arr.begin(), arr.end());
        
        // 值到索引的映射
        unordered_map<int, int> idx;
        for (int i = 0; i < n; i++) idx[arr[i]] = i;
        
        vector<long long> dp(n, 1); // 每个数字至少是一棵单节点树
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < i; j++) {
                if ((long long)arr[i] % arr[j] == 0) {
                    long long right = arr[i] / arr[j];
                    if (idx.count(right)) {
                        dp[i] = (dp[i] + dp[j] * dp[idx[right]]) % MOD;
                    }
                }
            }
        }
        
        long long result = 0;
        for (long long x : dp) result = (result + x) % MOD;
        return result;
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ 不排序直接 DP | ✓ 必须先排序 | 否则可能用到未计算的子问题 |
| ✗ `arr[i] % arr[j]` 溢出 | ✓ `(long long)arr[i] % arr[j]` | arr 值最大 1e9，乘法会溢出 int |
| ✗ 只累加最后一个 dp 值 | ✓ 累加所有 dp[arr[i]] | 每个值都可以作为树根 |
| ✗ 忘记每个数字初始 dp 值为 1 | ✓ `dp[x] = 1` | 单节点树也是合法的二叉树 |
| ✗ 对 `dp[left] * dp[right]` 不取模 | ✓ 每次乘法后取模 | 两个 long long 相乘可能溢出 |

## 面试追问

**Q1: 为什么 (a, b) 和 (b, a) 算不同的树？**
> 因为二叉树的左右子节点是有区别的。`[10, 2, 5]` 和 `[10, 5, 2]` 是两棵不同的二叉树（左右子树互换）。所以枚举因子对时不需要去重。

**Q2: 时间复杂度能否优化到 O(N * sqrt(max_val))？**
> 可以。对每个 `arr[i]`，只枚举到 `sqrt(arr[i])` 的因子，找到因子对 `(j, arr[i]/j)` 后同时处理两个方向。这在数组值很大但数组长度不大时更高效。

**Q3: 如果允许 N-ary 树（每个节点可以有任意数量的子节点，值等于所有子节点的乘积），如何扩展？**
> 需要将因子分解从两因子扩展到多因子。可以用递归 DP：对每个值 v，枚举第一个因子 f，然后递归计算 v/f 的多因子分解方案。复杂度会显著增加。

## 相关题型

- [LC 96] Unique Binary Search Trees（DP 计数二叉树）
- [LC 95] Unique Binary Search Trees II（生成所有 BST）
- [LC 343] Integer Break（因子分解 DP）
- [LC 279] Perfect Squares（DP + 因子枚举）
