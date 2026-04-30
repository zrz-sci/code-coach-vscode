# 852. 山脉数组的峰顶索引

## 核心思路

山脉数组先递增后递减，本质上是在一个"有结构"的数组中找**转折点（峰值）**——即左边都在上升、右边都在下降的那个位置。这种"二段性"天然适合二分查找。

## 思维链

1. **读完题第一反应**：遍历一遍数组，找到 `arr[i] > arr[i-1] && arr[i] > arr[i+1]` 的位置就是峰值。这是 O(n) 的线性扫描。
2. **暴力解的瓶颈**：线性扫描必须逐个检查，时间 O(n)。但题目要求 O(log n)，提示我们用二分。
3. **凭什么能二分？** 关键观察：山脉数组具有"二段性"。峰值左侧是严格递增段（`arr[mid] < arr[mid+1]`），峰值右侧是严格递减段（`arr[mid] > arr[mid+1]`）。站在任意一个位置 mid，通过比较 `arr[mid]` 和 `arr[mid+1]` 就能判断自己在山的哪一侧，从而排除一半搜索空间。
4. **具体怎么二分？**
   - 如果 `arr[mid] < arr[mid+1]`：mid 在上升段，峰值一定在 mid 右边 → `left = mid + 1`
   - 如果 `arr[mid] > arr[mid+1]`：mid 在下降段（或就是峰值），峰值在 mid 或 mid 左边 → `right = mid`
   - 循环条件 `left < right`，最终 `left == right` 就是峰值索引

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 线性扫描 | 遍历找第一个下降点 | O(n) | O(1) | 能说出即可 |
| 二分查找 | 利用山脉二段性缩半 | O(log n) | O(1) | ⭐ 必须写出 |
| 三分查找 | 三等分比较排除 1/3 | O(log n) | O(1) | 加分项（了解即可） |

## 关键提示

1. **山脉数组的"二段性"**：峰值左边严格递增，右边严格递减。站在 mid 只需要看 `arr[mid]` 和 `arr[mid+1]` 的关系就能判断在哪一侧。
2. **为什么比较 mid 和 mid+1？** 因为山脉数组没有相邻元素相等的情况，所以 `arr[mid]` 和 `arr[mid+1]` 的大小关系明确告诉你递增还是递减。
3. **搜索空间的初始范围**：可以设 `left = 0, right = n-1`。题目保证峰值不在首尾（因为山脉数组至少先升后降），所以也可以 `left = 1, right = n-2`。
4. **循环终止时 `left == right`**：此时指向的就是峰值，不需要额外判断。

```
// 山脉数组的二段性示意（arr = [0, 2, 4, 7, 5, 3, 1]）:
//
//  arr:  0  2  4  7  5  3  1
//  idx:  0  1  2  3  4  5  6
//                 ^
//              峰值 idx=3
//
//  上升段: arr[0..3]  每个 arr[i] < arr[i+1]
//  下降段: arr[3..6]  每个 arr[i] > arr[i+1]
//
//  二分过程:
//  Round1: left=0, right=6, mid=3 → arr[3]=7 > arr[4]=5 → right=3
//  Round2: left=0, right=3, mid=1 → arr[1]=2 < arr[2]=4 → left=2
//  Round3: left=2, right=3, mid=2 → arr[2]=4 < arr[3]=7 → left=3
//  left==right==3 → 峰值索引=3 ✓
```

## 解法详解

### 解法1: 线性扫描 — O(n) / O(1)

**思考过程**：最直觉的方法——山脉数组先升后降，那从左往右扫，第一个 `arr[i] > arr[i+1]` 的位置就是峰值。因为在峰值之前都是 `arr[i] < arr[i+1]`（递增），峰值处第一次出现下降。

```cpp
class Solution {
public:
    int peakIndexInMountainArray(vector<int>& arr) {
        // 从左往右找第一个开始下降的位置
        for (int i = 1; i < (int)arr.size() - 1; i++) {
            if (arr[i] > arr[i + 1]) {
                return i;  // 第一个"比右邻大"的位置就是峰值
            }
        }
        return -1;  // 题目保证是山脉数组，不会走到这里
    }
};
```

**关键点**：这个方法正确但不满足题目 O(log n) 的要求。瓶颈在于线性遍历——我们没有利用"山脉数组的结构"来跳过大量无用的检查。

---

### 解法2: 二分查找 — O(log n) / O(1) ⭐ 面试首选

**从解法1优化**：线性扫描逐个检查，但其实我们站在任意一个位置 mid，只需比较 `arr[mid]` 和 `arr[mid+1]`：
- `arr[mid] < arr[mid+1]` → 当前在上升段，峰值在右边
- `arr[mid] > arr[mid+1]` → 当前在下降段或就是峰值，峰值在左边（含 mid）

这就是经典的"二段性"，每次排除一半搜索空间。

```
// 搜索空间收缩过程 (arr = [0, 2, 1, 0]):
//
// Round1: [0, 2, 1, 0]  left=0, right=3, mid=1
//             ^
//          arr[1]=2 > arr[2]=1 → 下降段 → right=1
//
// Round2: [0, 2]  left=0, right=1, mid=0
//          ^
//          arr[0]=0 < arr[1]=2 → 上升段 → left=1
//
// left==right==1 → 峰值索引=1 ✓
```

```cpp
class Solution {
public:
    int peakIndexInMountainArray(vector<int>& arr) {
        int left = 0, right = arr.size() - 1;
        
        while (left < right) {
            int mid = left + (right - left) / 2;
            
            if (arr[mid] < arr[mid + 1]) {
                // mid 在上升段，峰值一定在 mid+1 或更右
                // mid 本身不可能是峰值（因为右边还有更大的）
                left = mid + 1;
            } else {
                // arr[mid] > arr[mid+1]
                // mid 在下降段或就是峰值
                // mid 有可能是答案，所以 right = mid 而非 mid - 1
                right = mid;
            }
        }
        
        // 循环结束时 left == right，指向峰值
        return left;
    }
};
```

**关键点**：
- 使用 `left < right` 模板（不是 `<=`），因为我们要找的是一个"边界"，不是一个确切的目标值。
- `right = mid` 而非 `right = mid - 1`：因为 mid 自身可能就是峰值，不能排除。
- `left = mid + 1`：如果 `arr[mid] < arr[mid+1]`，说明 mid 肯定不是峰值（右边有更大的），可以安全排除。

---

### 解法3: 三分查找 — O(log n) / O(1)

**不同角度思考**：三分查找把区间分成三段，取两个中间点 m1、m2 来比较，每次排除约 1/3 的搜索空间。虽然也是 O(log n)，但常数因子比二分大，实际更慢，这里仅作了解。

```cpp
class Solution {
public:
    int peakIndexInMountainArray(vector<int>& arr) {
        int left = 0, right = arr.size() - 1;
        
        while (right - left > 2) {
            int m1 = left + (right - left) / 3;
            int m2 = right - (right - left) / 3;
            
            if (arr[m1] < arr[m2]) {
                // 峰值在 m1 右侧
                left = m1 + 1;
            } else {
                // 峰值在 m2 左侧
                right = m2 - 1;
            }
        }
        
        // 剩余 <= 3 个元素，线性找最大值
        int peak = left;
        for (int i = left + 1; i <= right; i++) {
            if (arr[i] > arr[peak]) peak = i;
        }
        return peak;
    }
};
```

**关键点**：三分查找更适合"没有明确二段性、只知道是单峰函数"的场景。本题有明确的二段性（上升 vs 下降），二分就够了。

## 解法对比

| | 线性扫描 | 二分查找 ⭐ | 三分查找 |
|---|---|---|---|
| 时间 | O(n) | O(log n) | O(log₃ n) ≈ O(log n) |
| 空间 | O(1) | O(1) | O(1) |
| 利用的性质 | "先升后降" | "二段性：比较 mid 和 mid+1" | "单峰函数" |
| 代码复杂度 | 最简单 | 简单 | 稍复杂 |
| 面试推荐 | 开场思路 | **首选** | 了解即可 |

**什么时候选哪个？**
- 面试：直接写二分，O(log n) 正好满足要求。
- 如果没有"二段性"但知道是单峰函数（比如浮点数域上），三分查找更通用。

## 易错点

1. **循环条件写成 `left <= right` 导致死循环**
   - ✗ `while (left <= right)` + `right = mid` → 当 `left == right == mid` 时永远不退出
   - ✓ `while (left < right)` + `right = mid` → `left == right` 时退出，指向答案

2. **`right = mid - 1` 导致漏掉峰值**
   - ✗ `right = mid - 1`：当 `arr[mid] > arr[mid+1]` 时 mid 本身可能是峰值，`mid-1` 跳过了它
   - ✓ `right = mid`：保留 mid 作为候选

3. **访问 `arr[mid+1]` 越界**
   - 当 `right = arr.size() - 1` 且 `left < right` 时，`mid` 最大为 `right - 1`，所以 `mid + 1 <= right`，不会越界。但如果误用 `left <= right`，`mid` 可能等于 `right = n-1`，此时 `arr[mid+1]` 越界。

4. **mid 计算溢出**
   - ✗ `int mid = (left + right) / 2`：当 left + right > INT_MAX 时溢出
   - ✓ `int mid = left + (right - left) / 2`：安全写法

## 面试追问

**Q1: 你能先用 O(n) 解决这道题吗？瓶颈在哪？**
> 线性扫描找第一个 `arr[i] > arr[i+1]` 的 i。瓶颈在于没有利用山脉数组的"二段性"，逐个检查浪费了结构信息。

**Q2: 怎么把 O(n) 优化到 O(log n)？为什么二分在这里可行？**
> 山脉数组有"二段性"：峰值左边 `arr[mid] < arr[mid+1]`，右边 `arr[mid] > arr[mid+1]`。站在 mid 看一眼就能判断峰值在哪一侧，每次排除一半。

**Q3: 如果数组不保证是山脉数组，而只是"有一个局部峰值"（可能有多个），怎么办？**
> 这就变成了 LeetCode 162 (Find Peak Element)。思路类似：比较 `arr[mid]` 和 `arr[mid+1]`，如果 `arr[mid] < arr[mid+1]` 则右侧一定存在峰值（因为如果一直上升到边界，边界本身就是峰值）。同样 O(log n)。

**Q4: 如果给的不是数组，而是一个"山脉函数" `f(x)` 只能查询，查询次数要最少？**
> 二分查找仍然适用，每次查询 f(mid) 和 f(mid+1)。如果是连续函数且精度有要求，可以用三分查找。

## 相关题型

- **162. Find Peak Element** — 复用完全相同的二分模板（比较 `arr[mid]` 和 `arr[mid+1]`），区别：162 的数组可以有多个峰值，只需返回任意一个。
- **34. Find First and Last Position of Element in Sorted Array** — 练习"找边界"的二分模板（`left < right` + `right = mid`），本题找的是"上升段和下降段的分界点"。
- **1095. Find in Mountain Array** — 本题的进阶版：先用本题方法找到峰值索引，然后在左半段（升序）和右半段（降序）各做一次标准二分查找目标值。