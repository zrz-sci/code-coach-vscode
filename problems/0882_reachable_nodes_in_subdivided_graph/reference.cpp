/*
 * LeetCode 882: Reachable Nodes In Subdivided Graph
 *
 * 解法：Dijkstra 最短路 + 边遍历统计细分节点
 *
 * 思路：
 * 1. 将每条边的权重设为 cnt+1（经过 cnt 个细分节点到达对端）
 * 2. Dijkstra 求从节点 0 到所有原始节点的最短距离
 * 3. 统计可达的原始节点（dist <= maxMoves）
 * 4. 对每条边，从两端分别计算可覆盖的细分节点数
 *
 * 时间复杂度：O(E log N)
 * 空间复杂度：O(N + E)
 */

class Solution {
public:
    int reachableNodes(vector<vector<int>>& edges, int maxMoves, int n) {
        // 第一步：建邻接表
        // adj[u] = {(v, cnt), ...}，表示 u 和 v 之间有 cnt 个细分节点
        vector<vector<pair<int, int>>> adj(n);
        for (auto& e : edges) {
            int u = e[0], v = e[1], cnt = e[2];
            adj[u].push_back({v, cnt});
            adj[v].push_back({u, cnt});
        }

        // 第二步：Dijkstra 求最短路
        // dist[i] 表示从节点 0 到节点 i 的最短距离
        vector<int> dist(n, INT_MAX);
        dist[0] = 0;

        // 最小堆：(距离, 节点编号)
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
        pq.push({0, 0});

        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();

            // 如果当前距离已经不是最优，跳过
            if (d > dist[u]) continue;

            // 松弛所有邻居
            for (auto& [v, cnt] : adj[u]) {
                // 边权 = cnt + 1（经过 cnt 个细分节点 + 到达 v 本身）
                int newDist = d + cnt + 1;
                if (newDist < dist[v]) {
                    dist[v] = newDist;
                    pq.push({newDist, v});
                }
            }
        }

        // 第三步：统计可达的原始节点
        int result = 0;
        for (int i = 0; i < n; i++) {
            if (dist[i] <= maxMoves) {
                result++;
            }
        }

        // 第四步：统计每条边上可达的细分节点
        for (auto& e : edges) {
            int u = e[0], v = e[1], cnt = e[2];

            // 从 u 端可以延伸多少个细分节点
            int fromU = 0;
            if (dist[u] <= maxMoves) {
                fromU = maxMoves - dist[u];
            }

            // 从 v 端可以延伸多少个细分节点
            int fromV = 0;
            if (dist[v] <= maxMoves) {
                fromV = maxMoves - dist[v];
            }

            // 该边上被覆盖的细分节点数
            // 取 min 防止两端重复覆盖同一个细分节点
            result += min(cnt, fromU + fromV);
        }

        return result;
    }
};

/*
 * 详细示例分析：
 *
 * edges = [[0,1,10],[0,2,1],[1,2,2]], maxMoves = 6, n = 3
 *
 * 建图（邻接表，权重为 cnt+1）：
 *   0 -> (1, 10), (2, 1)    边权：11, 2
 *   1 -> (0, 10), (2, 2)    边权：11, 3
 *   2 -> (0, 1), (1, 2)     边权：2, 3
 *
 * Dijkstra:
 *   初始：dist = [0, INF, INF]
 *   处理节点 0 (dist=0)：
 *     到节点 1：dist = 0 + 11 = 11
 *     到节点 2：dist = 0 + 2 = 2
 *   dist = [0, 11, 2]
 *
 *   处理节点 2 (dist=2)：
 *     到节点 0：dist = 2 + 2 = 4 > 0，不更新
 *     到节点 1：dist = 2 + 3 = 5 < 11，更新
 *   dist = [0, 5, 2]
 *
 *   处理节点 1 (dist=5)：无更新
 *
 *   最终 dist = [0, 5, 2]
 *
 * 可达原始节点：0(dist=0), 1(dist=5), 2(dist=2) 全部 <= 6
 *   原始节点数 = 3
 *
 * 细分节点统计：
 *   边 (0,1,10)：fromU = 6-0=6, fromV = 6-5=1, min(10, 6+1) = 7
 *   边 (0,2,1)：fromU = 6-0=6, fromV = 6-2=4, min(1, 6+4) = 1
 *   边 (1,2,2)：fromU = 6-5=1, fromV = 6-2=4, min(2, 1+4) = 2
 *   细分节点总数 = 7 + 1 + 2 = 10
 *
 * 总计 = 3 + 10 = 13 (correct!)
 */

/*
 * 补充说明：为什么不能在细分后的图上直接 BFS/Dijkstra？
 *
 * 细分后的节点数可能非常大：
 * - 最多 10^4 条边，每条边最多 10^4 个细分节点
 * - 总节点数可达 10^8，远超可处理范围
 *
 * 而在原始图上运行 Dijkstra（最多 3000 个节点），
 * 然后通过数学计算推断细分节点的可达性，效率高得多。
 */

/*
 * 易错变体：如果是有向图
 *
 * 如果边是有向的，从 u 到 v 的细分节点只能从 u 端覆盖，
 * 不能从 v 端反向覆盖。需要分别记录每个方向的覆盖量。
 * 本题是无向图，所以两端都可以覆盖。
 */
