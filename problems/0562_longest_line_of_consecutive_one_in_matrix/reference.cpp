// 【题目本质】
// 在二进制矩阵中找最长的连续1线段（水平/垂直/对角线/反对角线）。
// 核心：对每个格子维护4个方向的连续1长度。

// 【解法总览】
// Solution1: 3D DP — O(mn) 时间，O(mn) 空间 ⭐
// Solution2: 空间优化 DP — O(mn) 时间，O(n) 空间

// ===================== Solution1: 3D DP =====================
// dp[i][j] = {水平, 垂直, 对角线, 反对角线} 四个方向以(i,j)结尾的连续1长度
// 转移：
//   水平: dp[i][j-1][0] + 1
//   垂直: dp[i-1][j][1] + 1
//   对角线(↘): dp[i-1][j-1][2] + 1
//   反对角线(↙): dp[i-1][j+1][3] + 1
class Solution1 {
public:
    int longestLine(vector<vector<int>>& mat) {
        int m = mat.size(), n = mat[0].size();
        vector<vector<array<int,4>>> dp(m, vector<array<int,4>>(n, {0,0,0,0}));
        int res = 0;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (mat[i][j] == 1) {
                    dp[i][j][0] = (j > 0 ? dp[i][j-1][0] : 0) + 1;
                    dp[i][j][1] = (i > 0 ? dp[i-1][j][1] : 0) + 1;
                    dp[i][j][2] = (i > 0 && j > 0 ? dp[i-1][j-1][2] : 0) + 1;
                    dp[i][j][3] = (i > 0 && j < n-1 ? dp[i-1][j+1][3] : 0) + 1;
                    res = max({res, dp[i][j][0], dp[i][j][1],
                               dp[i][j][2], dp[i][j][3]});
                }
            }
        }
        return res;
    }
};

// ===================== Solution2: 空间优化 DP =====================
// 只保留 prev 行和 curr 行
class Solution2 {
public:
    int longestLine(vector<vector<int>>& mat) {
        int m = mat.size(), n = mat[0].size();
        vector<array<int,4>> prev(n, {0,0,0,0}), curr(n, {0,0,0,0});
        int res = 0;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (mat[i][j] == 1) {
                    curr[j][0] = (j > 0 ? curr[j-1][0] : 0) + 1;     // 水平
                    curr[j][1] = prev[j][1] + 1;                       // 垂直
                    curr[j][2] = (j > 0 ? prev[j-1][2] : 0) + 1;     // 对角线
                    curr[j][3] = (j < n-1 ? prev[j+1][3] : 0) + 1;   // 反对角线
                    res = max({res, curr[j][0], curr[j][1],
                               curr[j][2], curr[j][3]});
                } else {
                    curr[j] = {0, 0, 0, 0};
                }
            }
            swap(prev, curr);
            fill(curr.begin(), curr.end(), array<int,4>{0,0,0,0});
        }
        return res;
    }
};

// 【解法对比】
// | 解法 | 时间 | 空间 | 优势 |
// |------|------|------|------|
// | Solution1 3D DP | O(mn) | O(mn) | 逻辑清晰，面试首选 |
// | Solution2 滚动数组 | O(mn) | O(n) | 空间优化，适合大矩阵 |

// 【易错点】
// 1. 反对角线前驱是 (i-1, j+1)，不是 (i-1, j-1)
// 2. mat[i][j]==0 时四个方向必须全部清零
// 3. 边界检查：j>0（水平/对角线）、i>0（垂直/两条斜线）、j<n-1（反对角线）

// 【面试追问】
// Q1: 为什么4个方向就够？→ 水平覆盖左右，垂直覆盖上下，每个方向从前驱转移
// Q2: 8方向？→ 实际只有4条线（每条线双向），4个方向已覆盖
// Q3: m*n<=10^4 约束？→ 矩阵总元素有限，O(mn)解法高效
