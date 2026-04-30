/*
 * LeetCode 870: 优势洗牌 (Advantage Shuffle)
 *
 * 难度: Medium
 * 标签: Greedy, Array, Two Pointers, Sorting
 *
 * 核心思想: 田忌赛马 -- 能赢就用最小的赢，赢不了就送最差的
 */



// ============================================================
// 解法1: 贪心 + Multiset
// 时间: O(n log n)  空间: O(n)
//
// 思路:
// - 将 nums1 全部放入 multiset
// - 对 nums2 的每个元素，用 upper_bound 找 nums1 中刚好大于它的最小值
// - 找到则分配（能赢就赢），找不到则分配最小值（送人头）
//
// 图解:
// nums2 = [13, 25, 32, 11]
// multiset = {8, 12, 24, 32}
//
// nums2[0]=13: upper_bound(13)=24  -> result[0]=24, set={8,12,32}
// nums2[1]=25: upper_bound(25)=32  -> result[1]=32, set={8,12}
// nums2[2]=32: upper_bound(32)=end -> result[2]=8(最小), set={12}
// nums2[3]=11: upper_bound(11)=12  -> result[3]=12, set={}
// 结果: [24, 32, 8, 12]
// ============================================================
class Solution_Multiset {
public:
    vector<int> advantageCount(vector<int>& nums1, vector<int>& nums2) {
        // 把 nums1 的所有元素放入 multiset（支持重复元素 + 有序）
        multiset<int> avail(nums1.begin(), nums1.end());
        int n = nums2.size();
        vector<int> result(n);

        for (int i = 0; i < n; i++) {
            // 在可用的牌中，找到刚好大于 nums2[i] 的最小值
            // upper_bound 返回第一个严格大于 nums2[i] 的迭代器
            auto it = avail.upper_bound(nums2[i]);

            if (it != avail.end()) {
                // 找到了能赢的牌 -> 用它
                result[i] = *it;
            } else {
                // 没有能赢的牌 -> 送最小的当炮灰，保留好牌
                it = avail.begin();
                result[i] = *it;
            }

            // 用掉这张牌
            // 注意: erase(iterator) 只删一个, erase(value) 会删所有重复的
            avail.erase(it);
        }

        return result;
    }
};

// ============================================================
// 解法2: 贪心 + 排序 + 双指针  ⭐ 面试首选
// 时间: O(n log n)  空间: O(n)
//
// 思路:
// - 对 nums2 的索引按值从大到小排序（先处理最难赢的对手）
// - 对 nums1 排序
// - 双指针 lo/hi 指向 nums1 两端:
//   能赢 -> 用 hi（最大的牌），hi--
//   赢不了 -> 送 lo（最小的牌），lo++
//
// 策略图解:
//   nums1 排序: [8, 12, 24, 32]
//                lo            hi
//   nums2 从大到小处理: 32 -> 25 -> 13 -> 11
//
//   32: nums1[hi]=32 不能赢(需要严格大于) -> 送 nums1[lo]=8, lo++
//   25: nums1[hi]=32 能赢   -> result[1]=32, hi--
//   13: nums1[hi]=24 能赢   -> result[0]=24, hi--
//   11: nums1[hi]=12 能赢   -> result[3]=12, hi--
//
//   结果: [24, 32, 8, 12]
// ============================================================
class Solution_TwoPointers {
public:
    vector<int> advantageCount(vector<int>& nums1, vector<int>& nums2) {
        int n = nums1.size();

        // Step 1: 对 nums2 的索引按值从大到小排序
        // 这样我们先处理 nums2 中最大的元素（最难赢的对手）
        vector<int> idx(n);
        iota(idx.begin(), idx.end(), 0); // idx = {0, 1, 2, ..., n-1}
        sort(idx.begin(), idx.end(), [&](int a, int b) {
            return nums2[a] > nums2[b]; // 按 nums2 值降序
        });

        // Step 2: 对 nums1 升序排序
        sort(nums1.begin(), nums1.end());

        // Step 3: 双指针分配
        vector<int> result(n);
        int lo = 0, hi = n - 1;

        for (int i : idx) {
            // 对于 nums2[i]（从大到小处理）
            if (nums1[hi] > nums2[i]) {
                // nums1 最大的牌能赢 -> 用它
                result[i] = nums1[hi--];
            } else {
                // nums1 最大的牌都赢不了 -> 送最小的当炮灰
                result[i] = nums1[lo++];
            }
        }

        return result;
    }
};

// ============================================================
// 解法3: 贪心 + Map 计数
// 时间: O(n log n)  空间: O(n)
//
// 思路:
// - 用 map<int,int> 维护 nums1 中每个值的可用数量
// - 对 nums2 每个值，在 map 中找 upper_bound
// - 找到就分配，减少计数；找不到就分配 map 中最小的
//
// 与解法1区别: 使用计数而非 multiset，适合有大量重复元素的场景
// ============================================================
class Solution_Map {
public:
    vector<int> advantageCount(vector<int>& nums1, vector<int>& nums2) {
        // 统计 nums1 中每个值的数量
        map<int, int> cnt;
        for (int x : nums1) cnt[x]++;

        int n = nums2.size();
        vector<int> result(n);

        for (int i = 0; i < n; i++) {
            // 找到第一个严格大于 nums2[i] 的键
            auto it = cnt.upper_bound(nums2[i]);

            if (it == cnt.end()) {
                // 没有能赢的 -> 取最小值当炮灰
                it = cnt.begin();
            }

            result[i] = it->first;

            // 减少计数，如果用完了就从 map 中删除
            if (--it->second == 0) {
                cnt.erase(it);
            }
        }

        return result;
    }
};

// ============================================================
// 解法对比分析
// ============================================================
/*
 * 三种解法对比:
 *
 * | 解法          | 时间        | 空间  | 优点                | 缺点                |
 * |---------------|-------------|-------|---------------------|---------------------|
 * | Multiset      | O(n log n)  | O(n)  | 代码最简洁          | multiset 常数大     |
 * | 排序+双指针   | O(n log n)  | O(n)  | 思路清晰，常数小    | 需要索引排序        |
 * | Map 计数      | O(n log n)  | O(n)  | 处理重复元素直观    | 代码略长            |
 *
 * 面试建议: 优先写解法2（排序+双指针），最容易解释田忌赛马策略。
 *
 * 核心贪心证明（交换论证）:
 *
 * 设 OPT 是一个最优解。对于位置 i，OPT 用 nums1[x] 去赢 nums2[i]，
 * 而我们的贪心用 nums1[y]（y 是 upper_bound 结果，即刚好赢的最小值）。
 *
 * 若 x != y，则 nums1[x] > nums1[y] > nums2[i]。
 * 设 nums1[y] 在 OPT 中被分配到位置 j。
 *
 * 交换 OPT 中位置 i 和 j 的分配:
 * - 位置 i: nums1[y] > nums2[i]（仍然赢）
 * - 位置 j: nums1[x] >= nums1[y]，赢的能力只可能更强
 *
 * 所以交换后优势数不减少。重复此过程可将 OPT 转化为贪心解，
 * 证明贪心解的优势数 >= OPT 的优势数。 QED.
 */

// ============================================================
// 面试 Follow-up
// ============================================================
/*
 * Q1: 如果要求严格相等 nums1[i] == nums2[i] 的次数最多？
 * A1: 把 upper_bound 改成 find / lower_bound，找精确匹配。
 *
 * Q2: 如果 nums1 和 nums2 长度不同？
 * A2: 如果 |nums1| > |nums2|，多出的元素不用；
 *     如果 |nums1| < |nums2|，某些位置无牌可出。
 *
 * Q3: 如果不要求严格大于，>= 也算赢？
 * A3: upper_bound -> lower_bound
 *
 * Q4: 时间复杂度能优于 O(n log n) 吗？
 * A4: 不能，因为需要排序；除非值域有限可以用计数排序 O(n + V)。
 *
 * Q5: 这题和经典的"任务分配"有什么关系？
 * A5: 本质是二分图最大匹配的贪心特例。因为"赢"的关系是有序的，
 *     贪心等价于匈牙利算法的结果。
 */

// ============================================================
// 使用 Solution_TwoPointers 作为默认解法提交
// ============================================================
class Solution {
public:
    vector<int> advantageCount(vector<int>& nums1, vector<int>& nums2) {
        int n = nums1.size();
        vector<int> idx(n);
        iota(idx.begin(), idx.end(), 0);
        sort(idx.begin(), idx.end(), [&](int a, int b) {
            return nums2[a] > nums2[b];
        });
        sort(nums1.begin(), nums1.end());

        vector<int> result(n);
        int lo = 0, hi = n - 1;
        for (int i : idx) {
            if (nums1[hi] > nums2[i]) {
                result[i] = nums1[hi--];
            } else {
                result[i] = nums1[lo++];
            }
        }
        return result;
    }
};

// ============================================================
// 总结区块
// ============================================================
/*
 * 【解法对比】
 * | 解法            | 时间        | 空间  | 特点                         |
 * |----------------|-------------|-------|------------------------------|
 * | Multiset(解法1) | O(n log n) | O(n)  | 代码最短，upper_bound 一行搞定 |
 * | 排序+双指针(解法2)| O(n log n)| O(n) | 面试首选，田忌赛马最直观        |
 * | Map计数(解法3)   | O(n log n) | O(n) | 重复元素多时更高效             |
 *
 * 面试首选: 解法2(排序+双指针)。
 * 从大到小处理对手 + 双指针分配，完美映射"田忌赛马"策略。
 *
 * 【易错点清单】
 * 1. 严格大于: upper_bound 而非 lower_bound
 *    → nums1[i] == nums2[i] 不算赢，必须严格大于
 * 2. multiset.erase(it) vs erase(value):
 *    erase(iterator) 只删一个，erase(value) 删所有重复
 *    → 必须用 erase(it)！
 * 3. 双指针方向: 先处理 nums2 最大的（最难赢的对手）
 *    → 如果从小到大处理，好牌被提前用在容易赢的对手上，浪费
 * 4. 索引排序 vs 值排序: nums2 不能直接排序（会丢失原始位置）
 *    → 必须用 idx 数组对索引排序
 * 5. 送人头策略: 赢不了时送最小的，不是送当前最大的
 *    → 保留大牌给后面可能赢的对手
 *
 * 【面试追问链】
 * Q1: 如何最大化 nums1[i] > nums2[i] 的个数？（本题）
 *     → 田忌赛马贪心：能赢用最小的赢，赢不了送最差的
 * Q2: 如果要最大化 sum(nums1[i] - nums2[i])（差值之和）而非赢的次数？
 *     → 贪心策略不同：对 nums1 和 nums2 都排序，
 *       最大配最大（匈牙利分配），不再需要"送人头"
 * Q3: 如果允许不分配（跳过某些位置），求最大赢局数？
 *     → 二分图最大匹配：建图 nums1[i] → nums2[j] 当
 *       nums1[i] > nums2[j]，求最大匹配（本题贪心等价）
 */
