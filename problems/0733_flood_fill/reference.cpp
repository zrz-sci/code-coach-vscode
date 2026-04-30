// 733. Flood Fill
// 难度: Easy
// 标签: Depth-First Search, Breadth-First Search, Array, Matrix

// ===================== 解法一：DFS 递归（推荐） =====================
// 时间 O(m * n)  空间 O(m * n)  递归栈深度
// 思路：从起点出发递归染色所有同色相邻像素
class Solution {
public:
    vector<vector<int>> floodFill(vector<vector<int>>& image, int sr, int sc, int color) {
        int originalColor = image[sr][sc];
        // 关键：新颜色等于原颜色时无需操作，否则会无限递归
        if (originalColor == color) return image;

        dfs(image, sr, sc, originalColor, color);
        return image;
    }

private:
    // 四方向偏移量
    int dx[4] = {0, 0, 1, -1};
    int dy[4] = {1, -1, 0, 0};

    void dfs(vector<vector<int>>& image, int r, int c, int origColor, int newColor) {
        int m = image.size(), n = image[0].size();

        // 边界检查 + 颜色检查
        if (r < 0 || r >= m || c < 0 || c >= n) return;
        if (image[r][c] != origColor) return;

        // 染色（同时作为 visited 标记）
        image[r][c] = newColor;

        // 向四个方向递归
        for (int d = 0; d < 4; d++) {
            dfs(image, r + dx[d], c + dy[d], origColor, newColor);
        }
    }
};

// ===================== 解法二：BFS（推荐） =====================
// 时间 O(m * n)  空间 O(m * n)  队列大小
// 思路：从起点出发，用队列进行层序扩展染色
class Solution2 {
public:
    vector<vector<int>> floodFill(vector<vector<int>>& image, int sr, int sc, int color) {
        int originalColor = image[sr][sc];
        if (originalColor == color) return image;

        int m = image.size(), n = image[0].size();
        int dx[4] = {0, 0, 1, -1};
        int dy[4] = {1, -1, 0, 0};

        queue<pair<int, int>> q;
        q.push({sr, sc});
        image[sr][sc] = color; // 入队时就染色，防止重复入队

        while (!q.empty()) {
            auto [r, c] = q.front();
            q.pop();

            for (int d = 0; d < 4; d++) {
                int nr = r + dx[d];
                int nc = c + dy[d];

                // 边界检查 + 颜色检查
                if (nr >= 0 && nr < m && nc >= 0 && nc < n
                    && image[nr][nc] == originalColor) {
                    image[nr][nc] = color;
                    q.push({nr, nc});
                }
            }
        }
        return image;
    }
};

// ===================== 解法三：DFS 显式栈 =====================
// 时间 O(m * n)  空间 O(m * n)
// 思路：用显式栈替代递归，避免大矩阵的栈溢出风险
class Solution3 {
public:
    vector<vector<int>> floodFill(vector<vector<int>>& image, int sr, int sc, int color) {
        int originalColor = image[sr][sc];
        if (originalColor == color) return image;

        int m = image.size(), n = image[0].size();
        int dx[4] = {0, 0, 1, -1};
        int dy[4] = {1, -1, 0, 0};

        stack<pair<int, int>> stk;
        stk.push({sr, sc});
        image[sr][sc] = color;

        while (!stk.empty()) {
            auto [r, c] = stk.top();
            stk.pop();

            for (int d = 0; d < 4; d++) {
                int nr = r + dx[d];
                int nc = c + dy[d];

                if (nr >= 0 && nr < m && nc >= 0 && nc < n
                    && image[nr][nc] == originalColor) {
                    image[nr][nc] = color;
                    stk.push({nr, nc});
                }
            }
        }
        return image;
    }
};

// ===================== 解法四：DFS 精简写法 =====================
// 时间 O(m * n)  空间 O(m * n)
// 思路：将边界检查和颜色检查都放在递归入口，代码最短
class Solution4 {
public:
    vector<vector<int>> floodFill(vector<vector<int>>& image, int sr, int sc, int color) {
        int orig = image[sr][sc];
        if (orig != color) fill(image, sr, sc, orig, color);
        return image;
    }

private:
    void fill(vector<vector<int>>& img, int r, int c, int o, int n) {
        if (r < 0 || r >= (int)img.size() ||
            c < 0 || c >= (int)img[0].size() ||
            img[r][c] != o) return;

        img[r][c] = n;
        fill(img, r - 1, c, o, n);
        fill(img, r + 1, c, o, n);
        fill(img, r, c - 1, o, n);
        fill(img, r, c + 1, o, n);
    }
};

// ===================== 解法五：BFS + visited 数组（通用模板） =====================
// 时间 O(m * n)  空间 O(m * n)
// 思路：当新颜色可能等于其他已有颜色时，用 visited 数组保证正确性
//       本题已通过提前返回处理了这个问题，此解法作为更通用的模板
class Solution5 {
public:
    vector<vector<int>> floodFill(vector<vector<int>>& image, int sr, int sc, int color) {
        int m = image.size(), n = image[0].size();
        int originalColor = image[sr][sc];

        // 使用 visited 数组，不依赖颜色变化来标记
        vector<vector<bool>> visited(m, vector<bool>(n, false));
        int dx[4] = {0, 0, 1, -1};
        int dy[4] = {1, -1, 0, 0};

        queue<pair<int, int>> q;
        q.push({sr, sc});
        visited[sr][sc] = true;
        image[sr][sc] = color;

        while (!q.empty()) {
            auto [r, c] = q.front();
            q.pop();

            for (int d = 0; d < 4; d++) {
                int nr = r + dx[d];
                int nc = c + dy[d];

                if (nr >= 0 && nr < m && nc >= 0 && nc < n
                    && !visited[nr][nc]
                    && image[nr][nc] == originalColor) {
                    visited[nr][nc] = true;
                    image[nr][nc] = color;
                    q.push({nr, nc});
                }
            }
        }
        return image;
    }
};

/*
 * 总结：
 * - 解法一（DFS 递归）是面试中最推荐的写法：简洁直观
 * - 解法二（BFS）适合大矩阵场景，无栈溢出风险
 * - 解法三（显式栈 DFS）结合了两者优点
 * - 解法五（visited 数组）是通用模板，不依赖颜色变化标记
 *
 * 核心要点：
 * 1. 必须处理 originalColor == color 的边界情况
 * 2. 入队/递归前检查边界和颜色
 * 3. 染色操作本身可以替代 visited 标记（当新旧颜色不同时）
 * 4. 四方向遍历是矩阵搜索的标准模式
 */
