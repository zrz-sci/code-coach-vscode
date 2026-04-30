// LeetCode 778: Swim in Rising Water
// 难度: Hard | 标签: BFS, Binary Search, Union Find, Heap
//
// 解法一：优先队列 BFS（Dijkstra 变种）
// 时间 O(n^2 log n)  空间 O(n^2)
//
// 思路：minimax path —— 最小化路径上的最大高度
// 用最小堆贪心地优先探索高度最低的格子

class Solution {
public:
    int swimInWater(vector<vector<int>>& grid) {
        int n = grid.size();
        // {高度, 行, 列} 最小堆
        priority_queue<tuple<int,int,int>,
                       vector<tuple<int,int,int>>,
                       greater<tuple<int,int,int>>> pq;

        vector<vector<bool>> visited(n, vector<bool>(n, false));
        int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};

        pq.push({grid[0][0], 0, 0});
        visited[0][0] = true;
        int ans = 0;

        while (!pq.empty()) {
            auto [h, r, c] = pq.top();
            pq.pop();

            // 路径上的最大高度
            ans = max(ans, h);

            // 到达终点
            if (r == n - 1 && c == n - 1) return ans;

            for (auto& d : dirs) {
                int nr = r + d[0], nc = c + d[1];
                if (nr < 0 || nr >= n || nc < 0 || nc >= n) continue;
                if (visited[nr][nc]) continue;
                visited[nr][nc] = true;
                pq.push({grid[nr][nc], nr, nc});
            }
        }
        return ans;  // 不可达（题目保证可达）
    }
};

// =============================================================
// 解法二：二分答案 + BFS 判定
// 时间 O(n^2 log n)  空间 O(n^2)
//
// 二分水位 t，BFS 检查是否能只走高度 <= t 的格子到达终点

class Solution2 {
public:
    int swimInWater(vector<vector<int>>& grid) {
        int n = grid.size();
        int lo = max(grid[0][0], grid[n-1][n-1]);
        int hi = n * n - 1;
        int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};

        // BFS 判定：水位 t 下能否从 (0,0) 到 (n-1,n-1)
        auto canReach = [&](int t) -> bool {
            if (grid[0][0] > t) return false;
            vector<vector<bool>> visited(n, vector<bool>(n, false));
            queue<pair<int,int>> q;
            q.push({0, 0});
            visited[0][0] = true;
            while (!q.empty()) {
                auto [r, c] = q.front(); q.pop();
                if (r == n-1 && c == n-1) return true;
                for (auto& d : dirs) {
                    int nr = r + d[0], nc = c + d[1];
                    if (nr < 0 || nr >= n || nc < 0 || nc >= n) continue;
                    if (visited[nr][nc] || grid[nr][nc] > t) continue;
                    visited[nr][nc] = true;
                    q.push({nr, nc});
                }
            }
            return false;
        };

        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (canReach(mid)) {
                hi = mid;
            } else {
                lo = mid + 1;
            }
        }
        return lo;
    }
};

// =============================================================
// 解法三：并查集（Kruskal 变种）
// 时间 O(n^2 * alpha(n^2)) 约等于 O(n^2)  空间 O(n^2)
//
// 按高度排序所有格子，依次激活并与已激活的邻居合并。
// 当起点和终点连通时，当前格子的高度即为答案。

class Solution3 {
    vector<int> parent, rank_;

    int find(int x) {
        while (parent[x] != x) {
            parent[x] = parent[parent[x]]; // 路径压缩
            x = parent[x];
        }
        return x;
    }

    void unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return;
        if (rank_[a] < rank_[b]) swap(a, b);
        parent[b] = a;
        if (rank_[a] == rank_[b]) rank_[a]++;
    }

    bool connected(int a, int b) {
        return find(a) == find(b);
    }

public:
    int swimInWater(vector<vector<int>>& grid) {
        int n = grid.size();
        int total = n * n;
        parent.resize(total);
        rank_.resize(total, 0);
        for (int i = 0; i < total; i++) parent[i] = i;

        // 按高度排序所有格子
        vector<pair<int, int>> cells(total); // {高度, 编号}
        for (int r = 0; r < n; r++)
            for (int c = 0; c < n; c++)
                cells[r * n + c] = {grid[r][c], r * n + c};

        sort(cells.begin(), cells.end());

        int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};
        vector<bool> activated(total, false);

        for (auto& [h, id] : cells) {
            int r = id / n, c = id % n;
            activated[id] = true;

            // 与已激活的邻居合并
            for (auto& d : dirs) {
                int nr = r + d[0], nc = c + d[1];
                if (nr < 0 || nr >= n || nc < 0 || nc >= n) continue;
                int nid = nr * n + nc;
                if (activated[nid]) {
                    unite(id, nid);
                }
            }

            // 检查起点和终点是否连通
            if (connected(0, total - 1)) return h;
        }
        return cells.back().first; // 所有格子都激活后必定连通
    }
};
