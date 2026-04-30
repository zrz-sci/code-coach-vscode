# 34. 在排序数组中查找元素的第一个和最后一个位置

## 核心思路

本质上是在**有序数组**中分别找到 target 的**左边界**（第一个等于 target 的位置）和**右边界**（最后一个等于 target 的位置）。核心难点不是"能不能找到 target"，而是"找到 target 后不能停，要继续收缩区间找边界"。

## 思维链

1. **读完题第一反应**：线性扫描，从左往右找第一个 target，从右往左找最后一个 target，O(n)。但题目要求 O(log n)。

2. **O(log n) + 有序数组 = 二分查找**：标准二分能找到"某一个"等于 target 的位置，但不一定是第一个或最后一个。

3. **瓶颈在哪？** 标准二分找到 target 就停了。比如 `[5,7,7,8,8,10]` 找 8，二分可能先命中索引 3 也可能命中索引 4，我们无法控制。

4. **突破方式：改变"找到 target 后的行为"**。找左边界时，即使 `nums[mid] == target`，也不返回，而是继续向左收缩（`right = mid`）；找右边界时，继续向右收缩（`left = mid + 1`）。这样二分结束时自然停在边界上。

5. **两次二分**：第一次找"第一个 >= target 的位置"（左边界），第二次找"第一个 > target 的位置 - 1"（右边界）。两次都是 O(log n)，总计 O(log n)。

6. **验证**：找到左边界后，检查该位置的值是否真的等于 target（处理 target 不存在的情况）。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 线性扫描 | 从两头分别扫描 | O(n) | O(1) | 能说出即可 |
| 两次二分（统一模板） | 分别找左边界和右边界 | O(log n) | O(1) | ⭐ 必须写出 |
| STL lower/upper_bound | 利用标准库 | O(log n) | O(1) | 加分项（展示 STL 功底） |

## 关键提示

1. **找左边界**：当 `nums[mid] == target` 时，答案可能是 mid，也可能在 mid 左边。所以不要 return，而是 `right = mid`，继续往左找。

2. **找右边界**：当 `nums[mid] == target` 时，答案可能是 mid，也可能在 mid 右边。所以 `left = mid + 1`，最终答案是 `left - 1`。

3. **二分搜索空间收缩过程**：
```
在 [5,7,7,8,8,10] 中找 target=8 的左边界:
Round1: [5,7,7,8,8,10]  L=0 R=5 mid=2  nums[2]=7 < 8  → L=3
                  ↑
Round2: [8,8,10]         L=3 R=5 mid=4  nums[4]=8 == 8 → R=4
                    ↑
Round3: [8,8]            L=3 R=4 mid=3  nums[3]=8 == 8 → R=3
                  ↑
L==R==3 → 左边界 = 3 ✓

在 [5,7,7,8,8,10] 中找 target=8 的右边界:
Round1: [5,7,7,8,8,10]  L=0 R=5 mid=2  nums[2]=7 < 8  → L=3
Round2: [8,8,10]         L=3 R=5 mid=4  nums[4]=8 == 8 → L=5
Round3: [10]             L=5 R=5 mid=5  nums[5]=10 > 8 → R=4
L>R → 右边界 = L-1 = 4 ✓
```

4. **target 不存在的情况**：找到"左边界"位置后，必须检查 `nums[left] == target`。如果不等，说明数组中没有 target。

5. **两种思路等价**：找右边界 = 找 "第一个 > target 的位置" 再减 1 = 找 "target+1 的左边界" 再减 1。

## 解法详解

### 解法1: 线性扫描 — O(n) / O(1)

**思考过程**: 最直觉的做法。从左扫到第一个 target，从右扫到最后一个 target。不满足题目 O(log n) 的要求，但可以作为验证正确性的基准。

```cpp
class Solution {
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        int first = -1, last = -1;
        for (int i = 0; i < nums.size(); i++) {
            if (nums[i] == target) {
                if (first == -1) first = i;  // 第一次碰到
                last = i;                      // 每次都更新最后位置
            }
        }
        return {first, last};
    }
};
```

**关键点**: 当所有元素都等于 target 时退化为 O(n)，这正是暴力解的瓶颈——无法跳过中间已知的部分。

---

### 解法2: 两次二分查找 — O(log n) / O(1) ⭐ 面试首选

**从解法1优化**: 线性扫描的瓶颈是逐个检查。有序数组上，二分查找每次排除一半搜索空间。关键是改造二分，让它不是"找到就停"，而是"找到后继续收缩到边界"。

**模板选择**: 使用 `left < right` 模板找左边界（区间收缩到一个点），用类似思路找右边界。

```cpp
class Solution {
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        if (nums.empty()) return {-1, -1};
        
        int first = findFirst(nums, target);
        if (first == -1) return {-1, -1};  // target不存在，直接返回
        int last = findLast(nums, target);
        
        return {first, last};
    }
    
private:
    // 找第一个等于target的位置
    int findFirst(vector<int>& nums, int target) {
        int left = 0, right = nums.size() - 1;
        while (left < right) {
            int mid = left + (right - left) / 2;
            if (nums[mid] < target) {
                left = mid + 1;   // mid一定不是答案，排除
            } else {
                right = mid;      // nums[mid] >= target，mid可能是答案，保留
            }
        }
        // left == right，检查这个位置是否真的是target
        return nums[left] == target ? left : -1;
    }
    
    // 找最后一个等于target的位置
    int findLast(vector<int>& nums, int target) {
        int left = 0, right = nums.size() - 1;
        while (left < right) {
            // 注意：向上取整，避免 left=mid 导致死循环
            int mid = left + (right - left + 1) / 2;
            if (nums[mid] > target) {
                right = mid - 1;  // mid一定不是答案，排除
            } else {
                left = mid;       // nums[mid] <= target，mid可能是答案，保留
            }
        }
        return nums[left] == target ? left : -1;
    }
};
```

**关键点**:
- `findFirst` 中 mid 向下取整（默认），因为收缩方向是 `right = mid`，不会死循环。
- `findLast` 中 mid **必须向上取整** `(right - left + 1) / 2`，因为收缩方向是 `left = mid`。如果向下取整，当 `left + 1 == right` 时 `mid == left`，`left = mid` 不变 → 死循环！

---

### 解法3: 统一用 lower_bound 思路 — O(log n) / O(1)

**另一个视角**: 只写一个函数 `lowerBound(nums, target)` 表示"第一个 >= target 的位置"。那么：
- 左边界 = `lowerBound(nums, target)`
- 右边界 = `lowerBound(nums, target + 1) - 1`

```cpp
class Solution {
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        int left = lowerBound(nums, target);
        // 检查target是否存在
        if (left == nums.size() || nums[left] != target) return {-1, -1};
        // 右边界 = 第一个 > target 的位置 - 1
        int right = lowerBound(nums, target + 1) - 1;
        return {left, right};
    }
    
private:
    // 返回第一个 >= target 的索引（如果都小于target，返回nums.size()）
    int lowerBound(vector<int>& nums, int target) {
        int lo = 0, hi = nums.size();  // 注意：hi = size，不是 size-1
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (nums[mid] < target) {
                lo = mid + 1;
            } else {
                hi = mid;
            }
        }
        return lo;
    }
};
```

**为什么这个更优雅**: 只需要一个二分函数，逻辑统一，不容易写错。`lower_bound` 是二分查找最核心的"原子操作"，STL 也是这样设计的。

---

### 解法4: 直接用 STL — O(log n) / O(1)

```cpp
class Solution {
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        auto left = lower_bound(nums.begin(), nums.end(), target);
        auto right = upper_bound(nums.begin(), nums.end(), target);
        if (left == right) return {-1, -1};  // target不存在
        return {(int)(left - nums.begin()), (int)(right - nums.begin()) - 1};
    }
};
```

**适用场景**: 面试中展示 STL 功底后，面试官通常会追问"手写一个 lower_bound"，所以解法2/3才是核心。

## 解法对比

| | 线性扫描 | 两次二分(独立函数) | 统一 lowerBound | STL |
|---|---|---|---|---|
| 时间 | O(n) | O(log n) | O(log n) | O(log n) |
| 空间 | O(1) | O(1) | O(1) | O(1) |
| 代码量 | 最少 | 中等 | 较少 | 最少 |
| 易错程度 | 低 | 中（取整方向） | 低 | 低 |
| 面试推荐 | 开场暖身 | ⭐ 首选 | ⭐ 进阶首选 | 知道就好 |

**推荐**: 面试中先说解法3（统一 lowerBound），代码短、不容易出错。如果面试官追问"找右边界能不能不用 target+1"，再给出解法2。

## 易错点

1. **`findLast` 中 mid 不向上取整导致死循环**
   - ✗ `int mid = left + (right - left) / 2;` 配合 `left = mid;`
   - ✓ `int mid = left + (right - left + 1) / 2;` 配合 `left = mid;`
   - 原因：当 `left + 1 == right` 时，向下取整 `mid == left`，`left = mid` 不变，永远退不出循环。

2. **`lowerBound` 的右边界初始值**
   - ✗ `int hi = nums.size() - 1;` → 当 target 大于所有元素时，无法返回 `nums.size()`
   - ✓ `int hi = nums.size();` → 搜索空间是 `[0, n)`，允许返回 n 表示"不存在"

3. **忘记检查 target 是否真的存在**
   - ✗ 直接返回二分结果
   - ✓ 找到左边界后检查 `nums[left] == target`
   - 原因：`lowerBound` 返回的是"第一个 >= target 的位置"，如果数组中没有 target，这个位置的值可能是大于 target 的其他数。

4. **空数组忘记特判**
   - ✗ 直接进入二分 → `nums[left]` 越界
   - ✓ 开头 `if (nums.empty()) return {-1, -1};`

## 面试追问

**Q1（基础理解）**: "如果不要求 O(log n)，你会怎么做？瓶颈在哪？"
→ 线性扫描 O(n)。瓶颈是：有序数组的信息完全没利用。有序 + O(log n) 自然想到二分。

**Q2（核心追问）**: "标准二分找到 target 就返回了，怎么改造成找左/右边界？"
→ 找到 target 不返回。找左边界时 `right = mid`（继续向左收缩），找右边界时 `left = mid`（继续向右收缩）。循环结束时区间收缩到一个点就是答案。

**Q3（优化/统一）**: "能不能只写一个二分函数就解决两个边界？"
→ 可以。写 `lowerBound(target)` 找"第一个 >= target 的位置"。左边界 = `lowerBound(target)`，右边界 = `lowerBound(target+1) - 1`。

**Q4（变体）**: "如果数组中有 10^9 个元素，存在磁盘上，内存只能放很少的数据，怎么办？"
→ 二分查找天然适合这种场景：每次只需要读取 mid 位置的一个元素，总共只需要 O(log n) 次磁盘读取。

## 相关题型

- **35. 搜索插入位置** — 就是本题 `lowerBound` 函数的直接应用，找"第一个 >= target 的位置"
- **33. 搜索旋转排序数组** — 同样是二分，但需要先判断 mid 在哪个有序段上，再决定搜索方向
- **278. 第一个错误的版本** — 本质是找左边界：第一个满足 `isBadVersion(mid) == true` 的位置
- **162. 寻找峰值** — 二分不一定要数组有序，只要能根据 mid 判断答案在左半还是右半即可
- **74/240. 搜索二维矩阵** — 在二维有序结构上复用二分思想