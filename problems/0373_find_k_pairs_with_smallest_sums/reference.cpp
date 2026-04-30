// =====================================================
// 373. Find K Pairs with Smallest Sums
// 标签: Array, Heap (Priority Queue)
// 难度: Medium
// =====================================================

// 解法1: 最小堆 — 多路合并（最优解）⭐
// 时间 O(k * log(min(n,k))), 空间 O(min(n,k))
//
// 核心思路:
//   两个有序数组形成一个 "虚拟有序矩阵":
//     matrix[i][j] = nums1[i] + nums2[j]
//   每行递增，每列递增。问题转化为"从有序矩阵中找最小的 k 个元素"。
//
//   类比 "合并 K 个有序链表":
//     nums1 的每个元素产生一条有序链表:
//       链表 i: nums1[i]+nums2[0], nums1[i]+nums2[1], ...
//     共 n 条链表，用最小堆做多路合并。
//
//   多路合并示意:
//     nums1 = [1,7,11], nums2 = [2,4,6]
//
//           2    4    6
//      1   [3] → 5  → 7     ← 链表0
//      7   [9] → 11 → 13    ← 链表1
//     11  [13] → 15 → 17    ← 链表2
//          ↑
//     初始堆中有 [3, 9, 13]
//
//   Step1: 弹出 3 (0,0) → 推入 5 (0,1)
//   Step2: 弹出 5 (0,1) → 推入 7 (0,2)
//   Step3: 弹出 7 (0,2) → j+1 越界，不推入
//   结果: [[1,2], [1,4], [1,6]]
//
// 关键设计:
//   - 初始化: 只入堆 min(n, k) 个头部 (i, 0)
//   - 弹出 (i, j) 后: 只推入 (i, j+1)，不推入 (i+1, j)
//     因为 (i+1, 0) 在初始化时已入堆，之后沿 j 推进可达 (i+1, j)
//   - 这样无需 visited 集合去重
class Solution1 {
public:
    vector<vector<int>> kSmallestPairs(vector<int>& nums1, vector<int>& nums2, int k) {
        vector<vector<int>> res;
        if (nums1.empty() || nums2.empty()) return res;

        int n = nums1.size(), m = nums2.size();

        // 最小堆: {sum, i, j}
        // 注意: C++ priority_queue 默认大顶堆
        //       比较器返回 a > b 才是小顶堆
        auto cmp = [](const tuple<int,int,int>& a, const tuple<int,int,int>& b) {
            return get<0>(a) > get<0>(b);
        };
        priority_queue<tuple<int,int,int>, vector<tuple<int,int,int>>, decltype(cmp)> pq(cmp);

        // 初始化: 将每行的第一个元素入堆
        // 只需 min(n, k) 个，后面的行不可能贡献前 k 小
        for (int i = 0; i < min(n, k); i++) {
            pq.push({nums1[i] + nums2[0], i, 0});
        }

        // 弹出 k 个最小值
        while (k-- > 0 && !pq.empty()) {
            auto [sum, i, j] = pq.top();
            pq.pop();
            res.push_back({nums1[i], nums2[j]});

            // 推进 j: 将同一行的下一个元素入堆
            if (j + 1 < m) {
                pq.push({nums1[i] + nums2[j + 1], i, j + 1});
            }
            // 注意: 不推入 (i+1, j)
            // 因为 (i+1, 0) 已在初始化时入堆
        }
        return res;
    }
};


// =====================================================
// 解法2: 最小堆 — 从 (0,0) 出发，推进 i 和 j（需要 visited）
// 时间 O(k * log k), 空间 O(k)
//
// 另一种方式: 初始只放 (0,0)，弹出 (i,j) 后推入 (i+1,j) 和 (i,j+1)。
// 需要 visited 集合防止重复入堆。
// 代码更复杂，面试推荐解法1。
class Solution2 {
public:
    vector<vector<int>> kSmallestPairs(vector<int>& nums1, vector<int>& nums2, int k) {
        vector<vector<int>> res;
        if (nums1.empty() || nums2.empty()) return res;

        int n = nums1.size(), m = nums2.size();

        // visited 防止重复入堆
        set<pair<int,int>> visited;

        auto cmp = [&](const pair<int,int>& a, const pair<int,int>& b) {
            return nums1[a.first] + nums2[a.second] >
                   nums1[b.first] + nums2[b.second];
        };
        priority_queue<pair<int,int>, vector<pair<int,int>>, decltype(cmp)> pq(cmp);

        pq.push({0, 0});
        visited.insert({0, 0});

        while (k-- > 0 && !pq.empty()) {
            auto [i, j] = pq.top();
            pq.pop();
            res.push_back({nums1[i], nums2[j]});

            // 推入 (i+1, j)
            if (i + 1 < n && !visited.count({i + 1, j})) {
                pq.push({i + 1, j});
                visited.insert({i + 1, j});
            }
            // 推入 (i, j+1)
            if (j + 1 < m && !visited.count({i, j + 1})) {
                pq.push({i, j + 1});
                visited.insert({i, j + 1});
            }
        }
        return res;
    }
};


// =====================================================
// 解法3: 暴力法 + 排序（仅供对比，大数据会超时）
// 时间 O(nm log(nm)), 空间 O(nm)
class Solution3 {
public:
    vector<vector<int>> kSmallestPairs(vector<int>& nums1, vector<int>& nums2, int k) {
        vector<pair<int, pair<int,int>>> all;
        for (int i = 0; i < (int)nums1.size(); i++) {
            for (int j = 0; j < (int)nums2.size(); j++) {
                all.push_back({nums1[i] + nums2[j], {nums1[i], nums2[j]}});
            }
        }
        sort(all.begin(), all.end());

        vector<vector<int>> res;
        for (int t = 0; t < min(k, (int)all.size()); t++) {
            res.push_back({all[t].second.first, all[t].second.second});
        }
        return res;
    }
};


/*
 * =====================================================
 * 解法对比
 * =====================================================
 *
 * | 解法     | 时间                  | 空间         | 特点                |
 * |----------|-----------------------|-------------|---------------------|
 * | Solution1| O(k * log min(n,k))   | O(min(n,k)) | 最优解, 无需visited  |
 * | Solution2| O(k * log k)          | O(k)        | 需要visited set     |
 * | Solution3| O(nm * log(nm))       | O(nm)       | 暴力, 大数据超时     |
 *
 * =====================================================
 * 易错点 (具体 bug)
 * =====================================================
 *
 * 1. 堆比较函数方向搞反:
 *    C++ priority_queue 默认大顶堆。
 *    错误: return get<0>(a) < get<0>(b);  // 大顶堆, 最大值先出
 *    正确: return get<0>(a) > get<0>(b);  // 小顶堆, 最小值先出
 *
 * 2. 初始化入堆个数不对:
 *    应该 min(n, k) 而非 n。n=10^5 且 k=3 时入堆10^5个是浪费。
 *
 * 3. 弹出后推进 i 而非 j (在解法1中):
 *    错误: pq.push({nums1[i+1]+nums2[j], i+1, j});
 *    正确: pq.push({nums1[i]+nums2[j+1], i, j+1});
 *    因为 (i+1, 0) 初始化时已入堆。推进 i 会导致重复。
 *
 * 4. j+1 越界:
 *    推入前必须检查 j+1 < m。否则 nums2[j+1] 越界。
 *
 * 5. 堆空了还 pop:
 *    while 条件必须包含 !pq.empty()。k 可能大于实际配对数。
 *
 * 6. sum 溢出:
 *    nums1[i], nums2[j] 各可达 10^9, 和最大 2*10^9, 不溢出 int。
 *    但如果数据范围更大, 需要 long long。
 *
 * =====================================================
 * 面试追问
 * =====================================================
 *
 * Q1: 和 "合并 K 个有序链表" 的关系?
 * A:  完全同构。nums1 的每个元素生成一条有序链表。
 *     堆做多路合并, 取前 k 个。
 *
 * Q2: nums1 和 nums2 不有序?
 * A:  先排序 O(n log n + m log m), 再用堆。
 *
 * Q3: 能否用二分搜索?
 * A:  可以。二分目标值 mid, O(n+m) 计算有多少对和 <= mid。
 *     找到使得恰好 k 对的 mid 值。时间 O((n+m)*log(range))。
 *
 * Q4: 堆大小?
 * A:  始终 <= min(n, k)。初始 min(n,k) 个, 弹1推最多1, 不增长。
 *
 * Q5: 和 LC378 的区别?
 * A:  378 找第 k 小的值, 本题找前 k 小的配对。
 *     本质相同, 但本题需要返回具体配对而非仅值。
 */
