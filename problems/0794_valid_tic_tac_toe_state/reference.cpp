/*
 * LeetCode 794: 有效的井字游戏 (Valid Tic-Tac-Toe State)
 * 难度: Medium
 * 标签: Array, Matrix, Logic
 *
 * 【题目本质】
 * 判断给定 3x3 棋盘是否是合法的井字棋中间/结束状态。
 * 核心是分类讨论：验证棋子数量约束 + 获胜状态约束。
 * X 先手，因此 countX 要么等于 countO，要么等于 countO+1。
 * 一方获胜后游戏终止，不允许对方再落子。
 *
 * 【解法总览】
 * 解法一: 分类讨论 - wins辅助函数      O(1) / O(1)  面试首选
 * 解法二: 行列累加值 - 紧凑一次遍历     O(1) / O(1)  代码最短
 */


/*
 * ============================================================
 * 解法一: 分类讨论法（面试首选）
 * ============================================================
 *
 * 思路:
 *   三步走：
 *   Step 1: 统计 X 和 O 的数量，验证 countX == countO 或 countX == countO + 1
 *   Step 2: 分别检查 X 和 O 是否获胜（8条线：3行3列2对角）
 *   Step 3: 验证获胜状态与回合数的一致性
 *
 * 逻辑推导:
 *   X 先手 -> X 总是先下 -> 每个合法状态中 X >= O
 *   X 获胜 -> X 完成第三个连线棋子 -> X 刚下完 -> countX = countO + 1
 *   O 获胜 -> O 完成第三个连线棋子 -> O 刚下完 -> countX = countO
 *   同时获胜 -> 不可能（第一个获胜时游戏就停止了）
 *
 * 示例 1: ["O  ","   ","   "]
 *   countX=0, countO=1 -> 0 != 1 且 0 != 1+1 -> false
 *   (O 不可能在 X 之前下棋)
 *
 * 示例 2: ["XOX","O O","XOX"]
 *   countX=4, countO=3 -> 4 == 3+1 合法
 *   X赢? 检查8条线 -> 否
 *   O赢? 检查8条线 -> 否
 *   -> true (游戏还在进行中)
 *
 * 示例 3: ["XXX","OOO","   "]
 *   countX=3, countO=3 -> 3 == 3 合法
 *   X赢? 第一行全X -> 是
 *   O赢? 第二行全O -> 是
 *   xWin && oWin -> false (不可能同时赢)
 *
 * 示例 4: ["XXX","XOO","OO "]
 *   countX=4, countO=4 -> 4 == 4 合法
 *   X赢? 第一行全X -> 是
 *   xWin && countX != countO+1 -> 4 != 4+1=5 -> true条件触发 -> false
 *   (X赢了之后O又下了一步，非法)
 *
 * 时间: O(1), 空间: O(1)  (棋盘固定 3x3)
 */
class Solution1 {
public:
    bool validTicTacToe(vector<string>& board) {
        int countX = 0, countO = 0;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == 'X') countX++;
                else if (board[i][j] == 'O') countO++;
            }
        }

        // 约束 1: X 先手，数量关系固定
        if (countX != countO && countX != countO + 1) return false;

        bool xWin = wins(board, 'X');
        bool oWin = wins(board, 'O');

        // 约束 2: 不能同时赢 (游戏在第一个获胜者出现时就终止)
        if (xWin && oWin) return false;

        // 约束 3: X 赢了 -> 最后一步是 X 下的 -> countX == countO + 1
        if (xWin && countX != countO + 1) return false;

        // 约束 4: O 赢了 -> 最后一步是 O 下的 -> countX == countO
        if (oWin && countX != countO) return false;

        return true;
    }

private:
    // 检查 player 是否有三连线 (8条线: 3行+3列+2对角)
    bool wins(vector<string>& board, char player) {
        for (int i = 0; i < 3; i++) {
            // 检查第 i 行
            if (board[i][0] == player && board[i][1] == player && board[i][2] == player)
                return true;
            // 检查第 i 列
            if (board[0][i] == player && board[1][i] == player && board[2][i] == player)
                return true;
        }
        // 主对角线 (左上到右下)
        if (board[0][0] == player && board[1][1] == player && board[2][2] == player)
            return true;
        // 副对角线 (右上到左下)
        if (board[0][2] == player && board[1][1] == player && board[2][0] == player)
            return true;

        return false;
    }
};


/*
 * ============================================================
 * 解法二: 行列累加值法 - 紧凑一次遍历
 * ============================================================
 *
 * 思路:
 *   用数值编码棋子: X -> +1, O -> -1, 空 -> 0
 *   维护 rows[3], cols[3], diag, anti 四组累加值
 *   一次遍历同时完成计数和获胜检查
 *
 * 获胜判定:
 *   某行/列/对角累加和 == +3 -> X 占满该线 -> X 获胜
 *   某行/列/对角累加和 == -3 -> O 占满该线 -> O 获胜
 *
 * 优点: 代码紧凑，避免独立的 wins 函数
 *        如果推广到 NxN 棋盘，这种方式可以 O(N^2) 一次遍历完成
 * 缺点: 抽象程度高，面试讲解不如解法一清晰
 *
 * 示例: ["XOX","OXO","XOX"]
 *   rows = [1, -1, 1], cols = [1, -1, 1]
 *   diag = 3 (X赢), anti = -1
 *   countX = 5, countO = 4 -> 5 == 4+1 合法
 *   xWin = true, oWin = false
 *   xWin && countX == countO+1 -> 合法 -> true
 *
 * 时间: O(1), 空间: O(1)
 */
class Solution2 {
public:
    bool validTicTacToe(vector<string>& board) {
        int countX = 0, countO = 0;
        int rows[3] = {}, cols[3] = {};
        int diag = 0, anti = 0;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                int val = 0;
                if (board[i][j] == 'X') { val = 1; countX++; }
                else if (board[i][j] == 'O') { val = -1; countO++; }
                rows[i] += val;
                cols[j] += val;
                if (i == j) diag += val;           // 主对角线: (0,0)(1,1)(2,2)
                if (i + j == 2) anti += val;       // 副对角线: (0,2)(1,1)(2,0)
            }
        }

        // 数量约束: X 先手
        if (countX != countO && countX != countO + 1) return false;

        // 获胜检查: 用 lambda 简化
        auto checkWin = [&](int target) -> bool {
            for (int i = 0; i < 3; i++) {
                if (rows[i] == target || cols[i] == target) return true;
            }
            return diag == target || anti == target;
        };

        bool xWin = checkWin(3);    // +3 表示 X 占满某线
        bool oWin = checkWin(-3);   // -3 表示 O 占满某线

        // 获胜一致性检查 (同解法一)
        if (xWin && oWin) return false;
        if (xWin && countX != countO + 1) return false;
        if (oWin && countX != countO) return false;

        return true;
    }
};


/*
 * ============================================================
 * 【解法对比】
 *
 * | 解法              | 时间 | 空间 | 特点                       |
 * |-------------------|------|------|---------------------------|
 * | 分类讨论 (解法一)   | O(1) | O(1) | 逻辑清晰，wins函数独立       |
 * | 累加值法 (解法二)   | O(1) | O(1) | 一次遍历，代码紧凑           |
 *
 * 面试推荐: 解法一。逻辑清晰，辅助函数独立，方便讲解。
 * 解法二适合代码简洁控，或需要推广到 NxN 的场景。
 *
 * ============================================================
 * 【易错点】
 *
 * 1. 忘记 "X 和 O 不能同时赢":
 *    错: 分别检查 xWin 和 oWin 但没有互斥判断
 *    对: if (xWin && oWin) return false; 必须最先判断
 *
 * 2. 获胜与回合数不匹配:
 *    错: X 赢了但 countX == countO (说明 O 在 X 赢后又下了一步)
 *    对: X 赢时必须 countX == countO + 1
 *        O 赢时必须 countX == countO
 *
 * 3. 只检查数量不检查获胜:
 *    错: 认为数量关系正确就合法
 *    对: "XXX" / "OOO" / "   " 数量 3:3 合法但同时赢 -> 非法
 *
 * 4. 对角线检查遗漏:
 *    错: 只检查主对角线 (0,0)(1,1)(2,2)
 *    对: 副对角线 (0,2)(1,1)(2,0) 同样要检查
 *
 * 5. 解法二中 target 值错误:
 *    错: checkWin(1) 或 checkWin(-1) 只要求一个棋子在线上
 *    对: 必须用 3 和 -3，表示三个位置全部被占
 *
 * ============================================================
 * 【面试追问】
 *
 * Q1: 为什么 X 赢时必须 countX == countO + 1？
 * -> X 先手，获胜的那步一定是 X 下的。如果此时 countX == countO，
 *    说明 O 在 X 赢后又下了一步，违反"获胜即终止"规则。
 *
 * Q2: 为什么 X 和 O 不能同时赢？
 * -> 一方连成三个时游戏就结束了。之后不可能再有人落子。
 *    所以任何合法流程中最多一方获胜。
 *
 * Q3: 推广到 NxN 棋盘且 N 连珠获胜怎么做？
 * -> 数量约束不变。获胜检查用累加值法，一次 O(N^2) 遍历。
 *    rows[N], cols[N], diag, anti 全部扩展到 N。
 *    和为 N 表示 X 赢，和为 -N 表示 O 赢。
 *
 * Q4: 如果要生成所有合法状态呢？
 * -> 从空棋盘 BFS/DFS，X 和 O 交替放置，遇到获胜则停止。
 *    合法状态总数约 5478（3x3 棋盘），可穷举。
 * ============================================================
 */
