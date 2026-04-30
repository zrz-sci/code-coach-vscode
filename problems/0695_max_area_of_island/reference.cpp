/*
 * LeetCode 695: 岛屿的最大面积 (Max Area of Island)
 *
 * 【题目本质】
 * 在二维网格上找所有四方向连通的1组成的连通分量，返回最大连通分量的大小。
 * 经典的 Flood Fill / 图的连通分量问题。
 *
 * 【解法总览】
 * 解法1: DFS 递归 (沉岛法)    — O(m×n) / O(m×n) — 面试首选，代码最简洁
 * 解法2: BFS 队列 (沉岛法)    — O(m×n) / O(min(m,n)) — 无栈溢出风险
 * 解法3: DFS 迭代栈           — O(m×n) / O(m×n) — 面试追问"不用递归"
 * 解法4: 并查集 (Union Find)  — O(m×n·α) / O(m×n) — 加分项/动态场景
 *
 * 【网格示意】
 *
 *    0 0 1 0 0 0 0 1 0 0 0 0 0
 *    0 0 0 0 0 0 0 1 1 1 0 0 0
 *    0 1 1 0 1 0 0 0 0 0 0 0 0       最大岛 (面积=6):
 *    0 1 0 0 1 1 0 0[1]0[1]0 0         (3,8)
 *    0 1 0 0 1 1 0 0[1 1 1]0 0         (4,8)(4,9)(4,10)
 *    0 0 0 0 0 0 0 0 0 0[1]0 0         (5,10)
 *    0 0 0 0 0 0 0 1 1 1 0 0 0         (3,10)
 *    0 0 0 0 0 0 0 1 1 0 0 0 0
 *
 *  DFS 从 (3,8) 出发的扩展过程:
 *    visit(3,8)=1 → 下(4,8)=1 → 下(5,8)=0,右(4,9)=1 → 右(4,10)=1
 *    → 下(5,10)=1 → 下(6,10)=0,上已访问 → 回溯
 *    → 上(3,10)=1 → 上(2,10)=0 → 回溯
 *    总面积 = 6
 */

// ============================================================
// 解法1: DFS 递归 (沉岛法) — 面试首选
// 时间: O(m×n)  空间: O(m×n) 递归栈深度
//
// 【思路】
// 遍历网格，遇到1就启动一次 DFS。
// DFS 做的事：把当前格子沉没(置0)，递归扩展四方向，
// 返回 1 + 四方向递归结果之和 = 该岛面积。
// 为什么先沉后递归？—— 防止邻居递归时回头访问自己，造成死循环。
// ============================================================
class Solution1 {
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
        // 边界检查 + 是否是水：统一在入口判断，简化递归调用
        if (i < 0 || i >= m || j < 0 || j >= n || grid[i][j] == 0)
            return 0;

        // 沉岛：必须在递归四方向之前，否则会死循环
        grid[i][j] = 0;

        // 当前格子贡献面积1，加上四方向扩展的面积
        return 1 + dfs(grid, i + 1, j, m, n)
                 + dfs(grid, i - 1, j, m, n)
                 + dfs(grid, i, j + 1, m, n)
                 + dfs(grid, i, j - 1, m, n);
    }
};

// ============================================================
// 解法2: BFS 队列 (沉岛法) — 无栈溢出风险
// 时间: O(m×n)  空间: O(min(m,n)) 队列最大长度
//
// 【思路】
// 和 DFS 的区别：用队列逐层扩展，而不是递归深入。
// 关键细节：入队时就标记 grid[nx][ny]=0，不是出队时标记。
// 为什么？—— 如果出队才标记，同一个格子会被多个邻居重复入队，
// 导致 area 被多算。
//
// BFS 扩展过程示意:
//   初始: 入队(3,8), 标记(3,8)=0, area=0
//   出队(3,8): area=1, 邻居(4,8)=1→入队标记
//   出队(4,8): area=2, 邻居(4,9)=1→入队标记
//   出队(4,9): area=3, 邻居(4,10)=1→入队标记
//   出队(4,10): area=4, 邻居(5,10)=1→入队, (3,10)=1→入队
//   出队(5,10): area=5, 无新邻居
//   出队(3,10): area=6, 无新邻居
//   队列空, 返回 area=6
// ============================================================
class Solution2 {
public:
    int maxAreaOfIsland(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        int maxArea = 0;
        int dx[] = {0, 0, 1, -1};
        int dy[] = {1, -1, 0, 0};

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 1) {
                    int area = 0;
                    queue<pair<int,int>> q;
                    q.push({i, j});
                    grid[i][j] = 0;  // 入队时标记！不是出队时！

                    while (!q.empty()) {
                        auto [x, y] = q.front();
                        q.pop();
                        area++;  // 出队时计数

                        for (int d = 0; d < 4; d++) {
                            int nx = x + dx[d];
                            int ny = y + dy[d];
                            if (nx >= 0 && nx < m && ny >= 0 && ny < n
                                && grid[nx][ny] == 1) {
                                grid[nx][ny] = 0;  // 入队时标记
                                q.push({nx, ny});
                            }
                        }
                    }
                    maxArea = max(maxArea, area);
                }
            }
        }
        return maxArea;
    }
};

// ============================================================
// 解法3: DFS 迭代栈 — 面试追问"不用递归怎么做"
// 时间: O(m×n)  空间: O(m×n) 栈最大深度
//
// 【思路】
// 把解法1的递归改成显式栈。逻辑完全一样：
// - 递归调用 → 入栈
// - 递归返回 → 出栈
// 和 BFS 的唯一区别：用 stack (LIFO) 代替 queue (FIFO)。
// 遍历顺序不同（深度优先 vs 广度优先），但面积计算结果相同。
// ============================================================
class Solution3 {
public:
    int maxAreaOfIsland(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        int maxArea = 0;
        int dx[] = {0, 0, 1, -1};
        int dy[] = {1, -1, 0, 0};

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 1) {
                    int area = 0;
                    stack<pair<int,int>> stk;
                    stk.push({i, j});
                    grid[i][j] = 0;  // 入栈时标记

                    while (!stk.empty()) {
                        auto [x, y] = stk.top();
                        stk.pop();
                        area++;

                        for (int d = 0; d < 4; d++) {
                            int nx = x + dx[d];
                            int ny = y + dy[d];
                            if (nx >= 0 && nx < m && ny >= 0 && ny < n
                                && grid[nx][ny] == 1) {
                                grid[nx][ny] = 0;  // 入栈时标记
                                stk.push({nx, ny});
                            }
                        }
                    }
                    maxArea = max(maxArea, area);
                }
            }
        }
        return maxArea;
    }
};

// ============================================================
// 解法4: 并查集 (Union Find) — 加分项
// 时间: O(m×n·α(m×n)) ≈ O(m×n)  空间: O(m×n)
//
// 【思路】
// 换一种视角：每个值为1的格子是一个节点，相邻的1之间有边。
// 问题变成：找所有连通分量中最大的那个。
//
// 并查集做的事：
// 1. 遍历网格，对每个为1的格子，尝试和它右边、下边的1合并
//    （为什么只看右和下？因为左和上在之前的遍历中已经处理过了）
// 2. 合并时，维护每个集合的 size
// 3. 最后遍历所有为1的格子，找 size[find(cell)] 的最大值
//
// 适用场景：当格子可以动态变化（0→1）时，并查集可以增量维护，
// 而 DFS/BFS 每次都要重新遍历。参见 LeetCode 305。
// ============================================================
class Solution4 {
public:
    vector<int> parent, rank_, size_;

    int find(int x) {
        // 路径压缩：让所有节点直接指向根
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }

    void unite(int a, int b) {
        int ra = find(a), rb = find(b);
        if (ra == rb) return;  // 已在同一集合

        // 按秩合并：矮树挂到高树下面，保持平衡
        if (rank_[ra] < rank_[rb]) swap(ra, rb);
        parent[rb] = ra;
        size_[ra] += size_[rb];  // 合并后更新大小
        if (rank_[ra] == rank_[rb]) rank_[ra]++;
    }

    int maxAreaOfIsland(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        parent.resize(m * n);
        rank_.resize(m * n, 0);
        size_.resize(m * n, 1);

        for (int i = 0; i < m * n; i++) parent[i] = i;

        // 只需向右(0,1)和向下(1,0)合并，避免重复处理
        int dx[] = {1, 0};
        int dy[] = {0, 1};

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 1) {
                    for (int d = 0; d < 2; d++) {
                        int ni = i + dx[d], nj = j + dy[d];
                        if (ni < m && nj < n && grid[ni][nj] == 1) {
                            // 二维坐标 → 一维编号：i*n+j
                            unite(i * n + j, ni * n + nj);
                        }
                    }
                }
            }
        }

        // 遍历所有为1的格子，找最大集合
        int maxArea = 0;
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                if (grid[i][j] == 1)
                    maxArea = max(maxArea, size_[find(i * n + j)]);
        return maxArea;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法       | 时间    | 空间        | 特点                      |
// |-----------|---------|-------------|--------------------------|
// | DFS递归    | O(mn)   | O(mn) 栈深  | 代码最简洁，面试首选        |
// | BFS       | O(mn)   | O(min(m,n)) | 无栈溢出，工程实践首选      |
// | DFS迭代栈  | O(mn)   | O(mn)       | 面试追问"不用递归"         |
// | 并查集     | O(mn·α) | O(mn)       | 支持动态连通性，扩展性最强   |
//
// 面试策略：
// 1. 先写 DFS 递归（30秒写完，展示熟练度）
// 2. 被追问大矩阵栈溢出 → 改 BFS 或迭代栈
// 3. 被追问动态更新 → 提并查集
//
// ============================================================
// 【易错点】
//
// 1. BFS 出队时才标记 vs 入队时标记
//    ✗ while 循环内: auto [x,y] = q.front(); q.pop(); grid[x][y] = 0;
//    ✓ 入队时: grid[nx][ny] = 0; q.push({nx, ny});
//    原因: 出队才标记会导致同一格子被多个邻居重复入队，area 被多算。
//
// 2. DFS 沉岛时机
//    ✗ 先递归四方向，最后 grid[i][j] = 0;
//    ✓ 进入 DFS 后立刻 grid[i][j] = 0; 再递归
//    原因: 不先沉岛，邻居会回头访问自己，无限递归 → 栈溢出。
//
// 3. 并查集取 size 时忘记 find
//    ✗ maxArea = max(maxArea, size_[i * n + j]);
//    ✓ maxArea = max(maxArea, size_[find(i * n + j)]);
//    原因: size 只在根节点是正确的，非根节点的 size 可能是旧值。
//
// 4. 边界检查遗漏
//    ✗ if (grid[nx][ny] == 1) — 忘记检查 nx, ny 是否越界
//    ✓ if (nx >= 0 && nx < m && ny >= 0 && ny < n && grid[nx][ny] == 1)
//
// ============================================================
// 【面试追问】(递进链)
//
// Q1: 为什么遍历整个网格的时间是 O(m×n) 而不是 O(m×n × 岛屿大小)？
// → 每个格子最多被访问一次（被标记为0后不会再进入 DFS/BFS），
//   所以所有 DFS/BFS 的总工作量是 O(m×n)，不是每次 DFS 都遍历整个网格。
//
// Q2: 如果矩阵 10000×10000，递归 DFS 会怎样？怎么解决？
// → 最坏递归深度 10^8，栈溢出。改用 BFS（队列在堆上）或迭代 DFS（显式栈）。
//
// Q3: 如果不能修改原数组怎么办？
// → 额外用 vector<vector<bool>> visited(m, vector<bool>(n, false))，
//   空间 O(m×n)。或者先修改再恢复（需要记录修改的坐标列表）。
//
// Q4: 如果格子可以动态从 0 变为 1，如何实时维护最大岛屿面积？
// → 并查集。每次 0→1 时，与四邻居中为1的格子做 union，
//   更新全局 maxSize。参见 LeetCode 305: Number of Islands II。
//
// ============================================================
// 【相关题型】
//
// 200. 岛屿数量
//   — 本题的简化版，只需计数连通分量个数。DFS 框架完全复用，
//     去掉面积返回值，改为计数器+1。
//
// 463. 岛屿的周长
//   — 同样的网格遍历。区别：每个1贡献4条边，每个相邻的1共享2条边（各减1）。
//     不需要 DFS，直接遍历计算即可。
//
// 827. 最大人工岛 (Making A Large Island)
//   — 本题的升级版。先给每个岛编号+算面积（复用本题 DFS），
//     然后枚举每个0，看翻转后能连接哪些不同编号的岛，取最大和。
//
// 305. 岛屿数量 II (Number of Islands II)
//   — 动态版。复用本题解法4的并查集框架，每次加入一个1时做 union。
//
// 733. 图像渲染 (Flood Fill)
//   — 复用完全相同的 DFS/BFS 框架，只是"沉岛"变成"染色"。
// ============================================================
