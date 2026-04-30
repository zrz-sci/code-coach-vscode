# 496. Next Greater Element I - 下一个更大元素 I

## 核心思路
对 nums2 中每个元素预处理其"下一个更大元素"，用单调递减栈从右到左（或从左到右）扫描，结果存入哈希表，然后对 nums1 中每个元素查表。

## 思维链
1. 暴力：对 nums1 每个元素，在 nums2 中找位置再向右扫描 -> O(m*n)
2. 优化：预处理 nums2 中所有元素的下一个更大元素 -> 单调栈
3. 单调递减栈：遍历 nums2，栈中维护"待匹配"的元素
4. 当遇到比栈顶大的元素时，栈顶的 next greater 就是当前元素
5. 用 HashMap 存映射，O(1) 查询 nums1 的答案

## 解法概览表

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| ⭐ 单调栈 + 哈希 | O(m + n) | O(n) | 最优解，经典模板 |
| 暴力查找 | O(m * n) | O(1) | 简单直接 |

## 关键提示
1. 单调栈维护"还没找到 next greater 的元素"
2. 遍历 nums2 时，当前元素 > 栈顶 -> 栈顶找到答案
3. 所有元素唯一，可直接用值做 HashMap 的 key
4. 栈中剩余元素的 next greater 为 -1

## 解法详解

### 解法一：单调栈 + 哈希表 ⭐
**思路**：用单调递减栈遍历 nums2，预处理每个元素的 next greater，存入 map。

```cpp
class Solution {
public:
    vector<int> nextGreaterElement(vector<int>& nums1, vector<int>& nums2) {
        unordered_map<int, int> nextGreater;
        stack<int> stk;
        
        for (int num : nums2) {
            while (!stk.empty() && stk.top() < num) {
                nextGreater[stk.top()] = num;
                stk.pop();
            }
            stk.push(num);
        }
        
        vector<int> res;
        for (int num : nums1) {
            res.push_back(nextGreater.count(num) ? nextGreater[num] : -1);
        }
        return res;
    }
};
```

### 解法二：暴力查找
**思路**：对 nums1 中每个元素，在 nums2 中找到位置后向右扫描。

```cpp
class Solution {
public:
    vector<int> nextGreaterElement(vector<int>& nums1, vector<int>& nums2) {
        vector<int> res;
        for (int x : nums1) {
            int j = 0;
            while (nums2[j] != x) j++;
            j++;
            while (j < nums2.size() && nums2[j] <= x) j++;
            res.push_back(j < nums2.size() ? nums2[j] : -1);
        }
        return res;
    }
};
```

## 易错点
- ✗ 栈中存下标而非值（本题元素唯一，存值更方便）
- ✓ 存值直接做 HashMap key

- ✗ 忘记处理栈中剩余元素（它们的 next greater 是 -1）
- ✓ 不在 map 中的元素默认返回 -1

- ✗ 单调栈方向搞反（递增栈 vs 递减栈）
- ✓ 维护递减栈：遇到更大的就弹出并记录

## 面试追问

**Q1: 如果数组是循环的（Next Greater Element II, LC 503）怎么办？**
> 将数组"复制一份"（或 index % n 模拟），用同样的单调栈方法，遍历 2n 个元素。

**Q2: 如何找"下一个更小元素"？**
> 维护单调递增栈（而非递减），当遇到比栈顶小的元素时弹出记录。

**Q3: 单调栈的时间复杂度为什么是 O(n)？**
> 每个元素最多入栈一次、出栈一次，总操作数 <= 2n，所以均摊 O(n)。

## 相关题型
- [LeetCode 503] Next Greater Element II（循环数组单调栈）
- [LeetCode 739] Daily Temperatures（单调栈经典）
- [LeetCode 84] Largest Rectangle in Histogram（单调栈进阶）
- [LeetCode 456] 132 Pattern（单调栈变体）
