/*
 * LeetCode 548: 将数组分割成和相等的子数组 (Split Array with Equal Sum)
 *
 * 【题目本质】
 * 三刀切四段，每段和相等。暴力 O(n^3) 超时，
 * 用前缀和 + 固定中间切割点 j + HashSet 降到 O(n^2)。
 *
 * 【解法总览】
 * 解法1: 前缀和 + 枚举 j + HashSet — O(n^2) / O(n) — ⭐ 面试首选
 * 解法2: 暴力枚举 — O(n^3) / O(n) — 仅供理解
 *
 *  示例: nums = [1, 2, 1, 2, 1, 2, 1]
 *  切割: i=1, j=3, k=5
 *  四段: [1] | [1] | [1] | [1]  → 各段和均为 1
 *
 *  prefix = [0, 1, 3, 4, 6, 7, 9, 10]
 *  段和: prefix[1]=1, prefix[3]-prefix[2]=1,
 *       prefix[5]-prefix[4]=1, prefix[7]-prefix[6]=1
 */

// ============================================================
// 解法1: 前缀和 + 枚举 j + HashSet ⭐ 面试首选
// 时间: O(n^2)  空间: O(n)
//
// 【思路】
// 1. 预计算前缀和
// 2. 枚举中间切割点 j ∈ [3, n-4]
// 3. 对每个 j:
//    - 左半扫描 i ∈ [1, j-2]，第一段==第二段则存入 set
//    - 右半扫描 k ∈ [j+2, n-2]，第三段==第四段且在 set 中则返回 true
//
//  为什么固定 j？
//  j 是中间切割点，固定它后左右两半完全独立，
//  可以用 HashSet 桥接，总复杂度降为 O(n^2)。
// ============================================================
class Solution1 {
public:
    bool splitArray(vector<int>& nums) {
        int n = nums.size();
        if (n < 7) return false;  // 至少需要 7 个元素

        // 前缀和: prefix[i] = nums[0] + ... + nums[i-1]
        vector<int> prefix(n + 1, 0);
        for (int i = 0; i < n; i++) {
            prefix[i + 1] = prefix[i] + nums[i];
        }

        // 枚举中间切割点 j
        for (int j = 3; j < n - 3; j++) {
            unordered_set<int> seen;

            // 左半: 枚举 i ∈ [1, j-2]
            for (int i = 1; i < j - 1; i++) {
                int sum1 = prefix[i];                  // [0, i-1]
                int sum2 = prefix[j] - prefix[i + 1];  // [i+1, j-1]
                if (sum1 == sum2) {
                    seen.insert(sum1);
                }
            }

            // 右半: 枚举 k ∈ [j+2, n-2]
            for (int k = j + 2; k < n - 1; k++) {
                int sum3 = prefix[k] - prefix[j + 1];  // [j+1, k-1]
                int sum4 = prefix[n] - prefix[k + 1];  // [k+1, n-1]
                if (sum3 == sum4 && seen.count(sum3)) {
                    return true;
                }
            }
        }

        return false;
    }
};

// ============================================================
// 解法2: 暴力枚举（仅供理解，O(n^3) 超时）
// 时间: O(n^3)  空间: O(n)
//
// 【思路】
// 直接枚举 i, j, k 三个切割点，用前缀和 O(1) 判断四段和是否相等。
// n=2000 时约 8*10^9 次操作，超时。
// ============================================================
class Solution2 {
public:
    bool splitArray(vector<int>& nums) {
        int n = nums.size();
        if (n < 7) return false;

        vector<int> prefix(n + 1, 0);
        for (int i = 0; i < n; i++) {
            prefix[i + 1] = prefix[i] + nums[i];
        }

        for (int i = 1; i < n - 5; i++) {
            int sum1 = prefix[i];
            for (int j = i + 2; j < n - 3; j++) {
                int sum2 = prefix[j] - prefix[i + 1];
                if (sum1 != sum2) continue;
                for (int k = j + 2; k < n - 1; k++) {
                    int sum3 = prefix[k] - prefix[j + 1];
                    int sum4 = prefix[n] - prefix[k + 1];
                    if (sum1 == sum3 && sum3 == sum4) return true;
                }
            }
        }

        return false;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法           | 时间    | 空间  | 特点                     |
// |---------------|---------|-------|--------------------------|
// | 枚举 j + Set  | O(n^2) | O(n) | ⭐ 面试首选，降维关键      |
// | 暴力三重循环   | O(n^3) | O(n) | 超时，仅理解用            |
//
// 核心降维技巧：固定中间切割点 j，将三维搜索拆成
// 两个独立的一维搜索，用 HashSet 桥接。
//
// ============================================================
// 【易错点】
//
// 1. ✗ j 的范围写成 [1, n-2]，导致某段为空
//    ✓ j ∈ [3, n-4]，确保四段各至少一个元素
//
// 2. ✗ 前缀和下标混乱
//    ✓ 统一: prefix[0]=0, prefix[i]=sum(nums[0..i-1])
//    → 段[a, b]的和 = prefix[b+1] - prefix[a]
//
// 3. ✗ 忘记 n < 7 的提前返回
//    ✓ 4段 + 3个切割点，至少需要 7 个元素
//
// 4. ✗ 只判断 sum3==sum4，忘了和 set 比较
//    ✓ 三个条件同时满足: sum1==sum2, sum3==sum4, sum1==sum3
//
// ============================================================
// 【面试追问】
//
// Q1: 为什么固定 j 是最优策略？
// → j 是中间切割点，固定后左右完全独立，可用 HashSet 桥接。
//   固定 i 或 k 则 j 和另一端仍耦合。
//
// Q2: 能否进一步优化到 O(n)？
// → 很难。两个独立区间找匹配值，至少需要 O(n) 扫描每个区间，
//   外层枚举 j 共 O(n) 次，总 O(n^2) 已是最优。
//
// Q3: 如果数组全部相同（如 [0,0,0,...,0]）？
// → 所有 i,j,k 组合都满足，但算法仍能正确返回 true。
//   第一次找到匹配就立即返回。
//
// 【相关题型】
// - 560. 和为 K 的子数组 — 前缀和 + HashTable 经典
// - 1. 两数之和 — HashSet 桥接两侧搜索
// - 698. 划分为 K 个相等的子集 — 回溯法等和切割
// ============================================================
