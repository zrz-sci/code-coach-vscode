# 561. Array Partition - 数组拆分

## 核心思路
排序后取所有偶数下标元素之和。将相邻的两个数配对，较小的那个（偶数下标）被保留，这样"浪费"的值最小。

## 思维链
1. min(a, b) 总是丢弃较大值 → 要让被丢弃的值尽可能小
2. 如何让丢弃最小？ → 让每对的差距尽可能小
3. 怎么实现差距最小？ → 排序后相邻元素配对
4. 结果是什么？ → 排序后取 index 0, 2, 4, ... 的元素之和

## 解法概览表

| 解法 | 时间 | 空间 | 特点 |
|------|------|------|------|
| ⭐ 排序 + 取偶数下标 | O(n log n) | O(1) | 简单直接 |
| 计数排序 | O(n + k) | O(k) | 值域有限时更快 |

## 关键提示
1. 排序后 `nums[0] <= nums[1] <= ... <= nums[2n-1]`
2. 配对 `(nums[0], nums[1]), (nums[2], nums[3]), ...`
3. 每对取 min = 偶数下标元素
4. 值域 [-10^4, 10^4]，可用计数排序优化

## 解法详解

### 解法1: 排序 + 偶数下标求和 ⭐
```cpp
class Solution {
public:
    int arrayPairSum(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        int sum = 0;
        for (int i = 0; i < nums.size(); i += 2) {
            sum += nums[i];
        }
        return sum;
    }
};
```

### 解法2: 计数排序
```cpp
class Solution {
public:
    int arrayPairSum(vector<int>& nums) {
        vector<int> cnt(20001, 0);  // [-10000, 10000] → [0, 20000]
        for (int x : nums) cnt[x + 10000]++;
        
        int sum = 0;
        bool pick = true;  // 交替取/跳
        for (int i = 0; i <= 20000; i++) {
            while (cnt[i] > 0) {
                if (pick) sum += i - 10000;
                pick = !pick;
                cnt[i]--;
            }
        }
        return sum;
    }
};
```

## 易错点
- ✗ 步长写成 1 → ✓ 步长为 2，只取偶数下标
- ✗ 计数排序偏移量算错 → ✓ 值域 [-10^4, 10^4]，偏移 10000
- ✗ 以为需要复杂的配对算法 → ✓ 排序即是最优配对策略

## 面试追问

**Q1: 为什么排序后相邻配对是最优的？**
反证法：如果 a < b < c < d，配对 (a,c)(b,d) 得 a+b，而 (a,b)(c,d) 得 a+c > a+b。所以相邻配对让保留值更大。

**Q2: 值域很大时（如 [-10^9, 10^9]）怎么办？**
计数排序不可行，只能用比较排序 O(n log n)。

**Q3: 如果要求 max(ai, bi) 的总和最小呢？**
同样排序后相邻配对，取奇数下标之和。

## 相关题型
- [455. Assign Cookies](https://leetcode.com/problems/assign-cookies/) - 贪心配对
- [881. Boats to Save People](https://leetcode.com/problems/boats-to-save-people/) - 排序+双指针配对
