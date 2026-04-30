/*
 * LeetCode 3013: 将数组分成最小总代价的子数组 II
 *
 * 【题目】将 nums 分成 k 个连续子数组，代价 = 每个子数组第一个元素之和。
 *        约束: 第2个到第k个子数组的起始索引之差 <= dist。
 *        求最小总代价。
 *
 * 【问题转化】
 * nums[0] 固定作为第一个子数组代价。
 * 在 nums[1..n-1] 的滑动窗口 [i, i+dist] 内选 k-1 个最小元素。
 * 答案 = nums[0] + 窗口内前 k-1 小元素的和的最小值。
 *
 * 【解法1】滑动窗口 + 双 multiset — O(n log n) / O(n) ⭐ 首选
 * 【解法2】滑动窗口 + 懒删除堆 — O(n log n) / O(n) 替代方案
 */

// ============================================================
// 解法1: 滑动窗口 + 双 multiset ⭐ 面试首选
// ============================================================
/*
 * 数据结构:
 *   small (multiset): 窗口内最小的 k-1 个元素 (分割点候选)
 *   large (multiset): 窗口内其余元素 (候补)
 *   sumSmall (long long): small 的元素总和
 *
 * 不变量: |small| == need (= k-1), small.rbegin <= large.begin
 *
 * 操作:
 *   addToWindow(val):
 *     val <= small最大? → 放 small, 超出移到 large
 *     val > small最大?  → 放 large
 *
 *   removeFromWindow(val):
 *     val 在 small? → 删 small, 从 large 补一个最小的
 *     val 在 large? → 直接删
 *
 * 示例: nums=[1,3,2,6,4,2], k=3, dist=3
 *   need=2, 初始窗口 [1,4]={3,2,6,4}
 *     small={2,3} sum=5, large={4,6}
 *     ans = 1+5 = 6
 *   滑动: +nums[5]=2, -nums[1]=3
 *     add(2): 2<=3 → small={2,2,3} sum=7
 *       size>2 → move 3 to large: small={2,2} sum=4, large={3,4,6}
 *     remove(3): 3 在 large → large={4,6}
 *     ans = min(6, 1+4) = 5
 */
class Solution {
public:
    long long minimumCost(vector<int>& nums, int k, int dist) {
        int n = nums.size();
        int need = k - 1; // 需要选 need 个分割点

        multiset<int> small, large;
        long long sumSmall = 0;

        // ---- addToWindow: 将新元素加入窗口 ----
        // 决定放入 small 还是 large, 并维护 |small| == need
        auto addToWindow = [&](int val) {
            if ((int)small.size() < need || val <= *small.rbegin()) {
                // val 属于前 k-1 小, 放入 small
                small.insert(val);
                sumSmall += val;
                // small 过满, 最大的移到 large
                if ((int)small.size() > need) {
                    int moved = *small.rbegin();
                    // 注意: erase(find(val)) 只删一个, erase(val) 删所有!
                    small.erase(small.find(moved));
                    sumSmall -= moved;
                    large.insert(moved);
                }
            } else {
                // val 较大, 放入 large
                large.insert(val);
            }
        };

        // ---- removeFromWindow: 将离开窗口的元素移除 ----
        auto removeFromWindow = [&](int val) {
            auto it = small.find(val);
            if (it != small.end()) {
                // val 在 small 中
                small.erase(it);
                sumSmall -= val;
                // small 不足, 从 large 补最小的
                if (!large.empty() && (int)small.size() < need) {
                    int moved = *large.begin();
                    large.erase(large.begin());
                    small.insert(moved);
                    sumSmall += moved;
                }
            } else {
                // val 在 large 中
                large.erase(large.find(val));
            }
        };

        // ---- 初始化第一个窗口 [1, 1+dist] ----
        for (int i = 1; i <= 1 + dist && i < n; i++) {
            addToWindow(nums[i]);
        }

        long long ans = (long long)nums[0] + sumSmall;

        // ---- 滑动窗口 ----
        // 每次: 右端加入 nums[i], 左端移除 nums[i - dist - 1]
        for (int i = 2 + dist; i < n; i++) {
            addToWindow(nums[i]);                 // 新元素进入窗口
            removeFromWindow(nums[i - dist - 1]); // 旧元素离开窗口
            ans = min(ans, (long long)nums[0] + sumSmall);
        }

        return ans;
    }
};

// ============================================================
// 解法2: 滑动窗口 + 懒删除堆 (替代方案, 用于不支持 multiset 的语言)
// ============================================================
/*
 * 思路: 用两个堆模拟 small (大顶堆) 和 large (小顶堆)。
 * 堆不支持删除任意元素, 所以用"懒删除": 标记要删的元素,
 * 取堆顶时跳过已标记的。
 *
 * 注意: 懒删除堆的实现较复杂, 面试中推荐 multiset。
 * 此处仅展示思路框架。
 */

class Solution2 {
public:
    long long minimumCost(vector<int>& nums, int k, int dist) {
        int n = nums.size();
        int need = k - 1;

        // small: 大顶堆 (存前 need 小的元素, 堆顶是其中最大的)
        priority_queue<int> small;
        // large: 小顶堆 (存其余元素, 堆顶是其中最小的)
        priority_queue<int, vector<int>, greater<int>> large;

        // 懒删除计数器
        unordered_map<int, int> delCount;
        long long sumSmall = 0;
        int smallValid = 0; // small 中有效元素个数

        // 清理堆顶的已删除元素
        auto purgeSmall = [&]() {
            while (!small.empty() && delCount[small.top()] > 0) {
                delCount[small.top()]--;
                if (delCount[small.top()] == 0) delCount.erase(small.top());
                small.pop();
            }
        };
        auto purgeLarge = [&]() {
            while (!large.empty() && delCount[large.top()] > 0) {
                delCount[large.top()]--;
                if (delCount[large.top()] == 0) delCount.erase(large.top());
                large.pop();
            }
        };

        // small 过多 → 移到 large
        auto rebalance = [&]() {
            while (smallValid > need) {
                purgeSmall();
                int moved = small.top(); small.pop();
                sumSmall -= moved;
                smallValid--;
                large.push(moved);
            }
            while (smallValid < need) {
                purgeLarge();
                if (large.empty()) break;
                int moved = large.top(); large.pop();
                sumSmall += moved;
                smallValid++;
                small.push(moved);
            }
        };

        // 添加元素
        auto add = [&](int val) {
            purgeSmall();
            if (smallValid < need || (!small.empty() && val <= small.top())) {
                small.push(val);
                sumSmall += val;
                smallValid++;
            } else {
                large.push(val);
            }
            rebalance();
        };

        // 懒删除元素
        auto remove = [&](int val) {
            purgeSmall();
            if (!small.empty() && val <= small.top()) {
                // val 在 small 中
                delCount[val]++;
                sumSmall -= val;
                smallValid--;
            } else {
                // val 在 large 中
                delCount[val]++;
            }
            rebalance();
        };

        // 初始化窗口
        for (int i = 1; i <= 1 + dist && i < n; i++) {
            add(nums[i]);
        }

        long long ans = (long long)nums[0] + sumSmall;

        // 滑动
        for (int i = 2 + dist; i < n; i++) {
            add(nums[i]);
            remove(nums[i - dist - 1]);
            ans = min(ans, (long long)nums[0] + sumSmall);
        }

        return ans;
    }
};


// ============================================================
// 【解法对比】
//
// | 维度         | 解法1 双 multiset ⭐   | 解法2 懒删除堆          |
// |-------------|----------------------|------------------------|
// | 时间         | O(n log n)           | O(n log n)             |
// | 空间         | O(n)                 | O(n)                   |
// | 代码复杂度   | 中等                  | 高 (purge + rebalance)  |
// | 语言限制     | C++/Java TreeMap      | 任何语言               |
// | 面试推荐     | ⭐ 首选               | 追问/不支持 set 时备选   |
//
// 核心思想都是: 滑动窗口 + 维护窗口内前 k-1 小元素之和。
// multiset 天然支持 O(log n) 插入/删除/查找最值, 代码最简洁。
// 懒删除堆在 Python/Go 等无 multiset 的语言中有用。
//
// ============================================================
// 【易错点】
//
// 1. erase 全删 vs 删一个:
//    ✗ small.erase(val)   → 删除所有等于 val 的元素!
//    ✓ small.erase(small.find(val))  → 只删一个
//    multiset 中 erase(value) 删所有, erase(iterator) 删一个。
//
// 2. 窗口左端移除时机:
//    ✗ 先移除再加入 → small 可能临时为空, rbegin 段错误
//    ✓ 先加入再移除, 或 add/remove 内部检查 small 是否为空
//
// 3. 初始窗口范围:
//    ✗ [0, dist]  → nums[0] 是固定代价, 不参与选择
//    ✓ [1, 1+dist] → 分割点候选从 index 1 开始
//
// 4. long long 溢出:
//    ✗ nums[0] + sumSmall 用 int → 可能溢出
//    ✓ (long long)nums[0] + sumSmall
//
// 5. 懒删除堆中有效计数不同步:
//    标记删除时必须同步更新 smallValid/sumSmall,
//    否则 rebalance 逻辑出错。
//
// ============================================================
// 【面试追问】
//
// Q1: 为什么是"滑动窗口选前 k-1 小"?
//     第一个子数组代价固定为 nums[0]。剩余 k-1 个分割点
//     的起始索引必须在连续 dist+1 个位置的窗口内。
//     最小化代价 = 最小化窗口内 k-1 个最小元素之和。
//
// Q2: 不用两个 multiset, 用一个 set + nth_element 行不行?
//     不行。nth_element 是 O(n) 的无序操作, 不支持增量维护。
//     每次滑动重新 nth_element 是 O(n·dist), 总共 O(n²)。
//     双 multiset 方案每步 O(log n), 总 O(n log n)。
//
// Q3: 如果 k 很大 (接近 n), 有什么优化?
//     当 k-1 接近窗口大小时, large 集合很小,
//     可以反过来维护 "窗口内最大的 (窗口大小 - k + 1) 个元素"
//     并从总和中减去, 减少 set 操作次数。
//
// ============================================================
// 【相关题型】
//
// - 480. Sliding Window Median: 双 multiset 滑动窗口经典题
// - 295. Find Median from Data Stream: 双堆维护中位数
// - 239. Sliding Window Maximum: 单调队列滑动窗口
// - 2163. Minimum Difference in Sums After Removal of Elements: 前后缀+堆
// ============================================================
