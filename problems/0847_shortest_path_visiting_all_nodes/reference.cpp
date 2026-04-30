// LeetCode 847: Shortest Path Visiting All Nodes
// 解法一：状态压缩 BFS（推荐）
// 时间 O(n * 2^n)，空间 O(n * 2^n)
// 多源 BFS，状态为 (当前节点, 已访问 bitmask)

class Solution {
public:
    int shortestPathLength(vector<vector<int>>& graph) {
        int n = graph.size();
        if (n == 1) return 0;

        int fullMask = (1 << n) - 1;

        // visited[node][mask] = 是否已访问过此状态
        vector<vector<bool>> visited(n, vector<bool>(1 << n, false));

        // BFS 队列: {node, mask, distance}
        queue<tuple<int, int, int>> q;

        // 多源初始化：每个节点都可以是起点
        for (int i = 0; i < n; ++i) {
            int mask = 1 << i;
            q.push({i, mask, 0});
            visited[i][mask] = true;
        }

        while (!q.empty()) {
            auto [node, mask, dist] = q.front();
            q.pop();

            // 遍历邻居
            for (int next : graph[node]) {
                int newMask = mask | (1 << next);

                // 检查是否所有节点都已访问
                if (newMask == fullMask) return dist + 1;

                // 去重
                if (!visited[next][newMask]) {
                    visited[next][newMask] = true;
                    q.push({next, newMask, dist + 1});
                }
            }
        }
        return -1; // 不应到达这里（题目保证图连通）
    }
};

// ============================================================
// 解法二：Floyd 预处理 + 状态压缩 DP（经典 TSP）
// 时间 O(n^3 + n^2 * 2^n)，空间 O(n * 2^n)
// 先求所有节点对最短距离，再用 TSP DP

class Solution2 {
public:
    int shortestPathLength(vector<vector<int>>& graph) {
        int n = graph.size();
        if (n == 1) return 0;

        // 步骤 1：BFS 求所有节点对之间的最短距离
        vector<vector<int>> dist(n, vector<int>(n, INT_MAX));
        for (int i = 0; i < n; ++i) {
            dist[i][i] = 0;
            queue<int> bfsQ;
            bfsQ.push(i);
            while (!bfsQ.empty()) {
                int u = bfsQ.front(); bfsQ.pop();
                for (int v : graph[u]) {
                    if (dist[i][v] == INT_MAX) {
                        dist[i][v] = dist[i][u] + 1;
                        bfsQ.push(v);
                    }
                }
            }
        }

        // 步骤 2：TSP DP
        // dp[mask][i] = 访问了 mask 中的节点，最后在节点 i 的最短路径
        int fullMask = (1 << n) - 1;
        vector<vector<int>> dp(1 << n, vector<int>(n, INT_MAX));

        // 初始化：只访问了一个节点
        for (int i = 0; i < n; ++i) {
            dp[1 << i][i] = 0;
        }

        // 枚举所有 mask
        for (int mask = 1; mask <= fullMask; ++mask) {
            for (int last = 0; last < n; ++last) {
                if (dp[mask][last] == INT_MAX) continue;
                if (!(mask & (1 << last))) continue;

                // 尝试添加一个新节点
                for (int next = 0; next < n; ++next) {
                    int newMask = mask | (1 << next);
                    if (dist[last][next] == INT_MAX) continue;
                    int newDist = dp[mask][last] + dist[last][next];
                    dp[newMask][next] = min(dp[newMask][next], newDist);
                }
            }
        }

        // 答案：fullMask 对应的所有终点中的最小值
        int ans = INT_MAX;
        for (int i = 0; i < n; ++i) {
            ans = min(ans, dp[fullMask][i]);
        }
        return ans;
    }
};

// ============================================================
// 解法三：BFS 优化版（用数组代替 tuple 提升性能）
// 时间 O(n * 2^n)，空间 O(n * 2^n)
// 将距离信息存入 visited 数组避免队列存储开销

class Solution3 {
public:
    int shortestPathLength(vector<vector<int>>& graph) {
        int n = graph.size();
        if (n == 1) return 0;

        int fullMask = (1 << n) - 1;
        // dist[node][mask] = 到达状态 (node, mask) 的最短距离
        vector<vector<int>> distArr(n, vector<int>(1 << n, -1));

        queue<pair<int, int>> q;
        for (int i = 0; i < n; ++i) {
            distArr[i][1 << i] = 0;
            q.push({i, 1 << i});
        }

        while (!q.empty()) {
            auto [node, mask] = q.front();
            q.pop();
            int d = distArr[node][mask];

            for (int next : graph[node]) {
                int newMask = mask | (1 << next);
                if (newMask == fullMask) return d + 1;

                if (distArr[next][newMask] == -1) {
                    distArr[next][newMask] = d + 1;
                    q.push({next, newMask});
                }
            }
        }
        return -1;
    }
};
