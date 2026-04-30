# 795. 区间子数组个数 (Number of Subarrays with Bounded Maximum)

[Medium] | Array, Two Pointers

## 核心思路

计算最大值在 `[left, right]` 范围内的子数组个数。用减法思维：`count(right) - count(left - 1)`，其中 `count(x)` 是最大值 `<= x` 的子数组个数。`count(x)` 可以 O(n) 计算：维护连续的 `<= x` 的元素段长度，每个长度为 L 的段贡献 `L*(L+1)/2` 个子数组。

---

## 思维链

1. 直接数"最大值在 [left, right] 范围"比较难
2. 转换：`f(right) - f(left-1)`，其中 `f(x)` = 最大值 <= x 的子数组个数
3. `f(x)` 的计算：遍历数组，维护当前连续 `<= x` 的段长度 `cur`
   - `nums[i] <= x`: `cur++`，贡献 `cur` 个以 `nums[i]` 结尾的子数组
   - `nums[i] > x`: `cur = 0`，段被打断
4. 最终答案 = `f(right) - f(left - 1)`

---

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 推荐 |
|------|------|------|------|------|
| 解法一 | 减法 count(right) - count(left-1) | O(n) | O(1) | ⭐⭐⭐ 首选 |
| 解法二 | 双指针 - 一次遍历 | O(n) | O(1) | ⭐⭐⭐ 同样优秀 |

---

## 关键提示

### 提示 1 - 减法原理
`max in [left, right]` = `max <= right` 减去 `max <= left-1`。这将一个区间约束拆成两个单侧约束。

### 提示 2 - count(x) 的线性计算
遍历数组，维护连续 `<= x` 的长度 `cur`。每步 `cur` 就是以当前位置结尾、最大值 `<= x` 的子数组个数。

### 提示 3 - 双指针方法
维护两个位置：`j` = 最近一个 `> right` 的位置，`k` = 最近一个 `>= left` 且 `<= right` 的位置。以 `i` 结尾的有效子数组个数 = `k - j`（当 `k > j` 时）。

---

## 解法详解

### 解法一: 减法原理

```cpp
class Solution {
public:
    int numSubarrayBoundedMax(vector<int>& nums, int left, int right) {
        return countAtMost(nums, right) - countAtMost(nums, left - 1);
    }

    // 最大值 <= bound 的子数组个数
    int countAtMost(vector<int>& nums, int bound) {
        int count = 0, cur = 0;
        for (int x : nums) {
            if (x <= bound) {
                cur++;
                count += cur;
            } else {
                cur = 0;
            }
        }
        return count;
    }
};
```

**过程演示** (nums=[2,1,4,3], left=2, right=3):

```
countAtMost(3):     countAtMost(1):
  x=2: cur=1, cnt=1   x=2: cur=0, cnt=0
  x=1: cur=2, cnt=3   x=1: cur=1, cnt=1
  x=4: cur=0, cnt=3   x=4: cur=0, cnt=1
  x=3: cur=1, cnt=4   x=3: cur=0, cnt=1
  返回 4               返回 1

答案 = 4 - 1 = 3
验证: [2], [2,1], [3] 共 3 个
```

### 解法二: 双指针 - 一次遍历

```cpp
class Solution {
public:
    int numSubarrayBoundedMax(vector<int>& nums, int left, int right) {
        int n = nums.size();
        int count = 0;
        int j = -1; // 最近一个 > right 的位置
        int k = -1; // 最近一个在 [left, right] 的位置
        for (int i = 0; i < n; i++) {
            if (nums[i] > right) {
                j = i;
            }
            if (nums[i] >= left && nums[i] <= right) {
                k = i;
            }
            // 以 i 结尾的有效子数组: 左端从 j+1 到 k
            if (k > j) {
                count += k - j;
            }
        }
        return count;
    }
};
```

---

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ `countAtMost(right) - countAtMost(left)` | ✓ `countAtMost(right) - countAtMost(left - 1)` | left 本身应该包含在范围内 |
| ✗ `cur = 0; count += cur` 的顺序搞反 | ✓ 先更新 `cur` 再加入 `count` | `cur++` 必须在 `count += cur` 之前 |
| ✗ 双指针中忘记初始化 `j = -1, k = -1` | ✓ 初始化为 -1 表示还没遇到 | 初始化错误会导致首个元素计算错误 |

---

## 面试追问

**Q1: 为什么 `cur` 就是以当前位置结尾的子数组个数？**
→ `cur` 是连续 `<= bound` 的段长度。以位置 `i` 结尾、所有元素 `<= bound` 的子数组，左端点可以从 `i-cur+1` 到 `i`，共 `cur` 个。一旦遇到 `> bound` 的元素，段被打断，`cur` 归零。

**Q2: 双指针方法中 `k - j` 怎么理解？**
→ `j` 是最近的"太大"的位置（`> right`），子数组不能包含它。`k` 是最近的"刚好"的位置（在 `[left, right]`）。以 `i` 结尾的子数组，左端从 `j+1` 到 `k` 都保证最大值在范围内，共 `k - j` 个。

**Q3: 如果数组中有负数怎么办？**
→ 方法完全相同。`countAtMost` 和双指针都只比较元素值和边界，不依赖元素非负。

---

## 相关题型

| 题号 | 题目 | 关联 |
|------|------|------|
| 992 | Subarrays with K Different Integers | 减法技巧 atMost(k) - atMost(k-1) |
| 1248 | Count Number of Nice Subarrays | 减法技巧 |
| 713 | Subarray Product Less Than K | 滑动窗口计数 |
| 2444 | Count Subarrays With Fixed Bounds | 双指针 + 固定边界 |
