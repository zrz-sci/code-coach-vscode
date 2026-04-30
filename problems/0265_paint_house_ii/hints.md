# 265. Paint House II (粉刷房子 II)

## 核心思路

这是 Paint House (256) 的推广版本，从 3 种颜色扩展到 k 种颜色。核心仍是 DP：
- `dp[i][j]` = 前 i 栋房子、第 i 栋刷颜色 j 的最小花费
- 转移：`dp[i][j] = costs[i][j] + min(dp[i-1][c])` 其中 `c != j`

关键优化：每一行只需记录**最小值**和**次小值**，就能在 O(1) 完成转移，总时间从 O(nk^2) 降到 O(nk)。

## 思维链

1. **状态定义**: dp[i][j] = 粉刷前 i+1 栋房子且第 i 栋用颜色 j 的最小总费用
2. **状态转移**: dp[i][j] = costs[i][j] + min(dp[i-1][c]) 其中 c != j
3. **暴力转移**: 对每个 j，遍历所有 c != j 找最小值 => O(nk^2)
4. **优化观察**: 上一行的最小值如果颜色不同于 j 就直接用，否则用次小值
5. **维护 min1, min2**: 每行计算完后记录最小值、次小值及其颜色索引
6. **空间优化**: 可以原地修改 costs 数组或只保留上一行状态

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| DP + min1/min2 优化 | O(nk) | O(1) | ⭐ 最优，面试必须掌握 |
| 朴素 DP | O(nk^2) | O(nk) | 容易理解但不满足 Follow-up |
| 空间优化朴素 DP | O(nk^2) | O(k) | 中间方案 |

## 关键提示

- **核心优化**: 每行只需要上一行的最小值(min1)、次小值(min2)和最小值对应的颜色索引(minIdx)
- 当 `j == minIdx` 时，用 `min2`；否则用 `min1`
- 可以直接在 costs 上做原地修改省空间
- 边界：只有一栋房子时直接返回 min(costs[0])

## 解法详解

### 解法1: DP + 最小/次小值优化 ⭐

```cpp
class Solution {
public:
    int minCostII(vector<vector<int>>& costs) {
        int n = costs.size(), k = costs[0].size();
        if (n == 0) return 0;
        if (k == 1) return n == 1 ? costs[0][0] : -1; // 只有1种颜色且多栋房子无解
        
        // 上一行的最小值、次小值、最小值颜色索引
        int prevMin1 = 0, prevMin2 = 0, prevIdx = -1;
        
        for (int i = 0; i < n; i++) {
            int curMin1 = INT_MAX, curMin2 = INT_MAX, curIdx = -1;
            for (int j = 0; j < k; j++) {
                // 如果颜色 j 和上一行最小值颜色相同，则用次小值
                int cost = costs[i][j] + (j == prevIdx ? prevMin2 : prevMin1);
                
                if (cost < curMin1) {
                    curMin2 = curMin1;
                    curMin1 = cost;
                    curIdx = j;
                } else if (cost < curMin2) {
                    curMin2 = cost;
                }
            }
            prevMin1 = curMin1;
            prevMin2 = curMin2;
            prevIdx = curIdx;
        }
        
        return prevMin1;
    }
};
```

### 解法2: 朴素 DP（O(nk^2)）

```cpp
class Solution {
public:
    int minCostII(vector<vector<int>>& costs) {
        int n = costs.size(), k = costs[0].size();
        // dp[j] = 上一栋房子刷颜色 j 的最小花费
        vector<int> dp(costs[0].begin(), costs[0].end());
        
        for (int i = 1; i < n; i++) {
            vector<int> ndp(k, INT_MAX);
            for (int j = 0; j < k; j++) {
                for (int c = 0; c < k; c++) {
                    if (c != j) {
                        ndp[j] = min(ndp[j], dp[c] + costs[i][j]);
                    }
                }
            }
            dp = ndp;
        }
        
        return *min_element(dp.begin(), dp.end());
    }
};
```

## 易错点

| ✗ 错误写法 | ✓ 正确写法 | 说明 |
|-----------|-----------|------|
| 只记录最小值 | 同时记录最小值和次小值 | j == minIdx 时需要次小值 |
| curMin2 初始化为 0 | curMin2 初始化为 INT_MAX | 初值必须足够大 |
| 第一行也加 prevMin1 | 第一行 prevMin1=0, prevMin2=0 | 第一行没有上一行 |
| k=1 时不特判 | k=1 且 n>1 无解 | 相邻房子颜色必须不同 |

## 面试追问

- **Q1**: 时间复杂度能否低于 O(nk)？
  → 不能，因为每个 costs[i][j] 至少访问一次
- **Q2**: 如果相邻两栋房子可以同色，但间隔一栋不能同色怎么办？
  → 状态需要记录前两栋房子的颜色，dp[i][j1][j2]
- **Q3**: 如果要输出具体涂色方案而不只是最小花费？
  → 额外记录 choice[i] 数组，回溯构造方案

## 相关题型

- [256. Paint House](https://leetcode.com/problems/paint-house/) - 3 种颜色版本
- [1473. Paint House III](https://leetcode.com/problems/paint-house-iii/) - 分组约束
- [198. House Robber](https://leetcode.com/problems/house-robber/) - 类似"不相邻"约束的 DP
- [121. Best Time to Buy and Sell Stock](https://leetcode.com/problems/best-time-to-buy-and-sell-stock/) - 状态优化思想
