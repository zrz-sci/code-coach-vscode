/*
 * LCR 105: 岛屿的最大面积 (Max Area of Island)
 * 同主站 695. Max Area of Island
 *
 * 思路: 遍历网格, 遇到 1 启动 DFS/BFS 搜索连通区域, 沉岛标记, 取最大面积
 *
 * 时间: O(m*n)   空间: O(m*n) 递归栈 / O(min(m,n)) BFS 队列
 */

// ═══════════════════════════════════════════════════════
// 解法一: DFS 沉岛法 (面试首选, 代码最短)
// ═══════════════════════════════════════════════════════

class Solution {
public:
    int maxAreaOfIsland(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        int maxArea = 0;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 1) {
                    maxArea = max(maxArea, dfs(grid, i, j, m, n));
                }
            }
        }
        return maxArea;
    }

private:
    int dfs(vector<vector<int>>& grid, int i, int j, int m, int n) {
        // 越界或水域 → 面积 0
        if (i < 0 || i >= m || j < 0 || j >= n || grid[i][j] == 0) {
            return 0;
        }
        grid[i][j] = 0;  // 沉岛: 标记已访问
        // 当前格子 1 + 四方向递归
        return 1 + dfs(grid, i + 1, j, m, n)   // 下
                 + dfs(grid, i - 1, j, m, n)   // 上
                 + dfs(grid, i, j + 1, m, n)   // 右
                 + dfs(grid, i, j - 1, m, n);  // 左
    }
};


// ═══════════════════════════════════════════════════════
// 解法二: BFS 沉岛法 (避免栈溢出)
// ═══════════════════════════════════════════════════════

class Solution_BFS {
public:
    int maxAreaOfIsland(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        int maxArea = 0;
        int dx[] = {0, 0, 1, -1};
        int dy[] = {1, -1, 0, 0};

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] != 1) continue;

                queue<pair<int, int>> q;
                q.push({i, j});
                grid[i][j] = 0;  // 入队时标记!
                int area = 0;

                while (!q.empty()) {
                    auto [x, y] = q.front();
                    q.pop();
                    area++;

                    for (int d = 0; d < 4; d++) {
                        int nx = x + dx[d], ny = y + dy[d];
                        if (nx >= 0 && nx < m && ny >= 0 && ny < n
                            && grid[nx][ny] == 1) {
                            grid[nx][ny] = 0;  // 入队时立即标记
                            q.push({nx, ny});
                        }
                    }
                }

                maxArea = max(maxArea, area);
            }
        }
        return maxArea;
    }
};


// ═══════════════════════════════════════════════════════
// 解法三: Union-Find 并查集
// ═══════════════════════════════════════════════════════

class UnionFind {
    vector<int> parent, rank_, size_;
    int maxSize;
public:
    UnionFind(int n) : parent(n), rank_(n, 0), size_(n, 1), maxSize(0) {
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }

    void unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return;
        if (rank_[px] < rank_[py]) swap(px, py);
        parent[py] = px;
        size_[px] += size_[py];
        if (rank_[px] == rank_[py]) rank_[px]++;
        maxSize = max(maxSize, size_[px]);
    }

    void activate(int x) { maxSize = max(maxSize, 1); }
    int getMaxSize() const { return maxSize; }
};

class Solution_UF {
public:
    int maxAreaOfIsland(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        UnionFind uf(m * n);

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] != 1) continue;
                uf.activate(i * n + j);
                // 只向右和向下合并, 避免重复
                if (i + 1 < m && grid[i + 1][j] == 1)
                    uf.unite(i * n + j, (i + 1) * n + j);
                if (j + 1 < n && grid[i][j + 1] == 1)
                    uf.unite(i * n + j, i * n + j + 1);
            }
        }

        return uf.getMaxSize();
    }
};


/*
 * ═══════════════════════════════════════════════════════
 * 干跑示例 (简化网格)
 * ═══════════════════════════════════════════════════════
 *
 * grid:
 *   1 1 0
 *   1 0 0
 *   0 0 1
 *
 * 扫描 (0,0): grid[0][0]=1 → dfs(0,0)
 *   grid[0][0]=0, area=1
 *   +dfs(1,0): grid[1][0]=0, area=1
 *     +dfs(2,0): grid[2][0]=0 → return 0
 *     +dfs(0,0): 已=0 → return 0
 *     +dfs(1,1): grid[1][1]=0 → return 0
 *     +dfs(1,-1): 越界 → return 0
 *     return 1
 *   +dfs(-1,0): 越界 → return 0
 *   +dfs(0,1): grid[0][1]=0, area=1
 *     +dfs(1,1): 已=0 → return 0
 *     +dfs(-1,1): 越界 → return 0
 *     +dfs(0,2): grid[0][2]=0 → return 0
 *     +dfs(0,0): 已=0 → return 0
 *     return 1
 *   +dfs(0,-1): 越界 → return 0
 *   total = 1+1+0+1+0 = 3
 *   maxArea = max(0, 3) = 3
 *
 * 扫描 (0,1): grid[0][1]=0, 跳过
 * 扫描 (1,0): grid[1][0]=0, 跳过
 * ...
 * 扫描 (2,2): grid[2][2]=1 → dfs(2,2)
 *   grid[2][2]=0, 四方向均为0或越界
 *   total = 1
 *   maxArea = max(3, 1) = 3
 *
 * 答案: 3 ✓
 *
 * ═══════════════════════════════════════════════════════
 * 题目示例 (8x13 网格)
 * ═══════════════════════════════════════════════════════
 *
 * 岛屿识别:
 *   A: (0,2) → 面积 1
 *   B: (0,7),(1,7),(1,8),(1,9) → 面积 4
 *   C: (2,1),(2,2),(3,1),(4,1) → 面积 4 (L 形)
 *   D: (2,4),(3,4),(3,5),(4,4),(4,5) → 面积 5?
 *      让我仔细数: (2,4)=1 没有, 看原数据
 *      row2: 0 1 1 0 [1] 0 0 ...  (2,4)=1
 *      row3: 0 1 0 0 [1][1] 0 ... (3,4)=1,(3,5)=1
 *      row4: 0 1 0 0 [1][1] 0 ... (4,4)=1,(4,5)=1
 *      (2,4) 与 (3,4) 相邻 → 连通
 *      面积 = 5?  但题目答案是 6
 *
 *   重新看: row4 后面还有
 *   row3: 0 1 0 0 1 1 0 0 [1] 0 [1] 0 0
 *   row4: 0 1 0 0 1 1 0 0 [1][1][1] 0 0
 *   row5: 0 0 0 0 0 0 0 0  0  0 [1] 0 0
 *
 *   (3,8),(3,10),(4,8),(4,9),(4,10),(5,10) →
 *   (3,8)与(4,8)相邻, (4,8)与(4,9)相邻, (4,9)与(4,10)相邻
 *   (4,10)与(3,10)相邻, (4,10)与(5,10)相邻
 *   → 6 个格子全部连通, 面积 = 6
 *
 * 最大岛屿面积 = 6 ✓
 *
 * ═══════════════════════════════════════════════════════
 * 边界情况
 * ═══════════════════════════════════════════════════════
 *
 * 全为 0: [[0,0,0,0,0,0,0,0]] → 0
 * 全为 1: [[1,1],[1,1]] → 4
 * 单格子: [[1]] → 1
 *
 * ═══════════════════════════════════════════════════════
 * 易错点
 * ═══════════════════════════════════════════════════════
 *
 * ✗ DFS 入口忘记边界检查 → 越界访问导致 crash
 * ✗ BFS 出队才标记(而非入队时标记) → 同一格子重复入队, 面积多算
 * ✗ 八方向搜索 → 题目只要求四方向(上下左右), 不含对角线
 * ✗ 修改原数组但题目不允许 → 需用 visited 数组或搜索后恢复原值
 * ✗ 方向数组 dx/dy 写反或写漏 → 少搜方向, 面积少算
 *
 * ═══════════════════════════════════════════════════════
 * 面试追问
 * ═══════════════════════════════════════════════════════
 *
 * Q1: 不能修改原数组怎么办?
 * A1: 用额外的 visited 二维数组标记, 或搜索完后恢复原值(回溯).
 *
 * Q2: DFS 栈溢出怎么处理?
 * A2: 50x50 全1时递归深度达 2500. 改用 BFS 或迭代 DFS(显式栈).
 *
 * Q3: 如果陆地可以动态添加(如 LC 305)?
 * A3: DFS/BFS 每次重新搜索不高效. Union-Find 支持 O(alpha(n)) 合并.
 *
 * Q4: DFS 和 BFS 空间复杂度对比?
 * A4: DFS 递归栈 O(m*n) 最坏; BFS 队列 O(min(m,n)) — BFS 更省空间.
 */
