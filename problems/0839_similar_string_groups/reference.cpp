// LeetCode 839: Similar String Groups
// 解法一：并查集（Union-Find）- 最优解
// 时间复杂度: O(N^2 * M), 空间复杂度: O(N)
// 其中 N = strs.size(), M = strs[0].size()

class Solution {
public:
    vector<int> parent;
    vector<int> rank_;

    int find(int x) {
        // 路径压缩: 将x直接连到根
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    void unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return;
        // 按秩合并: 矮树挂在高树下
        if (rank_[px] < rank_[py]) swap(px, py);
        parent[py] = px;
        if (rank_[px] == rank_[py]) rank_[px]++;
    }

    // 判断两个字符串是否"相似"
    // 相似定义: 相同 或 恰好有2个位置不同（交换即可变成一样）
    bool isSimilar(const string& a, const string& b) {
        int diff = 0;
        for (int i = 0; i < (int)a.size(); i++) {
            if (a[i] != b[i]) {
                diff++;
                if (diff > 2) return false; // 提前终止
            }
        }
        // diff == 0: 完全相同（也算相似）
        // diff == 2: 恰好可以交换两个位置
        return diff == 0 || diff == 2;
    }

    int numSimilarGroups(vector<string>& strs) {
        int n = strs.size();
        parent.resize(n);
        rank_.resize(n, 0);

        // 初始化并查集: 每个字符串自成一组
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }

        // 两两比较，相似则合并
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                if (isSimilar(strs[i], strs[j])) {
                    unite(i, j);
                }
            }
        }

        // 统计不同的根节点数 = 连通分量数
        int groups = 0;
        for (int i = 0; i < n; i++) {
            if (find(i) == i) {
                groups++;
            }
        }

        return groups;
    }
};

// ================================================================
// 解法二：DFS求连通分量
// 时间复杂度: O(N^2 * M), 空间复杂度: O(N^2)

class Solution_DFS {
public:
    bool isSimilar(const string& a, const string& b) {
        int diff = 0;
        for (int i = 0; i < (int)a.size(); i++) {
            if (a[i] != b[i]) {
                if (++diff > 2) return false;
            }
        }
        return diff == 0 || diff == 2;
    }

    int numSimilarGroups(vector<string>& strs) {
        int n = strs.size();

        // 建邻接表
        vector<vector<int>> adj(n);
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                if (isSimilar(strs[i], strs[j])) {
                    adj[i].push_back(j);
                    adj[j].push_back(i);
                }
            }
        }

        // DFS遍历求连通分量数
        vector<bool> visited(n, false);
        int groups = 0;

        for (int i = 0; i < n; i++) {
            if (visited[i]) continue;
            groups++;

            // DFS从i出发标记整个连通分量
            stack<int> stk;
            stk.push(i);
            visited[i] = true;

            while (!stk.empty()) {
                int node = stk.top(); stk.pop();
                for (int neighbor : adj[node]) {
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        stk.push(neighbor);
                    }
                }
            }
        }

        return groups;
    }
};

// ================================================================
// 解法三：优化版 - 当M远小于N时，枚举交换位置
// 时间复杂度: O(N * M^2 * alpha(N)), 空间复杂度: O(N * M)
// 当 N >> M 时比 O(N^2 * M) 更优

class Solution_SwapEnum {
public:
    vector<int> parent;
    vector<int> rank_;

    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }

    void unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return;
        if (rank_[px] < rank_[py]) swap(px, py);
        parent[py] = px;
        if (rank_[px] == rank_[py]) rank_[px]++;
    }

    int numSimilarGroups(vector<string>& strs) {
        int n = strs.size();
        int m = strs[0].size();
        parent.resize(n);
        rank_.resize(n, 0);
        for (int i = 0; i < n; i++) parent[i] = i;

        // 建立字符串到索引的映射
        unordered_map<string, vector<int>> strToIdx;
        for (int i = 0; i < n; i++) {
            strToIdx[strs[i]].push_back(i);
        }

        // 先合并完全相同的字符串
        for (auto& [s, indices] : strToIdx) {
            for (int k = 1; k < (int)indices.size(); k++) {
                unite(indices[0], indices[k]);
            }
        }

        // 对每个字符串，枚举所有可能的交换
        for (int i = 0; i < n; i++) {
            string s = strs[i];
            for (int a = 0; a < m; a++) {
                for (int b = a + 1; b < m; b++) {
                    swap(s[a], s[b]);
                    // 检查交换后的字符串是否在集合中
                    auto it = strToIdx.find(s);
                    if (it != strToIdx.end()) {
                        unite(i, it->second[0]);
                    }
                    swap(s[a], s[b]); // 恢复
                }
            }
        }

        int groups = 0;
        for (int i = 0; i < n; i++) {
            if (find(i) == i) groups++;
        }
        return groups;
    }
};

// ================================================================
// 解法四：BFS求连通分量（适合需要输出分组成员的变体）
// 时间复杂度: O(N^2 * M), 空间复杂度: O(N^2)

class Solution_BFS {
public:
    bool isSimilar(const string& a, const string& b) {
        int diff = 0;
        for (int i = 0; i < (int)a.size(); i++) {
            if (a[i] != b[i]) {
                if (++diff > 2) return false;
            }
        }
        return diff == 0 || diff == 2;
    }

    int numSimilarGroups(vector<string>& strs) {
        int n = strs.size();
        vector<bool> visited(n, false);
        int groups = 0;

        for (int i = 0; i < n; i++) {
            if (visited[i]) continue;
            groups++;

            // BFS从i出发
            queue<int> q;
            q.push(i);
            visited[i] = true;

            while (!q.empty()) {
                int curr = q.front(); q.pop();
                // 检查所有未访问的节点
                for (int j = 0; j < n; j++) {
                    if (!visited[j] && isSimilar(strs[curr], strs[j])) {
                        visited[j] = true;
                        q.push(j);
                    }
                }
            }
        }

        return groups;
    }
};
