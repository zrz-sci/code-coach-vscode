/*
 * LeetCode 3381: Maximum Subarray Sum With Length Divisible by K
 * (长度可被 K 整除的子数组的最大元素和)
 *
 * 【题目本质】
 * 在数组中找一个子数组，要求长度是 k 的倍数，且元素和最大。
 * 核心转化：前缀和差最大，且两个前缀和的索引模 k 同余。
 *
 * 【解法总览】
 * 解法1: 暴力枚举         — O(n^2/k) / O(n)  — 枚举所有 k 倍长度子数组
 * 解法2: 前缀和+同余分组  — O(n)     / O(k)  — 面试首选
 */


// ============================================================
// 解法1: 暴力枚举 — 枚举所有长度为 k 的倍数的子数组
// 时间: O(n^2/k)  空间: O(n)
//
// 【思路】
// 先计算前缀和数组，然后枚举所有 (i, j) 使得 (j - i) % k == 0。
// 这等价于：对每个起点 i，步长 k 枚举终点 j。
//
// 示例: nums = [-1,-2,-3,-4,-5], k = 4
//   prefix = [0, -1, -3, -6, -10, -15]
//   i=0: j=4, sum = prefix[4]-prefix[0] = -10-0 = -10
//   i=1: j=5, sum = prefix[5]-prefix[1] = -15-(-1) = -14
//   最大 = -10 ✓
//
// 瓶颈分析：
//   对每个起点 i，我们在所有满足 j%k == i%k 的 j 中遍历，
//   本质上在找 prefix[j] 最大值 - prefix[i]。
//   但如果反过来想：固定 j，找同余类中 prefix[i] 最小值，
//   就可以用一个数组维护 → 引出解法2
// ============================================================
class Solution1 {
public:
    long long maxSubarraySum(vector<int>& nums, int k) {
        int n = nums.size();
        // 计算前缀和
        vector<long long> prefix(n + 1, 0);
        for (int i = 0; i < n; i++) {
            prefix[i + 1] = prefix[i] + nums[i];
        }

        long long ans = LLONG_MIN;

        // 枚举所有 (i, j) 使得 (j - i) % k == 0 且 j > i
        for (int i = 0; i < n; i++) {
            for (int j = i + k; j <= n; j += k) {
                ans = max(ans, prefix[j] - prefix[i]);
            }
        }
        return ans;
    }
};

// ============================================================
// 解法2: 前缀和 + 同余分组 — 最优解 ⭐
// 时间: O(n)  空间: O(k)
//
// 【核心推导】
//
// 子数组 nums[l..r] 的和:
//   sum = prefix[r+1] - prefix[l]
//   长度 = r - l + 1 = (r+1) - l
//
// 令 j = r+1, i = l:
//   sum = prefix[j] - prefix[i]
//   len = j - i
//   要求: len % k == 0
//   即:   (j - i) % k == 0
//   即:   j % k == i % k        ← 同余条件!
//
// 问题转化:
//   对每个 j (1..n), 在所有满足 i%k == j%k 且 i < j 的 i 中,
//   找使得 prefix[j] - prefix[i] 最大的 i,
//   即找 prefix[i] 最小的 i.
//
// 维护: minPrefix[r] = 余数为 r 的位置中已见过的最小前缀和
//
// 【初始化】
//   prefix[0] = 0, 索引 0, 余数 0%k = 0
//   所以 minPrefix[0] = 0
//   其余余数类: minPrefix[r] = LLONG_MAX (尚无数据)
//
// 【遍历过程】 (先查后更新, 保证 i < j)
//   for j = 1 to n:
//     r = j % k
//     if minPrefix[r] != LLONG_MAX:
//       ans = max(ans, prefix[j] - minPrefix[r])
//     minPrefix[r] = min(minPrefix[r], prefix[j])
//
// 【示例详解】
//
// nums = [1, 2], k = 1
//
// prefix:    [0, 1, 3]
// 索引:       0  1  2
// 余数%1:     0  0  0    (模1全为0)
//
// 初始: minPrefix = [0]
//
// j=1 (prefix[1]=1):
//   r = 1%1 = 0
//   minPrefix[0] = 0 → ans = max(-INF, 1-0) = 1
//   更新 minPrefix[0] = min(0, 1) = 0
//
// j=2 (prefix[2]=3):
//   r = 2%1 = 0
//   minPrefix[0] = 0 → ans = max(1, 3-0) = 3
//   更新 minPrefix[0] = min(0, 3) = 0
//
// 答案: 3  ✓  (子数组 [1,2], 长度2, 2%1==0)
//
// ---
//
// nums = [-1,-2,-3,-4,-5], k = 4
//
// prefix:    [0, -1, -3, -6, -10, -15]
// 索引:       0   1   2   3    4    5
// 余数%4:     0   1   2   3    0    1
//
// 初始: minPrefix = [0, INF, INF, INF]
//
// j=1 (prefix=-1): r=1, minPrefix[1]=INF → skip
//   更新 minPrefix[1] = -1
//
// j=2 (prefix=-3): r=2, minPrefix[2]=INF → skip
//   更新 minPrefix[2] = -3
//
// j=3 (prefix=-6): r=3, minPrefix[3]=INF → skip
//   更新 minPrefix[3] = -6
//
// j=4 (prefix=-10): r=0, minPrefix[0]=0
//   ans = max(-INF, -10-0) = -10
//   更新 minPrefix[0] = min(0, -10) = -10
//
// j=5 (prefix=-15): r=1, minPrefix[1]=-1
//   ans = max(-10, -15-(-1)) = max(-10, -14) = -10
//   更新 minPrefix[1] = min(-1, -15) = -15
//
// 答案: -10  ✓  (子数组 [-1,-2,-3,-4], 长度4, 4%4==0)
//
// ---
//
// nums = [-1, 3, 2], k = 3
//
// prefix:    [0, -1, 2, 4]
// 索引:       0   1  2  3
// 余数%3:     0   1  2  0
//
// 初始: minPrefix = [0, INF, INF]
//
// j=1 (prefix=-1): r=1, skip → update minPrefix[1]=-1
// j=2 (prefix=2):  r=2, skip → update minPrefix[2]=2
// j=3 (prefix=4):  r=0, minPrefix[0]=0 → ans = 4-0 = 4
//   更新 minPrefix[0] = min(0, 4) = 0
//
// 答案: 4  ✓  (子数组 [-1,3,2], 长度3, 3%3==0)
// ============================================================
class Solution2 {
public:
    long long maxSubarraySum(vector<int>& nums, int k) {
        int n = nums.size();

        // minPrefix[r] = 余数为 r 的前缀和索引中，已见过的最小前缀和值
        // 初始化：prefix[0] = 0 在索引 0，余数为 0%k = 0
        vector<long long> minPrefix(k, LLONG_MAX);
        minPrefix[0] = 0;

        long long prefix = 0;    // 滚动计算前缀和 (等价于 prefix[j+1])
        long long ans = LLONG_MIN;

        for (int j = 0; j < n; j++) {
            prefix += nums[j];
            // 当前前缀和对应 prefix[j+1]，索引为 j+1
            int r = (j + 1) % k;

            // 先查：同余类中是否有可用的前缀和
            if (minPrefix[r] != LLONG_MAX) {
                ans = max(ans, prefix - minPrefix[r]);
            }

            // 后更新：将当前前缀和纳入同余类的最小值追踪
            minPrefix[r] = min(minPrefix[r], prefix);
        }

        return ans;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法             | 时间        | 空间  | 适用场景             |
// |-----------------|------------|-------|---------------------|
// | 暴力枚举         | O(n^2/k)   | O(n)  | n 小或 k 接近 n 时  |
// | 前缀和+同余分组   | O(n)       | O(k)  | 通用，最优           |
//
// 面试选择：先说暴力思路 + 前缀和基本框架，然后推导同余条件优化。
//
// ============================================================
// 【易错点】
//
// 1. 余数计算:
//    ✗ int r = j % k;         → prefix 实际对应 prefix[j+1]
//    ✓ int r = (j + 1) % k;   → 正确对应 prefix[j+1] 的索引
//
// 2. 初始化遗漏:
//    ✗ minPrefix 全部初始化为 LLONG_MAX
//      → prefix[0] = 0 是合法的起点，余数0组缺少初始值
//    ✓ minPrefix[0] = 0;
//
// 3. 先更新后查:
//    ✗ minPrefix[r] = min(minPrefix[r], prefix);
//      ans = max(ans, prefix - minPrefix[r]);
//      → prefix - prefix = 0, 等于空子数组
//    ✓ 先查 ans, 再更新 minPrefix[r]
//
// 4. 数据类型溢出:
//    ✗ int prefix = 0;
//      → n 最大 2*10^5, 元素最大 10^9, 前缀和可达 2*10^14
//    ✓ long long prefix = 0;
//
// 5. 答案初始化:
//    ✗ long long ans = 0;
//      → 所有合法子数组和为负时返回 0 (错误)
//    ✓ long long ans = LLONG_MIN;
//
// ============================================================
// 【面试追问 — 递进链】
//
// Q1: 为什么同余条件 j%k == i%k 保证长度被 k 整除?
//     因为 j - i = j%k - i%k + k*(j/k - i/k)。
//     当 j%k == i%k 时，j - i = k*(j/k - i/k)，是 k 的倍数。
//
// Q2: 如果改成"长度恰好为 k"?
//     滑动窗口：维护大小为 k 的窗口，O(n) 时间 O(1) 空间。
//     或者更简单：prefix[j] - prefix[j-k] 对每个 j >= k 取最大。
//
// Q3: 如果要返回具体的子数组区间?
//     在 minPrefix 中额外存储达到最小值时的索引。
//     ans 更新时记录 (minIdx, j-1) 作为答案子数组的范围。
//
// Q4: 这个"前缀和+同余"技巧还在哪些题出现?
//     - 523: 子数组和是 k 的倍数 (存在性判断)
//     - 974: 和可被 K 整除的子数组 (计数)
//     - 560: 和为 K 的子数组 (前缀和+哈希, 不用模但同一框架)
//
// ============================================================
// 【相关题型】
//
// - 523. 连续的子数组和:
//   判断是否存在长度>=2的子数组，和为 k 的倍数。
//   前缀和模 k，用 HashMap 记录每个余数第一次出现的位置。
//
// - 974. 和可被 K 整除的子数组:
//   统计和能被 k 整除的子数组个数。
//   前缀和模 k，同余的前缀和两两配对，用组合数 C(cnt, 2) 计数。
//
// - 560. 和为 K 的子数组:
//   前缀和 + HashMap 查 prefix[j] - k 是否出现过。
//   核心"前缀和差值"思想相同，只是约束从模运算变为精确匹配。
//
// - 862. 和至少为 K 的最短子数组:
//   prefix[j] - prefix[i] >= k，最小化 j-i。
//   前缀和 + 单调队列。不同约束，同一前缀和框架。
// ============================================================
