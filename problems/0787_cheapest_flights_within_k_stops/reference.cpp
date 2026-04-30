/*
 * LeetCode 787: Cheapest Flights Within K Stops
 *
 * 【题目本质】
 * 带步数限制的单源最短路径：从 src 到 dst，最多经过 k+1 条边（k次中转），
 * 求最小花费。
 *
 * 【解法总览】
 * 解法1: Bellman-Ford (k+1轮松弛) — O(k·E) / O(n)   — 面试首选 ⭐
 * 解法2: BFS 逐层扩展            — O(k·E) / O(n+E)  — 直觉清晰
 * 解法3: DP 二维表               — O(k·E) / O(k·n)  — 理解本质
 * 解法4: Dijkstra + 状态扩展     — O(E·k·log(nk)) / O(nk) — 加分项
 *
 * 【图示 — 示例1】
 *
 *     0 ---100---> 1 ---100---> 2
 *     ^            |            |
 *     |           600          200
 *     |            v            v
 *     +---100------+            3
 *
 *   k=1: 0→1→3 = 700 ✓ (1次中转)
 *         0→1→2→3 = 400 ✗ (2次中转，超出)
 *
 * 【Bellman-Ford 松弛过程 — 示例1】
 *
 *   初始:    dist = [0, INF, INF, INF]
 *
 *   第1轮 (最多1条边):
 *     prev = [0, INF, INF, INF]    ← 拷贝快照
 *     边(0→1,100): prev[0]=0  → dist[1] = min(INF, 0+100) = 100
 *     边(1→2,100): prev[1]=INF → 跳过
 *     边(2→0,100): prev[2]=INF → 跳过
 *     边(1→3,600): prev[1]=INF → 跳过
 *     边(2→3,200): prev[2]=INF → 跳过
 *     结果: dist = [0, 100, INF, INF]
 *
 *   第2轮 (最多2条边):
 *     prev = [0, 100, INF, INF]   ← 拷贝快照
 *     边(0→1,100): prev[0]=0  → dist[1] = min(100, 100) = 100 (不变)
 *     边(1→2,100): prev[1]=100 → dist[2] = min(INF, 200) = 200
 *     边(2→0,100): prev[2]=INF → 跳过
 *     边(1→3,600): prev[1]=100 → dist[3] = min(INF, 700) = 700
 *     边(2→3,200): prev[2]=INF → 跳过
 *     结果: dist = [0, 100, 200, 700]
 *
 *   答案: dist[3] = 700 ✓
 *
 * 【为什么要拷贝 prev？反例】
 *   如果不拷贝，第1轮中:
 *     边(0→1,100): dist[1] = 100
 *     边(1→2,100): dist[1]已=100 → dist[2] = 200  ← 错! 一轮走了2条边!
 *   这相当于 k=0 时就能 0→1→2，但实际需要2次中转。
 */

// ============================================================
// 解法1: Bellman-Ford (k+1轮松弛) — 面试首选 ⭐
// 时间: O((k+1) * E)  空间: O(n)
//
// 【思路】
// Bellman-Ford 的第 t 轮松弛得到的是"最多经过 t 条边"的最短路。
// k 次中转 = k+1 条边，所以跑 k+1 轮即可。
// 每轮必须用上一轮的快照做松弛源，防止同一轮内连续松弛多条边。
// 这是 Bellman-Ford 的标准做法在"限制边数"场景下的直接应用。
// ============================================================
class Solution1 {
public:
    int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int k) {
        const int INF = 1e9;
        vector<int> dist(n, INF);
        dist[src] = 0;

        // k 次中转 = 最多 k+1 条边 → 松弛 k+1 轮
        for (int t = 0; t <= k; t++) {
            // 必须拷贝：确保本轮只基于上一轮的结果松弛
            vector<int> prev = dist;
            for (auto& f : flights) {
                int u = f[0], v = f[1], w = f[2];
                // 只有 prev[u] 可达时才松弛，避免 INF + w 溢出
                if (prev[u] < INF) {
                    dist[v] = min(dist[v], prev[u] + w);
                }
            }
        }

        return dist[dst] >= INF ? -1 : dist[dst];
    }
};

// ============================================================
// 解法2: BFS 逐层扩展
// 时间: O((k+1) * E)  空间: O(n + E)
//
// 【思路】
// 把问题看成 BFS：每一"层"代表多走一条边（多一次中转）。
// 从 src 开始，最多展开 k+1 层。每层更新各节点的最小花费。
// 和 Bellman-Ford 本质一样：按步数展开，只是用队列组织遍历顺序。
// 优点是更直觉：BFS 天然按"步数"分层。
// ============================================================
class Solution2 {
public:
    int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int k) {
        // 建邻接表
        vector<vector<pair<int, int>>> graph(n);
        for (auto& f : flights) {
            graph[f[0]].push_back({f[1], f[2]});
        }

        const int INF = 1e9;
        vector<int> dist(n, INF);
        dist[src] = 0;

        // BFS 队列存 (节点, 到达该节点的花费)
        queue<pair<int, int>> q;
        q.push({src, 0});
        int stops = 0;

        while (!q.empty() && stops <= k) {
            int sz = q.size();
            // 拷贝当前层开始时的 dist，防止本层内连锁更新
            vector<int> temp = dist;
            while (sz--) {
                auto [u, costU] = q.front();
                q.pop();
                for (auto& [v, w] : graph[u]) {
                    int newCost = costU + w;
                    // 只有比当前已知的更优才入队（剪枝）
                    if (newCost < temp[v]) {
                        temp[v] = newCost;
                        q.push({v, newCost});
                    }
                }
            }
            dist = temp;
            stops++;
        }

        return dist[dst] >= INF ? -1 : dist[dst];
    }
};

// ============================================================
// 解法3: DP 二维表
// 时间: O((k+1) * E)  空间: O((k+2) * n)
//
// 【思路】
// 显式定义状态: dp[t][v] = 从 src 出发，最多经过 t 条边到达 v 的最小花费。
// 状态转移: dp[t][v] = min(dp[t-1][v], min_{所有边(u→v,w)} dp[t-1][u] + w)
// 答案: dp[k+1][dst]
//
// 【DP 填表方向】
//         node:  0      1      2      3
// t=0 (0边):    0      INF    INF    INF     ← base case
// t=1 (1边):    0      100    INF    INF     ← 只看 t=0 行做转移
// t=2 (2边):    0      100    200    700     ← 只看 t=1 行做转移
//                                     ↑ 答案
//
// 这就是 Bellman-Ford 的 DP 本质。解法1 是它的空间优化版（滚动数组）。
// ============================================================
class Solution3 {
public:
    int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int k) {
        const int INF = 1e9;
        // dp[t][v]: 最多经过 t 条边从 src 到 v 的最小花费
        vector<vector<int>> dp(k + 2, vector<int>(n, INF));
        dp[0][src] = 0;

        for (int t = 1; t <= k + 1; t++) {
            // 先继承上一行（不走新边的情况）
            dp[t] = dp[t - 1];
            // 尝试每条边做松弛
            for (auto& f : flights) {
                int u = f[0], v = f[1], w = f[2];
                if (dp[t - 1][u] < INF) {
                    dp[t][v] = min(dp[t][v], dp[t - 1][u] + w);
                }
            }
        }

        return dp[k + 1][dst] >= INF ? -1 : dp[k + 1][dst];
    }
};

// ============================================================
// 解法3b: DP 空间优化 (滚动数组)
// 时间: O((k+1) * E)  空间: O(n)
//
// 【思路】
// dp[t] 只依赖 dp[t-1]，所以只需两个一维数组滚动。
// 这就退化成了解法1的 Bellman-Ford！
// 写出来是为了展示 DP 到 Bellman-Ford 的推导过程。
// ============================================================
class Solution3b {
public:
    int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int k) {
        const int INF = 1e9;
        vector<int> dp(n, INF);
        dp[src] = 0;

        for (int t = 1; t <= k + 1; t++) {
            vector<int> prev = dp; // 上一轮的快照
            for (auto& f : flights) {
                int u = f[0], v = f[1], w = f[2];
                if (prev[u] < INF) {
                    dp[v] = min(dp[v], prev[u] + w);
                }
            }
        }

        return dp[dst] >= INF ? -1 : dp[dst];
    }
};

// ============================================================
// 解法4: Dijkstra + 状态扩展
// 时间: O(E * k * log(n*k))  空间: O(n * k)
//
// 【思路】
// 普通 Dijkstra 对每个节点只确定一次最短距离，但本题有步数限制。
// 关键洞察：到达同一节点，使用不同的中转次数是不同的"状态"。
// 状态定义: (cost, node, stopsUsed)，用优先队列按 cost 排序。
//
// 剪枝: visited[v] 记录到达 v 的最少 stops 数。
// 如果当前 stops >= visited[v]，说明之前用更少步数、更低花费到过 v，跳过。
// 这个剪枝之所以正确，是因为优先队列保证先出的 cost 更小，
// 所以同一个 (v, stops) 第一次出队时一定是该状态下的最优。
// ============================================================
class Solution4 {
public:
    int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int k) {
        // 建邻接表
        vector<vector<pair<int, int>>> graph(n);
        for (auto& f : flights) {
            graph[f[0]].push_back({f[1], f[2]});
        }

        // 优先队列: (cost, node, stopsUsed)，按 cost 小顶堆
        priority_queue<tuple<int, int, int>,
                       vector<tuple<int, int, int>>,
                       greater<>> pq;
        pq.push({0, src, 0});

        // visited[v] = 到达 v 时使用过的最少 stops 数
        // 注意不是记录最小 cost，因为同一节点可能需要用更多 stops 但更低 cost 的路径
        vector<int> visited(n, INT_MAX);

        while (!pq.empty()) {
            auto [cost, u, stops] = pq.top();
            pq.pop();

            // 到达目的地，由于优先队列按 cost 排序，第一次到达就是最优
            if (u == dst) return cost;

            // 中转次数超限，剪枝
            if (stops > k) continue;

            // 之前用更少 stops 到过 u（且 cost 一定 ≤ 当前），跳过
            if (stops >= visited[u]) continue;
            visited[u] = stops;

            // 扩展邻居
            for (auto& [v, w] : graph[u]) {
                pq.push({cost + w, v, stops + 1});
            }
        }

        return -1;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法        | 时间            | 空间      | 特点                |
// |-------------|-----------------|-----------|---------------------|
// | Bellman-Ford| O((k+1)·E)     | O(n)      | 最简洁，面试首选     |
// | BFS         | O((k+1)·E)     | O(n+E)    | 直觉清晰            |
// | DP 二维     | O((k+1)·E)     | O(k·n)    | 能看完整 DP 表      |
// | DP 空间优化 | O((k+1)·E)     | O(n)      | = Bellman-Ford      |
// | Dijkstra    | O(Ek·log(nk))  | O(nk)     | 可提前终止          |
//
// 面试场景：
// - 默认写 Bellman-Ford（代码最短，10行搞定）
// - 追问"还有别的方法吗？"→ 说 BFS 或 DP 视角
// - 追问"能用 Dijkstra 吗？"→ 解释状态扩展的思路
//
// 【易错点】
//
// 1. ✗ 不拷贝上一轮的 dist 直接松弛：
//      dist[v] = min(dist[v], dist[u] + w)  ← 一轮可能走多条边
//    ✓ 用 prev 拷贝:
//      vector<int> prev = dist;
//      dist[v] = min(dist[v], prev[u] + w)
//
// 2. ✗ 循环 k 次: for(int t=0; t<k; t++) → 只松弛了 k 轮 = k 条边 = k-1 次中转
//    ✓ 循环 k+1 次: for(int t=0; t<=k; t++)
//
// 3. ✗ 不检查 prev[u] < INF 就松弛 → INF + w 可能溢出
//    ✓ if (prev[u] < INF) dist[v] = min(dist[v], prev[u] + w)
//
// 4. ✗ Dijkstra 用 dist[u] < cost 剪枝（普通 Dijkstra 的方式）
//    ✓ 用 stops >= visited[u] 剪枝（本题需要按 stops 维度判断）
//
// 【面试追问 — 递进链】
//
// Q1: "暴力怎么做？瓶颈在哪？"
// A1: DFS 枚举所有路径，最坏指数级。瓶颈是大量重复子问题（同一节点同一步数被多次访问）。
//
// Q2: "Bellman-Ford 每轮为什么要拷贝数组？"
// A2: 不拷贝会导致一轮内 A→B→C 连续松弛两条边，违反"每轮只多走一条边"的约束。
//     对应 DP 中 dp[t][v] 只能用 dp[t-1] 转移，不能用 dp[t] 转移。
//
// Q3: "如果没有 k 的限制呢？"
// A3: 就是标准的单源最短路，可以用不限轮数的 Bellman-Ford 或普通 Dijkstra。
//     Bellman-Ford 跑 n-1 轮即可（最短路最多 n-1 条边）。
//
// Q4: "如果有大量查询 (不同 src, dst, k)？"
// A4: Floyd-Warshall 变体: dp[k][i][j] 预处理所有城市对、所有步数限制。
//     O(K·n³) 预处理，O(1) 查询。
// ============================================================
