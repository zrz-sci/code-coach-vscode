// 解法一：枚举行对 + 统计共同列
// 时间 O(m^2 * n)，空间 O(1)
// 固定两行，统计同时为1的列数cnt，贡献 C(cnt,2) 个角矩形
class Solution {
public:
    int countCornerRectangles(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        int result = 0;

        // 枚举所有行对 (r1, r2)
        for (int r1 = 0; r1 < m; ++r1) {
            for (int r2 = r1 + 1; r2 < m; ++r2) {
                // 统计两行同时为1的列数
                int cnt = 0;
                for (int c = 0; c < n; ++c) {
                    if (grid[r1][c] == 1 && grid[r2][c] == 1) {
                        ++cnt;
                    }
                }
                // 从 cnt 个共同列中选2个，形成矩形
                result += cnt * (cnt - 1) / 2;
            }
        }

        return result;
    }
};

// 解法二：列对计数 DP（增量法）
// 时间 O(m * n^2)（仅对值为1的列操作），空间 O(n^2)
// 逐行扫描，对当前行每对 (c1, c2) 都为1时，
// 之前累积的 count[c1][c2] 即为新增矩形数
class Solution2 {
public:
    int countCornerRectangles(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        int result = 0;

        // count[c1][c2] 表示在之前的行中，列c1和列c2同时为1的行数
        vector<vector<int>> count(n, vector<int>(n, 0));

        for (int r = 0; r < m; ++r) {
            // 收集当前行值为1的列
            vector<int> ones;
            for (int c = 0; c < n; ++c) {
                if (grid[r][c] == 1) {
                    ones.push_back(c);
                }
            }

            // 枚举当前行的列对
            for (int i = 0; i < (int)ones.size(); ++i) {
                for (int j = i + 1; j < (int)ones.size(); ++j) {
                    int c1 = ones[i], c2 = ones[j];
                    // 之前有 count[c1][c2] 行也在这两列同时为1
                    // 每一行都可与当前行配对形成一个矩形
                    result += count[c1][c2];
                    ++count[c1][c2];
                }
            }
        }

        return result;
    }
};

// 解法三：自适应策略 - 根据 m 和 n 大小选择最优方向
// 当 m < n 时用行对枚举，当 m >= n 时用列对枚举
class Solution3 {
public:
    int countCornerRectangles(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();

        if (m <= n) {
            // 枚举行对: O(m^2 * n)
            return countByRowPairs(grid, m, n);
        } else {
            // 转置后枚举行对（等价于枚举列对）
            vector<vector<int>> transposed(n, vector<int>(m));
            for (int i = 0; i < m; ++i) {
                for (int j = 0; j < n; ++j) {
                    transposed[j][i] = grid[i][j];
                }
            }
            return countByRowPairs(transposed, n, m);
        }
    }

private:
    int countByRowPairs(vector<vector<int>>& grid, int rows, int cols) {
        int result = 0;
        for (int r1 = 0; r1 < rows; ++r1) {
            for (int r2 = r1 + 1; r2 < rows; ++r2) {
                int cnt = 0;
                for (int c = 0; c < cols; ++c) {
                    if (grid[r1][c] == 1 && grid[r2][c] == 1) {
                        ++cnt;
                    }
                }
                result += cnt * (cnt - 1) / 2;
            }
        }
        return result;
    }
};
