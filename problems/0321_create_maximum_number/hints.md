# 321. 拼接最大数 (Create Maximum Number)

**难度**: Hard | **标签**: Stack, Greedy, Array, Two Pointers, Monotonic Stack

## 核心思路

将问题分解为三个子问题：
1. **单数组取最大子序列**: 从一个数组中取 t 个数，保持相对顺序使组成的数最大（单调栈）
2. **枚举分配方案**: 从 nums1 取 i 个，从 nums2 取 k-i 个 (0 <= i <= k)
3. **合并两个序列**: 将两个最大子序列合并为一个最大序列（类似归并，用字典序比较决定取哪个）

## 思维链

1. **分治思想**: 总共取 k 个数，枚举从 nums1 取 i 个（0 <= i <= min(m,k)），从 nums2 取 k-i 个
2. **单调栈选最大**: 对每个数组，用单调递减栈选出长度为 t 的最大子序列
3. **贪心合并**: 两个子序列合并时，每步比较剩余部分的字典序，取较大者的当前元素
4. **全局取最大**: 枚举所有合法的 i，取合并结果中字典序最大的

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐度 | 说明 |
|------|-----------|-----------|--------|------|
| 单调栈 + 枚举 + 合并 | O(k * (m+n)) | O(m+n) | ⭐⭐⭐⭐⭐ | 经典三步分解 |

## 关键提示

1. **maxSubseq(nums, t)**: 单调栈 —— 栈中元素 + 剩余元素 >= t 时可以弹出较小元素
2. **merge 的陷阱**: 不能简单比较当前元素，要比较**剩余子序列的字典序**
3. **枚举范围**: i 的范围是 max(0, k-n) <= i <= min(k, m)
4. **最终比较**: 用 `>` 运算符比较 vector<int> 即字典序

## 解法详解

### 解法一：单调栈 + 枚举分配 + 归并（标准解法）

```cpp
class Solution {
public:
    vector<int> maxNumber(vector<int>& nums1, vector<int>& nums2, int k) {
        int m = nums1.size(), n = nums2.size();
        vector<int> best;
        
        // 枚举从 nums1 取 i 个, nums2 取 k-i 个
        for (int i = max(0, k - n); i <= min(k, m); i++) {
            vector<int> sub1 = maxSubseq(nums1, i);
            vector<int> sub2 = maxSubseq(nums2, k - i);
            vector<int> merged = merge(sub1, sub2);
            if (merged > best) best = merged;
        }
        
        return best;
    }
    
private:
    // 从 nums 中选 t 个数，保持顺序，使组成的数最大
    vector<int> maxSubseq(vector<int>& nums, int t) {
        int n = nums.size(), drop = n - t;
        vector<int> stk;
        for (int num : nums) {
            while (drop > 0 && !stk.empty() && stk.back() < num) {
                stk.pop_back();
                drop--;
            }
            stk.push_back(num);
        }
        stk.resize(t);
        return stk;
    }
    
    // 合并两个子序列，每步取字典序较大的一方
    vector<int> merge(vector<int>& a, vector<int>& b) {
        vector<int> result;
        int i = 0, j = 0;
        while (i < (int)a.size() || j < (int)b.size()) {
            // 比较剩余部分的字典序
            if (compare(a, i, b, j)) {
                result.push_back(a[i++]);
            } else {
                result.push_back(b[j++]);
            }
        }
        return result;
    }
    
    // 判断 a[i:] 是否字典序 >= b[j:]
    bool compare(vector<int>& a, int i, vector<int>& b, int j) {
        while (i < (int)a.size() && j < (int)b.size() && a[i] == b[j]) {
            i++; j++;
        }
        if (j == (int)b.size()) return true;
        if (i == (int)a.size()) return false;
        return a[i] > b[j];
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ merge 时只比较当前元素 | ✓ 比较剩余序列的字典序 | 当前元素相同时需看后续 |
| ✗ maxSubseq 的 drop 计算错误 | ✓ `drop = n - t` | drop 是可以丢弃的元素数 |
| ✗ 枚举 i 范围不对 | ✓ `max(0, k-n) <= i <= min(k, m)` | 需要保证两数组都能取出足够元素 |
| ✗ stk 没有 resize(t) | ✓ 最后裁剪到 t 个 | 可能多压入了元素 |
| ✗ compare 中一方耗尽返回错误 | ✓ b 耗尽返回 true，a 耗尽返回 false | 较长序列字典序更大 |

## 面试追问

**Q1: maxSubseq 的单调栈原理？**
> 维护一个"能保留的最大子序列"，当栈顶小于当前元素且还有丢弃配额时弹出栈顶。最终栈中前 t 个即答案。

**Q2: 为什么 merge 不能只比较当前元素？**
> 例如 [6,7] 和 [6,0,4]，两个 6 相同时必须看后续: [7] > [0,4]，所以先取第一个 6。否则结果不是最大。

**Q3: 时间复杂度的详细推导？**
> 枚举 O(k) 种分配，每种分配: maxSubseq O(m) + O(n)，merge O(k)，compare 每次最坏 O(k)。总计 O(k^2 * (m+n)/k) = O(k*(m+n))。

## 相关题型

- [402. 移掉 K 位数字](https://leetcode.com/problems/remove-k-digits/) - 单调栈选最值
- [316. 去除重复字母](https://leetcode.com/problems/remove-duplicate-letters/) - 单调栈
- [84. 柱状图中最大的矩形](https://leetcode.com/problems/largest-rectangle-in-histogram/) - 单调栈经典
- [88. 合并两个有序数组](https://leetcode.com/problems/merge-sorted-array/) - 归并思想
