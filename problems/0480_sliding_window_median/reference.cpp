/*
 * LeetCode 480: 滑动窗口中位数 (Sliding Window Median)
 *
 * 【题目本质】
 * 在一个大小为 k 的滑动窗口中，每次删一个旧元素、加一个新元素，
 * 高效维护中位数。核心是"动态有序集合的中位数维护"。
 *
 * 【解法总览】
 * 解法1: 暴力排序           — O(nk log k) / O(k)  — 最直觉
 * 解法2: multiset + 迭代器  — O(n log k) / O(k)   — 面试首选
 * 解法3: 对顶堆 + 惰性删除  — O(n log k) / O(n)   — 进阶/追问
 */

// ============================================================
// 解法1: 暴力排序
// 时间: O(nk log k)  空间: O(k)
//
// 【思路】
// 最直觉的方法：每个窗口取出 k 个元素排序，取中间值。
// 窗口滑动 n-k+1 次，每次排序 O(k log k)。
// 瓶颈：每次完全重新排序，没利用上一个窗口的排序结果。
// ============================================================
class Solution1 {
public:
    vector<double> medianSlidingWindow(vector<int>& nums, int k) {
        vector<double> result;
        for (int i = 0; i + k <= (int)nums.size(); i++) {
            vector<int> window(nums.begin() + i, nums.begin() + i + k);
            sort(window.begin(), window.end());
            if (k % 2 == 1) {
                result.push_back(window[k / 2]);
            } else {
                // 用 double 防止两个 INT_MAX 相加溢出
                result.push_back(((double)window[k / 2 - 1] + window[k / 2]) / 2.0);
            }
        }
        return result;
    }
};

// ============================================================
// 解法2: multiset + 迭代器 — 面试首选
// 时间: O(n log k)  空间: O(k)
//
// 【思路】
// 解法1 的瓶颈是每次重新排序。窗口每次只变2个元素，
// 用 multiset（底层红黑树）维护有序集合：
//   插入 O(log k), 删除 O(log k), 迭代器定位中位数 O(1)
//
// 核心技巧：维护一个指向中位数的迭代器 mid。
// 每次插入/删除后，根据新元素和 *mid 的大小关系，
// 决定 mid 应该左移还是右移（最多移1步）。
//
// 示例: k=5, 窗口 = {-3, -1, 1, 3, 5}
//   multiset: [-3, -1, 1, 3, 5]
//                       ↑ mid
//
//   插入 0（< *mid=1）→ [-3, -1, 0, 1, 3, 5] → mid 左移
//                              ↑ mid
//   删除 -3（< *mid=0）→ [-1, 0, 1, 3, 5] → mid 右移
//                              ↑ mid (回到 1)
// ============================================================
class Solution2 {
public:
    vector<double> medianSlidingWindow(vector<int>& nums, int k) {
        multiset<int> window(nums.begin(), nums.begin() + k);
        // mid 指向第 (k-1)/2 个元素（0-indexed）
        // k=3: 指向第1个 → 中位数
        // k=4: 指向第1个 → 左中位数
        auto mid = next(window.begin(), (k - 1) / 2);

        vector<double> result;
        for (int i = k; ; i++) {
            // 计算中位数
            // k 奇数: next(mid, 0) = mid 本身, 结果 = (*mid + *mid) / 2 = *mid
            // k 偶数: next(mid, 1) 是右中位数, 结果 = (*mid + *next(mid)) / 2
            result.push_back(((double)(*mid) + *next(mid, k % 2 == 0)) / 2.0);

            if (i >= (int)nums.size()) break;

            // --- 插入新元素 nums[i] ---
            window.insert(nums[i]);
            // 新元素插在 mid 左边 → 左半部分多了一个 → mid 应左移
            if (nums[i] < *mid) --mid;

            // --- 删除旧元素 nums[i-k] ---
            // 旧元素在 mid 左边或就是 mid → 删除后 mid 应右移
            if (nums[i - k] <= *mid) ++mid;
            // 只删一个！不能用 erase(value) 会删所有相同值
            window.erase(window.lower_bound(nums[i - k]));
        }
        return result;
    }
};

// ============================================================
// 解法3: 对顶堆 + 惰性删除
// 时间: O(n log k)  空间: O(n) 最坏情况
//
// 【思路】
// 这是 LeetCode 295 "数据流中位数" 的扩展版。
// 295 题用对顶堆（大顶堆存小半、小顶堆存大半）维护中位数，
// 但只有 add 操作。本题窗口滑动需要 remove，而堆不支持按值删除。
//
// 解决办法：惰性删除（Lazy Deletion）
//   - 要删元素时，不真正从堆里删，而是在 delayed 哈希表里记一笔
//   - 只有当待删元素出现在堆顶时，才真正 pop 掉
//   - 用 smallSize/bigSize 追踪逻辑上的有效大小
//
// 对顶堆结构:
//   maxHeap (大顶堆)           minHeap (小顶堆)
//   存较小的 (k+1)/2 个        存较大的 k/2 个
//        3                          5
//       / \                        / \
//      1   2                      7   6
//   top = 3 (左半最大)        top = 5 (右半最小)
//   中位数 = 3 (k奇) 或 (3+5)/2 (k偶)
// ============================================================
class Solution3 {
public:
    vector<double> medianSlidingWindow(vector<int>& nums, int k) {
        // maxHeap: 存较小的一半（大顶堆）
        priority_queue<int> maxHeap;
        // minHeap: 存较大的一半（小顶堆）
        priority_queue<int, vector<int>, greater<int>> minHeap;
        // 惰性删除记录: delayed[val] = 还欠多少次真正删除
        unordered_map<int, int> delayed;

        // 逻辑上的有效大小
        int smallSize = 0, bigSize = 0;

        // 清理堆顶的已标记删除元素
        auto prune = [&](auto& heap) {
            while (!heap.empty() && delayed.count(heap.top()) && delayed[heap.top()] > 0) {
                delayed[heap.top()]--;
                if (delayed[heap.top()] == 0) delayed.erase(heap.top());
                heap.pop();
            }
        };

        // 平衡两堆: smallSize 目标 = (k+1)/2
        auto rebalance = [&]() {
            if (smallSize > (k + 1) / 2) {
                // maxHeap 多了，移堆顶到 minHeap
                minHeap.push(maxHeap.top());
                maxHeap.pop();
                smallSize--;
                bigSize++;
                prune(maxHeap); // 新堆顶可能是待删元素
            } else if (smallSize < (k + 1) / 2) {
                // minHeap 多了，移堆顶到 maxHeap
                maxHeap.push(minHeap.top());
                minHeap.pop();
                bigSize--;
                smallSize++;
                prune(minHeap);
            }
        };

        auto getMedian = [&]() -> double {
            if (k & 1) return maxHeap.top();
            return ((double)maxHeap.top() + minHeap.top()) / 2.0;
        };

        // 初始化: 排序前 k 个元素，分配到两个堆
        vector<int> init(nums.begin(), nums.begin() + k);
        sort(init.begin(), init.end());
        for (int i = 0; i < (k + 1) / 2; i++) {
            maxHeap.push(init[i]);
            smallSize++;
        }
        for (int i = (k + 1) / 2; i < k; i++) {
            minHeap.push(init[i]);
            bigSize++;
        }

        vector<double> result;
        result.push_back(getMedian());

        for (int i = k; i < (int)nums.size(); i++) {
            int add = nums[i];
            int del = nums[i - k];

            // 步骤1: 惰性删除 del
            delayed[del]++;
            if (del <= maxHeap.top()) {
                // del 属于 maxHeap（较小一半）
                smallSize--;
            } else {
                // del 属于 minHeap（较大一半）
                bigSize--;
            }
            // 如果堆顶恰好是 del，立即清理
            prune(maxHeap);
            prune(minHeap);

            // 步骤2: 插入 add
            if (!maxHeap.empty() && add <= maxHeap.top()) {
                maxHeap.push(add);
                smallSize++;
            } else {
                minHeap.push(add);
                bigSize++;
            }

            // 步骤3: 平衡 + 清理
            rebalance();
            prune(maxHeap);
            prune(minHeap);

            result.push_back(getMedian());
        }
        return result;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法       | 时间        | 空间  | 核心难点              | 适用场景        |
// |------------|-------------|-------|-----------------------|-----------------|
// | 暴力排序   | O(nk log k) | O(k)  | 无                    | 暖场/小 k       |
// | multiset   | O(n log k)  | O(k)  | 迭代器移动逻辑        | ⭐面试首选      |
// | 对顶堆     | O(n log k)  | O(n)  | 惰性删除 + 平衡维护   | 追问295扩展时用 |
//
// multiset 写法简洁、空间更优，面试中推荐优先写这个。
// 对顶堆展示更底层的数据结构设计能力，是进阶加分项。
//
// ============================================================
// 【易错点】
//
// 1. multiset 的 erase 陷阱:
//    ✗ window.erase(nums[i-k])       → 删除所有等于该值的元素！
//    ✓ window.erase(window.lower_bound(nums[i-k])) → 只删一个
//
// 2. int 溢出:
//    ✗ (*mid + *next(mid)) / 2.0     → 两个 INT_MAX 相加溢出 int
//    ✓ ((double)(*mid) + *next(mid)) / 2.0
//
// 3. multiset 迭代器调整顺序:
//    必须先插入并调 mid，再删除并调 mid。
//    如果先删除，mid 可能指向被删元素导致未定义行为。
//
// 4. 对顶堆: rebalance 后必须再 prune:
//    rebalance 把一个堆的堆顶移到另一个堆后，
//    源堆的新堆顶可能是待删元素，不 prune 会导致 getMedian 取到脏数据。
//
// 5. 对顶堆: del 归属判断用 <= maxHeap.top():
//    ✗ del < maxHeap.top()  → 等于 maxHeap.top() 的元素被归到 minHeap
//    ✓ del <= maxHeap.top() → 等于堆顶的属于 maxHeap 这边
//
// ============================================================
// 【面试追问】
//
// Q1: 暴力解的瓶颈？怎么利用上一个窗口的信息？
//     → 每次重新排序 O(k log k)。只变2个元素，用有序结构
//       维护，插入删除 O(log k)。
//
// Q2: multiset 方案中 mid 迭代器为什么不会失效？
//     → multiset 底层红黑树，插入/删除不影响其他迭代器。
//       但删除前必须确保 mid 不指向要删的元素（先 ++mid 再删）。
//
// Q3: 如果改成"滑动窗口第 K 大"怎么做？
//     → multiset: 维护指向第 K 大的迭代器。
//       对顶堆: 调整两堆的目标 size 比例。
//       也可用 GNU pbds 的 order-statistic tree。
//
// Q4: 对顶堆的惰性删除最坏空间 O(n)，能优化吗？
//     → 这是惰性删除的固有代价。multiset 方案不存在此问题，
//       所以 multiset 在空间上更优。
// ============================================================
