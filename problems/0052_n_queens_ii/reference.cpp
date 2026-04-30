/*
 * LeetCode 52: N-Queens II (N 皇后 II)
 *
 * 【题目本质】
 * 在 n×n 棋盘上逐行放置皇后，使得没有两个皇后同行、同列、同对角线。
 * 只需返回合法方案的总数（与 51 题区别：不需要返回具体方案）。
 *
 * 【解法总览】
 * 解法1: 回溯 + 逐个检查冲突  — O(n! × n) / O(n) — 最直觉
 * 解法2: 回溯 + 集合标记冲突  — O(n!)     / O(n) — 面试首选
 * 解法3: 回溯 + 位运算        — O(n!)     / O(n) — 极致优化
 */

// ============================================================
// 解法1: 回溯 + 逐个检查 — 最朴素的做法
// 时间: O(n! × n)  空间: O(n)
//
// 【思路】
// 逐行放皇后，每行尝试每一列。放之前遍历之前所有行已放的皇后，
// 检查是否同列或同对角线。如果不冲突就放下去，递归下一行。
//
// 决策树 (n=4, 部分):
//                    row=0
//         /      |       |      \
//     col=0   col=1   col=2   col=3
//       |       / \     ...     |
//    row1:c2  c0  c3           row1:c0
//       |      ✗   |            |
//    row2:✗      row2:c0      row2:c1
//                  |            ✗
//               row3:c2
//                  ✓ → count++
//
// 最终 n=4 有 2 个解: [1,3,0,2] 和 [2,0,3,1]
// ============================================================
class Solution1 {
public:
    int totalNQueens(int n) {
        int count = 0;
        // queens[row] = col，记录第 row 行皇后放在哪一列
        vector<int> queens(n, -1);
        backtrack(queens, 0, n, count);
        return count;
    }

private:
    void backtrack(vector<int>& queens, int row, int n, int& count) {
        if (row == n) {
            // 所有行都放完了，找到一个合法方案
            count++;
            return;
        }
        for (int col = 0; col < n; col++) {
            if (isValid(queens, row, col)) {
                queens[row] = col;                        // 做选择
                backtrack(queens, row + 1, n, count);     // 递归下一行
                queens[row] = -1;                         // 撤销选择
            }
        }
    }

    bool isValid(vector<int>& queens, int row, int col) {
        // 遍历之前所有行，检查同列和对角线冲突
        for (int i = 0; i < row; i++) {
            // 同列: queens[i] == col
            // 同对角线: 行差 == 列差 (|queens[i]-col| == |i-row|)
            if (queens[i] == col || abs(queens[i] - col) == abs(i - row))
                return false;
        }
        return true;
    }
};

// ============================================================
// 解法2: 回溯 + 集合标记 — O(1) 冲突检测 (面试首选)
// 时间: O(n!)  空间: O(n)
//
// 【思路】
// 解法1 的瓶颈：每次判断冲突要遍历之前所有行，O(n)。
// 优化：用三个集合分别记录被占用的列、主对角线、副对角线。
//
// 关键观察——对角线编号:
//   主对角线: 同一条主对角线上 row - col 值相同
//   副对角线: 同一条副对角线上 row + col 值相同
//
// 验证 (4×4 棋盘):
//   row-col:          row+col:
//    0 -1 -2 -3        0  1  2  3
//    1  0 -1 -2        1  2  3  4
//    2  1  0 -1        2  3  4  5
//    3  2  1  0        3  4  5  6
//
// 这样冲突检测变成 O(1) 的集合查找。
// ============================================================
class Solution2 {
public:
    int totalNQueens(int n) {
        int count = 0;
        unordered_set<int> cols, diag1, diag2;
        backtrack(0, n, cols, diag1, diag2, count);
        return count;
    }

private:
    void backtrack(int row, int n,
                   unordered_set<int>& cols,
                   unordered_set<int>& diag1,
                   unordered_set<int>& diag2,
                   int& count) {
        if (row == n) {
            count++;
            return;
        }
        for (int col = 0; col < n; col++) {
            // O(1) 检测：该列或对角线是否已被占用
            if (cols.count(col) || diag1.count(row - col) || diag2.count(row + col))
                continue; // 冲突，跳过

            // 做选择：标记占用
            cols.insert(col);
            diag1.insert(row - col);  // 主对角线编号
            diag2.insert(row + col);  // 副对角线编号

            backtrack(row + 1, n, cols, diag1, diag2, count);

            // 撤销选择：取消占用
            cols.erase(col);
            diag1.erase(row - col);
            diag2.erase(row + col);
        }
    }
};

// ============================================================
// 解法2b: 回溯 + 布尔数组标记 — 避免哈希开销
// 时间: O(n!)  空间: O(n)
//
// 【思路】
// 与解法2 逻辑完全一样，但用 vector<bool> 替代 unordered_set，
// 避免哈希表的常数开销。
//
// 注意: row - col 可能为负数，所以 diag1 数组大小为 2n-1，
// 索引时加偏移 n-1。
// ============================================================
class Solution2b {
public:
    int totalNQueens(int n) {
        int count = 0;
        vector<bool> cols(n, false);
        vector<bool> diag1(2 * n - 1, false);  // 主对角线: row - col + (n-1) 作为索引
        vector<bool> diag2(2 * n - 1, false);  // 副对角线: row + col 作为索引
        backtrack(0, n, cols, diag1, diag2, count);
        return count;
    }

private:
    void backtrack(int row, int n,
                   vector<bool>& cols,
                   vector<bool>& diag1,
                   vector<bool>& diag2,
                   int& count) {
        if (row == n) {
            count++;
            return;
        }
        for (int col = 0; col < n; col++) {
            int d1 = row - col + n - 1;  // 偏移防止负数索引
            int d2 = row + col;
            if (cols[col] || diag1[d1] || diag2[d2])
                continue;

            cols[col] = diag1[d1] = diag2[d2] = true;   // 标记
            backtrack(row + 1, n, cols, diag1, diag2, count);
            cols[col] = diag1[d1] = diag2[d2] = false;  // 撤销
        }
    }
};

// ============================================================
// 解法3: 回溯 + 位运算 — 极致优化
// 时间: O(n!)  空间: O(n) (递归栈)
//
// 【思路】
// 用三个整数的二进制位表示冲突状态:
//   cols  : 第 k 位为 1 表示第 k 列已被占
//   diag1 : 主对角线冲突传播（每递归一层左移一位）
//   diag2 : 副对角线冲突传播（每递归一层右移一位）
//
// 为什么对角线要移位？
// 画图理解: 皇后在 (row, col)
//   到 row+1 时，主对角线攻击 col-1 位置 → 影响"左移"
//   到 row+1 时，副对角线攻击 col+1 位置 → 影响"右移"
//
//   示例 n=4, 皇后在 (0,1):
//   row=0: cols=0010, d1=0010, d2=0010
//   row=1: cols=0010, d1=0100(左移), d2=0001(右移)
//          occupied = 0010|0100|0001 = 0111
//          avail = ~0111 & 1111 = 1000 → 只有 col=3 可放
//
// lowbit 技巧:
//   pos = avail & (-avail)  提取最低位的 1
//   avail -= pos            去掉该位，处理下一个可用位置
//
// 终止条件:
//   cols == (1<<n)-1 说明所有 n 列都被占 → 放完了所有皇后
// ============================================================
class Solution3 {
public:
    int totalNQueens(int n) {
        int count = 0;
        solve(n, 0, 0, 0, count);
        return count;
    }

private:
    void solve(int n, int cols, int diag1, int diag2, int& count) {
        if (cols == (1 << n) - 1) {
            // 所有列都被占满 → n 个皇后全部放完
            count++;
            return;
        }

        // 计算当前行所有可放的位置 (bit=1 表示可放)
        int avail = ((1 << n) - 1) & ~(cols | diag1 | diag2);

        while (avail) {
            // 取最低位的 1: 选一个可用列
            int pos = avail & (-avail);
            // 去掉这一位，下次循环处理其他可用列
            avail -= pos;

            // 递归下一行:
            // cols | pos        : 标记该列被占
            // (diag1 | pos) << 1: 主对角线影响左移传给下一行
            // (diag2 | pos) >> 1: 副对角线影响右移传给下一行
            solve(n,
                  cols | pos,
                  (diag1 | pos) << 1,
                  (diag2 | pos) >> 1,
                  count);
            // 不需要手动撤销——参数传值，递归返回后自动恢复
        }
    }
};

// ============================================================
// 【解法对比】
//
// | 维度         | 解法1(逐个检查)  | 解法2(集合标记) | 解法3(位运算) |
// |-------------|-----------------|----------------|-------------|
// | 冲突检测     | O(n) 遍历       | O(1) 集合查找   | O(1) 位运算  |
// | 常数因子     | 大              | 中(哈希开销)    | 最小         |
// | 代码可读性   | 最好            | 好             | 需要位运算基础|
// | 面试推荐     | 展示理解        | ⭐首选          | 追问加分     |
//
// 与第51题差异: 51题需要 vector<string> board 记录路径，
// 在 row==n 时把 board 加入 result。52 题只需 count++。
// 回溯框架完全一样，只改了"收集结果"那一行。
//
// ============================================================
// 【易错点】
//
// 1. 对角线索引越界:
//    ✗ vector<bool> diag1(n) — row-col 可能为负，越界!
//    ✓ vector<bool> diag1(2*n-1)，索引用 row-col+n-1
//
// 2. 位运算方向搞反:
//    ✗ 主对角线右移，副对角线左移
//    ✓ 主对角线左移 (<<1)，副对角线右移 (>>1)
//    验证: 皇后在(0,2), 到(1,?)时攻击col=1(左)和col=3(右)
//
// 3. 位运算终止条件:
//    ✗ if (row == n) — 解法3没有传row参数
//    ✓ if (cols == (1<<n)-1) — 所有n列都被占满
//
// 4. 忘记撤销选择(解法1/2):
//    ✗ 只 insert 没有 erase → 回溯后状态错乱
//    ✓ 解法3 用参数传递天然回溯，不需要手动撤销
//
// 5. abs() 检测对角线时忘记取绝对值:
//    ✗ queens[i] - col == i - row — 只检测了一个方向
//    ✓ abs(queens[i] - col) == abs(i - row) — 两个方向都检测
//
// ============================================================
// 【面试追问】
//
// Q1: 和第51题有什么区别？代码改哪里？
// → 51题要返回所有方案的棋盘字符串，需要维护 board 路径。
//   52题只计数，不需要路径。框架一样，只改结果收集部分。
//
// Q2: 时间复杂度为什么是 O(n!) 不是 O(n^n)？
// → 第0行 n 种选择，第1行最多 n-1 种(至少一列被占)，
//   第2行最多 n-2 种... 上界 n×(n-1)×...×1 = n!。
//   实际由于对角线约束，比 n! 更小。
//
// Q3: 位运算版本为什么不需要手动撤销选择？
// → 因为冲突状态通过函数参数传递(值传递)，
//   递归返回后参数自动恢复到调用前的值，天然回溯。
//
// Q4: 能否利用对称性剪枝？
// → 可以。第一行只搜 0~n/2 列，结果×2。
//   如果 n 是奇数，中间列单独计算(不×2)。
//   可以减少约一半的搜索量。
// ============================================================
