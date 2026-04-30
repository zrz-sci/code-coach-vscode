// LeetCode 840: Magic Squares In Grid
// 解法一：暴力枚举 + 完整验证（推荐）
// 时间 O(R*C)，空间 O(1)
// 遍历所有 3x3 子网格，验证是否为幻方

class Solution {
public:
    int numMagicSquaresInside(vector<vector<int>>& grid) {
        int rows = grid.size(), cols = grid[0].size();
        int count = 0;
        for (int i = 0; i <= rows - 3; ++i) {
            for (int j = 0; j <= cols - 3; ++j) {
                if (isMagic(grid, i, j)) {
                    ++count;
                }
            }
        }
        return count;
    }

private:
    bool isMagic(vector<vector<int>>& grid, int r, int c) {
        // 剪枝：中心必须是 5
        if (grid[r + 1][c + 1] != 5) return false;

        // 检查 1-9 不重复
        vector<bool> seen(10, false);
        for (int i = r; i < r + 3; ++i) {
            for (int j = c; j < c + 3; ++j) {
                int val = grid[i][j];
                if (val < 1 || val > 9 || seen[val]) return false;
                seen[val] = true;
            }
        }

        // 检查行和
        for (int i = r; i < r + 3; ++i) {
            if (grid[i][c] + grid[i][c + 1] + grid[i][c + 2] != 15)
                return false;
        }

        // 检查列和
        for (int j = c; j < c + 3; ++j) {
            if (grid[r][j] + grid[r + 1][j] + grid[r + 2][j] != 15)
                return false;
        }

        // 检查对角线和
        if (grid[r][c] + grid[r + 1][c + 1] + grid[r + 2][c + 2] != 15)
            return false;
        if (grid[r][c + 2] + grid[r + 1][c + 1] + grid[r + 2][c] != 15)
            return false;

        return true;
    }
};

// ============================================================
// 解法二：预计算所有 8 种 3x3 幻方模板进行匹配
// 时间 O(R*C*8)，空间 O(1)
// 3x3 幻方仅有 8 种排列（旋转+翻转）

class Solution2 {
public:
    // 所有 8 种 3x3 幻方（展平为一维）
    vector<vector<int>> magicSquares = {
        {2,7,6,9,5,1,4,3,8},
        {6,1,8,7,5,3,2,9,4},
        {8,3,4,1,5,9,6,7,2},
        {4,9,2,3,5,7,8,1,6},
        {2,9,4,7,5,3,6,1,8},
        {4,3,8,9,5,1,2,7,6},
        {8,1,6,3,5,7,4,9,2},
        {6,7,2,1,5,9,8,3,4}
    };

    int numMagicSquaresInside(vector<vector<int>>& grid) {
        int rows = grid.size(), cols = grid[0].size();
        int count = 0;
        for (int i = 0; i <= rows - 3; ++i) {
            for (int j = 0; j <= cols - 3; ++j) {
                if (matchAnyMagic(grid, i, j)) {
                    ++count;
                }
            }
        }
        return count;
    }

private:
    bool matchAnyMagic(vector<vector<int>>& grid, int r, int c) {
        // 提取 3x3 子网格为一维
        vector<int> flat;
        for (int i = r; i < r + 3; ++i) {
            for (int j = c; j < c + 3; ++j) {
                flat.push_back(grid[i][j]);
            }
        }
        // 与所有 8 种幻方模板比较
        for (auto& magic : magicSquares) {
            if (flat == magic) return true;
        }
        return false;
    }
};

// ============================================================
// 解法三：利用数学性质逐步验证
// 时间 O(R*C)，空间 O(1)
// 在验证过程中利用更多数学约束提前退出

class Solution3 {
public:
    int numMagicSquaresInside(vector<vector<int>>& grid) {
        int rows = grid.size(), cols = grid[0].size();
        int count = 0;
        for (int i = 0; i <= rows - 3; ++i) {
            for (int j = 0; j <= cols - 3; ++j) {
                if (isMagicOptimized(grid, i, j)) {
                    ++count;
                }
            }
        }
        return count;
    }

private:
    bool isMagicOptimized(vector<vector<int>>& grid, int r, int c) {
        // 步骤 1：中心必须是 5
        if (grid[r + 1][c + 1] != 5) return false;

        // 步骤 2：所有数必须在 [1,9] 且不重复
        int mask = 0;
        for (int i = r; i < r + 3; ++i) {
            for (int j = c; j < c + 3; ++j) {
                int v = grid[i][j];
                if (v < 1 || v > 9) return false;
                int bit = 1 << v;
                if (mask & bit) return false;
                mask |= bit;
            }
        }
        // mask 应该是 0b1111111110 = 1022
        if (mask != 1022) return false;

        // 步骤 3：四角之和必须为 20
        int corners = grid[r][c] + grid[r][c + 2] +
                      grid[r + 2][c] + grid[r + 2][c + 2];
        if (corners != 20) return false;

        // 步骤 4：验证所有行列对角线和为 15
        // 由于已知中心=5、四角和=20、数字不重复，
        // 此时只需验证少量条件
        for (int i = r; i < r + 3; ++i) {
            if (grid[i][c] + grid[i][c + 1] + grid[i][c + 2] != 15)
                return false;
        }
        for (int j = c; j < c + 3; ++j) {
            if (grid[r][j] + grid[r + 1][j] + grid[r + 2][j] != 15)
                return false;
        }
        // 对角线（可选验证，因为前面条件已较强）
        if (grid[r][c] + 5 + grid[r + 2][c + 2] != 15) return false;
        if (grid[r][c + 2] + 5 + grid[r + 2][c] != 15) return false;

        return true;
    }
};
