/*
 * 【题目本质】
 * 找无根树的"中心节点"→ 以中心为根时树高最小
 * 树的中心 = 直径的中点，最多 1~2 个
 *
 * 【解法总览】
 * Solution1: 拓扑排序（剥叶子） ⭐⭐⭐⭐⭐  O(n) / O(n) — 从外向内收缩
 * Solution2: 两次 BFS 找直径     ⭐⭐⭐⭐    O(n) / O(n) — 找直径取中点
 */

// ===================== Solution1: 拓扑排序 — 逐层剥叶子（推荐） =====================
// 思路：反复去掉度为 1 的叶子节点，最后剩下 1~2 个就是答案
class Solution1 {
public:
    vector<int> findMinHeightTrees(int n, vector<vector<int>>& edges) {
        if (n == 1) return {0}; // 特判：单节点

        // 建邻接表 + 统计度
        vector<vector<int>> adj(n);
        vector<int> degree(n, 0);
        for (auto& e : edges) {
            adj[e[0]].push_back(e[1]);
            adj[e[1]].push_back(e[0]);
            degree[e[0]]++;
            degree[e[1]]++;
        }

        // 所有叶子（度 == 1）入队
        queue<int> q;
        for (int i = 0; i < n; i++)
            if (degree[i] == 1) q.push(i);

        int remaining = n;
        // 逐层剥离叶子，直到剩余 <= 2
        while (remaining > 2) {
            int sz = q.size();
            remaining -= sz;
            for (int i = 0; i < sz; i++) {
                int leaf = q.front(); q.pop();
                for (int nei : adj[leaf]) {
                    degree[nei]--;
                    if (degree[nei] == 1) // 变成新叶子
                        q.push(nei);
                }
            }
        }

        // 剩余节点即为答案
        vector<int> result;
        while (!q.empty()) {
            result.push_back(q.front());
            q.pop();
        }
        return result;
    }
};

// ===================== Solution2: 两次 BFS 找直径中点 =====================
// 思路：第一次 BFS 从任意节点找最远点 u，第二次 BFS 从 u 找最远点 v
// u-v 路径即为直径，取中间 1~2 个节点
class Solution2 {
public:
    vector<int> findMinHeightTrees(int n, vector<vector<int>>& edges) {
        if (n == 1) return {0};

        vector<vector<int>> adj(n);
        for (auto& e : edges) {
            adj[e[0]].push_back(e[1]);
            adj[e[1]].push_back(e[0]);
        }

        // 第一次 BFS：从 0 出发找最远端点 u
        auto [u, p1] = bfs(adj, 0, n);
        // 第二次 BFS：从 u 出发找最远端点 v，记录 parent
        auto [v, parent] = bfs(adj, u, n);

        // 回溯 parent 获得直径路径
        vector<int> path;
        for (int cur = v; cur != -1; cur = parent[cur])
            path.push_back(cur);

        // 取中间节点
        int len = path.size();
        if (len % 2 == 1)
            return {path[len / 2]};
        else
            return {path[len / 2 - 1], path[len / 2]};
    }

private:
    // 返回 {最远节点, parent 数组}
    pair<int, vector<int>> bfs(vector<vector<int>>& adj, int start, int n) {
        vector<int> dist(n, -1), parent(n, -1);
        queue<int> q;
        q.push(start);
        dist[start] = 0;
        int farthest = start;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) {
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    parent[v] = u;
                    q.push(v);
                    if (dist[v] > dist[farthest]) farthest = v;
                }
            }
        }
        return {farthest, parent};
    }
};

/*
 * 【解法对比】
 * ┌──────────────────────┬──────────┬──────────┬──────────────────────────┐
 * │ 解法                 │ 时间     │ 空间     │ 特点                     │
 * ├──────────────────────┼──────────┼──────────┼──────────────────────────┤
 * │ Solution1 剥叶子     │ O(n)     │ O(n)     │ 最直观，面试首选         │
 * │ Solution2 直径中点   │ O(n)     │ O(n)     │ 数学直觉，需要两次 BFS   │
 * └──────────────────────┴──────────┴──────────┴──────────────────────────┘
 *
 * 【易错点】
 * 1. 特判 n==1：没有边，没有叶子，直接返回 {0}
 * 2. 终止条件是 remaining > 2，不是 > 1（答案可能有 2 个）
 * 3. 无向图必须双向建边
 * 4. 剥叶子时要按层处理（类似 BFS 层序遍历），不能逐个处理
 * 5. 两次 BFS 方法中，parent 数组的起始值是 -1（表示根没有父节点）
 *
 * 【面试追问】
 * Q1: 为什么答案最多 2 个？→ 直径中点：奇数长度 1 个，偶数长度 2 个
 * Q2: 两种方法的关系？→ 等价：剥叶子 = 从外向内找中心 = 找直径中点
 * Q3: 有权图怎么办？→ 两次 BFS 找带权直径，在路径上二分/遍历找中间位置
 */
