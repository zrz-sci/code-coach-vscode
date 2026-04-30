# 665. 非递减数列 (Non-decreasing Array)

[Medium] | Array

## 核心思路

遍历数组找到 `nums[i] > nums[i+1]` 的"违规点"。最多允许一次违规。关键在于：发现违规后，应该修改 `nums[i]` 还是 `nums[i+1]`？需要看 `nums[i-1]` 和 `nums[i+1]` 的大小关系来决定。

---

## 思维链

1. 如果没有任何 `nums[i] > nums[i+1]`，数组本身就是非递减的，直接返回 true
2. 如果出现两次以上违规，返回 false
3. 只有一次违规时，需要判断能否通过修改一个元素修复：
   - 如果 `i == 0` 或 `nums[i-1] <= nums[i+1]`：可以把 `nums[i]` 降低到 `nums[i+1]`
   - 否则：可以把 `nums[i+1]` 升高到 `nums[i]`，但需要检查后续是否仍满足

---

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 推荐 |
|------|------|------|------|------|
| 解法一 | 贪心 - 计数+模拟修改 | O(n) | O(1) | ⭐⭐⭐ 首选 |
| 解法二 | 贪心 - 仅计数+条件判断 | O(n) | O(1) | ⭐⭐ 更简洁 |

---

## 关键提示

### 提示 1 - 违规点的定义
找到第一个 `i` 使得 `nums[i] > nums[i+1]`。如果不存在，直接返回 true。

### 提示 2 - 修改哪个元素
发现违规 `nums[i] > nums[i+1]` 时：
- 优先尝试降低 `nums[i]`（改为 `nums[i+1]`），这样不影响后面
- 但如果 `nums[i-1] > nums[i+1]`，降低 `nums[i]` 会导致 `nums[i-1] > nums[i]`，此时只能升高 `nums[i+1]`（改为 `nums[i]`）

### 提示 3 - 贪心策略
修改后继续检查剩余部分是否非递减。如果再次出现违规，返回 false。

---

## 解法详解

### 解法一: 贪心 - 模拟修改

遇到违规时实际修改数组值，然后继续检查：

```cpp
class Solution {
public:
    bool checkPossibility(vector<int>& nums) {
        int count = 0;
        for (int i = 0; i + 1 < nums.size(); i++) {
            if (nums[i] > nums[i + 1]) {
                count++;
                if (count > 1) return false;
                // 决定修改哪个
                if (i > 0 && nums[i - 1] > nums[i + 1]) {
                    nums[i + 1] = nums[i]; // 升高 nums[i+1]
                } else {
                    nums[i] = nums[i + 1]; // 降低 nums[i]
                }
            }
        }
        return true;
    }
};
```

### 解法二: 贪心 - 仅计数判断

不修改数组，找到第一个违规点后判断两种修改是否可行：

```cpp
class Solution {
public:
    bool checkPossibility(vector<int>& nums) {
        int n = nums.size();
        int pos = -1;
        for (int i = 0; i + 1 < n; i++) {
            if (nums[i] > nums[i + 1]) {
                if (pos != -1) return false; // 第二次违规
                pos = i;
            }
        }
        // 没有违规，或者违规在首尾，或者两种修改之一可行
        return pos == -1 || pos == 0 || pos == n - 2
            || nums[pos - 1] <= nums[pos + 1]
            || nums[pos] <= nums[pos + 2];
    }
};
```

---

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ 只统计违规次数 <= 1 就返回 true | ✓ 还需判断修改后是否真的能修复 | `[3,4,2,3]` 有1次违规但无法修复 |
| ✗ 总是降低 `nums[i]` | ✓ 当 `nums[i-1] > nums[i+1]` 时应升高 `nums[i+1]` | `[1,4,2,3]` 降低4→2可以；`[3,4,2,5]` 降低4→2不行因为3>2 |
| ✗ 忘记处理 `i==0` 的边界 | ✓ `i==0` 时没有 `nums[i-1]`，直接降低 `nums[i]` | 首元素违规总是可以修复 |

---

## 面试追问

**Q1: 为什么贪心策略是正确的？**
→ 当遇到违规 `nums[i] > nums[i+1]` 时，我们优先尝试降低 `nums[i]`，因为这样对后续影响最小（让当前值尽量小，给后面留更多空间）。只有当降低会破坏与 `nums[i-1]` 的关系时，才升高 `nums[i+1]`。这就是贪心的核心 -- 每次做局部最优选择。

**Q2: 能否不修改原数组？**
→ 可以。找到第一个违规位置 `pos` 后，判断 `nums[pos-1] <= nums[pos+1]`（删除 `nums[pos]` 是否可行）或 `nums[pos] <= nums[pos+2]`（删除 `nums[pos+1]` 是否可行），同时处理边界情况。

**Q3: 如果改成"最多修改 k 个元素"，怎么做？**
→ 类似的贪心思路，维护违规计数器，每次违规都贪心修改。但 k>1 时贪心不一定最优，可能需要更复杂的策略（如动态规划求最长非递减子序列 LIS，答案为 n - LIS >= n - k）。

---

## 相关题型

| 题号 | 题目 | 关联 |
|------|------|------|
| 896 | Monotonic Array | 判断数组单调性 |
| 300 | Longest Increasing Subsequence | 最长递增子序列 |
| 376 | Wiggle Subsequence | 贪心处理数组序列 |
