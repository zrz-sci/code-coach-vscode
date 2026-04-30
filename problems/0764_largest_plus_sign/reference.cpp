// 解法一：四方向 DP + 单数组优化（最优解）
// 时间 O(n^2)，空间 O(n^2)
// 核心思路：用一个 DP 数组记录每个位置四方向连续 1 的最小臂长
class Solution {
public:
    int orderOfLargestPlusSign(int n, vector<vector<int>>& mines) {
        // 用 unordered_set 快速查找 mine 位置
        // 将二维坐标编码为一维: r * n + c
        unordered_set<int> mineSet;
        for (auto& mine : mines) {
            mineSet.insert(mine[0] * n + mine[1]);
        }

        // dp[r][c] 表示以 (r,c) 为中心的最大加号阶数
        // 初始化为 n（最大可能值），之后四次扫描取 min
        vector<vector<int>> dp(n, vector<int>(n, n));

        // 将 mine 位置设为 0（不可能形成加号）
        for (auto& mine : mines) {
            dp[mine[0]][mine[1]] = 0;
        }

        // 四次扫描：对每个位置，用当前方向的连续 1 计数更新 dp
        for (int r = 0; r < n; ++r) {
            // 从左到右：计算每个位置向左的连续 1 长度
            int count = 0;
            for (int c = 0; c < n; ++c) {
                count = mineSet.count(r * n + c) ? 0 : count + 1;
                dp[r][c] = min(dp[r][c], count);
            }
            // 从右到左：计算每个位置向右的连续 1 长度
            count = 0;
            for (int c = n - 1; c >= 0; --c) {
                count = mineSet.count(r * n + c) ? 0 : count + 1;
                dp[r][c] = min(dp[r][c], count);
            }
        }

        int ans = 0;
        for (int c = 0; c < n; ++c) {
            // 从上到下：计算每个位置向上的连续 1 长度
            int count = 0;
            for (int r = 0; r < n; ++r) {
                count = mineSet.count(r * n + c) ? 0 : count + 1;
                dp[r][c] = min(dp[r][c], count);
            }
            // 从下到上：计算每个位置向下的连续 1 长度
            count = 0;
            for (int r = n - 1; r >= 0; --r) {
                count = mineSet.count(r * n + c) ? 0 : count + 1;
                dp[r][c] = min(dp[r][c], count);
                // 最后一个方向扫完后，dp[r][c] 就是最终结果
                ans = max(ans, dp[r][c]);
            }
        }

        return ans;
    }
};

// 解法二：四方向 DP（四数组版本，代码最清晰）
// 时间 O(n^2)，空间 O(n^2)
// 核心思路：分别计算四个方向的连续 1 长度，最终取 min 的 max
class Solution2 {
public:
    int orderOfLargestPlusSign(int n, vector<vector<int>>& mines) {
        // 构建网格
        vector<vector<int>> grid(n, vector<int>(n, 1));
        for (auto& mine : mines) {
            grid[mine[0]][mine[1]] = 0;
        }

        // 四个方向的连续 1 长度数组
        vector<vector<int>> left(n, vector<int>(n, 0));
        vector<vector<int>> right(n, vector<int>(n, 0));
        vector<vector<int>> up(n, vector<int>(n, 0));
        vector<vector<int>> down(n, vector<int>(n, 0));

        // 计算 left: 从左到右，连续 1 计数（含自身）
        for (int r = 0; r < n; ++r) {
            for (int c = 0; c < n; ++c) {
                left[r][c] = grid[r][c] == 0 ? 0
                           : (c > 0 ? left[r][c - 1] + 1 : 1);
            }
        }

        // 计算 right: 从右到左
        for (int r = 0; r < n; ++r) {
            for (int c = n - 1; c >= 0; --c) {
                right[r][c] = grid[r][c] == 0 ? 0
                            : (c < n - 1 ? right[r][c + 1] + 1 : 1);
            }
        }

        // 计算 up: 从上到下
        for (int c = 0; c < n; ++c) {
            for (int r = 0; r < n; ++r) {
                up[r][c] = grid[r][c] == 0 ? 0
                         : (r > 0 ? up[r - 1][c] + 1 : 1);
            }
        }

        // 计算 down: 从下到上
        for (int c = 0; c < n; ++c) {
            for (int r = n - 1; r >= 0; --r) {
                down[r][c] = grid[r][c] == 0 ? 0
                           : (r < n - 1 ? down[r + 1][c] + 1 : 1);
            }
        }

        // 遍历所有位置，取四方向最小值中的最大值
        int ans = 0;
        for (int r = 0; r < n; ++r) {
            for (int c = 0; c < n; ++c) {
                int order = min({left[r][c], right[r][c],
                                 up[r][c], down[r][c]});
                ans = max(ans, order);
            }
        }

        return ans;
    }
};

// 解法三：暴力扩展法（便于理解，面试可作为初始思路）
// 时间 O(n^3)，空间 O(n^2)
// 核心思路：对每个位置，向四个方向逐步扩展，检查最大可行阶数
class Solution3 {
public:
    int orderOfLargestPlusSign(int n, vector<vector<int>>& mines) {
        // 构建网格
        vector<vector<int>> grid(n, vector<int>(n, 1));
        for (auto& mine : mines) {
            grid[mine[0]][mine[1]] = 0;
        }

        int ans = 0;
        for (int r = 0; r < n; ++r) {
            for (int c = 0; c < n; ++c) {
                if (grid[r][c] == 0) continue;

                // 从中心向外逐步扩展
                int k = 1;
                while (r - k >= 0 && r + k < n &&
                       c - k >= 0 && c + k < n) {
                    // 检查四个方向第 k 步是否都为 1
                    if (grid[r - k][c] && grid[r + k][c] &&
                        grid[r][c - k] && grid[r][c + k]) {
                        ++k;
                    } else {
                        break;
                    }
                }
                // k 即为以 (r,c) 为中心的最大加号阶数
                ans = max(ans, k);
            }
        }

        return ans;
    }
};
