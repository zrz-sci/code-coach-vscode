// 【题目本质】
// 矩阵中找能同时流到两个海洋的格子，核心技巧是"逆向搜索"从边界出发

// 【解法总览】
// Solution1: 反向 BFS — O(m*n) / O(m*n) — 面试首选，无栈溢出风险
// Solution2: 反向 DFS — O(m*n) / O(m*n) — 代码更短，递归实现

// ===================== Solution 1: 反向 BFS =====================
// 从两个海洋的边界分别 BFS，标记可达格子，取交集
class Solution1 {
public:
    vector<vector<int>> pacificAtlantic(vector<vector<int>>& heights) {
        int m = heights.size(), n = heights[0].size();
        vector<vector<bool>> pacific(m, vector<bool>(n, false));
        vector<vector<bool>> atlantic(m, vector<bool>(n, false));
        queue<pair<int,int>> pq, aq;

        // 初始化边界
        for (int i = 0; i < m; i++) {
            pq.push({i, 0});     pacific[i][0] = true;       // 左边界→太平洋
            aq.push({i, n - 1}); atlantic[i][n - 1] = true;  // 右边界→大西洋
        }
        for (int j = 0; j < n; j++) {
            pq.push({0, j});     pacific[0][j] = true;       // 上边界→太平洋
            aq.push({m - 1, j}); atlantic[m - 1][j] = true;  // 下边界→大西洋
        }

        int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};
        auto bfs = [&](queue<pair<int,int>>& q, vector<vector<bool>>& vis) {
            while (!q.empty()) {
                auto [r, c] = q.front(); q.pop();
                for (auto& d : dirs) {
                    int nr = r + d[0], nc = c + d[1];
                    if (nr >= 0 && nr < m && nc >= 0 && nc < n &&
                        !vis[nr][nc] && heights[nr][nc] >= heights[r][c]) {
                        vis[nr][nc] = true;
                        q.push({nr, nc});
                    }
                }
            }
        };

        bfs(pq, pacific);
        bfs(aq, atlantic);

        // 取交集
        vector<vector<int>> res;
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                if (pacific[i][j] && atlantic[i][j])
                    res.push_back({i, j});
        return res;
    }
};

// ===================== Solution 2: 反向 DFS =====================
class Solution2 {
public:
    int m, n;
    int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};

    void dfs(vector<vector<int>>& h, vector<vector<bool>>& vis, int r, int c) {
        vis[r][c] = true;
        for (auto& d : dirs) {
            int nr = r + d[0], nc = c + d[1];
            if (nr >= 0 && nr < m && nc >= 0 && nc < n &&
                !vis[nr][nc] && h[nr][nc] >= h[r][c]) {
                dfs(h, vis, nr, nc);
            }
        }
    }

    vector<vector<int>> pacificAtlantic(vector<vector<int>>& heights) {
        m = heights.size(); n = heights[0].size();
        vector<vector<bool>> pac(m, vector<bool>(n)), atl(m, vector<bool>(n));

        for (int i = 0; i < m; i++) {
            dfs(heights, pac, i, 0);
            dfs(heights, atl, i, n - 1);
        }
        for (int j = 0; j < n; j++) {
            dfs(heights, pac, 0, j);
            dfs(heights, atl, m - 1, j);
        }

        vector<vector<int>> res;
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                if (pac[i][j] && atl[i][j])
                    res.push_back({i, j});
        return res;
    }
};

// 【解法对比】
// Solution1 (BFS): 无栈溢出风险，适合大矩阵(200x200)
// Solution2 (DFS): 代码更短，但递归深度可达 m*n = 40000，可能栈溢出

// 【易错点】
// 1. 反向搜索条件是 >=（逆流而上），不是 <=
// 2. 边界初始化：太平洋=上+左，大西洋=下+右
// 3. 角点(0,0)同时邻接两个海洋，两次搜索都会覆盖到

// 【面试追问】
// Q1: 为什么反向? → 正向 O(m^2*n^2)，反向 O(m*n)
// Q2: BFS vs DFS? → BFS 更安全无栈溢出，DFS 代码更短
// Q3: 一个 visited? → 位标记 bit0=pac bit1=atl，值==3 即交集
