/*
 * LeetCode 410: 分割数组的最大值 (Split Array Largest Sum)
 *
 * 【题目本质】
 * 把数组分成 k 段连续子数组，使得"最大段和"最小。
 * 经典的"最小化最大值"问题，有 DP 和二分答案两条路线。
 *
 * 【解法总览】
 * 解法1: DP + 前缀和        — O(n²k) / O(nk) — 经典区间 DP 思路
 * 解法2: DP 空间优化         — O(n²k) / O(n)  — 滚动数组
 * 解法3: 二分答案 + 贪心验证 — O(n·logS) / O(1) — 面试首选 ⭐
 */

// ============================================================
// 解法1: DP + 前缀和
// 时间: O(n²k)  空间: O(nk)
//
// 【思路】
// 分割问题的自然思路：定义 dp[i][j] = 前 i 个元素分成 j 段时，
// 各段和最大值的最小值。
//
// 转移：枚举最后一段的起点 m+1，前 m 个元素分 j-1 段。
// dp[i][j] = min over m of max(dp[m][j-1], sum(m+1..i))
//
// 用前缀和 O(1) 计算区间和。
//
// 填表方向和依赖关系：
//        j=0  j=1   j=2
// i=0     0    ∞     ∞       base case: dp[0][0] = 0
// i=1     ∞   [7]    ∞       dp[1][1] = max(dp[0][0], sum[0..0]) = 7
// i=2     ∞   [7+2]  ∞       dp[2][1] = 9
// i=3     ∞   [14]   ∞       dp[3][1] = 14
// i=4     ∞   [24]  [14]     dp[4][2] = min(max(7,17), max(9,15), max(14,10)) = 14
// i=5     ∞   [32]  [18]     dp[5][2] = min(max(7,25), max(9,23), max(14,18), max(24,8)) = 18
//
// dp[i][j] 依赖于同一列 j-1 的所有行 → 按 j 从小到大、i 从小到大填表
// ============================================================
class Solution1 {
public:
    int splitArray(vector<int>& nums, int k) {
        int n = nums.size();
        
        // 前缀和：prefix[i] = nums[0] + ... + nums[i-1]
        vector<long long> prefix(n + 1, 0);
        for (int i = 0; i < n; i++) {
            prefix[i + 1] = prefix[i] + nums[i];
        }
        
        // dp[i][j] = 前i个元素分j段的最小"最大段和"
        vector<vector<long long>> dp(n + 1, vector<long long>(k + 1, LLONG_MAX));
        dp[0][0] = 0; // 0个元素分0段，合法且最大段和为0
        
        for (int i = 1; i <= n; i++) {
            // j 不能超过 i（每段至少一个元素），也不能超过 k
            for (int j = 1; j <= min(i, k); j++) {
                // 枚举最后一段 [m+1..i]
                // m >= j-1：前 m 个元素要能分成 j-1 段，至少需要 j-1 个元素
                for (int m = j - 1; m < i; m++) {
                    if (dp[m][j - 1] == LLONG_MAX) continue;
                    long long lastSegSum = prefix[i] - prefix[m];
                    // "最大段和"取 max，对所有切割方案取 min
                    dp[i][j] = min(dp[i][j], max(dp[m][j - 1], lastSegSum));
                }
            }
        }
        
        return (int)dp[n][k];
    }
};

// ============================================================
// 解法2: DP 空间优化（滚动数组）
// 时间: O(n²k)  空间: O(n)
//
// 【思路】
// dp[i][j] 只依赖 dp[*][j-1]，所以可以用两个一维数组 prev/curr
// 交替存储，把空间从 O(nk) 降到 O(n)。
// ============================================================
class Solution2 {
public:
    int splitArray(vector<int>& nums, int k) {
        int n = nums.size();
        vector<long long> prefix(n + 1, 0);
        for (int i = 0; i < n; i++)
            prefix[i + 1] = prefix[i] + nums[i];
        
        // prev[i] 对应 dp[i][j-1]
        vector<long long> prev(n + 1, LLONG_MAX);
        prev[0] = 0;
        
        vector<long long> curr(n + 1);
        
        for (int j = 1; j <= k; j++) {
            fill(curr.begin(), curr.end(), LLONG_MAX);
            for (int i = j; i <= n; i++) {
                for (int m = j - 1; m < i; m++) {
                    if (prev[m] == LLONG_MAX) continue;
                    long long lastSeg = prefix[i] - prefix[m];
                    curr[i] = min(curr[i], max(prev[m], lastSeg));
                }
            }
            swap(prev, curr);
        }
        
        return (int)prev[n];
    }
};

// ============================================================
// 解法3: 二分答案 + 贪心验证 ⭐ 面试首选
// 时间: O(n · log(sum - max))  空间: O(1)
//
// 【思路】
// "最小化最大值"→ 经典的二分答案信号。
//
// 关键洞察：答案（最大段和）有一个明确的范围 [max(nums), sum(nums)]，
// 且满足单调性：
//   - 允许的最大段和越大 → 需要的段数越少（越容易满足 ≤ k）
//   - 允许的最大段和越小 → 需要的段数越多（越难满足）
//
// 所以二分这个"允许的最大段和"，对每个 mid 贪心验证是否能分成 ≤ k 段。
//
// 贪心策略：从左到右，当前段能放就放（curSum + num ≤ mid），放不下就开新段。
// 这样做能最小化段数——因为提前切割只会导致后面需要更多段。
//
// 二分搜索空间收缩过程 (nums=[7,2,5,10,8], k=2):
//
//   left=10, right=32
//   Round1: mid=21, 贪心: [7,2,5](14) [10,8](18) → 2段 ≤ 2 ✓ → right=21
//   Round2: mid=15, 贪心: [7,2,5](14) [10](10) [8](8) → 3段 > 2 ✗ → left=16
//   Round3: mid=18, 贪心: [7,2,5](14) [10,8](18) → 2段 ≤ 2 ✓ → right=18
//   Round4: mid=17, 贪心: [7,2,5](14) [10](10) [8](8) → 3段 > 2 ✗ → left=18
//   left == right == 18 → 返回 18 ✓
// ============================================================
class Solution3 {
public:
    int splitArray(vector<int>& nums, int k) {
        // 搜索空间: [max(nums), sum(nums)]
        // left = max(nums)：每段至少要能容纳最大的那个元素
        // right = sum(nums)：全部放一段的情况
        long long left = *max_element(nums.begin(), nums.end());
        long long right = accumulate(nums.begin(), nums.end(), 0LL);
        
        // 二分模板: left < right，收缩到 left == right 即为答案
        while (left < right) {
            long long mid = left + (right - left) / 2;
            
            if (canSplit(nums, k, mid)) {
                // mid 可行（≤ k 段即可），尝试更小的上限
                right = mid;
            } else {
                // mid 不可行（需要 > k 段），上限要更大
                left = mid + 1;
            }
        }
        
        return (int)left;
    }

private:
    // 贪心验证: 每段和 ≤ maxSum 时，能否分成 ≤ k 段？
    bool canSplit(vector<int>& nums, int k, long long maxSum) {
        int segments = 1;   // 第一段自动开始（不是0！）
        long long curSum = 0;
        
        for (int num : nums) {
            if (curSum + num > maxSum) {
                // 当前段放不下 num，必须开新段
                segments++;
                curSum = num;   // num 是新段的第一个元素
                // 提前终止：已经超过 k 段，不用继续了
                if (segments > k) return false;
            } else {
                // 还能放进当前段
                curSum += num;
            }
        }
        
        return true; // segments <= k
    }
};

// ============================================================
// 【解法对比】
//
// | 解法      | 时间       | 空间   | 特点                          |
// |-----------|-----------|--------|-------------------------------|
// | DP        | O(n²k)   | O(nk)  | 能回溯具体分割方案            |
// | DP优化    | O(n²k)   | O(n)   | 空间更优但仍较慢              |
// | 二分+贪心 | O(n·logS) | O(1)   | 最快最简洁，面试首选 ⭐       |
//
// DP 的优势：可以回溯 dp 表得到具体分割方案。
// 二分的优势：代码量少、复杂度低、不受 k 大小影响。
//
// 【易错点】
// 1. 二分左边界设为 0 而不是 max(nums):
//    ✗ left = 0 → maxSum < max(nums) 时单个元素都放不下
//    ✓ left = *max_element(nums.begin(), nums.end())
//
// 2. 验证函数 segments 初始化为 0:
//    ✗ int segments = 0 → 最后一段漏计，结果偏小导致误判为可行
//    ✓ int segments = 1 → 第一段自动开始
//
// 3. DP 中 m 的下界:
//    ✗ for(int m = 0; m < i; m++) → 前0个元素分1段不合法
//    ✓ for(int m = j-1; m < i; m++) → 保证每段至少一个元素
//
// 4. 整数溢出:
//    ✗ int sum = accumulate(nums.begin(), nums.end(), 0)
//    ✓ long long sum = accumulate(nums.begin(), nums.end(), 0LL)
//
// 5. 二分 mid 的计算:
//    ✗ mid = (left + right) / 2 → left+right 可能溢出
//    ✓ mid = left + (right - left) / 2
//
// 【面试追问 — 递进链】
// Q1: 为什么能二分？单调性体现在哪？
//     → 最大段和允许越大，贪心分段数越少。存在一个临界点，
//       使得 ≥ 临界点都能分成 ≤ k 段。
//
// Q2: 如果要输出具体分割方案怎么办？
//     → 方案1: 用 DP 解法，记录转移路径回溯。
//       方案2: 二分找到答案 ans 后，再用 ans 做一次贪心扫描，
//       记录每次开新段的位置即可。
//
// Q3: 如果数组元素可以为负数，还能用二分吗？
//     → 不能！因为单调性被破坏：更大的 maxSum 不一定意味着
//       更少的段数（负数可能使区间和变小）。此时只能用 DP。
//
// Q4: 这和"在 D 天内送达包裹的能力"(LC 1011)有什么关系？
//     → 完全相同的模型：nums=包裹重量, k=天数, 答案=最小运力。
//       代码验证函数可以一字不改直接复用。
// ============================================================
