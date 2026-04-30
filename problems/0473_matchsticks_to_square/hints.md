# 473. 火柴拼正方形 (Matchsticks to Square)

## 核心思路

这道题本质上是 **k 等分子集问题（k=4）**：将所有火柴分成 4 组，使每组的总长度恰好等于 `sum/4`。这是一个典型的**回溯搜索 + 剪枝优化**问题。数据范围 `n <= 15` 暗示了指数级算法是可行的，但必须有高效的剪枝才能通过。

## 思维链

1. **读完题第一反应**：把火柴分成 4 组，每组长度相等。这不就是"分割等和子集"的升级版吗？从 2 组变成了 4 组。

2. **先做预检查**：
   - 总长度不能被 4 整除 → 直接 false
   - 最长的火柴超过 `sum/4` → 无法放入任何一条边 → 直接 false
   - 火柴数量少于 4 → 不可能组成正方形 → 直接 false

3. **回溯框架**：维护 4 条边的当前长度 `sides[4]`，对每根火柴做选择——放入哪条边？如果所有火柴都放完了且 4 条边长度相等，返回 true。

4. **为什么需要剪枝？** 最坏情况 4^15 ≈ 10^9 次操作，不剪枝必定超时。关键剪枝：
   - **降序排序**：大的火柴先放，选择空间小，更早触发失败回溯
   - **跳过相同长度的边**：如果 `sides[j] == sides[j-1]`，放入 j 和 j-1 效果一样，跳过避免重复
   - **超长剪枝**：`sides[j] + matchsticks[i] > target` 直接跳过

5. **还有别的方法吗？** 可以用**状态压缩 DP**——用 bitmask 表示哪些火柴已被使用，枚举所有子集。时间 `O(n * 2^n)`，空间 `O(2^n)`。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 回溯 + 剪枝 | 每根火柴选放哪条边，DFS 搜索 | O(4^n) 最坏，剪枝后远好于此 | O(n) | ⭐ 必须写出 |
| 状态压缩 DP | bitmask 枚举火柴子集 | O(n * 2^n) | O(2^n) | 进阶方案 |

## 关键提示

1. **降序排序是最关键的剪枝**：大的火柴选择空间小（能放的边更少），先处理大的能大幅减少搜索树的分支数。没有降序排序，很多用例会 TLE。

2. **跳过相同长度的边**：假设 `sides = [3, 3, 0, 0]`，把火柴放入 sides[0] 和 sides[1] 效果完全一样。通过 `if (j > 0 && sides[j] == sides[j-1]) continue` 跳过，可以避免大量重复搜索。

3. **决策树长这样（以 matchsticks = [2, 2, 1, 1, 2]，target = 2 为例）**：

```
// 排序后：[2, 2, 2, 1, 1]，target = sum/4 = 8/4 = 2
//
//                                 sides = [0,0,0,0]
//                                    放 ms[0]=2
//                    ┌──────────────────┼──────────────────┐
//                 边0:[2,0,0,0]     边1:[0,2,0,0]     (边2,3同边0,1,跳过)
//                    放 ms[1]=2          放 ms[1]=2
//                ┌───────┐            ┌───────┐
//          [2,2,0,0]  (边2同边1,跳过)  (只能放边0或边2)
//             放 ms[2]=2              ...
//          ┌───────┐
//     [2,2,2,0] (边3同边2,跳过)
//        放 ms[3]=1
//     ┌───────┐
//  边3:[2,2,2,1]
//     放 ms[4]=1
//  边3:[2,2,2,2] ✓ 全部等于 target=2，返回 true!
```

4. **"放火柴到边"vs"放火柴到组"**：思考模型是"对每根火柴做选择"（放入 4 条边之一），而不是"对每条边选火柴"。前者的回溯框架更自然。

5. **状态压缩 DP 的思路**：`dp[mask]` 表示使用 mask 对应的火柴子集后，当前正在填充的那条边已累积的长度。如果累积长度达到 target，自动切换到下一条边。

## 解法详解

### 解法1: 回溯 + 剪枝 — O(4^n) 最坏 / O(n) ⭐ 面试首选

**思考过程**：对每根火柴，尝试放入 4 条边中的某一条。如果放入后该边长度不超过 target，就递归处理下一根火柴。所有火柴放完后检查 4 条边是否相等。

**为什么降序排序？** 假设有一根长度为 7 的火柴，target=8。如果先处理它，后面只能再放总长度为 1 的火柴在同一条边。选择非常受限，搜索树很窄，失败分支很早被剪掉。如果最后才处理它，前面的小火柴已经形成了大量分支，白白浪费时间。

```cpp
class Solution {
public:
    bool makesquare(vector<int>& matchsticks) {
        int n = matchsticks.size();
        if (n < 4) return false;
        
        long long total = accumulate(matchsticks.begin(), matchsticks.end(), 0LL);
        if (total % 4 != 0) return false;
        
        int target = total / 4;
        
        // 【剪枝1】降序排序：大的先放，更早发现不可行
        sort(matchsticks.rbegin(), matchsticks.rend());
        
        // 【剪枝2】最大的火柴超过 target，不可能拼成
        if (matchsticks[0] > target) return false;
        
        vector<int> sides(4, 0);
        return backtrack(matchsticks, sides, 0, target);
    }
    
private:
    bool backtrack(vector<int>& ms, vector<int>& sides, int idx, int target) {
        // 终止条件：所有火柴都放完了
        if (idx == (int)ms.size()) {
            // 因为 total % 4 == 0 且每条边都 <= target，
            // 只要前 3 条边等于 target，第 4 条必然也等于
            return sides[0] == target && sides[1] == target && sides[2] == target;
        }
        
        for (int j = 0; j < 4; j++) {
            // 【剪枝3】放入后超过 target，跳过
            if (sides[j] + ms[idx] > target) continue;
            
            // 【剪枝4】跳过相同长度的边，避免重复搜索
            // 如果 sides[j] == sides[j-1]，放入 j 和 j-1 的效果完全一样
            if (j > 0 && sides[j] == sides[j - 1]) continue;
            
            // 做选择：将 ms[idx] 放入第 j 条边
            sides[j] += ms[idx];
            
            // 递归：处理下一根火柴
            if (backtrack(ms, sides, idx + 1, target)) return true;
            
            // 撤销选择
            sides[j] -= ms[idx];
        }
        
        return false;  // 4 条边都试过了，都不行
    }
};
```

**复杂度分析**：
- 时间：最坏 O(4^n)，但剪枝后实际远好于此。降序排序 + 跳过相同边 + 超长剪枝使得搜索树大幅缩减。
- 空间：O(n) 递归栈深度 + O(4) sides 数组。

---

### 解法2: 状态压缩 DP — O(n * 2^n) / O(2^n)

**从解法1的另一种思路**：不再"对每根火柴选边"，而是"对每个可能的火柴组合判断是否合法"。

**核心思想**：用一个 n 位的 bitmask 表示哪些火柴已经被使用。`dp[mask]` 存储的是：使用 mask 对应的火柴后，**当前正在填充的那条边已经累积的长度**（对 target 取模后的余数）。

**状态转移**：
- 对于每个 mask，尝试加入一根新火柴 i（第 i 位为 0）
- 新状态 `new_mask = mask | (1 << i)`
- 新的累积长度 = `dp[mask] + matchsticks[i]`
- 如果累积长度超过 target，跳过（非法状态）
- 如果累积长度恰好等于 target，新状态的累积重置为 0（开始填下一条边）

```cpp
class Solution {
public:
    bool makesquare(vector<int>& matchsticks) {
        int n = matchsticks.size();
        if (n < 4) return false;
        
        int total = accumulate(matchsticks.begin(), matchsticks.end(), 0);
        if (total % 4 != 0) return false;
        
        int target = total / 4;
        
        // dp[mask] = 使用 mask 中的火柴后，当前边已累积的长度
        // -1 表示该状态不可达
        vector<int> dp(1 << n, -1);
        dp[0] = 0;  // 初始：没有火柴被使用，累积长度为 0
        
        for (int mask = 0; mask < (1 << n); mask++) {
            if (dp[mask] == -1) continue;  // 不可达状态，跳过
            
            for (int i = 0; i < n; i++) {
                // 火柴 i 还没被使用
                if (mask & (1 << i)) continue;
                
                // 当前边累积长度 + 新火柴长度
                int newLen = dp[mask] + matchsticks[i];
                
                // 超过 target，非法
                if (newLen > target) continue;
                
                int newMask = mask | (1 << i);
                
                // 如果恰好等于 target，重置为 0（开始下一条边）
                dp[newMask] = (newLen == target) ? 0 : newLen;
            }
        }
        
        // 所有火柴都被使用（全 1 mask），且累积长度为 0（最后一条边也恰好填满）
        return dp[(1 << n) - 1] == 0;
    }
};
```

**复杂度分析**：
- 时间：O(n * 2^n)，枚举每个 mask（2^n 个）× 每个火柴（n 个）。
- 空间：O(2^n)，dp 数组。
- n=15 时：15 * 32768 ≈ 50万，轻松通过。

**与回溯法的对比**：
- DP 保证每个状态只被计算一次（无重复搜索），最坏情况下更稳定。
- 回溯法有剪枝，最好情况下更快（大量分支被剪掉），但最坏情况可能慢于 DP。
- 面试中先写回溯（思路清晰），追问时提 DP（展示状态压缩技巧）。

## 解法对比

|  | 解法1: 回溯+剪枝 | 解法2: 状态压缩DP |
|---|---|---|
| 思路 | 每根火柴选放哪条边 | bitmask 枚举所有火柴组合 |
| 时间最坏 | O(4^n) | O(n * 2^n) |
| 时间实际 | 剪枝后远好于最坏 | 稳定 |
| 空间 | O(n) | O(2^n) |
| 代码难度 | 中等 | 较难 |
| 面试推荐 | ⭐ 首选 | 追问时展示 |
| 可拓展性 | 改 k=4 即可变成 k 等分 | 同上 |

**什么时候选哪个？**
- 面试第一轮：写解法1，展示回溯 + 剪枝技巧。
- 面试追问"能否避免重复计算"：给解法2，展示状态压缩 DP。
- 如果面试官问"k 等分"的通用解：两种方法都可以泛化，回溯法将 `sides[4]` 改为 `sides[k]` 即可。

## 易错点

1. **忘记降序排序导致 TLE**
   ```cpp
   // ✗ 没有排序，小的火柴先处理，搜索树太庞大
   // sort(matchsticks.rbegin(), matchsticks.rend()); // 漏掉这行
   
   // ✓ 必须降序排序
   sort(matchsticks.rbegin(), matchsticks.rend());
   ```

2. **忘记"跳过相同长度的边"剪枝**
   ```cpp
   // ✗ 没有这个剪枝，sides=[3,3,0,0] 时会重复搜索
   // 放入 sides[0] 和 sides[1] 的效果一样，但会搜两次
   
   // ✓ 加上这一行
   if (j > 0 && sides[j] == sides[j - 1]) continue;
   ```

3. **整数溢出：总长度计算**
   ```cpp
   // ✗ 如果 matchsticks[i] 最大 10^8，15 个加起来可能超过 int
   int total = accumulate(matchsticks.begin(), matchsticks.end(), 0);
   // 实际上 15 * 10^8 = 1.5 * 10^9 < 2.1 * 10^9 (INT_MAX)，刚好不溢出
   // 但用 long long 更安全：
   long long total = accumulate(matchsticks.begin(), matchsticks.end(), 0LL);
   ```

4. **状态压缩 DP 中 newLen == target 时忘记重置为 0**
   ```cpp
   // ✗ 直接赋值，不重置
   dp[newMask] = newLen;  // 错！newLen==target时应该重置
   
   // ✓ 恰好填满一条边时，重置为 0 开始下一条边
   dp[newMask] = (newLen == target) ? 0 : newLen;
   ```

5. **终止条件只检查了部分边**
   ```cpp
   // ✗ 只检查了 sides[0]
   if (idx == ms.size()) return sides[0] == target;
   
   // ✓ 至少检查前 3 条边（第 4 条由总和保证）
   if (idx == ms.size()) return sides[0] == target && sides[1] == target && sides[2] == target;
   ```

## 面试追问

**Q1（基础理解）**：这道题和 LeetCode 416 分割等和子集有什么区别？
> 416 是将数组分成 **2** 组使和相等，可以用背包 DP 解决（O(n*sum)）。本题是分成 **4** 组，背包 DP 不直接适用（需要多维背包），所以用回溯或状态压缩 DP。从 2 等分到 k 等分，问题难度从 DP 升级到了 NP-完全。

**Q2（剪枝分析）**：为什么降序排序能大幅提速？
> 大火柴的约束更强（能放的边更少），先处理它们等于在搜索树的高层就大幅减少了分支数。比如一根长度为 7 的火柴（target=8），只能放在累积长度 <=1 的边上，选择极少。如果放在搜索树底层，前面小火柴已经展开了海量分支，浪费大量时间。

**Q3（泛化）**：如果把 4 条边改成 k 条边（k 等分），怎么改？
> 回溯法：`sides` 数组改为大小 k，for 循环遍历 k 条边。状态压缩 DP 同理。时间复杂度从 O(4^n) 变为 O(k^n)，DP 仍为 O(n * 2^n)。当 k 较大时 DP 更有优势。

**Q4（复杂度分析）**：状态压缩 DP 的 O(n * 2^n) 怎么理解？
> 共有 2^n 个状态（每个 bitmask），对每个状态尝试加入 n 根火柴之一，所以总操作数是 n * 2^n。n=15 时约 50 万，非常快。而回溯法最坏 O(4^n) = O(4^15) ≈ 10^9，虽然剪枝后远好于此，但理论最坏更差。

**Q5（实战优化）**：还有哪些剪枝可以加？
> (1) 如果某根火柴放入一条空边后回溯失败了，那么放入其他空边也一样会失败——因为空边是等价的。(2) 如果当前火柴放入某条边恰好填满（`sides[j] + ms[idx] == target`），但后续递归失败了，那么无需尝试其他边——这条边最终都需要被填满，现在不填以后也要填。

## 相关题型

- **LeetCode 416 分割等和子集** — 2 等分版本，用 0/1 背包 DP 解决。是本题的简化版。
- **LeetCode 698 划分为 k 个相等的子集** — 本题的通用版（k 等分），回溯框架几乎相同。
- **LeetCode 526 优美的排列** — 回溯 + 状态压缩 DP 的经典题。
- **LeetCode 1986 完成任务的最少工作时间段** — 类似的"分组"问题，用状态压缩 DP。
- **LeetCode 78 子集** — 理解子集枚举和 bitmask 的基础。
