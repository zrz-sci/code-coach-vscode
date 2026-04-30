/*
 * LeetCode 348: Design Tic-Tac-Toe
 * 
 * 【题目本质】
 * 每次落子后 O(1) 判断是否有玩家赢了。
 * 本质是增量计数问题：维护行/列/对角线的计数器，
 * 而不是每次重新扫描棋盘。
 *
 * 【解法总览】
 * 解法1: 暴力模拟 — O(n) per move / O(n²) — 存棋盘，每次扫描
 * 解法2: 行列对角线计数器 — O(1) per move / O(n) — +1/-1 技巧 ⭐
 */

// ============================================================
// 解法1: 暴力模拟 — 存储棋盘 + 每次扫描落子所在行/列/对角线
// move(): O(n)  空间: O(n²)
// 
// 【思路】
// 最直觉的做法：把棋盘完整存下来，每次落子后，
// 检查该行是否全是同一玩家、该列是否全是同一玩家、
// 如果在对角线上再检查对角线。
//
// 每次最多检查 4 条线，每条线长 n，所以是 O(n)。
//
// 示例: n=3, 玩家1(X)落子过程
//
//  move(0,0,1):    move(2,2,1):    move(2,0,1):    move(2,1,1):
//  |X| | |        |X| | |        |X| | |        |X| | |
//  | | | |        | | | |        | | | |        | | | |
//  | | | |        | | |X|        |X| |X|        |X|X|X| ← row2全X，赢!
//
// ============================================================
class Solution1_TicTacToe {
    vector<vector<int>> board;
    int n;
public:
    Solution1_TicTacToe(int n) : board(n, vector<int>(n, 0)), n(n) {}
    
    int move(int row, int col, int player) {
        board[row][col] = player;
        
        // 检查第 row 行：遍历该行所有列
        bool win = true;
        for (int j = 0; j < n; j++) {
            if (board[row][j] != player) { win = false; break; }
        }
        if (win) return player;
        
        // 检查第 col 列：遍历该列所有行
        win = true;
        for (int i = 0; i < n; i++) {
            if (board[i][col] != player) { win = false; break; }
        }
        if (win) return player;
        
        // 检查主对角线（仅当落子在主对角线上，即 row == col）
        if (row == col) {
            win = true;
            for (int i = 0; i < n; i++) {
                if (board[i][i] != player) { win = false; break; }
            }
            if (win) return player;
        }
        
        // 检查副对角线（仅当落子在副对角线上，即 row + col == n - 1）
        // 注意：不能用 else if，因为中心格同时在两条对角线上
        if (row + col == n - 1) {
            win = true;
            for (int i = 0; i < n; i++) {
                if (board[i][n - 1 - i] != player) { win = false; break; }
            }
            if (win) return player;
        }
        
        return 0;
    }
};

// ============================================================
// 解法2: 行列对角线计数器 — O(1) per move ⭐ 面试首选
// move(): O(1)  空间: O(n)
//
// 【思路】
// 解法1 的瓶颈：每次要遍历一行/列来判断是否全是同一玩家。
// 但我们真正需要的信息只是"这行有几个该玩家的棋子"。
//
// 核心优化：用计数器替代遍历。
// - rows[i]: 第 i 行的累计值
// - cols[j]: 第 j 列的累计值
// - diag:    主对角线的累计值
// - anti_diag: 副对角线的累计值
//
// +1/-1 技巧：
// - 玩家1 落子 → 计数器 +1
// - 玩家2 落子 → 计数器 -1
// - 当某计数器绝对值 == n → 有人赢了
//
// 为什么正确？
// 如果一行中既有玩家1又有玩家2的棋子，+1和-1会互相抵消，
// 绝对值永远到不了 n。只有某人独占整条线，才能达到 ±n。
//
// 示例: n=3 的计数器变化
//
//  行:  rows = [0, 0, 0]     列:  cols = [0, 0, 0]
//  对角线: diag = 0           副对角线: anti_diag = 0
//
//  move(0,0,1): delta=+1
//    rows[0]=1, cols[0]=1, diag=1 (因为0==0), 无副对角线
//
//  move(0,2,2): delta=-1
//    rows[0]=0, cols[2]=-1, anti_diag=-1 (因为0+2==2==n-1)
//
//  move(2,2,1): delta=+1
//    rows[2]=1, cols[2]=0, diag=2 (因为2==2)
//
//  move(1,1,2): delta=-1
//    rows[1]=-1, cols[1]=-1, diag=1 (因为1==1), anti_diag=-2 (因为1+1==2)
//
//  move(2,0,1): delta=+1
//    rows[2]=2, cols[0]=2, anti_diag=-1 (因为2+0==2)
//
//  move(1,0,2): delta=-1
//    rows[1]=-2, cols[0]=1
//
//  move(2,1,1): delta=+1
//    rows[2]=3 → |3| == n → 玩家1赢! 返回 1
//
// ============================================================
class TicTacToe {
    vector<int> rows, cols;
    int diag, anti_diag;
    int n;
public:
    TicTacToe(int n) : rows(n, 0), cols(n, 0), diag(0), anti_diag(0), n(n) {}
    
    int move(int row, int col, int player) {
        // 玩家1 → +1, 玩家2 → -1
        // 这样两人共用一套计数器，不需要分别维护
        int delta = (player == 1) ? 1 : -1;
        
        // 更新行计数器
        rows[row] += delta;
        
        // 更新列计数器
        cols[col] += delta;
        
        // 主对角线：row == col 的格子才在主对角线上
        if (row == col) {
            diag += delta;
        }
        
        // 副对角线：row + col == n - 1 的格子才在副对角线上
        // 注意：不能用 else if！当 n 为奇数时，中心格 (n/2, n/2)
        // 同时满足两个条件，必须都更新
        if (row + col == n - 1) {
            anti_diag += delta;
        }
        
        // 判断胜负：任一计数器的绝对值达到 n
        if (abs(rows[row]) == n || abs(cols[col]) == n ||
            abs(diag) == n || abs(anti_diag) == n) {
            return player;  // 返回赢家编号，不是返回 delta 的符号
        }
        
        return 0;
    }
};

// ============================================================
// 【解法对比】
//
// | 维度         | 解法1 暴力模拟  | 解法2 计数器     |
// |-------------|----------------|-----------------|
// | move() 时间  | O(n)           | O(1) ⭐         |
// | 空间         | O(n²) 存棋盘    | O(n) 仅计数器    |
// | 是否存棋盘    | 是              | 否              |
// | 支持回溯/展示 | 是              | 需额外记录       |
// | 面试价值      | 说明理解题意     | 展示优化能力 ⭐  |
//
// 面试中：先口述解法1思路（30秒），直接写解法2代码。
//
// ============================================================
// 【易错点】
//
// 1. 对角线用 else if 连接 → 中心格只更新一条对角线
//    ✗ if (row == col) { diag += delta; }
//      else if (row + col == n-1) { anti_diag += delta; }
//    ✓ 两个独立的 if
//
// 2. delta 的计算方式错误
//    ✗ int delta = player;  // 玩家2时 delta=2，不是-1
//    ✓ int delta = (player == 1) ? 1 : -1;
//
// 3. 返回值写成 return 1 而不是 return player
//    ✗ if (...) return 1;  // 玩家2赢时也返回了1
//    ✓ if (...) return player;
//
// 4. 副对角线条件写错
//    ✗ row - col == n - 1  或  col - row == n - 1
//    ✓ row + col == n - 1
//
// ============================================================
// 【面试追问】
//
// Q1: 暴力解 move() 复杂度？瓶颈？
//   → O(n)，瓶颈在遍历行/列。只需要计数信息，不需要具体内容。
//
// Q2: O(1) 怎么做？两玩家共用计数器不会冲突吗？
//   → +1/-1 技巧。只有一个玩家独占一条线时绝对值才能到 n，
//     不会冲突——有对手棋子就会"抵消"，绝对值到不了 n。
//
// Q3: 如果要支持 undo() 撤销操作？
//   → 用栈记录历史 (row, col, player)，undo 时取反 delta 即可。
//     计数器天然支持可逆操作。
//
// Q4: 如果是 m×n 矩形棋盘，胜利条件是连续 k 个？
//   → 简单计数器不再适用（一行内可能只需部分连续），
//     需要对每个方向维护连续段长度，类似五子棋判断，
//     每次 move 检查四个方向的连续数，O(k) per move。
//
// ============================================================
