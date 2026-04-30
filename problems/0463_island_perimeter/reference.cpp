// 【题目本质】计算网格中单个岛屿的边界长度
// 【解法总览】解法1: 计数公式 O(mn)/O(1) | 解法2: 逐格检查边界 O(mn)/O(1)

// ===================== Solution 1: 陆地数 - 相邻对数 =====================
// 思路：perimeter = 4 * islands - 2 * neighbors
//       只检查右方和下方避免重复计数
// 时间 O(m*n)，空间 O(1)
class Solution1 {
public:
    int islandPerimeter(vector<vector<int>>& grid) {
        int islands = 0, neighbors = 0;
        int m = grid.size(), n = grid[0].size();
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 1) {
                    islands++;
                    if (i + 1 < m && grid[i+1][j] == 1) neighbors++;
                    if (j + 1 < n && grid[i][j+1] == 1) neighbors++;
                }
            }
        }
        return 4 * islands - 2 * neighbors;
    }
};

// ===================== Solution 2: 逐格检查四方向 =====================
// 思路：对每个陆地格子，四个方向遇到水或边界就 +1
// 时间 O(m*n)，空间 O(1)
class Solution2 {
public:
    int islandPerimeter(vector<vector<int>>& grid) {
        int perimeter = 0;
        int m = grid.size(), n = grid[0].size();
        int dx[] = {0, 0, 1, -1};
        int dy[] = {1, -1, 0, 0};
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 1) {
                    for (int d = 0; d < 4; d++) {
                        int ni = i + dx[d], nj = j + dy[d];
                        if (ni < 0 || ni >= m || nj < 0 || nj >= n || grid[ni][nj] == 0) {
                            perimeter++;
                        }
                    }
                }
            }
        }
        return perimeter;
    }
};

// 【解法对比】
// Solution1: 公式法，代码最短，只需检查两个方向
// Solution2: 更直观，每个格子检查四方向，逻辑清晰

// 【易错点】
// 1. 相邻对重复计数 -> 只检查右和下（或左和上）
// 2. 用 DFS 但本题不需要 -> 简单遍历即可
// 3. 边界条件：越界时也要贡献周长

// 【面试追问】
// Q1: 多个岛屿各自周长？-> DFS 分连通分量 + 计数法
// Q2: 计数法为什么是 -2？-> 每对相邻各减 1 条边，共减 2
// Q3: 能否不遍历全部格子？-> 不能，最坏情况全是陆地
