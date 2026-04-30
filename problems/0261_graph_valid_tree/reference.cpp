/*
 * 【题目本质】
 * 判断无向图是否为有效树。
 * 树 = 无环 + 连通的无向图。
 * 等价条件：边数 = n-1 且 (无环 或 连通)。
 *
 * 【解法总览】
 * 解法1: Union Find（并查集）- O(E*α(N)) ⭐ 面试首选
 * 解法2: DFS 检测连通性 - O(N+E)
 * 解法3: BFS 检测连通性 - O(N+E)
 */

// ============================================================
// 解法1: Union Find（并查集）⭐ 推荐
// 思路：先检查边数 = n-1，再用并查集检测是否有环
// 时间 O(E * α(N))，空间 O(N)
// ============================================================
class Solution1 {
public:
    vector<int> parent, rank_;

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]); // 路径压缩
        }
        return parent[x];
    }

    bool unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return false; // 同一集合 -> 有环
        // 按秩合并
        if (rank_[px] < rank_[py]) swap(px, py);
        parent[py] = px;
        if (rank_[px] == rank_[py]) rank_[px]++;
        return true;
    }

    bool validTree(int n, vector<vector<int>>& edges) {
        // 必要条件：树有 n-1 条边
        if ((int)edges.size() != n - 1) return false;

        parent.resize(n);
        rank_.resize(n, 0);
        iota(parent.begin(), parent.end(), 0); // parent[i] = i

        for (auto& e : edges) {
            if (!unite(e[0], e[1])) return false;
        }
        // 边数 = n-1 且无环 => 一定连通 => 是树
        return true;
    }
};

// ============================================================
// 解法2: DFS 检测连通性
// 思路：先检查边数 = n-1，再用 DFS 检查是否所有节点连通
// 时间 O(N + E)，空间 O(N + E)
// ============================================================
class Solution2 {
public:
    bool validTree(int n, vector<vector<int>>& edges) {
        if ((int)edges.size() != n - 1) return false;

        // 建邻接表
        vector<vector<int>> adj(n);
        for (auto& e : edges) {
            adj[e[0]].push_back(e[1]);
            adj[e[1]].push_back(e[0]);
        }

        // 迭代 DFS
        vector<bool> visited(n, false);
        stack<int> stk;
        stk.push(0);
        visited[0] = true;
        int count = 0;

        while (!stk.empty()) {
            int node = stk.top(); stk.pop();
            count++;
            for (int nei : adj[node]) {
                if (!visited[nei]) {
                    visited[nei] = true;
                    stk.push(nei);
                }
            }
        }
        return count == n;
    }
};

// ============================================================
// 解法3: BFS 检测连通性
// 思路：与 DFS 类似，改用队列
// 时间 O(N + E)，空间 O(N + E)
// ============================================================
class Solution3 {
public:
    bool validTree(int n, vector<vector<int>>& edges) {
        if ((int)edges.size() != n - 1) return false;

        vector<vector<int>> adj(n);
        for (auto& e : edges) {
            adj[e[0]].push_back(e[1]);
            adj[e[1]].push_back(e[0]);
        }

        vector<bool> visited(n, false);
        queue<int> q;
        q.push(0);
        visited[0] = true;
        int count = 0;

        while (!q.empty()) {
            int node = q.front(); q.pop();
            count++;
            for (int nei : adj[node]) {
                if (!visited[nei]) {
                    visited[nei] = true;
                    q.push(nei);
                }
            }
        }
        return count == n;
    }
};

/*
 * 【解法对比】
 * | 解法        | 时间         | 空间      | 优势             |
 * |------------|-------------|----------|-----------------|
 * | Union Find | O(E*α(N))   | O(N)     | 空间最优，支持在线 |
 * | DFS        | O(N+E)      | O(N+E)   | 代码直观         |
 * | BFS        | O(N+E)      | O(N+E)   | 层序遍历变体      |
 *
 * 【易错点】
 * 1. 忘记先检查 edges.size() == n-1，直接做 DFS/UF 可能漏判断
 * 2. 无向图 DFS 必须标记 visited，否则来回访问导致 TLE
 * 3. n=1 时 edges 为空是合法树，不要 return false
 * 4. Union Find 忘记路径压缩导致退化为 O(N) 每次查找
 *
 * 【面试追问】
 * Q: 如果边是一条条动态添加的，怎么实时判断？
 * A: Union Find 天然支持动态添加，每加一条边 unite 一次，
 *    若 unite 返回 false 说明形成环。
 *
 * Q: 如何求无向图的连通分量数？
 * A: Union Find 做完后统计 parent[i]==i 的个数。
 *    或做 DFS/BFS 统计启动次数。
 */
