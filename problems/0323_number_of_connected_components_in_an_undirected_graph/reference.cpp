/*
 * 【题目本质】
 * 求无向图的连通分量数 — 图论基础问题
 * 三种经典方法：DFS / BFS / Union-Find
 *
 * 【解法总览】
 * Solution1: DFS — 邻接表 + visited 标记，启动次数 = 连通分量数
 * Solution2: BFS — 队列遍历，与 DFS 等价
 * Solution3: Union-Find — 路径压缩 + 按秩合并，适用于动态连通性
 */

// ===================== Solution1: DFS =====================
// 思路：建邻接表，从每个未访问节点启动DFS，启动次数即连通分量数
// 时间 O(V+E), 空间 O(V+E)
class Solution1 {
public:
    int countComponents(int n, vector<vector<int>>& edges) {
        vector<vector<int>> adj(n);
        for (auto& e : edges) {
            adj[e[0]].push_back(e[1]);
            adj[e[1]].push_back(e[0]);
        }

        vector<bool> visited(n, false);
        int count = 0;

        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                dfs(adj, visited, i);
                count++;
            }
        }
        return count;
    }

private:
    void dfs(vector<vector<int>>& adj, vector<bool>& visited, int u) {
        visited[u] = true;
        for (int v : adj[u]) {
            if (!visited[v]) {
                dfs(adj, visited, v);
            }
        }
    }
};

// ===================== Solution2: BFS =====================
// 思路：队列遍历代替递归，避免栈溢出风险
// 时间 O(V+E), 空间 O(V+E)
class Solution2 {
public:
    int countComponents(int n, vector<vector<int>>& edges) {
        vector<vector<int>> adj(n);
        for (auto& e : edges) {
            adj[e[0]].push_back(e[1]);
            adj[e[1]].push_back(e[0]);
        }

        vector<bool> visited(n, false);
        int count = 0;

        for (int i = 0; i < n; i++) {
            if (visited[i]) continue;
            count++;
            queue<int> q;
            q.push(i);
            visited[i] = true;
            while (!q.empty()) {
                int u = q.front(); q.pop();
                for (int v : adj[u]) {
                    if (!visited[v]) {
                        visited[v] = true;
                        q.push(v);
                    }
                }
            }
        }
        return count;
    }
};

// ===================== Solution3: Union-Find =====================
// 思路：初始 n 个独立集合，每条边合并两个集合
// 成功合并一次 count--，最终 count 即连通分量数
// 时间 O(V + E*alpha(V)) ≈ O(V+E), 空间 O(V)
class Solution3 {
public:
    int countComponents(int n, vector<vector<int>>& edges) {
        parent.resize(n);
        rank_.resize(n, 0);
        iota(parent.begin(), parent.end(), 0);

        int count = n;
        for (auto& e : edges) {
            if (unite(e[0], e[1])) {
                count--;
            }
        }
        return count;
    }

private:
    vector<int> parent, rank_;

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);  // 路径压缩
        }
        return parent[x];
    }

    bool unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return false;  // 已同组
        // 按秩合并
        if (rank_[px] < rank_[py]) swap(px, py);
        parent[py] = px;
        if (rank_[px] == rank_[py]) rank_[px]++;
        return true;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 优势 |
 * |------|------|------|------|
 * | Solution1 DFS | O(V+E) | O(V+E) | 最直观，面试首选 |
 * | Solution2 BFS | O(V+E) | O(V+E) | 避免递归栈溢出 |
 * | Solution3 UF | O(V+E*α) | O(V) | 动态连通性场景最优 |
 *
 * 【易错点】
 * 1. 无向图建邻接表时必须双向加边
 * 2. DFS 进入节点立即标记 visited，否则死循环
 * 3. Union-Find 必须路径压缩，否则退化为 O(n)
 * 4. Union-Find 中 count 初始为 n（每个节点一个分量）
 * 5. BFS 入队时标记 visited（而非出队时），避免重复入队
 *
 * 【面试追问】
 * Q1: 如果边是动态添加的，用哪种方法？
 *     Union-Find 最优 — 每次 union O(α(n)) ≈ O(1)
 * Q2: 如何扩展到有向图的强连通分量？
 *     用 Tarjan 或 Kosaraju 算法
 * Q3: 判断图是否为树？(LC 261)
 *     连通分量=1 且 边数=n-1（或 Union-Find 中无环）
 */
