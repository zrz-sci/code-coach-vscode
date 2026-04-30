/*
 * LeetCode 419: 棋盘上的战舰 (Battleships in a Board)
 *
 * 【题目本质】
 * 计数矩阵中直线形连通块（战舰）的数量。
 * 战舰只能水平或垂直放置，且两艘战舰之间至少隔一个空格。
 *
 * 【解法总览】
 * 解法1: DFS Flood Fill    — O(m×n) / O(m×n) — 经典岛屿计数模板
 * 解法2: 数战舰头部(一次扫描) — O(m×n) / O(1)   — 利用题目特殊约束，最优
 */

// ============================================================
// 解法1: DFS Flood Fill — 经典连通块计数
// 时间: O(m×n)  空间: O(m×n) visited 数组 + O(min(m,n)) 递归栈
//
// 【思路】
// 这道题如果忽略"战舰只能直线"的约束，本质就是 LeetCode 200 岛屿数量。
// 遍历每个格子，遇到未访问的 'X'，就 DFS 把整艘战舰标记为已访问，
// 计数 +1。
//
// 矩阵示意 (示例1):
//   X . . X
//   . . . X
//   . . . X
//
// DFS 过程:
//   (0,0): 'X' 未访问 → DFS 标记 (0,0)，无相邻X → count=1
//   (0,3): 'X' 未访问 → DFS 标记 (0,3)→(1,3)→(2,3) → count=2
//   其余所有 'X' 已被访问，跳过
//   结果: 2
// ============================================================
class Solution1 {
public:
    int countBattleships(vector<vector<char>>& board) {
        int m = board.size(), n = board[0].size();
        vector<vector<bool>> visited(m, vector<bool>(n, false));
        int count = 0;
        
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (board[i][j] == 'X' && !visited[i][j]) {
                    dfs(board, visited, i, j, m, n);
                    count++;
                }
            }
        }
        return count;
    }

private:
    // 方向数组: 上下左右
    int dx[4] = {0, 0, 1, -1};
    int dy[4] = {1, -1, 0, 0};
    
    void dfs(vector<vector<char>>& board, vector<vector<bool>>& visited,
             int i, int j, int m, int n) {
        // 越界、不是战舰、已访问 → 返回
        if (i < 0 || i >= m || j < 0 || j >= n) return;
        if (board[i][j] != 'X' || visited[i][j]) return;
        
        visited[i][j] = true;  // 标记已访问，防止重复计数和无限递归
        
        for (int d = 0; d < 4; d++) {
            dfs(board, visited, i + dx[d], j + dy[d], m, n);
        }
    }
};

// ============================================================
// 解法1b: DFS 修改原矩阵 — 省掉 visited 数组
// 时间: O(m×n)  空间: O(min(m,n)) 递归栈
//
// 【思路】
// 和解法1相同，但直接把访问过的 'X' 改成 '.'，
// 省掉 O(m×n) 的 visited 数组。
// 缺点：修改了输入 board，不满足 Follow-up 要求。
// ============================================================
class Solution1b {
public:
    int countBattleships(vector<vector<char>>& board) {
        int m = board.size(), n = board[0].size();
        int count = 0;
        
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (board[i][j] == 'X') {
                    dfs(board, i, j, m, n);
                    count++;
                }
            }
        }
        return count;
    }

private:
    void dfs(vector<vector<char>>& board, int i, int j, int m, int n) {
        if (i < 0 || i >= m || j < 0 || j >= n) return;
        if (board[i][j] != 'X') return;
        
        board[i][j] = '.';  // 直接改原矩阵当作 visited 标记
        dfs(board, i + 1, j, m, n);
        dfs(board, i - 1, j, m, n);
        dfs(board, i, j + 1, m, n);
        dfs(board, i, j - 1, m, n);
    }
};

// ============================================================
// 解法2: 数战舰头部（一次扫描）— Follow-up 最优解
// 时间: O(m×n)  空间: O(1)，不修改 board
//
// 【思路】
// 解法1 的"问题"在于需要额外空间或修改 board。
// 
// 关键观察: 战舰只能是水平或垂直的直线，且互不相邻。
// 所以每艘战舰恰好有一个"头部"——最左上角的 'X'。
// 头部特征: 上方不是 'X'，左方也不是 'X'。
// 
// 为什么正确？
// - 水平战舰 X X X: 只有最左的 X 左方不是 X → 计数1次
// - 垂直战舰:       只有最上的 X 上方不是 X → 计数1次
//     X
//     X
//     X
// - 单个 X:          上方、左方都不是 X → 计数1次
// - 不会有 L/T 形:  题目保证，所以不会重复
//
// 遍历示意:
//   col:  0 1 2 3
//  row 0: X . . X     (0,0): X, 上=边界, 左=边界 → 头部! count=1
//  row 1: . . . X     (0,3): X, 上=边界, 左='.'  → 头部! count=2
//  row 2: . . . X     (1,3): X, 上='X'           → 跳过 (属于战舰2的中间)
//                      (2,3): X, 上='X'           → 跳过 (属于战舰2的尾部)
//  结果: count = 2 ✓
// ============================================================
class Solution2 {
public:
    int countBattleships(vector<vector<char>>& board) {
        int m = board.size(), n = board[0].size();
        int count = 0;
        
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                // 跳过空位
                if (board[i][j] != 'X') continue;
                
                // 上方有 'X' → 当前格是垂直战舰的非头部，跳过
                if (i > 0 && board[i - 1][j] == 'X') continue;
                
                // 左方有 'X' → 当前格是水平战舰的非头部，跳过
                if (j > 0 && board[i][j - 1] == 'X') continue;
                
                // 通过所有检查 → 这是一艘战舰的头部
                count++;
            }
        }
        return count;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法          | 时间    | 空间    | 修改board | 适用范围              |
// |-------------- |---------|---------|----------|-----------------------|
// | DFS+visited   | O(m×n)  | O(m×n)  | 否       | 任意形状连通块(通用)   |
// | DFS+改board   | O(m×n)  | O(递归) | 是       | 任意形状连通块(通用)   |
// | 数头部        | O(m×n)  | O(1)    | 否       | 仅限直线+不相邻(本题)  |
//
// 面试策略:
// - 先提 DFS 展示基本功 → 面试官问 Follow-up → 给出"数头部"拿高分
// - "数头部"法巧妙但不通用，要能说清楚为什么本题可以用
//
// 【易错点】
// 1. 边界越界: 检查 board[i-1][j] 前必须确认 i > 0，
//    否则 i=0 时 board[-1][j] 越界。
//    ✗ if (board[i-1][j] == 'X') continue;  // i=0 时崩溃
//    ✓ if (i > 0 && board[i-1][j] == 'X') continue;
//
// 2. 逻辑方向搞反: 头部是上方和左方"都没有"X 才计数，
//    不是"有"X 才计数。
//    ✗ if (上方是X || 左方是X) count++;      // 完全反了
//    ✓ if (上方是X) continue;                 // 排除非头部
//      if (左方是X) continue;
//      count++;                               // 剩下的才是头部
//
// 3. DFS 中忘记标记导致无限递归:
//    ✗ 先递归再标记 visited → 可能被其他路径再次进入
//    ✓ 进入函数后立即 visited[i][j] = true
//
// 【面试追问】
// Q1: 这道题和"岛屿数量"(200)有什么联系和区别？
// → 联系: 都是矩阵上计数连通块。DFS/BFS 模板可以直接复用。
//   区别: 本题战舰形状受限(直线+不相邻)，允许更巧妙的 O(1) 解法。
//
// Q2 (Follow-up): 能否 O(1) 空间、不修改 board？
// → 可以。数"头部"法: 'X' 且上方、左方都不是 'X' 才计数。
//   正确性依赖于"直线+不相邻"的约束。
//
// Q3: 如果战舰可以是任意形状，O(1) 解法还能用吗？
// → 不能。任意形状的连通块可能有多个"左上角"，
//   必须用 DFS/BFS + visited 来标记整个连通块。
//
// Q4: 如果要输出每艘战舰的起止坐标呢？
// → 找到头部后，向右探测到非 X 得到列范围，向下探测得到行范围。
//   仍然 O(1) 额外空间(不算输出)。
// ============================================================
