# 280. Wiggle Sort (摆动排序)

**难度**: Medium | **标签**: Greedy, Array, Sorting

## 核心思路

要求 `nums[0] <= nums[1] >= nums[2] <= nums[3] ...`，即奇数索引处的元素是局部最大值。**贪心法**: 遍历数组，当违反摆动条件时交换相邻元素即可，无需全局排序。

## 思维链

1. **朴素想法**: 先排序，然后交换相邻对 --> O(n log n)
2. **贪心观察**: 只需保证局部条件，不需要全局有序
3. **核心规则**:
   - 偶数索引 i: `nums[i] <= nums[i+1]`，若不满足则交换
   - 奇数索引 i: `nums[i] >= nums[i+1]`，若不满足则交换
4. **正确性证明**: 交换只影响相邻两个元素，修复当前不等式不会破坏前一个

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐 |
|------|-----------|-----------|------|
| 贪心单次遍历 | O(n) | O(1) | ⭐ |
| 排序后交换 | O(n log n) | O(1) | |

## 关键提示

```
贪心交换过程:

原数组: [3, 5, 2, 1, 6, 4]
规则:   <=  >=  <=  >=  <=

i=0 (偶): nums[0]=3 <= nums[1]=5? Yes ✓
  [3, 5, 2, 1, 6, 4]

i=1 (奇): nums[1]=5 >= nums[2]=2? Yes ✓
  [3, 5, 2, 1, 6, 4]

i=2 (偶): nums[2]=2 <= nums[3]=1? No ✗ → swap(2,1)
  [3, 5, 1, 2, 6, 4]

i=3 (奇): nums[3]=2 >= nums[4]=6? No ✗ → swap(2,6)
  [3, 5, 1, 6, 2, 4]

i=4 (偶): nums[4]=2 <= nums[5]=4? Yes ✓
  [3, 5, 1, 6, 2, 4]  ✓ 完成!

正确性证明 (偶数位 i 交换后):
  交换前: a >= b (违反 a <= b)
  交换后: b, a  → b <= a ✓
  且前一个条件 nums[i-1] >= nums[i]:
    原来 nums[i-1] >= a >= b
    交换后 nums[i-1] >= a, nums[i]=b <= a
    所以 nums[i-1] >= b ✓ (不会破坏前一个条件)
```

## 解法详解

### 解法一: 贪心单次遍历 (推荐)

```cpp
class Solution {
public:
    void wiggleSort(vector<int>& nums) {
        for (int i = 0; i < (int)nums.size() - 1; i++) {
            if (i % 2 == 0) {
                // 偶数位: 应该 nums[i] <= nums[i+1]
                if (nums[i] > nums[i + 1]) {
                    swap(nums[i], nums[i + 1]);
                }
            } else {
                // 奇数位: 应该 nums[i] >= nums[i+1]
                if (nums[i] < nums[i + 1]) {
                    swap(nums[i], nums[i + 1]);
                }
            }
        }
    }
};
```

### 解法二: 排序后交换相邻对

```cpp
class Solution {
public:
    void wiggleSort(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        // 从第2个元素开始，每两个一组交换
        for (int i = 1; i + 1 < (int)nums.size(); i += 2) {
            swap(nums[i], nums[i + 1]);
        }
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ `i < nums.size() - 1` (unsigned 减法) | ✓ `i < (int)nums.size() - 1` | size() 返回 unsigned，空数组时 -1 溢出 |
| ✗ 奇数位判断 `nums[i] > nums[i+1]` | ✓ `nums[i] < nums[i+1]` | 奇数位应该是 >=，不满足时才交换 |
| ✗ 认为需要全局排序 | ✓ 只需保证局部条件 | 贪心思想，局部最优即全局最优 |
| ✗ 担心交换破坏前面的条件 | ✓ 数学证明交换不会破坏 | 详见关键提示中的正确性证明 |

## 面试追问

**Q1: 为什么贪心交换不会破坏前面已满足的条件?**
--> 在偶数位 i 交换时: 已知 `nums[i-1] >= nums[i]`(前一个奇数位条件), 现在 `nums[i] > nums[i+1]`(违反), 交换后 `nums[i]` 变小，`nums[i-1] >= 原nums[i] > 新nums[i]`，前一条件仍成立。

**Q2: 与 324. Wiggle Sort II 的区别?**
--> 324 要求严格不等 (`<` 和 `>` 而非 `<=` 和 `>=`)，不能用简单交换，需要更复杂的算法（排序+穿插或三路划分）。

**Q3: 能否使用其他 O(n) 排序（如计数排序）优化解法二?**
--> 可以，但贪心法已经是 O(n) 且 O(1) 空间，更优。

## 相关题型

- [324. Wiggle Sort II](../0324_wiggle_sort_ii/) - 严格摆动排序 (Hard)
- [376. Wiggle Subsequence](../0376_wiggle_subsequence/) - 最长摆动子序列
- [75. Sort Colors](../0075_sort_colors/) - 三路划分
- [215. Kth Largest Element](../0215_kth_largest_element_in_an_array/) - 快速选择
