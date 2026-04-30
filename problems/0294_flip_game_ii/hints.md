# 294. Flip Game II (翻转游戏 II)

**难度**: Medium | **标签**: Memoization, Math, Dynamic Programming, Backtracking, Game Theory

## 核心思路

博弈搜索：当前玩家尝试每一步合法操作，如果存在某一步使得对手**无法获胜**（必败），则当前玩家必胜。用记忆化避免重复状态计算。

## 思维链

1. 这是一个**组合博弈**（Combinatorial Game Theory）问题
2. 先手必胜 <=> 存在一步操作使得对手进入必败状态
3. 必败状态 = 无论怎么操作，对手都能赢
4. 递归 + 记忆化：以字符串状态为 key，缓存结果
5. 进阶优化：Sprague-Grundy 定理将问题分解为独立子游戏

## 解法概览

| 解法 | 思路 | 时间复杂度 | 空间复杂度 | 推荐 |
|------|------|-----------|-----------|------|
| 回溯 + 记忆化 | 枚举操作 + memo | O(2^n) | O(2^n) | ⭐ |
| Sprague-Grundy | SG值分解子游戏 | O(n^2) | O(n) | |

## 关键提示

```
博弈树示意 ("++++"):

        ++++           <- 先手
       / | \
   --++ +--+ ++--      <- 后手
   /     |     \
  ...  +--+   ...     <- 无法操作 => 对手必败
       后手无法翻转 => 先手必胜！

核心逻辑:
canWin(state):
  for each valid move:
    next_state = apply(move)
    if !canWin(next_state):   // 对手必败
      return true             // 当前必胜
  return false                // 所有操作对手都赢 => 必败

记忆化关键:
  "+--++--+" 可能从多条路径到达
  用 HashMap<string, bool> 缓存结果
```

## 解法详解

### 解法一：回溯 + 记忆化（推荐）

```cpp
class Solution {
public:
    bool canWin(string currentState) {
        unordered_map<string, bool> memo;
        return canWinHelper(currentState, memo);
    }
    
private:
    bool canWinHelper(string& s, unordered_map<string, bool>& memo) {
        if (memo.count(s)) return memo[s];
        
        for (int i = 0; i + 1 < (int)s.size(); i++) {
            if (s[i] == '+' && s[i + 1] == '+') {
                s[i] = s[i + 1] = '-';  // 操作
                
                bool opponentWins = canWinHelper(s, memo);
                
                s[i] = s[i + 1] = '+';  // 还原
                
                if (!opponentWins) {
                    memo[s] = true;
                    return true;
                }
            }
        }
        
        memo[s] = false;
        return false;
    }
};
```

### 解法二：Sprague-Grundy 定理

```cpp
class Solution {
public:
    bool canWin(string currentState) {
        // 将字符串按 '-' 分割成连续 '+' 的段
        // 每段是独立子游戏，计算 SG 值后 XOR
        int grundy = 0;
        int count = 0;
        
        for (char c : currentState) {
            if (c == '+') {
                count++;
            } else {
                if (count >= 2) {
                    grundy ^= calcGrundy(count);
                }
                count = 0;
            }
        }
        if (count >= 2) {
            grundy ^= calcGrundy(count);
        }
        
        return grundy != 0;
    }
    
private:
    unordered_map<int, int> sgCache;
    
    int calcGrundy(int n) {
        if (n <= 1) return 0;
        if (sgCache.count(n)) return sgCache[n];
        
        unordered_set<int> reachable;
        for (int i = 0; i + 1 < n; i++) {
            // 翻转位置 i 和 i+1 后，分裂成长度为 i 和 n-i-2 的两段
            int left = i;
            int right = n - i - 2;
            int sg = 0;
            if (left >= 2) sg ^= calcGrundy(left);
            if (right >= 2) sg ^= calcGrundy(right);
            reachable.insert(sg);
        }
        
        // 计算 mex (minimum excludant)
        int mex = 0;
        while (reachable.count(mex)) mex++;
        
        sgCache[n] = mex;
        return mex;
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ `if (canWinHelper(next))` return true | ✓ `if (!canWinHelper(next))` return true | 对手输了自己才赢！逻辑反转 |
| ✗ 不用记忆化 | ✓ 用 memo 缓存状态 | 重复状态很多，不缓存超时 |
| ✗ memo 用引用传字符串 | ✓ 操作前/后修改同一字符串 | 确保 memo 的 key 是当前状态 |
| ✗ SG分段忘记处理末尾 | ✓ 循环结束后检查最后一段 | 最后一段连续 '+' 可能没被处理 |

## 面试追问

**Q1: 为什么用记忆化？不同操作顺序可能到达相同状态吗？**
> 是的。例如 "++++" 先翻 [0,1] 再翻 [2,3] 和先翻 [2,3] 再翻 [0,1] 得到同一个 "----"。状态数远少于操作序列数。

**Q2: Sprague-Grundy 定理的核心思想？**
> 将博弈分解为若干独立子游戏，每个子游戏的 SG 值等于它所有后继状态 SG 值的 mex（最小排除值）。总游戏的 SG 值等于各子游戏 SG 值的 XOR。SG != 0 则先手必胜。

**Q3: 时间复杂度对比？**
> 记忆化回溯：最坏 O(2^n) 个不同状态。SG 方法：只需计算长度 1..n 的 SG 值，O(n^2)。

## 相关题型

| 题号 | 题目 | 关联 |
|------|------|------|
| 293 | Flip Game | 本题的基础子问题 |
| 292 | Nim Game | 经典博弈论 |
| 464 | Can I Win | 带记忆化的博弈 |
| 486 | Predict the Winner | 博弈 DP |
| 877 | Stone Game | 博弈论 + DP |
