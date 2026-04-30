# 698. 划分为K个相等的子集

## 核心思路

本题是经典的 **回溯 + 剪枝** 问题，也可以用 **状态压缩 DP** 解决。目标是将数组分成 k 个非空子集，每个子集的和相等。关键约束 `n <= 16` 暗示了指数级算法是可接受的。核心思想是：把问题看作"把 n 个球分进 k 个桶，每个桶装满 target = sum/k"。

## 思维链

1. **读完题第一反应**：把 n 个元素分成 k 组，每组和相同。如果 `sum % k != 0`，直接不可能。否则每组目标和 `target = sum / k`。

2. **方向选择 — 两种回溯视角**：
   - **视角A (元素视角)**：对每个元素，决定它放进哪个桶。每个元素有 k 种选择，总搜索空间 O(k^n)。
   - **视角B (桶视角)**：对每个桶，从剩余元素中选择一个子集使其和为 target。填满一个桶后继续填下一个。
   
   两种视角都能解题，但视角A配合剪枝效果更好。

3. **关键剪枝1 — 降序排序**：先放大元素。大元素选择少、约束强，能更早发现不可行路径。如果最大元素 > target，直接返回 false。

4. **关键剪枝2 — 空桶等价**：如果当前元素放入某个空桶后回溯失败了，那么放入另一个空桶结果一定相同（空桶之间没有区别）。直接 break 跳出。这个剪枝能减少海量重复搜索。

5. **关键剪枝3 — 桶溢出**：`buckets[i] + nums[idx] > target` 直接 skip。

6. **另一条路 — 状态压缩 DP**：n <= 16，可以用 16 位 bitmask 表示哪些元素已被选择。`dp[mask]` 表示选了 mask 对应元素后，当前正在填充的桶剩余容量。这是 O(n * 2^n) 的精确算法。

```
决策树示意 (nums=[4,3,2,3,5,2,1], k=4, target=5):

降序排序后: [5,4,3,3,2,2,1]

                        放 5
              ┌─── 桶0=[5] ───┐
              │                │
            放 4             (桶1=[5] 等价于桶0，空桶剪枝 break)
       ┌── 桶0? 5+4>5 跳过
       ├── 桶1=[4]
       │     │
       │   放 3
       │   ├── 桶1? 4+3>5 跳过
       │   ├── 桶2=[3]
       │   │     │
       │   │   放 3
       │   │   ├── 桶2? 3+3>5 跳过
       │   │   ├── 桶3=[3]
       │   │   │     │
       │   │   │   放 2
       │   │   │   ├── 桶1? 4+2>5 跳过
       │   │   │   ├── 桶2? 3+2=5 ✓ → 桶2满
       │   │   │   │     │
       │   │   │   │   放 2
       │   │   │   │   ├── 桶1? 4+2>5 跳过
       │   │   │   │   ├── 桶3? 3+2=5 ✓ → 桶3满
       │   │   │   │   │     │
       │   │   │   │   │   放 1
       │   │   │   │   │   ├── 桶1? 4+1=5 ✓ → 桶1满
       │   │   │   │   │   │
       │   │   │   │   │   所有元素已放完 → return true!
       │   │   │   │   │
       │   │   │   │   ...
       
最终结果: 桶0={5}, 桶1={4,1}, 桶2={3,2}, 桶3={3,2} → true
```

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 回溯 + 剪枝（元素视角） | k个桶，逐个放元素 | O(k^n) 最坏，剪枝后远小于此 | O(n + k) | ⭐ 必须写出 |
| 状态压缩 DP | bitmask 记录已选元素 | O(n * 2^n) | O(2^n) | 进阶加分 |

## 关键提示

1. **预检查**：`sum % k != 0` 直接返回 false。`max(nums) > target` 也直接返回 false。这两个提前剪枝能排除大量无效输入。

2. **降序排序的重要性**：排序后大元素先放，可以更早地触发"桶溢出"剪枝。实测中这一步可以将运行时间从 TLE 变成 ms 级。

3. **空桶等价剪枝是灵魂**：如果 `buckets[i] == 0` 且放入当前元素后回溯失败，那么 `buckets[i+1]`（也是空桶）必然也失败。因为空桶之间完全对称，选哪个空桶没有区别。`break` 而非 `continue`。

4. **状态压缩 DP 的思路**：用 `curSum` 追踪当前桶已累积的和。每当 `curSum` 达到 `target` 时，自动开始填下一个桶（`curSum` 对 `target` 取模归零）。`dp[mask]` 就是到达 mask 状态时当前桶的累积和。

5. **两种回溯视角的比较**：
   - 元素视角: 外层遍历元素，内层尝试 k 个桶。搜索空间 O(k^n)。空桶剪枝效果极好。
   - 桶视角: 外层遍历桶，内层从剩余元素中选子集。需要 visited 数组。剪枝相对复杂。
   面试推荐元素视角，代码简洁且剪枝直观。

## 解法详解

### 解法1: 回溯 + 剪枝（元素视角）— O(k^n) / O(n+k) ⭐

**思考过程**：维护 k 个桶的当前和。对每个元素（降序排列后），尝试放入每个桶。三种剪枝保证效率。

```cpp
class Solution {
public:
    bool canPartitionKSubsets(vector<int>& nums, int k) {
        int sum = 0;
        for (int x : nums) sum += x;
        if (sum % k != 0) return false;
        int target = sum / k;
        
        // 降序排序: 大元素先放，更早触发剪枝
        sort(nums.rbegin(), nums.rend());
        if (nums[0] > target) return false;
        
        vector<int> buckets(k, 0);
        return backtrack(nums, buckets, 0, target);
    }
    
private:
    bool backtrack(vector<int>& nums, vector<int>& buckets, int idx, int target) {
        // 所有元素都已分配
        if (idx == (int)nums.size()) return true;
        
        for (int i = 0; i < (int)buckets.size(); i++) {
            // 剪枝1: 桶溢出
            if (buckets[i] + nums[idx] > target) continue;
            
            // 做选择: 把 nums[idx] 放入桶 i
            buckets[i] += nums[idx];
            if (backtrack(nums, buckets, idx + 1, target)) return true;
            buckets[i] -= nums[idx]; // 撤销选择
            
            // 剪枝2: 空桶等价
            // 如果当前桶是空的，放入后仍然失败，后续空桶也必然失败
            if (buckets[i] == 0) break;
        }
        return false;
    }
};
```

**为什么空桶剪枝有效？**

假设 buckets = [3, 0, 0, 0]，要放元素 2：
- 尝试放入桶1（空桶），递归失败，buckets[1] 恢复为 0
- 如果接着尝试桶2（也是空桶），起始状态完全一样，必然也失败
- 所以直接 break，跳过所有后续空桶

这个剪枝在 k 较大时效果极其显著。没有它，k=16 时很可能 TLE。

### 解法2: 状态压缩 DP — O(n * 2^n) / O(2^n)

**思考过程**：n <= 16，可以用 16 位整数表示哪些元素已被选入某个桶。`dp[mask]` 记录选了 mask 对应元素后，当前正在填充的桶中已累积的和。如果 `dp[mask] == -1` 表示 mask 状态不可达。

```cpp
class Solution {
public:
    bool canPartitionKSubsets(vector<int>& nums, int k) {
        int n = nums.size();
        int sum = 0;
        for (int x : nums) sum += x;
        if (sum % k != 0) return false;
        int target = sum / k;
        
        sort(nums.begin(), nums.end());
        if (nums.back() > target) return false;
        
        int totalMasks = 1 << n;
        // dp[mask] = 当前桶中已累积的和 (对 target 取模后的值)
        // -1 表示不可达
        vector<int> dp(totalMasks, -1);
        dp[0] = 0;
        
        for (int mask = 0; mask < totalMasks; mask++) {
            if (dp[mask] == -1) continue;
            
            for (int i = 0; i < n; i++) {
                // 第 i 个元素还没被选
                if (mask & (1 << i)) continue;
                
                // 当前桶剩余容量能放下 nums[i]
                if (dp[mask] + nums[i] <= target) {
                    int newMask = mask | (1 << i);
                    // 对 target 取模: 桶满了自动归零，开始填下一个桶
                    dp[newMask] = (dp[mask] + nums[i]) % target;
                }
            }
        }
        return dp[totalMasks - 1] == 0;
    }
};
```

**状态转移解释**：
- `dp[mask]` 表示选了 mask 中的元素后，"当前桶"中已有的和
- 当 `dp[mask] + nums[i] == target` 时，当前桶恰好装满，取模后为 0，自动开始装下一个桶
- 最终 `dp[(1<<n)-1] == 0` 表示所有元素都被选中，且最后一个桶也恰好装满

## 解法对比

| 维度 | 回溯+剪枝 | 状态压缩 DP |
|------|----------|------------|
| 时间复杂度 | O(k^n)，实际远快 | O(n * 2^n) |
| 空间复杂度 | O(n + k) | O(2^n) |
| 代码难度 | 中等 | 中等偏高 |
| 最坏情况 | 可能慢 | 稳定 |
| 面试推荐 | 优先写这个 | 追问时说出 |
| 适用 n 范围 | n <= 16 配合剪枝 | n <= 20 左右 |

## 易错点

1. **忘记降序排序**：不排序直接回溯，大量小元素先填桶，大元素最后放不进去才发现不行，回溯极深。降序排序是必须的优化，否则会 TLE。

2. **空桶剪枝写成 continue 而非 break**：
   ```cpp
   // 错误: 继续尝试下一个空桶 → 重复搜索
   if (buckets[i] == 0) continue;
   
   // 正确: 跳出循环 → 所有空桶等价
   if (buckets[i] == 0) break;
   ```

3. **状压 DP 中忘记取模**：`dp[newMask] = dp[mask] + nums[i]` 不取模的话，没有"桶装满后开始下一个桶"的效果，dp 值会无限增长。

4. **nums 包含 0 的处理**：题目约束 `nums[i] >= 1`，但如果有 0，要注意 target 取模时除以 0 的问题。

5. **回溯中 idx 的推进**：必须是 `idx + 1` 而非 `i + 1`。idx 是当前要放置的元素编号，i 是桶编号，两者不能混淆。

6. **状压 DP 的初始状态**：`dp[0] = 0`（没选任何元素，当前桶为空），其余全部 -1。如果初始化错误，整个 DP 就会失效。

## 面试追问

1. **"能否用贪心解？"**
   → 不能。贪心（如每次找最接近 target 的子集）无法保证全局最优。反例很容易构造：`nums = [1,1,1,1,1,5,5,5,5,5]`, `k = 5`, `target = 6`，贪心可能把 5+1 凑一组后剩下的无法分。

2. **"回溯的最坏时间 O(k^n)，能优化吗？"**
   → 理论上界难以改进，但实际中三个剪枝（排序、溢出、空桶）将搜索量减少了几个数量级。状压 DP 的 O(n * 2^n) 是多项式 × 指数，在 n <= 16 时约 10^6，是可接受的。

3. **"如果 n 达到 100，怎么办？"**
   → 回溯和状压都不行。需要考虑 NP-hard 的近似算法，如 LPT (Longest Processing Time) 贪心启发式，或者 branch and bound。这个问题本质上是多处理器调度问题的判定版本。

4. **"和 416. Partition Equal Subset Sum 有什么关系？"**
   → 416 是 k=2 的特殊情况，可以用 O(n * sum) 的 0-1 背包 DP 解决。k > 2 时背包方法不适用，必须回溯或状压。

5. **"回溯中两种视角（元素视角 vs 桶视角）哪个更好？"**
   → 元素视角：搜索空间 O(k^n)，空桶剪枝强，代码简洁。
   → 桶视角：搜索空间 O(2^n * k)，需要 visited 数组，但当 k 远小于 n 时可能更优。
   一般面试推荐元素视角。

## 相关题型

| 题号 | 题目 | 关联 |
|------|------|------|
| 416 | Partition Equal Subset Sum | k=2 的特殊情况，0-1 背包 |
| 473 | Matchsticks to Square | k=4 的特殊情况，本题的子问题 |
| 1681 | Minimum Incompatibility | 状压 DP 的进阶版 |
| 78 | Subsets | 回溯/位运算枚举子集的基础 |
| 526 | Beautiful Arrangement | 状态压缩 DP 入门 |
| 2305 | Fair Distribution of Cookies | 类似的回溯 + 剪枝问题 |
