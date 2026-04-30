# 719. Find K-th Smallest Pair Distance

## 核心思路

在值域上二分搜索答案，利用排序后的双指针在 O(n) 时间内统计"距离 <= mid 的数对数量"，通过单调性快速定位第 k 小的距离值。

---

## 思维链

1. **暴力思路**：枚举所有 C(n,2) 个数对，算出距离后排序取第 k 个。n 最大 10^4，数对数可达 5*10^7，排序 TLE。
2. **能不能避免枚举所有对？** 如果能快速判断"第 k 小的距离是否 <= d"，就可以在 d 的值域上二分答案。
3. **单调性验证**：countPairs(d) = "距离 <= d 的数对数量"关于 d 单调不减。d 越大，满足条件的对越多。
4. **如何高效计算 countPairs(d)？** 排序后，对于固定右端点 right，left 只需单调右移，双指针 O(n) 完成计数。
5. **最终方案**：sort + binary search on value range + two pointers counting。

---

## 解法概览

| 解法 | 思路 | 时间复杂度 | 空间复杂度 | 推荐度 |
|------|------|-----------|-----------|--------|
| 二分 + 双指针 | 值域二分 + 排序双指针计数 | O(n log n + n log W) | O(log n) | 首选 |
| 二分 + upper_bound | 值域二分 + 每轮 upper_bound 计数 | O(n log n * log W) | O(log n) | 备选 |
| 暴力排序 | 枚举所有对并排序 | O(n^2 log n) | O(n^2) | 仅验证 |

---

## 关键提示

### 提示 1 — 为什么能二分？

countPairs(d) 函数是关于 d 的单调非递减函数。思考：如果距离 <= d 有 m 对，那么距离 <= d+1 至少也有 m 对。

<details>
<summary>展开</summary>

```
d:         0   1   2   3   4   5
count(d):  1   1   3   4   4   6
                   ^
                   第一个 >= k=3 的 d 就是答案
```

我们需要找最小的 d 使得 count(d) >= k，这正是二分搜索"找左边界"的经典模板。

</details>

### 提示 2 — 排序之后双指针怎么工作？

排序后，对固定的 right，满足 nums[right] - nums[left] <= mid 的 left 构成连续区间。

<details>
<summary>展开</summary>

排序后数组: [1, 1, 3, 6]，mid = 2

- right=1: nums[1]-nums[0]=0 <= 2, left 不动, 贡献 1-0=1
- right=2: nums[2]-nums[0]=2 <= 2, left 不动, 贡献 2-0=2
- right=3: nums[3]-nums[0]=5 > 2, left++; nums[3]-nums[1]=5>2, left++; nums[3]-nums[2]=3>2, left++; 贡献 3-3=0

总计 3 对，正好 >= k=3，所以 mid=2 是一个可行答案。

**核心**: left 随 right 递增而单调不减，总移动次数 O(n)。

</details>

### 提示 3 — 二分边界与收敛

lo = 0, hi = nums[n-1] - nums[0]（排序后最大距离）。while(lo < hi)，lo==hi 时即为答案。

<details>
<summary>展开</summary>

- cnt >= k → hi = mid（mid 本身可能就是答案，不能跳过）
- cnt < k → lo = mid + 1（mid 一定不是答案）

**不要写 hi = mid - 1**：如果当前 mid 就是答案，缩小 hi 到 mid-1 会漏掉它。

</details>

### 提示 4 — 答案一定是真实存在的距离值吗？

<details>
<summary>展开</summary>

是的。设答案为 d，则 count(d) >= k 且 count(d-1) < k。这意味着恰好有一些数对的距离等于 d，所以 d 必然对应某个实际的数对距离。如果 d 不是任何一对的距离，那么 count(d) == count(d-1)，这与 count(d) >= k > count(d-1) 矛盾。

</details>

---

## 解法详解

### 解法一：二分答案 + 双指针（最优）

**算法步骤**：

1. 对 nums 排序
2. 设 lo=0, hi=nums[n-1]-nums[0]
3. 每轮取 mid=(lo+hi)/2，用双指针统计距离 <= mid 的数对数
4. 根据计数与 k 的关系收缩搜索区间
5. lo == hi 时返回答案

**双指针计数的精确实现**：
- left 初始化为 0，在 for 循环外
- right 从 1 遍历到 n-1
- 当 nums[right] - nums[left] > mid 时 left++
- 每个 right 贡献 right - left 个有效数对

**时间复杂度**：O(n log n + n log W)，其中 W = max - min
**空间复杂度**：O(log n) 仅排序栈

### 解法二：二分 + upper_bound

将双指针计数替换为：对每个 left，用 upper_bound 在 [left+1, n) 中找第一个大于 nums[left]+mid 的位置，差值即贡献。

每轮计数 O(n log n)，总时间 O(n log n * log W)，略慢但更直观。

### 解法三：暴力枚举

枚举所有 C(n,2) 对，用 nth_element 找第 k 小。时间 O(n^2)，n=10^4 时可能 TLE。仅用于小规模验证。

---

## 易错点

1. **忘记排序**：双指针计数依赖数组有序，未排序会得到错误结果
2. **二分模板选错**：应使用"找第一个 >= k"的左边界模板，cnt >= k 时 hi = mid 而非 hi = mid - 1
3. **left 每轮重置**：left 应在 for 循环外初始化一次，不要在 right 循环体内重置为 0
4. **计数溢出**：count 在极端情况下可达 n*(n-1)/2 ~ 5*10^7，用 int 勉强够但 long long 更安全
5. **hi 初始值错误**：hi 应为 nums.back() - nums[0]（排序后），不是 max_element - min_element 的原始位置

---

## 面试追问

**Q1: 为什么二分一定能找到一个真实存在的距离值？**

countPairs(d) 是阶梯函数，只在真实距离值处跳增。二分找的是第一个 count >= k 的 d，如果 d 不是真实距离，则 count(d) == count(d-1)，两者都 >= k，那 d 就不是"第一个"，矛盾。所以 d 一定对应某个实际数对距离。

**Q2: 重复元素对双指针计数有影响吗？**

无影响。排序后相同元素相邻，距离为 0。假设有 m 个相同元素，它们贡献 C(m,2) 对距离为 0 的数对，双指针通过 right - left 的累加会精确计入所有这些对。

**Q3: 能否用桶排序优化？**

如果值域 W 小（如 nums[i] <= 10^6），可以用计数数组加前缀和直接计算距离分布，时间 O(W)。但面试中排序+二分+双指针是最安全、最通用的方案。

**Q4: 如果要找第 k 大而非第 k 小？**

将 k 转换为 totalPairs - k + 1，其中 totalPairs = n*(n-1)/2，然后用同样的方法找第 (totalPairs - k + 1) 小。

---

## 相关题型

| 题号 | 题目 | 关联点 |
|------|------|--------|
| 378 | Kth Smallest Element in a Sorted Matrix | 二分答案 + 计数 |
| 668 | Kth Smallest Number in Multiplication Table | 二分答案 + 行计数 |
| 786 | K-th Smallest Prime Fraction | 二分答案变体 |
| 373 | Find K Pairs with Smallest Sums | 堆解法对比 |
| 2040 | Kth Smallest Product of Two Sorted Arrays | 二分答案 + 双指针 |
| 4 | Median of Two Sorted Arrays | 二分搜索的值域思维 |
