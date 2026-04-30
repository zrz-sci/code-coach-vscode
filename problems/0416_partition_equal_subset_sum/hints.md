# 416. 分割等和子集

## 核心思路

这道题本质上是一个 **0/1 背包问题**：能否从数组中选出若干个数，使得它们的和恰好等于总和的一半？

## 思维链

1. **读完题第一反应**：分成两个和相等的子集 → 总和必须是偶数，否则直接返回 false。每个子集的目标和 = `totalSum / 2`。
2. **转化问题**：不需要真的"分成两组"，只需要问"能否从数组中选出一些数，凑出 `target = sum/2`？" → 这不就是 **子集和问题 / 0/1 背包**吗？
3. **暴力解是什么？** 对每个元素，选或不选，枚举所有 2^n 种子集，检查是否有和为 target 的。时间 O(2^n)，n 最大 200，完全不可行。
4. **暴力解的瓶颈？** 大量重叠子问题！"用前 i 个数能否凑出和 j" 这个子问题会被反复计算。
5. **DP 优化**：定义 `dp[i][j]` = 用前 i 个数能否恰好凑出和 j。对于第 i 个数，要么不选（`dp[i-1][j]`），要么选（`dp[i-1][j-nums[i]]`）→ 经典 0/1 背包转移。
6. **空间优化**：因为 `dp[i]` 只依赖 `dp[i-1]`，可以压缩成一维数组，但必须**倒序遍历**（这是 0/1 背包的关键，防止同一物品被选多次）。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力递归 | 每个元素选/不选，枚举 2^n | O(2^n) | O(n) | 能说出即可 |
| 记忆化搜索 | 暴力递归 + memo | O(n × target) | O(n × target) | 能说出即可 |
| 二维 DP | 0/1 背包递推 | O(n × target) | O(n × target) | 加分项 |
| 一维 DP（空间优化） | 滚动数组 + 倒序遍历 | O(n × target) | O(target) | ⭐ 必须写出 |

## 关键提示

1. **总和为奇数直接返回 false** — 这是最基本的剪枝，别忘了。
2. **问题转化是关键**：不要想"怎么分两组"，要想"能否选一些数凑出 sum/2"。
3. **这是 0/1 背包**，不是完全背包 — 每个数只能用一次，所以一维 DP 必须倒序遍历 j。
4. **提前终止**：如果数组中某个数 > target，直接返回 false。
5. **DP 含义**：`dp[j] = true` 表示"从已遍历的数中，可以选出若干个凑出和 j"。

## 解法详解

### 解法1: 暴力递归（DFS） — O(2^n) / O(n)

**思考过程**：最朴素的想法 — 对于每个数，我可以"选"或者"不选"它放进子集里。递归地尝试所有组合，看是否存在和为 target 的子集。

```cpp
class Solution {
public:
    bool canPartition(vector<int>& nums) {
        int sum = 0;
        for (int x : nums) sum += x;
        if (sum % 2 != 0) return false;  // 奇数不可能
        return dfs(nums, 0, sum / 2);
    }

private:
    // 从 index 开始，能否从剩余元素中选出和为 remain 的子集
    bool dfs(vector<int>& nums, int index, int remain) {
        if (remain == 0) return true;   // 凑齐了
        if (index >= nums.size() || remain < 0) return false;  // 越界或超了

        // 选 nums[index] 或不选
        return dfs(nums, index + 1, remain - nums[index])  // 选
            || dfs(nums, index + 1, remain);                // 不选
    }
};
```

**关键点**：这个解法思路清晰但时间复杂度 O(2^n)。n=200 时完全不可行，但它帮助我们理解"选或不选"的决策结构 — 这正是 0/1 背包的本质。

```
// 决策树（以 nums = [1,5,11,5], target = 11 为例）:
//                    remain=11
//                   /         \
//          选1: remain=10    不选1: remain=11
//              /     \            /      \
//     选5: r=5   不选5: r=10  选5: r=6  不选5: r=11
//        /  \       /   \       ...       ...
//   选11:  不选11
//   r=-6✗  r=5
//          / \
//     选5: 不选5:
//     r=0✓  r=5✗    ← 找到 [1,5,5]，和=11
```

---

### 解法2: 记忆化搜索 — O(n × target) / O(n × target)

**从解法1优化**：暴力递归中存在大量重叠子问题。例如 `dfs(3, 5)` 可能从不同路径被多次调用。加一个 memo 数组缓存结果即可。

```cpp
class Solution {
public:
    bool canPartition(vector<int>& nums) {
        int sum = 0;
        for (int x : nums) sum += x;
        if (sum % 2 != 0) return false;

        int target = sum / 2;
        // memo[i][j]: 用 nums[i..n-1] 能否凑出 j, -1=未算, 0=false, 1=true
        vector<vector<int>> memo(nums.size(), vector<int>(target + 1, -1));
        return dfs(nums, 0, target, memo);
    }

private:
    bool dfs(vector<int>& nums, int index, int remain, vector<vector<int>>& memo) {
        if (remain == 0) return true;
        if (index >= nums.size() || remain < 0) return false;
        if (memo[index][remain] != -1) return memo[index][remain];

        bool result = dfs(nums, index + 1, remain - nums[index], memo)
                   || dfs(nums, index + 1, remain, memo);

        memo[index][remain] = result;
        return result;
    }
};
```

**关键点**：状态空间从 2^n 降到 n × target。这是从暴力到 DP 的桥梁。

---

### 解法3: 二维 DP（0/1 背包） — O(n × target) / O(n × target)

**从记忆化翻译成递推**：把自顶向下的递归翻译成自底向上的填表。

`dp[i][j]` = 使用前 i 个数（`nums[0..i-1]`），能否恰好凑出和 j。

状态转移：
- 不选第 i 个数：`dp[i][j] = dp[i-1][j]`
- 选第 i 个数（前提 `j >= nums[i-1]`）：`dp[i][j] = dp[i-1][j - nums[i-1]]`
- 两者取或：`dp[i][j] = dp[i-1][j] || dp[i-1][j - nums[i-1]]`

```cpp
class Solution {
public:
    bool canPartition(vector<int>& nums) {
        int sum = 0;
        for (int x : nums) sum += x;
        if (sum % 2 != 0) return false;

        int n = nums.size(), target = sum / 2;
        // dp[i][j]: 前 i 个数能否凑出 j
        vector<vector<bool>> dp(n + 1, vector<bool>(target + 1, false));
        // base case: 凑出和 0 总是可以的（什么都不选）
        for (int i = 0; i <= n; i++) dp[i][0] = true;

        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= target; j++) {
                dp[i][j] = dp[i - 1][j];  // 不选 nums[i-1]
                if (j >= nums[i - 1]) {
                    dp[i][j] = dp[i][j] || dp[i - 1][j - nums[i - 1]];  // 选
                }
            }
        }
        return dp[n][target];
    }
};
```

```
// dp 填表过程 (nums = [1,5,11,5], target = 11):
//
//        j:  0  1  2  3  4  5  6  7  8  9  10  11
// i=0 (无):  T  F  F  F  F  F  F  F  F  F   F   F
// i=1 (1) :  T  T  F  F  F  F  F  F  F  F   F   F
// i=2 (5) :  T  T  F  F  F  T  T  F  F  F   F   F
// i=3 (11):  T  T  F  F  F  T  T  F  F  F   F   T  ← dp[3][11]=T (选11)
// i=4 (5) :  T  T  F  F  F  T  T  F  F  F   T   T
//
// dp[4][11] = true → 答案 true
// 
// 依赖关系: dp[i][j] 依赖上一行的 dp[i-1][j] 和 dp[i-1][j-nums[i-1]]
//           ↑ 正上方               ↑ 左上方
```

**关键点**：`dp[i][0] = true` 的含义 — 空集的和为 0，这是合法的。

---

### 解法4: 一维 DP（空间优化）— O(n × target) / O(target) ⭐ 面试首选

**从解法3优化**：观察到 `dp[i][j]` 只依赖 `dp[i-1][...]`（上一行），所以可以压缩成一维数组。**但必须倒序遍历 j**，否则同一个数会被"选多次"（变成完全背包了）。

```cpp
class Solution {
public:
    bool canPartition(vector<int>& nums) {
        int sum = 0;
        for (int x : nums) sum += x;
        if (sum % 2 != 0) return false;

        int target = sum / 2;
        vector<bool> dp(target + 1, false);
        dp[0] = true;  // 凑出和 0 永远可以

        for (int num : nums) {
            // 倒序遍历！保证每个 num 只被选一次（0/1 背包核心）
            for (int j = target; j >= num; j--) {
                dp[j] = dp[j] || dp[j - num];
                // dp[j]      = 不选 num（保留旧值，相当于 dp[i-1][j]）
                // dp[j - num] = 选 num（此时 j-num < j，还未被本轮更新，
                //               仍然是"上一行"的值，即 dp[i-1][j-num]）
            }
            // 提前终止：如果已经能凑出 target，无需继续
            if (dp[target]) return true;
        }
        return dp[target];
    }
};
```

**为什么必须倒序？**
```
// 假设 nums = [3, ...], dp = [T, F, F, T, F, F, ...]
//
// 【正序（错误 — 变成完全背包）】:
// j=3: dp[3] = dp[3] || dp[0] = T   ← 选了一次 3，OK
// j=6: dp[6] = dp[6] || dp[3] = T   ← dp[3] 已被本轮更新！
//      等价于选了两次 3，错误！
//
// 【倒序（正确 — 0/1 背包）】:
// j=6: dp[6] = dp[6] || dp[3] = T   ← dp[3] 还是旧值，OK
// j=3: dp[3] = dp[3] || dp[0] = T   ← 正常更新
//      每个数最多选一次 ✓
```

---

## 解法对比

| | 暴力递归 | 记忆化搜索 | 二维 DP | 一维 DP |
|---|---|---|---|---|
| 时间 | O(2^n) | O(n × target) | O(n × target) | O(n × target) |
| 空间 | O(n) | O(n × target) | O(n × target) | O(target) |
| 思路 | 最直觉 | 加缓存 | 翻译成表 | 空间优化 |
| 面试推荐 | 起点 | 过渡 | 讲解用 | ⭐ 首选 |

关键区别：一维 DP 和二维 DP 的逻辑完全相同，只是利用"当前行只依赖上一行"这个性质压缩空间。面试中直接写一维 DP 即可，但要能解释清楚倒序遍历的原因。

## 易错点

1. **忘记判断奇数**
   - ✗ 直接计算 `target = sum / 2` 开始 DP
   - ✓ 先 `if (sum % 2 != 0) return false;`
   - 总和为奇数时不可能分成两个相等的整数和

2. **一维 DP 正序遍历（变成完全背包）**
   - ✗ `for (int j = num; j <= target; j++)` → 同一个数被选多次
   - ✓ `for (int j = target; j >= num; j--)` → 每个数最多选一次
   - 这是 0/1 背包和完全背包的**唯一代码区别**

3. **dp 初始化错误**
   - ✗ `dp[0] = false` 或所有位置初始化为 true
   - ✓ 只有 `dp[0] = true`，其余为 false
   - `dp[0] = true` 意味着"和为 0 可以通过不选任何数达到"

4. **没有提前终止**
   - 不算错，但效率可以更高
   - ✓ 在每轮循环后检查 `if (dp[target]) return true;`
   - ✓ 如果 `nums` 中某个元素 > target，直接 return false

## 面试追问

**Q1: 这道题和经典背包问题的关系是什么？**
→ 这是 0/1 背包的判定版本：n 个物品每个重量 nums[i]，背包容量 target，问能否恰好装满。价值不重要，只关心"能否装满"。

**Q2: 如果改成"分成两组使差最小"呢？**
→ 仍然用 0/1 背包。dp 求出所有能凑出的和 S1（S1 ≤ sum/2），取最大的 S1，答案是 `sum - 2 * S1`。对应 [LeetCode 1049. 最后一块石头的重量 II]。

**Q3: 如果元素可以重复使用呢？**
→ 变成完全背包，内层循环改为正序即可。对应 [LeetCode 518. 零钱兑换 II] 的思路。

**Q4: 如果要输出具体的分割方案呢？**
→ 在 DP 过程中额外记录每个状态是"选了哪个数"转移来的，最后回溯路径。用 `parent[j] = num` 或者二维 DP 中记录选择。

## 相关题型

- **[494. 目标和 (Target Sum)](https://leetcode.com/problems/target-sum/)** — 复用同样的 0/1 背包框架，区别：target 变成 `(sum + S) / 2`，需要数学推导将 +/- 转化为选/不选。
- **[1049. 最后一块石头的重量 II](https://leetcode.com/problems/last-stone-weight-ii/)** — 复用本题框架，区别：不是判断"能否恰好凑出 target"，而是"能凑出的最大值是多少"，即 dp 中最大的 true 位置。
- **[474. 一和零](https://leetcode.com/problems/ones-and-zeroes/)** — 二维 0/1 背包：两个维度的容量限制（0 的个数和 1 的个数），转移逻辑相同但多一维。
- **[322. 零钱兑换](https://leetcode.com/problems/coin-change/)** — 完全背包（每个硬币可用无限次），区别：内层循环正序。和本题对比可以深刻理解 0/1 vs 完全背包。