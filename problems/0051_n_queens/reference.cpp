/*
 * LeetCode 51: N-Queens (N 皇后)
 *
 * 【题目本质】
 * 在 n×n 棋盘上放 n 个皇后使其互不攻击，本质是"列号的全排列 + 对角线约束"的回溯搜索。
 * 每行恰好放一个皇后，核心在于高效判断列和对角线冲突。
 *
 * 【解法总览】
 * 解法1: 回溯 + 暴力冲突检查   — O(n! × n) / O(n²) — 最直觉
 * 解法2: 回溯 + 集合/数组优化  — O(n!)     / O(n²) — 面试首选 ⭐
 * 解法3: 回溯 + 位运算优化     — O(n!)     / O(n²) — 极致优化
 */

// ============================================================
// 解法1: 回溯 + 暴力冲突检查
// 时间: O(n! × n)  空间: O(n²) (棋盘字符串)
//
// 【思路】
// 关键观察：n 个皇后 + n 行 + 不能同行 → 每行恰好一个皇后。
// 逐行放置，每行尝试每列，放之前遍历已放的 0~row-1 行检查冲突。
// 冲突条件：同列 或 在同一条对角线上(行差的绝对值==列差的绝对值)。
//
// 决策树 (n=4):
//                              row=0
//                /         |          |         \
//            col=0      col=1      col=2      col=3
//              Q...      .Q..       ..Q.       ...Q
//            / | | \    / | | \    ...         ...
//         row=1尝试   row=1尝试
//         c0✗ c1✗    c0✗ c2✗
//         c2✓ c3✗    c3✓
//         ...         ...
//
// 完整展开后 n=4 只有 2 个合法解:
//   解1: .Q.. / ...Q / Q... / ..Q.
//   解2: ..Q. / Q... / ...Q / .Q..
// ============================================================
class Solution1 {
public:
    vector<vector<string>> solveNQueens(int n) {
        vector<vector<string>> result;
        vector<int> queens(n, -1);  // queens[i] = 第i行皇后所在列
        backtrack(result, queens, n, 0);
        return result;
    }

private:
    void backtrack(vector<vector<string>>& result, vector<int>& queens, int n, int row) {
        if (row == n) {
            result.push_back(buildBoard(queens, n));
            return;
        }
        for (int col = 0; col < n; col++) {
            if (isValid(queens, row, col)) {
                queens[row] = col;                          // 做选择
                backtrack(result, queens, n, row + 1);      // 递归下一行
                queens[row] = -1;                           // 撤销选择
            }
        }
    }

    // 暴力检查：遍历前面所有行
    // 为什么只检查前面的行？因为我们逐行放置，后面的行还没放
    bool isValid(vector<int>& queens, int row, int col) {
        for (int i = 0; i < row; i++) {
            // 同列：queens[i] == col
            // 同对角线：行差 == 列差（绝对值）
            if (queens[i] == col || abs(queens[i] - col) == abs(i - row)) {
                return false;
            }
        }
        return true;
    }

    vector<string> buildBoard(vector<int>& queens, int n) {
        vector<string> board(n, string(n, '.'));
        for (int i = 0; i < n; i++) {
            board[i][queens[i]] = 'Q';
        }
        return board;
    }
};


// ============================================================
// 解法2: 回溯 + 集合/数组优化（面试首选 ⭐）
// 时间: O(n!)  空间: O(n²) (棋盘字符串)
//
// 【思路】
// 解法1的瓶颈：每次 isValid 要遍历前面所有行，O(n) 检查。
// 优化：维护三个 bool 数组分别记录已占用的列、主对角线(↘)、副对角线(↙)。
// 冲突判断从 O(n) 降到 O(1)。
//
// 对角线编号的数学基础 (以 n=4 为例):
//
//   主对角线 ↘ 编号 = row - col + (n-1)    副对角线 ↙ 编号 = row + col
//   
//   row\col  0  1  2  3                     row\col  0  1  2  3
//     0      3  2  1  0                       0      0  1  2  3
//     1      4  3  2  1                       1      1  2  3  4
//     2      5  4  3  2                       2      2  3  4  5
//     3      6  5  4  3                       3      3  4  5  6
//
//   主对角线共 2n-1 = 7 条，编号 0~6
//   副对角线共 2n-1 = 7 条，编号 0~6
//
// 为什么 row-col 能标识主对角线？
// → 沿 ↘ 方向走，row+1 且 col+1，差值 row-col 不变。
// 为什么 row+col 能标识副对角线？
// → 沿 ↙ 方向走，row+1 且 col-1，和值 row+col 不变。
// ============================================================
class Solution2 {
public:
    vector<vector<string>> solveNQueens(int n) {
        vector<vector<string>> result;
        vector<int> queens(n, -1);
        vector<bool> colUsed(n, false);
        vector<bool> diag1(2 * n - 1, false);  // 主对角线 ↘
        vector<bool> diag2(2 * n - 1, false);  // 副对角线 ↙
        backtrack(result, queens, n, 0, colUsed, diag1, diag2);
        return result;
    }

private:
    void backtrack(vector<vector<string>>& result, vector<int>& queens,
                   int n, int row,
                   vector<bool>& colUsed, vector<bool>& diag1, vector<bool>& diag2) {
        if (row == n) {
            result.push_back(buildBoard(queens, n));
            return;
        }
        for (int col = 0; col < n; col++) {
            int d1 = row - col + n - 1;  // 主对角线编号，加 n-1 避免负索引
            int d2 = row + col;           // 副对角线编号

            // O(1) 冲突判断 — 这是相比解法1的核心优化点
            if (colUsed[col] || diag1[d1] || diag2[d2]) continue;

            // 做选择：放置皇后并标记三个约束
            queens[row] = col;
            colUsed[col] = diag1[d1] = diag2[d2] = true;

            backtrack(result, queens, n, row + 1, colUsed, diag1, diag2);

            // 撤销选择：必须把三个标记都恢复！
            queens[row] = -1;
            colUsed[col] = diag1[d1] = diag2[d2] = false;
        }
    }

    vector<string> buildBoard(vector<int>& queens, int n) {
        vector<string> board(n, string(n, '.'));
        for (int i = 0; i < n; i++) {
            board[i][queens[i]] = 'Q';
        }
        return board;
    }
};


// ============================================================
// 解法3: 回溯 + 位运算优化
// 时间: O(n!)  空间: O(n²) (棋盘字符串，位运算本身 O(1))
//
// 【思路】
// 用整数的二进制位代替 bool 数组：
//   cols:  第 k 位为 1 表示第 k 列被占用
//   diag1: 主对角线的攻击位图（每层右移1位）
//   diag2: 副对角线的攻击位图（每层左移1位）
//
// 核心操作：
//   available = ~(cols | diag1 | diag2) & ((1<<n)-1)
//   → 所有为1的位就是当前行可以放皇后的列
//
//   pos = available & (-available)  → 取出最低位的1（选一个可用列）
//   col = __builtin_ctz(pos)       → 该位是第几列
//
// 为什么对角线要移位？
// 以主对角线 ↘ 为例：
//   如果 row=0 在 col=1 放了皇后（pos = 0b0010）
//   到 row=1，这个皇后沿 ↘ 攻击 col=2 → 0b0010 右移变 0b0100... 
//   等等，这里需要仔细想。
//
//   实际上我们存储的是"当前行被对角线攻击的列"：
//   row=0 放 col=1 → 主对角线在 row=1 攻击 col=2 → 0b0010 << 1 = 0b0100
//   不对... 让我用一种更清晰的方式：
//
//   我们从"传到下一行时，攻击范围如何变化"的角度理解：
//   - 列攻击：不变（cols 直接传递）
//   - 主对角线 ↘：每下一行，攻击位置向右偏移一列 → 左移1位
//   - 副对角线 ↙：每下一行，攻击位置向左偏移一列 → 右移1位
//
// 位运算示例 (n=4):
//   假设 row=0 放在 col=1:
//   cols  = 0010
//   diag1 = 0010 → 传到 row=1: (0010 << 1) = 0100  (攻击 col=2)
//   diag2 = 0010 → 传到 row=1: (0010 >> 1) = 0001  (攻击 col=0)
//   available@row1 = ~(0010 | 0100 | 0001) & 1111
//                  = ~(0111) & 1111
//                  = 1000 & 1111 = 1000  → 只有 col=3 可用
// ============================================================
class Solution3 {
public:
    vector<vector<string>> solveNQueens(int n) {
        vector<vector<string>> result;
        vector<int> queens(n, -1);
        backtrack(result, queens, n, 0, 0, 0, 0);
        return result;
    }

private:
    void backtrack(vector<vector<string>>& result, vector<int>& queens,
                   int n, int row, int cols, int diag1, int diag2) {
        if (row == n) {
            result.push_back(buildBoard(queens, n));
            return;
        }

        // 可用位置 = 不被任何攻击覆盖的列
        // ~(cols | diag1 | diag2) 取反得到空闲位
        // & ((1<<n)-1) 只保留低 n 位（忽略高位的1）
        int available = ~(cols | diag1 | diag2) & ((1 << n) - 1);

        while (available) {
            int pos = available & (-available);     // 提取最低位的1
            int col = __builtin_ctz(pos);           // 计算是第几位（即列号）

            queens[row] = col;
            // 递归时更新三个约束：
            //   cols | pos: 标记该列已用
            //   (diag1 | pos) << 1: 主对角线攻击范围向左移一位（下一行攻击更右的列）
            //   (diag2 | pos) >> 1: 副对角线攻击范围向右移一位（下一行攻击更左的列）
            backtrack(result, queens, n, row + 1,
                      cols | pos,
                      (diag1 | pos) << 1,
                      (diag2 | pos) >> 1);
            queens[row] = -1;

            available &= (available - 1);           // 去掉最低位的1，尝试下一个可用列
        }
    }

    vector<string> buildBoard(vector<int>& queens, int n) {
        vector<string> board(n, string(n, '.'));
        for (int i = 0; i < n; i++) {
            board[i][queens[i]] = 'Q';
        }
        return board;
    }
};


// ============================================================
// 【解法对比】
//
// | 维度         | 解法1 暴力检查    | 解法2 集合优化     | 解法3 位运算      |
// |-------------|------------------|-------------------|------------------|
// | 冲突判断     | O(n) 遍历已放行   | O(1) 查 bool 数组  | O(1) 位运算       |
// | 枚举可选列   | 逐列 O(n)        | 逐列 O(n)         | 直接算出可用位     |
// | 额外空间     | O(n) queens      | O(n) 三个 bool 数组| O(1) 三个 int     |
// | 代码难度     | 最简单            | 适中              | 需理解位运算       |
// | 面试推荐     | 开场讲            | ⭐ 主力            | 追问时展示         |
//
// 三种解法搜索树完全相同（都是约 n! 个节点），区别在每个节点的操作开销。
//
// ============================================================
// 【易错点】
//
// 1. 主对角线编号忘加偏移量:
//    ✗ diag1[row - col]  →  当 row < col 时索引为负，数组越界！
//    ✓ diag1[row - col + n - 1]  → 映射到 [0, 2n-2]
//
// 2. 位运算对角线移位方向搞反:
//    ✗ diag1 右移, diag2 左移
//    ✓ diag1 (主对角线↘) 左移, diag2 (副对角线↙) 右移
//    → 建议画 4×4 格子手动验证某个皇后的攻击传播方向
//
// 3. 回溯时漏撤销:
//    ✗ 只设 colUsed[col]=true，递归后忘了设回 false
//    ✓ 做选择→递归→撤销选择，三步缺一不可
//
// 4. 构造结果时机:
//    ✗ 在回溯中直接操作 vector<string> board（修改/撤销容易出错）
//    ✓ 用 queens[i] 存列号，到 row==n 时一次性构造字符串
//
// ============================================================
// 【面试追问】
//
// Q1: 为什么逐行放置能穷举所有解？
// → n皇后+n行+不能同行 → 每行恰好一个。逐行决策穷举了所有合法排列。
//
// Q2: 时间复杂度为什么是 O(n!) 而非 O(n^n)？
// → 列不能重复（同列冲突），所以上界是排列数 n!。
//    加上对角线剪枝，实际搜索空间远小于 n!。
//
// Q3: 如果只要解的个数（LeetCode 52），怎么改？
// → 去掉 queens 数组和 buildBoard，换成 count++ 即可。
//    位运算版最干净，因为不需要记录具体放置位置。
//
// Q4: 对角线移位的方向怎么记忆？
// → 画图。在第 row 行 col 列放皇后，到 row+1 行：
//    主对角线 ↘ 攻击 col+1 → 位模式往高位移 → 左移
//    副对角线 ↙ 攻击 col-1 → 位模式往低位移 → 右移
//
// Q5: n 最大只有 9，位运算优化意义大吗？
// → 对于这道题实际意义不大（n=9 时三种解法都是毫秒级）。
//    但位运算技巧本身很通用，在更大规模的约束满足问题中非常有价值。
//    面试中能展示这个解法说明对位运算的理解很深。
// ============================================================
