/*
 * 304. 二维区域和检索 - 矩阵不可变 (Range Sum Query 2D - Immutable)
 * 难度: Medium
 * 标签: Design, Array, Matrix, Prefix Sum
 *
 * 题目:
 *   设计 NumMatrix 类。给定 m*n 矩阵，O(1) 查询子矩形元素之和。
 *   sumRegion(row1, col1, row2, col2) 返回 (row1,col1) 到 (row2,col2) 的和。
 *   1<=m,n<=200, -10^4<=matrix[i][j]<=10^4, 最多 10^4 次查询
 *
 * 核心思路:
 *   二维前缀和 + 容斥原理。
 *   prefix[i][j] = 从 (0,0) 到 (i-1,j-1) 的矩形元素总和。
 *   构建: prefix[i][j] = 上 + 左 - 左上 + 当前
 *   查询: 大矩形 - 上方 - 左方 + 左上角
 *
 * 解法概览:
 *   解法1 - 二维前缀和 ⭐: 构建 O(mn), 查询 O(1), 空间 O(mn)
 *   解法2 - 行前缀和(对比): 构建 O(mn), 查询 O(m), 空间 O(mn)
 */

// ============================================================
// 解法 1: 二维前缀和（推荐）⭐
// ============================================================
// 思路: 利用容斥原理构建二维前缀和数组
//       prefix[i][j] = sum of rectangle from (0,0) to (i-1,j-1)
// 构建: O(mn)  查询: O(1)  空间: O(mn)
//
// 容斥原理图解:
//   构建 prefix[i][j]:
//     +-------+---+
//     |prefix |   |
//     |[i-1]  |   |
//     |  [j]  |   |
//     +-------+---+  prefix[i][j] = prefix[i-1][j]
//     |prefix | X |                + prefix[i][j-1]
//     |[i]    |   |                - prefix[i-1][j-1]
//     | [j-1] |   |                + matrix[i-1][j-1]
//     +-------+---+
//
//   查询 sumRegion(r1,c1,r2,c2):
//     +-----+-----+
//     |  D  |  C  |
//     +-----+-----+ <- r1
//     |  B  |  A  |   A = prefix[r2+1][c2+1]
//     +-----+-----+ <- r2      - prefix[r1][c2+1]
//           ^     ^            - prefix[r2+1][c1]
//          c1    c2            + prefix[r1][c1]
//
// 前缀和构建模拟 (示例矩阵):
//   matrix:                prefix (多一行一列 padding):
//   3  0  1  4  2         0   0   0   0   0   0
//   5  6  3  2  1         0   3   3   4   8  10
//   1  2  0  1  5         0   8  14  18  24  27
//   4  1  0  1  7         0   9  17  21  28  36
//   1  0  3  0  5         0  13  22  26  34  49
//                         0  14  23  30  38  58
//
//   sumRegion(2,1,4,3) = prefix[5][4] - prefix[2][4] - prefix[5][1] + prefix[2][1]
//                       = 38 - 24 - 14 + 8 = 8
class Solution1 {
public:
    // 作为 NumMatrix 类实现
    vector<vector<int>> prefix;

    void init(vector<vector<int>>& matrix) {
        int m = matrix.size(), n = matrix[0].size();
        // (m+1)x(n+1) padding: 第 0 行/列全为 0，消除边界判断
        prefix.assign(m + 1, vector<int>(n + 1, 0));
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                // 容斥: 上方 + 左方 - 左上角(重叠) + 当前格
                prefix[i][j] = prefix[i - 1][j] + prefix[i][j - 1]
                             - prefix[i - 1][j - 1] + matrix[i - 1][j - 1];
            }
        }
    }

    int sumRegion(int row1, int col1, int row2, int col2) {
        // 容斥: 大矩形 - 上方多余 - 左方多余 + 左上角(减多了)
        return prefix[row2 + 1][col2 + 1] - prefix[row1][col2 + 1]
             - prefix[row2 + 1][col1]     + prefix[row1][col1];
    }
    // 关键点:
    //   - padding 让 prefix[-1][*] = prefix[*][-1] = 0, 无需特判
    //   - matrix[i][j] 对应 prefix[i+1][j+1]
    //   - 查询公式中 row2/col2 要 +1, row1/col1 不需要
    //   - int 范围: 最大和 = 200*200*10^4 = 4*10^8, int 够用
};

// ============================================================
// 解法 2: 行前缀和（不满足 O(1)，用于理解推导过程）
// ============================================================
// 思路: 对每行独立计算前缀和，查询时逐行求列区间和再累加
// 构建: O(mn)  查询: O(m)  空间: O(mn)
//
// 思考过程:
//   1. 每行独立做一维前缀和
//   2. 查询 sumRegion: 对 row1..row2 每行 O(1) 求列区间和
//   3. 总查询 O(row2-row1+1) = O(m), 不满足 O(1) 要求
//   4. 作为面试中从一维到二维的过渡讲解
class Solution2 {
public:
    vector<vector<int>> rowPrefix;

    void init(vector<vector<int>>& matrix) {
        int m = matrix.size(), n = matrix[0].size();
        rowPrefix.assign(m, vector<int>(n + 1, 0));
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                rowPrefix[i][j + 1] = rowPrefix[i][j] + matrix[i][j];
    }

    int sumRegion(int row1, int col1, int row2, int col2) {
        int sum = 0;
        for (int i = row1; i <= row2; i++)
            sum += rowPrefix[i][col2 + 1] - rowPrefix[i][col1];
        return sum;
    }
    // 关键点:
    //   - 比暴力好 (每行 O(1))，但不满足 O(1) 查询要求
    //   - 有助于理解从一维前缀和到二维的推导
};

// ============================================================
// LeetCode 提交版（完整 NumMatrix 类）
// ============================================================
class NumMatrix {
public:
    vector<vector<int>> prefix;

    NumMatrix(vector<vector<int>>& matrix) {
        int m = matrix.size(), n = matrix[0].size();
        prefix.assign(m + 1, vector<int>(n + 1, 0));
        for (int i = 1; i <= m; i++)
            for (int j = 1; j <= n; j++)
                prefix[i][j] = prefix[i - 1][j] + prefix[i][j - 1]
                             - prefix[i - 1][j - 1] + matrix[i - 1][j - 1];
    }

    int sumRegion(int row1, int col1, int row2, int col2) {
        return prefix[row2 + 1][col2 + 1] - prefix[row1][col2 + 1]
             - prefix[row2 + 1][col1]     + prefix[row1][col1];
    }
};

/*
 * ============================================================
 * 解法对比
 * ============================================================
 * | 维度       | 解法1 二维前缀和 ⭐ | 解法2 行前缀和  |
 * |-----------|---------------------|----------------|
 * | 构造时间   | O(mn)               | O(mn)          |
 * | 查询时间   | O(1)                | O(m)           |
 * | 空间       | O(mn)               | O(mn)          |
 * | 满足要求   | 是                  | 否             |
 * | 核心技巧   | 容斥原理            | 一维前缀和     |
 *
 * ============================================================
 * 易错点
 * ============================================================
 * 1. prefix 下标偏移搞混
 *    错: prefix[i][j] = ... + matrix[i][j]
 *    对: prefix[i][j] = ... + matrix[i-1][j-1]  (prefix 有 padding)
 *
 * 2. 查询公式的加减号写错
 *    记忆法: 画十字分四块，对角线同号(左上+、右下+)，另两块异号
 *    prefix[r2+1][c2+1] - prefix[r1][c2+1] - prefix[r2+1][c1] + prefix[r1][c1]
 *
 * 3. 忘记 padding 导致越界
 *    prefix 必须是 (m+1)x(n+1), 否则 i=0 时 prefix[i-1] 越界
 *
 * 4. 查询时 row2/col2 忘记 +1
 *    错: prefix[row2][col2] - ...
 *    对: prefix[row2+1][col2+1] - ...
 *
 * ============================================================
 * 面试追问
 * ============================================================
 * Q1: 矩阵可以修改怎么办? (update 某个元素)
 * A1: 二维树状数组(BIT), 更新和查询都是 O(log m * log n)。
 *     见 LeetCode 308。
 *
 * Q2: 推广到三维?
 * A2: 三维容斥: 构建用 7 项(3面-3边+1角), 查询同理。
 *     prefix[i][j][k] = sum of cube from (0,0,0) to (i-1,j-1,k-1)。
 *
 * Q3: 如果矩阵非常大(10^6 x 10^6)放不进内存?
 * A3: 分块/分治处理，或者用外存算法。实际中可按行分块
 *     计算前缀和。
 */
