// 727. Minimum Window Subsequence
// 难度: Hard
// 标签: String, Dynamic Programming, Sliding Window

// ===================== 解法一：前向匹配 + 后向收缩（推荐） =====================
// 时间 O(n * m)  空间 O(1)
// 思路：先正向贪心匹配 s2 找到窗口右端点，
//       再反向贪心收缩找到窗口左端点。取所有候选窗口中最短的。
class Solution {
public:
    string minWindow(string s1, string s2) {
        int n = s1.size(), m = s2.size();
        int bestLeft = -1, bestLen = INT_MAX;

        int i = 0; // s1 的指针
        while (i < n) {
            // ===== 阶段一：正向匹配，找到包含 s2 子序列的窗口右端 =====
            int j = 0;
            while (i < n && j < m) {
                if (s1[i] == s2[j]) {
                    j++;
                }
                i++;
            }
            // 如果没有完成匹配，不可能再找到更多窗口
            if (j < m) break;

            // i-1 是窗口右端点（因为 i 在匹配成功后多加了 1）
            int right = i - 1;

            // ===== 阶段二：反向收缩，从右端点往左找最优左端点 =====
            j = m - 1;
            int k = right;
            while (k >= 0 && j >= 0) {
                if (s1[k] == s2[j]) {
                    j--;
                }
                k--;
            }
            // k+1 是窗口左端点
            int left = k + 1;

            // 更新最优结果
            int curLen = right - left + 1;
            if (curLen < bestLen) {
                bestLen = curLen;
                bestLeft = left;
            }

            // 从 left+1 继续寻找下一个候选窗口
            i = left + 1;
        }

        if (bestLeft == -1) return "";
        return s1.substr(bestLeft, bestLen);
    }
};

// ===================== 解法二：动态规划 =====================
// 时间 O(n * m)  空间 O(n * m)
// dp[i][j] = 在 s1[0..i-1] 中使 s2[0..j-1] 成为子序列的最大起始索引
// 值越大说明窗口越短。-1 表示不可行。
class Solution2 {
public:
    string minWindow(string s1, string s2) {
        int n = s1.size(), m = s2.size();

        // dp[i][j]: s1 前 i 个字符中，s2 前 j 个字符作为子序列的最晚起始位置
        // 初始化为 -1 表示不可行
        vector<vector<int>> dp(n + 1, vector<int>(m + 1, -1));

        // s2 前 0 个字符（空串）总是 s1 任意位置的子序列
        // dp[i][0] 不需要使用，因为空子序列没有起始位置的意义

        int bestLeft = -1, bestLen = INT_MAX;

        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                if (s1[i - 1] == s2[j - 1]) {
                    if (j == 1) {
                        // s2 的第一个字符匹配，起始位置就是 i-1
                        dp[i][j] = i - 1;
                    } else {
                        // 继承前一个状态的起始位置
                        dp[i][j] = dp[i - 1][j - 1];
                    }
                } else {
                    // 当前字符不匹配，继承上一行
                    dp[i][j] = dp[i - 1][j];
                }
            }

            // 检查是否完成了 s2 的完整匹配
            if (dp[i][m] != -1) {
                int start = dp[i][m];
                int len = i - start; // 窗口长度 = 右端 i-1 - 左端 start + 1 = i - start
                if (len < bestLen) {
                    bestLen = len;
                    bestLeft = start;
                }
            }
        }

        if (bestLeft == -1) return "";
        return s1.substr(bestLeft, bestLen);
    }
};

// ===================== 解法三：DP 空间优化版 =====================
// 时间 O(n * m)  空间 O(m)
// 观察到 dp[i][j] 只依赖 dp[i-1][j-1] 和 dp[i-1][j]，
// 可以用一维数组 + 临时变量完成。
class Solution3 {
public:
    string minWindow(string s1, string s2) {
        int n = s1.size(), m = s2.size();

        // dp[j] = 当前 s1 位置下，使 s2 前 j 个字符成为子序列的最晚起始位置
        vector<int> dp(m + 1, -1);

        int bestLeft = -1, bestLen = INT_MAX;

        for (int i = 1; i <= n; i++) {
            // 需要保存 dp[j-1] 的旧值，因为更新 dp[j] 时会用到
            int prev = -1; // dp[i-1][j-1] 的值
            for (int j = 1; j <= m; j++) {
                int oldDpJ = dp[j]; // 保存 dp[i-1][j] 用于下一轮作为 prev
                if (s1[i - 1] == s2[j - 1]) {
                    if (j == 1) {
                        dp[j] = i - 1;
                    } else {
                        dp[j] = prev;
                    }
                }
                // 若不匹配，dp[j] 保持不变（即 dp[i-1][j]）
                prev = oldDpJ;
            }

            if (dp[m] != -1) {
                int start = dp[m];
                int len = i - start;
                if (len < bestLen) {
                    bestLen = len;
                    bestLeft = start;
                }
            }
        }

        if (bestLeft == -1) return "";
        return s1.substr(bestLeft, bestLen);
    }
};

/*
 * 总结：
 * - 解法一（前向+后向双指针）最为直觉，空间 O(1)，面试中首推
 * - 解法二（DP）逻辑清晰，适合需要讨论状态转移的场景
 * - 解法三（DP 空间优化）展示了滚动数组技巧
 *
 * 与 LC 76 的关键区别：
 * - LC 76 是字符集合匹配（顺序无关），用经典滑动窗口
 * - LC 727 是子序列匹配（顺序有关），滑动窗口不直接适用
 * - 本题的"后向收缩"本质上替代了滑动窗口中"左指针右移"的角色
 */
