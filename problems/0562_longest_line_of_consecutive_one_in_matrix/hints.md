# 562. Longest Line of Consecutive One in Matrix - 矩阵中最长的连续1线段

## 核心思路
对每个值为 1 的格子，追踪它在水平、垂直、对角线、反对角线四个方向上的连续 1 长度，取全局最大值。

## 思维链
1. 连续 1 线段有几个方向？ → 水平、垂直、对角线(↘)、反对角线(↙)，共4个
2. 如何高效计算？ → DP：`dp[i][j][d]` 表示以 (i,j) 结尾在方向 d 的连续 1 长度
3. 转移关系？ → 从前一个位置继承 +1，或从 0 开始
4. 空间优化？ → 可以只保留上一行（但需注意对角线方向）

## 解法概览表

| 解法 | 时间 | 空间 | 特点 |
|------|------|------|------|
| ⭐ 3D DP | O(mn) | O(mn) | 清晰直观 |
| 空间优化 DP | O(mn) | O(n) | 只保留两行 |

## 关键提示
1. 四个方向的前驱：水平(i,j-1)、垂直(i-1,j)、对角线(i-1,j-1)、反对角线(i-1,j+1)
2. `mat[i][j] == 0` 时所有方向归零
3. 注意反对角线从右上来，j 的遍历顺序影响空间优化

## 解法详解

### 解法1: 3D DP ⭐
```cpp
class Solution {
public:
    int longestLine(vector<vector<int>>& mat) {
        int m = mat.size(), n = mat[0].size();
        // dp[i][j] = {horizontal, vertical, diagonal, anti-diagonal}
        vector<vector<array<int,4>>> dp(m, vector<array<int,4>>(n, {0,0,0,0}));
        int res = 0;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (mat[i][j] == 1) {
                    dp[i][j][0] = (j > 0 ? dp[i][j-1][0] : 0) + 1;
                    dp[i][j][1] = (i > 0 ? dp[i-1][j][1] : 0) + 1;
                    dp[i][j][2] = (i > 0 && j > 0 ? dp[i-1][j-1][2] : 0) + 1;
                    dp[i][j][3] = (i > 0 && j < n-1 ? dp[i-1][j+1][3] : 0) + 1;
                    res = max({res, dp[i][j][0], dp[i][j][1], dp[i][j][2], dp[i][j][3]});
                }
            }
        }
        return res;
    }
};
```

### 解法2: 空间优化 DP
```cpp
class Solution {
public:
    int longestLine(vector<vector<int>>& mat) {
        int m = mat.size(), n = mat[0].size();
        vector<array<int,4>> prev(n, {0,0,0,0}), curr(n, {0,0,0,0});
        int res = 0;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (mat[i][j] == 1) {
                    curr[j][0] = (j > 0 ? curr[j-1][0] : 0) + 1;
                    curr[j][1] = prev[j][1] + 1;
                    curr[j][2] = (j > 0 ? prev[j-1][2] : 0) + 1;
                    curr[j][3] = (j < n-1 ? prev[j+1][3] : 0) + 1;
                    res = max({res, curr[j][0], curr[j][1], curr[j][2], curr[j][3]});
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
```

## 易错点
- ✗ 忘记反对角线方向是 (i-1, j+1) 而非 (i-1, j-1) → ✓ 反对角线从右上到左下
- ✗ mat[i][j] == 0 时不清零 dp → ✓ 必须全部归零，断开连续
- ✗ 空间优化时反对角线方向被当前行覆盖 → ✓ 需要用 prev 行的 j+1

## 面试追问

**Q1: 为什么不需要向反方向（如从右到左）再扫一遍？**
因为 dp[i][j] 记录的是"以(i,j)结尾"的连续1长度。每个方向只需从一个前驱转移，遍历整个矩阵即可覆盖所有线段。

**Q2: 如果允许 8 个方向呢？**
只需要4个方向。因为水平包含了左→右和右→左，垂直包含了上→下和下→上。每个方向从一端扫到另一端就够了。

**Q3: m*n <= 10^4 的约束意味着什么？**
矩阵可以很"瘦长"（如 1x10000），但总元素不超过 10^4，所以 O(mn) 解法完全够用。

## 相关题型
- [329. Longest Increasing Path in a Matrix](https://leetcode.com/problems/longest-increasing-path-in-a-matrix/) - 矩阵DP
- [1102. Path With Maximum Minimum Value](https://leetcode.com/problems/path-with-maximum-minimum-value/) - 矩阵路径
- [498. Diagonal Traverse](https://leetcode.com/problems/diagonal-traverse/) - 对角线遍历
