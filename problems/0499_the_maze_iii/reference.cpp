/*
 * 499. The Maze III
 *
 * 【题目本质】
 * 带权图最短路径 + 字典序最小路径 + 滚动物理模拟
 * 球在迷宫中滚动碰墙才停，途中经过洞会掉入，求最短距离的字典序最小指令
 *
 * 【解法总览】
 * Solution1: Dijkstra + 优先队列 — O(mn·log(mn)·k)  ⭐推荐
 * Solution2: BFS + 松弛（类Bellman-Ford）— O(mn·max(m,n)·mn)
 */

// ===================== Solution 1: Dijkstra + Priority Queue =====================
// 思路：最小堆按(距离, 路径字符串)排序，每次弹出最优状态
// 滚动模拟时检测洞口，方向按字典序 d→l→r→u
// 时间: O(mn·log(mn)·k)  空间: O(mn·k)  k=路径字符串长度

class Solution1 {
public:
    string findShortestWay(vector<vector<int>>& maze, vector<int>& ball, vector<int>& hole) {
        int m = maze.size(), n = maze[0].size();
        vector<vector<pair<int,string>>> dist(m, vector<pair<int,string>>(n, {INT_MAX, ""}));
        dist[ball[0]][ball[1]] = {0, ""};

        // {距离, 路径, 行, 列}
        priority_queue<tuple<int,string,int,int>,
                       vector<tuple<int,string,int,int>>,
                       greater<>> pq;
        pq.push({0, "", ball[0], ball[1]});

        // 方向按字典序排列: d, l, r, u
        int dr[] = {1, 0, 0, -1};
        int dc[] = {0, -1, 1, 0};
        string dirs = "dlru";

        while (!pq.empty()) {
            auto [d, path, r, c] = pq.top(); pq.pop();
            if (make_pair(d, path) > dist[r][c]) continue;

            for (int i = 0; i < 4; i++) {
                int nr = r, nc = c, steps = 0;
                while (nr + dr[i] >= 0 && nr + dr[i] < m &&
                       nc + dc[i] >= 0 && nc + dc[i] < n &&
                       maze[nr + dr[i]][nc + dc[i]] == 0) {
                    nr += dr[i];
                    nc += dc[i];
                    steps++;
                    if (nr == hole[0] && nc == hole[1]) break;
                }

                string newPath = path + dirs[i];
                int newDist = d + steps;
                if (steps > 0 && make_pair(newDist, newPath) < dist[nr][nc]) {
                    dist[nr][nc] = {newDist, newPath};
                    pq.push({newDist, newPath, nr, nc});
                }
            }
        }

        auto& [bestDist, bestPath] = dist[hole[0]][hole[1]];
        return bestDist == INT_MAX ? "impossible" : bestPath;
    }
};

// ===================== Solution 2: BFS + 松弛 =====================
// 思路：普通队列BFS，维护(最短距离, 最小路径)，发现更优解时松弛并重新入队
// 时间: O(mn·max(m,n)·mn)  空间: O(mn·k)
// 效率不如 Dijkstra，可能重复访问节点

class Solution2 {
public:
    string findShortestWay(vector<vector<int>>& maze, vector<int>& ball, vector<int>& hole) {
        int m = maze.size(), n = maze[0].size();
        vector<vector<int>> bestDist(m, vector<int>(n, INT_MAX));
        vector<vector<string>> bestPath(m, vector<string>(n, ""));
        bestDist[ball[0]][ball[1]] = 0;

        queue<pair<int,int>> q;
        q.push({ball[0], ball[1]});

        int dr[] = {1, 0, 0, -1};
        int dc[] = {0, -1, 1, 0};
        string dirs = "dlru";

        while (!q.empty()) {
            auto [r, c] = q.front(); q.pop();

            for (int i = 0; i < 4; i++) {
                int nr = r, nc = c, steps = 0;
                while (nr + dr[i] >= 0 && nr + dr[i] < m &&
                       nc + dc[i] >= 0 && nc + dc[i] < n &&
                       maze[nr + dr[i]][nc + dc[i]] == 0) {
                    nr += dr[i]; nc += dc[i]; steps++;
                    if (nr == hole[0] && nc == hole[1]) break;
                }
                int nd = bestDist[r][c] + steps;
                string np = bestPath[r][c] + dirs[i];
                if (steps > 0 && (nd < bestDist[nr][nc] ||
                    (nd == bestDist[nr][nc] && np < bestPath[nr][nc]))) {
                    bestDist[nr][nc] = nd;
                    bestPath[nr][nc] = np;
                    q.push({nr, nc});
                }
            }
        }

        return bestDist[hole[0]][hole[1]] == INT_MAX
            ? "impossible" : bestPath[hole[0]][hole[1]];
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 优劣 |
 * |------|------|------|------|
 * | Solution1 Dijkstra | O(mn·log(mn)·k) | O(mn·k) | 最优，每节点至多处理一次 |
 * | Solution2 BFS松弛 | O(mn·max(m,n)·mn) | O(mn·k) | 可能重复松弛，效率差 |
 *
 * 【易错点】
 * 1. 边权不为1 → 不能用普通BFS求最短路，需要Dijkstra
 * 2. 滚动途中要检测洞口 → while循环内需判断是否经过hole并break
 * 3. 去重需比较(距离, 路径)二元组 → 仅比较距离会丢失字典序更小的路径
 * 4. steps==0时不应处理 → 说明碰墙无法移动
 *
 * 【面试追问】
 * Q1: Maze I/II/III 三题区别？
 *     I=可达性(BFS), II=最短距离(Dijkstra), III=最短距离+字典序路径+洞口检测
 * Q2: 如果洞口可以选择不掉入（经过但不落入），如何修改？
 *     滚动时不break，而是将hole位置作为额外停靠点加入队列
 * Q3: 路径字符串存储开销大，如何优化？
 *     可以只存前驱方向(一个char)和前驱坐标，最后回溯构建路径
 */
