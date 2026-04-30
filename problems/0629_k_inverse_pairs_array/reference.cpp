// LeetCode 629: K Inverse Pairs Array
// 方法：DP + 前缀和优化
// 时间复杂度: O(n*k), 空间复杂度: O(k)

class Solution {
public:
    int kInversePairs(int n, int k) {
        const int MOD = 1e9 + 7;
        // dp[j] = 用 1~i 组成的排列中恰好有 j 个逆序对的方案数
        vector<long long> dp(k + 1, 0);
        dp[0] = 1; // base case: 1个数, 0个逆序对

        for (int i = 2; i <= n; i++) {
            // 用前缀和数组加速转移
            vector<long long> prefix(k + 2, 0);
            for (int j = 0; j <= k; j++) {
                prefix[j + 1] = (prefix[j] + dp[j]) % MOD;
            }
            for (int j = 0; j <= k; j++) {
                // dp[i][j] = sum of dp[i-1][j-x] for x in [0, min(j, i-1)]
                // = prefix[j+1] - prefix[max(0, j-i+1)]
                dp[j] = (prefix[j + 1] - prefix[max(0, j - i + 1)] + MOD) % MOD;
            }
        }
        return (int)dp[k];
    }
};
