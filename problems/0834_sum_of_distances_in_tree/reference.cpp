// LeetCode 834: Sum of Distances in Tree
// 解法一：换根DP（两次DFS）- 最优解
// 时间复杂度: O(n), 空间复杂度: O(n)

class Solution {
public:
    vector<int> sumOfDistancesInTree(int n, vector<vector<int>>& edges) {
        // 建立邻接表
        vector<vector<int>> graph(n);
        for (auto& e : edges) {
            graph[e[0]].push_back(e[1]);
            graph[e[1]].push_back(e[0]);
        }

        vector<int> count(n, 1); // count[i] = 以i为根的子树节点数
        vector<int> ans(n, 0);   // ans[i] = 节点i到所有其他节点的距离和

        // 第一次DFS：自底向上，计算count和ans[0]
        // 以节点0为根，计算每个子树的大小
        // ans[0] 在这个过程中通过累加子树贡献来计算
        function<void(int, int)> dfs1 = [&](int node, int parent) {
            for (int child : graph[node]) {
                if (child == parent) continue;
                dfs1(child, node);
                count[node] += count[child];
                // child子树中所有节点到node的距离 = 它们到child的距离 + count[child]
                // 因为每个节点需要额外走一步从child到node
                ans[0] += count[child];
            }
        };

        // 第二次DFS：自顶向下，利用换根公式推导所有节点的ans
        // 换根公式: ans[child] = ans[parent] - count[child] + (n - count[child])
        //         = ans[parent] + n - 2 * count[child]
        // 解释：
        //   当根从parent移到child时：
        //   - child子树内的count[child]个节点，每个距离减少1 → 总距离减少count[child]
        //   - child子树外的(n-count[child])个节点，每个距离增加1 → 总距离增加(n-count[child])
        function<void(int, int)> dfs2 = [&](int node, int parent) {
            for (int child : graph[node]) {
                if (child == parent) continue;
                ans[child] = ans[node] + n - 2 * count[child];
                dfs2(child, node);
            }
        };

        dfs1(0, -1);
        dfs2(0, -1);

        return ans;
    }
};

// ================================================================
// 解法二：暴力BFS（仅用于理解，面试不推荐）
// 时间复杂度: O(n^2), 空间复杂度: O(n)

class Solution_BFS {
public:
    vector<int> sumOfDistancesInTree(int n, vector<vector<int>>& edges) {
        vector<vector<int>> graph(n);
        for (auto& e : edges) {
            graph[e[0]].push_back(e[1]);
            graph[e[1]].push_back(e[0]);
        }

        vector<int> ans(n, 0);

        // 对每个节点做一次BFS
        for (int i = 0; i < n; i++) {
            vector<bool> visited(n, false);
            queue<int> q;
            q.push(i);
            visited[i] = true;
            int dist = 0;

            while (!q.empty()) {
                int sz = q.size();
                for (int j = 0; j < sz; j++) {
                    int node = q.front(); q.pop();
                    ans[i] += dist;
                    for (int neighbor : graph[node]) {
                        if (!visited[neighbor]) {
                            visited[neighbor] = true;
                            q.push(neighbor);
                        }
                    }
                }
                dist++;
            }
        }

        return ans;
    }
};

// ================================================================
// 解法三：迭代版换根DP（避免栈溢出）
// 适用于n非常大的情况

class Solution_Iterative {
public:
    vector<int> sumOfDistancesInTree(int n, vector<vector<int>>& edges) {
        vector<vector<int>> graph(n);
        for (auto& e : edges) {
            graph[e[0]].push_back(e[1]);
            graph[e[1]].push_back(e[0]);
        }

        vector<int> count(n, 1);
        vector<int> ans(n, 0);
        vector<int> parent(n, -1);
        vector<int> order; // BFS序

        // BFS建立拓扑序
        vector<bool> visited(n, false);
        queue<int> q;
        q.push(0);
        visited[0] = true;
        while (!q.empty()) {
            int node = q.front(); q.pop();
            order.push_back(node);
            for (int child : graph[node]) {
                if (!visited[child]) {
                    visited[child] = true;
                    parent[child] = node;
                    q.push(child);
                }
            }
        }

        // 自底向上：逆序遍历BFS序
        for (int i = n - 1; i >= 1; i--) {
            int node = order[i];
            int par = parent[node];
            count[par] += count[node];
            ans[0] += count[node]; // 每个节点贡献其子树大小
        }

        // 自顶向下：正序遍历BFS序
        for (int i = 1; i < n; i++) {
            int node = order[i];
            int par = parent[node];
            ans[node] = ans[par] + n - 2 * count[node];
        }

        return ans;
    }
};
