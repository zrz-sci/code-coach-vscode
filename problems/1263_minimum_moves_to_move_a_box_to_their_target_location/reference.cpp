/*
 * LeetCode 1263: 推箱子 (Minimum Moves to Move a Box to Their Target Location)
 *
 * 【题目】m x n 网格, S=玩家, B=箱子, T=目标, #=墙, .=地板
 *        求将箱子推到目标的最少推动次数。玩家只能站在箱子旁边沿箱子方向推。
 *
 * 【解法1】BFS + 内层BFS — O(m^2*n^2) / O(m*n) ⭐ 首选
 *        外层 BFS 搜索最少推动路径
 *        内层 BFS 判断玩家能否到达推动需求位置
 *
 * 【解法2】0-1 BFS (Deque BFS) — O(m^2*n^2) / O(m^2*n^2)
 *        统一状态 (箱子位置, 玩家位置)
 *        玩家移动 cost=0 加 deque 前端, 推箱子 cost=1 加后端
 */

// ============================================================
// 解法1: BFS + 内层 BFS ⭐ 面试首选
// ============================================================

/*
 * 推箱子状态转移详解:
 *
 * 当前状态: 箱子(br,bc), 玩家(pr,pc)
 * 推动方向 d = (dr,dc), 4个方向:
 *   dirs = {(0,1)右, (0,-1)左, (1,0)下, (-1,0)上}
 *
 * 推向方向 d:
 *   1. 玩家需求位 = (br-dr, bc-dc)    [箱子的反方向]
 *   2. canReach(pr,pc → needR,needC, 绕过箱子)
 *   3. 新箱子位 = (br+dr, bc+dc)      [箱子的正方向]
 *   4. 新玩家位 = (br, bc)             [箱子的旧位置]
 *   5. pushes + 1
 *
 * 示例 (Example 1):
 *   # # # # # #
 *   # T # # # #       T=(1,1), B=(2,3), S=(4,4)
 *   # . . B . #
 *   # . # # . #
 *   # . . . S #
 *   # # # # # #
 *
 * Push 1: 向左推(0,-1)
 *   玩家需在(2,4), canReach(4,4→2,4, 绕过(2,3))? YES
 *   → 箱子(2,3)→(2,2), 玩家→(2,3)
 *
 * Push 2: 向左推(0,-1)
 *   玩家在(2,3), 需在(2,3)? 已在! YES
 *   → 箱子(2,2)→(2,1), 玩家→(2,2)
 *
 * Push 3: 向上推(-1,0)
 *   玩家在(2,2), 需在(3,1), canReach?
 *   路线: (2,2)→(3,2)→(4,2)→(4,1)→(3,1) YES
 *   → 箱子(2,1)→(1,1)=Target!
 *   答案: 3
 */

class Solution {
    int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};

    // 内层 BFS: 玩家能否从 (sr,sc) 走到 (tr,tc)?
    // 箱子位置 (boxR,boxC) 视为不可通行
    bool canReach(vector<vector<char>>& grid,
                  int sr, int sc, int tr, int tc,
                  int boxR, int boxC) {
        if (sr == tr && sc == tc) return true;
        int m = grid.size(), n = grid[0].size();
        // 目标位置合法性检查
        if (tr < 0 || tr >= m || tc < 0 || tc >= n) return false;
        if (grid[tr][tc] == '#') return false;

        vector<vector<bool>> vis(m, vector<bool>(n, false));
        queue<pair<int,int>> q;
        q.push({sr, sc});
        vis[sr][sc] = true;
        vis[boxR][boxC] = true; // 关键: 箱子是障碍!

        while (!q.empty()) {
            auto [r, c] = q.front(); q.pop();
            for (auto& d : dirs) {
                int nr = r + d[0], nc = c + d[1];
                if (nr >= 0 && nr < m && nc >= 0 && nc < n
                    && !vis[nr][nc] && grid[nr][nc] != '#') {
                    if (nr == tr && nc == tc) return true;
                    vis[nr][nc] = true;
                    q.push({nr, nc});
                }
            }
        }
        return false;
    }

public:
    int minPushBox(vector<vector<char>>& grid) {
        int m = grid.size(), n = grid[0].size();
        int boxR = -1, boxC = -1;
        int playerR = -1, playerC = -1;
        int targetR = -1, targetC = -1;

        // 定位 B, S, T
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 'B') { boxR = i; boxC = j; }
                if (grid[i][j] == 'S') { playerR = i; playerC = j; }
                if (grid[i][j] == 'T') { targetR = i; targetC = j; }
            }
        }

        // visited[br][bc][dir]: 是否已从方向dir推箱子到(br,bc)
        // dir: 0=右, 1=左, 2=下, 3=上 (与dirs数组对应)
        // 状态空间: m * n * 4 ≤ 20*20*4 = 1600
        vector<vector<vector<bool>>> visited(
            m, vector<vector<bool>>(n, vector<bool>(4, false)));

        struct State {
            int br, bc, pr, pc, pushes;
        };
        queue<State> q;
        q.push({boxR, boxC, playerR, playerC, 0});

        while (!q.empty()) {
            auto [br, bc, pr, pc, pushes] = q.front();
            q.pop();

            // 箱子已到达目标
            if (br == targetR && bc == targetC) return pushes;

            // 尝试 4 个推动方向
            for (int d = 0; d < 4; d++) {
                // 玩家需要站的位置 = 箱子的反方向
                int needR = br - dirs[d][0];
                int needC = bc - dirs[d][1];
                // 推后箱子的新位置 = 箱子的正方向
                int newBR = br + dirs[d][0];
                int newBC = bc + dirs[d][1];

                // 新箱子位置合法性
                if (newBR < 0 || newBR >= m || newBC < 0 || newBC >= n) continue;
                if (grid[newBR][newBC] == '#') continue;
                // 去重: 同一方向同一箱子位置不重复处理
                if (visited[newBR][newBC][d]) continue;

                // 玩家能否走到需求位置? (绕过箱子)
                if (canReach(grid, pr, pc, needR, needC, br, bc)) {
                    visited[newBR][newBC][d] = true;
                    // 推动: 箱子到新位置, 玩家到箱子旧位置
                    q.push({newBR, newBC, br, bc, pushes + 1});
                }
            }
        }

        return -1; // 无法到达
    }
};


// ============================================================
// 解法2: 0-1 BFS (Deque BFS)
// ============================================================

/*
 * 0-1 BFS 核心思想:
 *
 * 统一视角: 状态 = (boxR, boxC, playerR, playerC)
 *
 * 边的类型:
 *   玩家普通移动 (不碰箱子): cost = 0 → deque.push_front
 *   玩家推箱子:              cost = 1 → deque.push_back
 *
 * 为什么用 deque 而不是优先队列?
 *   只有 0 和 1 两种权重, deque 天然按 cost 非递减排序
 *   push_front 放 cost=0, push_back 放 cost=1
 *   取 front 始终是当前最小 cost → 等效于 Dijkstra, 但 O(V+E)
 *
 * 巧妙之处: 玩家走到箱子位置 = 推箱子
 *   → 无需单独判断"推"的条件, 在移动逻辑中自然涵盖
 *
 * 状态空间: O(m^2 * n^2) ≤ 20^4 = 160000
 * 比解法1的 O(m*n*4) = 1600 大, 但对于 m,n≤20 仍可接受
 */

class Solution2 {
public:
    int minPushBox(vector<vector<char>>& grid) {
        int m = grid.size(), n = grid[0].size();
        int bR = -1, bC = -1, pR = -1, pC = -1;
        int tR = -1, tC = -1;

        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 'B') { bR = i; bC = j; }
                if (grid[i][j] == 'S') { pR = i; pC = j; }
                if (grid[i][j] == 'T') { tR = i; tC = j; }
            }

        int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};

        // 状态编码为唯一整数
        // encode(br,bc,pr,pc) = ((br*n+bc)*m+pr)*n+pc
        auto encode = [&](int br, int bc, int pr, int pc) -> int {
            return ((br * n + bc) * m + pr) * n + pc;
        };

        // dist[state] = 最少推动次数
        unordered_map<int, int> dist;
        deque<tuple<int,int,int,int>> dq;

        int startState = encode(bR, bC, pR, pC);
        dist[startState] = 0;
        dq.push_back({bR, bC, pR, pC});

        while (!dq.empty()) {
            auto [br, bc, pr, pc] = dq.front();
            dq.pop_front();

            int curState = encode(br, bc, pr, pc);
            int curDist = dist[curState];

            // 剪枝: 如果当前距离已过时, 跳过
            // (可能已有更短路径先到达了此状态)
            // 注: 0-1 BFS 中同一状态可能入队多次

            // 箱子到达目标
            if (br == tR && bc == tC) return curDist;

            // 尝试玩家的4个移动方向
            for (auto& d : dirs) {
                int npr = pr + d[0], npc = pc + d[1];
                if (npr < 0 || npr >= m || npc < 0 || npc >= n) continue;
                if (grid[npr][npc] == '#') continue;

                if (npr == br && npc == bc) {
                    // 玩家走到箱子位置 → 推箱子 (cost = 1)
                    int nbr = br + d[0], nbc = bc + d[1];
                    if (nbr < 0 || nbr >= m || nbc < 0 || nbc >= n) continue;
                    if (grid[nbr][nbc] == '#') continue;

                    int newState = encode(nbr, nbc, br, bc);
                    if (dist.find(newState) == dist.end() ||
                        curDist + 1 < dist[newState]) {
                        dist[newState] = curDist + 1;
                        dq.push_back({nbr, nbc, br, bc}); // cost 1 → 后端
                    }
                } else {
                    // 普通移动 (cost = 0)
                    int newState = encode(br, bc, npr, npc);
                    if (dist.find(newState) == dist.end() ||
                        curDist < dist[newState]) {
                        dist[newState] = curDist;
                        dq.push_front({br, bc, npr, npc}); // cost 0 → 前端
                    }
                }
            }
        }

        return -1;
    }
};


// ============================================================
// 复杂度分析
// ============================================================
//
// 解法1 (BFS + 内层 BFS):
//   外层状态数: O(m * n * 4) = 1600 (m=n=20)
//   每个状态的内层 BFS: O(m * n) = 400
//   总时间: O(m^2 * n^2) ≈ 640,000
//   空间: O(m * n * 4) = 1600 (visited) + O(m * n) (内层 BFS)
//
// 解法2 (0-1 BFS):
//   状态数: O(m^2 * n^2) = 160,000
//   每个状态处理: O(4) 方向
//   总时间: O(m^2 * n^2) ≈ 640,000
//   空间: O(m^2 * n^2) = 160,000 (dist map)
//
// 两种解法时间量级相同, 但:
//   解法1 空间更优 (1600 vs 160000)
//   解法2 代码更统一 (无嵌套 BFS)

// ============================================================
// 【解法对比】
//
// | 维度         | 解法1 BFS+内层BFS ⭐    | 解法2 0-1 BFS (Deque)    |
// |-------------|----------------------|------------------------|
// | 时间         | O(m²n²)              | O(m²n²)                |
// | 空间         | O(mn) ← 远优          | O(m²n²) ← 160K states |
// | 状态定义     | (箱子位置, 推动方向)    | (箱子位置, 玩家位置)     |
// | 代码难度     | 中等 (嵌套BFS)         | 较简 (统一框架)          |
// | 面试推荐     | ⭐ 首选               | 追问备选                |
//
// 面试选择:
//   首选解法1 — 状态空间小, 空间效率好, 逻辑清晰
//   0-1 BFS 适合追问 "能否统一建模" 时展示
//
// ============================================================
// 【易错点】
//
// 1. 玩家需求位计算反了:
//    ✗ needR = br + dr  → 这是箱子移动方向
//    ✓ needR = br - dr  → 玩家站在箱子的反方向
//
// 2. 内层BFS忘记把箱子设为障碍:
//    ✗ 只检查 grid[nr][nc] != '#'
//    ✓ 还要 vis[boxR][boxC] = true，否则玩家会"穿过"箱子
//
// 3. visited 状态不足:
//    ✗ visited[br][bc] → 同一箱子位置可从不同方向推到
//    ✓ visited[br][bc][dir] → 需要记录推动方向
//
// 4. 推后玩家位置搞错:
//    ✗ 玩家还在 needR,needC
//    ✓ 推后玩家站在箱子的旧位置 (br, bc)
//
// 5. 0-1 BFS 中 deque 方向用反:
//    ✗ cost=0 push_back, cost=1 push_front → 非递减性被破坏
//    ✓ cost=0 push_front, cost=1 push_back
//
// ============================================================
// 【面试追问】
//
// Q1: 为什么外层BFS就能保证最少推动次数?
//     BFS 按层展开, 每层 pushes+1, 第一次到达目标即最短。
//     内层 BFS 只判可达性, 不改变推动距离的度量。
//
// Q2: visited 为什么需要方向维度?
//     同一箱子位置可以从不同方向被推到, 对应的玩家位置不同。
//     例如从左推到 (2,3) vs 从上推到 (2,3), 后续可达状态不同。
//     去掉方向维度会遗漏合法路径。
//
// Q3: 0-1 BFS 为什么能替代嵌套BFS?
//     将"玩家移动"和"推箱子"统一为图的边, 分别赋权 0 和 1。
//     deque 天然维护 cost 非递减序, 等效 Dijkstra 但复杂度 O(V+E)。
//     代价是状态空间从 O(mn·4) 膨胀到 O(m²n²)。
//
// ============================================================
// 【相关题型】
//
// - 542. 01 Matrix: 多源 BFS
// - 1162. As Far from Land as Possible: 多源 BFS
// - 1091. Shortest Path in Binary Matrix: 标准 BFS
// - 787. Cheapest Flights Within K Stops: 带约束最短路
// ============================================================
