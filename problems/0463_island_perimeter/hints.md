# 463. Island Perimeter - 岛屿的周长

## 核心思路
每个陆地格子贡献 4 条边，每对相邻陆地共享 2 条边（各减 1）。周长 = 4 * 陆地数 - 2 * 相邻对数。

## 思维链
1. 单个陆地格子有 4 条边
2. 两个相邻陆地格子共享一条边界 -> 各减少 1 条贡献 -> 总共减 2
3. 遍历所有格子，计数陆地和相邻对数
4. 或者：对每个陆地格子，检查四个方向，遇到水或边界就 +1

## 解法概览表

| 解法 | 时间 | 空间 | 特点 |
|------|------|------|------|
| ⭐ 计数法（陆地-相邻） | O(m*n) | O(1) | 最简洁 |
| ⭐ 逐格检查边界 | O(m*n) | O(1) | 直观 |
| DFS/BFS | O(m*n) | O(m*n) | 杀鸡用牛刀 |

## 关键提示
1. 只需检查右方和下方邻居即可，避免重复计数相邻对
2. 公式：`perimeter = 4 * islands - 2 * neighbors`
3. 本题只有一个岛屿，无需 visited 数组

## 解法详解

### 解法一：计数法
```cpp
class Solution {
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
```

### 解法二：逐格检查四方向
```cpp
class Solution {
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
```

## 易错点
- ✗ 相邻对数只检查一个方向但忘记乘 2 -> ✓ 检查右和下，公式中 `*2` 补偿
- ✗ 认为需要 DFS 标记访问 -> ✓ 直接遍历即可，不需要连通分量
- ✗ 检查四方向时忘记边界判断 -> ✓ 越界也算一条周长边

## 面试追问

**Q1: 如果有多个岛屿，要求每个岛屿的周长怎么办？**
> 用 DFS/BFS 分别遍历每个连通分量，对每个分量内部用同样的计数法。

**Q2: 这个方法和 DFS 方法的优劣对比？**
> 计数法 O(1) 空间，DFS 需要 O(m*n) 栈/visited。本题只有一个岛且无需分组，计数法最优。

**Q3: 如果格子是六边形（六方向相邻），公式怎么变？**
> 单格 6 条边，每对相邻减 2，`perimeter = 6 * cells - 2 * adjacentPairs`。

## 相关题型
- [200. Number of Islands](https://leetcode.com/problems/number-of-islands/) - 岛屿计数
- [695. Max Area of Island](https://leetcode.com/problems/max-area-of-island/) - 最大面积
- [733. Flood Fill](https://leetcode.com/problems/flood-fill/) - 洪水填充
