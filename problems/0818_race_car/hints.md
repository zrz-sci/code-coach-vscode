# 818. 赛车

## 核心思路

这是一个**最短路径/最优决策**问题。车从位置 0、速度 +1 出发，每步可以选择 Accelerate（加速）或 Reverse（反转），求到达 target 的最少操作次数。可以用 **BFS（最短路径）** 或 **DP（子问题分解）** 来解。DP 解法的关键洞察是：连续 k 次加速会到达位置 `2^k - 1`，所以先找到能"越过"或"刚好到达" target 的加速次数，再分情况讨论。

## 思维链

1. **读完题第一反应** → 状态 = (position, speed)，操作 = A 或 R。这是一个状态空间搜索问题。
2. **BFS 可行吗？** → 状态是 (pos, speed)，BFS 可以求最短操作序列。但状态空间可能很大，需要合理剪枝。
3. **速度的规律** → 连续 k 次 A 后：speed = 2^k，position 增加 2^k - 1。所以 A^k 后位置 = 2^k - 1。
4. **DP 怎么定义？** → dp[t] = 到达位置 t 的最少操作次数。
5. **DP 转移** → 对于目标 t，找 k 使得 2^k - 1 >= t：
   - 如果 2^k - 1 == t，直接 k 步搞定。
   - 如果 2^k - 1 > t，有两种策略：(a) 先越过再回头；(b) 先加速到 2^(k-1)-1 不够，反转再前进一小段后再反转继续前进。
6. **为什么 DP 能分解？** → 反转后子问题规模变小，满足最优子结构。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| BFS | 搜索 (pos, speed) 状态空间 | O(t * log t) | O(t * log t) | 直觉但需剪枝 |
| DP | dp[t] = 到达 t 的最少步数 | O(t * log t) | O(t) | 面试首选 |

## 关键提示

1. **加速的数学性质**：连续 k 次 A 后，位置增加 `2^k - 1`，速度变为 `2^k`。这是理解 DP 转移的基础。
2. **只需考虑两种大策略**：(a) 加速到刚好越过 target，反转回来；(b) 加速到不到 target，反转，小走一段，再反转继续前进。
3. **BFS 剪枝**：位置超过 2*target 的状态可以剪掉（太远了回来不划算）；速度过大也可以剪掉。
4. **DP 的 base case**：dp[0] = 0（已经在原点），如果 target = 2^k - 1 则 dp[target] = k。
5. **反转需要 1 步 + 1 步**：R 操作不改变位置但改变速度方向。如果要"调头再调头"（先 R 再前进 j 步再 R），额外开销是 2 步（两次 R）加上前进的步数。

## 解法详解

### 解法1: BFS — O(t * log t) / O(t * log t)

**思考过程**: 状态 = (position, speed)。从 (0, 1) 出发，每步两种选择。BFS 保证第一次到达 target 时步数最少。

```
状态转移:
  A: (pos, speed) → (pos + speed, speed * 2)
  R: (pos, speed) → (pos, speed > 0 ? -1 : 1)

剪枝条件:
  - 0 <= pos <= 2 * target (位置不超过两倍目标)
  - |speed| 合理范围内
```

```cpp
class Solution {
public:
    int racecar(int target) {
        queue<tuple<int, int, int>> q; // (pos, speed, steps)
        q.push({0, 1, 0});
        set<pair<int,int>> visited;
        visited.insert({0, 1});
        
        while (!q.empty()) {
            auto [pos, speed, steps] = q.front(); q.pop();
            if (pos == target) return steps;
            
            // A: accelerate
            int npos = pos + speed, nspeed = speed * 2;
            if (npos > 0 && npos < 2 * target && !visited.count({npos, nspeed})) {
                visited.insert({npos, nspeed});
                q.push({npos, nspeed, steps + 1});
            }
            // R: reverse
            nspeed = speed > 0 ? -1 : 1;
            if (!visited.count({pos, nspeed})) {
                visited.insert({pos, nspeed});
                q.push({pos, nspeed, steps + 1});
            }
        }
        return -1;
    }
};
```

---

### 解法2: DP — O(t * log t) / O(t)  面试首选

**核心推导**: 定义 dp[t] = 从 (0, +1) 到达位置 t 的最少操作数。

连续 k 次 A 到达位置 `2^k - 1`。找最小的 k 使得 `2^k - 1 >= t`：

- **Case 1**: `2^k - 1 == t` → dp[t] = k
- **Case 2**: `2^k - 1 > t` → 越过了 target
  - 策略 A: 先走 k 步到 `2^k - 1`，反转，回头走距离 `2^k - 1 - t`
  - dp[t] = k + 1 + dp[2^k - 1 - t]
- **Case 3**: 不越过，走 k-1 步到 `2^(k-1) - 1`，然后反转，走 j 步，再反转
  - 走 j 步(j = 0, 1, ..., k-2)后位置 = `2^(k-1) - 1 - (2^j - 1)`
  - dp[t] = (k-1) + 1 + j + 1 + dp[t - (2^(k-1) - 1) + (2^j - 1)]

```
示例: target = 6
k=3: 2^3-1=7 > 6

Case2: 走3步到7, 反转, 回头1步
  dp[6] = 3 + 1 + dp[1] = 4 + 1 = 5  (dp[1] = 1, 一次A)

Case3: 走2步到3, 反转, 走j步, 反转, 继续
  j=0: dp[6] = 2 + 1 + 0 + 1 + dp[3] = 4 + 2 = 6
  j=1: dp[6] = 2 + 1 + 1 + 1 + dp[2] = 5 + dp[2]
  需要先算 dp[2]: k=2, 2^2-1=3>2, dp[2] = 2+1+dp[0] = 3+0 = ...
  
最终 dp[6] = 5 ("AAARA")
```

```cpp
class Solution {
public:
    int racecar(int target) {
        vector<int> dp(target + 1, INT_MAX);
        dp[0] = 0;
        
        for (int t = 1; t <= target; t++) {
            int k = 1;
            while ((1 << k) - 1 < t) k++;
            
            // Case 1: 恰好到达
            if ((1 << k) - 1 == t) {
                dp[t] = k;
                continue;
            }
            
            // Case 2: 越过后反转回来
            dp[t] = min(dp[t], k + 1 + dp[(1 << k) - 1 - t]);
            
            // Case 3: 不到，反转走一小段再反转
            for (int j = 0; j < k - 1; j++) {
                int pos = (1 << (k-1)) - 1 - ((1 << j) - 1);
                dp[t] = min(dp[t], (k-1) + 1 + j + 1 + dp[t - pos]);
            }
        }
        return dp[target];
    }
};
```

## 易错点

1. **加速公式搞错**：
   - 错误：连续 k 次 A 到达位置 `2^k`
   - 正确：到达位置 `2^k - 1`（从 0 开始，速度 1,2,4,...,2^(k-1)，总和 = 2^k - 1）

2. **DP 转移中的反转开销**：
   - 每次 R 操作消耗 1 步。Case 3 中有两次反转（先 R 再前进再 R），所以是 `+1+j+1` 而不是 `+j`

3. **BFS 状态空间爆炸**：
   - 不剪枝的话状态空间巨大。必须限制 position 范围（0 到 2*target）和 speed 合理范围

4. **Case 3 中 j 的范围**：
   - j 从 0 到 k-2（不包括 k-1），因为 j = k-1 时走回了原点，等于什么都没做

5. **dp 数组初始化**：
   - 初始化为 INT_MAX，但要注意加法溢出。可以用一个大但不溢出的值如 `target + 1` 的某个倍数

## 面试追问

**Q1: BFS 和 DP 各自的优缺点？**
> BFS 直觉简单但需要仔细剪枝，状态空间可能很大。DP 需要数学推导但效率更高、代码更短。面试首选 DP。

**Q2: 为什么 Case 2 中越过后回来一定是最优子结构？**
> 越过 target 后反转，从 `2^k - 1` 回到 target 等价于从 0 到 `2^k - 1 - t` 的子问题（方向对称）。子问题规模严格小于 t（因为 `2^k - 1 - t < t`，否则 k-1 就够了），保证了递推的正确性。

**Q3: 时间复杂度怎么推导？**
> 外层循环 O(t)，内层对每个 t 的 k 大约 O(log t)，Case 3 的 j 循环也是 O(log t)。总体 O(t * log t)。

**Q4: 能否用记忆化搜索代替 bottom-up DP？**
> 可以。用 `unordered_map<int, int>` 做记忆化。好处是只计算需要的子问题，如果 target 很大但有效子问题少的话更快。

## 相关题型

- **1654. 到家的最少跳跃次数** — 同样是一维状态空间的最短路径问题，可以用 BFS。区别是跳跃规则不同且有禁止位置。
- **752. 打开转盘锁** — BFS 求最短操作序列的经典模板题。状态空间不同但搜索框架相同。
- **279. 完全平方数** — DP 求最少操作次数到达目标值。子问题结构类似（dp[n] = min(dp[n - k^2]) + 1）。
- **322. 零钱兑换** — DP 求最少硬币数凑出目标金额，子问题结构和本题的 DP 类似。
