# 375. Guess Number Higher or Lower II - 猜数字大小 II

## 核心思路
极小化极大博弈 + 区间 DP：在 [1, n] 中选择一个猜测策略，使得最坏情况下的代价最小。对每个区间 [i, j]，枚举猜测点 k，取 `k + max(dp[i][k-1], dp[k+1][j])` 的最小值。

## 思维链
1. 不是简单二分！猜错要付猜的数字金额，大数代价高
2. 对手会选最坏情况（让你付最多钱），你要选最优策略（付最少钱保证赢）
3. 定义 `dp[i][j]` = 在 [i, j] 范围内保证猜对所需的最少金额
4. 枚举猜 k (i <= k <= j)，猜错付 k 元，然后在 [i,k-1] 或 [k+1,j] 继续
5. 对手选最坏情况：`cost(k) = k + max(dp[i][k-1], dp[k+1][j])`
6. 我们选最优策略：`dp[i][j] = min(cost(k)) for all k in [i,j]`
7. Base case：`dp[i][i] = 0`（只有一个数直接猜对），`dp[i][i-1] = 0`（空区间）

## 解法概览表 ⭐

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| 区间 DP | O(n^3) | O(n^2) | 标准解法，自底向上 |
| 记忆化搜索 | O(n^3) | O(n^2) | 自顶向下，更直观 |

## 关键提示
1. **极小化极大**：你要最小化对手能让你付的最大代价
2. **区间 DP 按长度枚举**：先算短区间，再算长区间
3. **dp[i][j] 仅当 i < j 有意义**，i == j 时为 0
4. **不要猜区间端点附近的大数**：通常最优猜测点偏向中间偏左
5. **n=1 返回 0**，n=2 返回 1（猜小的那个）

## 解法详解

### 解法一：区间 DP（推荐）⭐
**思路**：按区间长度从小到大填表，对每个区间枚举猜测点。

```cpp
class Solution {
public:
    int getMoneyAmount(int n) {
        // dp[i][j] = 在 [i,j] 范围保证猜对的最少金额
        vector<vector<int>> dp(n + 2, vector<int>(n + 2, 0));
        
        // 按区间长度枚举
        for (int len = 2; len <= n; len++) {
            for (int i = 1; i + len - 1 <= n; i++) {
                int j = i + len - 1;
                dp[i][j] = INT_MAX;
                for (int k = i; k <= j; k++) {
                    int cost = k + max(dp[i][k - 1], dp[k + 1][j]);
                    dp[i][j] = min(dp[i][j], cost);
                }
            }
        }
        return dp[1][n];
    }
};
```

### 解法二：记忆化搜索
**思路**：自顶向下递归，用 memo 数组缓存结果。

```cpp
class Solution {
    vector<vector<int>> memo;
    
    int solve(int i, int j) {
        if (i >= j) return 0;
        if (memo[i][j] != -1) return memo[i][j];
        
        int res = INT_MAX;
        for (int k = i; k <= j; k++) {
            int cost = k + max(solve(i, k - 1), solve(k + 1, j));
            res = min(res, cost);
        }
        return memo[i][j] = res;
    }
    
public:
    int getMoneyAmount(int n) {
        memo.assign(n + 1, vector<int>(n + 1, -1));
        return solve(1, n);
    }
};
```

## 易错点
- ✗ 误以为二分猜中间就是最优策略 → ✓ 最优猜测点不一定在中间，因为大数代价高
- ✗ dp 数组大小不够导致越界（k+1 可能到 n+1）→ ✓ 开 (n+2) x (n+2)
- ✗ 枚举区间时忘记按长度从小到大 → ✓ 外层循环是长度 len，内层是起点 i
- ✗ 混淆 min 和 max 的嵌套关系 → ✓ 外层 min（我选最优），内层 max（对手选最差）
- ✗ base case 忘记 dp[i][i] = 0 → ✓ 只有一个数不用猜，代价为 0

## 面试追问

**Q1: 为什么不能直接用二分策略？举例说明。**
A: 对于 n=3：二分猜 2，最坏代价 = 2（猜错后范围变 {1} 或 {3}，都直接猜对）。但猜 1 的最坏代价 = 1+2=3。猜 2 确实是最优。但对于更大 n，二分不是最优：n=10 时二分猜 5 的最坏代价是 5+8+9=22，而最优策略猜 7 的最坏代价是 16。

**Q2: 时间复杂度能优化吗？**
A: 枚举 k 时可以用单调性剪枝。观察到随着 i 增大，最优 k 也倾向增大，可以记录上次的最优 k 来缩小搜索范围。但仍然是 O(n^3) 最坏情况。

**Q3: 这道题和 Minimax 算法的关系？**
A: 这就是经典的 Minimax 决策：你（min 玩家）要最小化代价，对手（max 玩家/自然选择最坏情况）要最大化代价。dp[i][j] = min over k { k + max(left, right) } 就是 Minimax 递推式。

## 相关题型
- [374. Guess Number Higher or Lower](../0374_guess_number_higher_or_lower/) - 基础版二分查找
- [464. Can I Win](../0464_can_i_win/) - 极小化极大博弈
- [486. Predict the Winner](../0486_predict_the_winner/) - 区间 DP 博弈
- [877. Stone Game](../0877_stone_game/) - 博弈 DP
