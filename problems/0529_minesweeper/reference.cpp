/*
 * 【题目本质】
 * 模拟扫雷游戏点击操作：矩阵上的连通区域展开问题
 * 点击地雷→游戏结束；点击空白→统计相邻地雷数，为0则递归展开
 *
 * 【解法总览】
 * 解法一：DFS 递归 ⭐推荐  — O(m*n) / O(m*n)
 * 解法二：BFS 队列          — O(m*n) / O(m*n)
 */

// ===================== 解法一：DFS 递归（推荐） =====================
// 思路：从点击位置出发，统计周围地雷数
//       有地雷→标数字，无地雷→标'B'并递归展开8方向的'E'
class Solution {
public:
    // 8个方向：上下左右 + 4个对角线
    int dirs[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};

    vector<vector<char>> updateBoard(vector<vector<char>>& board, vector<int>& click) {
        int r = click[0], c = click[1];
        // 点击到地雷，游戏结束
        if (board[r][c] == 'M') {
            board[r][c] = 'X';
            return board;
        }
        dfs(board, r, c);
        return board;
    }

    void dfs(vector<vector<char>>& board, int r, int c) {
        int m = board.size(), n = board[0].size();
        // 统计周围地雷数
        int mineCount = 0;
        for (auto& d : dirs) {
            int nr = r + d[0], nc = c + d[1];
            if (nr >= 0 && nr < m && nc >= 0 && nc < n && board[nr][nc] == 'M') {
                mineCount++;
            }
        }
        if (mineCount > 0) {
            // 周围有地雷，标记数字
            board[r][c] = '0' + mineCount;
        } else {
            // 周围无地雷，标记为B并递归展开
            board[r][c] = 'B';
            for (auto& d : dirs) {
                int nr = r + d[0], nc = c + d[1];
                if (nr >= 0 && nr < m && nc >= 0 && nc < n && board[nr][nc] == 'E') {
                    dfs(board, nr, nc);
                }
            }
        }
    }
};

// ===================== 解法二：BFS 队列 =====================
// 思路：用队列逐层展开，入队前标记防止重复
class Solution2 {
public:
    vector<vector<char>> updateBoard(vector<vector<char>>& board, vector<int>& click) {
        int m = board.size(), n = board[0].size();
        int r = click[0], c = click[1];
        if (board[r][c] == 'M') {
            board[r][c] = 'X';
            return board;
        }

        int dirs[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
        queue<pair<int,int>> q;
        q.push({r, c});
        board[r][c] = 'B'; // 先标记防止重复入队

        while (!q.empty()) {
            auto [cr, cc] = q.front(); q.pop();
            // 统计周围地雷数
            int mineCount = 0;
            for (auto& d : dirs) {
                int nr = cr + d[0], nc = cc + d[1];
                if (nr >= 0 && nr < m && nc >= 0 && nc < n && board[nr][nc] == 'M')
                    mineCount++;
            }
            if (mineCount > 0) {
                // 有地雷，覆盖之前的'B'标记为数字
                board[cr][cc] = '0' + mineCount;
            } else {
                // 无地雷，展开邻居中的'E'
                for (auto& d : dirs) {
                    int nr = cr + d[0], nc = cc + d[1];
                    if (nr >= 0 && nr < m && nc >= 0 && nc < n && board[nr][nc] == 'E') {
                        board[nr][nc] = 'B'; // 入队前标记
                        q.push({nr, nc});
                    }
                }
            }
        }
        return board;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 优势 |
 * |------|------|------|------|
 * | DFS  | O(m*n) | O(m*n) 递归栈 | 代码简洁 |
 * | BFS  | O(m*n) | O(m*n) 队列   | 不会栈溢出，层序展开 |
 *
 * 【易错点】
 * 1. 方向必须是8个（含对角线），不是4个
 * 2. 只对 'E' 格子递归/入队，已揭开的不再处理
 * 3. BFS 必须在入队前标记，否则同一格子会被多次入队
 * 4. 数字转换：'0' + mineCount，确保是 char 类型
 *
 * 【面试追问】
 * Q1: DFS vs BFS 的选择？
 *     → DFS 简洁但可能栈溢出，BFS 更安全
 * Q2: 如何支持多次点击？
 *     → 每次 updateBoard 只处理一次点击，已揭开的格子自动跳过
 * Q3: 如何统计本次操作揭开的格子数？
 *     → DFS/BFS 中每次修改状态时计数
 */
