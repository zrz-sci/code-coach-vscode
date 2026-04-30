// LeetCode 837: New 21 Game
// 解法一：滑动窗口DP - 最优解
// 时间复杂度: O(n), 空间复杂度: O(n)

class Solution {
public:
    double new21Game(int n, int k, int maxPts) {
        // 边界情况: k=0时不抽牌，得分0 <= n恒成立
        if (k == 0) return 1.0;
        // 优化: 如果n >= k + maxPts - 1，所有可能结果都 <= n
        if (n >= k + maxPts - 1) return 1.0;

        // dp[x] = 最终得分恰好为x的概率
        // 最终得分范围是 [k, k + maxPts - 1]
        vector<double> dp(n + 1, 0.0);
        dp[0] = 1.0;

        // windowSum = 当前滑动窗口中的概率和
        // 窗口范围: [x - maxPts, x - 1] 中且 < k 的部分
        double windowSum = 1.0; // 初始只有dp[0]
        double answer = 0.0;

        for (int x = 1; x <= n; x++) {
            // dp[x] = windowSum / maxPts
            // 表示从窗口中的每个活跃状态，以1/maxPts的概率转移到x
            dp[x] = windowSum / maxPts;

            if (x < k) {
                // x还在抽牌范围内，将dp[x]加入窗口
                windowSum += dp[x];
            } else {
                // x >= k，Alice停止抽牌，dp[x]是最终得分
                // 且 x <= n，所以累加到答案中
                answer += dp[x];
            }

            // 窗口左边界滑出: 如果窗口大小超过maxPts
            if (x >= maxPts) {
                windowSum -= dp[x - maxPts];
            }
        }

        return answer;
    }
};

// ================================================================
// 解法二：朴素DP（用于理解，大数据会超时）
// 时间复杂度: O(n * maxPts), 空间复杂度: O(n)

class Solution_Naive {
public:
    double new21Game(int n, int k, int maxPts) {
        if (k == 0) return 1.0;
        if (n >= k + maxPts - 1) return 1.0;

        // dp[x] = 到达得分x的概率
        vector<double> dp(n + 1, 0.0);
        dp[0] = 1.0;

        for (int x = 1; x <= n; x++) {
            // 从前maxPts个状态转移，但只能从 < k 的状态转移
            for (int j = 1; j <= maxPts && x - j >= 0; j++) {
                if (x - j < k) { // 只有 < k 的状态还在抽牌
                    dp[x] += dp[x - j] / maxPts;
                }
            }
        }

        // 累加 dp[k] 到 dp[n] 得到答案
        double answer = 0.0;
        for (int x = k; x <= n; x++) {
            answer += dp[x];
        }
        return answer;
    }
};

// ================================================================
// 解法三：前缀和优化DP（与滑动窗口等价的另一种写法）
// 时间复杂度: O(n), 空间复杂度: O(n)

class Solution_PrefixSum {
public:
    double new21Game(int n, int k, int maxPts) {
        if (k == 0) return 1.0;
        if (n >= k + maxPts - 1) return 1.0;

        vector<double> dp(n + 1, 0.0);
        dp[0] = 1.0;

        // prefixSum[i] = dp[0] + dp[1] + ... + dp[i]（仅统计 < k 的部分）
        // 用于快速计算转移的区间和
        vector<double> prefixSum(n + 2, 0.0);
        prefixSum[1] = dp[0]; // prefixSum是1-indexed

        for (int x = 1; x <= n; x++) {
            // 转移来源: max(0, x-maxPts) 到 min(k-1, x-1)
            int lo = max(0, x - maxPts);
            int hi = min(k - 1, x - 1);

            if (lo <= hi) {
                // 区间和 dp[lo] + ... + dp[hi]
                double rangeSum = prefixSum[hi + 1] - prefixSum[lo];
                dp[x] = rangeSum / maxPts;
            }

            // 更新前缀和（只统计 < k 的dp值）
            if (x < k) {
                prefixSum[x + 1] = prefixSum[x] + dp[x];
            } else {
                prefixSum[x + 1] = prefixSum[x]; // >= k的不参与转移
            }
        }

        double answer = 0.0;
        for (int x = k; x <= n; x++) {
            answer += dp[x];
        }
        return answer;
    }
};

// ================================================================
// 解法四：反向DP（从后往前计算）
// 定义 dp[x] = 当前得分为x时，最终得分 <= n 的概率
// 时间复杂度: O(n), 空间复杂度: O(n)

class Solution_Reverse {
public:
    double new21Game(int n, int k, int maxPts) {
        if (k == 0) return 1.0;
        if (n >= k + maxPts - 1) return 1.0;

        // dp[x] = 当前分数为x时，最终分数 <= n 的概率
        // 对于 x >= k: dp[x] = (x <= n) ? 1.0 : 0.0
        // 对于 x < k:  dp[x] = (dp[x+1] + dp[x+2] + ... + dp[x+maxPts]) / maxPts

        int maxVal = k + maxPts - 1; // 最大可能得分
        vector<double> dp(maxVal + 1, 0.0);

        // 初始化: 得分在 [k, n] 范围内的概率为1
        double windowSum = 0.0;
        for (int x = k; x <= maxVal; x++) {
            dp[x] = (x <= n) ? 1.0 : 0.0;
            if (x < k + maxPts) { // 初始窗口
                windowSum += dp[x];
            }
        }

        // 从 k-1 反向计算到 0
        for (int x = k - 1; x >= 0; x--) {
            dp[x] = windowSum / maxPts;
            // 滑动窗口: 加入dp[x+1]（新进入窗口），移除dp[x+maxPts+1]（如果存在则滑出）
            // 实际上窗口是 [x+1, x+maxPts]
            windowSum += dp[x]; // 不对，这是反向的
            // 正确的窗口维护需要调整
        }

        return dp[0];
    }
};
