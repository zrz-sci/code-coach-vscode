/*
 * LeetCode 891: Sum of Subsequence Widths (子序列宽度之和)
 *
 * 【题目本质】
 * 排序 + 贡献度计算：子序列宽度只取决于 max 和 min，
 * 排序后按元素计算每个值作为 max/min 的贡献。
 *
 * 【解法总览】
 * 解法1: 暴力枚举            — O(2^n * n) / O(1)     — TLE
 * 解法2: 排序 + 贡献度       — O(n log n) / O(n)      — 面试首选
 * 解法3: 排序 + 配对优化     — O(n log n) / O(n)      — 对称简化
 */

// ============================================================
// 解法1: 暴力枚举 — 枚举所有子序列 [TLE]
// 时间: O(2^n * n)  空间: O(1)
//
// 【思路】
// 用 bitmask 枚举所有非空子序列，
// 对每个子序列找 max 和 min，累加 width = max - min。
//
// n 最大 10^5，2^n 完全不可行。
// 但这个思路帮助理解: width 只和 max/min 有关。
//
// 示例: nums = [2,1,3]
//   子序列 [1]:       width = 0
//   子序列 [2]:       width = 0
//   子序列 [3]:       width = 0
//   子序列 [1,2]:     width = 1
//   子序列 [1,3]:     width = 2
//   子序列 [2,3]:     width = 1
//   子序列 [1,2,3]:   width = 2
//   总和 = 0+0+0+1+2+1+2 = 6
// ============================================================
class Solution1 {
public:
    int sumSubseqWidths(vector<int>& nums) {
        int n = nums.size();
        long long MOD = 1e9 + 7;
        long long ans = 0;

        // 枚举所有非空子序列
        for (int mask = 1; mask < (1 << n); mask++) {
            int maxVal = INT_MIN, minVal = INT_MAX;
            for (int i = 0; i < n; i++) {
                if (mask & (1 << i)) {
                    maxVal = max(maxVal, nums[i]);
                    minVal = min(minVal, nums[i]);
                }
            }
            ans = (ans + maxVal - minVal) % MOD;
        }

        return (int)ans;
    }
};

// ============================================================
// 解法2: 排序 + 贡献度 — 按元素计算贡献（⭐ 面试首选）
// 时间: O(n log n)  空间: O(n)
//
// 【思路】
// 关键洞察: 子序列的 width = max - min 只取决于值，不取决于位置。
// 所以排序不影响结果。
//
// 排序后 nums[0] <= nums[1] <= ... <= nums[n-1]
//
// 换一个角度: 不按子序列枚举，而是按元素计算贡献。
//
// nums[i] 作为最大值:
//   子序列所有元素 <= nums[i]，即从 nums[0..i] 中选
//   必须包含 nums[i]，其余 i 个元素任选 → 2^i 个子序列
//   贡献: +nums[i] * 2^i
//
// nums[i] 作为最小值:
//   子序列所有元素 >= nums[i]，即从 nums[i..n-1] 中选
//   必须包含 nums[i]，其余 n-1-i 个元素任选 → 2^(n-1-i) 个子序列
//   贡献: -nums[i] * 2^(n-1-i)
//
// 总贡献:
//   ans = Σ nums[i] * (2^i - 2^(n-1-i))
//
// 具体推导示例: nums = [1, 2, 3] (已排序)
//
//   i=0 (nums[0]=1):
//     作为 max: 2^0 = 1 次 → 只有子序列 {1}
//     作为 min: 2^2 = 4 次 → {1}, {1,2}, {1,3}, {1,2,3}
//     贡献: 1 * (1 - 4) = -3
//
//   i=1 (nums[1]=2):
//     作为 max: 2^1 = 2 次 → {2}, {1,2}
//     作为 min: 2^1 = 2 次 → {2}, {2,3}
//     贡献: 2 * (2 - 2) = 0
//
//   i=2 (nums[2]=3):
//     作为 max: 2^2 = 4 次 → {3}, {1,3}, {2,3}, {1,2,3}
//     作为 min: 2^0 = 1 次 → 只有子序列 {3}
//     贡献: 3 * (4 - 1) = 9
//
//   总和 = -3 + 0 + 9 = 6 ✓
// ============================================================
class Solution2 {
public:
    int sumSubseqWidths(vector<int>& nums) {
        long long MOD = 1e9 + 7;
        int n = nums.size();

        // 排序: 子序列宽度只看 max/min，和顺序无关
        sort(nums.begin(), nums.end());

        // 预处理 2 的幂次方 (mod)
        vector<long long> pow2(n);
        pow2[0] = 1;
        for (int i = 1; i < n; i++) {
            pow2[i] = pow2[i - 1] * 2 % MOD;
        }

        long long ans = 0;
        for (int i = 0; i < n; i++) {
            // 作为最大值的贡献: +nums[i] * 2^i
            // 作为最小值的贡献: -nums[i] * 2^(n-1-i)
            // 净贡献: nums[i] * (2^i - 2^(n-1-i))
            long long coeff = (pow2[i] - pow2[n - 1 - i] + MOD) % MOD;
            long long contribution = (long long)nums[i] % MOD * coeff % MOD;
            ans = (ans + contribution) % MOD;
        }

        return (int)ans;
    }
};

// ============================================================
// 解法3: 排序 + 配对优化 — 利用对称性
// 时间: O(n log n)  空间: O(n)
//
// 【思路】
// 解法2 中 nums[i] 的系数是 (2^i - 2^(n-1-i))，
//         nums[n-1-i] 的系数是 (2^(n-1-i) - 2^i)。
//
// 两者符号相反！可以配对:
//   nums[i] * (2^i - 2^(n-1-i)) + nums[n-1-i] * (2^(n-1-i) - 2^i)
//   = (nums[i] - nums[n-1-i]) * (2^i - 2^(n-1-i))
//
// 更简单的配对方式:
//   对于每个 i，计算 (nums[i] - nums[n-1-i]) * 2^i
//   遍历 i = 0 到 n-1，每一对 (i, n-1-i) 只需一个 2^i
//
// 为什么 (nums[i] - nums[n-1-i]) * 2^i 就够了？
// 展开:
//   Σ nums[i] * 2^i = 正贡献
//   Σ -nums[i] * 2^(n-1-i) = 负贡献
//   = Σ -nums[n-1-i] * 2^i (做变量替换 i → n-1-i)
//   合并: Σ (nums[i] - nums[n-1-i]) * 2^i
//
// 示例: nums = [1, 2, 3]
//   i=0: (1 - 3) * 1 = -2
//   i=1: (2 - 2) * 2 = 0
//   i=2: (3 - 1) * 4 = 8
//   总和 = -2 + 0 + 8 = 6 ✓
// ============================================================
class Solution3 {
public:
    int sumSubseqWidths(vector<int>& nums) {
        long long MOD = 1e9 + 7;
        int n = nums.size();

        sort(nums.begin(), nums.end());

        // 预处理 2 的幂
        vector<long long> pow2(n);
        pow2[0] = 1;
        for (int i = 1; i < n; i++) {
            pow2[i] = pow2[i - 1] * 2 % MOD;
        }

        long long ans = 0;
        for (int i = 0; i < n; i++) {
            // 配对: (nums[i] - nums[n-1-i]) * 2^i
            long long diff = ((long long)nums[i] - nums[n - 1 - i]) % MOD;
            long long contribution = diff * pow2[i] % MOD;
            ans = (ans + contribution + MOD) % MOD;
        }

        return (int)ans;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法          | 时间         | 空间  | 特点                   |
// |--------------|-------------|------|---------------------- |
// | 暴力枚举      | O(2^n * n)  | O(1) | TLE，理解问题用        |
// | 排序+贡献度   | O(n log n)  | O(n) | ⭐面试首选，公式清晰    |
// | 排序+配对优化  | O(n log n)  | O(n) | 对称简化，代码更短      |
//
// 面试建议:
//   先说暴力 → 引出"宽度只看max/min，排序不影响"
//   → 推导贡献度公式 → 写解法2
//   → 追问优化 → 解释配对对称性
//
// 【易错点】
// 1. 忘记排序:
//    ✗ 未排序时 2^i 不代表"nums[i]作为max的子序列数"
//    ✓ 必须排序，排序后才能用位置 i 推断 max/min 关系
//
// 2. 取模负数:
//    ✗ (pow2[i] - pow2[n-1-i]) % MOD 可能是负数（C++中）
//    ✓ 加 MOD: (pow2[i] - pow2[n-1-i] + MOD) % MOD
//
// 3. 乘法溢出:
//    ✗ nums[i] * pow2[i] 用 int → 最大 10^5 * 10^9 ≈ 10^14 溢出
//    ✓ 转为 long long 后相乘，每步取模
//
// 4. pow2 数组越界:
//    ✗ 只开 n-1 个 → 访问 pow2[n-1] 越界
//    ✓ 开 n 个: pow2[0..n-1]
//
// 5. 混淆子序列和子数组:
//    ✗ 以为是连续子数组 → 用滑动窗口/单调栈
//    ✓ 子序列可以不连续，等价于子集（因为宽度只看值域）
//       所以排序完全合法
//
// 【面试追问 Interview Follow-ups】
// Q1: 排序为什么不影响结果？
// A1: width = max - min 只取决于值的大小，不取决于位置。
//     子序列作为值的集合，其 max 和 min 不受排序影响。
//
// Q2: 如果是子数组（连续）而非子序列怎么做？
// A2: 那就是 LC 2104 (Sum of Subarray Ranges)。
//     连续子数组不能排序，需要用单调栈计算每个元素作为
//     max/min 的范围（类似 LC 907）。
//
// Q3: 重复元素怎么处理？
// A3: 不需要特殊处理。排序后相等元素相邻，
//     每个元素独立计算 2^i 贡献，公式自动正确。
//
// Q4: 能否避免预处理 pow2 数组？
// A4: 可以在循环中维护一个变量 p = 1，每次 p = p*2 % MOD。
//     但需要同时维护从后往前的 2^(n-1-i)，不如预处理清晰。
// ============================================================
