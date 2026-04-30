/*
 * LeetCode 399: 除法求值 (Evaluate Division)
 *
 * 【题目本质】
 * 带权有向图的路径查询：变量是节点，a/b=k 是边权，求 c/d 就是找 c→d 路径的边权乘积。
 *
 * 【解法总览】
 * 解法1: BFS 图搜索        — O(Q×(V+E)) / O(V+E)  — 面试首选，思路清晰
 * 解法2: DFS 图搜索        — O(Q×(V+E)) / O(V+E)  — BFS 的递归版本
 * 解法3: Floyd 全源最短路   — O(V³+Q)   / O(V²)   — 预处理后查询 O(1)
 * 解法4: 带权并查集         — O((E+Q)α(V)) / O(V)  — 最优，适合动态场景
 */

// ============================================================
// 解法1: BFS — 建图 + 每次查询做 BFS 寻路
// 时间: O(Q × (V+E))  空间: O(V+E)
//
// 【思路】
// 把除法关系转化为带权有向图：
//   a/b = 2.0  →  a--2.0-->b  和  b--0.5-->a
//
// 对于查询 c/d，从 c 出发 BFS，维护累积乘积，
// 到达 d 时的乘积就是 c/d 的值。
//
// 图示 (a/b=2, b/c=3):
//
//        2.0       3.0
//   a ------→ b ------→ c
//     ←------   ←------
//       0.5      1/3
//
// 查询 a/c: BFS a→b(×2.0)→c(×3.0) = 6.0
// 查询 b/a: BFS b→a(×0.5) = 0.5
// ============================================================
class Solution1 {
public:
    vector<double> calcEquation(vector<vector<string>>& equations,
                                vector<double>& values,
                                vector<vector<string>>& queries) {
        // 建图：邻接表，每条边存 (邻居, 权重)
        unordered_map<string, vector<pair<string, double>>> graph;
        for (int i = 0; i < (int)equations.size(); i++) {
            const string& a = equations[i][0];
            const string& b = equations[i][1];
            // 双向边：a→b 权重 val，b→a 权重 1/val
            graph[a].push_back({b, values[i]});
            graph[b].push_back({a, 1.0 / values[i]});
        }

        vector<double> results;
        for (auto& q : queries) {
            results.push_back(bfs(q[0], q[1], graph));
        }
        return results;
    }

private:
    double bfs(const string& src, const string& dst,
               unordered_map<string, vector<pair<string, double>>>& graph) {
        // 变量不在图中 → 无法求值
        if (!graph.count(src) || !graph.count(dst)) return -1.0;
        // 自己除以自己 = 1.0（前提是在图中）
        if (src == dst) return 1.0;

        // 队列: (当前节点, 从src到此节点的累积乘积)
        queue<pair<string, double>> q;
        unordered_set<string> visited;
        q.push({src, 1.0});
        visited.insert(src);

        while (!q.empty()) {
            auto [node, product] = q.front();
            q.pop();

            for (auto& [neighbor, weight] : graph[node]) {
                if (neighbor == dst) {
                    return product * weight; // 到达目标，返回累积乘积
                }
                if (!visited.count(neighbor)) {
                    visited.insert(neighbor);   // 入队时标记，防止重复
                    q.push({neighbor, product * weight});
                }
            }
        }
        return -1.0; // src 和 dst 不连通
    }
};

// ============================================================
// 解法2: DFS — 建图 + 每次查询做 DFS 寻路
// 时间: O(Q × (V+E))  空间: O(V+E)
//
// 【思路】
// 和 BFS 完全一样的建图方式，区别只在搜索策略。
// DFS 用递归实现，代码更短，但需要注意：
// - 必须传 visited 集合防止在双向边上来回跳
// - 找到 dst 后要把结果层层返回
//
// 对有些人来说 DFS 更直觉：
//   "从 src 出发，尝试每个邻居，看能不能走到 dst"
// ============================================================
class Solution2 {
public:
    vector<double> calcEquation(vector<vector<string>>& equations,
                                vector<double>& values,
                                vector<vector<string>>& queries) {
        unordered_map<string, vector<pair<string, double>>> graph;
        for (int i = 0; i < (int)equations.size(); i++) {
            graph[equations[i][0]].push_back({equations[i][1], values[i]});
            graph[equations[i][1]].push_back({equations[i][0], 1.0 / values[i]});
        }

        vector<double> results;
        for (auto& q : queries) {
            if (!graph.count(q[0]) || !graph.count(q[1])) {
                results.push_back(-1.0);
                continue;
            }
            unordered_set<string> visited;
            double res = dfs(q[0], q[1], 1.0, visited, graph);
            results.push_back(res);
        }
        return results;
    }

private:
    double dfs(const string& curr, const string& dst, double product,
               unordered_set<string>& visited,
               unordered_map<string, vector<pair<string, double>>>& graph) {
        if (curr == dst) return product; // 到达目标

        visited.insert(curr);
        for (auto& [neighbor, weight] : graph[curr]) {
            if (!visited.count(neighbor)) {
                double result = dfs(neighbor, dst, product * weight, visited, graph);
                if (result != -1.0) return result; // 找到了，直接返回
            }
        }
        return -1.0; // 所有邻居都走不通
    }
};

// ============================================================
// 解法3: Floyd 全源最短路预处理
// 时间: O(V³ + Q)  空间: O(V²)
//
// 【思路】
// Floyd 通常用于"全源最短路径"，公式: dist[i][j] = min(dist[i][k] + dist[k][j])
// 本题是乘法版本: dist[i][j] = dist[i][k] * dist[k][j]
//
// 意义: i/j = (i/k) × (k/j)，即通过中间变量 k 传递除法关系。
//
// 预处理后，每次查询 O(1) 直接查表。
// 数据量小（变量 ≤ 40）时非常实用。
//
// Floyd 填表方向示意 (a/b=2, b/c=3):
//
//   初始:          k=b 后(关键步):
//       a    b    c        a    b    c
//  a [ 1.0  2.0   - ]  [ 1.0  2.0  6.0 ]   ← a/c = a/b × b/c = 2×3
//  b [ 0.5  1.0  3.0 ]  [ 0.5  1.0  3.0 ]
//  c [  -   1/3  1.0 ]  [ 1/6  1/3  1.0 ]   ← c/a = c/b × b/a = 1/3×0.5
// ============================================================
class Solution3 {
public:
    vector<double> calcEquation(vector<vector<string>>& equations,
                                vector<double>& values,
                                vector<vector<string>>& queries) {
        // 变量编号
        unordered_map<string, int> id;
        int idx = 0;
        for (auto& eq : equations) {
            if (!id.count(eq[0])) id[eq[0]] = idx++;
            if (!id.count(eq[1])) id[eq[1]] = idx++;
        }
        int n = idx;

        // 初始化: -1.0 表示不可达
        vector<vector<double>> dist(n, vector<double>(n, -1.0));
        for (int i = 0; i < n; i++) dist[i][i] = 1.0;

        for (int i = 0; i < (int)equations.size(); i++) {
            int u = id[equations[i][0]], v = id[equations[i][1]];
            dist[u][v] = values[i];
            dist[v][u] = 1.0 / values[i];
        }

        // Floyd: 三层循环，k 在最外层
        for (int k = 0; k < n; k++) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    // 只有 i→k 和 k→j 都可达时才能传递
                    if (dist[i][k] > 0 && dist[k][j] > 0) {
                        dist[i][j] = dist[i][k] * dist[k][j];
                    }
                }
            }
        }

        // 回答查询
        vector<double> results;
        for (auto& q : queries) {
            if (!id.count(q[0]) || !id.count(q[1])) {
                results.push_back(-1.0);
            } else {
                results.push_back(dist[id[q[0]]][id[q[1]]]);
            }
        }
        return results;
    }
};

// ============================================================
// 解法4: 带权并查集
// 时间: O((E+Q) × α(V))  空间: O(V)
//
// 【思路】
// 并查集天然处理连通性。在标准并查集基础上，给每个节点加权重:
//   weight[x] = x / root(x)
//
// 这样同一连通分量内任意两个变量 c, d:
//   c/d = (c/root) / (d/root) = weight[c] / weight[d]
//
// 关键难点在路径压缩和合并时的权重维护:
//
// 【路径压缩 find(x)】
//   原来: x → parent[x] → ... → root
//   压缩后: x → root
//   权重更新: weight[x] = x/parent × parent/grandparent × ... × .../root
//            = weight[x] × weight[parent[x]] × ...
//   实现: 递归 find，先让 parent 压缩到根(此时 weight[parent] 已更新)，
//         再 weight[x] *= weight[parent[x]]
//
// 【合并 unite(a, b, val)】 其中 a/b = val
//   需要: weight[rootA] = rootA / rootB
//   推导: a/b = val
//         (a/rootA) / (b/rootB) × (rootA/rootB)^(-1)... 不直觉
//   更清晰: rootA/rootB = (a/rootA)^(-1) × (a/b) × (b/rootB)
//                        = val × weight[b] / weight[a]
//
//   图示:
//     合并前:                合并后:
//     rootA    rootB         rootB
//       |        |          /     \
//      ...      ...      rootA   ...
//       |        |         |       |
//       a        b        ...     ...
//                          |
//                          a
// ============================================================
class Solution4 {
public:
    unordered_map<string, string> parent;
    unordered_map<string, double> weight; // weight[x] = x / root(x)

    string find(const string& x) {
        if (parent[x] != x) {
            // 先递归压缩父节点（此后 weight[parent[x]] = parent[x] / root）
            string root = find(parent[x]);
            // 再更新自己的权重: x/root = (x/parent) × (parent/root)
            weight[x] *= weight[parent[x]];
            parent[x] = root;
        }
        return parent[x];
    }

    void unite(const string& a, const string& b, double val) {
        // 确保节点存在
        if (!parent.count(a)) { parent[a] = a; weight[a] = 1.0; }
        if (!parent.count(b)) { parent[b] = b; weight[b] = 1.0; }

        string rootA = find(a), rootB = find(b);
        if (rootA == rootB) return; // 已连通

        // rootA 挂到 rootB 下面
        // weight[rootA] = rootA/rootB = val × weight[b] / weight[a]
        // 推导: a/b = val → rootA/rootB = (1/weight[a]) × val × weight[b]
        parent[rootA] = rootB;
        weight[rootA] = val * weight[b] / weight[a];
    }

    vector<double> calcEquation(vector<vector<string>>& equations,
                                vector<double>& values,
                                vector<vector<string>>& queries) {
        // 建立并查集
        for (int i = 0; i < (int)equations.size(); i++) {
            unite(equations[i][0], equations[i][1], values[i]);
        }

        vector<double> results;
        for (auto& q : queries) {
            const string& c = q[0], & d = q[1];
            if (!parent.count(c) || !parent.count(d)) {
                results.push_back(-1.0); // 变量不存在
            } else if (find(c) != find(d)) {
                results.push_back(-1.0); // 不在同一连通分量
            } else {
                // c/d = (c/root) / (d/root) = weight[c] / weight[d]
                results.push_back(weight[c] / weight[d]);
            }
        }
        return results;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法     | 建模方式        | 每次查询  | 预处理    | 代码难度 |
// |----------|----------------|----------|----------|---------|
// | BFS/DFS  | 邻接表+搜索     | O(V+E)   | O(E)     | ⭐      |
// | Floyd    | 邻接矩阵+全预处理| O(1)     | O(V³)    | ⭐⭐    |
// | 带权并查集| 树+路径压缩     | ~O(1)    | O(E×α)  | ⭐⭐⭐  |
//
// 面试推荐: BFS 最优先，思路清晰好解释好写。
// 追问优化: 提 Floyd（预处理）或 带权并查集（动态添加等式）。
//
// 【易错点】
// 1. 漏建反向边: a/b=2 必须同时建 b→a 权重 0.5，否则 b/a 查不到
// 2. a/a 处理: 必须先检查 a 在图中才返回 1.0，x/x 中 x 未定义返回 -1.0
// 3. BFS 忘记 visited: 双向边会导致 a→b→a→b 无限循环
// 4. 并查集路径压缩顺序: 必须先 find(parent[x]) 再 weight[x] *= weight[parent[x]]
//    ✗ weight[x] *= weight[parent[x]]; find(parent[x]);  // parent 还没压缩
//    ✓ string root = find(parent[x]); weight[x] *= weight[parent[x]];
// 5. Floyd 不可达判断: dist[i][k] > 0 而不是 >= 0 (因为题目保证 values > 0)
//
// 【面试追问】
// Q1: 这道题的本质是什么？
//     → 带权有向图的路径查询。a/b=k 就是边权为 k 的边。
//
// Q2: 如果查询量极大（百万级），怎么优化？
//     → Floyd 预处理 O(V³)，之后每次 O(1)。或带权并查集 近 O(1)。
//
// Q3: 如果等式是动态添加的（查询和添加交替进行），怎么处理？
//     → 带权并查集最适合：unite() 添加等式，find() + 查表 回答查询。
//        Floyd 每次新增都要重跑，BFS 可以但每次查询慢。
//
// Q4: 带权并查集的 find 中，为什么不能先更新权重再递归？
//     → 因为 weight[parent[x]] 此时还是"parent到其parent"的权重，
//        不是"parent到root"的权重。必须先递归让 parent 压缩到根，
//        weight[parent[x]] 才变成正确的 parent/root 值。
// ============================================================
