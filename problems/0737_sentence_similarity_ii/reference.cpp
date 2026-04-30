// LeetCode 737. Sentence Similarity II
// 解法一：Union-Find（并查集）- 推荐解法
// Time: O(n + p * alpha(p)), Space: O(p)
// 其中 n 为句子长度，p 为相似对数量

class Solution {
public:
    // parent 映射：字符串 -> 其所在集合的代表元
    unordered_map<string, string> parent;
    // rank 映射：按秩合并优化
    unordered_map<string, int> rank_;

    // 查找操作 + 路径压缩
    string find(const string& x) {
        if (parent.find(x) == parent.end()) {
            parent[x] = x;
            rank_[x] = 0;
        }
        if (parent[x] != x) {
            parent[x] = find(parent[x]); // 路径压缩
        }
        return parent[x];
    }

    // 合并操作 + 按秩合并
    void unite(const string& a, const string& b) {
        string pa = find(a), pb = find(b);
        if (pa == pb) return;
        // 按秩合并：矮树接到高树下
        if (rank_[pa] < rank_[pb]) swap(pa, pb);
        parent[pb] = pa;
        if (rank_[pa] == rank_[pb]) rank_[pa]++;
    }

    bool areSentencesSimilarTwo(vector<string>& sentence1,
                                vector<string>& sentence2,
                                vector<vector<string>>& similarPairs) {
        // 边界检查：长度必须相同
        if (sentence1.size() != sentence2.size()) return false;

        // 构建并查集：将所有相似对合并
        for (auto& pair : similarPairs) {
            unite(pair[0], pair[1]);
        }

        // 逐位比较
        for (int i = 0; i < (int)sentence1.size(); i++) {
            // 完全相同的单词直接跳过
            if (sentence1[i] == sentence2[i]) continue;
            // 检查是否在同一连通分量中
            if (find(sentence1[i]) != find(sentence2[i])) {
                return false;
            }
        }
        return true;
    }
};


// 解法二：DFS 建图判断可达性
// Time: O(n * p), Space: O(p)
// 适合不熟悉 Union-Find 时的替代方案

class Solution2 {
public:
    bool areSentencesSimilarTwo(vector<string>& sentence1,
                                vector<string>& sentence2,
                                vector<vector<string>>& similarPairs) {
        if (sentence1.size() != sentence2.size()) return false;

        // 邻接表建图
        unordered_map<string, vector<string>> graph;
        for (auto& pair : similarPairs) {
            graph[pair[0]].push_back(pair[1]);
            graph[pair[1]].push_back(pair[0]);
        }

        for (int i = 0; i < (int)sentence1.size(); i++) {
            if (sentence1[i] == sentence2[i]) continue;
            // DFS 判断从 sentence1[i] 能否到达 sentence2[i]
            if (!dfsReachable(graph, sentence1[i], sentence2[i])) {
                return false;
            }
        }
        return true;
    }

private:
    bool dfsReachable(unordered_map<string, vector<string>>& graph,
                      const string& src, const string& dst) {
        unordered_set<string> visited;
        return dfs(graph, src, dst, visited);
    }

    bool dfs(unordered_map<string, vector<string>>& graph,
             const string& cur, const string& dst,
             unordered_set<string>& visited) {
        if (cur == dst) return true;
        visited.insert(cur);

        if (graph.find(cur) == graph.end()) return false;

        for (const string& neighbor : graph[cur]) {
            if (visited.count(neighbor)) continue;
            if (dfs(graph, neighbor, dst, visited)) {
                return true;
            }
        }
        return false;
    }
};


// 解法三：BFS 判断可达性
// Time: O(n * p), Space: O(p)

class Solution3 {
public:
    bool areSentencesSimilarTwo(vector<string>& sentence1,
                                vector<string>& sentence2,
                                vector<vector<string>>& similarPairs) {
        if (sentence1.size() != sentence2.size()) return false;

        // 邻接表建图
        unordered_map<string, vector<string>> graph;
        for (auto& pair : similarPairs) {
            graph[pair[0]].push_back(pair[1]);
            graph[pair[1]].push_back(pair[0]);
        }

        for (int i = 0; i < (int)sentence1.size(); i++) {
            if (sentence1[i] == sentence2[i]) continue;
            if (!bfsReachable(graph, sentence1[i], sentence2[i])) {
                return false;
            }
        }
        return true;
    }

private:
    bool bfsReachable(unordered_map<string, vector<string>>& graph,
                      const string& src, const string& dst) {
        if (graph.find(src) == graph.end() || graph.find(dst) == graph.end()) {
            return false;
        }

        unordered_set<string> visited;
        queue<string> q;
        q.push(src);
        visited.insert(src);

        while (!q.empty()) {
            string cur = q.front();
            q.pop();

            if (cur == dst) return true;

            for (const string& neighbor : graph[cur]) {
                if (!visited.count(neighbor)) {
                    visited.insert(neighbor);
                    q.push(neighbor);
                }
            }
        }
        return false;
    }
};
