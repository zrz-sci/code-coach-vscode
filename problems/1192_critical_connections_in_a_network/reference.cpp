/*
 * LeetCode 1192: 查找集群内的关键连接 (Critical Connections in a Network)
 *
 * 【题目本质】
 * 在无向连通图中找所有的"桥"（Bridge）——删除后使图不连通的边。
 * 经典的 Tarjan 桥检测算法。
 *
 * 【解法总览】
 * 解法1: 暴力法 — O(E×(V+E)) / O(V+E) — 逐条删边检查连通性
 * 解法2: Tarjan 递归版 — O(V+E) / O(V+E) — 一次 DFS 找所有桥 ⭐面试首选
 * 解法3: Tarjan 迭代版 — O(V+E) / O(V+E) — 避免深递归栈溢出
 */

/*
 * 图示 - 示例1: n=4, connections=[[0,1],[1,2],[2,0],[1,3]]
 *
 *   原图:             DFS 树 (从0出发):
 *   0 --- 1           0 (disc=0, low=0)
 *   |   / |           |  ← 树边
 *   | /   |           1 (disc=1, low=0)
 *   2     3          / \  ← 树边
 *                   2    3 (disc=3, low=3)
 *                (disc=2, low=0)
 *                   ↑
 *                  回边 2→0 (使 low[2] = min(2, disc[0]) = 0)
 *
 *   桥判定:
 *     边(0,1): low[1]=0 <= disc[0]=0  → 不是桥 (1的子树能回到0)
 *     边(1,2): low[2]=0 <= disc[1]=1  → 不是桥 (2有回边到0)
 *     边(1,3): low[3]=3 >  disc[1]=1  → 是桥! (3完全无法回到1或更上方)
 *
 *   DFS 详细过程:
 *     visit(0): disc=0, low=0 → go to 1
 *     visit(1): disc=1, low=1 → go to 2
 *     visit(2): disc=2, low=2 → 回边到0: low[2]=min(2,0)=0 → return
 *     back to 1: low[1]=min(1, low[2])=min(1,0)=0, check(1,2): 0<=1 → not bridge
 *                → go to 3
 *     visit(3): disc=3, low=3 → no more neighbors → return
 *     back to 1: low[1]=min(0, low[3])=min(0,3)=0, check(1,3): 3>1 → BRIDGE!
 *     back to 0: low[0]=min(0, low[1])=min(0,0)=0, check(0,1): 0<=0 → not bridge
 */


// ============================================================
// 解法1: 暴力法 — 逐条删边，BFS 检查连通性
// 时间: O(E × (V+E))  空间: O(V+E)
//
// 【思路】
// 最直觉的方法：要知道一条边是不是桥，直接删掉它，
// 然后用 BFS/DFS 检查图是否仍然连通。
// 如果删掉后某些节点不可达，这条边就是桥。
//
// 问题：E 条边，每条都要做一次 O(V+E) 的遍历，
// 当 V=E=10^5 时约 10^10 次操作，严重超时。
// 但它帮助理解"桥"的定义，是思考的起点。
// ============================================================
class Solution1 {
public:
    vector<vector<int>> criticalConnections(int n, vector<vector<int>>& connections) {
        vector<vector<int>> result;
        int E = connections.size();
        
        for (int i = 0; i < E; i++) {
            // 构建邻接表，跳过第 i 条边（相当于删除）
            vector<vector<int>> adj(n);
            for (int j = 0; j < E; j++) {
                if (j == i) continue;
                adj[connections[j][0]].push_back(connections[j][1]);
                adj[connections[j][1]].push_back(connections[j][0]);
            }
            
            // BFS 从节点0出发，检查是否能到达所有节点
            vector<bool> visited(n, false);
            queue<int> q;
            q.push(0);
            visited[0] = true;
            int count = 1;
            while (!q.empty()) {
                int u = q.front(); q.pop();
                for (int v : adj[u]) {
                    if (!visited[v]) {
                        visited[v] = true;
                        count++;
                        q.push(v);
                    }
                }
            }
            
            // 不能访问全部节点 → 图不连通 → 这条边是桥
            if (count < n) {
                result.push_back(connections[i]);
            }
        }
        return result;
    }
};


// ============================================================
// 解法2: Tarjan 算法（递归版）— 一次 DFS 找所有桥
// 时间: O(V+E)  空间: O(V+E)
//
// 【思路】
// 暴力法的瓶颈: 对每条边都做一次全图遍历。
// 能否只做一次 DFS 就判断出所有桥？
//
// 关键观察: 在 DFS 树中，一条树边 (u→v) 是桥
// ⟺ v 的子树中没有任何回边连到 u 或 u 的祖先。
//
// 为了量化这个条件，定义两个数组:
//   disc[u]: 节点 u 被首次发现的时间戳
//   low[u]:  节点 u 及其子树能通过回边到达的最小 disc 值
//
// low[u] 的更新:
//   - 初始: low[u] = disc[u]
//   - 对树边 (u→v): low[u] = min(low[u], low[v])  // 子树能到哪，u就能到哪
//   - 对回边 (u→w): low[u] = min(low[u], disc[w])  // 通过回边直达 w
//
// 桥判定: 对树边 (u→v)，如果 low[v] > disc[u]，
// 说明 v 的整个子树都无法回到 u 或更上方，(u,v) 是桥。
//
// 为什么 low[v] == disc[u] 不是桥？
// 因为 low[v] == disc[u] 意味着 v 的子树有回边恰好连到 u 本身，
// 删掉 (u,v) 后 v 仍可通过那条回边到达 u，所以不是桥。
// ============================================================
class Solution2 {
public:
    vector<vector<int>> criticalConnections(int n, vector<vector<int>>& connections) {
        // 建邻接表
        vector<vector<int>> adj(n);
        for (auto& conn : connections) {
            adj[conn[0]].push_back(conn[1]);
            adj[conn[1]].push_back(conn[0]);
        }
        
        vector<int> disc(n, -1);  // -1 = 未访问，这是判断节点是否已访问的关键
        vector<int> low(n, 0);
        int timer = 0;
        vector<vector<int>> bridges;
        
        // DFS 核心：parent 用于跳过来时的边，避免把同一条无向边当回边
        function<void(int, int)> dfs = [&](int u, int parent) {
            disc[u] = low[u] = timer++;  // 发现 u，初始 low = disc
            
            for (int v : adj[u]) {
                if (v == parent) {
                    // 跳过来时的边（无向图中 u-v 和 v-u 是同一条边）
                    // 注意：本题无重边，所以简单跳过 parent 即可
                    // 如果有重边，需要用边的编号来区分
                    parent = -1;  // 只跳过一次！处理多条边到 parent 的情况
                    // 但本题无重边，这行可以换成 continue
                    continue;
                }
                
                if (disc[v] == -1) {
                    // v 未访问 → (u,v) 是树边
                    dfs(v, u);
                    
                    // 回溯：v 子树能到达的最远处，u 也能到达
                    low[u] = min(low[u], low[v]);
                    
                    // 桥判定：v 的子树完全无法回到 u 或更上方
                    if (low[v] > disc[u]) {
                        bridges.push_back({u, v});
                    }
                } else {
                    // v 已访问且不是 parent → (u,v) 是回边
                    // 通过这条回边，u 可以到达 disc[v] 那么早的节点
                    low[u] = min(low[u], disc[v]);
                }
            }
        };
        
        // 题目保证图连通，从 0 出发即可
        dfs(0, -1);
        return bridges;
    }
};


// ============================================================
// 解法3: Tarjan 算法（迭代版）— 用显式栈避免递归栈溢出
// 时间: O(V+E)  空间: O(V+E)
//
// 【思路】
// 解法2 的递归版在极端情况（如图退化为链，深度达 10^5）
// 可能导致栈溢出。用显式栈模拟 DFS 过程来解决。
//
// 关键技巧：用 idx[u] 数组记录节点 u 遍历邻居列表的进度，
// 这样从栈中弹出并重新处理时，能从上次断点继续。
// ============================================================
class Solution3 {
public:
    vector<vector<int>> criticalConnections(int n, vector<vector<int>>& connections) {
        vector<vector<int>> adj(n);
        for (auto& conn : connections) {
            adj[conn[0]].push_back(conn[1]);
            adj[conn[1]].push_back(conn[0]);
        }
        
        vector<int> disc(n, -1), low(n, 0);
        vector<int> par(n, -1);   // 记录每个节点的 DFS 父节点
        vector<int> idx(n, 0);    // idx[u]: u 当前遍历到邻居列表的第几个
        int timer = 0;
        vector<vector<int>> bridges;
        
        stack<int> stk;
        disc[0] = low[0] = timer++;
        stk.push(0);
        
        while (!stk.empty()) {
            int u = stk.top();
            
            if (idx[u] < (int)adj[u].size()) {
                // u 还有未处理的邻居
                int v = adj[u][idx[u]++];
                
                if (v == par[u]) continue;  // 跳过父边
                
                if (disc[v] == -1) {
                    // 树边：v 未访问，"递归"进入 v
                    disc[v] = low[v] = timer++;
                    par[v] = u;
                    stk.push(v);
                } else {
                    // 回边：v 已访问，用 disc[v] 更新 low[u]
                    low[u] = min(low[u], disc[v]);
                }
            } else {
                // u 的所有邻居都处理完毕 → "递归返回"
                stk.pop();
                
                if (par[u] != -1) {
                    // 等价于递归版中 dfs(u, par[u]) 返回后的操作
                    int p = par[u];
                    low[p] = min(low[p], low[u]);
                    
                    if (low[u] > disc[p]) {
                        bridges.push_back({p, u});
                    }
                }
            }
        }
        
        return bridges;
    }
};


// ============================================================
// 【解法对比】
//
// | 维度        | 暴力法          | Tarjan 递归    | Tarjan 迭代    |
// |------------|----------------|---------------|---------------|
// | 时间复杂度   | O(E×(V+E))     | O(V+E)        | O(V+E)        |
// | 空间复杂度   | O(V+E)         | O(V+E)+递归栈  | O(V+E)        |
// | 适用场景     | 口述理解         | 面试首选       | 数据极大/防爆栈  |
// | 代码量       | 简单            | 简洁           | 较多           |
//
// 面试中首选递归版 Tarjan（Solution2），简洁且能清楚展示算法思想。
// 如果面试官追问栈溢出风险，提及迭代版（Solution3）。
//
// ============================================================
// 【易错点】
//
// 1. disc 数组初始化为 0 而非 -1：
//    ✗ vector<int> disc(n, 0) — 节点0的disc本来就是0，无法区分"未访问"
//    ✓ vector<int> disc(n, -1) — 用-1明确标记未访问状态
//
// 2. 桥判定条件写成 >= 而非 >：
//    ✗ if (low[v] >= disc[u]) — 这是割点的判定条件!
//    ✓ if (low[v] > disc[u])  — 桥的判定，必须严格大于
//    原因：low[v]==disc[u] 意味着 v 子树有回边到 u，删(u,v)后v仍可达u
//
// 3. 回边更新 low 时搞混 disc[v] 和 low[v]：
//    两种在找桥时都正确，但建议统一用 disc[v]。
//    因为如果未来改写为找割点，用 low[v] 会出错。
//
// 4. 无向图中忘记跳过父节点：
//    ✗ 不判断 v == parent，导致树边被当成回边
//    ✓ if (v == parent) continue; 跳过来时的那条边
//
// 5. 有重边时简单跳过 parent 会出错：
//    本题明确无重边，但如果有重边（如两条 0-1），
//    简单 `v == parent` 会把两条都跳过。
//    需要用边编号区分：只跳过"来时走的那条具体的边"。
//
// ============================================================
// 【面试追问 — 递进链】
//
// Q1: "暴力法怎么做？时间复杂度？"
//     → 逐条删边 + BFS 检查连通性，O(E × (V+E))。
//       这展示了对"桥"定义的理解。
//
// Q2: "Tarjan 的 low 数组是什么意思？为什么 low[v]>disc[u] 就是桥？"
//     → low[v] 是 v 子树通过回边能到达的最早祖先的时间戳。
//       如果 low[v] > disc[u]，说明 v 的子树完全封闭，
//       没有任何"后路"能绕回 u 或 u 上方。
//
// Q3: "如果要找割点呢？条件怎么改？"
//     → 非根节点 u: 存在子节点 v 使得 low[v] >= disc[u]（>= 不是 >）
//       根节点: DFS 树中有 ≥2 个子节点
//       区别：桥用 >，割点用 >=，且根要特判。
//
// Q4: "图可能有重边怎么处理？"
//     → 不能简单用 v==parent 跳过。用边编号 (edge_id) 标识，
//       只跳过"来时走的那条边"(skip_edge_id)，不影响平行边。
//
// Q5: "递归太深会爆栈，怎么办？"
//     → 用显式栈 + idx 数组模拟 DFS（Solution3）。
//       idx[u] 记录 u 遍历邻居的进度，回溯时从断点继续。
//
// ============================================================
// 【相关题型】
//
// - LeetCode 323 (无向图连通分量数) — 更基础的图连通性，
//   复用邻接表+DFS框架，但不需要 disc/low
//
// - LeetCode 207/210 (课程表) — 有向图 DFS 检测环 + 拓扑排序，
//   复用"邻接表+DFS"框架，区别在于有向/无向和visited状态(三色标记)
//
// - 割点问题 (Articulation Point) — Tarjan 的姊妹问题，
//   直接复用 disc/low 框架，只改判定条件: > 改 >=，根特判
//
// - 双连通分量 (Biconnected Component) — 在 Tarjan 基础上用栈
//   记录边，找到桥/割点时弹出一个分量
// ============================================================
