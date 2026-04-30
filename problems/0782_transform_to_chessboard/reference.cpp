// 782. Transform to Chessboard
// 数学观察 + 贪心
// 时间复杂度: O(n^2), 空间复杂度: O(1)
//
// 核心思路:
// 1. 合法棋盘只有两种行(互补)和两种列(互补)
// 2. 验证: 任意2x2子矩阵的XOR必须为0
// 3. 行交换和列交换独立计算
// 4. 不匹配位置数 / 2 = 交换次数

class Solution {
public:
    int movesToChessboard(vector<vector<int>>& board) {
        int n = board.size();

        // Step 1: 验证合法性 - 任意矩形的四角XOR为0
        // 这保证了整个矩阵只有两种行模式(互补)和两种列模式(互补)
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                // board[0][0] ^ board[i][0] ^ board[0][j] ^ board[i][j]
                // 如果不为0，说明存在不满足互补关系的行列组合
                if (board[0][0] ^ board[i][0] ^ board[0][j] ^ board[i][j]) {
                    return -1;
                }
            }
        }

        // Step 2: 统计第一行和第一列中1的个数
        int rowOnes = 0, colOnes = 0;
        // 同时统计第一行/列中与位置奇偶不匹配的个数
        int rowMismatch = 0, colMismatch = 0;
        for (int i = 0; i < n; ++i) {
            rowOnes += board[0][i];       // 第一行的1的个数
            colOnes += board[i][0];       // 第一列的1的个数
            // 与目标模式 0,1,0,1,... 比较
            if (board[0][i] != (i % 2)) {
                rowMismatch++;
            }
            if (board[i][0] != (i % 2)) {
                colMismatch++;
            }
        }

        // Step 3: 验证1的个数是否合法
        // 合法棋盘中，1的个数必须为 n/2 或 (n+1)/2
        if (rowOnes < n / 2 || rowOnes > (n + 1) / 2) return -1;
        if (colOnes < n / 2 || colOnes > (n + 1) / 2) return -1;

        // Step 4: 计算最小交换次数
        // n为偶数: 两种目标模式都可以, 取不匹配数较小的那个
        // n为奇数: 不匹配数必须为偶数(因为交换一次修复2个位置)
        if (n % 2 == 0) {
            // 偶数情况: 0101... 和 1010... 两种目标
            // mismatch 和 n-mismatch 分别对应两种目标
            rowMismatch = min(rowMismatch, n - rowMismatch);
            colMismatch = min(colMismatch, n - colMismatch);
        } else {
            // 奇数情况: 只有一种合法目标
            // 不匹配数必须为偶数
            if (rowMismatch % 2 != 0) rowMismatch = n - rowMismatch;
            if (colMismatch % 2 != 0) colMismatch = n - colMismatch;
        }

        // 每两个不匹配位置可以通过一次交换修复
        return (rowMismatch + colMismatch) / 2;
    }
};

// ============================================================
// 解法二: 使用位运算优化验证 (更简洁的实现)
// 时间复杂度: O(n^2), 空间复杂度: O(1)
// ============================================================
class Solution_BitMask {
public:
    int movesToChessboard(vector<vector<int>>& board) {
        int n = board.size();

        // 把每一行看做一个位掩码
        // 合法棋盘只有两种行: mask 和 ~mask
        // 用第一行作为基准行
        int baseMask = 0;
        for (int j = 0; j < n; ++j) {
            baseMask |= (board[0][j] << j);
        }
        int complementMask = ((1 << n) - 1) ^ baseMask;

        int countBase = 0, countComplement = 0;
        for (int i = 0; i < n; ++i) {
            int rowMask = 0;
            for (int j = 0; j < n; ++j) {
                rowMask |= (board[i][j] << j);
            }
            if (rowMask == baseMask) {
                countBase++;
            } else if (rowMask == complementMask) {
                countComplement++;
            } else {
                return -1; // 存在第三种行模式，不合法
            }
        }

        // 验证两种行模式数量差不超过1
        if (abs(countBase - countComplement) > 1) return -1;

        // 同样的逻辑应用到列
        int baseColMask = 0;
        for (int i = 0; i < n; ++i) {
            baseColMask |= (board[i][0] << i);
        }
        int complementColMask = ((1 << n) - 1) ^ baseColMask;

        int countColBase = 0, countColComplement = 0;
        for (int j = 0; j < n; ++j) {
            int colMask = 0;
            for (int i = 0; i < n; ++i) {
                colMask |= (board[i][j] << i);
            }
            if (colMask == baseColMask) {
                countColBase++;
            } else if (colMask == complementColMask) {
                countColComplement++;
            } else {
                return -1;
            }
        }

        if (abs(countColBase - countColComplement) > 1) return -1;

        // 计算交换次数: 通过第一行/列与目标对比
        auto calcSwaps = [&](int mask, int n) -> int {
            int ones = __builtin_popcount(mask);
            if (ones < n / 2 || ones > (n + 1) / 2) return -1;

            // 目标模式: 0101... => evenMask, 1010... => oddMask
            int evenMask = 0; // 偶数位为1
            for (int i = 0; i < n; i += 2) evenMask |= (1 << i);
            int oddMask = ((1 << n) - 1) ^ evenMask;

            int diffEven = __builtin_popcount(mask ^ evenMask);
            int diffOdd  = __builtin_popcount(mask ^ oddMask);

            if (n % 2 == 0) {
                return min(diffEven, diffOdd) / 2;
            } else {
                // 奇数n: 只有一个目标合法(不匹配数为偶数的那个)
                return (diffEven % 2 == 0) ? diffEven / 2 : diffOdd / 2;
            }
        };

        int rowSwaps = calcSwaps(baseMask, n);
        int colSwaps = calcSwaps(baseColMask, n);
        if (rowSwaps < 0 || colSwaps < 0) return -1;

        return rowSwaps + colSwaps;
    }
};
