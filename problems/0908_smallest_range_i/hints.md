# 908. 最小差值 I (Smallest Range I)

## 核心思路

本题本质上是：**每个元素可以加减至多 k，问调整后数组的最大值与最小值之差的最小值**。关键洞察是只需关注原始数组的 `max - min`：如果 `max - min <= 2k`，差值可以缩为 0；否则差值最小为 `max - min - 2k`。

## 思维链

1. **读完题的第一反应**：每个元素 nums[i] 可以变成 [nums[i]-k, nums[i]+k] 范围内的任意值。目标是让调整后的最大值和最小值尽量接近。

2. **关键观察 -- 只有 max 和 min 重要**：中间的元素不影响"分数"(最大最小之差)。我们的目标是缩小 max 和 min 之间的距离。

3. **最优策略**：
   - 最大值尽量减小：max → max - k
   - 最小值尽量增大：min → min + k
   - 调整后的差值 = (max - k) - (min + k) = max - min - 2k

4. **但差值不能为负**：如果 max - min <= 2k，说明通过调整可以让所有元素相等（差值为 0）。取 max(0, max - min - 2k)。

5. **一行公式**：`return max(0, (maxVal - minVal) - 2 * k);`

6. **为什么中间元素无所谓？** 因为中间元素的调整范围包含了 max-k 到 min+k 之间的任何值。只要 max-k 和 min+k 能"碰头"，所有中间元素都能调整到同一值。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 数学公式 | max(0, maxVal-minVal-2k) | O(n) | O(1) | ⭐ 必须写出 |
| 排序后取首尾 | 排序找max/min | O(n log n) | O(1) | 能说不推荐 |

## 关键提示

1. **只需要 max 和 min**：不需要排序，一次遍历找到最大值和最小值即可。

2. **直觉理解**：
   - 原始差距 = max - min
   - 我们可以把 max 最多减少 k，把 min 最多增加 k
   - 总共能缩小 2k 的差距
   - 但差距不能缩到负数

3. **ASCII 示意图**：
```
原始:    min ←————————— gap ——————————→ max
                    gap = max - min

调整后:  min+k ←——— new gap ———→ max-k
         new gap = gap - 2k

如果 gap <= 2k:
         min+k ≥ max-k，范围重叠，所有数可以相等
         答案 = 0

如果 gap > 2k:
         答案 = gap - 2k

  [min]         [min+k]     [max-k]         [max]
    |──── +k ────→|           |←──── -k ──────|
                  |←new gap→ |
```

4. **示例验证**：
   - nums=[1], k=0: max=1, min=1, gap=0 → max(0, 0-0) = 0
   - nums=[0,10], k=2: max=10, min=0, gap=10 → max(0, 10-4) = 6
   - nums=[1,3,6], k=3: max=6, min=1, gap=5 → max(0, 5-6) = max(0,-1) = 0

5. **Edge case**：数组只有一个元素 → max == min → gap = 0 → 答案 = 0。

## 解法详解

### 解法1: 数学公式 -- O(n) / O(1) ⭐ 面试首选

**思考过程**：

核心洞察：**"分数"只取决于极端值，中间值怎么调都不影响**。最小化分数 = 最小化 (调整后max - 调整后min)。最佳策略是把 max 尽量往下拉 k，把 min 尽量往上推 k。

```cpp
class Solution {
public:
    int smallestRangeI(vector<int>& nums, int k) {
        // 一次遍历找到最大值和最小值
        int maxVal = *max_element(nums.begin(), nums.end());
        int minVal = *min_element(nums.begin(), nums.end());
        
        // 原始差距减去我们能缩小的量(2k)，但不能小于0
        return max(0, (maxVal - minVal) - 2 * k);
    }
};
```

**关键点**：
- `max_element` 和 `min_element` 各遍历一次，总共 O(n)
- 也可以在一次循环中同时找 max 和 min，减少一次遍历
- 公式 `max(0, gap - 2k)` 包含了所有 edge case

### 解法2: 单次遍历手动找 max/min -- O(n) / O(1)

**优化遍历次数**：只遍历一次同时找到 max 和 min。

```cpp
class Solution {
public:
    int smallestRangeI(vector<int>& nums, int k) {
        int maxVal = nums[0], minVal = nums[0];
        
        for (int i = 1; i < (int)nums.size(); i++) {
            maxVal = max(maxVal, nums[i]);
            minVal = min(minVal, nums[i]);
        }
        
        int gap = maxVal - minVal;
        return max(0, gap - 2 * k);
    }
};
```

### 解法3: 排序法 -- O(n log n) / O(1)

**最直观但不推荐**：排序后取首尾。

```cpp
class Solution {
public:
    int smallestRangeI(vector<int>& nums, int k) {
        sort(nums.begin(), nums.end());
        
        int gap = nums.back() - nums.front();
        return max(0, gap - 2 * k);
    }
};
```

## 易错点

1. **✗ 公式写成 `max - min - k` 而不是 `max - min - 2k`**：
   ```cpp
   // 错误：return max(0, maxVal - minVal - k);
   // 正确：return max(0, maxVal - minVal - 2 * k);
   // 因为 max 可以减 k，min 可以加 k，总共缩小 2k
   ```

2. **✗ 忘记和 0 取 max**：如果差距 < 2k，差值可以缩到 0（甚至重叠），但不能是负数。
   ```cpp
   // 错误：return maxVal - minVal - 2 * k; // 可能返回负数
   // 正确：return max(0, maxVal - minVal - 2 * k);
   ```

3. **✗ 试图对每个元素单独优化**：这道题不需要考虑中间元素怎么调整，只需要关注 max 和 min。过度复杂化反而出错。

4. **✗ 和 Smallest Range II (910) 混淆**：910 要求每个元素要么 +k 要么 -k（二选一，且必须选），比本题难很多。本题是"加任意 [-k, k]"，自由度更大。

## 面试追问

**Q1: 为什么中间的元素不影响答案？**
→ "分数"定义为 max - min。只要最终的最大值和最小值确定了，中间元素的值无关紧要。而中间元素的调整范围 [nums[i]-k, nums[i]+k] 一定包含了 [min+k, max-k] 区间（因为 min <= nums[i] <= max），所以它们总能调整到最终的 [min+k, max-k] 范围内。

**Q2: 如果改成 Smallest Range II（每个元素必须选 +k 或 -k），怎么做？**
→ 那就变成了 LC 910，难度显著增加。排序后枚举分界点：前半部分 +k，后半部分 -k。对每个分界点 i，新的 max = max(nums[i]+k, nums[n-1]-k)，新的 min = min(nums[0]+k, nums[i+1]-k)。O(n log n)。

**Q3: 如果可以对每个元素操作多次呢？**
→ 本题每个元素"at most once"操作，范围是 [-k, k]。如果可以操作多次，等价于每个元素可以变成任意值 → 答案始终为 0（除非 k=0）。

## 相关题型

- **910. 最小差值 II (Smallest Range II)** -- 本题的进阶版。区别：910 每个元素必须 +k 或 -k（二选一），不能选中间值。需要排序 + 枚举分界点。本题因为可以加任意 [-k,k]，自由度更大，直接公式即可。

- **628. 三个数的最大乘积 (Maximum Product of Three Numbers)** -- 同样只需要关注极端值（最大/最小值），不需要完整排序。"只需极端值"的思维方式是共通的。

- **561. 数组拆分 (Array Partition)** -- 另一道"通过调整/分组来优化极端值差"的题目。本题最小化极差，561最大化分组最小值之和。都需要理解极端值对答案的影响。
