/*
 * LeetCode 37: 解数独 (Sudoku Solver)
 * 
 * 【题目本质】
 * 约束满足问题 (CSP)：在 9×9 网格中用回溯搜索填入数字，
 * 使得每行、每列、每个 3×3 宫格内 1-9 各出现一次。
 *
 * 【解法总览】
 * 解法1: 基础回溯(扫描验证)     — O(9^m) / O(m)  — 最直觉
 * 解法2: 回溯+布尔约束数组       — O(9^m) / O(1)  — ⭐ 面试首选
 * 解法3: 回溯+位运算+MRV启发式   — O(9^m) / O(1)  — 极致优化
 */

// ============================================================
// 解法1: 基础回溯 — 每次扫描行/列/宫格验证合法性
// 时间: O(9^m)  空间: O(m) 递归栈，m 为空格数
//
// 【思路】
// 最朴素的方式：遍历 board 找到第一个空格，尝试填入 '1'-'9'，
// 每个数字用 isValid 函数扫描同行/同列/同宫格来检查是否冲突。
// 合法就递归处理下一个空格，如果递归失败就撤销选择。
//
// 决策树示意（3 个空格，每个假设有 2-3 个合法选择）:
//                     []
//              /      |      \
//        格1填2   格1填5   格1填8
//         / \        |
//    格2填3 格2填7  格2填1
//      |       |      |
//    格3填9  格3填4  格3填...
//      ✓       ✓
//   找到解就 return true，停止搜索
// ============================================================
class Solution1 {
public:
    void solveSudoku(vector<vector<char>>& board) {
        solve(board);
    }
    
    bool solve(vector<vector<char>>& board) {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (board[i][j] != '.') continue;
                
                for (char c = '1'; c <= '9'; c++) {
                    if (isValid(board, i, j, c)) {
                        board[i][j] = c;                    // 做选择
                        if (solve(board)) return true;      // 找到解就停
                        board[i][j] = '.';                   // 撤销选择
                    }
                }
                // 1-9 都不行 → 前面的选择有问题 → 回溯
                return false;
            }
        }
        // 所有格子都已填满 → 找到解
        return true;
    }
    
    bool isValid(vector<vector<char>>& board, int row, int col, char c) {
        for (int i = 0; i < 9; i++) {
            // 检查同行
            if (board[row][i] == c) return false;
            // 检查同列
            if (board[i][col] == c) return false;
            // 检查同宫格: 将 i 映射为宫格内的 (行偏移, 列偏移)
            int bi = (row / 3) * 3 + i / 3;
            int bj = (col / 3) * 3 + i % 3;
            if (board[bi][bj] == c) return false;
        }
        return true;
    }
};

// ============================================================
// 解法2: 回溯 + 布尔约束数组 — O(1) 合法性检查
// 时间: O(9^m)  空间: O(1) 额外（三个 9×9 bool 数组固定大小）
//
// 【思路】
// 解法1 的 isValid 每次扫描 27 个格子，在频繁调用时浪费时间。
// 维护三个约束数组：
//   rowUsed[i][d] = true  → 第 i 行已有数字 d+1
//   colUsed[j][d] = true  → 第 j 列已有数字 d+1
//   boxUsed[b][d] = true  → 第 b 个宫格已有数字 d+1
// 其中 b = (i/3)*3 + j/3
//
// 预处理：遍历 board，对已有数字标记约束。
// 回溯时只需 O(1) 检查三个数组即可判断合法性。
//
// 宫格编号映射:
//   0 0 0 | 1 1 1 | 2 2 2
//   0 0 0 | 1 1 1 | 2 2 2
//   0 0 0 | 1 1 1 | 2 2 2
//   ------+-------+------
//   3 3 3 | 4 4 4 | 5 5 5
//   3 3 3 | 4 4 4 | 5 5 5
//   3 3 3 | 4 4 4 | 5 5 5
//   ------+-------+------
//   6 6 6 | 7 7 7 | 8 8 8
//   6 6 6 | 7 7 7 | 8 8 8
//   6 6 6 | 7 7 7 | 8 8 8
// ============================================================
class Solution2 {
public:
    bool rowUsed[9][9];
    bool colUsed[9][9];
    bool boxUsed[9][9];
    
    void solveSudoku(vector<vector<char>>& board) {
        memset(rowUsed, false, sizeof(rowUsed));
        memset(colUsed, false, sizeof(colUsed));
        memset(boxUsed, false, sizeof(boxUsed));
        
        // 预处理：记录已有数字的约束
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (board[i][j] != '.') {
                    int d = board[i][j] - '1';
                    int b = (i / 3) * 3 + j / 3;
                    rowUsed[i][d] = true;
                    colUsed[j][d] = true;
                    boxUsed[b][d] = true;
                }
            }
        }
        
        solve(board, 0, 0);
    }
    
    bool solve(vector<vector<char>>& board, int r, int c) {
        // 跳过非空格子，找到下一个待填的空格
        while (r < 9) {
            if (board[r][c] == '.') break;
            c++;
            if (c == 9) { c = 0; r++; }
        }
        if (r == 9) return true;  // 全部填完
        
        int b = (r / 3) * 3 + c / 3;
        for (int d = 0; d < 9; d++) {
            // O(1) 检查：数字 d+1 是否在同行/同列/同宫格已被使用
            if (rowUsed[r][d] || colUsed[c][d] || boxUsed[b][d]) continue;
            
            // 做选择
            board[r][c] = '1' + d;
            rowUsed[r][d] = colUsed[c][d] = boxUsed[b][d] = true;
            
            // 递归处理下一个位置
            int nc = c + 1, nr = r;
            if (nc == 9) { nc = 0; nr++; }
            if (solve(board, nr, nc)) return true;
            
            // 撤销选择
            board[r][c] = '.';
            rowUsed[r][d] = colUsed[c][d] = boxUsed[b][d] = false;
        }
        
        return false;  // 0-8 都试过，此路不通
    }
};

// ============================================================
// 解法3: 回溯 + 位运算 + MRV 启发式 — 极致优化
// 时间: O(9^m) 但实际搜索树极小  空间: O(m) blanks列表
//
// 【思路】
// 两个优化维度：
// (1) 位运算：用 9 位整数代替 bool 数组
//     row[i] 的第 d 位为 1 → 第 i 行已有数字 d+1
//     可选数字 = ~(row[i] | col[j] | box[b]) & 0x1FF
//     用 x & (-x) 逐个提取可选数字
//
// (2) MRV (Minimum Remaining Values) 启发式：
//     每次选"可选数字最少"的空格先填
//     这样最容易触发冲突，尽早剪枝
//
// 位运算示例：
//   row[0] = 0b101100110  → 第0行已有 {2,3,6,7,9}
//   col[2] = 0b010001100  → 第2列已有 {3,4,8}
//   box[0] = 0b100100010  → 宫格0已有 {2,6,9}
//   used   = 0b111101110  (三者的 OR)
//   avail  = 0b000010001  (取反 & 0x1FF) → 可选 {1, 5}
//   pick   = avail & (-avail) = 0b000000001 → 数字 1
//   ctz(pick) = 0 → 对应 '1'
// ============================================================
class Solution3 {
public:
    int row[9], col[9], box[9];
    
    void solveSudoku(vector<vector<char>>& board) {
        memset(row, 0, sizeof(row));
        memset(col, 0, sizeof(col));
        memset(box, 0, sizeof(box));
        
        vector<pair<int,int>> blanks;
        
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (board[i][j] == '.') {
                    blanks.push_back({i, j});
                } else {
                    int mask = 1 << (board[i][j] - '1');
                    row[i] |= mask;
                    col[j] |= mask;
                    box[(i / 3) * 3 + j / 3] |= mask;
                }
            }
        }
        
        solve(board, blanks);
    }
    
    bool solve(vector<vector<char>>& board, vector<pair<int,int>>& blanks) {
        if (blanks.empty()) return true;
        
        // --- MRV: 找可选数字最少的空格 ---
        int bestIdx = 0, minCnt = 10;
        for (int k = 0; k < (int)blanks.size(); k++) {
            auto [i, j] = blanks[k];
            int used = row[i] | col[j] | box[(i / 3) * 3 + j / 3];
            int cnt = __builtin_popcount((~used) & 0x1FF);
            if (cnt < minCnt) {
                minCnt = cnt;
                bestIdx = k;
                if (cnt == 0) return false;  // 无可选数字，立即剪枝
                if (cnt == 1) break;          // 唯一选择，不用再找了
            }
        }
        
        auto [r, c] = blanks[bestIdx];
        int b = (r / 3) * 3 + c / 3;
        int available = (~(row[r] | col[c] | box[b])) & 0x1FF;
        
        // O(1) 从列表移除选中的空格（swap 到末尾再 pop）
        swap(blanks[bestIdx], blanks.back());
        blanks.pop_back();
        
        // 枚举所有可选数字
        while (available) {
            int pick = available & (-available);  // 最低位的 1
            int d = __builtin_ctz(pick);          // 位位置 → 数字索引
            
            // 做选择
            board[r][c] = '1' + d;
            row[r] |= pick;
            col[c] |= pick;
            box[b] |= pick;
            
            if (solve(board, blanks)) return true;
            
            // 撤销选择
            board[r][c] = '.';
            row[r] ^= pick;  // 异或清除该位
            col[c] ^= pick;
            box[b] ^= pick;
            
            available &= (available - 1);  // 去掉最低位的 1
        }
        
        // 恢复 blanks（把该空格放回列表）
        blanks.push_back({r, c});
        // 注意：不需要 swap 回原位，因为 blanks 的顺序不影响正确性
        // （MRV 每次都会重新找最优的空格）
        
        return false;
    }
};

// ============================================================
// 【解法对比】
//
// | 维度         | 解法1      | 解法2        | 解法3          |
// |-------------|-----------|-------------|---------------|
// | 合法性检查   | O(9) 扫描  | O(1) 查表    | O(1) 位运算    |
// | 搜索顺序     | 固定顺序   | 固定顺序     | MRV 动态选择   |
// | 代码量       | ~30 行     | ~40 行       | ~60 行         |
// | 实际速度     | ~50ms     | ~5ms        | ~0ms          |
//
// 面试中推荐解法2：代码量适中，逻辑清晰，性能足够。
// 解法3 在面试中如果时间充裕可以口述思路作为加分项。
//
// 【易错点】
// 1. 回溯返回值未传播:
//    ✗ solve(board, nr, nc);         // 忽略返回值
//    ✓ if (solve(board, nr, nc)) return true;  // 找到解立即停止
//    原因：不传播 true 的话，递归回来后会继续撤销和尝试，
//    最终把已经解好的 board 恢复成 '.'
//
// 2. 宫格编号公式错误:
//    ✗ int b = (i / 3) + (j / 3);     // 只有 0-5 种值，不同宫格冲突
//    ✓ int b = (i / 3) * 3 + j / 3;   // 0-8 九种值，一一对应
//
// 3. 预处理遗漏:
//    ✗ 直接开始回溯，没有把已有数字记录到约束数组
//    ✓ 先遍历整个 board，对所有非 '.' 格子标记约束
//    后果：约束数组全是 false，回溯时会在已有数字上叠加新数字
//
// 4. 位运算可用集忘记掩码:
//    ✗ int avail = ~used;             // 高位全是 1
//    ✓ int avail = (~used) & 0x1FF;   // 只保留低 9 位
//    后果：popcount 算出远大于 9 的值，MRV 策略完全失效
//
// 5. 解法1 中 return false 的位置:
//    ✗ 把 return false 放在 for(char c...) 循环内部
//    ✓ return false 在 for(char c...) 循环之后
//    含义：1-9 都试过都不行，当前空格无解 → 回溯
//
// 【面试追问】
// Q1(基础): "暴力回溯能通过吗？瓶颈在哪？"
//   → 能通过，约束剪枝使实际搜索树很小。瓶颈在 isValid 每次扫描 27 格。
//
// Q2(优化): "怎么加速合法性检查？"
//   → 用 rowUsed/colUsed/boxUsed 三个预计算数组，O(1) 判断。
//     进一步用位运算，一条 OR 指令得到所有约束。
//
// Q3(进阶): "怎么进一步减少搜索的分支数？"
//   → MRV 启发式：优先填可选数字最少的空格。
//     如果某格 0 个选择，立即回溯；某格 1 个选择，不分支。
//     这是 CSP 领域的经典策略。
//
// Q4(变体): "如何判断数独是否有唯一解？"
//   → 找到第一个解后不返回，继续搜索。用计数器统计解的数量，
//     > 1 时立即返回"多解"。搜索完只有 1 个就是唯一解。
// ============================================================
