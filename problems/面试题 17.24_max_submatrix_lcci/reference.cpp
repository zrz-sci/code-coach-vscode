/*
 * 面试题 17.24: 最大子矩阵 (Max Submatrix LCCI)
 *
 * 思路: 枚举上下行边界 + 列压缩 + Kadane 算法
 *       将二维最大子矩阵问题降维为一维最大子数组和问题
 *
 * 时间: O(N^2 * M)   空间: O(M)
 */

// ═══════════════════════════════════════════════════════
// 解法一: 增量列压缩 + Kadane（推荐）
// ═══════════════════════════════════════════════════════

class Solution {
public:
    vector<int> getMaxMatrix(vector<vector<int>>& matrix) {
        int n = matrix.size(), m = matrix[0].size();
        int maxSum = INT_MIN;
        vector<int> res(4);  // [r1, c1, r2, c2]

        for (int top = 0; top < n; top++) {
            // 每换一个 top, 列压缩数组清零
            vector<int> col(m, 0);

            for (int bottom = top; bottom < n; bottom++) {
                // 增量累加第 bottom 行
                for (int c = 0; c < m; c++) {
                    col[c] += matrix[bottom][c];
                }

                // 对 col[] 跑 Kadane, 同时追踪列坐标
                int curSum = 0;
                int tempStart = 0;  // 当前段的起始列

                for (int c = 0; c < m; c++) {
                    curSum += col[c];

                    // 先检查是否更新全局最优
                    if (curSum > maxSum) {
                        maxSum = curSum;
                        res = {top, tempStart, bottom, c};
                    }

                    // 再检查是否需要重置(止损)
                    if (curSum < 0) {
                        curSum = 0;
                        tempStart = c + 1;  // 新段从下一列开始
                    }
                }
            }
        }

        return res;
    }
};


// ═══════════════════════════════════════════════════════
// 解法二: 列前缀和 + Kadane
// ═══════════════════════════════════════════════════════
//
// 预先计算列前缀和, 使任意 [top, bottom] 行范围的列和 O(1) 得到.
// 本质与解法一相同, 但代码结构不同.
// 时间: O(N^2 * M)   空间: O(N * M)

class Solution_PrefixSum {
public:
    vector<int> getMaxMatrix(vector<vector<int>>& matrix) {
        int n = matrix.size(), m = matrix[0].size();

        // prefix[i][j] = sum(matrix[0..i-1][j])
        vector<vector<int>> prefix(n + 1, vector<int>(m, 0));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                prefix[i + 1][j] = prefix[i][j] + matrix[i][j];
            }
        }

        int maxSum = INT_MIN;
        vector<int> res(4);

        for (int top = 0; top < n; top++) {
            for (int bottom = top; bottom < n; bottom++) {
                int curSum = 0, tempStart = 0;

                for (int c = 0; c < m; c++) {
                    int colVal = prefix[bottom + 1][c] - prefix[top][c];

                    curSum += colVal;
                    if (curSum > maxSum) {
                        maxSum = curSum;
                        res = {top, tempStart, bottom, c};
                    }
                    if (curSum < 0) {
                        curSum = 0;
                        tempStart = c + 1;
                    }
                }
            }
        }

        return res;
    }
};


/*
 * ═══════════════════════════════════════════════════════
 * 干跑示例: matrix = [[9,-8,1,3,-2],[-3,7,6,-2,4],[6,-4,-4,8,-7]]
 * ═══════════════════════════════════════════════════════
 *
 * top=0, bot=0: col = [9, -8, 1, 3, -2]
 *   Kadane:
 *     c=0: cur=9 > INT_MIN, res=[0,0,0,0], maxSum=9
 *     c=1: cur=9+(-8)=1
 *     c=2: cur=1+1=2
 *     c=3: cur=2+3=5
 *     c=4: cur=5+(-2)=3
 *   局部最优=9
 *
 * top=0, bot=1: col = [6, -1, 7, 1, 2]
 *   Kadane:
 *     c=0: cur=6, 6<9 不更新
 *     c=1: cur=5
 *     c=2: cur=12 > 9, res=[0,0,1,2], maxSum=12
 *     c=3: cur=13 > 12, res=[0,0,1,3], maxSum=13
 *     c=4: cur=15 > 13, res=[0,0,1,4], maxSum=15
 *
 * top=0, bot=2: col = [12, -5, 3, 9, -5]
 *   Kadane:
 *     c=0: cur=12, 12<15 不更新
 *     c=1: cur=7
 *     c=2: cur=10
 *     c=3: cur=19 > 15, res=[0,0,2,3], maxSum=19   <-- 全局最优!
 *     c=4: cur=14
 *
 * top=1, bot=1: col = [-3, 7, 6, -2, 4]
 *   Kadane:
 *     c=0: cur=-3, -3<19 不更新, cur<0 重置 tempStart=1
 *     c=1: cur=7, c=2: cur=13, c=3: cur=11, c=4: cur=15
 *     局部最优=15 < 19
 *
 * top=1, bot=2: col = [3, 3, 2, 6, -3]
 *   Kadane: 3, 6, 8, 14, 11 → 局部最优=14 < 19
 *
 * top=2, bot=2: col = [6, -4, -4, 8, -7]
 *   Kadane: 6, 2, -2<0 重置, 8, 1 → 局部最优=8 < 19
 *
 * 最终: [0, 0, 2, 3], sum = 19
 *
 * 验证: 9+(-8)+1+3 + (-3)+7+6+(-2) + 6+(-4)+(-4)+8 = 5+8+6 = 19 ✓
 *
 * ═══════════════════════════════════════════════════════
 * 边界情况验证
 * ═══════════════════════════════════════════════════════
 *
 * 全负矩阵 [[-1,-2],[-3,-4]]:
 *   top=0,bot=0: col=[-1,-2], Kadane: cur=-1 > INT_MIN → maxSum=-1, res=[0,0,0,0]
 *   top=0,bot=1: col=[-4,-6], Kadane: cur=-4, cur<0 重置, cur=-6 → maxSum=-1
 *   top=1,bot=1: col=[-3,-4], Kadane: cur=-3 > -1? NO → maxSum=-1
 *   答案: [0,0,0,0], 最大元素 -1 ✓
 *
 * 题目示例 [[-1,0],[0,-1]]:
 *   top=0,bot=0: col=[-1,0], Kadane: cur=-1<0 重置, cur=0 > INT_MIN → [0,1,0,1]
 *   最终 maxSum=0, 答案 [0,1,0,1] ✓
 *
 * ═══════════════════════════════════════════════════════
 * 面试追问
 * ═══════════════════════════════════════════════════════
 *
 * Q1: 如果子矩阵和不能超过 K?
 *     → LC 363, 列压缩后不能用 Kadane, 改用有序集合 + 二分
 *       找满足 prefixSum[j] - prefixSum[i] <= K 的最大差
 *
 * Q2: 如果要求正方形子矩阵?
 *     → 枚举 (top, bottom) 时加约束 bottom-top == c2-c1
 *       Kadane 需要限制窗口宽度 = bottom-top+1
 *
 * Q3: N=M=1000 怎么优化?
 *     → O(N^2*M) = 10^9 可能 TLE, 考虑:
 *       1. 若 N > M 则转置矩阵使外层循环更短
 *       2. 剪枝: 列和全为负时可提前跳过
 *
 * ═══════════════════════════════════════════════════════
 * 易错点
 * ═══════════════════════════════════════════════════════
 *
 * ✗ globalMax 初始化为 0 → 全负矩阵时答案错误, 必须 INT_MIN
 * ✗ bottom 变化时重新计算整个列和 → 应增量累加, 否则 O(N^3*M)
 * ✗ tempStart 更新为 j 而非 j+1 → curSum<0 时当前列也被丢弃
 * ✗ 先判断 curSum<0 再更新 globalMax → 可能漏掉合法答案
 */
