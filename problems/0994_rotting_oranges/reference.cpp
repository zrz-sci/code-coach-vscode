/*
 * LeetCode 994: 腐烂的橘子 (Rotting Oranges)
 *
 * 【题目本质】
 * 多个腐烂橘子同时向四周扩散，求所有新鲜橘子被感染的最短时间。
 * 本质是 **多源 BFS 求最远传播距离**。
 *
 * 【解法总览】
 * 解法1: 暴力模拟 — O(m·n·max(m,n)) / O(m·n) — 最直觉
 * 解法2: 多源BFS  — O(m·n) / O(m·n)          — 面试首选 ⭐
 */

// ============================================================
// 解法1: 暴力模拟 — 每分钟遍历全图，感染邻居
// 时间: O(m·n·max(m,n))  空间: O(m·n)
//
// 【思路】
// 最朴素的想法：模拟每一分钟的扩散过程。
// 每一轮扫描整个网格，找到所有腐烂橘子，收集它们四周的新鲜邻居。
// 一轮结束后统一标记为腐烂。重复直到没有新感染。
//
// 瓶颈：每轮都扫描整个网格，但大部分格子不是"刚腐烂的"，
// 做了大量无用功。这就是解法2用BFS优化的动机。
//
// 示例1 扩散过程:
//   t=0:  2 1 1    t=1:  2 2 1    t=2:  2 2 2    t=3:  2 2 2    t=4:  2 2 2
//         1 1 0          2 1 0          2 2 0          2 2 0          2 2 0
//         0 1 1          0 1 1          0 1 1          0 2 1          0 2 2
// ============================================================
class Solution1 {
public:
    int orangesRotting(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        int minutes = 0;
        int dx[] = {0, 0, 1, -1};
        int dy[] = {1, -1, 0, 0};

        while (true) {
            // 收集这一轮新感染的位置
            // 为什么不能边遍历边修改？因为同一分钟新腐烂的不应该立刻感染更远的邻居
            vector<pair<int,int>> newRotten;

            for (int i = 0; i < m; i++) {
                for (int j = 0; j < n; j++) {
                    if (grid[i][j] == 2) {
                        for (int d = 0; d < 4; d++) {
                            int ni = i + dx[d], nj = j + dy[d];
                            if (ni >= 0 && ni < m && nj >= 0 && nj < n 
                                && grid[ni][nj] == 1) {
                                newRotten.push_back({ni, nj});
                            }
                        }
                    }
                }
            }

            if (newRotten.empty()) break;  // 没有新感染，停止

            for (auto& [r, c] : newRotten) {
                grid[r][c] = 2;  // 统一标记为腐烂
            }
            minutes++;
        }

        // 检查是否还有新鲜橘子
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                if (grid[i][j] == 1) return -1;

        return minutes;
    }
};

// ============================================================
// 解法2: 多源 BFS — 所有腐烂橘子同时入队，逐层扩展 ⭐ 面试首选
// 时间: O(m·n)  空间: O(m·n)
//
// 【思路】
// 解法1的瓶颈：每轮盲目扫全图找腐烂橘子。
// 优化：用队列维护"待扩散前线"——只关注刚腐烂的橘子。
//
// 多源 BFS 的核心洞察：
// 所有初始腐烂橘子是"第0层"，它们同时扩散。
// 效果等价于加一个虚拟超级源点连接所有初始腐烂橘子。
//
//   BFS 逐层扩展示意 (示例1):
//   
//   初始队列: [(0,0)]  freshCount=6
//   
//   第1层: (0,0)→感染(0,1),(1,0)
//     队列: [(0,1),(1,0)]  freshCount=4  minutes=1
//   
//   第2层: (0,1)→(0,2), (1,0)→(1,1)
//     队列: [(0,2),(1,1)]  freshCount=2  minutes=2
//   
//   第3层: (0,2)→无新邻居, (1,1)→(2,1)
//     队列: [(2,1)]  freshCount=1  minutes=3
//   
//   第4层: (2,1)→(2,2)
//     队列: [(2,2)]  freshCount=0  minutes=4
//   
//   第5层: (2,2)→无新邻居, 队列空
//     minutes=5, 但最后一层没有实际感染 → 返回 minutes-1=4
//
// 为什么用 grid 本身做 visited？
// grid[i][j] = 2 意味着已腐烂/已访问，不会重复入队。
// 这样不需要额外的 visited 数组。
// ============================================================
class Solution2 {
public:
    int orangesRotting(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        queue<pair<int,int>> q;
        int freshCount = 0;

        // 第一步：预扫描——收集所有初始腐烂橘子（多源BFS的起点）
        //         同时统计新鲜橘子总数（用于最后判断是否全部腐烂）
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 2) {
                    q.push({i, j});
                } else if (grid[i][j] == 1) {
                    freshCount++;
                }
            }
        }

        // 特判：如果一开始就没有新鲜橘子，0分钟完成
        // 为什么需要这个？避免BFS空转后返回错误值
        if (freshCount == 0) return 0;

        int minutes = 0;
        int dx[] = {0, 0, 1, -1};
        int dy[] = {1, -1, 0, 0};

        // 第二步：BFS 逐层扩展
        while (!q.empty()) {
            int size = q.size();  // 当前层节点数 = 这一分钟同时扩散的橘子数

            for (int i = 0; i < size; i++) {
                auto [r, c] = q.front();
                q.pop();

                // 向四个方向扩散
                for (int d = 0; d < 4; d++) {
                    int nr = r + dx[d], nc = c + dy[d];

                    // 只感染：在网格内 且 是新鲜橘子
                    if (nr >= 0 && nr < m && nc >= 0 && nc < n 
                        && grid[nr][nc] == 1) {
                        grid[nr][nc] = 2;   // 标记腐烂（= visited）
                        freshCount--;       // 新鲜橘子少一个
                        q.push({nr, nc});   // 入队，下一层处理
                    }
                }
            }

            minutes++;  // 处理完一整层，时间+1
        }

        // 第三步：返回结果
        // minutes-1：因为最后一层节点出队时没有新感染，但 minutes 仍然 +1 了
        // freshCount != 0：存在不可达的新鲜橘子
        return freshCount == 0 ? minutes - 1 : -1;
    }
};

// ============================================================
// 解法2 变体: 避免 minutes-1 的写法
// 
// 【思路】
// 只有当本轮确实感染了新橘子时才 minutes++，
// 这样不需要最后减 1，逻辑更清晰。
// ============================================================
class Solution2b {
public:
    int orangesRotting(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        queue<pair<int,int>> q;
        int freshCount = 0;

        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 2) q.push({i, j});
                else if (grid[i][j] == 1) freshCount++;
            }

        if (freshCount == 0) return 0;

        int minutes = 0;
        int dx[] = {0, 0, 1, -1};
        int dy[] = {1, -1, 0, 0};

        while (!q.empty()) {
            int size = q.size();
            bool infected = false;  // 本轮是否有新感染

            for (int i = 0; i < size; i++) {
                auto [r, c] = q.front();
                q.pop();

                for (int d = 0; d < 4; d++) {
                    int nr = r + dx[d], nc = c + dy[d];
                    if (nr >= 0 && nr < m && nc >= 0 && nc < n 
                        && grid[nr][nc] == 1) {
                        grid[nr][nc] = 2;
                        freshCount--;
                        q.push({nr, nc});
                        infected = true;  // 有新感染
                    }
                }
            }

            if (infected) minutes++;  // 只有确实感染了才算一分钟
        }

        return freshCount == 0 ? minutes : -1;
    }
};

// ============================================================
// 【解法对比】
// 
// 解法1 (暴力模拟):
//   - 每轮扫全图 → O(m·n) 每轮，最坏 O(m·n) 轮 → O((m·n)²)
//   - 好处：思路直观，不需要额外数据结构
//   - 坏处：大量重复扫描无用格子
//
// 解法2 (多源BFS):
//   - 每个格子最多入队出队一次 → 总共 O(m·n)
//   - 好处：最优时间，标准 BFS 模板
//   - 坏处：需要 O(m·n) 队列空间
//
// 面试中直接写解法2，暴力解口头说一下即可。
//
// 【易错点】
//
// 1. ✗ 忘记 minutes-1（Solution2）或不用 infected 标志（Solution2b）:
//      BFS 最后一轮处理完后队列为空，但 minutes 已经多加了 1。
//    ✓ 要么返回 minutes-1，要么只在有新感染时才 minutes++。
//
// 2. ✗ 暴力解中边遍历边修改 grid:
//      同一轮循环里，刚被标记为 2 的橘子立刻被当成"已腐烂"去感染邻居，
//      导致一分钟内传播多步，结果偏小。
//    ✓ 先收集所有新感染位置，循环结束后统一修改。BFS 天然避免此问题。
//
// 3. ✗ 没有特判 freshCount==0 直接返回 0:
//      如果没有新鲜橘子，BFS 仍会运行（处理腐烂橘子的空扩展），
//      可能返回 -1 而不是 0。
//    ✓ 在 BFS 前加 if (freshCount == 0) return 0;
//
// 4. ✗ BFS 结束后忘记检查 freshCount:
//      有些新鲜橘子被空格隔开，永远无法被感染。
//    ✓ 必须 return freshCount == 0 ? minutes : -1;
//
// 5. ✗ 方向数组只写了 2 个方向:
//      题目说"4个方向相邻"，必须上下左右都要。
//    ✓ dx[]={0,0,1,-1}, dy[]={1,-1,0,0} 四个方向。
//
// 【面试追问】
//
// Q1 (基础理解): 为什么用 BFS 而不是 DFS？
//   → BFS 按层扩展，每层=1分钟，天然求最短传播时间。
//     DFS 深入一条路径，无法高效模拟"同时扩散"。
//
// Q2 (优化): 如果不想修改原始 grid 怎么办？
//   → 额外用 visited[m][n] 数组，或用 set 记录已腐烂坐标。
//     空间多 O(m·n)，但不破坏输入数据。
//
// Q3 (变体): 如果不同腐烂橘子传播速度不同怎么办？
//   → 变成带权图的最短路径问题，用 Dijkstra（优先队列 BFS）。
//     普通 BFS 只适用于所有边权相同（每步1分钟）的情况。
//
// Q4 (变体): 如果可以花费一次代价让任意一个新鲜橘子立刻腐烂，
//     求最少分钟数？
//   → 需要枚举"额外放一个腐烂源"的位置，对每种情况跑 BFS。
//     优化可以预计算每个新鲜橘子到最近腐烂橘子的距离。
// ============================================================
