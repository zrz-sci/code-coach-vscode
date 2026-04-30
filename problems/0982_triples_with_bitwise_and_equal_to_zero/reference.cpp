/*
 * LeetCode 982: 按位与为零的三元组 (Triples with Bitwise AND Equal to Zero)
 *
 * 【题目本质】
 * 给定整数数组 nums，统计满足 nums[i] & nums[j] & nums[k] == 0 的三元组 (i,j,k) 个数。
 * i, j, k 可以相同。核心优化：利用 AND 的结合律拆成两步。
 *
 * 【解法总览】
 * 解法1: 暴力三重循环       — O(n^3) / O(1)     — 最直觉，但 TLE
 * 解法2: 哈希表预处理两两AND — O(n^2 + n*2^16) / O(2^16) — 面试首选 ⭐
 * 解法3: 子集枚举优化       — O(n^2 + n*2^16) / O(2^16) — 进阶优化
 */

// ============================================================
// 解法1: 暴力三重循环
// 时间: O(n^3)  空间: O(1)
//
// 【思路】
// 最直接的做法：三层循环枚举所有 (i, j, k) 组合。
// n <= 1000 时 O(n^3) = 10^9，几乎一定超时。
// 但能帮助我们看清：可以把 nums[i] & nums[j] 先提取出来。
// ============================================================
class Solution1 {
public:
    int countTriplets(vector<int>& nums) {
        int n = nums.size(), count = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                int ab = nums[i] & nums[j]; // 提取两两AND
                for (int k = 0; k < n; k++) {
                    if ((ab & nums[k]) == 0) {
                        count++;
                    }
                }
            }
        }
        return count;
    }
};

// ============================================================
// 解法2: 哈希表预处理两两AND — 面试首选 ⭐
// 时间: O(n^2 + n * 2^16)  空间: O(2^16)
//
// 【思路】
// 为什么想到这个？
// 1. 三重循环太慢 → 能否降维？
// 2. a & b & c = (a & b) & c → 先计算所有两两AND的结果和频率
// 3. 0 <= nums[i] < 2^16 → AND结果也在 [0, 2^16) 内
// 4. 哈希表/数组最多 65536 个键，遍历成本可控
//
// 第一步: O(n^2) 预处理所有 nums[i] & nums[j] 的计数
// 第二步: 对每个 nums[k]，遍历哈希表找所有与其AND为0的值
//
// 示例: nums = [2, 1, 3]
// 两两AND频率: {2:3, 0:2, 1:3, 3:1}
//   2&2=2, 2&1=0, 2&3=2, 1&2=0, 1&1=1, 1&3=1, 3&2=2, 3&1=1, 3&3=3
// 对 nums[k]=2(10): 需要 val 的 bit1 为 0 → val=0(cnt=2), val=1(cnt=3) → 贡献 5
// 对 nums[k]=1(01): 需要 val 的 bit0 为 0 → val=0(cnt=2), val=2(cnt=3) → 贡献 5
// 对 nums[k]=3(11): 需要 val=0 → 贡献 2
// 总计 = 5 + 5 + 2 = 12 ✓
// ============================================================
class Solution2 {
public:
    int countTriplets(vector<int>& nums) {
        int n = nums.size();
        // 用数组代替哈希表，下标就是 AND 值，访问 O(1)
        vector<int> cnt(1 << 16, 0);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cnt[nums[i] & nums[j]]++;
            }
        }

        int result = 0;
        for (int k = 0; k < n; k++) {
            // 遍历所有可能的两两AND值
            for (int val = 0; val < (1 << 16); val++) {
                if (cnt[val] > 0 && (val & nums[k]) == 0) {
                    result += cnt[val];
                }
            }
        }
        return result;
    }
};

// ============================================================
// 解法3: 子集枚举优化
// 时间: O(n^2 + n * 2^16)  空间: O(2^16)
//
// 【思路】
// 解法2的第二步遍历了整个 [0, 2^16) 范围，但很多值的 cnt 为 0。
// 优化: 对于 nums[k]，要找 val 使得 val & nums[k] == 0。
// 这等价于 val 是 ~nums[k] 的子集（val 的每个 1 位都在 nums[k] 的 0 位上）。
//
// 子集枚举技巧:
//   mask = (~nums[k]) & 0xFFFF  // 取反后的低16位
//   for (sub = mask; sub > 0; sub = (sub - 1) & mask)
//     result += cnt[sub];
//   result += cnt[0];  // sub=0 的情况
//
// 为什么 sub = (sub-1) & mask 能遍历所有子集？
//   sub-1 翻转了 sub 的最低 1 位以及更低的所有位
//   & mask 确保结果仍是 mask 的子集
//   每次 sub 严格减小，最终到 0
//
// 单个 mask 的子集数 = 2^popcount(mask)
// 所有 n 个 nums[k] 的总子集枚举量 ≤ n * 2^16 (最坏情况相同)
// 但实际当 nums[k] 有很多 1 位时，mask 的 1 位少，子集少，平均更快
// ============================================================
class Solution3 {
public:
    int countTriplets(vector<int>& nums) {
        int n = nums.size();
        vector<int> cnt(1 << 16, 0);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cnt[nums[i] & nums[j]]++;
            }
        }

        int result = 0;
        for (int k = 0; k < n; k++) {
            // ~nums[k] 的低 16 位就是 nums[k] 的补集
            int mask = (~nums[k]) & 0xFFFF;

            // 枚举 mask 的所有非空子集
            for (int sub = mask; sub > 0; sub = (sub - 1) & mask) {
                result += cnt[sub];
            }
            // 别忘了 sub=0: 0 & anything == 0
            result += cnt[0];
        }
        return result;
    }
};

// ============================================================
// 解法2的 unordered_map 版本（如果题目值域不确定时使用）
// ============================================================
class Solution4 {
public:
    int countTriplets(vector<int>& nums) {
        int n = nums.size();
        unordered_map<int, int> cnt;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cnt[nums[i] & nums[j]]++;
            }
        }

        int result = 0;
        for (int k = 0; k < n; k++) {
            for (auto& [val, freq] : cnt) {
                if ((val & nums[k]) == 0) {
                    result += freq;
                }
            }
        }
        return result;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法 | 时间 | 空间 | 代码复杂度 | 推荐场景 |
// |------|------|------|------------|----------|
// | 暴力 | O(n^3) | O(1) | 简单 | 说思路 |
// | 数组预处理 | O(n^2+n*2^16) | O(2^16) | 中等 | ⭐面试首选 |
// | 子集枚举 | O(n^2+n*2^16) | O(2^16) | 中等偏难 | 追问优化 |
// | 哈希表版 | O(n^2+n*keys) | O(keys) | 中等 | 值域不确定时 |
//
// 【易错点】
//
// 1. 忘记 i, j, k 可以相同
//    ✗ for (int j = i+1; ...) → 只统计了 i < j 的情况
//    ✓ i, j, k 各自独立从 0 到 n-1，允许相同
//
// 2. 子集枚举漏掉 sub=0
//    ✗ for (sub = mask; sub > 0; ...) 后没加 cnt[0]
//    ✓ 循环外必须单独处理 result += cnt[0]
//
// 3. 取反时忘记 & 0xFFFF
//    ✗ int mask = ~nums[k]; → 高位全 1，枚举 2^32 个子集
//    ✓ int mask = (~nums[k]) & 0xFFFF; → 只看低 16 位
//
// 4. 用 unordered_map 时直接遍历同时修改
//    ✗ 在遍历 cnt 的同时往里插入新键（UB）
//    ✓ 预处理和查询分成两个独立阶段
//
// 【面试追问】
//
// Q1: 三重循环能过吗？
// → n=1000, O(10^9), TLE。需要降维。
//
// Q2: 为什么用数组而不是哈希表？
// → nums[i] < 2^16, AND 结果 ∈ [0, 65535]，数组 O(1) 访问比哈希表快得多。
//
// Q3: 子集枚举的复杂度怎么分析？
// → 对于掩码 mask，其子集数 = 2^popcount(mask)。
//    所有 n 个 mask 的总枚举量：sum(2^popcount(~nums[k] & 0xFFFF))。
//    最坏当 nums[k]=0 时 mask=0xFFFF, 子集数=2^16=65536。
//    最好当 nums[k]=0xFFFF 时 mask=0, 子集数=1。
//
// Q4: 能否用 FWT 做到 O(n * 16 * 2^16)?
// → 可以。先对 cnt 做快速沃尔什变换（AND卷积），但面试中几乎不会考。
// ============================================================
