# 973. 最接近原点的 K 个点

## 核心思路

本质上就是一个 **Top-K 问题**：在 n 个元素中找出最小（或最大）的 k 个。距离的比较可以用 `x²+y²` 代替，避免开根号。

## 思维链

1. **读完题第一反应**：算出每个点到原点的距离，排个序，取前 k 个就行了。距离比较不需要开根号，比 `x²+y²` 就够了。
2. **暴力排序的代价**：全量排序是 O(n log n)，但我们只需要前 k 小，把其余 n-k 个也排好序是浪费的。
3. **怎么只取前 k 小？**：
   - **堆（优先队列）**：维护一个大小为 k 的大顶堆。遍历时，如果当前点的距离比堆顶小，就替换堆顶。遍历结束后堆里就是最近的 k 个点。时间 O(n log k)。
   - **快速选择（Quickselect）**：类似快排的 partition，只需要把前 k 小的元素放到数组左侧，不需要完全排序。**平均** O(n)，最坏 O(n²)。
4. **面试中的最佳选择**：排序最简单不会出错；堆是经典 Top-K 手法，面试官最常期望；快速选择是加分项，能体现算法深度。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 排序 | 按距离排序取前 k | O(n log n) | O(log n) | 能说出即可 |
| 大顶堆 | 维护大小为 k 的大顶堆 | O(n log k) | O(k) | ⭐ 必须写出 |
| 快速选择 | Partition 把前 k 小放左侧 | O(n) 平均 | O(1) | 加分项 |

## 关键提示

1. **距离比较不需要开根号**：`x²+y²` 的大小关系和 `√(x²+y²)` 完全一致，省去浮点运算。
2. **Top-K 用堆怎么选大小顶？** 想要最小的 k 个 → 用**大顶堆**（堆顶是堆中最大的，方便淘汰）。如果用小顶堆就需要把所有 n 个元素入堆，空间 O(n)。
3. **快速选择的 partition 和快排完全一样**，区别在于快排两边都递归，快速选择只递归包含第 k 个元素的那一边。
4. **答案不要求有序**：所有解法都不需要对结果排序，这也是快速选择能用的前提。
5. **快速选择最坏退化为 O(n²)**：随机选取 pivot 可以有效避免，面试时记得提一句。

## 解法详解

### 解法1: 排序 — O(n log n) / O(log n)

**思考过程**: 最直觉的想法——把所有点按照到原点的距离排序，取前 k 个。

```cpp
class Solution {
public:
    vector<vector<int>> kClosest(vector<vector<int>>& points, int k) {
        // 按 x²+y² 排序，不需要开根号
        sort(points.begin(), points.end(), [](const vector<int>& a, const vector<int>& b) {
            return a[0]*a[0] + a[1]*a[1] < b[0]*b[0] + b[1]*b[1];
        });
        // 取前 k 个
        return vector<vector<int>>(points.begin(), points.begin() + k);
    }
};
```

**关键点**: 简单可靠，但对 n 很大、k 很小的场景（如 n=10⁷, k=10）做了大量无用排序。

---

### 解法2: 大顶堆 — O(n log k) / O(k) ⭐ 面试首选

**从解法1优化**: 排序的瓶颈是对所有 n 个元素排序，而我们只关心最小的 k 个。用大小为 k 的大顶堆：堆顶始终是"当前 k 个候选中最远的那个"。如果新来的点比堆顶更近，就淘汰堆顶、加入新点。

```
// 以 points=[[3,3],[5,-1],[-2,4],[1,1]], k=2 为例:
//
// dist: [3,3]=18, [5,-1]=26, [-2,4]=20, [1,1]=2
//
// 遍历 [3,3]:  堆 = {18}          (size < k, 直接加)
// 遍历 [5,-1]: 堆 = {26, 18}      (size < k, 直接加)  堆顶=26
// 遍历 [-2,4]: dist=20 < 堆顶26   → 弹出26, 加入20 → 堆 = {20, 18}
// 遍历 [1,1]:  dist=2  < 堆顶20   → 弹出20, 加入2  → 堆 = {18, 2}
//
// 结果: [3,3] 和 [1,1] — 即距离为 18 和 2 的两个点
```

```cpp
class Solution {
public:
    vector<vector<int>> kClosest(vector<vector<int>>& points, int k) {
        // 大顶堆：pair<距离, 索引>，距离大的在堆顶
        auto dist = [](const vector<int>& p) { return p[0]*p[0] + p[1]*p[1]; };
        
        // 自定义比较：距离小的优先级低（大顶堆）
        auto cmp = [&](const vector<int>& a, const vector<int>& b) {
            return dist(a) < dist(b); // 注意：< 构成大顶堆
        };
        priority_queue<vector<int>, vector<vector<int>>, decltype(cmp)> maxHeap(cmp);
        
        for (auto& p : points) {
            maxHeap.push(p);
            // 堆大小超过 k 就弹出最远的（堆顶）
            if ((int)maxHeap.size() > k) {
                maxHeap.pop();
            }
        }
        
        vector<vector<int>> res;
        while (!maxHeap.empty()) {
            res.push_back(maxHeap.top());
            maxHeap.pop();
        }
        return res;
    }
};
```

**关键点**:
- **为什么用大顶堆不用小顶堆？** 大顶堆堆顶是最大的，方便判断"新元素是否比当前最差的候选更好"并淘汰。小顶堆要取最大值就得遍历整个堆。
- **比较函数的方向很容易写反**：`<` 得到大顶堆（C++ priority_queue 的默认行为），`>` 得到小顶堆。

---

### 解法3: 快速选择 (Quickselect) — O(n) 平均 / O(1) 额外空间

**从解法2优化**: 堆的 O(n log k) 在 k 接近 n 时退化为 O(n log n)。快速选择利用 partition 思想，平均 O(n) 就能把前 k 小的元素放到数组左侧。

```
// Partition 过程 (以 pivot 距离=20 为例):
//
// 初始:  [18, 26, 20, 2]  (距离值)
//         ↑              ↑
//        left           right
//
// 目标: 让距离 ≤ pivot 的都在左边, > pivot 的在右边
//
// 一轮 partition 后: [18, 2, 20, 26]
//                     ↑ ↑     ↑
//                    ≤pivot  pivotIdx
//
// 如果 pivotIdx == k-1 → 前 k 个就是答案
// 如果 pivotIdx > k-1  → 答案在左半边，只递归左边
// 如果 pivotIdx < k-1  → 答案跨越 pivot，只递归右边
```

```cpp
class Solution {
public:
    vector<vector<int>> kClosest(vector<vector<int>>& points, int k) {
        int left = 0, right = points.size() - 1;
        while (left < right) {
            int pivotIdx = partition(points, left, right);
            if (pivotIdx == k) {
                break;           // 前 k 个已经在左侧
            } else if (pivotIdx < k) {
                left = pivotIdx + 1;  // 前 k 个还需要右边的一些
            } else {
                right = pivotIdx - 1; // 前 k 个全在左边更小的范围
            }
        }
        return vector<vector<int>>(points.begin(), points.begin() + k);
    }
    
private:
    int dist(const vector<int>& p) {
        return p[0]*p[0] + p[1]*p[1];
    }
    
    int partition(vector<vector<int>>& points, int left, int right) {
        // 随机选 pivot 避免最坏情况
        int randIdx = left + rand() % (right - left + 1);
        swap(points[randIdx], points[right]);
        
        int pivotDist = dist(points[right]);
        int storeIdx = left; // storeIdx 左边的都 < pivotDist
        
        for (int i = left; i < right; i++) {
            if (dist(points[i]) < pivotDist) {
                swap(points[i], points[storeIdx]);
                storeIdx++;
            }
        }
        swap(points[storeIdx], points[right]); // pivot 归位
        return storeIdx;
    }
};
```

**关键点**:
- **和快排的区别**：快排两边都递归 → O(n log n)，快速选择只递归一边 → O(n) 平均。
- **随机 pivot**：没有随机化时，已排序数组会导致每次 partition 只减少 1 个元素 → O(n²)。
- **会修改原数组**：如果不允许修改输入，需要先拷贝。

---

## 解法对比

| | 排序 | 大顶堆 | 快速选择 |
|---|---|---|---|
| 时间 | O(n log n) | O(n log k) | O(n) 平均 |
| 空间 | O(log n) 栈 | O(k) | O(1) 额外 |
| 修改原数组 | 是 | 否 | 是 |
| 结果有序 | 是 | 否 | 否 |
| 最坏情况 | 稳定 O(n log n) | 稳定 O(n log k) | O(n²) |
| 实现难度 | 简单 | 中等 | 较难 |

**面试建议**: 先说排序方案展示理解 → 写出堆的方案得到认可 → 口述快速选择得到加分。

## 易错点

1. **堆的比较方向写反**:
   - ✗ `return dist(a) > dist(b);` → 这构成了小顶堆，堆大小会膨胀到 n
   - ✓ `return dist(a) < dist(b);` → 大顶堆，堆顶是距离最大的，方便淘汰

2. **快速选择的终止条件用 `pivotIdx == k-1` 还是 `pivotIdx == k`**:
   - ✗ `pivotIdx == k - 1` 然后取 `points[0..k-1]` — 这漏掉了 pivot 右边可能有等距点的情况
   - ✓ 用 `pivotIdx == k` 表示 "左边恰好有 k 个元素"（pivot 本身是第 k+1 小的）。具体取决于你的 partition 写法，关键是要统一。

3. **距离计算溢出**:
   - ✗ `x*x + y*y` 当 x, y 接近 10⁴ 时结果为 2×10⁸，在 `int` 范围内（2.1×10⁹），**本题不会溢出**
   - ⚠️ 但如果坐标范围更大（如 10⁵），就需要用 `long long`

4. **开根号引入浮点误差**:
   - ✗ `sqrt(x*x + y*y)` 比较时可能有精度问题
   - ✓ 直接比较 `x*x + y*y`，整数运算无误差

## 面试追问

**Q1: 暴力排序能过吗？瓶颈在哪？**
→ n ≤ 10⁴ 完全能过。瓶颈是全量排序 O(n log n)，当 k << n 时做了大量无用功。

**Q2: 如果 n 特别大（10⁸）、k 很小（10），怎么优化？**
→ 用大小为 k 的大顶堆，O(n log k) ≈ O(n log 10) ≈ O(n)。甚至可以流式处理，不需要把所有数据加载到内存。

**Q3: 能否做到严格 O(n)？最坏情况也是 O(n)？**
→ 使用 **Introselect**（快速选择 + 中位数的中位数）可以保证最坏 O(n)，但实现复杂。实际面试中用随机化快速选择即可，平均 O(n)。

**Q4: 如果需要返回结果按距离排序呢？**
→ 快速选择只保证前 k 个在左侧但不保证有序。如果需要有序，对这 k 个再排序，O(n + k log k)。

## 相关题型

- **215. 数组中的第K个最大元素** — 完全相同的快速选择/堆模板，区别仅在于比较的是数值本身而非距离
- **347. 前 K 个高频元素** — 同样的 Top-K 框架，区别在于先用哈希表统计频率，再对频率做 Top-K
- **692. 前K个高频单词** — Top-K + 自定义排序（频率相同按字典序），复用堆模板但比较函数更复杂
- **quickselect 模板题**：学会本题的 partition 写法后，215 可以直接复用