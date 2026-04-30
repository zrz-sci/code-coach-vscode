/*
 * LeetCode 886: Possible Bipartition (可能的二分法)
 *
 * 【题目本质】
 * 图的二分性检测：把人看作节点、dislikes 看作边，
 * 问图能否被二着色（相邻节点颜色不同）。
 *
 * 【解法总览】
 * 解法1: BFS 染色      — O(V+E) / O(V+E)        — 面试首选
 * 解法2: DFS 染色      — O(V+E) / O(V+E)        — 面试首选
 * 解法3: Union-Find    — O(V+E·α(V)) / O(V+E)   — 加分项
 */

// ============================================================
// 解法1: BFS 染色 — 逐层交替染色（⭐ 面试首选）
// 时间: O(V+E)  空间: O(V+E)
//
// 【思路】
// 1. 把 dislikes 关系建成无向图邻接表
// 2. 对每个未染色的节点启动 BFS
// 3. BFS 过程中交替染两种颜色（1 和 -1）
// 4. 如果发现邻居已经染了和当前节点相同的颜色 → 冲突 → false
//
// 为什么必须遍历所有节点而不是只从 1 开始？
// 因为图可能不连通——多个独立的连通分量，每个都需要单独检测。
//
// 染色示例: n=4, dislikes=[[1,2],[1,3],[2,4]]
//
//   1(+1) --- 2(-1) --- 4(+1)
//     |
//   3(-1)
//
//   节点1染+1 → 邻居2,3染-1 → 邻居4(通过2)染+1
//   没有冲突 → 可以二分 → true
//
// 冲突示例: n=3, dislikes=[[1,2],[1,3],[2,3]]
//
//   1(+1) --- 2(-1)
//     |       |
//   3(-1) --- 3 已经是 -1，但 2 也是 -1 → 冲突!
//   即 2(-1) 的邻居 3(-1) 颜色相同 → false
// ============================================================
class Solution1 {
public:
    bool possibleBipartition(int n, vector<vector<int>>& dislikes) {
        // 建邻接表，1-indexed
        vector<vector<int>> graph(n + 1);
        for (auto& d : dislikes) {
            graph[d[0]].push_back(d[1]);
            graph[d[1]].push_back(d[0]);
        }

        // color[i]: 0=未染色, 1=颜色A, -1=颜色B
        vector<int> color(n + 1, 0);

        // 遍历所有节点，处理不连通的图
        for (int i = 1; i <= n; i++) {
            if (color[i] != 0) continue; // 已在某个连通分量中处理过

            // BFS 从节点 i 开始染色
            queue<int> q;
            q.push(i);
            color[i] = 1; // 起始节点染颜色 1

            while (!q.empty()) {
                int u = q.front();
                q.pop();

                for (int v : graph[u]) {
                    if (color[v] == 0) {
                        // 邻居未染色 → 染上对立颜色
                        color[v] = -color[u];
                        q.push(v);
                    } else if (color[v] == color[u]) {
                        // 邻居已染色且颜色相同 → 不是二分图
                        return false;
                    }
                    // 邻居已染色且颜色不同 → 无冲突，继续
                }
            }
        }

        return true;
    }
};

// ============================================================
// 解法2: DFS 染色 — 递归交替染色
// 时间: O(V+E)  空间: O(V+E)
//
// 【思路】
// 与 BFS 染色逻辑完全相同，只是遍历方式从队列变成递归。
// DFS 代码更简洁，但在链状图中可能有栈溢出风险。
//
// 递归过程：
// - 当前节点 u 已染色 color[u]
// - 对 u 的每个邻居 v：
//   - v 未染色 → 染上 -color[u]，递归 dfs(v)
//   - v 已染色且 color[v]==color[u] → 冲突
//   - v 已染色且 color[v]!=color[u] → 无冲突
// ============================================================
class Solution2 {
public:
    bool possibleBipartition(int n, vector<vector<int>>& dislikes) {
        vector<vector<int>> graph(n + 1);
        for (auto& d : dislikes) {
            graph[d[0]].push_back(d[1]);
            graph[d[1]].push_back(d[0]);
        }

        vector<int> color(n + 1, 0);

        for (int i = 1; i <= n; i++) {
            if (color[i] == 0) {
                color[i] = 1; // 起始节点染颜色 1
                if (!dfs(graph, color, i)) {
                    return false;
                }
            }
        }
        return true;
    }

private:
    bool dfs(vector<vector<int>>& graph, vector<int>& color, int u) {
        for (int v : graph[u]) {
            if (color[v] == 0) {
                color[v] = -color[u]; // 染对立颜色
                if (!dfs(graph, color, v)) {
                    return false; // 子图中发现冲突
                }
            } else if (color[v] == color[u]) {
                return false; // 颜色冲突
            }
        }
        return true; // 当前节点的所有邻居无冲突
    }
};

// ============================================================
// 解法3: Union-Find — "敌人的敌人是朋友"
// 时间: O(V + E·α(V))  空间: O(V+E)
//
// 【思路】
// 完全不做染色，利用并查集的分组能力。
//
// 核心观察：
// 对于节点 u 和它的所有邻居 v1, v2, ..., vk，
// 这些邻居必须在同一组（都和 u 对立），
// 所以把 v1~vk 全部 union 到一起。
//
// 同时，u 不能和任何邻居在同一组，
// 所以在 union 之前检查 find(u) == find(vi)，
// 如果成立 → 冲突 → false。
//
// 示例: n=4, dislikes=[[1,2],[1,3],[2,4]]
//   u=1: 邻居={2,3} → union(2,3)，检查 find(1)!=find(2), find(1)!=find(3) ✓
//   u=2: 邻居={1,4} → union(1,4)，检查 find(2)!=find(1), find(2)!=find(4) ✓
//   u=3: 邻居={1} → 只有一个邻居，检查 find(3)!=find(1) ✓
//   u=4: 邻居={2} → 只有一个邻居，检查 find(4)!=find(2) ✓
//   全部通过 → true
// ============================================================
class Solution3 {
public:
    bool possibleBipartition(int n, vector<vector<int>>& dislikes) {
        // 建邻接表
        vector<vector<int>> graph(n + 1);
        for (auto& d : dislikes) {
            graph[d[0]].push_back(d[1]);
            graph[d[1]].push_back(d[0]);
        }

        // Union-Find 初始化
        vector<int> parent(n + 1);
        vector<int> rank_(n + 1, 0);
        for (int i = 0; i <= n; i++) {
            parent[i] = i;
        }

        for (int u = 1; u <= n; u++) {
            if (graph[u].empty()) continue;

            // 取 u 的第一个邻居作为"对立组代表"
            int firstNeighbor = graph[u][0];

            for (int v : graph[u]) {
                // 检查：u 不能和邻居 v 在同一组
                if (find(parent, u) == find(parent, v)) {
                    return false; // 冲突！
                }
                // 把所有邻居合并到同一组
                unite(parent, rank_, firstNeighbor, v);
            }
        }

        return true;
    }

private:
    // 路径压缩的 find
    int find(vector<int>& parent, int x) {
        if (parent[x] != x) {
            parent[x] = find(parent, parent[x]);
        }
        return parent[x];
    }

    // 按秩合并的 union
    void unite(vector<int>& parent, vector<int>& rank_, int a, int b) {
        int ra = find(parent, a);
        int rb = find(parent, b);
        if (ra == rb) return;
        if (rank_[ra] < rank_[rb]) swap(ra, rb);
        parent[rb] = ra;
        if (rank_[ra] == rank_[rb]) rank_[ra]++;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法       | 时间           | 空间    | 特点                |
// |-----------|---------------|--------|-------------------- |
// | BFS 染色   | O(V+E)        | O(V+E) | ⭐面试首选，最直觉   |
// | DFS 染色   | O(V+E)        | O(V+E) | 代码简洁，注意栈溢出  |
// | Union-Find | O(V+E·α(V))   | O(V+E) | 并查集练习，加分项    |
//
// 面试建议:
//   先说 BFS/DFS 染色（任选一个）→ 追问其他方法 → Union-Find
//   BFS 更安全（不会栈溢出），DFS 代码更短
//
// 【易错点】
// 1. 忘记处理不连通图:
//    ✗ 只从节点 1 开始搜索 → 忽略其他连通分量
//    ✓ 遍历 1~n，对每个未染色节点启动搜索
//
// 2. 建图时忘记双向边:
//    ✗ 只加 graph[a].push_back(b) → 缺少从 b 到 a 的边
//    ✓ 同时加 graph[b].push_back(a)
//
// 3. 节点编号 1-indexed:
//    ✗ 数组大小开 n → graph[n] 越界
//    ✓ 数组大小开 n+1
//
// 4. Union-Find 先合并再检测:
//    ✗ 先 unite(u, v) 再检查 find(u)==find(v) → 已合并，永远检测不到
//    ✓ 先检查 find(u)==find(v) 是否冲突，再 unite(firstNeighbor, v)
//
// 【面试追问 Interview Follow-ups】
// Q1: 本题和 LC 785 (Is Graph Bipartite) 的区别？
// A1: 785 直接给邻接表，本题需要先从 dislikes 数组构建邻接表。
//     核心算法完全相同。
//
// Q2: 如果分成 3 组或 k 组呢？
// A2: k=2 是多项式时间（本题），k>=3 的图着色是 NP-完全问题。
//     面试中提到这个理论区分是加分项。
//
// Q3: Union-Find 为什么不直接 union(u, v)？
// A3: u 和 v 应该在不同组。正确做法是把 u 的所有邻居合并到一起
//     （它们都在 u 的对立面），同时检查 u 和邻居不在同一组。
// ============================================================
