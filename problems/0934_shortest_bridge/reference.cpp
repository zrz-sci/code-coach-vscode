/*
 * LeetCode 934: 最短的桥 (Shortest Bridge)
 *
 * 【题目本质】
 * 矩阵中恰好有两个岛（连通分量），求将它们连通所需翻转的最少 0 的数量。
 * 等价于：求两个连通分量之间的最短距离（BFS层数）。
 *
 * 【解法总览】
 * 解法1: 暴力枚举曼哈顿距离     — O(n⁴) / O(n²) — 最直觉
 * 解法2: DFS标记岛1 + 多源BFS   — O(n²) / O(n²) — 面试首选 ⭐
 * 解法3: 纯BFS（找岛+扩展都BFS）— O(n²) / O(n²) — 避免递归栈溢出
 */

// ============================================================
// 解法1: 暴力枚举曼哈顿距离
// 时间: O(n⁴)  空间: O(n²)
//
// 【思路】
// 最朴素的想法：收集两个岛的所有格子坐标，枚举每对(岛1格子, 岛2格子)，
// 计算曼哈顿距离，取最小值。
// 在无障碍物的网格上，曼哈顿距离 = 最短路径长度。
// 翻转的0个数 = 曼哈顿距离 - 1（起终点本身是陆地不用翻转）。
//
// 瓶颈：两个岛各最多 O(n²) 个格子，双重循环 O(n⁴)。
// ============================================================
class Solution1 {
public:
    int dx[4] = {0, 0, 1, -1};
    int dy[4] = {1, -1, 0, 0};

    void dfs(vector<vector<int>>& grid, int r, int c, vector<pair<int,int>>& island) {
        int n = grid.size();
        if (r < 0 || r >= n || c < 0 || c >= n || grid[r][c] != 1) return;
        grid[r][c] = 2;
        island.push_back({r, c});
        for (int d = 0; d < 4; d++) {
            dfs(grid, r + dx[d], c + dy[d], island);
        }
    }

    int shortestBridge(vector<vector<int>>& grid) {
        int n = grid.size();
        vector<pair<int,int>> island1, island2;

        // 找到并标记第一个岛
        bool found = false;
        for (int i = 0; i < n && !found; i++) {
            for (int j = 0; j < n && !found; j++) {
                if (grid[i][j] == 1) {
                    dfs(grid, i, j, island1);
                    found = true;
                }
            }
        }

        // 收集第二个岛（剩下的1）
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 1) {
                    island2.push_back({i, j});
                }
            }
        }

        // 枚举所有对求最小曼哈顿距离
        int minDist = INT_MAX;
        for (auto& [r1, c1] : island1) {
            for (auto& [r2, c2] : island2) {
                minDist = min(minDist, abs(r1 - r2) + abs(c1 - c2));
            }
        }

        // 翻转数 = 曼哈顿距离 - 1
        return minDist - 1;
    }
};


// ============================================================
// 解法2: DFS标记岛1 + 多源BFS — 面试首选 ⭐
// 时间: O(n²)  空间: O(n²)
//
// 【思路】
// 解法1的瓶颈是枚举所有格子对。核心优化思路：
// "从一个集合出发，求到另一个集合的最短距离" → 多源BFS。
//
// Step1: 用DFS找到岛1的所有格子，标记为2，全部入队。
// Step2: 从队列中多源BFS向外扩展，每层代表翻转一个水域。
//        第一次碰到值为1的格子（岛2），当前层数就是答案。
//
// 为什么正确？BFS按层扩展，保证第一次到达的就是最短距离。
//
// 扩展过程示意（示例2: grid = [[0,1,0],[0,0,0],[0,0,1]]）:
//
//  原始:     DFS标记:    level=0出队     level=1扩展     level=2扩展
//  0 1 0     0 2 0      检查邻居        0 2 0           0 2 0
//  0 0 0     0 0 0      (0,0)(0,2)      0 2 0           2 2 2
//  0 0 1     0 0 1      (1,1)都是0      0 0 1           2 2 1 ← 碰到1!
//                       入队标记2                        return 2
//
// ============================================================
class Solution2 {
public:
    int dx[4] = {0, 0, 1, -1};
    int dy[4] = {1, -1, 0, 0};

    // DFS标记岛1并将所有格子入队（作为BFS多源起点）
    void dfs(vector<vector<int>>& grid, int r, int c, queue<pair<int,int>>& q) {
        int n = grid.size();
        if (r < 0 || r >= n || c < 0 || c >= n || grid[r][c] != 1) return;
        grid[r][c] = 2;  // 标记为"岛1"
        q.push({r, c});  // 入队作为BFS起点
        for (int d = 0; d < 4; d++) {
            dfs(grid, r + dx[d], c + dy[d], q);
        }
    }

    int shortestBridge(vector<vector<int>>& grid) {
        int n = grid.size();
        queue<pair<int,int>> q;

        // Step 1: 找到第一个1，DFS标记整个岛1
        bool found = false;
        for (int i = 0; i < n && !found; i++) {
            for (int j = 0; j < n && !found; j++) {
                if (grid[i][j] == 1) {
                    dfs(grid, i, j, q);
                    found = true;
                }
            }
        }

        // Step 2: 多源BFS，从岛1向外一层层扩展
        int level = 0;
        while (!q.empty()) {
            int size = q.size();  // 当前层的节点数
            for (int i = 0; i < size; i++) {
                auto [r, c] = q.front();
                q.pop();
                // 检查四个方向
                for (int d = 0; d < 4; d++) {
                    int nr = r + dx[d];
                    int nc = c + dy[d];
                    // 越界检查
                    if (nr < 0 || nr >= n || nc < 0 || nc >= n) continue;
                    // 已访问（岛1自身或已扩展的水域）
                    if (grid[nr][nc] == 2) continue;
                    // 碰到岛2！BFS保证这是最短距离
                    if (grid[nr][nc] == 1) return level;
                    // 水域，标记并入队，下一层继续扩展
                    grid[nr][nc] = 2;
                    q.push({nr, nc});
                }
            }
            level++;  // 完成一层扩展，相当于翻转了一层水域
        }

        return -1; // 题目保证有两个岛，不会到这里
    }
};


// ============================================================
// 解法3: 纯BFS（找岛+扩展都用BFS，避免DFS递归栈溢出）
// 时间: O(n²)  空间: O(n²)
//
// 【思路】
// 和解法2逻辑完全一致，唯一区别是Step1用BFS代替DFS来标记岛1。
// 好处：当岛非常大时（比如n=100，岛占满一半），DFS可能递归深度
// 达到5000+，有栈溢出风险。BFS用队列，没有递归深度限制。
//
// 面试追问"如何避免栈溢出"时给出此解法。
// ============================================================
class Solution3 {
public:
    int dx[4] = {0, 0, 1, -1};
    int dy[4] = {1, -1, 0, 0};

    int shortestBridge(vector<vector<int>>& grid) {
        int n = grid.size();
        queue<pair<int,int>> q; // 多源BFS的队列

        // Step 1: BFS找到并标记岛1
        bool found = false;
        for (int i = 0; i < n && !found; i++) {
            for (int j = 0; j < n && !found; j++) {
                if (grid[i][j] == 1) {
                    // 用BFS遍历整个岛1
                    queue<pair<int,int>> bfsIsland;
                    bfsIsland.push({i, j});
                    grid[i][j] = 2;
                    while (!bfsIsland.empty()) {
                        auto [r, c] = bfsIsland.front();
                        bfsIsland.pop();
                        q.push({r, c}); // 同时加入多源BFS队列
                        for (int d = 0; d < 4; d++) {
                            int nr = r + dx[d], nc = c + dy[d];
                            if (nr >= 0 && nr < n && nc >= 0 && nc < n 
                                && grid[nr][nc] == 1) {
                                grid[nr][nc] = 2;
                                bfsIsland.push({nr, nc});
                            }
                        }
                    }
                    found = true;
                }
            }
        }

        // Step 2: 多源BFS扩展（和解法2完全一样）
        int level = 0;
        while (!q.empty()) {
            int size = q.size();
            for (int i = 0; i < size; i++) {
                auto [r, c] = q.front();
                q.pop();
                for (int d = 0; d < 4; d++) {
                    int nr = r + dx[d], nc = c + dy[d];
                    if (nr < 0 || nr >= n || nc < 0 || nc >= n) continue;
                    if (grid[nr][nc] == 2) continue;
                    if (grid[nr][nc] == 1) return level;
                    grid[nr][nc] = 2;
                    q.push({nr, nc});
                }
            }
            level++;
        }

        return -1;
    }
};


// ============================================================
// 【解法对比】
//
// 解法1 (暴力): O(n⁴)，思路最直接，面试中说思路展示理解即可。
// 解法2 (DFS+BFS): O(n²)，面试首选，代码简洁，思路清晰。
//   关键转变：从"枚举所有对"到"多源BFS一层层扩展"。
// 解法3 (纯BFS): O(n²)，和解法2逻辑相同，避免递归栈溢出。
//   适用场景：面试追问"大矩阵怎么办"时提出。
//
// 【易错点】
// 1. ✗ 碰到 grid[nr][nc]==1 后还入队继续扩展
//    ✓ 应立即 return level，BFS第一次到达就是最短
//
// 2. ✗ level++ 放在 for(int i=0;i<size;i++) 的里面
//    ✓ 必须放在处理完当前层所有节点之后（for循环外面）
//
// 3. ✗ DFS 标记岛1时忘记 q.push({r, c})，导致BFS起点不完整
//    ✓ DFS 中每标记一个格子就要入队
//
// 4. ✗ 暴力解返回 minDist 而不是 minDist - 1
//    ✓ 曼哈顿距离 d 意味着中间有 d-1 个水域格子需要翻转
//
// 5. ✗ grid 参数没加引用 &，导致 DFS 修改的是副本
//    ✓ void dfs(vector<vector<int>>& grid, ...) 注意 &
//
// 【面试追问】
// Q1: 为什么多源BFS第一次碰到岛2就是最短？
//     → BFS的基本性质：按层扩展，距离单调递增，第一次到达即最短。
//
// Q2: 只把岛1的边界格子入队会更好吗？
//     → 可以减少初始队列大小，但不影响时间复杂度。
//       内部格子出队后四个邻居都是2，不会产生新入队操作。
//       面试中提到是加分项。
//
// Q3: k个岛连通所有岛的最小翻转数？
//     → 变成MST问题：对每对岛做多源BFS求距离，再用Kruskal求MST。
//
// Q4: 不允许修改原矩阵怎么办？
//     → 用额外 visited 数组标记，空间不变。
// ============================================================
