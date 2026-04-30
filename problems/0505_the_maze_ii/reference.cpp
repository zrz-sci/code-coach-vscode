/*
 * 【题目本质】
 * 带权最短路径：球滚到撞墙才停（边权=滚动格数），求 start→destination 最短距离。
 * 不等权图 → Dijkstra，不能用普通 BFS。
 *
 * 【解法总览】
 * 解法1: Dijkstra + 优先队列  O(mn log(mn)) / O(mn)  ⭐推荐
 * 解法2: BFS + 距离松弛       O(mn * max(m,n)) / O(mn)
 */

// ===================== 解法1: Dijkstra + 优先队列 =====================
// 思路：最小堆按距离排序，每次扩展距离最小的停止位置
class Solution1 {
public:
    int shortestDistance(vector<vector<int>>& maze, vector<int>& start, vector<int>& destination) {
        int m = maze.size(), n = maze[0].size();
        vector<vector<int>> dist(m, vector<int>(n, INT_MAX));
        dist[start[0]][start[1]] = 0;

        // (距离, 行, 列)
        priority_queue<tuple<int,int,int>, vector<tuple<int,int,int>>, greater<>> pq;
        pq.push({0, start[0], start[1]});

        int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};

        while (!pq.empty()) {
            auto [d, r, c] = pq.top(); pq.pop();

            if (d > dist[r][c]) continue; // 过时条目
            if (r == destination[0] && c == destination[1]) return d;

            for (auto& dir : dirs) {
                int nr = r, nc = c, steps = 0;
                // 滚到撞墙
                while (nr + dir[0] >= 0 && nr + dir[0] < m &&
                       nc + dir[1] >= 0 && nc + dir[1] < n &&
                       maze[nr + dir[0]][nc + dir[1]] == 0) {
                    nr += dir[0];
                    nc += dir[1];
                    steps++;
                }
                // 松弛
                if (dist[r][c] + steps < dist[nr][nc]) {
                    dist[nr][nc] = dist[r][c] + steps;
                    pq.push({dist[nr][nc], nr, nc});
                }
            }
        }

        return -1;
    }
};

// ===================== 解法2: BFS + 距离松弛 =====================
// 思路：普通队列 BFS，发现更短路径时重新入队
// 正确但比 Dijkstra 慢（同一节点可能多次入队）
class Solution2 {
public:
    int shortestDistance(vector<vector<int>>& maze, vector<int>& start, vector<int>& destination) {
        int m = maze.size(), n = maze[0].size();
        vector<vector<int>> dist(m, vector<int>(n, INT_MAX));
        dist[start[0]][start[1]] = 0;

        queue<pair<int,int>> q;
        q.push({start[0], start[1]});

        int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};

        while (!q.empty()) {
            auto [r, c] = q.front(); q.pop();

            for (auto& dir : dirs) {
                int nr = r, nc = c, steps = 0;
                while (nr + dir[0] >= 0 && nr + dir[0] < m &&
                       nc + dir[1] >= 0 && nc + dir[1] < n &&
                       maze[nr + dir[0]][nc + dir[1]] == 0) {
                    nr += dir[0];
                    nc += dir[1];
                    steps++;
                }
                if (dist[r][c] + steps < dist[nr][nc]) {
                    dist[nr][nc] = dist[r][c] + steps;
                    q.push({nr, nc});
                }
            }
        }

        int ans = dist[destination[0]][destination[1]];
        return ans == INT_MAX ? -1 : ans;
    }
};

/*
 * 【解法对比】
 * 解法1（Dijkstra）：每个节点最多处理一次（出堆即最优），时间更优
 * 解法2（BFS松弛）：同一节点可能多次入队和处理，时间较差但代码更简单
 * 面试首选解法1，它体现了对加权图最短路的正确理解
 *
 * 【易错点】
 * 1. 不能用 visited 数组 — 同一位置可能被更短路径再次到达
 * 2. 球必须在 destination 碰壁停下，经过不算到达
 * 3. 每次移动是"滚到撞墙"，不是移动一格
 * 4. Dijkstra 中 d > dist[r][c] 时要 continue（过时条目）
 *
 * 【面试追问】
 * Q1: 和 490 The Maze 的区别 → 490 只要可达性(BFS/DFS)，505 要最短距离(Dijkstra)
 * Q2: 为何不能普通 BFS → 边权不等，先出队的不一定距离最短
 * Q3: 球可任意停下 → 退化为普通 BFS（等权图），每步一格
 */
