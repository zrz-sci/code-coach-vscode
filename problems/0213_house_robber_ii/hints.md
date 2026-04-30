# 213. 打家劫舍 II

## 核心思路

这道题本质上是**环形约束下的线性 DP**：房屋围成一圈导致第一个和最后一个互为邻居，不能同时选。解决方法是**把环拆成两条链**，分别跑一次 House Robber I，取最大值。

## 与 House Robber I (198题) 的关系

> **系列题对比**：198 是直线排列，本题是环形排列。唯一的区别是首尾相邻的约束。代码上只需要在 198 的基础上调用两次线性打家劫舍，改变的仅仅是传入的数组范围。

## 思维链

1. **读完题第一反应**：和 198 题几乎一样，但房屋围成了圈。暴力做法是枚举所有不相邻的子集，指数级复杂度。
2. **回忆 198 的解法**：线性排列时，dp[i] = max(dp[i-1], dp[i-2] + nums[i])，O(n) 搞定。能不能复用？
3. **环形带来的问题**：如果选了 nums[0]，就不能选 nums[n-1]；如果没选 nums[0]，nums[n-1] 可以选。这两种情况互斥。
4. **关键突破：拆环为链**：
   - 情况A：考虑 nums[0..n-2]（可能选第一个，一定不选最后一个）
   - 情况B：考虑 nums[1..n-1]（一定不选第一个，可能选最后一个）
   - 答案 = max(情况A, 情况B)
5. **为什么这样拆是完备的**？任何合法方案，要么不包含 nums[n-1]（被情况A覆盖），要么不包含 nums[0]（被情况B覆盖）。没有遗漏。
6. **空间优化**：每次线性 DP 只依赖前两个状态，用两个变量即可，O(1) 空间。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力枚举子集 | 枚举所有不相邻的组合 | O(2^n) | O(n) | 能说出即可 |
| DP 数组 | 拆成两条链，各跑一次线性 DP | O(n) | O(n) | 能说出即可 |
| DP + 空间优化 | 滚动变量代替数组 | O(n) | O(1) | ⭐ 必须写出 |

## 关键提示

1. **想一想**：如果你已经会做 198. House Robber I，本题唯一新增的约束是什么？
2. **拆环**：环形结构 = 首尾互斥。任何合法方案要么不含第一个，要么不含最后一个——能不能分两种情况讨论？
3. **边界**：n=1 时只有一个房子，直接返回 nums[0]。别忘了特判。
4. **复用**：写一个 `robLinear(nums, start, end)` 辅助函数，调用两次就是完整解法。
5. **DP 填表方向和依赖**：
```
// nums = [1, 2, 3, 1]
// 
// 情况A: nums[0..2] = [1, 2, 3]
//   dp[0]=1, dp[1]=max(1,2)=2, dp[2]=max(2, 1+3)=4
//
// 情况B: nums[1..3] = [2, 3, 1]
//   dp[0]=2, dp[1]=max(2,3)=3, dp[2]=max(3, 2+1)=3
//
// 答案 = max(4, 3) = 4 ✓
```

## 解法详解

### 解法1: 暴力递归 — O(2^n) / O(n)

**思考过程**：最直觉的方式——对每个房子决定"偷"或"不偷"，递归枚举所有可能，同时处理环形约束（如果偷了第一个，最后一个不能偷）。

```cpp
class Solution {
public:
    int rob(vector<int>& nums) {
        int n = nums.size();
        if (n == 1) return nums[0];
        // 情况A: 偷第一个（那最后一个不能偷）→ 在 [0, n-2] 上递归
        // 情况B: 不偷第一个 → 在 [1, n-1] 上递归
        return max(helper(nums, 0, n - 2), helper(nums, 1, n - 1));
    }
    
    // 在 nums[start..end] 上暴力递归：从 idx 开始能偷到的最大金额
    int helper(vector<int>& nums, int start, int end) {
        if (start > end) return 0;
        // 选择1: 偷 start，跳过 start+1
        // 选择2: 不偷 start，从 start+1 继续
        return max(nums[start] + helper(nums, start + 2, end),
                   helper(nums, start + 1, end));
    }
};
```

**关键点**：这个解法展示了问题的本质——每一步的"偷/不偷"决策。但大量重叠子问题导致指数级复杂度。

---

### 解法2: DP 数组 — O(n) / O(n)

**从解法1优化**：暴力递归有大量重叠子问题（helper(3,5) 可能被算很多次），用 dp 数组缓存结果。

```cpp
class Solution {
public:
    int rob(vector<int>& nums) {
        int n = nums.size();
        if (n == 1) return nums[0];
        // 拆环为两条链，各跑一次线性 House Robber
        return max(robLinear(nums, 0, n - 2), robLinear(nums, 1, n - 1));
    }
    
private:
    // 198题的标准线性DP解法，在 nums[start..end] 上运行
    int robLinear(vector<int>& nums, int start, int end) {
        int len = end - start + 1;
        if (len == 1) return nums[start];
        
        vector<int> dp(len);
        dp[0] = nums[start];
        dp[1] = max(nums[start], nums[start + 1]);
        
        // dp[i] = 考虑前 i+1 个房子能偷到的最大值
        // 状态转移: 偷第i个 → dp[i-2]+nums[i]，不偷 → dp[i-1]
        for (int i = 2; i < len; i++) {
            dp[i] = max(dp[i - 1], dp[i - 2] + nums[start + i]);
        }
        return dp[len - 1];
    }
};
```

**DP 填表过程**：
```
// nums = [2, 3, 2]
//
// 情况A: robLinear([2, 3], 0, 1)
//   dp = [2, 3]  → 结果 3
//
// 情况B: robLinear([3, 2], 1, 2)
//   dp = [3, 3]  → 结果 3
//
// 答案 = max(3, 3) = 3 ✓
```

---

### 解法3: DP + 滚动变量空间优化 — O(n) / O(1) ⭐ 面试首选

**从解法2优化**：dp[i] 只依赖 dp[i-1] 和 dp[i-2]，不需要整个数组，用两个变量 prev2 和 prev1 滚动即可。

```cpp
class Solution {
public:
    int rob(vector<int>& nums) {
        int n = nums.size();
        if (n == 1) return nums[0];  // 特判: 只有一个房子直接偷
        
        // 拆环为两条链: [0, n-2] 和 [1, n-1]
        return max(robLinear(nums, 0, n - 2), robLinear(nums, 1, n - 1));
    }
    
private:
    int robLinear(vector<int>& nums, int start, int end) {
        int prev2 = 0;  // dp[i-2]
        int prev1 = 0;  // dp[i-1]
        
        for (int i = start; i <= end; i++) {
            int curr = max(prev1, prev2 + nums[i]);  // 偷 or 不偷
            prev2 = prev1;  // 滚动: 为下一轮做准备
            prev1 = curr;
        }
        return prev1;
    }
};
```

**滚动过程图示**：
```
// nums = [1, 2, 3, 1], robLinear(0, 2) → [1, 2, 3]
//
// i=0: prev2=0, prev1=0 → curr=max(0, 0+1)=1 → prev2=0, prev1=1
// i=1: prev2=0, prev1=1 → curr=max(1, 0+2)=2 → prev2=1, prev1=2
// i=2: prev2=1, prev1=2 → curr=max(2, 1+3)=4 → prev2=2, prev1=4
// 返回 4
//
// robLinear(1, 3) → [2, 3, 1]
// i=1: prev2=0, prev1=0 → curr=max(0, 0+2)=2 → prev2=0, prev1=2
// i=2: prev2=0, prev1=2 → curr=max(2, 0+3)=3 → prev2=2, prev1=3
// i=3: prev2=2, prev1=3 → curr=max(3, 2+1)=3 → prev2=3, prev1=3
// 返回 3
//
// 答案 = max(4, 3) = 4 ✓
```

## 解法对比

| | 暴力递归 | DP 数组 | DP 滚动变量 |
|---|---|---|---|
| 时间 | O(2^n) | O(n) | O(n) |
| 空间 | O(n) 栈深 | O(n) 数组 | O(1) |
| 核心 | 穷举所有选择 | 缓存子问题结果 | 只保留最近两个状态 |
| 面试 | 说思路即可 | 中间过渡 | **首选** |

**解法之间的递进关系**：暴力递归 → 发现重叠子问题 → 加 memo/改递推 → 发现只依赖两个前驱 → 滚动变量。

## 易错点

1. **忘记特判 n=1**：
   - ✗ 直接 `robLinear(0, n-2)` 当 n=1 时 end=−1，区间为空返回 0，结果偷不了唯一的房子
   - ✓ 先 `if (n == 1) return nums[0];`

2. **环拆链的范围搞错**：
   - ✗ `robLinear(0, n-1)` 和 `robLinear(1, n)`——第一个没去掉最后一个，第二个越界
   - ✓ `robLinear(0, n-2)` 和 `robLinear(1, n-1)`

3. **滚动变量初始化错误**：
   - ✗ `prev2 = nums[start], prev1 = nums[start+1]`——当 start==end 时 start+1 越界
   - ✓ `prev2 = 0, prev1 = 0`，让循环自然处理所有情况

4. **认为"不选第一个"和"不选最后一个"会重复计算**：
   - 确实会有重叠（两者都不选的方案被算了两次），但我们取的是 max，重复计算不影响正确性

## 面试追问

**Q1: 为什么拆成两条链就能覆盖所有情况？有没有遗漏？**

> 对于任何合法的偷窃方案，第一个和最后一个房子不可能同时被偷。所以要么方案不包含最后一个（被 [0, n-2] 覆盖），要么不包含第一个（被 [1, n-1] 覆盖）。这两种情况并集 = 所有合法方案。

**Q2: 如果有 3 个房子围成环怎么办？你的解法能处理 n=2 吗？**

> n=2 时：robLinear(0,0) = nums[0]，robLinear(1,1) = nums[1]，返回 max(nums[0], nums[1])。完全正确——两个相邻的房子只能偷一个。n=3 同理，robLinear 处理长度为 2 的子数组没有任何问题。

**Q3: 如果把环扩展成更一般的图（比如树形结构），怎么偷？**

> 这就是 **337. House Robber III**（树形DP）。每个节点返回一对值 (偷自己的最大值, 不偷自己的最大值)，自底向上做树形 DP。核心思想一样：对每个节点做"偷/不偷"的决策。

## 相关题型

- **198. House Robber I** — 本题的前置题，线性排列。本题直接复用 198 的 `robLinear` 函数，只是调用两次。**代码差异：只多了一行 `max(robLinear(0,n-2), robLinear(1,n-1))`**。
- **337. House Robber III** — 系列的下一题，从线性/环形升级到树形。复用"偷/不偷"的决策框架，但用树形 DP（后序遍历）代替线性 DP。
- **740. Delete and Earn** — 本质上也是 House Robber：选了数字 x 就不能选 x-1 和 x+1，将值域排列后变成线性打家劫舍。
- **1388. Pizza With 3n Slices** — 环形选不相邻元素的进阶版，选 n 个，需要二维 DP。