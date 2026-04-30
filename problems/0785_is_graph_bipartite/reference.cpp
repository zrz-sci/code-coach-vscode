// 785. Is Graph Bipartite?
// 二分图判定: 二染色法 / 并查集

// ============================================================
// 解法一: BFS 染色法 (推荐)
// 时间复杂度: O(V + E), 空间复杂度: O(V)
// ============================================================
class Solution {
public:
    bool isBipartite(vector<vector<int>>& graph) {
        int n = graph.size();
        // color[i]: -1=未染色, 0=颜色A, 1=颜色B
        vector<int> color(n, -1);

        // 图可能不连通, 需要对每个连通分量分别检查
        for (int i = 0; i < n; ++i) {
            if (color[i] != -1) continue; // 已访问

            // BFS 从节点 i 开始染色
            queue<int> q;
            q.push(i);
            color[i] = 0;

            while (!q.empty()) {
                int u = q.front();
                q.pop();

                for (int v : graph[u]) {
                    if (color[v] == -1) {
                        // 未染色: 染上与 u 相反的颜色
                        color[v] = 1 - color[u];
                        q.push(v);
                    } else if (color[v] == color[u]) {
                        // 已染色且与 u 同色: 存在奇环, 非二分图
                        return false;
                    }
                    // 已染色且颜色不同: 合法, 跳过
                }
            }
        }

        return true;
    }
};

// ============================================================
// 解法二: DFS 染色法
// 时间复杂度: O(V + E), 空间复杂度: O(V)
// ============================================================
class Solution_DFS {
public:
    bool isBipartite(vector<vector<int>>& graph) {
        int n = graph.size();
        vector<int> color(n, -1);

        for (int i = 0; i < n; ++i) {
            if (color[i] != -1) continue;
            // 对每个未访问的连通分量启动 DFS
            if (!dfs(graph, color, i, 0)) {
                return false;
            }
        }
        return true;
    }

    bool dfs(vector<vector<int>>& graph, vector<int>& color, int u, int c) {
        color[u] = c;

        for (int v : graph[u]) {
            if (color[v] == -1) {
                // 未染色: 递归染相反颜色
                if (!dfs(graph, color, v, 1 - c)) {
                    return false;
                }
            } else if (color[v] == c) {
                // 同色冲突
                return false;
            }
        }
        return true;
    }
};

// ============================================================
// 解法三: 并查集
// 时间复杂度: O(V + E * alpha(V)), 空间复杂度: O(V)
//
// 思路: 对于每个节点u, 它的所有邻居应该在同一侧(与u不同侧)
//       把u的所有邻居union到一起
//       如果u和某个邻居在同一集合中, 说明非二分图
// ============================================================
class Solution_UnionFind {
public:
    vector<int> parent, rank_;

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]); // 路径压缩
        }
        return parent[x];
    }

    void unite(int x, int y) {
        int rx = find(x), ry = find(y);
        if (rx == ry) return;
        // 按秩合并
        if (rank_[rx] < rank_[ry]) swap(rx, ry);
        parent[ry] = rx;
        if (rank_[rx] == rank_[ry]) rank_[rx]++;
    }

    bool isBipartite(vector<vector<int>>& graph) {
        int n = graph.size();
        parent.resize(n);
        rank_.resize(n, 0);
        for (int i = 0; i < n; ++i) parent[i] = i;

        for (int u = 0; u < n; ++u) {
            if (graph[u].empty()) continue;

            // 将u的所有邻居union到一起(它们应该在同一侧)
            for (int j = 1; j < (int)graph[u].size(); ++j) {
                unite(graph[u][0], graph[u][j]);
            }

            // 检查u是否和某个邻居在同一集合
            for (int v : graph[u]) {
                if (find(u) == find(v)) {
                    return false;
                }
            }
        }

        return true;
    }
};

// ============================================================
// 解法四: DFS 染色 (简洁lambda写法)
// 时间复杂度: O(V + E), 空间复杂度: O(V)
// ============================================================
class Solution_Lambda {
public:
    bool isBipartite(vector<vector<int>>& graph) {
        int n = graph.size();
        vector<int> color(n, -1);

        // lambda DFS
        function<bool(int, int)> dfs = [&](int u, int c) -> bool {
            color[u] = c;
            for (int v : graph[u]) {
                if (color[v] == -1) {
                    if (!dfs(v, 1 - c)) return false;
                } else if (color[v] == c) {
                    return false;
                }
            }
            return true;
        };

        for (int i = 0; i < n; ++i) {
            if (color[i] == -1) {
                if (!dfs(i, 0)) return false;
            }
        }
        return true;
    }
};
