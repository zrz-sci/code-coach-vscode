/*
 * LeetCode 1838: 最高频元素的频数 (Frequency of the Most Frequent Element)
 *
 * 【题目本质】
 * 排序后，在总操作次数 ≤ k 的约束下，找最长的连续子数组 [left, right]
 * 使得所有元素都可以增加到 nums[right]。
 * 代价公式: nums[right] * windowSize - windowSum ≤ k
 *
 * 【解法总览】
 * 解法1: 暴力枚举 — O(n²) / O(1) — 最直觉，帮助理解
 * 解法2: 排序 + 前缀和 + 二分查找 — O(n log n) / O(n) — 独立查询每个 right
 * 解法3: 排序 + 滑动窗口 — O(n log n) / O(1) — 最优，面试首选
 */

// ============================================================
// 解法1: 暴力枚举 — 排序后对每个 right 向左扩展
// 时间: O(n²)  空间: O(1) (不算排序)
//
// 【思路】
// 排序后，枚举每个元素作为目标值（right 端点），
// 从 right-1 向左逐个累加代价，直到超过 k 就停止。
// 为什么排序？排序后左边的元素更小，拉齐代价递增，
// 而且最优选择一定是连续的一段（不连续的代价只会更高）。
//
// 瓶颈：对每个 right 都要线性扫描，总计 O(n²)。
// ============================================================
class Solution1 {
public:
    int maxFrequency(vector<int>& nums, int k) {
        sort(nums.begin(), nums.end());
        int n = nums.size(), ans = 1;
        
        for (int right = 1; right < n; right++) {
            long long cost = 0;
            int count = 1; // nums[right] 本身
            for (int left = right - 1; left >= 0; left--) {
                // 把 nums[left] 增加到 nums[right] 需要的操作次数
                cost += (long long)(nums[right] - nums[left]);
                if (cost > k) break; // 预算不够，停止扩展
                count++;
            }
            ans = max(ans, count);
        }
        return ans;
    }
};

// ============================================================
// 解法2: 排序 + 前缀和 + 二分查找
// 时间: O(n log n)  空间: O(n)
//
// 【思路】
// 解法1的瓶颈：对每个 right 线性找最远的 left。
// 
// 关键观察：固定 right 时，left 越小（窗口越大），代价越高。
// 代价关于 left 是单调的！所以可以二分查找最小的 left 
// 使得代价 ≤ k。
//
// 代价 = nums[right] * (right - left + 1) - sum(nums[left..right])
// 用前缀和 O(1) 求区间和，二分 O(log n) 找 left，总计 O(n log n)。
//
// 二分搜索空间收缩示例 ([1,4,8,13], k=5, right=3):
//   搜索 left ∈ [0, 3]:
//   Round1: lo=0, hi=3, mid=1
//     window=[4,8,13], cost=13*3-25=14 > 5 → lo=2
//   Round2: lo=2, hi=3, mid=2
//     window=[8,13], cost=13*2-21=5 ≤ 5 → hi=2
//   lo==hi==2, 频数=3-2+1=2
// ============================================================
class Solution2 {
public:
    int maxFrequency(vector<int>& nums, int k) {
        sort(nums.begin(), nums.end());
        int n = nums.size();
        
        // 前缀和: prefix[i+1] = nums[0] + ... + nums[i]
        vector<long long> prefix(n + 1, 0);
        for (int i = 0; i < n; i++) {
            prefix[i + 1] = prefix[i] + nums[i];
        }
        
        int ans = 1;
        for (int right = 0; right < n; right++) {
            // 二分找最小的 left 使得代价 ≤ k
            int lo = 0, hi = right;
            while (lo < hi) {
                int mid = lo + (hi - lo) / 2;
                long long windowSize = right - mid + 1;
                long long windowSum = prefix[right + 1] - prefix[mid];
                long long cost = (long long)nums[right] * windowSize - windowSum;
                if (cost <= k) {
                    // mid 可行，尝试更远的 left（更大窗口）
                    hi = mid;
                } else {
                    // 代价太大，left 必须右移
                    lo = mid + 1;
                }
            }
            ans = max(ans, right - lo + 1);
        }
        return ans;
    }
};

// ============================================================
// 解法3: 排序 + 滑动窗口 ⭐ 面试首选
// 时间: O(n log n)  空间: O(1)
//
// 【思路】
// 解法2对每个 right 独立二分找 left。
// 但更好的观察：right 右移时，目标值变大（排序后），代价变大，
// left 也只会右移不会左移。这是经典的"同向双指针"滑动窗口！
//
// 维护窗口和 windowSum，每次 right 扩展时加入新元素，
// 当代价 > k 时左边界收缩（丢弃最小元素，它代价最高）。
//
// 滑动窗口过程 ([1,2,4], k=5):
//   right=0: sum=1, window=[1], cost=1*1-1=0 ≤ 5 → ans=1
//   right=1: sum=3, window=[1,2], cost=2*2-3=1 ≤ 5 → ans=2
//   right=2: sum=7, window=[1,2,4], cost=4*3-7=5 ≤ 5 → ans=3
//
// 滑动窗口过程 ([1,4,8,13], k=5):
//   right=0: sum=1, window=[1], cost=0 → ans=1
//   right=1: sum=5, window=[1,4], cost=3 → ans=2
//   right=2: sum=13, window=[1,4,8], cost=11>5
//     → 收缩: sum-=1, left=1, window=[4,8], cost=4 → ans=2
//   right=3: sum=25, window=[4,8,13], cost=14>5
//     → 收缩: sum-=4, left=2, window=[8,13], cost=5 → ans=2
// ============================================================
class Solution3 {
public:
    int maxFrequency(vector<int>& nums, int k) {
        sort(nums.begin(), nums.end());
        int n = nums.size(), ans = 1;
        long long windowSum = 0; // 必须 long long，避免溢出
        int left = 0;
        
        for (int right = 0; right < n; right++) {
            windowSum += nums[right];
            
            // 代价 = 把窗口所有元素拉到 nums[right] 的总操作次数
            // = nums[right] * windowSize - windowSum
            // 当代价 > k 时，收缩左边界
            while ((long long)nums[right] * (right - left + 1) - windowSum > k) {
                windowSum -= nums[left]; // 丢弃窗口最小元素
                left++;
            }
            
            ans = max(ans, right - left + 1);
        }
        return ans;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法   | 时间        | 空间   | 特点              |
// |--------|-------------|--------|-------------------|
// | 暴力   | O(n²)       | O(1)   | 直觉，帮助理解    |
// | 二分   | O(n log n)  | O(n)   | 独立查询每个right |
// | 滑窗   | O(n log n)  | O(1)   | 最优，代码最简    |
//
// 滑动窗口 vs 二分：
// - 滑动窗口利用了 left 单调右移的性质，总移动 O(n)
// - 二分对每个 right 独立查找，每次 O(log n)，总计 O(n log n)
// - 滑动窗口代码更短，面试首选
// - 当 left 不满足单调性时（如某些变体题），只能用二分
//
// 【易错点】
// 1. 整数溢出:
//    ✗ nums[right] * (right - left + 1)  — int * int 溢出
//    ✓ (long long)nums[right] * (right - left + 1)
//    原因: 10^5 * 10^5 = 10^10 > INT_MAX(≈2*10^9)
//
// 2. 忘记排序:
//    ✗ 直接滑窗 — 未排序时窗口内元素不连续，代价公式不成立
//    ✓ 必须先排序，保证窗口右端点是最大值
//
// 3. 滑窗收缩时忘记更新 windowSum:
//    ✗ left++ 但不减 windowSum — windowSum 越来越大，永远满足条件
//    ✓ 先 windowSum -= nums[left]，再 left++
//
// 4. 二分中 hi = mid vs hi = mid - 1:
//    ✗ hi = mid - 1 — 可能跳过最优 left
//    ✓ hi = mid — 因为 mid 满足条件时自身可能是答案
//    搭配 while (lo < hi) 使用
//
// 【面试追问】
// Q1(基础): 为什么排序后选连续子数组一定最优？
//   → 排序后，越靠近目标值的元素代价越小。跳过中间元素选更远的
//     只会增加总代价，所以最优选择一定是连续的。
//
// Q2(优化): 滑动窗口为什么 left 不会回退？
//   → right 右移后，nums[right] 变大（排序后），代价增加。
//     要维持 cost ≤ k，left 只能右移缩小窗口，不可能左移。
//     这保证了 left 和 right 都单调前进，总移动 O(n)。
//
// Q3(变体): 如果允许增加和减少操作呢？
//   → 目标值不再是窗口最大值，而是中位数（最小化绝对偏差之和）。
//     需要排序 + 滑动窗口 + 动态维护中位数（用两个堆）。
//     参考 LeetCode 462。
//
// Q4(扩展): 能否用"不收缩"的滑动窗口技巧？
//   → 是的！因为我们只关心最大窗口长度，窗口不需要真正收缩，
//     只需要"不缩小"。当代价 > k 时，left 和 right 同时右移一步
//     （窗口大小不变），直到找到更大的合法窗口。
//     代码改 while 为 if 即可，但面试中 while 版本更容易解释。
// ============================================================
