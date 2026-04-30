// LeetCode 477: Total Hamming Distance
// 核心思想：按位独立统计，每位贡献 = ones * zeros
//
// 关键洞察：
// - 对第 b 位，若有 c 个数该位为1，n-c 个数为0
// - 该位产生的汉明距离总和 = c * (n - c)
// - 所有位求和即为最终答案
//
// 时间 O(30n) = O(n)，空间 O(1)

// ==================== 解法1: 按位统计（推荐）====================
class Solution {
public:
    int totalHammingDistance(vector<int>& nums) {
        int n = nums.size();
        int result = 0;

        // 枚举每一位 (0~29 覆盖 0 到 10^9)
        for (int bit = 0; bit < 30; ++bit) {
            int ones = 0;  // 统计该位为 1 的数的个数
            for (int x : nums) {
                ones += (x >> bit) & 1;
            }
            // 该位的贡献 = 有 1 的个数 * 有 0 的个数
            // 因为只有 1 和 0 配对时才会产生汉明距离 1
            result += ones * (n - ones);
        }

        return result;
    }
};

// ==================== 解法2: 暴力枚举所有对（TLE，仅供理解）====================
// 直接枚举所有 (i, j) 对，用 XOR + popcount 计算汉明距离
// 时间 O(n^2)，n=10^4 时约 10^8 边界情况
class Solution_Brute {
public:
    int totalHammingDistance(vector<int>& nums) {
        int n = nums.size();
        int result = 0;

        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                // XOR 得到不同的位，popcount 统计 1 的个数
                result += __builtin_popcount(nums[i] ^ nums[j]);
            }
        }

        return result;
    }
};

// ==================== 解法1 变体: 使用数组预存每位的 1 的计数 ====================
// 如果需要多次查询或者代码更清晰，可以先统计每一位的 1 的个数
class Solution_Variant {
public:
    int totalHammingDistance(vector<int>& nums) {
        int n = nums.size();
        int bitCount[30] = {};  // bitCount[b] = 第 b 位为 1 的数的个数

        // 一次遍历统计所有位
        for (int x : nums) {
            for (int b = 0; b < 30 && x > 0; ++b) {
                bitCount[b] += x & 1;
                x >>= 1;
            }
        }

        // 计算总汉明距离
        int result = 0;
        for (int b = 0; b < 30; ++b) {
            result += bitCount[b] * (n - bitCount[b]);
        }

        return result;
    }
};

// ==================== 复杂度分析 ====================
// 解法1: 时间 O(30 * n) = O(n)，空间 O(1)
// 解法2: 时间 O(n^2)，空间 O(1) — 会 TLE
//
// 为什么按位统计是正确的？
// 考虑所有对 (i, j)，汉明距离 = popcount(nums[i] XOR nums[j])
// = sum_b ((nums[i]>>b)&1) XOR ((nums[j]>>b)&1)
// 交换求和顺序 = sum_b (所有对在第 b 位的异或之和)
// 第 b 位异或为 1 当且仅当一个为 0 一个为 1
// 这样的对数 = ones * zeros = c * (n - c)
