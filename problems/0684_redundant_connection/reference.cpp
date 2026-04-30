// LeetCode 684: Redundant Connection - Reference Solution
// 方法: 并查集 (Union-Find)
// 时间: O(n * alpha(n)), 空间: O(n)

class Solution {
public:
    vector<int> parent, rank_;

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]); // 路径压缩
        }
        return parent[x];
    }

    bool unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return false; // 已在同一集合，形成环
        // 按秩合并
        if (rank_[px] < rank_[py]) swap(px, py);
        parent[py] = px;
        if (rank_[px] == rank_[py]) rank_[px]++;
        return true;
    }

    vector<int> findRedundantConnection(vector<vector<int>>& edges) {
        int n = edges.size();
        parent.resize(n + 1);
        rank_.resize(n + 1, 0);
        for (int i = 1; i <= n; i++) {
            parent[i] = i;
        }

        for (auto& e : edges) {
            if (!unite(e[0], e[1])) {
                return e; // 这条边使两个已连通的节点再次相连，形成环
            }
        }

        return {}; // 不会到达
    }
};
