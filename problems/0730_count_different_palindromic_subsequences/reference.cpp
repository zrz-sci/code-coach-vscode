// LeetCode 730: Count Different Palindromic Subsequences
// 解法：区间 DP + 按字符分类
// 时间 O(n^2)，空间 O(n^2)

class Solution {
public:
    int countPalindromicSubsequences(string s) {
        int n = s.size();
        const int MOD = 1e9 + 7;

        // dp[i][j] = s[i..j] 中不同回文子序列的数量
        vector<vector<long long>> dp(n, vector<long long>(n, 0));

        // 单个字符本身是回文
        for (int i = 0; i < n; ++i) {
            dp[i][i] = 1;
        }

        // 按区间长度从小到大填表
        for (int len = 2; len <= n; ++len) {
            for (int i = 0; i <= n - len; ++i) {
                int j = i + len - 1;

                // 对四个字符分别计算贡献
                for (char c = 'a'; c <= 'd'; ++c) {
                    // 找字符 c 在 s[i..j] 中的最左和最右位置
                    int lo = i, hi = j;
                    while (lo <= j && s[lo] != c) lo++;
                    while (hi >= i && s[hi] != c) hi--;

                    if (lo > j) {
                        // 字符 c 不在区间内
                        continue;
                    } else if (lo == hi) {
                        // 只有一个字符 c，贡献 "c"
                        dp[i][j] = (dp[i][j] + 1) % MOD;
                    } else {
                        // lo < hi：贡献 = dp[lo+1][hi-1] + 2
                        // +2 对应 "c" 和 "cc"
                        dp[i][j] = (dp[i][j] + dp[lo + 1][hi - 1] + 2) % MOD;
                    }
                }
            }
        }

        return dp[0][n - 1];
    }
};
