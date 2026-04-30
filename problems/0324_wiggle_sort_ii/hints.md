# 324. 摆动排序 II (Wiggle Sort II)

**难度**: Medium | **标签**: Greedy, Array, Divide and Conquer, Quickselect, Sorting

## 核心思路

要求 `nums[0] < nums[1] > nums[2] < nums[3] ...`（严格不等）。核心思路：**排序后将数组分为较小半和较大半，交替放置**。关键技巧是**逆序填充**以避免相等元素相邻。进阶做法用 nth_element + 三路划分实现 O(n) 时间 + O(1) 空间。

## 思维链

1. **排序分组**: 排序后分为较小半 S 和较大半 L
2. **交替放置**: 偶数位放 S 中元素，奇数位放 L 中元素
3. **逆序填充**: 从 S 和 L 的末尾开始取，避免中位数相等元素相邻
4. **进阶优化**: nth_element 找中位数 + 虚拟索引映射 + 三路划分

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐度 | 说明 |
|------|-----------|-----------|--------|------|
| 排序 + 逆序交替填充 | O(n log n) | O(n) | ⭐⭐⭐⭐⭐ | 直观可靠，面试推荐 |
| nth_element + 三路划分 | O(n) | O(1) | ⭐⭐⭐ | 进阶，代码复杂 |

## 关键提示

1. **严格不等**: 与 Wiggle Sort I 不同，这里要求严格 `<` 和 `>`
2. **逆序填充的必要性**: 正序填充时相等的中位数元素可能相邻
3. **分割点**: n 个元素，较小半取 (n+1)/2 个，较大半取 n/2 个
4. **虚拟索引映射**: `(1 + 2*i) % (n | 1)` 将线性索引映射到摆动位置

## 解法详解

### 解法一：排序 + 逆序交替填充（推荐）

```cpp
class Solution {
public:
    void wiggleSort(vector<int>& nums) {
        int n = nums.size();
        vector<int> sorted(nums);
        sort(sorted.begin(), sorted.end());
        
        // 较小半从 sorted[(n-1)/2] 开始逆序填入偶数位
        // 较大半从 sorted[n-1] 开始逆序填入奇数位
        int small = (n - 1) / 2;  // 较小半最后一个索引
        int large = n - 1;        // 较大半最后一个索引
        
        for (int i = 0; i < n; i++) {
            if (i % 2 == 0) {
                nums[i] = sorted[small--];
            } else {
                nums[i] = sorted[large--];
            }
        }
    }
};
```

### 解法二：nth_element + 虚拟索引三路划分（O(n) 时间, O(1) 空间）

```cpp
class Solution {
public:
    void wiggleSort(vector<int>& nums) {
        int n = nums.size();
        
        // 找中位数
        auto midIt = nums.begin() + n / 2;
        nth_element(nums.begin(), midIt, nums.end());
        int mid = *midIt;
        
        // 虚拟索引映射：将线性索引映射到摆动顺序
        // 映射 i -> (1 + 2*i) % (n | 1)
        // 效果：0->1, 1->3, 2->5, ..., 然后 ->0, ->2, ->4, ...
        auto mapIdx = [n](int i) { return (1 + 2 * i) % (n | 1); };
        
        // 三路划分（Dutch National Flag），在虚拟索引空间操作
        int lo = 0, hi = n - 1, cur = 0;
        while (cur <= hi) {
            if (nums[mapIdx(cur)] > mid) {
                swap(nums[mapIdx(cur)], nums[mapIdx(lo)]);
                lo++;
                cur++;
            } else if (nums[mapIdx(cur)] < mid) {
                swap(nums[mapIdx(cur)], nums[mapIdx(hi)]);
                hi--;
            } else {
                cur++;
            }
        }
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ 正序填充两半 | ✓ **逆序**填充两半 | 正序时中位数可能相邻导致 `=` 而非 `<` |
| ✗ 较小半取 n/2 个 | ✓ 较小半取 (n+1)/2 个 | 偶数位比奇数位多1（当n为奇数时）|
| ✗ Wiggle Sort I 的做法（非严格） | ✓ 必须排序分半 | 单次遍历交换无法保证严格不等 |
| ✗ 虚拟索引公式写错 | ✓ `(1 + 2*i) % (n | 1)` | `n | 1` 确保模数为奇数 |

## 面试追问

**Q1: 为什么需要逆序填充？**
> 例如 [1,2,2,2,3,3]，正序填：[1,3,2,3,2,2] 有 nums[4]=2, nums[5]=2 不满足。逆序填：[2,3,2,3,1,3] 正确。逆序使相等的中位数元素尽量远离。

**Q2: O(n) 时间 + O(1) 空间的做法如何理解虚拟索引？**
> 虚拟索引将数组看作两个交错子数组（奇数位和偶数位），三路划分在虚拟顺序下将 >mid 放前面（映射到奇数位），<mid 放后面（映射到偶数位），=mid 放中间。

**Q3: nth_element 的平均/最坏复杂度？**
> 平均 O(n)，最坏 O(n^2)。C++ STL 实现通常用 Introselect（结合快速选择和中位数的中位数），保证 O(n)。

## 相关题型

- [75. 颜色分类](https://leetcode.com/problems/sort-colors/) - 三路划分 (Dutch National Flag)
- [280. 摆动排序](https://leetcode.com/problems/wiggle-sort/) - 非严格版本，O(n) 贪心
- [215. 数组中的第K个最大元素](https://leetcode.com/problems/kth-largest-element-in-an-array/) - nth_element
- [347. 前 K 个高频元素](https://leetcode.com/problems/top-k-frequent-elements/) - 快速选择
