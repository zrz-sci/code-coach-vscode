// LeetCode 685: Redundant Connection II
// 解法: 并查集 + 入度分析（三种情况统一处理）
// 时间: O(n * alpha(n)) ≈ O(n)
// 空间: O(n)

// ============================================================
// 核心思路:
// 有根树多了一条边，可能导致:
//   Case A: 某节点入度为2，无环 → 删后出现的候选边
//   Case B: 某节点入度为2，有环 → 删在环上的那条候选边
//   Case C: 无入度为2的节点，有环 → 并查集找成环边
//
// 策略: 先扫描入度找候选边，再用并查集验证
// ============================================================

class Solution {
public:
    vector<int> parent, rnk;

    int find(int x) {
        // 路径压缩：将 x 到根路径上所有节点直接挂到根
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    bool unite(int x, int y) {
        // 按秩合并：矮树挂到高树上
        int px = find(x), py = find(y);
        if (px == py) return false; // 已连通 → 成环
        if (rnk[px] < rnk[py]) swap(px, py);
        parent[py] = px;
        if (rnk[px] == rnk[py]) rnk[px]++;
        return true;
    }

    vector<int> findRedundantDirectedConnection(vector<vector<int>>& edges) {
        int n = edges.size();

        // ===== 第一阶段: 扫描入度，寻找入度为 2 的节点 =====
        // inEdge[v] 记录第一条指向 v 的边的索引
        vector<int> inEdge(n + 1, -1);
        int cand1 = -1, cand2 = -1; // 候选边索引

        for (int i = 0; i < n; i++) {
            int v = edges[i][1]; // 边的终点
            if (inEdge[v] != -1) {
                // 节点 v 已有一条入边，现在是第二条
                cand1 = inEdge[v]; // 先出现的边
                cand2 = i;          // 后出现的边
                break;
            }
            inEdge[v] = i;
        }

        // ===== 第二阶段: 并查集检测环 =====
        parent.resize(n + 1);
        rnk.assign(n + 1, 0);
        for (int i = 1; i <= n; i++) parent[i] = i;

        if (cand2 != -1) {
            // ---- 存在入度为 2 的节点 ----
            // 策略: 先试删 cand2（后出现的），看剩余边能否构成合法树
            for (int i = 0; i < n; i++) {
                if (i == cand2) continue; // 跳过 cand2
                if (!unite(edges[i][0], edges[i][1])) {
                    // 跳过 cand2 后仍有环 → cand1 在环上，必须删 cand1
                    return edges[cand1];
                }
            }
            // 跳过 cand2 后无环 → cand2 是多余的边
            return edges[cand2];
        } else {
            // ---- 不存在入度为 2 的节点，纯环问题 ----
            // 等价于 LC 684，找到成环的最后一条边
            for (int i = 0; i < n; i++) {
                if (!unite(edges[i][0], edges[i][1])) {
                    return edges[i];
                }
            }
        }

        return {}; // 理论上不会到达
    }
};

// ============================================================
// 解法二: 拓扑排序方法（更直观但代码更长）
// 时间: O(n), 空间: O(n)
//
// 思路:
// 1. 找入度为 2 的节点（如果有），记录两条候选边
// 2. 对图做拓扑排序（反向BFS从叶子往根缩）
// 3. 拓扑排序后剩余在环上的边就是需要分析的
// ============================================================

class Solution2 {
public:
    vector<int> findRedundantDirectedConnection(vector<vector<int>>& edges) {
        int n = edges.size();

        // 建图并统计入度
        vector<int> indeg(n + 1, 0);
        vector<vector<pair<int,int>>> graph(n + 1); // graph[u] = {(v, edgeIdx)}
        vector<int> parentOf(n + 1, -1); // parentOf[v] = 指向 v 的边的索引

        int cand1 = -1, cand2 = -1;
        for (int i = 0; i < n; i++) {
            int u = edges[i][0], v = edges[i][1];
            indeg[v]++;
            graph[u].push_back({v, i});
            if (parentOf[v] != -1) {
                cand1 = parentOf[v];
                cand2 = i;
            }
            parentOf[v] = i;
        }

        // 如果没有入度为 2 的节点 → 找环上最后出现的边
        if (cand2 == -1) {
            // 类似 LC 684 的并查集做法
            vector<int> par(n + 1), rk(n + 1, 0);
            iota(par.begin(), par.end(), 0);

            function<int(int)> findP = [&](int x) -> int {
                return par[x] == x ? x : par[x] = findP(par[x]);
            };

            for (int i = 0; i < n; i++) {
                int pu = findP(edges[i][0]), pv = findP(edges[i][1]);
                if (pu == pv) return edges[i];
                if (rk[pu] < rk[pv]) swap(pu, pv);
                par[pv] = pu;
                if (rk[pu] == rk[pv]) rk[pu]++;
            }
        }

        // 有入度为 2 的节点 → 试删 cand2，用并查集验证
        vector<int> par(n + 1), rk(n + 1, 0);
        iota(par.begin(), par.end(), 0);

        function<int(int)> findP = [&](int x) -> int {
            return par[x] == x ? x : par[x] = findP(par[x]);
        };

        for (int i = 0; i < n; i++) {
            if (i == cand2) continue;
            int pu = findP(edges[i][0]), pv = findP(edges[i][1]);
            if (pu == pv) return edges[cand1];
            if (rk[pu] < rk[pv]) swap(pu, pv);
            par[pv] = pu;
            if (rk[pu] == rk[pv]) rk[pu]++;
        }

        return edges[cand2];
    }
};
