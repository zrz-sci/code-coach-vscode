# 724. Find Pivot Index

## 核心思路

利用前缀和思想：先计算数组总和 total，然后从左到右遍历，维护左侧和 leftSum。对于每个位置 i，右侧和 = total - leftSum - nums[i]。当 leftSum == 右侧和时，i 即为中心下标（pivot index）。

---

## 思维链

1. **暴力思路**：对每个位置 i，分别计算左侧和与右侧和，比较是否相等。需要 O(n) 计算每个位置的两侧和，总时间 O(n^2)。
2. **优化观察**：左侧和 + nums[i] + 右侧和 = total。所以右侧和 = total - leftSum - nums[i]，不需要每次重新计算。
3. **进一步化简**：判断条件 leftSum == total - leftSum - nums[i] 等价于 2 * leftSum + nums[i] == total，只需维护一个变量 leftSum。
4. **一次遍历**：先求 total，然后从左到右遍历，每到一个位置检查条件，不满足则 leftSum += nums[i]。
5. **返回最左的**：从左到右遍历，第一个满足条件的位置就是答案。

---

## 解法概览

| 解法 | 思路 | 时间复杂度 | 空间复杂度 | 推荐度 |
|------|------|-----------|-----------|--------|
| 前缀和（一次遍历） | total - leftSum - nums[i] = leftSum | O(n) | O(1) | 首选 |
| 前缀和数组 | 建前缀和数组后查询 | O(n) | O(n) | 备选 |
| 暴力 | 每个位置重新算两侧和 | O(n^2) | O(1) | 仅理解 |

---

## 关键提示

### 提示 1 — 中心下标的数学定义

设 leftSum(i) = nums[0] + ... + nums[i-1]，rightSum(i) = nums[i+1] + ... + nums[n-1]。中心下标 i 满足 leftSum(i) == rightSum(i)。

<details>
<summary>展开</summary>

由于 leftSum(i) + nums[i] + rightSum(i) = total，所以 rightSum(i) = total - leftSum(i) - nums[i]。

判断条件变为：leftSum == total - leftSum - nums[i]，即 **2 * leftSum + nums[i] == total**。

只需维护一个变量 leftSum，从左到右累加即可。

</details>

### 提示 2 — 边界情况：i=0 和 i=n-1

<details>
<summary>展开</summary>

- i=0 时：左侧没有元素，leftSum = 0。条件变为 nums[0] == total - nums[0]，即 2*nums[0] == total。
- i=n-1 时：右侧没有元素，rightSum = 0。条件变为 leftSum == 0，即前 n-1 个元素的和为 0。

这两种边界在通用公式 2*leftSum + nums[i] == total 中自然成立，不需要特殊处理。

</details>

### 提示 3 — 负数的影响

<details>
<summary>展开</summary>

数组中可能有负数。这意味着：
- leftSum 可能为负
- 可能存在多个中心下标
- 从左到右第一个满足条件的就是答案（题目要求最左的）

负数不影响算法正确性，2*leftSum + nums[i] == total 对负数同样适用。

</details>

### 提示 4 — 为什么不需要前缀和数组？

<details>
<summary>展开</summary>

虽然可以构建前缀和数组 prefix[]，然后用 prefix[i] 和 prefix[n] - prefix[i+1] 判断。但这需要 O(n) 额外空间。

由于我们是从左到右顺序遍历，leftSum 可以实时维护（每步 O(1) 更新），不需要存储整个数组。空间从 O(n) 优化到 O(1)。

</details>

---

## 解法详解

### 解法一：前缀和 + 一次遍历（最优）

**算法步骤**：

1. 计算数组总和 total = sum(nums)
2. 初始化 leftSum = 0
3. 从左到右遍历 i = 0 到 n-1：
   - 检查 2 * leftSum + nums[i] == total
   - 若满足，返回 i
   - 否则 leftSum += nums[i]
4. 遍历结束无满足条件的位置，返回 -1

**时间复杂度**：O(n) 两次遍历（求和 + 查找）
**空间复杂度**：O(1) 只用两个变量

### 解法二：前缀和数组

**算法步骤**：

1. 构建前缀和数组 prefix[]，其中 prefix[0]=0, prefix[i+1]=prefix[i]+nums[i]
2. 遍历每个位置 i：
   - 左侧和 = prefix[i]
   - 右侧和 = prefix[n] - prefix[i+1]
   - 检查两者是否相等
3. 返回第一个满足条件的 i

**时间复杂度**：O(n)
**空间复杂度**：O(n) 前缀和数组

### 解法三：暴力

每个位置单独计算左右两侧的和。O(n^2) 时间，面试中不推荐。

---

## 易错点

1. **leftSum 更新时机**：必须先检查条件，再更新 leftSum += nums[i]。如果先更新再检查，nums[i] 会被算入左侧和
2. **返回的是最左的**：从左到右遍历，第一个满足条件的直接返回即可
3. **total 可能为奇数**：2*leftSum + nums[i] == total 中不涉及除法，不用担心奇偶问题
4. **空数组**：题目保证 nums 长度 >= 1，但如果数组只有一个元素，leftSum=0, rightSum=0，返回 0
5. **全零数组**：每个位置都是中心下标，返回 0（最左的）
6. **整数溢出**：nums[i] 范围 [-1000, 1000]，n 最大 10^4，total 最大 10^7，int 安全

---

## 面试追问

**Q1: 这道题和 LeetCode 1991 "Find the Middle Index in Array" 有什么关系？**

完全相同的题目，只是换了个名字。解法、边界条件、时间空间复杂度全部一致。

**Q2: 如果要找所有中心下标怎么办？**

继续遍历，不在第一个满足条件时返回，而是收集所有满足条件的 i 到结果数组中。时间复杂度不变。

**Q3: 如果数组是循环的（首尾相连），中心下标怎么定义？**

循环数组中，每个位置的"左侧"和"右侧"都是数组去掉该元素后的一半。可以用总和减去 nums[i]，检查是否为偶数，并且前半段和等于 (total - nums[i]) / 2。需要额外考虑循环遍历的起止点。

**Q4: 能否用二分搜索？**

不能直接二分。leftSum 不一定单调递增（因为可能有负数），所以无法通过二分定位中心下标。必须线性扫描。

**Q5: 如果是二维矩阵呢？**

二维版本中"中心位置"的定义更复杂。一种定义：行 r 满足上方所有行的和等于下方所有行的和。可以用行前缀和 O(n) 解决。如果是行列双中心，需要分别检查行中心和列中心。

---

## 相关题型

| 题号 | 题目 | 关联点 |
|------|------|--------|
| 238 | Product of Array Except Self | 前缀积/前缀和思维 |
| 560 | Subarray Sum Equals K | 前缀和 + HashMap |
| 303 | Range Sum Query - Immutable | 前缀和基础 |
| 1991 | Find the Middle Index in Array | 完全相同的题目 |
| 1480 | Running Sum of 1d Array | 前缀和入门 |
| 523 | Continuous Subarray Sum | 前缀和 + 模运算 |
