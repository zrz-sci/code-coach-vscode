/*
 * LeetCode 1905: Count Sub Islands (统计子岛屿)
 *
 * 【题目本质】
 * 两个 m x n 二进制网格 grid1 和 grid2。
 * grid2 中的岛屿如果每个格子在 grid1 中也是陆地 → 子岛屿。
 * 返回 grid2 中子岛屿的数量。
 *
 * 本质: 在"岛屿数量"(LC200)基础上加一个附加条件检查。
 *
 * 【解法总览】
 * 解法1: DFS 一趟检查       — O(m*n) / O(m*n)       — 面试首选
 * 解法2: BFS 一趟检查       — O(m*n) / O(min(m,n))   — 无栈溢出
 * 解法3: 预处理淹没 + 计数  — O(m*n) / O(m*n)       — 逻辑最清晰
 *
 * 【岛屿可视化】
 *
 *   grid1:              grid2:
 *   1 1 1 0 0           1 1 1 0 0     岛屿A: (0,0~2)(1,2~4)
 *   0 1 1 1 1           0 0 1 1 1       grid1对应全1 → 子岛 ✓
 *   0 0 0 0 0           0 1 0 0 0     岛屿B: (2,1)
 *   1 0 0 0 0           1 0 1 1 0       grid1[2][1]=0 → 非子岛 ✗
 *   1 1 0 1 1           0 1 0 1 0     岛屿C: (3,0) → 子岛 ✓
 *                                     岛屿D: (3,2)(3,3) → grid1[3][2]=0 ✗
 *   DFS 处理 grid2:                   岛屿E: (4,1) → 子岛 ✓
 *   遇到1 → DFS整个岛 → 检查grid1     岛屿F: (4,3) → 子岛 ✓(但答案3)
 *   答案: 3个子岛屿
 *
 * 【关键陷阱: 不能提前终止DFS!】
 *
 *   即使发现 grid1[i][j]==0, 也必须继续DFS标记grid2整个岛屿。
 *   否则未标记的格子会在后续被当成新岛屿再处理, 导致计数错误。
 *
 *   错误写法 (提前返回):
 *     if (grid1[i][j] == 0) return false;  // ✗ 剩余格子未标记!
 *
 *   错误写法 (短路求值):
 *     return grid1[i][j] && dfs(i+1,j) && dfs(i-1,j) && ...;
 *     // ✗ 前面为false时后面的dfs不执行, 岛屿标记不完整!
 *
 *   正确写法:
 *     bool ok = (grid1[i][j] == 1);
 *     ok = dfs(i+1,j) && ok;  // dfs先执行, ok后判断
 *     ok = dfs(i-1,j) && ok;
 *     ok = dfs(i,j+1) && ok;
 *     ok = dfs(i,j-1) && ok;
 *     return ok;
 */


// ============================================================
// 解法1: DFS 一趟检查 — 面试首选
// 时间: O(m * n)  每个格子最多访问一次
// 空间: O(m * n)  最坏递归深度
//
// 【思路】
// 遍历 grid2，遇到 1 就 DFS 整个岛屿:
// - 标记 grid2[i][j] = 0 (沉岛，替代 visited)
// - 检查 grid1[i][j] 是否为 1
// - 四个方向递归
// - 返回 bool: 整个岛是否全在 grid1 陆地上
//
// 【避免短路求值的技巧】
// C++ 的 && 有短路特性: 左边为 false 时右边不执行。
// 如果写 `dfs() && isSub`, dfs 先执行（保证标记）。
// 如果写 `isSub && dfs()`, isSub=false 时 dfs 被跳过!
// 所以必须把 dfs() 放在 && 的左边。
// ============================================================
class Solution1 {
public:
    int m, n;

    // DFS 探索 grid2 中的一个岛屿
    // 返回: 该岛是否是子岛屿 (grid2 的每个格子在 grid1 中也是 1)
    bool dfs(vector<vector<int>>& grid1, vector<vector<int>>& grid2,
             int i, int j) {
        // 越界或水域 → 不影响子岛判定
        if (i < 0 || i >= m || j < 0 || j >= n || grid2[i][j] == 0)
            return true;

        // 沉岛: 标记已访问
        grid2[i][j] = 0;

        // 检查当前格在 grid1 中是否为陆地
        bool isSub = (grid1[i][j] == 1);

        // 四个方向递归
        // 关键: dfs() 放在 && 左边, 保证无论 isSub 为何都会执行
        isSub = dfs(grid1, grid2, i + 1, j) && isSub;
        isSub = dfs(grid1, grid2, i - 1, j) && isSub;
        isSub = dfs(grid1, grid2, i, j + 1) && isSub;
        isSub = dfs(grid1, grid2, i, j - 1) && isSub;

        return isSub;
    }

    int countSubIslands(vector<vector<int>>& grid1,
                        vector<vector<int>>& grid2) {
        m = grid1.size();
        n = grid1[0].size();
        int count = 0;

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid2[i][j] == 1) {
                    // DFS 整个岛并判定是否为子岛
                    if (dfs(grid1, grid2, i, j)) {
                        count++;
                    }
                }
            }
        }
        return count;
    }
};


// ============================================================
// 解法2: BFS 一趟检查 — 无栈溢出风险
// 时间: O(m * n)
// 空间: O(min(m, n))  BFS 队列最大长度
//
// 【思路】
// 和解法1 完全相同的逻辑, 只是用队列替代递归。
// 好处: grid 最大 500x500=250000, DFS 递归可能栈溢出,
//       BFS 用堆内存, 更安全。
//
// 【BFS 过程可视化 (从(0,0)出发)】
//
//   初始: queue=[(0,0)], grid2[0][0]=0, grid1[0][0]=1 → isSub=true
//   弹出(0,0):
//     → (0,1): grid2=1, 入队沉岛, grid1=1 → isSub仍true
//     → (1,0): grid2=0, 跳过
//   弹出(0,1):
//     → (0,2): grid2=1, 入队沉岛, grid1=1 → isSub仍true
//   弹出(0,2):
//     → (1,2): grid2=1, 入队沉岛, grid1=1 → isSub仍true
//   弹出(1,2):
//     → (1,3): grid2=1, 入队沉岛, grid1=1 → isSub仍true
//   弹出(1,3):
//     → (1,4): grid2=1, 入队沉岛, grid1=1 → isSub仍true
//   弹出(1,4): 四周全0或越界
//   队列空, isSub=true → count=1, 这是一个子岛屿!
// ============================================================
class Solution2 {
public:
    int countSubIslands(vector<vector<int>>& grid1,
                        vector<vector<int>>& grid2) {
        int m = grid2.size(), n = grid2[0].size();
        int dx[] = {0, 0, 1, -1};
        int dy[] = {1, -1, 0, 0};
        int count = 0;

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid2[i][j] == 1) {
                    bool isSub = true;
                    queue<pair<int, int>> q;

                    // 入队时就沉岛!
                    grid2[i][j] = 0;
                    q.push({i, j});
                    if (grid1[i][j] == 0) isSub = false;

                    while (!q.empty()) {
                        auto [x, y] = q.front();
                        q.pop();

                        for (int d = 0; d < 4; d++) {
                            int nx = x + dx[d], ny = y + dy[d];
                            if (nx >= 0 && nx < m && ny >= 0 && ny < n
                                && grid2[nx][ny] == 1) {
                                // 入队时标记, 检查 grid1
                                grid2[nx][ny] = 0;
                                if (grid1[nx][ny] == 0) isSub = false;
                                q.push({nx, ny});
                            }
                        }
                    }

                    if (isSub) count++;
                }
            }
        }
        return count;
    }
};


// ============================================================
// 解法3: 预处理淹没 + 标准岛屿计数 — 逻辑最清晰
// 时间: O(m * n)  两趟遍历
// 空间: O(m * n)  递归栈
//
// 【思路: 两步分离】
// Step 1 (预处理): 找所有 grid1[i][j]=0 但 grid2[i][j]=1 的格子,
//   从这些点出发 DFS 把 grid2 中对应的整个岛屿全部沉掉。
//   因为这些岛包含"grid1中是水"的格子, 不可能是子岛。
//
// Step 2 (计数): 此时 grid2 中剩下的岛屿, 保证每个格子在
//   grid1 中都是1, 即全是子岛屿。直接数岛屿数即可!
//   → 和 LC200 "岛屿数量" 完全一样的代码。
//
// 【优点】
// 把"子岛判定"和"岛屿计数"拆成两个独立步骤,
// 每一步都很简单, 不容易出错。
// ============================================================
class Solution3 {
public:
    int m, n;

    // 标准沉岛 DFS (同 LC200)
    void sink(vector<vector<int>>& grid, int i, int j) {
        if (i < 0 || i >= m || j < 0 || j >= n || grid[i][j] == 0)
            return;
        grid[i][j] = 0;
        sink(grid, i + 1, j);
        sink(grid, i - 1, j);
        sink(grid, i, j + 1);
        sink(grid, i, j - 1);
    }

    int countSubIslands(vector<vector<int>>& grid1,
                        vector<vector<int>>& grid2) {
        m = grid1.size();
        n = grid1[0].size();

        // Step 1: 淹掉 grid2 中"肯定不是子岛"的岛屿
        // 如果 grid1[i][j]=0 但 grid2[i][j]=1,
        // 从 (i,j) 出发把 grid2 中整个岛沉掉
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid1[i][j] == 0 && grid2[i][j] == 1) {
                    sink(grid2, i, j);
                }
            }
        }

        // Step 2: 数 grid2 中剩余的岛屿 (全是子岛)
        // 和 LC200 完全一样的逻辑
        int count = 0;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid2[i][j] == 1) {
                    count++;
                    sink(grid2, i, j);
                }
            }
        }

        return count;
    }
};


// ============================================================
// 【解法对比】
//
// | 维度       | DFS一趟        | BFS一趟         | 预处理淹没      |
// |-----------|---------------|----------------|----------------|
// | 时间       | O(m*n)        | O(m*n)         | O(m*n) 两趟    |
// | 空间       | O(m*n) 栈     | O(min(m,n)) 队列| O(m*n) 栈      |
// | 代码复杂度 | 中等           | 较长            | 中等 (两个简单步)|
// | 栈溢出风险 | ⚠️ 有          | 无              | ⚠️ 有           |
// | 逻辑清晰度 | 中等           | 中等            | 最清晰          |
//
// 面试建议:
// - 先说预处理思路(最容易解释), 写 DFS 一趟检查(代码更短)
// - 提到 BFS 作为避免栈溢出的替代方案
//
// ============================================================
// 【易错点】
//
// 1. 短路求值导致 DFS 不完整
//    ✗ return grid1[i][j]==1 && dfs(i+1,j) && dfs(i-1,j) && ...
//      → grid1[i][j]==0 时后面的 dfs 全被跳过, 岛屿标记不完整
//    ✓ bool ok = grid1[i][j]==1;
//      ok = dfs(i+1,j) && ok;  // dfs() 先执行, ok 后判断
//
// 2. 发现不满足就提前 return
//    ✗ if (grid1[i][j]==0) return false;  // 整个岛剩余部分未标记
//    ✓ 设 flag, 继续 DFS 标记完整个岛, 最后返回 flag
//
// 3. BFS 中出队才标记
//    ✗ auto [x,y] = q.front(); q.pop(); grid2[x][y]=0;
//      → 同一格子被多个邻居入队
//    ✓ grid2[nx][ny]=0; q.push({nx,ny});
//      → 入队时就标记
//
// 4. 预处理法中只标记单格而非整个岛
//    ✗ if (grid1[i][j]==0) grid2[i][j]=0;  // 只沉了一个格子
//    ✓ if (grid1[i][j]==0 && grid2[i][j]==1) sink(grid2, i, j);
//      → DFS 沉掉整个岛
//
// ============================================================
// 【面试追问】
//
// Q1: 为什么不能在 DFS 中提前 return false?
//     → 未标记的格子会在后续被当成新岛屿再处理,
//       导致一个岛被拆成多个, 计数错误
//
// Q2: DFS 递归深度最坏多少?
//     → 500*500 = 250000, 蛇形路径时递归这么深。
//       可能栈溢出。改用 BFS 避免。
//
// Q3: 预处理法为什么正确?
//     → 如果 grid2 岛屿中有任何格子在 grid1 中是水,
//       Step1 会从那个格子出发淹掉整个岛。
//       Step2 中剩下的岛, 保证每个格子在 grid1 中都是陆地,
//       即全是子岛屿。
//
// Q4: 和 LC200 岛屿数量相比, 核心区别是什么?
//     → 框架完全一样 (DFS/BFS 遍历连通分量)。
//       区别是本题需要在遍历时检查附加条件 (grid1 对应位置)。
//       DFS 函数从 void → bool, 或者用预处理法分离条件。
//
// Q5: 如果不允许修改输入 grid2 怎么办?
//     → 用额外的 bool visited[m][n] 数组, 空间 O(m*n)。
//       或者先拷贝 grid2, 在副本上操作。
// ============================================================
