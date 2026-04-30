// ============================================================
// 361. Bomb Enemy
// ============================================================
// 标签: Array, Dynamic Programming, Matrix
// 难度: Medium
//
// 【题目本质】
//   在一个有墙、敌人、空地的矩阵中，放一颗炸弹在空地上，
//   炸弹能沿行列方向炸死敌人（遇墙停止）。求最多炸多少敌人。
//   核心: 墙把行/列分成独立区段，同区段内共享行/列方向的敌人数。
//
// 【解法总览】
//   解法1: 分段缓存 -- 遇墙重算，O(n) 列缓存 + 1 行变量
//   解法2: 四方向预处理DP -- 4个mn矩阵分别存四方向敌人数
// ============================================================


// ============================================================
// 解法1: 分段缓存法 (推荐)
//
// 核心思想:
//   行方向: 当 j==0 或 grid[i][j-1]=='W' 时，重新向右扫描计E
//   列方向: 当 i==0 或 grid[i-1][j]=='W' 时，重新向下扫描计E
//   对每个空格: 总数 = rowHits + colHits[j]
//
// 摊还分析:
//   行方向每个元素最多被扫描2次(起点+被扫); 列同理
//   总时间 O(mn), 空间 O(n)
//
// 过程可视化 (grid=[["0","E","0","0"],["E","0","W","E"],["0","E","0","0"]]):
//
//   (0,0): j==0, rowHits=向右数E=1  i==0, colHits[0]=向下数E=1
//          grid=='0', total=1+1=2
//   (0,1): grid=='E', 跳过
//   (0,2): grid=='0', total=1+0=1
//   (0,3): grid=='0', total=1+1=2
//   (1,0): j==0, rowHits=1  grid=='E', 跳过
//   (1,1): grid=='0', total=1+2=3 ★ 最大!
//   (1,2): grid[i][j-1]不是W(grid[1][1]=='0'), 但grid=='W', 跳过
//   (1,3): grid[i][j-1]=='W', rowHits=向右数E=1, grid=='E', 跳过
//   ...
//   最终 ans = 3
//
// 时间: O(m * n) 摊还
// 空间: O(n)
// ============================================================
class Solution {
public:
    int maxKilledEnemies(vector<vector<char>>& grid) {
        int m = grid.size(), n = grid[0].size();
        int ans = 0;
        int rowHits = 0;             // 当前行区段内的敌人数
        vector<int> colHits(n, 0);   // 每列当前区段内的敌人数

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                // 行方向: 新区段起点 -> 重新计算
                if (j == 0 || grid[i][j - 1] == 'W') {
                    rowHits = 0;
                    for (int k = j; k < n && grid[i][k] != 'W'; k++) {
                        if (grid[i][k] == 'E') rowHits++;
                    }
                }

                // 列方向: 新区段起点 -> 重新计算
                if (i == 0 || grid[i - 1][j] == 'W') {
                    colHits[j] = 0;
                    for (int k = i; k < m && grid[k][j] != 'W'; k++) {
                        if (grid[k][j] == 'E') colHits[j]++;
                    }
                }

                // 只在空格处统计
                if (grid[i][j] == '0') {
                    ans = max(ans, rowHits + colHits[j]);
                }
            }
        }
        return ans;
    }
};


// ============================================================
// 解法2: 四方向预处理DP
//
// 思路:
//   建4个 m x n 矩阵:
//     left[i][j]: (i,j)向左能炸到的敌人数
//     right[i][j]: 向右
//     up[i][j]: 向上
//     down[i][j]: 向下
//   每个方向一次线性扫描填充，最后对每个空格取四方向之和。
//
// 递推:
//   left[i][j]:
//     if grid[i][j]=='W': left[i][j]=0
//     elif grid[i][j]=='E': left[i][j]=left[i][j-1]+1
//     else: left[i][j]=left[i][j-1]
//   其余方向类似，注意扫描方向。
//
// 时间: O(m * n)
// 空间: O(m * n) -- 4个矩阵
// ============================================================
class Solution2 {
public:
    int maxKilledEnemies(vector<vector<char>>& grid) {
        int m = grid.size(), n = grid[0].size();
        // 四方向敌人数矩阵
        vector<vector<int>> left(m, vector<int>(n, 0));
        vector<vector<int>> right(m, vector<int>(n, 0));
        vector<vector<int>> up(m, vector<int>(n, 0));
        vector<vector<int>> down(m, vector<int>(n, 0));

        // 从左到右填 left
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 'W') {
                    left[i][j] = 0;
                } else {
                    left[i][j] = (grid[i][j] == 'E' ? 1 : 0)
                                 + (j > 0 ? left[i][j - 1] : 0);
                }
            }
        }

        // 从右到左填 right
        for (int i = 0; i < m; i++) {
            for (int j = n - 1; j >= 0; j--) {
                if (grid[i][j] == 'W') {
                    right[i][j] = 0;
                } else {
                    right[i][j] = (grid[i][j] == 'E' ? 1 : 0)
                                  + (j < n - 1 ? right[i][j + 1] : 0);
                }
            }
        }

        // 从上到下填 up
        for (int j = 0; j < n; j++) {
            for (int i = 0; i < m; i++) {
                if (grid[i][j] == 'W') {
                    up[i][j] = 0;
                } else {
                    up[i][j] = (grid[i][j] == 'E' ? 1 : 0)
                               + (i > 0 ? up[i - 1][j] : 0);
                }
            }
        }

        // 从下到上填 down
        for (int j = 0; j < n; j++) {
            for (int i = m - 1; i >= 0; i--) {
                if (grid[i][j] == 'W') {
                    down[i][j] = 0;
                } else {
                    down[i][j] = (grid[i][j] == 'E' ? 1 : 0)
                                 + (i < m - 1 ? down[i + 1][j] : 0);
                }
            }
        }

        // 对每个空格取四方向之和的最大值
        int ans = 0;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == '0') {
                    int total = left[i][j] + right[i][j]
                                + up[i][j] + down[i][j];
                    ans = max(ans, total);
                }
            }
        }
        return ans;
    }
};


// ============================================================
// 【解法对比】
//   解法1 (分段缓存):  O(mn) 时间, O(n) 空间 -- 面试首选
//   解法2 (四方向DP):  O(mn) 时间, O(mn) 空间 -- 更直观, 面试也可
//
// 【易错点】
//   1. 区段起点判断: 是 grid[i][j-1]=='W' 不是 grid[i][j]=='W'
//      当前格是墙时不需要重算，当前格是墙后面的第一格才需要重算
//   2. 只在 grid[i][j]=='0' 时统计: 炸弹只能放空地
//   3. 四方向DP中墙的处理: 遇墙归零，不是累加
//   4. colHits 跨行复用: 不要每行重置
//   5. 边界: j==0 时无需检查 grid[i][j-1], i==0 时无需检查 grid[i-1][j]
//
// 【面试追问】
//   Q1: 能否只用一个变量而不用 colHits 数组?
//       → 不行。行方向同一行内所有列共享一个 rowHits，但列方向
//         不同列的区段独立，必须每列各存一个值。
//
//   Q2: 如果矩阵非常大(比如 10000 x 10000)，有什么优化?
//       → 两种方法都是 O(mn)，瓶颈在 I/O。可以考虑分块处理
//         或并行化（按行/列独立计算）。
//
//   Q3: 如果炸弹可以穿透一堵墙呢?
//       → 需要改变区段逻辑: 不是遇墙停止，而是穿过一堵墙后
//         继续扫描。可以用类似的缓存思路但需要跟踪"穿墙次数"。
// ============================================================
