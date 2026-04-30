# 896. 单调数列

## 核心思路

本质上是一个**线性扫描判断问题**：遍历数组一次，检查是否全程满足单调递增（`nums[i] <= nums[i+1]`）或全程满足单调递减（`nums[i] >= nums[i+1]`）。只要两者之一成立，就返回 true。

## 思维链

1. **读完题第一反应**：单调递增意味着每一步都不减，单调递减意味着每一步都不增。可以分别检查是否单调递增、是否单调递减，任一为 true 就返回 true。

2. **两次遍历方案**：第一次遍历检查是否全部 `nums[i] <= nums[i+1]`（单调递增），第二次检查是否全部 `nums[i] >= nums[i+1]`（单调递减）。时间 O(n)，两次遍历。

3. **一次遍历方案**：可以在一次遍历中同时跟踪两个标志 `isInc` 和 `isDec`。遍历相邻元素对，如果发现 `nums[i] > nums[i+1]` 则 `isInc = false`，如果发现 `nums[i] < nums[i+1]` 则 `isDec = false`。最后返回 `isInc || isDec`。

4. **进一步思考**：也可以只检查第一个"方向信号"——找到第一对不相等的相邻元素确定方向，然后验证后续是否一致。但这比同时跟踪两个标志的方法更复杂、更容易出错。

5. **边界情况**：全相等的数组（如 [3,3,3]）既是单调递增又是单调递减，应返回 true。长度为 1 或 2 的数组也一定是单调的。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 双标志一次遍历 | 同时跟踪 isInc 和 isDec | O(n) | O(1) | ⭐ 面试首选 |
| 两次遍历 | 分别检查递增和递减 | O(n) | O(1) | 等价写法 |
| 差分符号法 | 检查相邻差的符号是否一致 | O(n) | O(1) | 等价写法 |

> n = nums.length

## 关键提示

1. **单调递增允许相等**：题目定义"单调递增"为 `nums[i] <= nums[i+1]`（非严格递增），包含了相等的情况。同样"单调递减"为 `nums[i] >= nums[i+1]`（非严格递减）。

2. **全相等数组是单调的**：`[3, 3, 3, 3]` 既满足单调递增又满足单调递减，应返回 true。初始化 `isInc = true, isDec = true` 并且只在发现违反时设为 false，就能自然处理这种情况。

3. **一次遍历的核心逻辑**：
   - 看到 `nums[i] > nums[i+1]`（下降）→ 不可能是单调递增 → `isInc = false`
   - 看到 `nums[i] < nums[i+1]`（上升）→ 不可能是单调递减 → `isDec = false`
   - 如果 `!isInc && !isDec` → 既有上升又有下降 → 提前返回 false

4. **提前退出优化**：一旦 `isInc` 和 `isDec` 都变成 false，就可以提前返回 false，无需遍历完整个数组。在最坏情况下不改善复杂度，但在平均情况下更快。

5. **注意不要用 sort 或 reverse**：有人可能想"排序后和原数组比较"或"反转后和原数组比较"，这样做时间 O(n log n) 或 O(n) 但空间 O(n)，不如直接一次遍历 O(n)/O(1)。

## 解法详解

### 解法1: 双标志一次遍历 — O(n) / O(1) ⭐ 面试首选

**思考过程**：同时维护两个布尔标志 isInc 和 isDec，初始都为 true。遍历相邻元素对，发现上升就标记"不可能递减"，发现下降就标记"不可能递增"。最终只要有一个标志仍为 true 就返回 true。

```cpp
class Solution {
public:
    bool isMonotonic(vector<int>& nums) {
        bool isInc = true, isDec = true;
        
        for (int i = 0; i + 1 < (int)nums.size(); i++) {
            if (nums[i] > nums[i + 1]) isInc = false;
            if (nums[i] < nums[i + 1]) isDec = false;
            // 提前退出: 既有上升又有下降
            if (!isInc && !isDec) return false;
        }
        
        return true;  // isInc || isDec 一定为 true
    }
};
```

**关键点**：初始化 `isInc = isDec = true`，只在发现违反时设为 false。提前退出可以避免不必要的遍历。

### 解法2: 两次遍历 — O(n) / O(1)

**更直觉的方法**：分别检查是否单调递增和是否单调递减，封装成辅助函数，两个判断取或。

```cpp
class Solution {
public:
    bool isMonotonic(vector<int>& nums) {
        return isIncreasing(nums) || isDecreasing(nums);
    }
    
private:
    bool isIncreasing(vector<int>& nums) {
        for (int i = 0; i + 1 < (int)nums.size(); i++) {
            if (nums[i] > nums[i + 1]) return false;
        }
        return true;
    }
    
    bool isDecreasing(vector<int>& nums) {
        for (int i = 0; i + 1 < (int)nums.size(); i++) {
            if (nums[i] < nums[i + 1]) return false;
        }
        return true;
    }
};
```

**关键点**：代码更清晰易读，但最坏情况下遍历两次（如果数组是单调递减的，isIncreasing 在第一个元素就返回 false，然后 isDecreasing 完整遍历一次）。

### 解法3: 差分符号法 — O(n) / O(1)

**不同角度**：计算所有相邻差 `nums[i+1] - nums[i]` 的符号，只要所有非零差的符号一致就是单调的。

```cpp
class Solution {
public:
    bool isMonotonic(vector<int>& nums) {
        int direction = 0;  // 0=未确定, 1=递增, -1=递减
        
        for (int i = 0; i + 1 < (int)nums.size(); i++) {
            int diff = nums[i + 1] - nums[i];
            if (diff == 0) continue;  // 相等不影响方向
            
            int sign = (diff > 0) ? 1 : -1;
            if (direction == 0) {
                direction = sign;  // 首次确定方向
            } else if (direction != sign) {
                return false;  // 方向不一致
            }
        }
        
        return true;
    }
};
```

**关键点**：跳过相等的相邻对，只看非零差分的方向是否一致。全相等数组 direction 始终为 0，最终返回 true。

## 解法对比

| | 双标志一次遍历 | 两次遍历 | 差分符号法 |
|--|--------------|---------|----------|
| 时间 | O(n) | O(n) | O(n) |
| 空间 | O(1) | O(1) | O(1) |
| 遍历次数 | 1 | 最多 2 | 1 |
| 代码简洁 | ⭐ 最简洁 | 最清晰 | 中等 |
| 面试建议 | 首选 | 备选 | 追问时给 |

**选择建议**：
- 面试首选双标志法：一次遍历，O(1) 空间，代码简短
- 如果面试官要求代码可读性，两次遍历法更好解释
- 差分符号法作为"另一种思路"在追问时给出

## 易错点

1. **使用严格不等号判断**
   - x `nums[i] < nums[i+1]` 作为递增条件 -> [1,2,2,3] 会被误判为非单调
   - o `nums[i] <= nums[i+1]` 才是非严格递增（题目要求）

2. **忘记处理全相等数组**
   - x 两个标志都没被设为 false，但返回条件写成 `isInc ^ isDec`（异或）-> [3,3,3] 返回 false
   - o 返回 `isInc || isDec`，全相等时两个都是 true，结果正确

3. **差分计算整数溢出**
   - x `nums[i+1] - nums[i]` 当 nums 值很大时可能溢出
   - o 题目约束 `|nums[i]| <= 10^5`，差值不超过 2*10^5，不会溢出 int。但在更大范围的变体中需要用 long long

4. **循环边界写错**
   - x `for (int i = 0; i < nums.size(); i++)` 然后访问 `nums[i+1]` -> 最后一个元素越界
   - o `for (int i = 0; i + 1 < (int)nums.size(); i++)`

5. **只检查了递增没检查递减**
   - x 只返回 isIncreasing() 的结果 -> [6,5,4,4] 返回 false
   - o 必须同时检查递增和递减

## 面试追问

**Q1: 能不能用 STL 函数一行解决？**
-> 可以。`return is_sorted(nums.begin(), nums.end()) || is_sorted(nums.rbegin(), nums.rend());` 但面试中最好手写展示理解。

**Q2: 如果要判断严格单调呢？**
-> 把 `<=` 改成 `<`，`>=` 改成 `>`。或者在双标志法中，`nums[i] >= nums[i+1]` 时设 isStrictInc = false。

**Q3: 如果数组非常大（百万级），你会做什么优化？**
-> 核心算法已经是 O(n) 了无法优化。但可以利用提前退出（一旦两个标志都为 false 就停止），在大多数非单调数组中能提前返回。另外可以考虑多线程分段检查。

**Q4: 如果是链表而不是数组呢？**
-> 方法完全一样，遍历链表时维护前一个值和两个标志。只是无法随机访问，必须顺序遍历。时间 O(n)，空间 O(1)。

## 相关题型

- **LeetCode 1232 缀点成线** — 同为遍历检查某种一致性（所有点共线 vs 所有差分同向），方法论类似，都是一次遍历 + 状态检查。
- **LeetCode 941 有效的山脉数组** — 同为数组形状判断题，但需要先上升再下降（单峰），比单调更复杂一步。可以用类似的扫描思路。
- **LeetCode 2210 统计数组中峰和谷的数量** — 需要判断数组中的局部极值点，是单调性判断的延伸。
- **LeetCode 665 非递减数列** — 判断能否通过修改最多一个元素使数组变成非递减的，是单调性判断的进阶版。
