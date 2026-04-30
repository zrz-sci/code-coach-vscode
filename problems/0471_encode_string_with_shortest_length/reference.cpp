/*
 * LeetCode 471: Encode String with Shortest Length
 * 难度: Hard | 标签: String, Dynamic Programming
 *
 * 核心思路: 区间DP + 拼接取余发现重复模式
 *
 * 拼接取余技巧:
 *   t = "abcabc" (len=6)
 *   t+t = "abcabcabcabc"
 *   find(t, 1) = 3  <  len=6
 *   => t = "abc" 重复 6/3=2 次
 *   编码: 2[abc]
 *
 * DP定义: dp[i][j] = s[i..j] 的最短编码字符串
 *
 * 转移:
 *   1) dp[i][j] = s[i..j]  (不编码)
 *   2) dp[i][j] = k[dp[i][i+pos-1]]  (整体编码，如果有重复)
 *   3) dp[i][j] = dp[i][mid] + dp[mid+1][j]  (分割)
 *   取最短
 *
 * 嵌套示例:
 *   "abbbabbbcabbbabbbc"
 *   -> dp["abbb"] = "abbb"
 *   -> dp["abbbabbb"] = "2[abbb]"
 *   -> dp["abbbabbbc"] = "2[abbb]c"
 *   -> dp[全串] = "2[2[abbb]c]"
 */


// ================================================================
// 解法1: 区间DP + 拼接取余 -- O(n^3) 时间, O(n^2) 空间
// ================================================================
// 从短到长枚举所有子串
// 每个子串:
//   1. 默认不编码 (原串)
//   2. 检查重复模式 (拼接取余)
//   3. 枚举分割点
// 取三者最短
//
// 长度阈值: len <= 4 时编码不可能更短
//   "2[a]" = 4字符, "aa" = 2字符  -> 编码更长
//   "5[a]" = 4字符, "aaaaa" = 5字符  -> 编码更短
class Solution {
public:
    string encode(string s) {
        int n = s.size();
        // dp[i][j] = s[i..j] 的最短编码
        vector<vector<string>> dp(n, vector<string>(n));

        for (int len = 1; len <= n; len++) {
            for (int i = 0; i + len - 1 < n; i++) {
                int j = i + len - 1;
                string sub = s.substr(i, len);
                dp[i][j] = sub;  // 默认: 不编码

                if (len <= 4) continue;  // 太短，不可能编码更短

                // ----- 尝试整体编码 -----
                // 拼接取余: 在 sub+sub 中从位置1查找 sub
                string doubled = sub + sub;
                int pos = doubled.find(sub, 1);
                if (pos < (int)sub.size()) {
                    // sub 由长度 pos 的前缀重复 len/pos 次
                    // 注意: 内部使用 dp[i][i+pos-1] (可能已被递归编码)
                    string candidate = to_string(len / pos)
                                     + "[" + dp[i][i + pos - 1] + "]";
                    if (candidate.size() < dp[i][j].size()) {
                        dp[i][j] = candidate;
                    }
                }

                // ----- 尝试分割 -----
                for (int k = i; k < j; k++) {
                    string combined = dp[i][k] + dp[k + 1][j];
                    if (combined.size() < dp[i][j].size()) {
                        dp[i][j] = combined;
                    }
                }
            }
        }
        return dp[0][n - 1];
    }
};


// ================================================================
// 解法2: 记忆化递归写法 -- O(n^3) 时间, O(n^2) 空间
// ================================================================
// 逻辑相同，自顶向下递归实现
// 优点: 只计算实际需要的子问题
class Solution2 {
public:
    string encode(string s) {
        int n = s.size();
        vector<vector<string>> memo(n, vector<string>(n));
        return solve(s, 0, n - 1, memo);
    }

private:
    string solve(const string& s, int i, int j,
                 vector<vector<string>>& memo) {
        if (!memo[i][j].empty()) return memo[i][j];

        int len = j - i + 1;
        string sub = s.substr(i, len);
        memo[i][j] = sub;  // 默认不编码

        if (len <= 4) return memo[i][j];

        // 检查重复模式
        string doubled = sub + sub;
        int pos = doubled.find(sub, 1);
        if (pos < len) {
            string candidate = to_string(len / pos)
                             + "[" + solve(s, i, i + pos - 1, memo) + "]";
            if (candidate.size() < memo[i][j].size()) {
                memo[i][j] = candidate;
            }
        }

        // 分割
        for (int k = i; k < j; k++) {
            string left = solve(s, i, k, memo);
            string right = solve(s, k + 1, j, memo);
            string combined = left + right;
            if (combined.size() < memo[i][j].size()) {
                memo[i][j] = combined;
            }
        }
        return memo[i][j];
    }
};


/*
 * ================================================================
 * 复杂度分析:
 *
 * 时间: O(n^3)
 *   - 枚举子串: O(n^2)
 *   - 每个子串: 分割O(n) + find O(n)
 *   - 总共: O(n^3)
 *
 * 空间: O(n^2)
 *   - dp表存储 O(n^2) 个字符串
 *   - 每个字符串最长 O(n)
 *   - 实际总空间 O(n^3)，但均摊后为 O(n^2)
 *
 * ================================================================
 * 拼接取余技巧详解:
 *
 * 为什么 (t+t).find(t, 1) < len(t) 意味着 t 有重复模式?
 *
 * 如果 t = p * k (p重复k次)，例:
 *   t = "abcabc" = "abc" * 2
 *   t+t = "abcabcabcabc" = "abc" * 4
 *   从位置1找: ...bcabcabc|abcabc... 位置3找到 -> pos=3 = len(p)
 *
 * 如果 t 没有重复模式:
 *   t = "abcd"
 *   t+t = "abcdabcd"
 *   从位置1找: pos=4 = len(t) -> 不是重复
 *
 * ================================================================
 * 易错点:
 *
 * 1. find起始位置: 必须从1开始，否则位置0立即匹配自身
 * 2. 内部子串必须用dp值: dp[i][i+pos-1]可能已被进一步编码
 * 3. 长度阈值: <= 4 时编码 k[x] 不可能更短
 * 4. 比较用 .size(): 字符串比较应比长度，不是字典序
 *
 * ================================================================
 * 面试追问:
 *
 * Q1: 拼接取余的数学原理?
 *     等价于 KMP 中的 failure function。pos 就是最短周期长度。
 *     t+t 中 pos 1 处找 t, 如果 pos < len(t), 则 t[0..pos-1] 是周期。
 *
 * Q2: 为什么阈值是4?
 *     编码 "k[x]" 至少占 k的位数+2(括号) 字符。最短编码 "2[a]"=4字符。
 *     原串 <= 4 字符时编码不可能更短。
 *
 * Q3: 解码怎么做? (反向问题)
 *     LC 394 Decode String。用栈处理嵌套括号。
 * ================================================================
 */
