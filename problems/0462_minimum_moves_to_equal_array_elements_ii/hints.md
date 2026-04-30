# 462. Minimum Moves to Equal Array Elements II - 最小操作次数使数组元素相等 II

## 核心思路
所有元素向中位数靠拢，总移动次数最小。中位数是使绝对差之和最小的点。

## 思维链
1. 目标值选什么？-> 使 sum(|nums[i] - target|) 最小
2. 数学性质：中位数最小化绝对偏差之和（平均值最小化平方偏差）
3. 找中位数 -> 排序取中间，或 nth_element O(n)
4. 累加每个元素到中位数的绝对差

## 解法概览表

| 解法 | 时间 | 空间 | 特点 |
|------|------|------|------|
| ⭐ 排序 + 中位数 | O(n log n) | O(1) | 简洁易懂 |
| ⭐ nth_element 快速选择 | O(n) 均摊 | O(1) | 最优时间 |
| 排序 + 双指针 | O(n log n) | O(1) | 无需显式求中位数 |

## 关键提示
1. **中位数定理**：中位数是使绝对偏差和最小的点
2. 排序后 `nums[n/2]` 即为中位数（奇偶均可）
3. 双指针技巧：排序后 `ans += nums[right] - nums[left]`，无需知道中位数是谁

## 解法详解

### 解法一：排序 + 中位数
```cpp
class Solution {
public:
    int minMoves2(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        int median = nums[nums.size() / 2];
        int moves = 0;
        for (int num : nums) {
            moves += abs(num - median);
        }
        return moves;
    }
};
```

### 解法二：快速选择 + 中位数
```cpp
class Solution {
public:
    int minMoves2(vector<int>& nums) {
        int n = nums.size();
        nth_element(nums.begin(), nums.begin() + n / 2, nums.end());
        int median = nums[n / 2];
        int moves = 0;
        for (int num : nums) {
            moves += abs(num - median);
        }
        return moves;
    }
};
```

### 解法三：排序 + 双指针
```cpp
class Solution {
public:
    int minMoves2(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        int moves = 0;
        int left = 0, right = nums.size() - 1;
        while (left < right) {
            moves += nums[right] - nums[left];
            left++;
            right--;
        }
        return moves;
    }
};
```

## 易错点
- ✗ 选平均值作为目标 -> ✓ 必须选中位数（平均值最小化平方和，不是绝对值和）
- ✗ 整数溢出，result 用 int -> ✓ 本题保证答案在 32 位内，但计算过程注意
- ✗ 偶数长度数组认为需要选两个中位数之一 -> ✓ 选任一中位数结果相同

## 面试追问

**Q1: 为什么中位数是最优的，而不是平均值？**
> 数学证明：绝对值函数 |x-a| 的和在 a=中位数 时取极值。直觉上，目标从中位数偏移一位，一侧减少一步但另一侧增加更多步（元素更多的那侧）。

**Q2: 如果每次操作可以增减任意值（不限于1），最小操作次数是多少？**
> 那就变成了"使所有元素相等的最少操作次数"，答案是 n - 最多相同元素个数（不同的元素各操作一次）。

**Q3: nth_element 的时间复杂度和最坏情况？**
> 均摊 O(n)，最坏 O(n^2)（类似快排）。C++ STL 实现通常使用 Introselect 保证 O(n)。

## 相关题型
- [453. Minimum Moves to Equal Array Elements](https://leetcode.com/problems/minimum-moves-to-equal-array-elements/) - 增减规则不同
- [296. Best Meeting Point](https://leetcode.com/problems/best-meeting-point/) - 二维中位数
- [2033. Minimum Operations to Make a Uni-Value Grid](https://leetcode.com/problems/minimum-operations-to-make-a-uni-value-grid/) - 步长限制版
