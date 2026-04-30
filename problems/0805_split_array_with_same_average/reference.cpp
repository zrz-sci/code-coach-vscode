/*
 * LeetCode 805: Split Array With Same Average (数组的均值分割)
 *
 * 【题目本质】
 * 将数组分成两个非空子集 A 和 B，使得 avg(A) == avg(B)。
 * 数学等价：找大小为 k (1<=k<=n/2) 的子集使其和 = k * totalSum / n。
 * n <= 30 适合折半搜索 (Meet in the Middle)。
 *
 * 【解法总览】
 * 解法1: Meet in the Middle — O(2^(n/2)*n) / O(2^(n/2)) — 面试首选
 * 解法2: DP（子集和）       — O(n²*sum) / O(n*sum)      — 备选方案
 */

// ============================================================
// 解法1: Meet in the Middle（折半搜索） — 面试首选 ⭐
// 时间: O(2^(n/2) * n)  空间: O(2^(n/2))
//
// 【思路】
// 数学推导：
//   avg(A) == avg(B) == avg(全数组)
//   ⟹ sum(A) / |A| = totalSum / n
//   ⟹ sum(A) * n = totalSum * |A|
//
// 为避免浮点，令 a[i] = nums[i] * n - totalSum
// 则目标变为：找非空真子集使得 sum(a_subset) = 0
//
// 折半搜索：
//   n=30 → 2^30 ≈ 10^9 太大
//   但 2^15 ≈ 32768 可接受
//   将数组分成两半，分别枚举所有子集
//   在两半之间查找互补的 (count, sum) 对
//
// 示例: nums = [1,2,3,4,5,6,7,8], totalSum = 36, n = 8
//   标准化: a[i] = nums[i]*8 - 36
//   a = [-28, -20, -12, -4, 4, 12, 20, 28]
//   左半 a[0..3] = [-28,-20,-12,-4]
//   右半 a[4..7] = [4,12,20,28]
//   枚举左半子集，如 {-28,-4} sum=-32, cnt=2
//   枚举右半子集，如 {4,28} sum=32, cnt=2
//   total = 2+2 = 4 ∈ [1,7] ✓, leftSum + rightSum = 0 ✓
//   找到！返回 true
// ============================================================
class Solution1 {
public:
    bool splitArraySameAverage(vector<int>& nums) {
        int n = nums.size();
        if (n == 1) return false;

        // 计算总和
        int totalSum = 0;
        for (int x : nums) totalSum += x;

        // 标准化: a[i] = nums[i] * n - totalSum
        // 目标：找非空真子集使 sum = 0
        vector<int> a(n);
        for (int i = 0; i < n; i++) {
            a[i] = nums[i] * n - totalSum;
        }

        int half = n / 2;

        // 枚举左半 a[0..half-1] 的所有子集
        // left[cnt] = 选 cnt 个元素时所有可能的 sum 集合
        vector<unordered_set<int>> left(half + 1);
        for (int mask = 0; mask < (1 << half); mask++) {
            int s = 0, cnt = 0;
            for (int j = 0; j < half; j++) {
                if (mask & (1 << j)) {
                    s += a[j];
                    cnt++;
                }
            }
            left[cnt].insert(s);
        }

        // 枚举右半 a[half..n-1] 的所有子集
        int rLen = n - half;
        for (int mask = 0; mask < (1 << rLen); mask++) {
            int s = 0, cnt = 0;
            for (int j = 0; j < rLen; j++) {
                if (mask & (1 << j)) {
                    s += a[half + j];
                    cnt++;
                }
            }

            // 在左半中查找互补
            // 需要: leftSum = -s, leftCnt + cnt ∈ [1, n-1]
            for (int lc = 0; lc <= half; lc++) {
                int total = lc + cnt;
                if (total == 0 || total == n) continue; // 非空非全
                if (left[lc].count(-s)) return true;
            }
        }

        return false;
    }
};

// ============================================================
// 解法2: DP（子集和） — 备选方案
// 时间: O(n² * sum)  空间: O(n * sum)
//
// 【思路】
// dp[k] = 选 k 个元素可达到的所有 sum 的集合。
// 对于每个 num，逆序更新 dp[k] (类似01背包)。
// 最终检查是否存在 k ∈ [1, n/2] 使得
// dp[k] 包含 totalSum * k / n（需要是整数）。
//
// 剪枝：先检查是否存在 k 使得 totalSum * k % n == 0。
// 如果对所有 k 都不整除，直接返回 false。
//
// 为什么只检查 k ≤ n/2？
//   如果选 k 个元素的子集 A 满足条件，
//   那么剩余 n-k 个元素的子集 B 也满足条件。
//   所以只需要检查较小的一半。
// ============================================================
class Solution2 {
public:
    bool splitArraySameAverage(vector<int>& nums) {
        int n = nums.size();
        int totalSum = 0;
        for (int x : nums) totalSum += x;

        // 快速剪枝: 检查是否存在合法的 k
        bool possible = false;
        for (int k = 1; k <= n / 2; k++) {
            if ((long long)totalSum * k % n == 0) {
                possible = true;
                break;
            }
        }
        if (!possible) return false;

        // dp[k] = 选 k 个数可达到的 sum 集合
        vector<unordered_set<int>> dp(n / 2 + 1);
        dp[0].insert(0);

        for (int num : nums) {
            // 逆序更新，避免同一个 num 被选多次（01背包）
            for (int k = min((int)dp.size() - 1, n / 2); k >= 1; k--) {
                for (int prevSum : dp[k - 1]) {
                    dp[k].insert(prevSum + num);
                }
            }
        }

        // 检查是否有合法的 (k, sum) 对
        for (int k = 1; k <= n / 2; k++) {
            if ((long long)totalSum * k % n == 0) {
                int target = (int)((long long)totalSum * k / n);
                if (dp[k].count(target)) return true;
            }
        }
        return false;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法             | 时间            | 空间          | 优势                   | 劣势                |
// |-----------------|-----------------|---------------|----------------------|---------------------|
// | Meet in Middle  | O(2^(n/2) * n)  | O(2^(n/2))    | 更快，适合 n ≤ 30-40   | 实现稍复杂           |
// | DP 子集和        | O(n² * sum)     | O(n * sum)    | 逻辑直观               | sum 大时空间和时间都大 |
//
// 面试策略：先讲数学推导（avg 等价转化），再实现 Meet in the Middle。
// 如果面试官对折半搜索不熟悉，可以用 DP 解释得更清楚。
//
// ============================================================
// 【易错点】
//
// 1. 浮点精度:
//    ✗ 直接比较 double 类型的平均值 → 浮点误差导致判断错误
//    ✓ 转化为整数条件: sum(A) * n == totalSum * |A|
//
// 2. 允许空子集或全集:
//    ✗ 不检查 total ∈ [1, n-1] → 将空集或全集当成合法答案
//    ✓ 严格要求 1 ≤ total ≤ n-1 (两个子集都非空)
//
// 3. DP 正序更新:
//    ✗ for (int k = 1; ...) → 同一个 num 在同一轮被多次选取
//    ✓ for (int k = maxK; k >= 1; k--) → 01背包逆序保证每个 num 只选一次
//
// 4. 整除检查遗漏:
//    ✗ 不检查 totalSum * k % n == 0 就直接计算 target
//    ✓ 只有整除时 target 才是整数，不整除则跳过该 k
//
// ============================================================
// 【面试追问 — 递进链】
//
// Q1: 为什么 avg(A)==avg(B) 等价于 avg(A)==avg(全数组)?
//     avg(B) = (totalSum - sum(A)) / (n - |A|)。
//     avg(A) = sum(A) / |A|。
//     设 avg(A) = avg(B) = x，则 sum(A) = x*|A|, totalSum-sum(A) = x*(n-|A|)
//     ⟹ totalSum = x*n ⟹ x = totalSum/n = avg(全数组)。
//
// Q2: Meet in the Middle 适用于什么范围的 n？
//     20 ≤ n ≤ 40。n < 20 时暴力 2^n 即可；n > 40 时 2^(n/2) 也太大。
//     本题 n ≤ 30，2^15 = 32768，在可接受范围内。
//
// Q3: 能否用 bitset 替代 unordered_set 优化？
//     可以。DP 解法中 dp[k] 用 bitset 表示（每个 bit 代表一个 sum 值），
//     位运算做集合并（|=），常数因子降低约 64 倍。
//
// ============================================================
// 【相关题型】
//
// - 416. 分割等和子集:
//   判断能否将数组分成两个等和子集。本题的特殊情况（|A|和|B|不要求相等）。
//
// - 1755. 最接近目标值的子序列和:
//   Meet in the Middle 的直接应用。折半枚举+排序+双指针查找最近值。
//
// - 698. 划分为k个相等的子集:
//   分成 k 个等和子集，回溯+剪枝。比本题更复杂。
//
// - 494. 目标和:
//   子集和 DP 的变体（每个元素选 + 或 -），核心 DP 思路与本题相同。
// ============================================================
