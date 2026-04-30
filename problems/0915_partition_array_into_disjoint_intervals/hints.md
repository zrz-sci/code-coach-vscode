[file://solution.cpp](./solution.cpp)

# 915. 分隔数组为连续子数组

## 核心思路

给定数组 nums，要找到一个最小的分割点 i，使得 left = nums[0..i-1] 中的每个元素都小于等于 right = nums[i..n-1] 中的每个元素。换言之，left 的最大值 <= right 的最小值。核心观察：这等价于找最小的 i 使得 max(nums[0..i-1]) <= min(nums[i..n-1])。我们可以通过预处理前缀最大值和后缀最小值来高效完成，也可以用单次遍历的贪心策略。

## 思维链

1. **读完题第一反应**：暴力做法——枚举每个分割点 i（从 1 到 n-1），对每个 i 分别算 left 的最大值和 right 的最小值，检查是否满足条件。时间 O(n^2)。
2. **暴力解的瓶颈**：对每个分割点都重新计算最大值/最小值，大量重复计算。能否预处理？
3. **自然优化——前缀最大 + 后缀最小**：
   - 用数组 `leftMax[i]` 存储 nums[0..i] 的最大值（前缀最大值）
   - 用数组 `rightMin[i]` 存储 nums[i..n-1] 的最小值（后缀最小值）
   - 然后找最小的 i 使得 `leftMax[i-1] <= rightMin[i]`
   - 时间 O(n)，空间 O(n)
4. **能否省掉额外数组？** 可以！用一次遍历的贪心方法：维护当前分割点 partitionIdx、左半部分的最大值 leftMax、以及全局到当前位置的最大值 globalMax。当 nums[i] < leftMax 时，说明 nums[i] 必须被包含在左半部分，因此更新 partitionIdx = i，同时 leftMax = globalMax。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 前缀最大 + 后缀最小 | 预处理两个数组，扫一遍找分割点 | O(n) | O(n) | 容易理解 |
| 一次遍历贪心 | 维护 leftMax 和 globalMax，动态扩展左半部分 | O(n) | O(1) | 面试首选 |

## 关键提示

1. **分割条件的等价转化**：left 的所有元素 <= right 的所有元素，等价于 max(left) <= min(right)。这是把"所有对"的比较简化为两个极值的比较。
2. **前缀最大/后缀最小的预处理模式**：这是数组问题的经典套路——当需要"左边的某种聚合值"和"右边的某种聚合值"做比较时，先预处理前缀和后缀数组。
3. **贪心的核心直觉**：从左向右遍历时，如果遇到一个比当前 leftMax 还小的元素，那这个元素没法放在 right 中（因为 right 中每个元素都要 >= leftMax），所以必须把左半部分扩展到包含这个元素。扩展之后，leftMax 可能要更新为到目前为止见过的全局最大值。
4. **为什么维护 globalMax？** 当我们把分割点右移到 i 时，左半部分包含了 nums[0..i]，其最大值就是 globalMax（到 i 位置见过的最大值），而不仅仅是之前的 leftMax。

```
// 贪心过程示意 (nums = [5, 0, 3, 8, 6]):
//
// i=0: 初始化 partitionIdx=0, leftMax=5, globalMax=5
//
// i=1: nums[1]=0 < leftMax=5
//      → 0 必须归入左半部分
//      → partitionIdx=1, leftMax=globalMax=5
//      globalMax=max(5,0)=5
//
// i=2: nums[2]=3 < leftMax=5
//      → 3 必须归入左半部分
//      → partitionIdx=2, leftMax=globalMax=5
//      globalMax=max(5,3)=5
//
// i=3: nums[3]=8 >= leftMax=5 → 不用扩展
//      globalMax=max(5,8)=8
//
// i=4: nums[4]=6 >= leftMax=5 → 不用扩展
//      globalMax=max(8,6)=8
//
// 结果: partitionIdx=2, 返回 partitionIdx+1 = 3
// left=[5,0,3], right=[8,6] ✓
```

## 解法详解

### 解法1: 前缀最大 + 后缀最小 — O(n) / O(n)

**思考过程**：最直觉的优化——预处理前缀最大值数组和后缀最小值数组，然后一次扫描找到分割点。

```cpp
class Solution {
public:
    int partitionDisjoint(vector<int>& nums) {
        int n = nums.size();
        
        // leftMax[i] = max(nums[0], nums[1], ..., nums[i])
        vector<int> leftMax(n);
        leftMax[0] = nums[0];
        for (int i = 1; i < n; i++) {
            leftMax[i] = max(leftMax[i - 1], nums[i]);
        }
        
        // rightMin[i] = min(nums[i], nums[i+1], ..., nums[n-1])
        vector<int> rightMin(n);
        rightMin[n - 1] = nums[n - 1];
        for (int i = n - 2; i >= 0; i--) {
            rightMin[i] = min(rightMin[i + 1], nums[i]);
        }
        
        // 找最小的分割点: left = nums[0..i], right = nums[i+1..n-1]
        // 条件: leftMax[i] <= rightMin[i+1]
        for (int i = 0; i < n - 1; i++) {
            if (leftMax[i] <= rightMin[i + 1]) {
                return i + 1;  // left 的长度
            }
        }
        
        return -1;  // 题目保证有解
    }
};
```

**关键点**：两个辅助数组各需要 O(n) 空间。逻辑清晰但空间可优化。

---

### 解法2: 一次遍历贪心 — O(n) / O(1) 面试首选

**从解法1优化**：解法1需要两个额外数组，能否用常数空间？关键观察——我们从左向右遍历时，可以动态维护"左半部分的最大值"和"全局最大值"。每当遇到一个元素比 leftMax 小，就必须把左半部分扩展到包含它，此时 leftMax 更新为 globalMax。

```cpp
class Solution {
public:
    int partitionDisjoint(vector<int>& nums) {
        int n = nums.size();
        int partitionIdx = 0;       // 当前分割点（左半部分的最后一个索引）
        int leftMax = nums[0];      // 左半部分的最大值
        int globalMax = nums[0];    // 到目前为止见过的最大值
        
        for (int i = 1; i < n; i++) {
            globalMax = max(globalMax, nums[i]);
            
            if (nums[i] < leftMax) {
                // nums[i] 比左半部分的最大值还小
                // 它不可能待在右半部分（右半部分的每个元素都要 >= leftMax）
                // 所以必须把左半部分扩展到 i
                partitionIdx = i;
                // 扩展后左半部分包含了 nums[0..i]，最大值变成 globalMax
                leftMax = globalMax;
            }
        }
        
        return partitionIdx + 1;  // left 的长度 = 最后一个索引 + 1
    }
};
```

**关键点**：
- `leftMax` 和 `globalMax` 的区别至关重要：leftMax 是当前左半部分的最大值，globalMax 是遍历过的所有元素的最大值。当扩展左半部分时，leftMax 要跳到 globalMax。
- 这个贪心是正确的，因为如果某个元素 nums[i] < leftMax，那它放在右半部分会违反条件，所以必须放在左半部分。

## 解法对比

| | 前缀最大 + 后缀最小 | 一次遍历贪心 |
|---|---|---|
| 时间 | O(n) | O(n) |
| 空间 | O(n) | O(1) |
| 核心思想 | 预处理两个数组比较 | 动态维护两个最大值 |
| 代码复杂度 | 简单直观 | 稍微需要理解 |
| 面试推荐 | 开场过渡 | **首选** |

**什么时候选哪个？**
- 面试时先口述解法1的思路，说明可以用前缀/后缀数组做，然后直接写空间优化的解法2。
- 如果解法2不好理解，退回解法1也完全可以。

## 易错点

1. **分割点返回的是长度而非索引**
   - 题目要求返回 left 的长度，不是最后一个索引。如果 partitionIdx 是左半部分的最后一个索引，返回 partitionIdx + 1。

2. **混淆 leftMax 和 globalMax**
   - leftMax 在没有扩展时不会改变，但 globalMax 每步都在更新。只有当分割点右移时，leftMax 才跳到 globalMax。

3. **忘记初始化 partitionIdx = 0**
   - 初始时左半部分至少包含第一个元素，所以 partitionIdx 从 0 开始（不是 -1）。

4. **前缀/后缀数组边界处理错误**
   - leftMax[0] = nums[0]，rightMin[n-1] = nums[n-1]。检查分割点时 i 的范围是 0 到 n-2（因为 right 不能为空）。

## 面试追问

**Q1: 暴力解怎么做？瓶颈在哪？**
> 对每个分割点计算 left 的 max 和 right 的 min，O(n^2)。瓶颈是重复计算——每次分割都从头算聚合值。

**Q2: 解法2的贪心为什么正确？**
> 如果 nums[i] < leftMax，把 nums[i] 放在右半部分会违反 max(left) <= min(right)，所以 nums[i] 必须在左半部分。扩展左半部分后，新的 leftMax = globalMax（因为左半部分现在包含到 i 了）。

**Q3: 如果有多个合法分割点，这个算法为什么能找到最小的？**
> 从左到右遍历时，只在必要时（nums[i] < leftMax）才扩展。一旦没有必要扩展了，当前分割点就是最小的合法分割点。

**Q4: 如果改成"左半部分的最大值严格小于右半部分的最小值"，代码怎么改？**
> 把比较条件从 `nums[i] < leftMax` 改成 `nums[i] <= leftMax`，因为等于也不满足"严格小于"。

## 相关题型

- **238. Product of Array Except Self** — 同样的"前缀/后缀"预处理技巧，前缀乘积 + 后缀乘积，模式完全一致。
- **42. Trapping Rain Water** — 前缀最大值 + 后缀最大值的经典应用，预处理模式相同。
- **135. Candy** — 类似的两次遍历（左到右 + 右到左）确定每个位置的值，思路可以互相借鉴。
