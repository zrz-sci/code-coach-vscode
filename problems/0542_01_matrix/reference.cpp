/*
 * 【题目本质】
 * 求矩阵中每个格子到最近 0 的曼哈顿距离。
 * 经典多源 BFS / DP 两遍扫描问题。
 *
 * 【解法总览】
 * Solution1: 多源 BFS — 所有 0 同时入队，层序扩展（推荐）
 * Solution2: 动态规划 — 两遍扫描，左上→右下 + 右下→左上
 */

// 解法一：多源 BFS（推荐）
// 思路：所有 0 加入初始队列，BFS 层序扩展，层数即距离
// 时间 O(m*n)，空间 O(m*n)
class Solution1 {
public:
    vector<vector<int>> updateMatrix(vector<vector<int>>& mat) {
        int m = mat.size(), n = mat[0].size();
        vector<vector<int>> dist(m, vector<int>(n, INT_MAX));
        queue<pair<int, int>> q;

        // 所有 0 入队
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (mat[i][j] == 0) {
                    dist[i][j] = 0;
                    q.push({i, j});
                }
            }
        }

        int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};
        while (!q.empty()) {
            auto [x, y] = q.front();
            q.pop();
            for (auto& d : dirs) {
                int nx = x + d[0], ny = y + d[1];
                if (nx >= 0 && nx < m && ny >= 0 && ny < n
                    && dist[nx][ny] > dist[x][y] + 1) {
                    dist[nx][ny] = dist[x][y] + 1;
                    q.push({nx, ny});
                }
            }
        }
        return dist;
    }
};

// 解法二：动态规划（两遍扫描）
// 思路：第一遍从左上到右下（看上/左），第二遍从右下到左上（看下/右）
// 时间 O(m*n)，空间 O(1) 额外（dist 数组是返回值）
class Solution2 {
public:
    vector<vector<int>> updateMatrix(vector<vector<int>>& mat) {
        int m = mat.size(), n = mat[0].size();
        vector<vector<int>> dist(m, vector<int>(n, m + n)); // 最大可能距离

        // 第一遍：左上 → 右下
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (mat[i][j] == 0) {
                    dist[i][j] = 0;
                } else {
                    if (i > 0) dist[i][j] = min(dist[i][j], dist[i-1][j] + 1);
                    if (j > 0) dist[i][j] = min(dist[i][j], dist[i][j-1] + 1);
                }
            }
        }

        // 第二遍：右下 → 左上
        for (int i = m - 1; i >= 0; i--) {
            for (int j = n - 1; j >= 0; j--) {
                if (i < m - 1) dist[i][j] = min(dist[i][j], dist[i+1][j] + 1);
                if (j < n - 1) dist[i][j] = min(dist[i][j], dist[i][j+1] + 1);
            }
        }
        return dist;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 特点 |
 * |------|------|------|------|
 * | 多源BFS | O(m*n) | O(m*n) | 直观，BFS保证最短距离 |
 * | DP两遍 | O(m*n) | O(1)额外 | 无需队列，空间更优 |
 *
 * 【易错点】
 * 1. 不能从每个1出发BFS找0（O(m^2*n^2) 超时）
 * 2. DP 必须两遍扫描，一遍只覆盖两个方向
 * 3. 1 的初始距离必须设为 MAX，否则 min 比较失效
 * 4. BFS 中必须检查 dist[nx][ny] > dist[x][y]+1 才更新入队
 *
 * 【面试追问】
 * Q1: 多源 vs 单源 BFS？→ 多源等价于虚拟超级源点连接所有0
 * Q2: DP 为什么两遍？→ 一遍只覆盖左/上，需要第二遍覆盖右/下
 * Q3: 与 1765 Map of Highest Peak 关系？→ 本质相同题目
 */
