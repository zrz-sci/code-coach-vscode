# 486. 预测赢家 (Predict the Winner)

## 核心思路

经典博弈论区间 DP。定义 dp[i][j] 为在 nums[i..j] 中，当前玩家相对对手的最大净得分差。
当前玩家选左端得 nums[i] - dp[i+1][j]，选右端得 nums[j] - dp[i][j-1]，取两者最大值。
最终判断 dp[0][n-1] >= 0。

## 思维链

1. **博弈建模**：双人零和博弈，双方都最优策略，需要用 Minimax 思想
2. **状态定义**：dp[i][j] = 当前玩家面对 nums[i..j] 时，能获得的最大净分差（自己得分 - 对手得分）
3. **状态转移**：选左 nums[i] 后对手面对 dp[i+1][j]，净差 = nums[i] - dp[i+1][j]；选右类似
4. **取最优**：dp[i][j] = max(nums[i] - dp[i+1][j], nums[j] - dp[i][j-1])
5. **基础情况**：dp[i][i] = nums[i]（只剩一个数，当前玩家必取）
6. **最终判断**：dp[0][n-1] >= 0 则玩家1赢

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| 区间DP (2D) | O(n^2) | O(n^2) | ⭐ 经典博弈DP |
| 空间优化1D | O(n^2) | O(n) | 滚动数组优化 |
| 记忆化递归 | O(n^2) | O(n^2) | 直觉清晰 |

## 关键提示

1. "净分差"定义巧妙：dp[i][j] 代表当前玩家比对手多多少分，避免跟踪两人分数
2. 转移中的减号：选了 nums[i] 后，对手成为"当前玩家"，所以减去 dp[i+1][j]
3. n <= 20，O(n^2) 完全够用
4. 平局时玩家1也算赢（>= 0 而非 > 0）

## 解法详解

### 解法1: 区间 DP

```cpp
class Solution1 {
public:
    bool predictTheWinner(vector<int>& nums) {
        int n = nums.size();
        // dp[i][j]: 面对 nums[i..j] 时当前玩家的最大净分差
        vector<vector<int>> dp(n, vector<int>(n, 0));
        
        // 基础: 长度为 1 的区间
        for (int i = 0; i < n; i++) {
            dp[i][i] = nums[i];
        }
        
        // 按区间长度从小到大填表
        for (int len = 2; len <= n; len++) {
            for (int i = 0; i <= n - len; i++) {
                int j = i + len - 1;
                // 选左端: nums[i] - dp[i+1][j]
                // 选右端: nums[j] - dp[i][j-1]
                dp[i][j] = max(nums[i] - dp[i + 1][j],
                               nums[j] - dp[i][j - 1]);
            }
        }
        
        return dp[0][n - 1] >= 0;
    }
};
```

### 解法2: 空间优化 1D DP

```cpp
class Solution2 {
public:
    bool predictTheWinner(vector<int>& nums) {
        int n = nums.size();
        vector<int> dp(n);
        
        for (int i = 0; i < n; i++) dp[i] = nums[i];
        
        for (int len = 2; len <= n; len++) {
            for (int i = 0; i <= n - len; i++) {
                int j = i + len - 1;
                dp[i] = max(nums[i] - dp[i + 1], nums[j] - dp[i]);
            }
        }
        
        return dp[0] >= 0;
    }
};
```

### 解法3: 记忆化递归

```cpp
class Solution3 {
public:
    bool predictTheWinner(vector<int>& nums) {
        int n = nums.size();
        vector<vector<int>> memo(n, vector<int>(n, INT_MIN));
        return maxDiff(nums, 0, n - 1, memo) >= 0;
    }
    
    int maxDiff(vector<int>& nums, int i, int j, vector<vector<int>>& memo) {
        if (i == j) return nums[i];
        if (memo[i][j] != INT_MIN) return memo[i][j];
        
        int pickLeft = nums[i] - maxDiff(nums, i + 1, j, memo);
        int pickRight = nums[j] - maxDiff(nums, i, j - 1, memo);
        memo[i][j] = max(pickLeft, pickRight);
        return memo[i][j];
    }
};
```

## 易错点

| ✗ 错误写法 | ✓ 正确写法 | 原因 |
|-----------|-----------|------|
| `dp[i][j] = max(nums[i]+dp[i+1][j], ...)` | `dp[i][j] = max(nums[i]-dp[i+1][j], ...)` | 减号：对手的净差对我来说是负的 |
| `return dp[0][n-1] > 0` | `return dp[0][n-1] >= 0` | 平局玩家1也算赢 |
| `for len=1` 开始 | `len=1` 是基础情况已初始化 | 区间长度从 2 开始填表 |
| `memo` 初始化为 0 | 初始化为 INT_MIN | 0 可能是合法的 dp 值 |

## 面试追问

**Q1: 为什么定义"净分差"而不是分别跟踪两人分数？**
A: 零和博弈中两人总分固定（= sum(nums)），只需一个变量表示差值。dp[i][j] 表示当前玩家相对对手的优势，简化了状态定义和转移。

**Q2: 这道题和 877. 石子游戏 有什么区别？**
A: 877 题数组长度为偶数且保证和为奇数，先手必胜可以数学证明（按奇偶位分组）。本题长度可奇可偶，需要实际 DP 计算。

**Q3: 如何理解转移方程中的减号？**
A: dp[i][j] 是"当前玩家的净差"。选了 nums[i] 后，对手面对 [i+1..j]，对手的净差是 dp[i+1][j]。当前玩家的净差 = nums[i] - (对手的净差) = nums[i] - dp[i+1][j]。

## 相关题型

- [877. 石子游戏](../0877_stone_game/) - 偶数长度博弈
- [1140. 石子游戏 II](../1140_stone_game_ii/) - 博弈+DP
- [464. 我能赢吗](../0464_can_i_win/) - 状态压缩博弈
- [375. 猜数字大小 II](../0375_guess_number_higher_or_lower_ii/) - Minimax DP
