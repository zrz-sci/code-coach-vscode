# 493. Reverse Pairs - 翻转对

## 核心思路
统计满足 i < j 且 nums[i] > 2 * nums[j] 的 (i,j) 对数。暴力 O(n^2) 超时，利用归并排序在合并时统计跨越左右的翻转对，或用 BIT/线段树维护已遍历元素的信息。

## 思维链
1. 暴力枚举所有 (i,j) 对 -> O(n^2) 超时
2. 联想到"逆序对"经典问题 -> 归并排序
3. 归并排序合并时，左右两半已排序，可用双指针高效统计跨越的翻转对
4. 注意 2 * nums[j] 可能溢出 int -> 用 long long
5. 统计和归并是两个独立步骤，先统计再归并

## 解法概览表

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| ⭐ 归并排序 | O(n log n) | O(n) | 经典分治，最常考 |
| BIT (树状数组) | O(n log n) | O(n) | 离散化 + BIT |

## 关键提示
1. 归并排序中"统计"和"合并"是分开的两步
2. `2 * nums[j]` 会溢出 int，必须转 long long
3. 统计时左右两半各自有序，用双指针 O(n) 完成
4. BIT 方法需要先离散化，将值域压缩到 [1, 2n]

## 解法详解

### 解法一：归并排序 ⭐
**思路**：分治法。递归排序左右两半后，利用两半有序的性质用双指针统计跨越的翻转对，再归并。

```cpp
class Solution {
public:
    int reversePairs(vector<int>& nums) {
        return mergeSort(nums, 0, nums.size() - 1);
    }
    
    int mergeSort(vector<int>& nums, int left, int right) {
        if (left >= right) return 0;
        int mid = left + (right - left) / 2;
        int count = mergeSort(nums, left, mid) + mergeSort(nums, mid + 1, right);
        
        // 统计跨越左右的翻转对
        int j = mid + 1;
        for (int i = left; i <= mid; i++) {
            while (j <= right && (long long)nums[i] > 2LL * nums[j]) {
                j++;
            }
            count += (j - mid - 1);
        }
        
        // 归并排序
        vector<int> temp(right - left + 1);
        int i = left, k = 0;
        j = mid + 1;
        while (i <= mid && j <= right) {
            if (nums[i] <= nums[j]) temp[k++] = nums[i++];
            else temp[k++] = nums[j++];
        }
        while (i <= mid) temp[k++] = nums[i++];
        while (j <= right) temp[k++] = nums[j++];
        copy(temp.begin(), temp.end(), nums.begin() + left);
        
        return count;
    }
};
```

### 解法二：树状数组 (BIT)
**思路**：从右到左遍历，对每个 nums[i] 查询 BIT 中有多少个值 < nums[i]/2（离散化后）。

```cpp
class Solution {
public:
    int reversePairs(vector<int>& nums) {
        vector<long long> sorted_vals;
        for (int x : nums) {
            sorted_vals.push_back(x);
            sorted_vals.push_back(2LL * x + 1);
        }
        sort(sorted_vals.begin(), sorted_vals.end());
        sorted_vals.erase(unique(sorted_vals.begin(), sorted_vals.end()), sorted_vals.end());
        
        int n = sorted_vals.size();
        vector<int> bit(n + 1, 0);
        int count = 0;
        
        for (int i = 0; i < nums.size(); i++) {
            int idx = lower_bound(sorted_vals.begin(), sorted_vals.end(), 2LL * nums[i] + 1) - sorted_vals.begin() + 1;
            count += query(bit, n) - query(bit, idx);
            
            idx = lower_bound(sorted_vals.begin(), sorted_vals.end(), (long long)nums[i]) - sorted_vals.begin() + 1;
            update(bit, idx, n);
        }
        return count;
    }
    
    void update(vector<int>& bit, int i, int n) {
        for (; i <= n; i += i & (-i)) bit[i]++;
    }
    
    int query(vector<int>& bit, int i) {
        int s = 0;
        for (; i > 0; i -= i & (-i)) s += bit[i];
        return s;
    }
};
```

## 易错点
- ✗ 直接写 `nums[i] > 2 * nums[j]`，int 溢出
- ✓ 使用 `(long long)nums[i] > 2LL * nums[j]`

- ✗ 统计和归并混在一起，逻辑混乱
- ✓ 先用双指针统计翻转对，再做归并排序

- ✗ 归并排序的 base case 忘记处理 left >= right
- ✓ left >= right 时直接返回 0

## 面试追问

**Q1: 和经典"逆序对"(LC 315) 有什么区别？**
> 经典逆序对条件是 nums[i] > nums[j]，本题是 nums[i] > 2*nums[j]。统计步骤不同，但框架相同。经典逆序对可以在归并过程中同时统计，本题需要分开。

**Q2: 为什么统计和归并要分两步？**
> 因为翻转对条件 (>2x) 和排序顺序 (>) 不同。如果条件一致（如经典逆序对），可以合并；条件不同时必须分开。

**Q3: 能否用平衡 BST 或 CDQ 分治来做？**
> 可以。平衡 BST（如 std::multiset + order statistics）可以在线维护。CDQ 分治是归并排序的推广，适用于更复杂的偏序问题。

## 相关题型
- [LeetCode 315] Count of Smaller Numbers After Self（归并/BIT）
- [LeetCode 327] Count of Range Sum（归并排序变体）
- [LeetCode 51] 数组中的逆序对（剑指 Offer，经典归并）
