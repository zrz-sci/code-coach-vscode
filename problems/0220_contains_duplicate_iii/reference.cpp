/*
 * 【题目本质】
 * 在滑动窗口（大小 indexDiff）内找两个元素值差 <= valueDiff。
 * 双约束搜索：下标距离约束 + 值差约束。
 *
 * 【解法总览】
 * 解法1: 桶排序 ⭐ — O(N) 时间，O(indexDiff) 空间
 * 解法2: 有序集合 (set + lower_bound) — O(N log indexDiff) 时间
 */

// ===================== 解法1: 桶排序 (最优) =====================
// 思路：
// 1. 桶宽度 w = valueDiff + 1，同桶内元素差一定 <= valueDiff
// 2. 相邻桶的元素需要额外检查差值
// 3. 每个桶最多存一个元素（有两个就直接返回 true）
// 4. 滑动窗口维护桶，窗口大小为 indexDiff
//
// 注意：负数桶号需要特殊处理（C++ 负数除法向零取整）
class Solution1 {
public:
    bool containsNearbyAlmostDuplicate(vector<int>& nums, int indexDiff, int valueDiff) {
        int n = nums.size();
        unordered_map<long long, long long> buckets;
        long long w = (long long)valueDiff + 1;  // 桶宽度，+1 避免除零

        for (int i = 0; i < n; i++) {
            long long val = (long long)nums[i];

            // 计算桶号（负数特殊处理）
            // 正数: 0,1,...,w-1 → 桶0; w,...,2w-1 → 桶1
            // 负数: -1,...,-w → 桶-1; -w-1,...,-2w → 桶-2
            long long bucket = val >= 0 ? val / w : (val + 1) / w - 1;

            // 检查1: 同桶（差值一定 <= valueDiff）
            if (buckets.count(bucket)) return true;
            // 检查2: 左邻桶
            if (buckets.count(bucket - 1) && val - buckets[bucket - 1] <= valueDiff)
                return true;
            // 检查3: 右邻桶
            if (buckets.count(bucket + 1) && buckets[bucket + 1] - val <= valueDiff)
                return true;

            // 当前元素入桶
            buckets[bucket] = val;

            // 滑出窗口：删除 nums[i - indexDiff] 所在的桶
            if (i >= indexDiff) {
                long long old_val = (long long)nums[i - indexDiff];
                long long old_bucket = old_val >= 0 ? old_val / w : (old_val + 1) / w - 1;
                buckets.erase(old_bucket);
            }
        }
        return false;
    }
};

// ===================== 解法2: 有序集合 (set + lower_bound) =====================
// 思路：
// 1. 用 set 维护大小为 indexDiff 的滑动窗口
// 2. 对当前元素 x，查找 set 中 >= x - valueDiff 的最小元素
// 3. 如果该元素 <= x + valueDiff，则找到满足条件的配对
// 4. 窗口超过 indexDiff 时删除最老的元素
//
// 关键：lower_bound 保证找到的是最接近 x 的候选
class Solution2 {
public:
    bool containsNearbyAlmostDuplicate(vector<int>& nums, int indexDiff, int valueDiff) {
        set<long long> window;

        for (int i = 0; i < (int)nums.size(); i++) {
            long long val = nums[i];

            // 在有序集合中查找 >= val - valueDiff 的最小元素
            auto it = window.lower_bound(val - valueDiff);

            // 检查是否 <= val + valueDiff（即差值在 valueDiff 范围内）
            if (it != window.end() && *it <= val + valueDiff) {
                return true;
            }

            window.insert(val);

            // 维护窗口大小
            if (i >= indexDiff) {
                window.erase((long long)nums[i - indexDiff]);
            }
        }
        return false;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 优缺点 |
 * |------|------|------|--------|
 * | 桶排序 ⭐ | O(N) | O(indexDiff) | 最优，负数桶号处理是难点 |
 * | 有序集合 | O(NlogK) | O(indexDiff) | 直观易理解，面试推荐 |
 *
 * 【易错点】
 * 1. 必须用 long long！nums[i] 范围 [-10^9, 10^9]，差值运算溢出 int
 * 2. 桶宽度 = valueDiff + 1（不是 valueDiff），避免 valueDiff=0 时除零
 * 3. 负数桶号计算：C++ 中 -1/3 = 0（向零取整），但我们需要 -1 → 桶-1
 *    公式：val >= 0 ? val/w : (val+1)/w - 1
 * 4. set 中存的是 long long，erase 时也要转 long long
 *
 * 【面试追问】
 * Q1: 为什么同桶最多一个元素？ → 有两个就差 <= valueDiff，已返回 true
 * Q2: 能否用 multiset？ → 可以，但 set 够用（重复值差=0<=valueDiff 直接匹配）
 * Q3: 推广到二维找 <=d 的点对？ → 网格桶 d*d，检查周围9格
 */
