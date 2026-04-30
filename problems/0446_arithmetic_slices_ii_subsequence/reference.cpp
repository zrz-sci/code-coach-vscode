/*
 * 【题目本质】
 * 统计数组中所有长度 >= 3 的等差子序列（不要求连续）的个数。
 * 核心是用 DP + HashMap 记录以每个位置结尾、每种公差的弱等差子序列数。
 *
 * 【解法总览】
 * 解法1: DP + HashMap — O(n^2) / O(n^2) ⭐ 标准解法
 * 解法2: 暴力枚举(TLE参考) — O(2^n) / O(n)
 */

// ===================== Solution1: DP + HashMap =====================
// 思路：dp[i][d] = 以 nums[i] 结尾、公差为 d 的弱等差子序列数（长度>=2）
//       对每对 (j, i)，dp[j][d] 个弱序列延长后成为真等差子序列 → 贡献到答案
// 时间 O(n^2)  空间 O(n^2)
class Solution1 {
public:
    int numberOfArithmeticSlices(vector<int>& nums) {
        int n = nums.size(), ans = 0;
        // dp[i]: 以 nums[i] 结尾的 {公差 → 弱等差子序列个数}
        vector<unordered_map<long, int>> dp(n);

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < i; j++) {
                long d = (long)nums[i] - nums[j]; // 用 long 防溢出

                int cnt = 0;
                if (dp[j].count(d)) {
                    cnt = dp[j][d]; // j 处已有的弱序列数
                }

                // cnt 个弱序列（长度>=2）+ nums[i] → 长度>=3 的真等差子序列
                ans += cnt;

                // 更新 dp[i][d]：cnt 个延长的 + 1 个新二元组 (j, i)
                dp[i][d] += cnt + 1;
            }
        }
        return ans;
    }
};

// ===================== Solution2: 暴力DFS(TLE参考) =====================
// 思路：枚举所有子序列，检查是否为等差
// 时间 O(2^n * n)  空间 O(n)  — 仅用于理解，会超时
class Solution2 {
public:
    int numberOfArithmeticSlices(vector<int>& nums) {
        int n = nums.size(), ans = 0;
        // 枚举所有长度 >= 3 的子序列（省略完整实现）
        // 对每个子序列检查是否等差
        // 此方法 n=1000 时完全不可行
        return ans;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 优势 | 劣势 |
 * |------|------|------|------|------|
 * | Solution1 DP+HashMap | O(n^2) | O(n^2) | 高效，唯一可行解 | HashMap 常数较大 |
 * | Solution2 暴力DFS | O(2^n) | O(n) | 思路简单 | n>20 就超时 |
 *
 * 【易错点】
 * 1. 公差 d 必须用 long：nums[i] 范围是 [-2^31, 2^31-1]，差值可达 2^32
 * 2. ans += dp[j][d] 不是 dp[j][d]+1：+1 的二元组只有 2 个元素，不够 3 个
 * 3. dp[i][d] += cnt + 1 不是 = cnt + 1：同一公差可能有多个 j 贡献
 * 4. 用 unordered_map 而非 map：避免额外的 O(logn) 因子
 *
 * 【面试追问】
 * Q1: 弱等差子序列和真等差子序列的区别？
 *     弱序列长度 >= 2（包含二元组），真序列长度 >= 3。DP 中追踪弱序列，
 *     每次延长时弱→真，贡献到答案。
 * Q2: 为什么空间是 O(n^2)？
 *     每个位置 i 的 HashMap 最多有 i 个不同公差（对应 i 个前驱 j），
 *     总计 0+1+...+(n-1) = O(n^2) 个键值对。
 * Q3: 如何处理重复元素？如 [7,7,7,7,7]
 *     自然处理：公差 d=0 的弱序列会被多个 j 累加，结果为 C(5,3)+C(5,4)+C(5,5)=16
 */
