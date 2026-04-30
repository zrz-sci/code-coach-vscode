// ============================================================
// LeetCode 598: 区间加法 II (Range Addition II)
// ============================================================

// 【题目本质】
// 所有操作都从 (0,0) 开始加 1，最大值区域就是所有操作的交集矩形。
// 求交集面积 = min(所有 a_i) * min(所有 b_i)。

// 【解法总览】
// 解法一：求交集（推荐） - O(k) 时间 O(1) 空间
// 解法二：暴力模拟 - O(m*n*k) 时间（不推荐）

// ============================================================
// 解法一：求交集（推荐）
// ============================================================
class Solution {
public:
    int maxCount(int m, int n, vector<vector<int>>& ops) {
        int minRow = m, minCol = n;
        for (auto& op : ops) {
            minRow = min(minRow, op[0]);
            minCol = min(minCol, op[1]);
        }
        return minRow * minCol;
    }
};

// ============================================================
// 解法二：暴力模拟（面试中不推荐，仅供理解）
// ============================================================
// class Solution {
// public:
//     int maxCount(int m, int n, vector<vector<int>>& ops) {
//         vector<vector<int>> mat(m, vector<int>(n, 0));
//         for (auto& op : ops) {
//             for (int i = 0; i < op[0]; i++) {
//                 for (int j = 0; j < op[1]; j++) {
//                     mat[i][j]++;
//                 }
//             }
//         }
//         int maxVal = 0, cnt = 0;
//         for (int i = 0; i < m; i++) {
//             for (int j = 0; j < n; j++) {
//                 if (mat[i][j] > maxVal) {
//                     maxVal = mat[i][j];
//                     cnt = 1;
//                 } else if (mat[i][j] == maxVal) {
//                     cnt++;
//                 }
//             }
//         }
//         return cnt;
//     }
// };

// 【解法对比】
// | 解法       | 时间     | 空间    | 优势           |
// |-----------|---------|---------|---------------|
// | 求交集 ⭐  | O(k)    | O(1)   | 最优，一次遍历  |
// | 暴力模拟   | O(m*n*k)| O(m*n) | 直观但不可用    |

// 【易错点】
// 1. minRow/minCol 初始值应为 m/n，不是 0 或 INT_MAX
//    → ops 为空时直接返回 m*n
// 2. 返回值是 minRow * minCol（面积），不是和
// 3. 注意整数溢出：m, n <= 4*10^4，乘积 <= 1.6*10^9，int 够用

// 【面试追问】
// Q1: 操作不从 (0,0) 开始 → 需要二维差分数组
// Q2: ops 极大但矩阵小 → 直接模拟或 O(k) 扫描
// Q3: 与 LC 370 的关系 → 370 是一维差分，本题退化为交集
