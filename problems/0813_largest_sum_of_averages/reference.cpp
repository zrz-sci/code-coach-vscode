/*
 * LeetCode 813: Largest Sum of Averages
 *
 * 解法一：二维 DP + 前缀和（推荐）
 * 时间复杂度: O(n^2 * k)
 * 空间复杂度: O(n * k)
 *
 * 思路：
 * dp[i][j] = 前 i 个元素分成 j 组的最大平均值之和
 * 利用前缀和 O(1) 计算子数组平均值
 */

class Solution {
public:
    double largestSumOfAverages(vector<int>& nums, int k) {
        int n = nums.size();

        // 构建前缀和
        vector<double> prefix(n + 1, 0.0);
        for (int i = 0; i < n; ++i) {
            prefix[i + 1] = prefix[i] + nums[i];
        }

        // dp[i][j] = 前 i 个元素分成 j 组的最大平均值之和
        vector<vector<double>> dp(n + 1, vector<double>(k + 1, 0.0));

        // 基础情况：分成 1 组
        for (int i = 1; i <= n; ++i) {
            dp[i][1] = prefix[i] / i;
        }

        // 枚举分组数 j 从 2 到 k
        for (int j = 2; j <= k; ++j) {
            // 至少需要 j 个元素才能分成 j 组
            for (int i = j; i <= n; ++i) {
                // 枚举最后一组的起始位置
                // 前 m 个元素分 j-1 组，nums[m..i-1] 是第 j 组
                for (int m = j - 1; m < i; ++m) {
                    double lastGroupAvg = (prefix[i] - prefix[m]) / (i - m);
                    dp[i][j] = max(dp[i][j], dp[m][j - 1] + lastGroupAvg);
                }
            }
        }

        return dp[n][k];
    }
};


/*
 * 解法二：一维 DP（空间优化）
 * 时间复杂度: O(n^2 * k)
 * 空间复杂度: O(n)
 *
 * 思路：
 * 由于 dp[i][j] 只依赖 dp[*][j-1]，可以用一维数组滚动更新。
 * 关键：内层循环必须逆序遍历，避免覆盖上一轮的值。
 */

class SolutionOptimized {
public:
    double largestSumOfAverages(vector<int>& nums, int k) {
        int n = nums.size();

        // 构建前缀和
        vector<double> prefix(n + 1, 0.0);
        for (int i = 0; i < n; ++i) {
            prefix[i + 1] = prefix[i] + nums[i];
        }

        // 一维 dp，初始化为 1 组的情况
        vector<double> dp(n + 1, 0.0);
        for (int i = 1; i <= n; ++i) {
            dp[i] = prefix[i] / i;
        }

        // 枚举分组数
        for (int j = 2; j <= k; ++j) {
            // 逆序遍历 i，防止覆盖旧值
            for (int i = n; i >= j; --i) {
                for (int m = j - 1; m < i; ++m) {
                    double lastGroupAvg = (prefix[i] - prefix[m]) / (i - m);
                    dp[i] = max(dp[i], dp[m] + lastGroupAvg);
                }
            }
        }

        return dp[n];
    }
};


/*
 * 解法三：记忆化搜索（Top-Down）
 * 时间复杂度: O(n^2 * k)
 * 空间复杂度: O(n * k)
 *
 * 思路：
 * dfs(start, groups) = 从 start 开始分 groups 组的最大平均值之和
 * 枚举当前段的结束位置，递归处理剩余部分
 * 更直观，适合面试时快速实现
 */

class SolutionMemo {
public:
    double largestSumOfAverages(vector<int>& nums, int k) {
        int n = nums.size();

        // 前缀和
        vector<double> prefix(n + 1, 0.0);
        for (int i = 0; i < n; ++i) {
            prefix[i + 1] = prefix[i] + nums[i];
        }

        // 记忆化表：memo[start][groups]
        vector<vector<double>> memo(n, vector<double>(k + 1, -1.0));

        // 辅助函数：计算 nums[l..r] 的平均值（0-indexed，闭区间）
        auto avg = [&](int l, int r) -> double {
            return (prefix[r + 1] - prefix[l]) / (r - l + 1);
        };

        // 递归函数
        function<double(int, int)> dfs = [&](int start, int groups) -> double {
            // 只剩一组，把剩余元素全放进去
            if (groups == 1) {
                return avg(start, n - 1);
            }

            // 查表
            if (memo[start][groups] >= 0) {
                return memo[start][groups];
            }

            double best = 0.0;
            // 枚举当前段结束位置 end
            // 剩余 n - end - 1 个元素需要至少 groups - 1 组
            // 所以 end <= n - groups
            for (int end = start; end <= n - groups; ++end) {
                double current = avg(start, end) + dfs(end + 1, groups - 1);
                best = max(best, current);
            }

            memo[start][groups] = best;
            return best;
        };

        return dfs(0, k);
    }
};


/*
 * 补充说明：为什么"最多k组"等价于"恰好k组"
 *
 * 数学证明：
 * 假设最优解分成了 j < k 组，其中某组有 >=2 个元素 [a1, a2, ..., am]。
 * 设 avg = (a1 + a2 + ... + am) / m。
 *
 * 如果我们把 a1 单独分出来：
 * - 原来的贡献：avg = (a1 + rest_sum) / m
 * - 拆分后贡献：a1 + rest_sum / (m-1)
 *
 * 拆分后 - 拆分前 = a1 + rest_sum/(m-1) - (a1 + rest_sum)/m
 *                  = a1 * (1 - 1/m) + rest_sum * (1/(m-1) - 1/m)
 *                  = a1 * (m-1)/m + rest_sum / (m*(m-1))
 *                  > 0 （因为 a1 >= 1, rest_sum >= 0, m >= 2）
 *
 * 所以拆分总是有利的，分 k 组一定 >= 分 j < k 组。
 */
