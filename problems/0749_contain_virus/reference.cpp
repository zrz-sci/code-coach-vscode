// 749. Contain Virus
// 难度: Hard
// 标签: DFS, BFS, Array, Matrix, Simulation

// ============================================================
// 解法一：BFS 模拟 + 贪心 - 推荐
// 时间: O((m*n)^2)  空间: O(m*n)
// ============================================================
// 每轮：
// 1. BFS 找所有感染区域，统计各区域的威胁数和墙数
// 2. 隔离威胁最大的区域
// 3. 其余区域扩散

class Solution {
public:
    int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};

    int containVirus(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        int totalWalls = 0;

        while (true) {
            // 每轮重新找所有活跃感染区域
            vector<vector<bool>> visited(m, vector<bool>(n, false));

            // 每个区域的信息
            vector<vector<pair<int,int>>> regionCells;    // 区域内的感染格子
            vector<set<pair<int,int>>> regionThreatened;  // 威胁的未感染格子（去重）
            vector<int> regionWalls;                       // 隔离需要的墙数

            // BFS 找所有感染区域
            for (int i = 0; i < m; ++i) {
                for (int j = 0; j < n; ++j) {
                    if (grid[i][j] == 1 && !visited[i][j]) {
                        // 新的感染区域
                        vector<pair<int,int>> cells;
                        set<pair<int,int>> threatened;
                        int walls = 0;

                        queue<pair<int,int>> q;
                        q.push({i, j});
                        visited[i][j] = true;

                        while (!q.empty()) {
                            auto [x, y] = q.front();
                            q.pop();
                            cells.push_back({x, y});

                            for (auto& d : dirs) {
                                int nx = x + d[0], ny = y + d[1];
                                if (nx < 0 || nx >= m || ny < 0 || ny >= n) continue;

                                if (grid[nx][ny] == 0) {
                                    // 未感染邻居：威胁（set去重）+ 墙数（不去重）
                                    threatened.insert({nx, ny});
                                    walls++;
                                } else if (grid[nx][ny] == 1 && !visited[nx][ny]) {
                                    visited[nx][ny] = true;
                                    q.push({nx, ny});
                                }
                                // grid[nx][ny] == -1 是已隔离区域，跳过
                            }
                        }

                        if (!threatened.empty()) {
                            regionCells.push_back(cells);
                            regionThreatened.push_back(threatened);
                            regionWalls.push_back(walls);
                        }
                    }
                }
            }

            // 终止条件：没有活跃的威胁区域
            if (regionCells.empty()) break;

            // 找威胁最大的区域
            int maxThreat = 0, maxIdx = 0;
            for (int r = 0; r < (int)regionCells.size(); ++r) {
                if ((int)regionThreatened[r].size() > maxThreat) {
                    maxThreat = regionThreatened[r].size();
                    maxIdx = r;
                }
            }

            // 隔离威胁最大的区域：标记为 -1
            totalWalls += regionWalls[maxIdx];
            for (auto& [x, y] : regionCells[maxIdx]) {
                grid[x][y] = -1;
            }

            // 其余区域扩散
            for (int r = 0; r < (int)regionCells.size(); ++r) {
                if (r == maxIdx) continue;  // 跳过被隔离的
                for (auto& [x, y] : regionThreatened[r]) {
                    grid[x][y] = 1;
                }
            }
        }

        return totalWalls;
    }
};

// ============================================================
// 解法二：DFS 模拟 + 贪心
// 时间: O((m*n)^2)  空间: O(m*n)
// ============================================================
// 用 DFS 代替 BFS 找连通分量，逻辑相同

class Solution {
public:
    int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};
    int m, n;

    void dfs(vector<vector<int>>& grid, vector<vector<bool>>& visited,
             int x, int y,
             vector<pair<int,int>>& cells,
             set<pair<int,int>>& threatened,
             int& walls) {
        visited[x][y] = true;
        cells.push_back({x, y});

        for (auto& d : dirs) {
            int nx = x + d[0], ny = y + d[1];
            if (nx < 0 || nx >= m || ny < 0 || ny >= n) continue;

            if (grid[nx][ny] == 0) {
                threatened.insert({nx, ny});
                walls++;
            } else if (grid[nx][ny] == 1 && !visited[nx][ny]) {
                dfs(grid, visited, nx, ny, cells, threatened, walls);
            }
        }
    }

    int containVirus(vector<vector<int>>& grid) {
        m = grid.size();
        n = grid[0].size();
        int totalWalls = 0;

        while (true) {
            vector<vector<bool>> visited(m, vector<bool>(n, false));
            vector<vector<pair<int,int>>> allCells;
            vector<set<pair<int,int>>> allThreatened;
            vector<int> allWalls;

            for (int i = 0; i < m; ++i) {
                for (int j = 0; j < n; ++j) {
                    if (grid[i][j] == 1 && !visited[i][j]) {
                        vector<pair<int,int>> cells;
                        set<pair<int,int>> threatened;
                        int walls = 0;

                        dfs(grid, visited, i, j, cells, threatened, walls);

                        if (!threatened.empty()) {
                            allCells.push_back(cells);
                            allThreatened.push_back(threatened);
                            allWalls.push_back(walls);
                        }
                    }
                }
            }

            if (allCells.empty()) break;

            // 找威胁最大的区域
            int maxIdx = 0;
            for (int r = 1; r < (int)allCells.size(); ++r) {
                if (allThreatened[r].size() > allThreatened[maxIdx].size()) {
                    maxIdx = r;
                }
            }

            // 隔离
            totalWalls += allWalls[maxIdx];
            for (auto& [x, y] : allCells[maxIdx]) {
                grid[x][y] = -1;
            }

            // 扩散
            for (int r = 0; r < (int)allCells.size(); ++r) {
                if (r == maxIdx) continue;
                for (auto& [x, y] : allThreatened[r]) {
                    grid[x][y] = 1;
                }
            }
        }

        return totalWalls;
    }
};

// ============================================================
// 测试用例分析
// ============================================================
// 输入: [[0,1,0,0,0,0,0,1],
//        [0,1,0,0,0,0,0,1],
//        [0,0,0,0,0,0,0,1],
//        [0,0,0,0,0,0,0,0]]
//
// 第1轮:
//   区域A: (0,1),(1,1) -> 威胁 {(0,0),(0,2),(1,0),(1,2),(2,1)} = 5个
//                          墙数 = 5 (每个威胁格子恰好一面墙)
//   区域B: (0,7),(1,7),(2,7) -> 威胁 {(0,6),(1,6),(2,6),(3,7)} = 4个
//                                墙数 = 5
//   区域A威胁更多(5>4)，隔离A，花费5面墙
//   区域B扩散：(0,6),(1,6),(2,6),(3,7) 变为 1
//
// 第2轮:
//   区域B(扩大后): 威胁新的邻居
//   隔离B，花费5面墙
//
// 总计: 10  正确
//
// 输入: [[1,1,1],[1,0,1],[1,1,1]]
// 只有一个区域，威胁中心的 (1,1)
// 墙数 = 4（上下左右4面墙）
// 输出: 4  正确
//
// 输入: [[1,1,1,0,0,0,0,0,0],
//        [1,0,1,0,1,1,1,1,1],
//        [1,1,1,0,0,0,0,0,0]]
// 第1轮:
//   左区域威胁: (1,1) = 1个, 墙数 = 4
//   右区域威胁: 多个 0 格子, 墙数更多
//   右区域威胁更大，隔离右区域
//   左区域扩散占领 (1,1)
// 第2轮:
//   左区域（已扩大）被完全包围，隔离需要 2 面新墙
// 总计: 11 + 2 = 13  正确
