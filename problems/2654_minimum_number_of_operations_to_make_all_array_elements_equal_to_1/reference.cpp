// ============================================================
// 2654. Minimum Number of Operations to Make All Array
//       Elements Equal to 1
// ============================================================
// 标签: Array, Math, Number Theory
// 难度: Medium
//
// 核心思路:
//   1. 如果数组中已有 1，答案 = n - count(1)
//   2. 否则找最短子数组使 GCD = 1，答案 = (minLen-1) + (n-1)
//   3. 如果全局 GCD > 1，返回 -1
//
// 两阶段分解:
//   Phase 1 — 从无到有，制造第一个 1
//   Phase 2 — 用 1 感染全数组 (每次一个, 共 n-1 次)
// ============================================================


// ============================================================
// 解法1: 枚举子数组 GCD (推荐)
//
// 策略:
//   固定左端点 i，逐步扩展右端点 j
//   维护 gcd(nums[i..j])
//   GCD 随子数组扩展单调不增，一旦为 1 即 break（不会更短）
//
// 过程可视化 (nums = [2, 6, 3, 4]):
//
//   Step 1: 检查已有的 1 -> cnt_ones = 0
//
//   Step 2: 枚举子数组
//     l=0: [2]       g=2
//          [2,6]     g=gcd(2,6)=2
//          [2,6,3]   g=gcd(2,3)=1  -> 长度3
//     l=1: [6]       g=6
//          [6,3]     g=gcd(6,3)=3
//          [6,3,4]   g=gcd(3,4)=1  -> 长度3
//     l=2: [3]       g=3
//          [3,4]     g=gcd(3,4)=1  -> 长度2 ★最短!
//     l=3: [4]       g=4
//
//   minLen = 2
//   答案 = (2-1) + (4-1) = 1 + 3 = 4
//
// 时间: O(n^2 * log(maxVal))
// 空间: O(1)
// ============================================================
class Solution {
public:
    int minOperations(vector<int>& nums) {
        int n = nums.size();

        // ---- 特判: 数组中已有 1 ----
        // gcd(x, 1) = 1，每个 1 感染一个邻居
        // 只需要把所有非 1 元素变成 1
        int cntOnes = count(nums.begin(), nums.end(), 1);
        if (cntOnes > 0) {
            return n - cntOnes;
        }

        // ---- Phase 1: 找最短子数组使 GCD = 1 ----
        int minLen = INT_MAX;

        for (int i = 0; i < n; i++) {
            int g = 0;  // gcd(0, x) = x，作为初始值
            for (int j = i; j < n; j++) {
                g = __gcd(g, nums[j]);
                if (g == 1) {
                    minLen = min(minLen, j - i + 1);
                    break;  // GCD 单调不增，再扩展只会更长
                }
            }
        }

        // ---- 判断无解 ----
        // 没有任何子数组 GCD 为 1 => 全局 GCD > 1
        if (minLen == INT_MAX) {
            return -1;
        }

        // ---- 计算答案 ----
        // Phase 1: minLen - 1 次操作制造第一个 1
        // Phase 2: n - 1 次操作用 1 感染全数组
        return (minLen - 1) + (n - 1);
    }
};

// ============================================================
// 解法2: O(n log V) 优化版
//
// 利用性质: 以右端点 j 结尾的所有子数组的 GCD 值
// 最多只有 O(log V) 个不同值。
// 因为每当 GCD 变小时，它至少变为原值的某个真因子（至少减半）。
//
// 维护一个集合 {(gcd_value, leftmost_start)}，
// 每加入 nums[j] 时与所有已有 GCD 值合并并去重。
//
// 时间: O(n * log^2 V)  (n次迭代 * log V 个GCD * log V 计算gcd)
// 空间: O(log V)
// ============================================================
class Solution_Optimized {
public:
    int minOperations(vector<int>& nums) {
        int n = nums.size();

        int cntOnes = count(nums.begin(), nums.end(), 1);
        if (cntOnes > 0) return n - cntOnes;

        int minLen = INT_MAX;

        // gcd_set: 以当前 j 为右端点的所有不同 GCD 值
        // 每个元素 = (gcd值, 最早的左端点索引)
        vector<pair<int, int>> gcd_set;

        for (int j = 0; j < n; j++) {
            // 1. nums[j] 单独作为子数组
            vector<pair<int, int>> new_set;
            new_set.push_back({nums[j], j});

            // 2. 与之前的 GCD 值合并
            for (auto& [g, l] : gcd_set) {
                int new_g = __gcd(g, nums[j]);
                new_set.push_back({new_g, l});
            }

            // 3. 去重: 相同 GCD 保留最小左端点（最短子数组）
            sort(new_set.begin(), new_set.end());
            gcd_set.clear();
            for (auto& [g, l] : new_set) {
                if (gcd_set.empty() || gcd_set.back().first != g) {
                    gcd_set.push_back({g, l});
                } else {
                    gcd_set.back().second = min(gcd_set.back().second, l);
                }
            }

            // 4. 检查是否有 GCD = 1
            for (auto& [g, l] : gcd_set) {
                if (g == 1) {
                    minLen = min(minLen, j - l + 1);
                }
            }
        }

        if (minLen == INT_MAX) return -1;
        return (minLen - 1) + (n - 1);
    }
};

// ============================================================
// 测试代码
// ============================================================
void test() {
    Solution sol;

    // 测试1: 示例 [2,6,3,4] -> 4
    {
        vector<int> nums = {2, 6, 3, 4};
        assert(sol.minOperations(nums) == 4);
    }

    // 测试2: 无解 [2,10,6,14] -> -1
    {
        vector<int> nums = {2, 10, 6, 14};
        assert(sol.minOperations(nums) == -1);
    }

    // 测试3: 已有 1
    {
        vector<int> nums = {1, 2, 3, 4};
        assert(sol.minOperations(nums) == 3);  // n - 1 = 3
    }

    // 测试4: 全是 1
    {
        vector<int> nums = {1, 1, 1};
        assert(sol.minOperations(nums) == 0);
    }

    // 测试5: 多个 1
    {
        vector<int> nums = {1, 5, 1, 7};
        assert(sol.minOperations(nums) == 2);  // n - 2 = 2
    }

    // 测试6: 相邻互质
    {
        vector<int> nums = {3, 4};
        assert(sol.minOperations(nums) == 2);
        // minLen=2, (2-1)+(2-1) = 1+1 = 2
    }

    // 测试7: 间接互质
    {
        vector<int> nums = {6, 10, 15};
        // gcd(6,10)=2, gcd(2,15)=1 -> minLen=3
        // 答案 = (3-1) + (3-1) = 2+2 = 4
        assert(sol.minOperations(nums) == 4);
    }

    // 测试优化版
    Solution_Optimized sol2;
    {
        vector<int> nums = {2, 6, 3, 4};
        assert(sol2.minOperations(nums) == 4);
    }
    {
        vector<int> nums = {2, 10, 6, 14};
        assert(sol2.minOperations(nums) == -1);
    }
    {
        vector<int> nums = {6, 10, 15};
        assert(sol2.minOperations(nums) == 4);
    }

    cout << "All tests passed!" << endl;
}

// int main() { test(); return 0; }

/*
 * 【易错点】
 * 1. 忘记特判已有的 1: 如果数组中已有 1，不需要 Phase 1，直接 n - cnt_ones。
 * 2. 公式记错: Phase 1 是 minLen-1（不是 minLen），Phase 2 是 n-1（不是 n-cnt_ones）。
 * 3. 判断无解条件: 不是看有没有 1，而是看全局 GCD 是否为 1。
 *    全局 GCD > 1 意味着所有元素共享质因子，永远无法产生 1。
 * 4. 子数组 vs 子序列: 只能操作相邻元素，所以必须是连续子数组。
 * 5. 忘记剪枝: GCD 到 1 后继续扩展没有意义（GCD 单调不增），应 break。
 *
 * 【面试追问】
 * Q1: 如果 n 很大（如 10^5），如何优化?
 *     → 利用"以右端点 j 结尾的不同 GCD 值最多 O(log V) 个"的性质。
 *       维护 (gcd_value, leftmost_index) 集合，总时间 O(n log^2 V)。
 *
 * Q2: 如果能操作任意两个元素（不限相邻）呢?
 *     → 不需要"连续子数组"了。只要全局 GCD=1 就有解。但最优策略分析更复杂。
 *
 * Q3: 为什么 Phase 2 一定需要 n-1 步，不能更快?
 *     → 每次操作最多让一个位置从非 1 变成 1（gcd(x,1)=1 只改变一个元素）。
 *       有 n-1 个非 1 元素，所以至少需要 n-1 步。
 */
