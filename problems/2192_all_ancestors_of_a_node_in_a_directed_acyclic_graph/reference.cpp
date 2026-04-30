/*
 * LeetCode 2192: 有向无环图中一个节点的所有祖先
 *
 * 【题目】给定 DAG 的 n 个节点和边集，对每个节点返回其所有祖先（升序）。
 *
 * 【解法1】正向 BFS from each node — O(n*(n+e)) / O(n^2)
 * 对每个节点 u，BFS 遍历其所有后代，将 u 加入后代的祖先集合。
 *
 * 【解法2】拓扑排序 + 集合传递 — O(n*(n+e)) / O(n^2) ⭐ 首选
 * Kahn 拓扑排序，按拓扑序传递祖先集合：
 * ancestors[v] = Union(ancestors[p] ∪ {p}) for all p->v
 *
 * 【解法3】反向图 + BFS — O(n*(n+e)) / O(n^2+e)
 * 建反向图，对每个节点 v 在反图上 BFS 找到所有祖先。
 */

// ============================================================
// 解法1: 正向 BFS from each node
// ============================================================
class Solution {
public:
    vector<vector<int>> getAncestors(int n, vector<vector<int>>& edges) {
        // 建正向邻接表
        vector<vector<int>> graph(n);
        for (auto& e : edges) {
            graph[e[0]].push_back(e[1]);
        }

        // 每个节点用 set 存储祖先（自动排序去重）
        vector<set<int>> ancestorSets(n);

        // 从节点 u=0..n-1 出发，BFS 标记后代
        // u 按升序遍历，但 set 保证最终有序
        for (int u = 0; u < n; u++) {
            // 每轮 BFS 独立的 visited 数组
            vector<bool> visited(n, false);
            queue<int> q;
            q.push(u);
            visited[u] = true;

            while (!q.empty()) {
                int cur = q.front(); q.pop();
                for (int next : graph[cur]) {
                    if (!visited[next]) {
                        visited[next] = true;
                        // u 能到达 next → u 是 next 的祖先
                        ancestorSets[next].insert(u);
                        q.push(next);
                    }
                }
            }
        }

        // set → vector
        vector<vector<int>> result(n);
        for (int i = 0; i < n; i++) {
            result[i].assign(ancestorSets[i].begin(), ancestorSets[i].end());
        }
        return result;
    }
};

// ============================================================
// 解法2: 拓扑排序 + 集合传递 ⭐ 面试首选
// ============================================================
/*
 * 核心思想：DAG 上按拓扑序处理节点。
 * 处理节点 u 时，u 的祖先集合已经完整收集。
 * 对 u 的每条出边 u->v：
 *   ancestors[v] ∪= ancestors[u] ∪ {u}
 *
 * 示例: n=8
 *   拓扑序: 0→1→2→3→4→5→6→7
 *   处理 3 时: ancestors[3] = {0,1} 已完整
 *   传递给 5: ancestors[5] ∪= {0,1,3}
 *   传递给 6: ancestors[6] ∪= {0,1,3}
 *   传递给 7: ancestors[7] ∪= {0,1,3}
 */
class Solution2 {
public:
    vector<vector<int>> getAncestors(int n, vector<vector<int>>& edges) {
        vector<vector<int>> graph(n);
        vector<int> indegree(n, 0);

        for (auto& e : edges) {
            graph[e[0]].push_back(e[1]);
            indegree[e[1]]++;
        }

        // Kahn's BFS 拓扑排序
        queue<int> q;
        for (int i = 0; i < n; i++) {
            if (indegree[i] == 0) q.push(i);
        }

        vector<set<int>> ancestorSets(n);

        while (!q.empty()) {
            int u = q.front(); q.pop();

            for (int v : graph[u]) {
                // 传递祖先集合：v 的祖先包含 u 的所有祖先和 u 自身
                ancestorSets[v].insert(ancestorSets[u].begin(), ancestorSets[u].end());
                ancestorSets[v].insert(u);

                if (--indegree[v] == 0) {
                    q.push(v);
                }
            }
        }

        // set → vector
        vector<vector<int>> result(n);
        for (int i = 0; i < n; i++) {
            result[i].assign(ancestorSets[i].begin(), ancestorSets[i].end());
        }
        return result;
    }
};

// ============================================================
// 解法3: 反向图 + BFS
// ============================================================
/*
 * 思路: 建反向图，对每个节点 v 在反图上 BFS，
 * 所有能到达的节点就是 v 的祖先。
 *
 * 正图: 0→3 表示 0 是 3 的祖先
 * 反图: 3→0 表示从 3 出发能找到祖先 0
 */
class Solution3 {
public:
    vector<vector<int>> getAncestors(int n, vector<vector<int>>& edges) {
        // 建反向邻接表
        vector<vector<int>> reverseGraph(n);
        for (auto& e : edges) {
            reverseGraph[e[1]].push_back(e[0]); // 反转边方向
        }

        vector<vector<int>> result(n);

        for (int v = 0; v < n; v++) {
            // 在反图上从 v 出发 BFS
            set<int> ancestors;
            vector<bool> visited(n, false);
            queue<int> q;
            q.push(v);
            visited[v] = true;

            while (!q.empty()) {
                int cur = q.front(); q.pop();
                for (int prev : reverseGraph[cur]) {
                    if (!visited[prev]) {
                        visited[prev] = true;
                        ancestors.insert(prev); // prev 是 v 的祖先
                        q.push(prev);
                    }
                }
            }

            result[v].assign(ancestors.begin(), ancestors.end());
        }

        return result;
    }
};


// ============================================================
// 总结
// ============================================================
//
// 【解法对比】
// ┌─────────────────────┬────────────┬────────┬──────────────────────────────┐
// │ 解法                 │ 时间       │ 空间   │ 适用场景                     │
// ├─────────────────────┼────────────┼────────┼──────────────────────────────┤
// │ 1. 正向BFS from each │ O(n(n+e))  │ O(n^2) │ 直觉清晰, 无需拓扑排序      │
// │ 2. 拓扑排序+集合传递 │ O(n(n+e))  │ O(n^2) │ ⭐ 面试首选, 思路最自然      │
// │ 3. 反向图+BFS        │ O(n(n+e))  │ O(n^2) │ 反向思维, 代码独立性好       │
// └─────────────────────┴────────────┴────────┴──────────────────────────────┘
//
// 三种解法时间复杂度相同, 但实际常数因子有差异:
// - 解法2 利用拓扑序保证祖先集合完整后才传递, set 合并次数最少
// - 解法1/3 每个节点独立 BFS, 有大量重复遍历
//
// 【易错点】
// 1. 结果要求每个节点的祖先列表升序 → 用 set 自动排序或最后 sort
// 2. 解法1中 BFS 从 u 出发, u 是后代的祖先, 不要搞反方向
// 3. 拓扑排序传递时 ancestors[v] ∪= ancestors[u] ∪ {u},
//    不要漏掉 u 本身 (u 也是 v 的祖先)
// 4. 反向图建边方向: 原图 u→v 变成 v→u, 不要弄反
// 5. set 的 insert 是 O(logn), 合并两个 set 是 O(m*logn),
//    总复杂度中 set 操作贡献了 O(n^2 * logn) 的隐藏常数
//
// 【面试追问】
// Q1: 如果只需要查询某些特定节点的祖先 (而非所有节点)?
//     → 对查询节点在反向图上做 BFS/DFS 即可, 不需要处理全部节点.
//       或用 bitset 优化: ancestors[v] 用 bitset<N> 存储, 合并用 OR 操作 O(n/64).
//
// Q2: 如果图可能有环 (不是 DAG), 怎么处理?
//     → 先用 Tarjan/Kosaraju 求 SCC, 缩点成 DAG,
//       同一 SCC 内的节点互为祖先, 然后在缩点后的 DAG 上跑拓扑排序.
//
// Q3: 如果要求"最近公共祖先 (LCA)"而非"所有祖先"?
//     → DAG 上的 LCA 比树复杂: 一个节点可能有多个父节点.
//       可以用每个节点的祖先 bitset 做交集, 再取拓扑序最大的那个.
