// LeetCode 467: Unique Substrings in Wraparound String
// 解法：动态规划 + 按结尾字母去重
// 时间 O(n)，空间 O(1)

class Solution {
public:
    int findSubstringInWraproundString(string s) {
        // dp[c] = 以字母 c 结尾的最长连续子串长度
        vector<int> dp(26, 0);
        int len = 0;

        for (int i = 0; i < s.size(); i++) {
            // 判断当前字符是否紧接前一个字符（含 z->a 环绕）
            if (i > 0 && (s[i] - s[i - 1] + 26) % 26 == 1) {
                len++;
            } else {
                len = 1;
            }
            // 更新以当前字母结尾的最长连续长度
            dp[s[i] - 'a'] = max(dp[s[i] - 'a'], len);
        }

        // 答案 = 所有字母对应最长长度之和
        int ans = 0;
        for (int x : dp) ans += x;
        return ans;
    }
};
