# 464. Can I Win - 我能赢吗

## 核心思路
博弈 + 状态压缩 DP。用 bitmask 表示已使用数字的集合，记忆化搜索判断当前玩家是否能赢。

## 思维链
1. 每个数字只能用一次 -> 需要记录"哪些数字已被选"
2. maxChoosableInteger <= 20 -> 可以用一个 int 的 20 位做 bitmask
3. 当前玩家选一个数 i，如果 i >= remaining 则直接赢
4. 否则递归：如果对手在新状态下必输，则当前玩家赢
5. 剪枝：若所有数之和 < desiredTotal，谁都赢不了 -> false

## 解法概览表

| 解法 | 时间 | 空间 | 特点 |
|------|------|------|------|
| ⭐ Bitmask + 记忆化搜索 | O(2^n * n) | O(2^n) | 标准解法 |
| 状态压缩 DP（自底向上） | O(2^n * n) | O(2^n) | 理论可行但不直观 |

## 关键提示
1. **状态 = bitmask**：第 i 位为 1 表示数字 i+1 已被使用
2. **不需要记录"谁的回合"**：因为状态隐含了轮次（1的个数 = 已走步数）
3. **边界判断**：`desiredTotal <= 0` 时先手直接赢；总和不够时返回 false
4. 用 `unordered_map<int, bool>` 或 `vector<int>` 做备忘录

## 解法详解

### 解法一：Bitmask 记忆化搜索
```cpp
class Solution {
public:
    unordered_map<int, bool> memo;
    
    bool canIWin(int maxChoosableInteger, int desiredTotal) {
        if (desiredTotal <= 0) return true;
        int sum = maxChoosableInteger * (maxChoosableInteger + 1) / 2;
        if (sum < desiredTotal) return false;
        return canWin(maxChoosableInteger, desiredTotal, 0);
    }
    
    bool canWin(int n, int remaining, int used) {
        if (memo.count(used)) return memo[used];
        for (int i = 1; i <= n; i++) {
            if (used & (1 << i)) continue; // 已用过
            // 选 i 后直接赢，或对手在新状态下输
            if (i >= remaining || !canWin(n, remaining - i, used | (1 << i))) {
                memo[used] = true;
                return true;
            }
        }
        memo[used] = false;
        return false;
    }
};
```

## 易错点
- ✗ 忘记判断总和 < desiredTotal 的情况 -> ✓ 提前返回 false（平局）
- ✗ bitmask 记录状态时忘记排除已选数字 -> ✓ `if (used & (1 << i)) continue`
- ✗ 递归时忘记 remaining - i -> ✓ 每选一个数，剩余目标减少
- ✗ desiredTotal = 0 时返回 false -> ✓ 应返回 true（先手不选就已达标）

## 面试追问

**Q1: 为什么不需要记录"轮到谁"？**
> bitmask 中 1 的个数决定了当前是第几步，奇数步是先手、偶数步是后手。所以状态唯一确定了轮次。

**Q2: 时间复杂度为什么是 O(2^n * n)？**
> 共 2^n 种 bitmask 状态，每个状态最多遍历 n 个数字做选择，所以总时间 O(2^n * n)。

**Q3: 如果允许重复使用数字怎么办？**
> 那就不需要 bitmask，状态只有 remaining。但需要注意状态空间变为 O(desiredTotal)，且每个状态遍历 n 个选择。

## 相关题型
- [486. Predict the Winner](https://leetcode.com/problems/predict-the-winner/) - 区间DP博弈
- [877. Stone Game](https://leetcode.com/problems/stone-game/) - 经典博弈
- [1140. Stone Game II](https://leetcode.com/problems/stone-game-ii/) - 博弈 + 记忆化
