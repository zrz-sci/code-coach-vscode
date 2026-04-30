/*
 * LeetCode 286: 墙与门 (Walls and Gates)
 *
 * 【题目本质】
 * 多源最短路径：从所有门（值为0）同时出发 BFS，
 * 给每个空房间（INF）填上到最近门的距离。
 *
 * 【解法总览】
 * 解法1: 暴力 BFS（从每个空房间出发）— O(m²n²) / O(mn) — 最直觉
 * 解法2: 多源 BFS（从所有门同时出发）— O(mn) / O(mn) — 最优 ⭐
 * 解法3: DFS（从每个门出发，剪枝）   — O(mn × 门数) / O(mn) — 备选
 */

// ============================================================
// 解法1: 暴力 BFS — 对每个空房间，独立做一次 BFS 找最近的门
// 时间: O(m²n²)  空间: O(mn)
//
// 【思路】
// 最直觉的想法：遍历网格，对每个值为 INF 的空房间，启动一次 BFS
// 搜索最近的门（值为 0），BFS 天然保证第一次找到的门就是最近的。
// 
// 瓶颈：每个空房间都要遍历整个网格，空房间数量最多 O(mn)，
// 所以总时间 O(mn × mn) = O(m²n²)。大量重复搜索。
// ============================================================
class Solution1 {
public:
    void wallsAndGates(vector<vector<int>>& rooms) {
        int m = rooms.size(), n = rooms[0].size();
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (rooms[i][j] == INT_MAX) {
                    rooms[i][j] = bfs(rooms, i, j, m, n);
                }
            }
        }
    }
    
private:
    int bfs(vector<vector<int>>& rooms, int si, int sj, int m, int n) {
        int dx[] = {0, 0, 1, -1};
        int dy[] = {1, -1, 0, 0};
        vector<vector<bool>> visited(m, vector<bool>(n, false));
        queue<pair<int,int>> q;
        q.push({si, sj});
        visited[si][sj] = true;
        int dist = 0;
        
        while (!q.empty()) {
            int sz = q.size();
            for (int k = 0; k < sz; k++) {
                auto [x, y] = q.front(); q.pop();
                // 找到门，BFS保证这是最近的
                if (rooms[x][y] == 0) return dist;
                for (int d = 0; d < 4; d++) {
                    int nx = x + dx[d], ny = y + dy[d];
                    if (nx >= 0 && nx < m && ny >= 0 && ny < n
                        && !visited[nx][ny] && rooms[nx][ny] != -1) {
                        visited[nx][ny] = true;
                        q.push({nx, ny});
                    }
                }
            }
            dist++;
        }
        return INT_MAX; // 无法到达任何门
    }
};

// ============================================================
// 解法2: 多源 BFS — 从所有门同时出发，层层向外扩展 ⭐
// 时间: O(mn)  空间: O(mn)
//
// 【思路】
// 解法1的瓶颈：每个空房间独立搜索，大量重复遍历。
// 
// 关键观察：反转搜索方向！不是"从房间找门"，而是"从门找房间"。
// 更进一步，所有门同时作为BFS起点，一次BFS搞定全部。
//
// 为什么正确：BFS按层扩展，第d层的格子到最近门的距离恰好是d。
// 每个格子第一次被访问时就是被最近的门到达的，因为更近的门
// 在更早的层就已经把它"抢占"了。
//
// 不需要 visited：空房间初始值 INF，更新后不再是 INF，
// 所以 rooms[nx][ny] == INT_MAX 就是"未访问"的判断。
//
// 多源 BFS 扩展过程 (示例1):
//
//  原始网格:                 所有门入队 (距离0):
//  INF  -1   0  INF         INF  -1  [0] INF
//  INF INF  INF  -1         INF INF  INF  -1
//  INF  -1  INF  -1         INF  -1  INF  -1
//   0   -1  INF INF         [0]  -1  INF INF
//
//  第1层 (距离=1):           第2层 (距离=2):
//  INF  -1   0   1           INF  -1   0   1
//  INF INF   1   -1           2    2   1   -1
//   1   -1  INF  -1           1   -1   2   -1
//   0   -1  INF INF           0   -1  INF INF
//
//  第3层 (距离=3):           第4层 (距离=4):
//   3   -1   0   1            3   -1   0   1
//   2    2   1   -1            2    2   1   -1
//   1   -1   2   -1            1   -1   2   -1
//   0   -1   3  INF            0   -1   3   4
// ============================================================
class Solution2 {
public:
    void wallsAndGates(vector<vector<int>>& rooms) {
        int m = rooms.size(), n = rooms[0].size();
        queue<pair<int,int>> q;
        
        // 把所有门(值为0)加入初始队列 — 这是"多源"的关键
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (rooms[i][j] == 0) {
                    q.push({i, j});
                }
            }
        }
        
        int dx[] = {0, 0, 1, -1};
        int dy[] = {1, -1, 0, 0};
        
        // BFS: 从所有门同时向外扩展
        while (!q.empty()) {
            auto [x, y] = q.front(); q.pop();
            for (int d = 0; d < 4; d++) {
                int nx = x + dx[d], ny = y + dy[d];
                // 只处理值为 INF 的格子：
                // 1. 墙(-1)自动跳过
                // 2. 门(0)自动跳过
                // 3. 已更新的格子(< INF)自动跳过 — 天然的visited机制
                if (nx >= 0 && nx < m && ny >= 0 && ny < n
                    && rooms[nx][ny] == INT_MAX) {
                    // 距离 = 当前格子的距离 + 1
                    rooms[nx][ny] = rooms[x][y] + 1;
                    q.push({nx, ny});
                }
            }
        }
        // 无法到达任何门的空房间保持 INF 不变 — 自动满足题目要求
    }
};

// ============================================================
// 解法3: DFS — 从每个门出发，DFS更新距离，用剪枝避免重复
// 时间: O(mn × 门数) 最坏  空间: O(mn) 递归栈
//
// 【思路】
// 从每个门出发做DFS，携带当前距离。
// 如果到达一个格子时，当前距离 > 该格子已记录的值，说明
// 已有更短的路径到达过这里，没必要继续 → 剪枝。
//
// 这个解法可能对同一个格子从不同门出发更新多次，
// 最坏情况（如门很多且分布密集）不如多源BFS稳定。
// ============================================================
class Solution3 {
public:
    void wallsAndGates(vector<vector<int>>& rooms) {
        int m = rooms.size(), n = rooms[0].size();
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (rooms[i][j] == 0) {
                    dfs(rooms, i, j, 0, m, n);
                }
            }
        }
    }

private:
    void dfs(vector<vector<int>>& rooms, int x, int y, int dist, int m, int n) {
        // 越界
        if (x < 0 || x >= m || y < 0 || y >= n) return;
        // 遇墙
        if (rooms[x][y] == -1) return;
        // 剪枝：当前路径不比已有的更短，没必要继续
        // 注意是 >，不是 >=，因为起点 dist=0 == rooms[x][y]=0 要允许进入
        if (dist > rooms[x][y]) return;
        
        rooms[x][y] = dist;
        
        int dx[] = {0, 0, 1, -1};
        int dy[] = {1, -1, 0, 0};
        for (int d = 0; d < 4; d++) {
            dfs(rooms, x + dx[d], y + dy[d], dist + 1, m, n);
        }
    }
};

// ============================================================
// 【解法对比】
//
// 解法1 (暴力BFS):
//   - 从空房间出发找门，每个房间独立BFS
//   - 大量重复搜索，O(m²n²)
//   - 理解题意用，面试中提一句即可
//
// 解法2 (多源BFS) ⭐:
//   - 反转方向，从所有门同时出发
//   - 每个格子恰好入队一次，O(mn)
//   - 面试必须写出的解法
//   - 代码最简洁，不需要额外visited
//
// 解法3 (DFS):
//   - 从门出发DFS，利用剪枝减少搜索
//   - 最坏情况同一格子被多个门更新
//   - 代码简洁但效率不如多源BFS稳定
//
// 【易错点】
// 1. ✗ 判断邻居可达用 rooms[nx][ny] != -1
//    ✓ 应该用 rooms[nx][ny] == INT_MAX
//    原因: != -1 会让已更新的格子再次入队，导致重复处理甚至错误
//
// 2. ✗ DFS剪枝条件写成 dist >= rooms[x][y]
//    ✓ 应该是 dist > rooms[x][y]
//    原因: 门的初始调用 dist=0, rooms=0，相等时必须允许进入
//
// 3. ✗ 先访问 rooms[nx][ny] 再检查边界
//    ✓ 边界检查必须在数组访问之前 (短路求值)
//    原因: 否则越界访问导致 undefined behavior
//
// 4. ✗ 多源BFS中用层循环 for(int k=0;k<sz;k++) 并维护层号
//    ✓ 不需要！距离直接由 rooms[x][y]+1 计算
//    原因: 距离信息已编码在格子值中，不需要全局层号
//
// 【面试追问】
// Q1: 为什么不从空房间出发找门？瓶颈在哪？
//     → 每个空房间独立BFS，K个空房间就要BFS K次，
//       总时间O(K·mn)，K最大接近mn，所以O(m²n²)。
//
// Q2: 多源BFS为什么每个格子只入队一次就能保证最优？
//     → BFS按层扩展，第d层格子离某个源(门)距离恰好d。
//       多个门同时扩展，先到达的一定是最近的门。
//       格子更新后不再是INF，不会被重复入队。
//
// Q3: 如果要输出每个空房间到最近门的具体路径，怎么改？
//     → 维护 parent[x][y] 记录每个格子被哪个邻居更新，
//       查询时从目标沿parent回溯到门。
//
// Q4: 如果门有不同的权重（通过某些门代价更高），怎么办？
//     → 不再是普通BFS，需要用 Dijkstra（优先队列BFS），
//       初始时按权重将门加入优先队列。
// ============================================================
