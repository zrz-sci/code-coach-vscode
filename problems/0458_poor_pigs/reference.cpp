/*
 * 【题目本质】
 * 信息编码问题：p 只猪在 T 轮测试中能区分 (T+1)^p 种桶。
 * 找最小 p 使 (T+1)^p >= buckets。
 *
 * 【解法总览】
 * Solution1: 循环乘法 O(log n)/O(1) ⭐ 安全无浮点误差
 * Solution2: 对数公式 O(1)/O(1) 简洁但有精度风险
 */

// ===================== Solution 1: 循环乘法 =====================
// T = minutesToTest / minutesToDie，base = T + 1
// 每只猪有 base 种状态，p 只猪编码 base^p 种
// 循环乘直到 base^p >= buckets
class Solution1 {
public:
    int poorPigs(int buckets, int minutesToDie, int minutesToTest) {
        int T = minutesToTest / minutesToDie;
        int base = T + 1;
        int pigs = 0;
        long long power = 1;
        while (power < buckets) {
            power *= base;
            pigs++;
        }
        return pigs;
    }
};

// ===================== Solution 2: 对数公式 =====================
// pigs = ceil(log(buckets) / log(base))
// 注意浮点精度问题和 buckets == 1 的特判
class Solution2 {
public:
    int poorPigs(int buckets, int minutesToDie, int minutesToTest) {
        if (buckets <= 1) return 0;
        int T = minutesToTest / minutesToDie;
        return (int)ceil(log((double)buckets) / log((double)(T + 1)));
    }
};

/*
 * 【解法对比】
 * Solution1: 循环乘法，无浮点误差，代码清晰
 * Solution2: 一行公式，但 ceil/log 可能有精度问题
 *
 * 【易错点】
 * 1. T+1 种状态（不是 T 种）：包含"一直不死"这个状态
 * 2. buckets == 1 时答案是 0（不需要猪）
 * 3. log 公式的浮点精度：log(8)/log(2) 可能不精确等于 3
 *
 * 【面试追问】
 * Q1: 如何理解 (T+1) 进制编码？
 *     A: 把每个桶编号转为 (T+1) 进制，每一位对应一只猪。
 *     某猪在第 k 轮死 ↔ 该位值为 k；不死 ↔ 该位值为 0。
 * Q2: DP 解法怎么做？
 *     A: dp[i][j] = i 只猪 j 轮能区分的最大桶数
 *     dp[i][j] = sum(C(i,k) * dp[i-k][j-1]) for k=0..i
 *     但贪心/信息论解法更简洁
 * Q3: 如果毒药有延迟不确定（范围内），怎么办？
 *     A: 需要更保守的策略，每轮测试的"确定性"降低
 */
