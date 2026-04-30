// LeetCode 675: Cut Off Trees for Golf Event - Reference Solution
// 难度: Hard
// 标签: BFS, Array, Matrix, Heap (Priority Queue)

// ============================================================
// 解法一: 排序 + 多次 BFS（推荐面试解法）
// ============================================================
// 核心思路:
//   1. 收集所有树（值 > 1），按高度升序排列
//   2. 从 (0,0) 出发，依次 BFS 到每棵目标树
//   3. 累加每段 BFS 的最短距离
//   4. 任意一段不可达则返回 -1
//
// 时间复杂度: O(T * m * n)，T 为树数量，最坏 O(m^2 * n^2)
// 空间复杂度: O(m * n)

class Solution {
public:
    int cutOffTree(vector<vector<int>>& forest) {
        int m = forest.size(), n = forest[0].size();
        if (forest[0][0] == 0) return -1;

        // 步骤1: 收集所有树 {高度, 行, 列}
        vector<tuple<int, int, int>> trees;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (forest[i][j] > 1) {
                    trees.push_back({forest[i][j], i, j});
                }
            }
        }

        // 步骤2: 按高度升序排列，确定砍树顺序
        sort(trees.begin(), trees.end());

        // 步骤3: 依次 BFS 到每棵目标树
        int totalSteps = 0;
        int sr = 0, sc = 0;  // 当前起点

        for (auto& [height, tr, tc] : trees) {
            int steps = bfs(forest, sr, sc, tr, tc, m, n);
            if (steps == -1) return -1;  // 不可达
            totalSteps += steps;
            // 更新起点为当前砍完的树的位置
            sr = tr;
            sc = tc;
        }

        return totalSteps;
    }

private:
    // 标准网格 BFS: 求从 (sr,sc) 到 (tr,tc) 的最短步数
    int bfs(vector<vector<int>>& forest, int sr, int sc,
            int tr, int tc, int m, int n) {
        if (sr == tr && sc == tc) return 0;

        vector<vector<bool>> visited(m, vector<bool>(n, false));
        queue<pair<int, int>> q;
        q.push({sr, sc});
        visited[sr][sc] = true;

        int dirs[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
        int steps = 0;

        while (!q.empty()) {
            steps++;
            int sz = q.size();
            for (int i = 0; i < sz; i++) {
                auto [r, c] = q.front();
                q.pop();
                for (auto& d : dirs) {
                    int nr = r + d[0], nc = c + d[1];
                    if (nr >= 0 && nr < m && nc >= 0 && nc < n
                        && !visited[nr][nc] && forest[nr][nc] > 0) {
                        if (nr == tr && nc == tc) return steps;
                        visited[nr][nc] = true;
                        q.push({nr, nc});
                    }
                }
            }
        }

        return -1;  // 不可达
    }
};

// ============================================================
// 解法二: 排序 + A* 搜索（优化版）
// ============================================================
// 核心思路:
//   用 A* 替代朴素 BFS，启发函数为曼哈顿距离。
//   优先队列按 f = g + h 排序（g 为已走步数，h 为曼哈顿距离）。
//   平均情况下探索节点更少，但最坏复杂度不变。
//
// 时间复杂度: O(T * m * n * log(m*n))  最坏情况
// 空间复杂度: O(m * n)

class Solution2 {
public:
    int cutOffTree(vector<vector<int>>& forest) {
        int m = forest.size(), n = forest[0].size();
        if (forest[0][0] == 0) return -1;

        vector<tuple<int, int, int>> trees;
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                if (forest[i][j] > 1)
                    trees.push_back({forest[i][j], i, j});

        sort(trees.begin(), trees.end());

        int totalSteps = 0;
        int sr = 0, sc = 0;

        for (auto& [height, tr, tc] : trees) {
            int steps = astar(forest, sr, sc, tr, tc, m, n);
            if (steps == -1) return -1;
            totalSteps += steps;
            sr = tr;
            sc = tc;
        }

        return totalSteps;
    }

private:
    int astar(vector<vector<int>>& forest, int sr, int sc,
              int tr, int tc, int m, int n) {
        if (sr == tr && sc == tc) return 0;

        // 优先队列: {f, g, row, col}，f = g + h
        auto cmp = [](tuple<int,int,int,int>& a, tuple<int,int,int,int>& b) {
            return get<0>(a) > get<0>(b);
        };
        priority_queue<tuple<int,int,int,int>,
                       vector<tuple<int,int,int,int>>,
                       decltype(cmp)> pq(cmp);

        vector<vector<int>> dist(m, vector<int>(n, INT_MAX));
        dist[sr][sc] = 0;

        int h0 = abs(sr - tr) + abs(sc - tc);
        pq.push({h0, 0, sr, sc});

        int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};

        while (!pq.empty()) {
            auto [f, g, r, c] = pq.top();
            pq.pop();

            if (r == tr && c == tc) return g;

            // 跳过过时的条目
            if (g > dist[r][c]) continue;

            for (auto& d : dirs) {
                int nr = r + d[0], nc = c + d[1];
                if (nr >= 0 && nr < m && nc >= 0 && nc < n
                    && forest[nr][nc] > 0) {
                    int ng = g + 1;
                    if (ng < dist[nr][nc]) {
                        dist[nr][nc] = ng;
                        int nh = abs(nr - tr) + abs(nc - tc);
                        pq.push({ng + nh, ng, nr, nc});
                    }
                }
            }
        }

        return -1;
    }
};
