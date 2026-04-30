/*
 * LeetCode 903: Valid Permutations for DI Sequence
 *
 * 解法1: DP + 前缀和优化 + 滚动数组 (面试首选)
 * 时间: O(n^2)  空间: O(n)
 *
 * dp[j] = 当前位置放"剩余数中第j小"的方案数
 * 'D' → 后缀和转移, 'I' → 前缀和转移
 */

// ==================== 解法1: 滚动数组 DP ====================

class Solution {
public:
    int numPermsDISequence(string s) {
        int n = s.size();
        long long MOD = 1e9 + 7;

        // 初始化: 位置 0 可以放 {0,1,...,n} 中任意一个
        // 每个选择都是 1 种方案
        vector<long long> dp(n + 1, 1);

        for (int i = 0; i < n; i++) {
            // 本轮结束后，剩余可用数字数量 = n - i
            // 新 dp 的有效索引范围: [0, n-i-1]
            vector<long long> ndp(n - i, 0);

            if (s[i] == 'D') {
                // Decrease: perm[i] > perm[i+1]
                // perm[i] 是旧剩余中第 j 小，删除后
                // perm[i+1] 要比 perm[i] 小
                // → ndp[j] = sum of dp[j+1], dp[j+2], ..., dp[n-i]
                // 用后缀和从右往左累加
                long long suffix = 0;
                for (int j = n - i - 1; j >= 0; j--) {
                    suffix = (suffix + dp[j + 1]) % MOD;
                    ndp[j] = suffix;
                }
            } else {
                // Increase: perm[i] < perm[i+1]
                // → ndp[j] = sum of dp[0], dp[1], ..., dp[j]
                // 用前缀和从左往右累加
                long long prefix = 0;
                for (int j = 0; j < n - i; j++) {
                    prefix = (prefix + dp[j]) % MOD;
                    ndp[j] = prefix;
                }
            }

            dp = ndp;
        }

        // 最后只剩 1 个数可放，答案就是 dp[0]
        return dp[0];
    }
};

// ==================== 解法2: 完整 2D DP ====================

/*
 * 时间: O(n^2)  空间: O(n^2)
 * 更直观，便于理解和调试
 */

class Solution_2D {
public:
    int numPermsDISequence(string s) {
        int n = s.size();
        long long MOD = 1e9 + 7;

        // dp[i][j]: 已填好位置 0..i，位置 i 放的是
        //           剩余数中第 j 小的数
        vector<vector<long long>> dp(n + 1, vector<long long>(n + 1, 0));

        // 基础: 第 0 个位置可放 0~n 中任一个
        for (int j = 0; j <= n; j++) {
            dp[0][j] = 1;
        }

        for (int i = 1; i <= n; i++) {
            // 位置 i 有 n - i + 1 个数可选，排名 0 ~ n-i
            if (s[i - 1] == 'D') {
                // 后缀和: dp[i][j] = dp[i-1][j+1] + dp[i-1][j+2] + ...
                long long suffix = 0;
                for (int j = n - i; j >= 0; j--) {
                    suffix = (suffix + dp[i - 1][j + 1]) % MOD;
                    dp[i][j] = suffix;
                }
            } else { // 'I'
                // 前缀和: dp[i][j] = dp[i-1][0] + dp[i-1][1] + ... + dp[i-1][j]
                long long prefix = 0;
                for (int j = 0; j <= n - i; j++) {
                    prefix = (prefix + dp[i - 1][j]) % MOD;
                    dp[i][j] = prefix;
                }
            }
        }

        return dp[n][0];
    }
};

// ==================== 解法3: 记忆化搜索 ====================

/*
 * 时间: O(n^2)  空间: O(n^2)
 * 递归版本，思路最直观但有函数调用开销
 */

class Solution_Memo {
    int n;
    long long MOD = 1e9 + 7;
    vector<vector<long long>> memo;

    // dfs(i, j): 位置 i 放的是剩余数中第 j 小
    //            返回从位置 i 到位置 n 的合法方案数
    long long dfs(const string& s, int i, int j) {
        if (i == n) return 1; // 所有位置都填完了
        if (memo[i][j] != -1) return memo[i][j];

        long long res = 0;
        int remaining = n - i; // 删除位置 i 的数后，还剩 remaining 个

        if (s[i] == 'D') {
            // 下一个数必须更小，在新剩余中排名 [0, j-1]
            for (int k = 0; k < j; k++) {
                res = (res + dfs(s, i + 1, k)) % MOD;
            }
        } else {
            // 下一个数必须更大，在新剩余中排名 [j, remaining-1]
            for (int k = j; k < remaining; k++) {
                res = (res + dfs(s, i + 1, k)) % MOD;
            }
        }

        return memo[i][j] = res;
    }

public:
    int numPermsDISequence(string s) {
        n = s.size();
        memo.assign(n + 1, vector<long long>(n + 1, -1));

        long long ans = 0;
        for (int j = 0; j <= n; j++) {
            ans = (ans + dfs(s, 0, j)) % MOD;
        }
        return ans;
    }
};

/*
 * ==================== DP 转移推导 ====================
 *
 * 设剩余可用数字按升序排列为 r_0 < r_1 < ... < r_m
 *
 * 位置 i 放了 r_j。删除 r_j 后，剩余变成:
 *   r_0, r_1, ..., r_{j-1}, r_{j+1}, ..., r_m
 *
 * 如果 s[i] == 'D': perm[i+1] < perm[i] = r_j
 *   → perm[i+1] 必须从 {r_0, ..., r_{j-1}} 中选
 *   → 在新剩余中，这些数的排名是 0, 1, ..., j-1
 *   → dp[i+1][k] += dp[i][j]  对于 k = 0, 1, ..., j-1
 *   → 反过来: dp[i+1][k] = sum(dp[i][k+1], dp[i][k+2], ...) = 后缀和
 *
 * 如果 s[i] == 'I': perm[i+1] > perm[i] = r_j
 *   → perm[i+1] 必须从 {r_{j+1}, ..., r_m} 中选
 *   → 删除 r_j 后，r_{j+1} 变成排名 j, r_{j+2} 变成排名 j+1, ...
 *   → dp[i+1][k] += dp[i][j]  对于 k = j, j+1, ..., m-1
 *   → 反过来: dp[i+1][k] = sum(dp[i][0], dp[i][1], ..., dp[i][k]) = 前缀和
 *
 * ==================== 复杂度总结 ====================
 *
 * | 解法        | 时间    | 空间    | 备注           |
 * |------------|---------|---------|----------------|
 * | 滚动 DP    | O(n^2)  | O(n)   | 面试首选        |
 * | 2D DP      | O(n^2)  | O(n^2) | 更好理解        |
 * | 记忆化搜索  | O(n^2)  | O(n^2) | 递归直观        |
 * | 暴力全排列  | O(n!)   | O(n)   | 仅作思路分析    |
 */
