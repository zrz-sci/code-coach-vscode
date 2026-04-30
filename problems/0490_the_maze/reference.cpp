/*
 * 490. The Maze
 * 难度: Medium | 标签: DFS, BFS, Array, Matrix
 *
 * 【题目本质】
 * 球在迷宫中沿一个方向滚动直到撞墙才停下。判断球能否从 start 停在 destination。
 * 关键: 搜索节点是"停止点"而非每个格子, 需要模拟球的滚动过程。
 *
 * 【解法总览】
 * Solution1: BFS  O(m*n*(m+n))  O(m*n)  ⭐ 推荐
 * Solution2: DFS  O(m*n*(m+n))  O(m*n)  代码简洁
 */

// ===================== Solution1: BFS =====================
// 从 start 出发 BFS, 每次向四个方向滚到底, 找到所有可达的停止点
class Solution1 {
public:
    bool hasPath(vector<vector<int>>& maze, vector<int>& start, vector<int>& destination) {
        int m = maze.size(), n = maze[0].size();
        vector<vector<bool>> visited(m, vector<bool>(n, false));
        queue<pair<int, int>> q;

        q.push({start[0], start[1]});
        visited[start[0]][start[1]] = true;

        // 四个方向: 右, 左, 下, 上
        int dirs[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

        while (!q.empty()) {
            auto [x, y] = q.front();
            q.pop();

            // 检查是否到达终点
            if (x == destination[0] && y == destination[1]) {
                return true;
            }

            // 向四个方向滚动
            for (auto& d : dirs) {
                int nx = x, ny = y;

                // 模拟球滚动: 一直走直到撞墙或出界
                while (nx + d[0] >= 0 && nx + d[0] < m &&
                       ny + d[1] >= 0 && ny + d[1] < n &&
                       maze[nx + d[0]][ny + d[1]] == 0) {
                    nx += d[0];
                    ny += d[1];
                }

                // (nx, ny) 是球停下来的位置
                if (!visited[nx][ny]) {
                    visited[nx][ny] = true;
                    q.push({nx, ny});
                }
            }
        }

        return false;
    }
};

// ===================== Solution2: DFS =====================
// 递归探索所有可达的停止点
class Solution2 {
public:
    bool hasPath(vector<vector<int>>& maze, vector<int>& start, vector<int>& destination) {
        int m = maze.size(), n = maze[0].size();
        vector<vector<bool>> visited(m, vector<bool>(n, false));
        return dfs(maze, start[0], start[1], destination, visited);
    }

private:
    int dirs[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    bool dfs(vector<vector<int>>& maze, int x, int y,
             vector<int>& dest, vector<vector<bool>>& visited) {
        // 到达终点
        if (x == dest[0] && y == dest[1]) return true;
        // 已访问过此停止点
        if (visited[x][y]) return false;

        visited[x][y] = true;
        int m = maze.size(), n = maze[0].size();

        for (auto& d : dirs) {
            int nx = x, ny = y;
            // 滚到底
            while (nx + d[0] >= 0 && nx + d[0] < m &&
                   ny + d[1] >= 0 && ny + d[1] < n &&
                   maze[nx + d[0]][ny + d[1]] == 0) {
                nx += d[0];
                ny += d[1];
            }
            // 递归探索新的停止点
            if (dfs(maze, nx, ny, dest, visited)) {
                return true;
            }
        }

        return false;
    }
};

// ===================== Solution3: BFS 提前终止 =====================
// 在入队时就检查是否到达终点, 减少不必要的出队操作
class Solution3 {
public:
    bool hasPath(vector<vector<int>>& maze, vector<int>& start, vector<int>& destination) {
        int m = maze.size(), n = maze[0].size();
        vector<vector<bool>> vis(m, vector<bool>(n, false));

        queue<pair<int, int>> q;
        q.push({start[0], start[1]});
        vis[start[0]][start[1]] = true;

        vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

        while (!q.empty()) {
            auto [cx, cy] = q.front();
            q.pop();

            for (auto& [dx, dy] : directions) {
                int nx = cx, ny = cy;
                // 滚动直到碰壁
                while (nx + dx >= 0 && nx + dx < m &&
                       ny + dy >= 0 && ny + dy < n &&
                       maze[nx + dx][ny + dy] == 0) {
                    nx += dx;
                    ny += dy;
                }

                // 提前检查: 停止点就是终点
                if (nx == destination[0] && ny == destination[1]) {
                    return true;
                }

                if (!vis[nx][ny]) {
                    vis[nx][ny] = true;
                    q.push({nx, ny});
                }
            }
        }

        return false;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 优势 |
 * |------|------|------|------|
 * | Solution1 BFS | O(m*n*(m+n)) | O(m*n) | 层序遍历, 可扩展到最短路 |
 * | Solution2 DFS | O(m*n*(m+n)) | O(m*n) | 代码最简洁 |
 * | Solution3 BFS提前终止 | O(m*n*(m+n)) | O(m*n) | 提前终止, 稍快 |
 *
 * 【易错点】
 * 1. 球不是走一步停: 必须用 while 循环滚到底 (撞墙才停)
 * 2. 判断下一格而非当前格: while 条件是 maze[nx+dx][ny+dy]==0
 *    不是 maze[nx][ny]==0, 否则会停在墙上
 * 3. 只标记停止点: visited 数组标记的是球停下来的位置
 *    中间经过的格子不需要标记
 * 4. 停下来才算到达: 球可以路过 destination 但不停下, 这不算到达
 *    必须在停止点检查是否 == destination
 * 5. 起点也要标记: start 本身就是第一个停止点, 要标记 visited
 *
 * 【面试追问】
 * Q1: 为什么 visited 只标记停止点?
 *     因为搜索图的节点是停止点。中间经过的格子是边上的中间状态,
 *     球不可能在那里改变方向。标记它们会漏掉合法路径。
 * Q2: 扩展到最短路径 (LC 505 The Maze II)?
 *     边权 = 滚动的距离。需要用 Dijkstra 而非普通 BFS,
 *     因为不同方向到达同一点的距离可能不同。
 * Q3: 球如果可以在 destination 刹车停住(即使没撞墙)呢?
 *     修改滚动逻辑: while 内加一个 (nx,ny) != destination 的条件,
 *     到达 destination 就立即停下。
 */
