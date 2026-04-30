# 278. 第一个错误的版本

## 核心思路

本质上是在一个有序序列 `[1, 2, ..., n]` 中，找到**第一个**满足 `isBadVersion(v) == true` 的位置。序列具有单调性：前面全是 `false`，后面全是 `true`，要找的是分界点。

## 思维链

1. **读完题第一反应**：从版本 1 开始一个个调用 `isBadVersion()`，遇到第一个返回 `true` 的就是答案。这就是线性扫描，O(n)。
2. **暴力解的瓶颈**：n 最大可达 2^31 - 1（约 21 亿），线性扫描会超时。瓶颈在于逐个检查太慢。
3. **关键观察**：版本序列天然有"单调性"——`[false, false, ..., false, true, true, ..., true]`。看到有序 + 查找 → 想到**二分查找**。
4. **怎么二分**：检查中间版本 `mid`，如果 `isBadVersion(mid) == true`，说明第一个坏版本在 `mid` 或 `mid` 左边；如果 `false`，说明第一个坏版本一定在 `mid` 右边。每次排除一半，O(log n)。
5. **细节确认**：这是"找左边界"的二分模板——找第一个满足条件的位置，用 `left < right` 模板，最终 `left == right` 就是答案。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 线性扫描 | 从 1 到 n 逐个检查 | O(n) | O(1) | 能说出即可 |
| 二分查找 | 利用单调性每次排除一半 | O(log n) | O(1) | ⭐ 必须写出 |

## 关键提示

- **提示1**：版本序列像 `[G, G, G, B, B, B]`（G=好, B=坏），你要找的是第一个 B。这个序列是"有序"的吗？
- **提示2**：如果 `mid` 是坏版本，答案可能是 `mid` 本身，所以不能跳过 `mid`。想想 `right = mid` 还是 `right = mid - 1`？
- **提示3**：n 最大 2^31 - 1，计算 `mid = (left + right) / 2` 会整数溢出！怎么避免？
- **提示4**：二分搜索空间收缩过程：
```
// 在 [1,2,3,4,5] 中找第一个坏版本 (bad=4):
// 
// 版本:     1     2     3     4     5
// 状态:    good  good  good  BAD   BAD
//          ↑                       ↑
//         left                   right
//
// Round1: mid=3, isBadVersion(3)=false → left=4
//                            ↑
// Round2: [4, 5], mid=4, isBadVersion(4)=true → right=4
//                 ↑
// Round3: left==right==4 → 返回 4 ✓
```

## 解法详解

### 解法1: 线性扫描 — O(n) / O(1)

**思考过程**：最直觉的做法，从版本 1 开始逐个检查，第一个返回 `true` 的就是答案。

```cpp
// The API isBadVersion is defined for you.
// bool isBadVersion(int version);

class Solution {
public:
    int firstBadVersion(int n) {
        for (int i = 1; i <= n; i++) {
            if (isBadVersion(i)) {
                return i;  // 找到第一个坏版本
            }
        }
        return n;  // 题目保证存在坏版本，理论上不会到这里
    }
};
```

**关键点**：思路简单但 n 最大约 21 亿，会 TLE。这个解法的价值在于确认理解了题意。

### 解法2: 二分查找 — O(log n) / O(1) ⭐ 面试首选

**从解法1优化**：线性扫描逐个检查，瓶颈是没有利用序列的单调性。版本状态是 `[false...false, true...true]`，这正是二分查找"找左边界"的经典场景。

```cpp
// The API isBadVersion is defined for you.
// bool isBadVersion(int version);

class Solution {
public:
    int firstBadVersion(int n) {
        int left = 1, right = n;
        while (left < right) {
            // 防溢出写法：等价于 (left + right) / 2
            int mid = left + (right - left) / 2;
            
            if (isBadVersion(mid)) {
                // mid 是坏版本 → 第一个坏版本在 [left, mid]
                // mid 本身可能就是答案，所以 right = mid（不能 mid-1）
                right = mid;
            } else {
                // mid 是好版本 → 第一个坏版本一定在 [mid+1, right]
                // mid 一定不是答案，可以安全跳过
                left = mid + 1;
            }
        }
        // 循环结束时 left == right，就是第一个坏版本
        return left;
    }
};
```

**关键点**：
- 使用 `left < right` 模板（不是 `left <= right`），因为我们找的是边界，不是确切值
- `isBadVersion(mid) == true` 时 `right = mid`（mid 可能是答案，不能跳过）
- `isBadVersion(mid) == false` 时 `left = mid + 1`（mid 一定不是答案）

## 解法对比

| | 线性扫描 | 二分查找 |
|---|---|---|
| 时间 | O(n) | O(log n) |
| API调用次数 | 最多 n 次 | 最多 ~31 次（log₂(2³¹)） |
| 能否通过 | ✗ TLE | ✓ |
| 思维难度 | 无 | 需理解二分模板 |

面试中直接写二分，但可以先口述"线性扫描 O(n) 是最朴素的想法，利用单调性可以二分到 O(log n)"。

## 易错点

1. **整数溢出**：
   - ✗ `int mid = (left + right) / 2;` — 当 left 和 right 都接近 INT_MAX 时溢出
   - ✓ `int mid = left + (right - left) / 2;` — 先减后加，不会溢出
   - 这道题 n 最大 2^31 - 1，这个 bug 会直接导致死循环或错误结果

2. **right 的更新方式搞反**：
   - ✗ `right = mid - 1` — 当 mid 恰好是第一个坏版本时，会跳过答案
   - ✓ `right = mid` — mid 是坏版本说明答案 ≤ mid，不能排除 mid

3. **循环条件用错**：
   - ✗ `while (left <= right)` 配合 `right = mid` → 死循环（当 left == right 时永远不退出）
   - ✓ `while (left < right)` 配合 `right = mid` → left == right 时退出，正确

4. **搜索范围起点错误**：
   - ✗ `int left = 0` — 版本号从 1 开始，不存在版本 0
   - ✓ `int left = 1`

## 面试追问

**Q1: 线性扫描 O(n) 能过吗？瓶颈在哪？**
> 不能，n 最大 21 亿会 TLE。瓶颈在于逐个检查没有利用"前面全是好、后面全是坏"的单调结构。

**Q2: 你用的是哪种二分模板？为什么选这个？**
> `left < right` + `right = mid` 模板，这是"找左边界"（第一个满足条件的位置）的标准模板。因为当 mid 是坏版本时，mid 可能是答案，不能跳过，所以 `right = mid`。配合 `left < right` 确保不死循环，退出时 `left == right` 就是答案。

**Q3: 如果 API 调用非常昂贵（比如每次要跑一套测试），还能进一步减少调用次数吗？**
> 二分已经是信息论下的最优——每次调用获得 1 bit 信息，n 个版本需要 log₂(n) bit 来定位。无法再减少。但如果允许并行调用，可以用并行二分在更短时间内完成。

## 相关题型

- **34. 在排序数组中查找元素的第一个和最后一个位置** — 复用同样的"找左边界"二分模板，区别：本题搜索空间是 [1,n] 调 API，34 是在数组上找值
- **35. 搜索插入位置** — 同样是找"第一个 ≥ target 的位置"，和本题的二分模板完全一致
- **69. x 的平方根** — 二分的搜索空间不是数组索引而是答案范围 [0, x]，但"排除一半"的思想相同
- **374. 猜数字大小** — 几乎一模一样的题，API 从 `isBadVersion` 换成 `guess`，二分模板直接复用