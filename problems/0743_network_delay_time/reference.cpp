// LeetCode 743. Network Delay Time
// 解法一：Dijkstra + 最小堆（推荐）
// Time: O(E log V), Space: O(V + E)

class Solution {
public:
    int networkDelayTime(vector<vector<int>>& times, int n, int k) {
        // 邻接表建图：graph[u] = [(v, w), ...]
        vector<vector<pair<int, int>>> graph(n + 1);
        for (auto& t : times) {
            graph[t[0]].push_back({t[1], t[2]});
        }

        // dist[i] = 从 k 到 i 的最短距离
        vector<int> dist(n + 1, INT_MAX);
        dist[k] = 0;

        // 最小堆：(距离, 节点)
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
        pq.push({0, k});

        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();

            // 惰性删除：跳过过时的堆条目
            if (d > dist[u]) continue;

            // 松弛所有出边
            for (auto& [v, w] : graph[u]) {
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    pq.push({dist[v], v});
                }
            }
        }

        // 收集答案：所有节点最短距离的最大值
        int ans = 0;
        for (int i = 1; i <= n; i++) {
            if (dist[i] == INT_MAX) return -1; // 不可达
            ans = max(ans, dist[i]);
        }

        return ans;
    }
};


// 解法二：Bellman-Ford
// Time: O(V * E), Space: O(V)
// 可处理负权边（本题不需要但作为知识点展示）

class Solution2 {
public:
    int networkDelayTime(vector<vector<int>>& times, int n, int k) {
        // dist[i] = 从 k 到 i 的最短距离
        vector<int> dist(n + 1, INT_MAX);
        dist[k] = 0;

        // 执行 n-1 轮松弛
        for (int round = 0; round < n - 1; round++) {
            bool updated = false;
            for (auto& edge : times) {
                int u = edge[0], v = edge[1], w = edge[2];
                // 注意防止 INT_MAX 溢出
                if (dist[u] != INT_MAX && dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    updated = true;
                }
            }
            // 提前退出优化：没有更新则已收敛
            if (!updated) break;
        }

        // 收集答案
        int ans = 0;
        for (int i = 1; i <= n; i++) {
            if (dist[i] == INT_MAX) return -1;
            ans = max(ans, dist[i]);
        }

        return ans;
    }
};


// 解法三：Floyd-Warshall（全源最短路径）
// Time: O(V^3), Space: O(V^2)
// 本题大材小用，但展示作为面试知识点

class Solution3 {
public:
    int networkDelayTime(vector<vector<int>>& times, int n, int k) {
        // 距离矩阵初始化
        const int INF = 1e9;
        vector<vector<int>> dist(n + 1, vector<int>(n + 1, INF));

        // 自身到自身距离为 0
        for (int i = 1; i <= n; i++) dist[i][i] = 0;

        // 填入直接边权
        for (auto& t : times) {
            dist[t[0]][t[1]] = t[2];
        }

        // Floyd-Warshall 三层循环
        for (int mid = 1; mid <= n; mid++) {
            for (int i = 1; i <= n; i++) {
                for (int j = 1; j <= n; j++) {
                    if (dist[i][mid] != INF && dist[mid][j] != INF) {
                        dist[i][j] = min(dist[i][j],
                                         dist[i][mid] + dist[mid][j]);
                    }
                }
            }
        }

        // 从 k 出发到所有节点的最大距离
        int ans = 0;
        for (int i = 1; i <= n; i++) {
            if (dist[k][i] == INF) return -1;
            ans = max(ans, dist[k][i]);
        }

        return ans;
    }
};


// 解法四：SPFA（Bellman-Ford 的队列优化）
// Time: O(V * E) 最坏, Space: O(V + E)
// 平均情况下比 Bellman-Ford 快很多

class Solution4 {
public:
    int networkDelayTime(vector<vector<int>>& times, int n, int k) {
        // 邻接表
        vector<vector<pair<int, int>>> graph(n + 1);
        for (auto& t : times) {
            graph[t[0]].push_back({t[1], t[2]});
        }

        vector<int> dist(n + 1, INT_MAX);
        vector<bool> inQueue(n + 1, false);
        dist[k] = 0;

        queue<int> q;
        q.push(k);
        inQueue[k] = true;

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            inQueue[u] = false;

            for (auto& [v, w] : graph[u]) {
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    if (!inQueue[v]) {
                        q.push(v);
                        inQueue[v] = true;
                    }
                }
            }
        }

        int ans = 0;
        for (int i = 1; i <= n; i++) {
            if (dist[i] == INT_MAX) return -1;
            ans = max(ans, dist[i]);
        }

        return ans;
    }
};
