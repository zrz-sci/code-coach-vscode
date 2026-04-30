# 81. 搜索旋转排序数组 II

## 核心思路

本题本质上是在一个**含重复元素的旋转有序数组**中做二分查找。和第 33 题（无重复元素）相比，核心难点在于：当 `nums[left] == nums[mid] == nums[right]` 时，**无法判断哪半边是有序的**，只能退化为线性收缩。

## 思维链

1. **读完题第一反应**：遍历一遍数组找 target，O(n) 一定能解决。但题目要求"尽可能减少操作步骤"，暗示要用二分。

2. **回忆第 33 题（无重复版）**：旋转数组虽然整体无序，但 mid 把数组分成两半，至少有一半是有序的。我们可以判断 target 是否在有序的那一半中，从而决定搜索方向。

3. **重复元素带来了什么问题？** 当 `nums[left] == nums[mid]` 时，无法判断左半边是有序还是包含旋转点。例如 `[1,3,1,1,1]` 和 `[1,1,1,3,1]`，mid 都指向 1，left 也是 1，但旋转点位置完全不同。

4. **怎么处理？** 当 `nums[left] == nums[mid]` 时，我们无法做出有效判断，唯一能做的是 `left++` 跳过一个重复元素。这一步是线性的，所以最坏情况时间退化为 O(n)。

5. **总结策略**：二分框架不变，只需在第 33 题的基础上加一个前置判断——遇到 `nums[left] == nums[mid]` 且 `nums[mid] == nums[right]` 时，收缩边界。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力遍历 | 线性扫描 | O(n) | O(1) | 能说出即可 |
| 二分查找（处理重复） | 二分 + 遇重复退化 | O(log n) 平均, O(n) 最坏 | O(1) | ⭐ 必须写出 |

## 关键提示

1. **先回忆第 33 题**：无重复时，`nums[left] <= nums[mid]` 就能确定左半边有序。重复元素打破了这个判断——想想 `nums[left] == nums[mid]` 时你能确定什么？

2. **核心问题**：`[1,3,1,1,1]` vs `[1,1,1,3,1]`，left=0, mid=2 时 nums[left]==nums[mid]==1，但有序半边完全相反。

3. **解决手段只有一个**：遇到无法判断的情况，`left++`（或 `right--`）跳过重复元素。这不会漏掉答案，因为 `nums[left] == nums[mid]`，即使跳过 left，mid 处的值依然存在。

4. **Follow-up 回答**：重复元素会影响时间复杂度。最坏情况（如全部相同元素）退化为 O(n)，因为每次只能排除一个元素。

5. **搜索空间收缩过程示例**：
```
// nums = [2,5,6,0,0,1,2], target = 0
//
// Round1: [2,5,6,0,0,1,2]  left=0, right=6, mid=3
//                    ↑
//          nums[mid]=0 == target → 找到! 返回 true
//
// nums = [2,5,6,0,0,1,2], target = 3
//
// Round1: [2,5,6,0,0,1,2]  left=0, right=6, mid=3
//                    ↑
//          nums[mid]=0, nums[left]=2 <= nums[mid]=0? No
//          → 右半边 [0,0,1,2] 有序, target=3 不在 [0,2] 范围内
//          → right = mid-1 = 2
//
// Round2: [2,5,6]  left=0, right=2, mid=1
//             ↑
//          nums[mid]=5, nums[left]=2 <= 5? Yes
//          → 左半边 [2,5] 有序, target=3 在 [2,5] 范围内
//          → right = mid-1 = 0
//
// Round3: [2]  left=0, right=0, mid=0
//          ↑
//          nums[mid]=2 != 3
//          left=1 > right=0 → 退出, 返回 false
```

## 解法详解

### 解法1: 暴力遍历 — O(n) / O(1)

**思考过程**：最直接的做法，遍历数组看 target 是否存在。作为 baseline 理解题意。

```cpp
class Solution {
public:
    bool search(vector<int>& nums, int target) {
        for (int num : nums) {
            if (num == target) return true;
        }
        return false;
    }
};
```

**关键点**：这个解法虽然简单，但它是最坏情况下的时间复杂度上界。后面的二分解法最坏也是 O(n)，所以这个暴力解法并非"完全被淘汰"。

---

### 解法2: 二分查找（处理重复元素） — O(log n) 平均 / O(1) ⭐ 面试首选

**从第 33 题出发**：第 33 题的核心逻辑是——判断 mid 在旋转点的哪一侧，从而确定哪半边有序。本题在此基础上，多加一个处理：**当 `nums[left] == nums[mid] == nums[right]` 时，无法判断哪边有序，收缩两端边界。**

**和第 33 题的代码差异**：仅多了一个 `if` 分支处理重复元素的情况。

```cpp
class Solution {
public:
    bool search(vector<int>& nums, int target) {
        int left = 0, right = nums.size() - 1;
        
        while (left <= right) {
            int mid = left + (right - left) / 2;
            
            if (nums[mid] == target) return true;
            
            // ★ 这是和第33题唯一的核心区别 ★
            // 无法判断哪边有序，只能线性收缩
            if (nums[left] == nums[mid] && nums[mid] == nums[right]) {
                left++;
                right--;
            }
            // 左半边有序
            else if (nums[left] <= nums[mid]) {
                // target 在左半边有序区间内
                if (nums[left] <= target && target < nums[mid]) {
                    right = mid - 1;
                } else {
                    left = mid + 1;
                }
            }
            // 右半边有序
            else {
                // target 在右半边有序区间内
                if (nums[mid] < target && target <= nums[right]) {
                    left = mid + 1;
                } else {
                    right = mid - 1;
                }
            }
        }
        
        return false;
    }
};
```

**关键点**：
- `nums[left] <= nums[mid]` 用 `<=` 而不是 `<`，因为当 left==mid 时（只有两个元素），左半边也是"有序的"（只有一个元素）。
- 处理重复的分支必须放在判断有序之前，否则 `nums[left] <= nums[mid]` 在有重复时可能误判。

## 解法对比

| | 暴力遍历 | 二分查找 |
|---|---|---|
| 时间 | O(n) | O(log n) 平均，O(n) 最坏 |
| 空间 | O(1) | O(1) |
| 最坏情况 | 稳定 O(n) | 全部重复时退化到 O(n) |
| 面试价值 | 仅展示理解 | 展示二分功底 |

**什么时候选哪个？** 面试中必须写二分。暴力法虽然最坏情况一样，但二分法在大部分输入下远快于暴力。面试官考的是你能否处理"重复元素导致二分失效"的 edge case。

## 易错点

1. **✗ 只写 `left++` 不写 `right--`**：
   ```cpp
   // ✗ 只收缩一边，虽然也能 AC 但效率更差
   if (nums[left] == nums[mid]) { left++; }
   ```
   ```cpp
   // ✓ 两边同时收缩，效率更高
   if (nums[left] == nums[mid] && nums[mid] == nums[right]) {
       left++;
       right--;
   }
   ```
   只写 `left++` 也是正确的（不会漏解），但同时收缩两端能更快地跳过无用重复元素。

2. **✗ 忘记先判断 `nums[mid] == target`**：
   ```cpp
   // ✗ 先判断有序再处理重复，可能在收缩边界时跳过 target
   if (nums[left] == nums[mid] && nums[mid] == nums[right]) {
       left++; right--;  // 万一 nums[left] 就是 target 呢？
   }
   ```
   ```cpp
   // ✓ 先检查 mid 是否就是 target，确保不会漏掉
   if (nums[mid] == target) return true;
   // 然后再收缩——因为 nums[mid] != target，
   // 而 nums[left] == nums[mid]，所以 nums[left] 也不是 target，跳过安全
   ```

3. **✗ 判断有序时用 `<` 而不是 `<=`**：
   ```cpp
   // ✗ 当 left == mid 时（区间只有两个元素），这会误判
   if (nums[left] < nums[mid])
   ```
   ```cpp
   // ✓ 用 <= 覆盖 left == mid 的情况
   if (nums[left] <= nums[mid])
   ```

## 面试追问

**Q1: 和第 33 题相比，代码上改了什么？为什么？**
> 仅多了一个 `if (nums[left] == nums[mid] && nums[mid] == nums[right])` 的分支。因为有重复元素时，这个条件下无法判断 mid 在旋转点的左侧还是右侧，只能退化为线性收缩。

**Q2: 最坏时间复杂度是多少？什么输入下会达到？**
> O(n)。当数组全为相同元素且 target 不存在时，例如 `nums = [1,1,1,1,1]`, `target = 2`。每次只能 `left++; right--`，需要 n/2 次才能结束。

**Q3: 能否避免最坏 O(n)？**
> 不能。这是这道题的本质限制。考虑数组 `[1,1,...,1,2,1,...,1,1]`，2 可能在任意位置，在知道 2 的位置之前，你无法排除任何一个 1 不是 2。信息论上，必须检查每个元素。

**Q4: 如果只需要 `left++` 不需要 `right--`，为什么还要写 `right--`？**
> 正确性上只 `left++` 就够了。但 `right--` 是一个对称的优化：既然 `nums[right] == nums[mid] != target`，right 指向的元素也不是答案，跳过它能更快收缩搜索区间。

## 相关题型

- **33. 搜索旋转排序数组** — 本题的"无重复"版本。本题在 33 题基础上**只加了一个 if 分支处理重复**。先做 33 再做 81 是标准学习路径。
- **153. 寻找旋转排序数组中的最小值** — 不找 target 而是找旋转点（最小值），同样用二分。
- **154. 寻找旋转排序数组中的最小值 II** — 153 的"含重复"版本，处理重复的手法和本题完全相同（遇到相等就 `right--`）。
- **34. 在排序数组中查找元素的第一个和最后一个位置** — 练习二分查找边界的经典题。