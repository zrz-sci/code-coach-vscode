/*
 * LeetCode 973: 最接近原点的 K 个点 (K Closest Points to Origin)
 *
 * 【题目本质】
 * 经典 Top-K 问题：在 n 个点中找到距离原点最近的 k 个。
 * 距离比较可以用 x²+y² 代替 √(x²+y²)，避免浮点运算。
 *
 * 【解法总览】
 * 解法1: 排序         — O(n log n) / O(log n)  — 最直觉
 * 解法2: 大顶堆       — O(n log k) / O(k)      — 面试首选
 * 解法3: 快速选择     — O(n) 平均 / O(1) 额外   — 最优/加分项
 */

// ============================================================
// 解法1: 排序 — 按距离排序取前 k 个
// 时间: O(n log n)  空间: O(log n) 排序栈空间
//
// 【思路】
// 最直觉的想法：既然要找最近的 k 个，那就按距离排个序，
// 取前 k 个就行了。
// 
// 瓶颈：我们只要前 k 小，却把所有 n 个都排好了序，
// 当 k << n 时浪费了大量计算。
// ============================================================
class Solution1 {
public:
    vector<vector<int>> kClosest(vector<vector<int>>& points, int k) {
        // 按 x²+y² 升序排列（不需要开根号，大小关系一致）
        sort(points.begin(), points.end(), [](const vector<int>& a, const vector<int>& b) {
            return a[0]*a[0] + a[1]*a[1] < b[0]*b[0] + b[1]*b[1];
        });
        // 排好序后前 k 个就是答案
        return vector<vector<int>>(points.begin(), points.begin() + k);
    }
};

// ============================================================
// 解法2: 大顶堆 — 维护大小为 k 的大顶堆，堆顶是候选中最远的
// 时间: O(n log k)  空间: O(k)
//
// 【思路】
// 排序的瓶颈：全量排序做了无用功。
// 优化思路：只维护"当前最近的 k 个"候选集。
//
// 为什么用大顶堆？
// - 堆顶 = 候选集中距离最大的点
// - 新点来了，和堆顶比：
//   比堆顶小 → 淘汰堆顶，加入新点（新点更好）
//   比堆顶大 → 忽略（新点比最差候选还差）
// - 遍历结束后堆里就是答案
//
// 如果用小顶堆？堆顶是最小的，无法快速判断最差候选是谁，
// 必须把 n 个全入堆才能 pop k 次，空间 O(n)。
//
// 示例: points=[[1,3],[-2,2],[3,3]], k=2
//   dist: [1,3]=10, [-2,2]=8, [3,3]=18
//
//   遍历 [1,3]:  堆={10}        (size<k, 直接加)
//   遍历 [-2,2]: 堆={10, 8}     (size==k) 堆顶=10
//   遍历 [3,3]:  dist=18 > 堆顶10 → 跳过
//   结果: [1,3] 和 [-2,2]
// ============================================================
class Solution2 {
public:
    vector<vector<int>> kClosest(vector<vector<int>>& points, int k) {
        // 比较函数：距离小的优先级低 → 距离大的在堆顶 → 大顶堆
        // C++ priority_queue 默认用 less<>，即 a < b 时 b 优先级更高
        // 所以 dist(a) < dist(b) 意味着 b 排在堆顶 → 大顶堆 ✓
        auto cmp = [](const vector<int>& a, const vector<int>& b) {
            return a[0]*a[0] + a[1]*a[1] < b[0]*b[0] + b[1]*b[1];
        };
        priority_queue<vector<int>, vector<vector<int>>, decltype(cmp)> maxHeap(cmp);
        
        for (auto& p : points) {
            maxHeap.push(p);
            if ((int)maxHeap.size() > k) {
                // 堆超过 k 个，弹出距离最大的（堆顶）
                maxHeap.pop();
            }
        }
        
        // 堆中剩余的 k 个就是答案
        vector<vector<int>> res;
        res.reserve(k);
        while (!maxHeap.empty()) {
            res.push_back(maxHeap.top());
            maxHeap.pop();
        }
        return res;
    }
};

// ============================================================
// 解法3: 快速选择 (Quickselect) — Partition 把前 k 小放到数组左侧
// 时间: O(n) 平均, O(n²) 最坏  空间: O(1) 额外 (原地操作)
//
// 【思路】
// 堆的瓶颈：当 k 接近 n 时，O(n log k) ≈ O(n log n)。
// 快速选择利用快排的 partition 思想，期望 O(n) 就够。
//
// 核心：我们不需要完全排序，只需要让最小的 k 个在左边。
// - partition 选一个 pivot，把 < pivot 的放左边，≥ pivot 的放右边
// - 如果 pivot 落在位置 k → 左边恰好 k 个，就是答案
// - 如果 pivot 落在位置 > k → 答案全在左边，递归左半
// - 如果 pivot 落在位置 < k → 左边不够 k 个，递归右半
//
// 和快排的区别：
// - 快排两边都递归 → T(n) = 2T(n/2) + O(n) = O(n log n)
// - 快速选择只递归一边 → T(n) = T(n/2) + O(n) = O(n) 平均
//
// 随机化 pivot 的重要性：
// - 不随机化：如果数组已排序，每次 partition 只排除 1 个 → O(n²)
// - 随机化：期望每次排除一半 → O(n) 平均
//
// Partition 过程示例:
// points 距离: [18, 26, 20, 2], k=2
// 随机选 pivot=20 (放到末尾)
//
//   i=0: dist=18 < 20 → swap(points[0], points[0]), store=1
//   i=1: dist=26 ≥ 20 → skip
//   i=2: dist=2  < 20 → swap(points[2], points[1]), store=2
//   最后: swap(points[store], points[right]) → pivot 放到位置 2
//
//   结果: [18, 2, | 20, 26]
//                   ↑ pivotIdx=2
//
//   pivotIdx == k=2 → 前 2 个 [18, 2] 就是答案！
// ============================================================
class Solution3 {
public:
    vector<vector<int>> kClosest(vector<vector<int>>& points, int k) {
        int left = 0, right = (int)points.size() - 1;
        
        while (left < right) {
            int pivotIdx = partition(points, left, right);
            
            if (pivotIdx == k) {
                // pivot 左边恰好 k 个元素，全部 < pivot 距离
                break;
            } else if (pivotIdx < k) {
                // 左边不够 k 个，需要从右半部分再取一些
                left = pivotIdx + 1;
            } else {
                // 左边超过 k 个，缩小范围
                right = pivotIdx - 1;
            }
        }
        
        return vector<vector<int>>(points.begin(), points.begin() + k);
    }
    
private:
    int dist(const vector<int>& p) {
        return p[0] * p[0] + p[1] * p[1];
    }
    
    int partition(vector<vector<int>>& points, int left, int right) {
        // 随机选 pivot，避免最坏情况
        int randIdx = left + rand() % (right - left + 1);
        swap(points[randIdx], points[right]); // pivot 放到末尾
        
        int pivotDist = dist(points[right]);
        int storeIdx = left; // storeIdx 左边的元素距离都 < pivotDist
        
        for (int i = left; i < right; i++) {
            if (dist(points[i]) < pivotDist) {
                swap(points[i], points[storeIdx]);
                storeIdx++;
            }
        }
        // 把 pivot 放到最终位置
        swap(points[storeIdx], points[right]);
        return storeIdx;
    }
};

// ============================================================
// 【解法对比】
//
// | 维度         | 排序           | 大顶堆         | 快速选择         |
// |-------------|---------------|----------------|-----------------|
// | 时间         | O(n log n)    | O(n log k)     | O(n) 平均       |
// | 空间         | O(log n)      | O(k)           | O(1) 额外       |
// | 修改原数组    | 是             | 否             | 是              |
// | 结果有序      | 是             | 否             | 否              |
// | 最坏情况      | 稳定           | 稳定            | O(n²)           |
// | 面试推荐      | 开场展示理解    | ⭐ 首选写出     | 口述加分         |
//
// 选择建议:
// - k << n 且需要流式处理 → 堆（数据源很大，一次读一个点）
// - 允许修改数组且追求极致性能 → 快速选择
// - 简单场景或写代码时间紧张 → 排序
//
// 【易错点】
// 1. 堆的比较方向:
//    ✗ auto cmp = [](a, b){ return dist(a) > dist(b); }; → 小顶堆
//      这样 pop 掉的是最近的点而不是最远的，结果完全错误
//    ✓ auto cmp = [](a, b){ return dist(a) < dist(b); }; → 大顶堆
//      pop 掉最远的点，留下最近的 k 个
//
// 2. 快速选择的比较条件:
//    ✗ if (dist(points[i]) <= pivotDist)  → 等于 pivot 的也放左边
//      可能导致 storeIdx 一直前进，pivot 无法归位，partition 退化
//    ✓ if (dist(points[i]) < pivotDist)   → 严格小于才放左边
//      等于 pivot 的留在右边，partition 更均匀
//
// 3. 终止条件 pivotIdx == k 而不是 k-1:
//    partition 返回的 pivotIdx 表示 "pivot 放在了位置 pivotIdx"
//    位置 [0, pivotIdx-1] 的元素都 < pivot
//    所以当 pivotIdx == k 时，左边恰好有 k 个 < pivot 的元素
//    如果写成 pivotIdx == k-1，就少取了一个
//
// 4. 距离不需要开根号:
//    ✗ sqrt(x*x + y*y) → 引入浮点误差，且更慢
//    ✓ x*x + y*y       → 整数运算，大小关系完全一致
//
// 【面试追问】
// Q1: "排序法能不能过？瓶颈在哪？"
//     → n ≤ 10⁴ 完全能过，瓶颈是当 k << n 时对 n-k 个无关元素也排了序。
//
// Q2: "如果数据量很大（10⁸个点），k=10，怎么办？"
//     → 大顶堆，O(n log 10) ≈ O(n)，且可以流式处理，
//       不需要把所有数据同时加载到内存。
//
// Q3: "能不能做到严格 O(n)，最坏也是？"
//     → 理论上用 Median of Medians 选 pivot 可以保证最坏 O(n)，
//       实际面试中随机化快速选择 + 提一句这个知识点即可。
//
// Q4: "快速选择会修改原数组，如果不允许呢？"
//     → 先拷贝一份再操作，空间变成 O(n)。或者用堆，天然不修改输入。
//
// Q5: "如果还要求返回结果按距离排序呢？"
//     → 快速选择后对前 k 个再 sort，总时间 O(n + k log k)。
//       堆直接 pop k 次就是有序的，O(n log k)。
// ============================================================
