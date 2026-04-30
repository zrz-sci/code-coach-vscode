# 494. 目标和

## 核心思路

本质上是一个 **0/1 背包问题的变体**：每个数字要么加（放入正数集合），要么减（放入负数集合），问有多少种分法使得最终结果等于 target。通过数学变换，可以把"加减号分配"问题转化为"从数组中选一些数使其和等于某个目标值"的经典子集和计数问题。

## 思维链

1. **读完题第一反应**：每个数字有两种选择（+ 或 -），共 n 个数字，所以一共 2^n 种组合。暴力枚举所有组合看哪些和为 target → 回溯/DFS。

2. **暴力解的瓶颈**：2^n 种枚举，n=20 时是 ~10^6 勉强能过，但如果 n 更大就不行了。更关键的是，这里有大量重复计算——不同的选择路径可能在同一个位置产生相同的剩余目标值。

3. **发现重叠子问题**：DFS(i, remainSum) 的调用中，(i, remainSum) 相同的情况会被反复计算。这就是动态规划的信号！加上记忆化就能避免重复。

4. **数学变换——问题简化**：设选 + 的数之和为 P，选 - 的数之和为 Q。则 P - Q = target 且 P + Q = sum。两式相加得 P = (sum + target) / 2。问题变成：**从 nums 中选若干数，使其和恰好为 (sum + target) / 2，有多少种选法？** 这就是标准的 0/1 背包计数问题！

5. **0/1 背包 DP**：dp[j] = 和为 j 的选法数。对每个 num，倒序遍历 j，dp[j] += dp[j - num]。时间 O(n * bagSize)，空间 O(bagSize)。

6. **边界判断**：(sum + target) 必须是非负偶数，否则无解。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 回溯(DFS) | 每个数选+或-，枚举所有组合 | O(2^n) | O(n) | 能说出即可 |
| 记忆化搜索 | DFS + memo，消除重复子问题 | O(n × sum) | O(n × sum) | 能说出即可 |
| 0/1背包DP(二维) | 数学变换 → 子集和计数 | O(n × bagSize) | O(n × bagSize) | 加分项 |
| 0/1背包DP(一维) | 滚动数组空间优化 | O(n × bagSize) | O(bagSize) | ⭐ 必须写出 |

## 关键提示

1. **数学变换是关键跳板**：不要直接在"加减号"上做 DP，先用 P + Q = sum, P - Q = target 推出 P = (sum + target) / 2，把问题转化为子集和。

2. **无解的判断**：如果 (sum + target) 是奇数或者 sum + target < 0，直接返回 0。这个边界条件漏掉会出错。

3. **0/1 背包的遍历方向**：一维 DP 必须**倒序**遍历容量 j，否则同一个物品会被重复选取（变成完全背包了）。

4. **dp[0] = 1 的含义**：和为 0 有 1 种方法——什么都不选。这是背包计数问题的标准初始化。

5. **nums 中有 0 的情况**：0 可以选 + 也可以选 -，不影响和，但会使方案数翻倍。这也是为什么 DP 能自然处理——dp[j] += dp[j - 0] = dp[j]，方案数确实翻倍。

## 解法详解

### 解法1: 回溯(DFS) — O(2^n) / O(n)

**思考过程**：最直觉的想法——每个数字有 + 和 - 两种选择，用 DFS 枚举所有可能。

```
// 决策树 (nums = [1,1,1,1,1], target = 3):
//                          sum=0
//                    +1 /        \ -1
//                  sum=1          sum=-1
//              +1 /    \ -1    +1 /    \ -1
//            sum=2    sum=0   sum=0    sum=-2
//           +1/ \-1  +1/ \-1
//          s=3  s=1  s=1  s=-1
//          ...  ...  ...  ...
// 树的深度 = n, 每层分两叉, 共 2^n 个叶子节点
```

```cpp
class Solution {
public:
    int findTargetSumWays(vector<int>& nums, int target) {
        return dfs(nums, 0, target);
    }
    
    int dfs(vector<int>& nums, int i, int remain) {
        // 终止条件：所有数字都已分配符号
        if (i == nums.size()) {
            return remain == 0 ? 1 : 0;
        }
        // 选 +：remain 减去 nums[i]（需要凑的值少了）
        // 选 -：remain 加上 nums[i]（需要凑的值多了）
        return dfs(nums, i + 1, remain - nums[i])
             + dfs(nums, i + 1, remain + nums[i]);
    }
};
```

**关键点**：n=20 时 2^20 ≈ 10^6，虽然能通过但效率不高，且无法扩展到更大的 n。

---

### 解法2: 记忆化搜索 — O(n × totalSum) / O(n × totalSum)

**从解法1优化**：解法1的瓶颈是重复计算。比如 dfs(3, 1) 可能从不同路径到达（+1+1-1 和 +1-1+1 到达同一个 (i=3, remain=1)）。用 memo 缓存即可。

```cpp
class Solution {
public:
    int findTargetSumWays(vector<int>& nums, int target) {
        // key: (index, remain) → value: 方案数
        // remain 可能为负数，用 map 比较方便
        unordered_map<int, unordered_map<int, int>> memo;
        return dfs(nums, 0, target, memo);
    }
    
    int dfs(vector<int>& nums, int i, int remain,
            unordered_map<int, unordered_map<int, int>>& memo) {
        if (i == nums.size()) {
            return remain == 0 ? 1 : 0;
        }
        if (memo.count(i) && memo[i].count(remain)) {
            return memo[i][remain];
        }
        int result = dfs(nums, i + 1, remain - nums[i], memo)
                   + dfs(nums, i + 1, remain + nums[i], memo);
        memo[i][remain] = result;
        return result;
    }
};
```

**关键点**：remain 的范围是 [-sum, sum]，所以状态空间是 O(n × 2*sum)。

---

### 解法3: 0/1 背包 DP（二维） — O(n × bagSize) / O(n × bagSize)

**数学变换——化简问题**：

设选 + 号的数集合之和为 P，选 - 号的数集合之和为 Q：
```
P - Q = target     ... (1)
P + Q = sum         ... (2)
(1)+(2): 2P = sum + target
      → P = (sum + target) / 2
```

问题转化为：**从 nums 中选若干数，使其和恰好为 P = (sum + target) / 2，求方案数。**

这就是经典的 **0/1 背包计数问题**！

```
// DP 填表过程 (nums = [1,1,1,1,1], target = 3):
// sum = 5, bagSize = (5+3)/2 = 4
// dp[i][j] = 用前 i 个数, 凑出和为 j 的方案数
//
//        j: 0  1  2  3  4
// 初始(i=0): 1  0  0  0  0    ← dp[0][0]=1 (什么都不选,和为0)
// i=1(num=1):1  1  0  0  0    ← dp[1][1] = dp[0][0] = 1
// i=2(num=1):1  2  1  0  0    ← dp[2][1] = dp[1][1]+dp[1][0] = 2
// i=3(num=1):1  3  3  1  0    ← dp[3][2] = dp[2][2]+dp[2][1] = 3
// i=4(num=1):1  4  6  4  1    ← dp[4][3] = dp[3][3]+dp[3][2] = 4
// i=5(num=1):1  5 10 10  5    ← dp[5][4] = dp[4][4]+dp[4][3] = 5 ✓
//
// 状态转移: dp[i][j] = dp[i-1][j]          (不选第i个数)
//                     + dp[i-1][j-nums[i]]  (选第i个数, 如果j>=nums[i])
```

```cpp
class Solution {
public:
    int findTargetSumWays(vector<int>& nums, int target) {
        int sum = 0;
        for (int num : nums) sum += num;
        
        // 边界条件: (sum + target) 必须非负且为偶数
        if ((sum + target) < 0 || (sum + target) % 2 != 0) return 0;
        
        int bagSize = (sum + target) / 2;
        int n = nums.size();
        
        // dp[i][j] = 用前 i 个数凑出和为 j 的方案数
        vector<vector<int>> dp(n + 1, vector<int>(bagSize + 1, 0));
        dp[0][0] = 1; // 什么都不选, 和为 0, 有 1 种方案
        
        for (int i = 1; i <= n; i++) {
            for (int j = 0; j <= bagSize; j++) {
                dp[i][j] = dp[i - 1][j]; // 不选第 i 个数
                if (j >= nums[i - 1]) {
                    dp[i][j] += dp[i - 1][j - nums[i - 1]]; // 选第 i 个数
                }
            }
        }
        return dp[n][bagSize];
    }
};
```

**关键点**：
- j 从 0 开始遍历而非从 nums[i-1] 开始，因为"不选"的转移对所有 j 都要做。
- dp[0][0] = 1 是唯一的初始化，表示空集和为 0。

---

### 解法4: 0/1 背包 DP（一维空间优化） — O(n × bagSize) / O(bagSize) ⭐ 面试首选

**从解法3优化**：dp[i][j] 只依赖 dp[i-1][j] 和 dp[i-1][j-num]，即只依赖上一行。可以用一维数组 + 倒序遍历来优化空间。

```
// 为什么必须倒序遍历 j？
// 
// 正序遍历的问题（以 num=1, dp=[1,0,0,0,0] 为例）:
//   j=0: dp[0] += dp[-1]? 跳过
//   j=1: dp[1] += dp[0] → dp=[1,1,0,0,0]
//   j=2: dp[2] += dp[1] → dp=[1,1,1,0,0]  ← 错! dp[1]已经被更新了
//                                              相当于 num=1 被选了两次!
//
// 倒序遍历就没问题:
//   j=4: dp[4] += dp[3] → dp=[1,0,0,0,0] (不变)
//   j=3: dp[3] += dp[2] → dp=[1,0,0,0,0] (不变)
//   j=2: dp[2] += dp[1] → dp=[1,0,0,0,0] (不变)
//   j=1: dp[1] += dp[0] → dp=[1,1,0,0,0] ✓ dp[0]还是原值
```

```cpp
class Solution {
public:
    int findTargetSumWays(vector<int>& nums, int target) {
        int sum = 0;
        for (int num : nums) sum += num;
        
        // 关键边界: (sum + target) 必须非负且为偶数
        if ((sum + target) < 0 || (sum + target) % 2 != 0) return 0;
        
        int bagSize = (sum + target) / 2;
        
        // dp[j] = 从已考虑的数中选若干个, 和恰好为 j 的方案数
        vector<int> dp(bagSize + 1, 0);
        dp[0] = 1; // 和为 0 有 1 种方案: 什么都不选
        
        for (int num : nums) {
            // 倒序遍历! 保证每个 num 只被选一次 (0/1 背包)
            for (int j = bagSize; j >= num; j--) {
                dp[j] += dp[j - num];
            }
        }
        return dp[bagSize];
    }
};
```

**关键点**：这是面试中最被青睐的解法——代码简洁，思路清晰，时间空间双优。

---

## 解法对比

|  | 回溯 DFS | 记忆化搜索 | 二维 DP | 一维 DP (⭐) |
|---|---|---|---|---|
| 时间 | O(2^n) | O(n × sum) | O(n × bagSize) | O(n × bagSize) |
| 空间 | O(n) | O(n × sum) | O(n × bagSize) | O(bagSize) |
| 思路直觉性 | 最直觉 | 自然优化 | 需要数学变换 | 需要数学变换 |
| 代码复杂度 | 简单 | 中等 | 中等 | 最简洁 |
| 适用场景 | n 很小 | 过渡理解 | 需要理解填表过程 | 面试首选 |

**关键区别**：解法1→2 是加缓存消除重复；解法2→3 是数学变换简化问题（从 remain 可正可负变成只需考虑正数和）；解法3→4 是经典的 DP 空间优化。

## 易错点

1. **忘记判断 (sum + target) < 0**
   - ✗ 只判断 `(sum + target) % 2 != 0`
   - ✓ 还要判断 `(sum + target) < 0`，因为 target 可能是负数且绝对值大于 sum
   - 例如 nums=[1], target=-2，sum+target=-1，bagSize 为负数会导致 UB

2. **忘记判断奇偶性**
   - ✗ 直接 `bagSize = (sum + target) / 2`
   - ✓ 先检查 `(sum + target) % 2 != 0` 则返回 0
   - 如果 sum + target 是奇数，不可能把数组分成整数和，无解

3. **一维 DP 正序遍历**
   - ✗ `for (int j = num; j <= bagSize; j++)` — 这是完全背包！
   - ✓ `for (int j = bagSize; j >= num; j--)` — 倒序保证 0/1 背包
   - 正序会让同一个 num 被多次选取

4. **dp[0] 初始化为 0**
   - ✗ `dp[0] = 0` — 没有初始方案，DP 永远算出全 0
   - ✓ `dp[0] = 1` — 空集的和为 0 是 1 种合法方案

5. **二维 DP 中 j 的遍历起点**
   - ✗ `for (int j = nums[i-1]; j <= bagSize; j++)` — 漏掉"不选"的情况
   - ✓ `for (int j = 0; j <= bagSize; j++)` — j < nums[i-1] 时也要写 dp[i][j] = dp[i-1][j]

## 面试追问

**Q1: 暴力 DFS 的时间复杂度是多少？能通过本题吗？**
> 2^n，n 最大 20，2^20 ≈ 10^6，可以通过。但如果 n=40 就不行了（2^40 ≈ 10^12）。

**Q2: 能否用 DP 优化？怎么定义状态？**
> 直接在 DFS 上记忆化：状态 (i, remain)，其中 remain 范围 [-sum, sum]。但更好的做法是数学变换：P = (sum + target) / 2，转化为 0/1 背包计数，状态空间更小（只有正数范围）。

**Q3: 一维 DP 为什么要倒序遍历？如果正序会怎样？**
> 正序遍历时，dp[j - num] 已经在本轮被更新过，相当于同一个 num 可以被重复选取，变成了完全背包。倒序保证用的是上一轮的 dp 值，每个 num 最多选一次。

**Q4: 如果 nums 中有大量 0 怎么办？对结果有什么影响？**
> 每个 0 可以选 + 或 -，不影响和，但方案数翻倍。如果有 k 个 0，最终答案是"去掉所有 0 后的方案数 × 2^k"。一维 DP 自然处理了这种情况：dp[j] += dp[j - 0] = dp[j]，即翻倍。

**Q5: 这道题和 416 分割等和子集有什么关系？**
> 416 是问"能否"分成两个等和子集（dp 值是 bool），本题是问"多少种"分法（dp 值是 int）。核心都是 0/1 背包，只是 dp 的含义和转移方程中的操作不同（416 用 `||`，本题用 `+`）。

## 相关题型

- **416. 分割等和子集** — 同样的 0/1 背包框架，区别：416 的 dp 是 bool（能否凑出），本题是 int（方案数）；转移从 `dp[j] = dp[j] || dp[j-num]` 变成 `dp[j] += dp[j-num]`。
- **518. 零钱兑换 II** — 完全背包计数问题，和本题对比：完全背包正序遍历 j（每个硬币可重复选），0/1 背包倒序遍历 j（每个数只能选一次），一行代码的区别。
- **474. 一和零** — 二维 0/1 背包（两个容量限制：0 的个数和 1 的个数），本题的自然扩展。
- **1049. 最后一块石头的重量 II** — 同样的数学变换思路，把问题转化为"选子集使和尽量接近 sum/2"。