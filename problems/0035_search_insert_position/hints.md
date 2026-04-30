# 35. 搜索插入位置

## 核心思路

本题本质上是在**有序数组中找到第一个 ≥ target 的位置**（即 lower_bound）。如果 target 存在就返回它的索引，不存在就返回它应该插入的位置——这两种情况的答案恰好都是"第一个 ≥ target 的元素下标"。

## 思维链

1. **读完题第一反应**：数组有序，找一个值的位置 → 最朴素做法是从左到右扫一遍，找到第一个 ≥ target 的位置就返回。
2. **暴力解的瓶颈**：线性扫描是 O(n)，但题目明确要求 O(log n)。
3. **怎么突破**：O(log n) + 有序数组 = **二分查找**。每次比较中间元素，能排除一半的搜索空间。
4. **关键观察**：不管 target 在不在数组中，我们要找的都是同一个东西——**第一个 ≥ target 的下标**。这就是经典的 lower_bound 问题。
5. **边界思考**：如果 target 比所有元素都大，答案是 `nums.length`（插到末尾）；如果比所有元素都小，答案是 `0`（插到开头）。二分模板能自然处理这些边界。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 线性扫描 | 从左到右找第一个 ≥ target 的位置 | O(n) | O(1) | 能说出即可 |
| 二分查找 (左闭右闭) | 在 [0, n-1] 上二分找 lower_bound | O(log n) | O(1) | ⭐ 必须写出 |
| 二分查找 (左闭右开) | 在 [0, n) 上二分找 lower_bound | O(log n) | O(1) | 加分项（理解多种模板） |
| STL lower_bound | 直接调用标准库 | O(log n) | O(1) | 能说出即可（展示工程意识） |

## 关键提示

- **提示1**：不要把"找到 target"和"找不到 target"分成两个问题来处理。想想它们的答案有什么共同点？
- **提示2**：二分结束后 `left` 的值是什么含义？它恰好指向哪里？
- **提示3**：当 `nums[mid] == target` 时，你应该返回还是继续收缩？如果继续收缩，往哪个方向收缩？
- **提示4**：搜索空间收缩的完整例子：
```
在 [1,3,5,6] 中找 target=2 (不存在，应返回1):
Round1: left=0, right=3, mid=1, nums[1]=3 ≥ 2 → right=mid-1=0
                              ↑
Round2: left=0, right=0, mid=0, nums[0]=1 < 2  → left=mid+1=1
                          ↑
left=1 > right=0 → 结束，返回 left=1 ✓

在 [1,3,5,6] 中找 target=5 (存在，应返回2):
Round1: left=0, right=3, mid=1, nums[1]=3 < 5  → left=mid+1=2
                              ↑
Round2: left=2, right=3, mid=2, nums[2]=5 ≥ 5  → right=mid-1=1
                              ↑
left=2 > right=1 → 结束，返回 left=2 ✓
```

## 解法详解

### 解法1: 线性扫描 — O(n) / O(1)

**思考过程**：最直觉的做法。从左到右扫描，第一个 ≥ target 的位置就是答案。如果所有元素都 < target，答案就是数组长度（插入末尾）。

```cpp
class Solution {
public:
    int searchInsert(vector<int>& nums, int target) {
        for (int i = 0; i < nums.size(); i++) {
            // 找到第一个 >= target 的位置
            if (nums[i] >= target) return i;
        }
        // 所有元素都 < target，插入末尾
        return nums.size();
    }
};
```

**关键点**：这个解法虽然简单正确，但时间复杂度是 O(n)，不满足题目 O(log n) 的要求。不过它帮我们明确了一件事——**本题就是找第一个 ≥ target 的下标**。

---

### 解法2: 二分查找（左闭右闭 `[left, right]`） — O(log n) / O(1) ⭐ 面试首选

**从解法1优化**：线性扫描的瓶颈是逐个检查。数组有序意味着如果 `nums[mid] < target`，mid 及其左边所有元素都不可能是答案，直接排除一半。

**核心逻辑**：维护搜索区间 `[left, right]`，循环不变量是"答案一定在 `[left, right]` 范围内（或 left 越界到 right+1）"。

```cpp
class Solution {
public:
    int searchInsert(vector<int>& nums, int target) {
        int left = 0, right = nums.size() - 1;
        
        while (left <= right) {   // 搜索区间 [left, right] 非空时继续
            int mid = left + (right - left) / 2;  // 防溢出写法
            
            if (nums[mid] < target) {
                // mid 及其左边都 < target，不可能是插入位置
                left = mid + 1;
            } else {
                // nums[mid] >= target，mid 可能是答案，但左边可能有更小的
                right = mid - 1;
            }
        }
        
        // 循环结束时 left = right + 1
        // left 恰好是第一个 >= target 的位置
        return left;
    }
};
```

**为什么返回 left？**
- 循环结束时 `left = right + 1`，意味着搜索区间 `[left, right]` 为空。
- 所有 < target 的元素都在 left 左边（因为它们让 left 右移了）。
- 所有 ≥ target 的元素都在 right 右边（因为它们让 right 左移了）。
- 所以 left 就是"第一个 ≥ target 的位置"。

---

### 解法3: 二分查找（左闭右开 `[left, right)`） — O(log n) / O(1)

**不同模板，同样的思想**：搜索区间变成 `[left, right)`，循环条件和边界移动都相应调整。

```cpp
class Solution {
public:
    int searchInsert(vector<int>& nums, int target) {
        int left = 0, right = nums.size();  // 注意 right 初始化为 n（开区间）
        
        while (left < right) {   // [left, right) 非空时继续（left < right 不是 <=）
            int mid = left + (right - left) / 2;
            
            if (nums[mid] < target) {
                left = mid + 1;     // mid 不可能是答案，排除
            } else {
                right = mid;        // 注意：right = mid 不是 mid-1（开区间）
            }
        }
        
        // left == right，搜索区间为空，left 就是答案
        return left;
    }
};
```

**与解法2的区别**：
- `right` 初始化为 `n` 而不是 `n-1`（因为开区间，right 本身不在搜索范围内）
- 循环条件 `<` 而不是 `<=`（`left == right` 时区间 `[left, right)` 已经为空）
- 收缩右边界时 `right = mid` 而不是 `mid - 1`（开区间不包含 right，所以 mid 已被排除）

---

### 解法4: STL lower_bound — O(log n) / O(1)

**工程实践**：C++ 标准库已经封装了这个操作。面试中可以提及表示你知道，但通常面试官会要求手写。

```cpp
class Solution {
public:
    int searchInsert(vector<int>& nums, int target) {
        // lower_bound 返回第一个 >= target 的迭代器
        return lower_bound(nums.begin(), nums.end(), target) - nums.begin();
    }
};
```

## 解法对比

| | 线性扫描 | 二分(左闭右闭) | 二分(左闭右开) | STL |
|---|---|---|---|---|
| 时间 | O(n) | O(log n) | O(log n) | O(log n) |
| 空间 | O(1) | O(1) | O(1) | O(1) |
| 适用场景 | 理解题意 | **面试标准答案** | 与 STL 风格一致 | 工程场景 |
| 关键差异 | — | `right=n-1`, `<=`, `mid-1` | `right=n`, `<`, `mid` | 一行搞定 |

**面试建议**：选一种二分模板练熟并坚持用同一种。左闭右闭模板更直觉（搜索区间两端都包含），左闭右开模板与 STL 的 `lower_bound` 行为一致。

## 易错点

1. **✗ `mid = (left + right) / 2`** → **✓ `mid = left + (right - left) / 2`**
   - 当 left 和 right 都很大时，`left + right` 可能整数溢出。虽然本题数据量小不会出问题，但好习惯要养成。

2. **✗ 左闭右闭模板中 `right = mid` 而不是 `right = mid - 1`**
   - 在 `left <= right` 的模板中，如果写 `right = mid`，当 `left == right == mid` 时会死循环！因为 mid 永远不会被排除。

3. **✗ 左闭右开模板中 `right = mid - 1` 而不是 `right = mid`**
   - 在 `left < right` 的模板中，right 本身就不在搜索范围内。如果写 `right = mid - 1`，会漏掉 mid 这个候选答案。

4. **✗ 把 `nums[mid] == target` 单独判断直接返回**
   - 写成 `if (nums[mid] == target) return mid;` 虽然对本题也能 AC（因为无重复元素），但这**不是 lower_bound 的正确写法**。如果数组有重复元素，这样写就找不到"第一个"≥ target 的位置了。养成找边界的习惯。

5. **✗ 忘记处理 target 比所有元素都大的情况**
   - 二分结束后 left 可能等于 `nums.size()`，这是正确的（表示插入末尾）。不要加不必要的特判导致逻辑混乱。

## 面试追问

**Q1: 线性扫描能解吗？瓶颈在哪？**
→ 能，O(n) 遍历找第一个 ≥ target 的位置。瓶颈是没有利用数组有序的性质，每个元素都要检查。

**Q2: 二分查找结束后为什么 left 就是答案？能否证明？**
→ 循环不变量：所有使 left 右移的元素都 < target（在 left 左边），所有使 right 左移的元素都 ≥ target（在 right 右边）。循环结束时 left = right + 1，所以 left 恰好是分界点。

**Q3: 如果数组有重复元素，要找第一个 == target 的位置（找不到返回 -1），代码怎么改？**
→ 二分部分不变（仍然找 lower_bound），最后加一步检查：`if (left < n && nums[left] == target) return left; else return -1;`

**Q4: 你了解 `lower_bound` 和 `upper_bound` 的区别吗？**
→ `lower_bound` 找第一个 ≥ target 的位置，`upper_bound` 找第一个 > target 的位置。区别仅在于 `==` 时的处理：lower_bound 把 `==` 归到"≥"一侧继续收缩右边界，upper_bound 把 `==` 归到"≤"一侧继续收缩左边界。

## 相关题型

- **704. 二分查找** — 最基础的二分，找精确值。本题是它的变体：找不到时不返回 -1 而是返回插入位置。**复用完全相同的二分框架，区别在于 704 找到即返回，35 要找边界位置。**
- **34. 在排序数组中查找元素的第一个和最后一个位置** — 本题找 lower_bound，34 题同时需要 lower_bound 和 upper_bound。**直接复用本题的二分模板，调用两次：一次找 ≥ target（左边界），一次找 > target 再减 1（右边界）。**
- **69. x 的平方根** — 二分搜索的搜索空间不是数组索引，而是答案范围 [0, x]。**复用本题"找边界"的思想：找最大的 k 使得 k² ≤ x。**
- **278. 第一个错误的版本** — 典型的二分找左边界，**模板和本题的左闭右开版本几乎一模一样。**