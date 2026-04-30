# 1095. 山脉数组中查找目标值

## 核心思路

山脉数组 = 先升后降，本质是**两段有序数组拼接**。核心任务是：先找到峰顶（分割点），然后分别在升序段和降序段做二分查找。由于 `get()` 调用次数有限（≤100），必须全程使用二分。

## 思维链

1. **读完题第一反应**：遍历整个数组找 target？但数组不能直接访问，每次 `get()` 有成本，且调用上限 100 次。暴力遍历最多 10⁴ 次 `get()`，可能超限。

2. **暴力解的瓶颈**：线性扫描 O(n) 次 `get()` 调用，当 n=10⁴ 时远超 100 次限制。

3. **怎么突破？** 山脉数组有结构——先严格递增，再严格递减。两段各自有序，有序 → 二分查找 O(log n)。log₂(10⁴) ≈ 14，三次二分最多约 42 次调用，远小于 100。

4. **分三步走**：
   - **Step 1**：二分找峰顶 peak（比较 `arr[mid]` 和 `arr[mid+1]`）
   - **Step 2**：在升序段 `[0, peak]` 二分查找 target
   - **Step 3**：若 Step 2 没找到，在降序段 `[peak+1, n-1]` 二分查找 target

5. **为什么先搜升序段？** 题目要求返回**最小**下标。升序段的下标比降序段小（同一个值出现两次时，升序段的在前面），所以先搜升序段，找到就直接返回。

6. **降序段二分的区别**：降序数组里 `arr[mid] > target` 时应该向右搜（而不是普通的向左），判断方向要反过来。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力线性扫描 | 从左到右遍历找第一个等于 target 的 | O(n) | O(1) | 能说出即可 |
| 三次二分 | 找峰顶 + 升序二分 + 降序二分 | O(log n) | O(1) | ⭐ 必须写出 |

## 关键提示

1. **找峰顶的判断条件**：比较 `arr[mid]` 和 `arr[mid+1]`，如果 `arr[mid] < arr[mid+1]`，峰顶在右边；否则峰顶在左边（包括 mid 自己）。

2. **升序 vs 降序二分的唯一区别**：`arr[mid] < target` 时移动方向相同吗？升序是 `left = mid + 1`，降序是 `right = mid - 1`——恰好反过来。

3. **为什么找峰顶时用 `left < right` 而不是 `left <= right`？** 因为我们比较的是 `mid` 和 `mid+1`，如果 `left == right` 就没有 `mid+1` 了。循环结束时 `left == right` 就是峰顶。

4. **`get()` 调用次数估算**：找峰顶 ≈ 2×14 = 28 次（每轮要 get(mid) 和 get(mid+1)），升序二分 ≈ 14 次，降序二分 ≈ 14 次，总共 ≈ 56 次，远小于 100。可以优化：缓存 get 结果减少调用。

5. **搜索空间示意**：
```
  数组:  [1, 2, 3, 4, 5, 3, 1]
  索引:   0  1  2  3  4  5  6

         5
        / \
       4   3
      /     \
     3       1
    /
   2
  /
  1

  ↑ 升序段 [0,4]    ↑ 降序段 [4,6]
  先二分找到 peak=4
  target=3: 升序段找到 index=2 → 返回 2
```

## 解法详解

### 解法1: 暴力线性扫描 — O(n) / O(1)

**思考过程**：最朴素的方法，从左到右遍历，找到第一个等于 target 的就返回。但 `get()` 调用次数可能达到 n=10⁴，超过 100 次限制。**不能通过此题**，但帮助理解题意。

```cpp
/**
 * // This is the MountainArray's API interface.
 * // You should not implement it, or speculate about its implementation
 * class MountainArray {
 *   public:
 *     int get(int index);
 *     int length();
 * };
 */
class Solution {
public:
    int findInMountainArray(int target, MountainArray &mountainArr) {
        int n = mountainArr.length();
        for (int i = 0; i < n; i++) {
            if (mountainArr.get(i) == target) {
                return i;
            }
        }
        return -1;
    }
};
```

**关键点**：这个解法正确但会 TLE/WA（超过 100 次调用）。它展示了"为什么必须用二分"——线性扫描的 `get()` 调用次数是 O(n)。

---

### 解法2: 三次二分 — O(log n) / O(1) ⭐ 面试首选

**从解法1优化**：线性扫描的瓶颈是每个元素都要 `get()` 一次。山脉数组的两段分别有序，有序意味着可以二分。只需三次 O(log n) 的二分：找峰顶、升序段搜索、降序段搜索。

**三次二分的搜索空间收缩过程**：

```
Step 1: 找峰顶 (在 [1,2,3,4,5,3,1] 中)
  Round1: left=0, right=6, mid=3, arr[3]=4 < arr[4]=5 → left=4
  Round2: left=4, right=6, mid=5, arr[5]=3 > arr[6]=1 → right=5
  Round3: left=4, right=5, mid=4, arr[4]=5 > arr[5]=3 → right=4
  left==right==4 → peak=4

Step 2: 升序段 [0,4] 找 target=3
  Round1: left=0, right=4, mid=2, arr[2]=3 == target → 找到! 返回 2

Step 3: (不需要，Step 2 已找到)
```

```cpp
class Solution {
public:
    int findInMountainArray(int target, MountainArray &mountainArr) {
        int n = mountainArr.length();
        
        // === Step 1: 二分找峰顶 ===
        // 搜索空间: [1, n-2]（峰顶不会在首尾）
        // 判断依据: arr[mid] vs arr[mid+1]
        //   如果 arr[mid] < arr[mid+1] → 还在上坡，峰顶在右边
        //   如果 arr[mid] > arr[mid+1] → 已经下坡或就是峰顶，峰顶在左边(含mid)
        int left = 0, right = n - 1;
        while (left < right) {
            int mid = left + (right - left) / 2;
            if (mountainArr.get(mid) < mountainArr.get(mid + 1)) {
                left = mid + 1;   // mid 不可能是峰顶（右边更高）
            } else {
                right = mid;      // mid 可能是峰顶（右边更矮或相等）
            }
        }
        int peak = left;  // left == right，就是峰顶
        
        // === Step 2: 在升序段 [0, peak] 二分查找 ===
        // 标准升序二分: arr[mid] < target → 往右搜
        left = 0;
        right = peak;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            int val = mountainArr.get(mid);
            if (val == target) {
                return mid;           // 升序段找到，一定是最小下标
            } else if (val < target) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        
        // === Step 3: 在降序段 [peak+1, n-1] 二分查找 ===
        // 降序二分: arr[mid] < target → 往左搜（因为左边更大）
        //          arr[mid] > target → 往右搜（因为右边更小）
        left = peak + 1;
        right = n - 1;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            int val = mountainArr.get(mid);
            if (val == target) {
                return mid;
            } else if (val < target) {
                right = mid - 1;      // ← 和升序段方向相反!
            } else {
                left = mid + 1;       // ← 和升序段方向相反!
            }
        }
        
        return -1;  // 两段都没找到
    }
};
```

**关键点**：
- 找峰顶用 `left < right`（不带等号），因为需要比较 `mid` 和 `mid+1`
- 升序/降序查找用 `left <= right`（带等号），是标准的查找确切值模板
- 降序段的移动方向和升序段**完全相反**

## 解法对比

| | 暴力线性扫描 | 三次二分 |
|---|---|---|
| `get()` 调用次数 | O(n)，最多 10⁴ | O(log n)，最多 ~56 |
| 能否通过 | ✗ 超过 100 次限制 | ✓ |
| 代码复杂度 | 极简 | 中等（三段二分） |
| 核心洞察 | 无 | 山脉 = 两段有序数组 |

## 易错点

1. **找峰顶时用了 `left <= right`**
   - ✗ `while (left <= right)` → 当 `left == right` 时 `mid+1` 会越界或死循环
   - ✓ `while (left < right)` → 循环结束时 `left == right` 即为峰顶

2. **降序段二分方向写反**
   - ✗ `val < target → left = mid + 1`（这是升序的逻辑）
   - ✓ `val < target → right = mid - 1`（降序中，值小说明要往左找更大的）

3. **峰顶的搜索范围写成 `[0, n-1]` 而不注意 `mid+1` 越界**
   - 实际上因为 `left < right` 且初始 `right = n-1`，`mid` 最大是 `n-2`，`mid+1` 最大是 `n-1`，不会越界。但如果你错误地初始化 `left=0, right=n`，就会出问题。

4. **升序段搜索范围应该包含 peak**
   - ✗ `right = peak - 1` → 漏掉峰顶元素
   - ✓ `right = peak` → 峰顶也属于升序段的一部分

5. **没有先搜升序段**
   - 题目要求**最小**下标，如果先搜降序段找到了就返回，可能错过升序段中更小的下标

## 面试追问

**Q1: 为什么暴力解不行？瓶颈在哪？**
暴力线性扫描需要 O(n) 次 `get()` 调用，题目限制最多 100 次。n 最大 10⁴，远超限制。瓶颈是：没有利用山脉数组"先升后降"的有序结构。

**Q2: 三次二分的 `get()` 调用次数的精确上界是多少？能否进一步减少？**
- 找峰顶：每轮 2 次 `get()`（mid 和 mid+1），最多 log₂(10⁴) ≈ 14 轮 → 28 次
- 升序二分：每轮 1 次，最多 14 次
- 降序二分：每轮 1 次，最多 14 次
- 总计最多 56 次
- **优化**：缓存 `get()` 的结果。找峰顶时，下一轮的 `get(mid)` 可能是上一轮已经算过的 `get(mid+1)`，用一个变量缓存可以减少到约 42 次。

**Q3: 如果山脉数组可能有重复元素（相邻元素可能相等），算法需要怎么改？**
如果有重复元素，`arr[mid] == arr[mid+1]` 时无法判断峰顶在左还是右。此时找峰顶不能保证 O(log n)，最坏退化为 O(n)。类似 LeetCode 81（搜索旋转排序数组 II）的处理方式：遇到相等时只能 `left++` 或 `right--` 线性排除。

## 相关题型

- **852. 山脉数组的峰顶索引** — 本题 Step 1 的子问题，直接复用找峰顶的二分模板
- **162. 寻找峰值** — 类似的"比较 mid 和 mid+1"二分技巧，区别是可能有多个峰（只需找任一个）
- **33. 搜索旋转排序数组** — 同样是"有序结构被打破后做二分"，区别：旋转数组是两段升序拼接，山脉是升序+降序
- **153. 寻找旋转排序数组的最小值** — 复用"在非标准有序数组上二分"的思想，判断条件不同