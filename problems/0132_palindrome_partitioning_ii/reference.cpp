/*
 * 【题目本质】
 * 将字符串切割为若干回文子串，求最少切割次数。
 * 核心是"回文判断 + 一维DP求最小分割"。
 *
 * 【解法总览】
 * 1. DP + 二维回文预处理: O(n^2) 时间, O(n^2) 空间 — 逻辑清晰
 * 2. DP + 中心扩展:       O(n^2) 时间, O(n)   空间 — 空间最优
 *
 * 【DP状态图示】
 *
 *   s = "a a b"
 *        0 1 2
 *
 *   dp[0] = -1 (哨兵)
 *   dp[1] = 0  ("a" 回文, dp[0]+1)
 *   dp[2] = 0  ("aa" 回文, dp[0]+1)
 *   dp[3] = 1  ("b" 回文, dp[2]+1 = 1; "aab" 不回文)
 *
 *   回文预处理表 isPalin[i][j]:
 *         j: 0  1  2
 *   i: 0    T  T  F
 *      1       T  F
 *      2          T
 */

// ===================== 解法1: DP + 二维回文预处理 =====================
// 思路: 先构建回文查询表(区间DP)，再用一维DP求最少切割
// 为什么先预处理? 因为DP转移需要O(1)查询任意子串是否回文
class Solution1 {
public:
    int minCut(string s) {
        int n = s.size();

        // 区间DP预处理回文: isPalin[i][j] = s[i..j] 是否回文
        // 为什么从大到小枚举i? 因为 isPalin[i][j] 依赖 isPalin[i+1][j-1]
        vector<vector<bool>> isPalin(n, vector<bool>(n, false));
        for (int i = n - 1; i >= 0; i--) {
            for (int j = i; j < n; j++) {
                if (s[i] == s[j] && (j - i < 2 || isPalin[i + 1][j - 1])) {
                    isPalin[i][j] = true;
                }
            }
        }

        // dp[i] = s[0..i-1] 的最少切割次数
        // 为什么 dp[0] = -1? 当整个 s[0..i-1] 是回文时, dp[i] = dp[0]+1 = 0
        vector<int> dp(n + 1);
        dp[0] = -1;
        for (int i = 1; i <= n; i++) {
            dp[i] = i - 1; // 最坏情况: 每个字符切一刀
            for (int j = 0; j < i; j++) {
                // 如果 s[j..i-1] 是回文，可以在 j 之前切一刀
                if (isPalin[j][i - 1]) {
                    dp[i] = min(dp[i], dp[j] + 1);
                }
            }
        }
        return dp[n];
    }
};

// ===================== 解法2: DP + 中心扩展 (空间优化) =====================
// 思路: 不建回文表，用中心扩展在O(n)空间内同步更新dp
// 为什么能省空间? 中心扩展发现回文时直接更新dp，不需要存储回文表
class Solution2 {
public:
    int minCut(string s) {
        int n = s.size();
        vector<int> dp(n + 1);
        for (int i = 0; i <= n; i++) dp[i] = i - 1;

        for (int center = 0; center < n; center++) {
            // 奇数长度回文: s[center-r..center+r]
            // 为什么从r=0开始? 单个字符一定是回文
            for (int r = 0; center - r >= 0 && center + r < n
                          && s[center - r] == s[center + r]; r++) {
                dp[center + r + 1] = min(dp[center + r + 1], dp[center - r] + 1);
            }
            // 偶数长度回文: s[center-r..center+1+r]
            for (int r = 0; center - r >= 0 && center + 1 + r < n
                          && s[center - r] == s[center + 1 + r]; r++) {
                dp[center + 1 + r + 1] = min(dp[center + 1 + r + 1], dp[center - r] + 1);
            }
        }
        return dp[n];
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 优势 |
 * |------|------|------|------|
 * | 解法1 | O(n^2) | O(n^2) | 逻辑清晰，面试首选 |
 * | 解法2 | O(n^2) | O(n)   | 空间最优，follow-up |
 *
 * 【易错点】
 * 1. dp[0] 必须初始化为 -1，不是 0
 * 2. 回文预处理 i 必须从大到小，否则 isPalin[i+1][j-1] 还没算
 * 3. 中心扩展的边界条件 center - r >= 0，不能写成 > 0
 *
 * 【面试追问】
 * Q1: 与131题的区别? → 131求所有方案(回溯)，本题求最优(DP)
 * Q2: 空间优化思路? → 中心扩展替代回文预处理表
 * Q3: 输出具体方案? → 额外记录 parent 数组回溯切割位置
 */
