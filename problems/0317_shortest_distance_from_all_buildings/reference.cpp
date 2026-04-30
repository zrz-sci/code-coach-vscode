/*
 * LeetCode 317: 离建筑物最近的距离 (Shortest Distance from All Buildings)
 *
 * 【题目本质】
 * 在网格中找一个空地，使得它到所有建筑的曼哈顿距离（BFS最短路）之和最小。
 * 空地必须能到达所有建筑才是合法候选。
 *
 * 【解法总览】
 * 解法1: 从空地出发BFS  — O(E×m×n) / O(m×n) — 最直觉但慢
 * 解法2: 从建筑出发BFS  — O(B×m×n) / O(m×n) — 面试首选
 * 解法3: 从建筑出发BFS+递减标记 — O(B×m×n) / O(m×n) — 最优，省空间+剪枝
 */

// ============================================================
// 解法1: 从空地出发BFS — 对每个空地做BFS找到所有建筑
// 时间: O(E × m × n), E = 空地数量，最坏 O(m²×n²)
// 空间: O(m × n)
//
// 【思路】
// 最直觉的做法：遍历每个空地，对它做一次BFS，
// 计算它到所有建筑的最短距离之和。取最小值。
//
// 瓶颈：空地数量可能远大于建筑数量(最多2500个空地)，
// 每个空地做一次完整BFS开销很大。
//
// 网格示意 (示例1):
//   1  0  2  0  1
//   0  0  0  0  0
//   0  0  1  0  0
//
// 对空地(1,2)做BFS:
//   到(0,0)=3步, 到(0,4)=3步, 到(2,2)=1步 → 总和=7
// ============================================================
class Solution1 {
public:
    int shortestDistance(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        int totalBuildings = 0;
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                if (grid[i][j] == 1) totalBuildings++;
        
        int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};
        int result = INT_MAX;
        
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] != 0) continue; // 只从空地出发
                
                vector<vector<bool>> visited(m, vector<bool>(n, false));
                queue<pair<int,int>> q;
                q.push({i, j});
                visited[i][j] = true;
                int dist = 0, totalDist = 0, reached = 0;
                
                while (!q.empty() && reached < totalBuildings) {
                    int sz = q.size();
                    dist++; // BFS层次=距离，每层+1
                    while (sz--) {
                        auto [x, y] = q.front(); q.pop();
                        for (auto& d : dirs) {
                            int nx = x + d[0], ny = y + d[1];
                            if (nx < 0 || nx >= m || ny < 0 || ny >= n) continue;
                            if (visited[nx][ny]) continue;
                            visited[nx][ny] = true;
                            if (grid[nx][ny] == 1) {
                                // 遇到建筑：累加距离，不入队(不可穿越)
                                totalDist += dist;
                                reached++;
                            } else if (grid[nx][ny] == 0) {
                                // 空地：继续扩展
                                q.push({nx, ny});
                            }
                            // grid[nx][ny]==2 是障碍，跳过
                        }
                    }
                }
                
                // 必须能到达所有建筑
                if (reached == totalBuildings)
                    result = min(result, totalDist);
            }
        }
        return result == INT_MAX ? -1 : result;
    }
};

// ============================================================
// 解法2: 从建筑出发BFS — 反转方向，从每栋建筑BFS到所有空地
// 时间: O(B × m × n), B = 建筑数量
// 空间: O(m × n) (distSum + reachCount + visited)
//
// 【思路】
// 解法1的瓶颈：空地可能非常多，每个空地做一次BFS浪费。
// 反转思路：从建筑出发做BFS，一次BFS就能算出该建筑到
// 所有可达空地的最短距离，累加到 distSum 数组上。
//
// 关键：用 reachCount[i][j] 记录空地(i,j)被几栋建筑访问过，
// 只有 reachCount == totalBuildings 的空地才是合法候选。
//
// BFS 扩展过程 (从建筑(0,0)出发):
//
//   B  1  X  .  .      B = 建筑(出发点)
//   1  2  3  4  5      数字 = BFS层次 = 距离
//   2  3  B  5  6      X = 障碍(不可穿越)
//                      B = 其他建筑(不可穿越)
//
// 从建筑(0,4)出发:
//   .  .  X  1  B
//   5  4  3  2  1
//   6  5  B  3  2
//
// 从建筑(2,2)出发:
//   .  .  X  .  .
//   2  2  .  2  2      (注意：从(2,2)出发到(1,2)=1步，
//   1  1  B  1  1       但到(0,1)无法直达因为(0,2)是障碍)
//
// distSum 累加后:
//   B  .  X  .  B
//   8  8  7  8  8      ← (1,2)=3+3+1=7 最小!
//   9  9  B  9  9
// ============================================================
class Solution2 {
public:
    int shortestDistance(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        int totalBuildings = 0;
        int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};
        
        // distSum[i][j] = 空地(i,j)到所有已处理建筑的距离之和
        // reachCount[i][j] = 有多少栋建筑的BFS能到达(i,j)
        vector<vector<int>> distSum(m, vector<int>(n, 0));
        vector<vector<int>> reachCount(m, vector<int>(n, 0));
        
        // 先统计建筑总数
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                if (grid[i][j] == 1) totalBuildings++;
        
        // 从每栋建筑出发BFS
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] != 1) continue; // 只从建筑出发
                
                // 标准层序BFS
                vector<vector<bool>> visited(m, vector<bool>(n, false));
                queue<pair<int,int>> q;
                q.push({i, j});
                visited[i][j] = true;
                int dist = 0;
                
                while (!q.empty()) {
                    int sz = q.size();
                    dist++;
                    while (sz--) {
                        auto [x, y] = q.front(); q.pop();
                        for (auto& d : dirs) {
                            int nx = x + d[0], ny = y + d[1];
                            if (nx < 0 || nx >= m || ny < 0 || ny >= n) continue;
                            if (visited[nx][ny]) continue;
                            // 只能走空地，建筑和障碍都不可穿越
                            if (grid[nx][ny] != 0) continue;
                            visited[nx][ny] = true;
                            distSum[nx][ny] += dist;     // 累加距离
                            reachCount[nx][ny]++;         // 可达计数+1
                            q.push({nx, ny});
                        }
                    }
                }
            }
        }
        
        // 在所有能到达全部建筑的空地中找最小距离和
        int result = INT_MAX;
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                if (grid[i][j] == 0 && reachCount[i][j] == totalBuildings)
                    result = min(result, distSum[i][j]);
        
        return result == INT_MAX ? -1 : result;
    }
};

// ============================================================
// 解法3: 从建筑出发BFS + 递减标记剪枝
// 时间: O(B × m × n), 实际更快(不可达格子被提前过滤)
// 空间: O(m × n) (仅 distSum，不需要 visited 和 reachCount)
//
// 【思路】
// 解法2每次BFS需要一个 visited 数组 + reachCount 数组。
// 利用一个巧妙的递减标记，把两者合并进 grid 本身：
//
// - 初始空地值 = 0
// - 第1轮BFS(从第1栋建筑出发): 只访问值==0的格子, 访问后改为-1
// - 第2轮BFS(从第2栋建筑出发): 只访问值==-1的格子, 访问后改为-2
// - 第k轮BFS: 只访问值==-(k-1)的格子, 访问后改为-k
//
// 为什么正确？
// 值为-(k-1)意味着该格子被前(k-1)栋建筑都访问过，
// 最后一轮访问的格子就是被所有建筑都可达的。
//
// 递减标记过程 (示例):
//   轮次    emptyLandMark    访问条件      访问后改为
//    1          0           grid==0        grid=-1
//    2         -1           grid==-1       grid=-2
//    3         -2           grid==-2       grid=-3
//
// 天然实现了:
// 1. visited功能: 格子被改值后不会被同一轮重复访问
// 2. reachCount功能: 只有被所有前序轮次访问过的格子才参与当前轮
// ============================================================
class Solution3 {
public:
    int shortestDistance(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};
        
        vector<vector<int>> distSum(m, vector<int>(n, 0));
        int result = INT_MAX;
        int emptyLandMark = 0; // 当前轮次期望的空地值
        
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] != 1) continue; // 只从建筑出发
                
                // 每轮重置result，最终只有最后一轮的result是完整的
                // 因为最后一轮才是所有建筑都处理完后的结果
                result = INT_MAX;
                queue<pair<int,int>> q;
                q.push({i, j});
                int dist = 0;
                
                while (!q.empty()) {
                    int sz = q.size();
                    dist++;
                    while (sz--) {
                        auto [x, y] = q.front(); q.pop();
                        for (auto& d : dirs) {
                            int nx = x + d[0], ny = y + d[1];
                            if (nx < 0 || nx >= m || ny < 0 || ny >= n) continue;
                            // 核心：只访问值等于 emptyLandMark 的格子
                            if (grid[nx][ny] != emptyLandMark) continue;
                            grid[nx][ny]--;           // 递减标记
                            distSum[nx][ny] += dist;  // 累加距离
                            q.push({nx, ny});
                            // 在BFS过程中顺便更新result
                            result = min(result, distSum[nx][ny]);
                        }
                    }
                }
                emptyLandMark--; // 下一轮期望值递减
            }
        }
        return result == INT_MAX ? -1 : result;
    }
};

// ============================================================
// 【解法对比】
//
// 解法1 vs 解法2:
//   - 核心区别：BFS的出发点不同。从空地出发 → 从建筑出发
//   - 建筑数B通常远小于空地数E，所以解法2更快
//   - 选择：面试中直接说解法2，解法1作为暴力baseline提一句即可
//
// 解法2 vs 解法3:
//   - 解法3省掉了 visited 和 reachCount 两个额外数组
//   - 解法3有天然剪枝：不可达的格子在后续轮次被自动跳过
//   - 代价：修改了原始grid（如果不允许修改需要拷贝）
//   - 选择：面试写解法2更清晰，解法3作为follow-up优化
//
// 【易错点】
//
// 1. BFS只能走空地，不能穿过其他建筑
//    ✗ if (!visited[nx][ny]) q.push({nx, ny});
//    ✓ if (!visited[nx][ny] && grid[nx][ny] == 0) q.push({nx, ny});
//    建筑(1)和障碍(2)都是不可穿越的！
//
// 2. 忘记检查 reachCount == totalBuildings
//    ✗ result = min(result, distSum[i][j]);  // 可能该空地到不了某栋建筑
//    ✓ if (reachCount[i][j] == totalBuildings) result = min(result, distSum[i][j]);
//
// 3. 解法3中result在每轮BFS开始时必须重置为INT_MAX
//    如果不重置，上一轮的result是只到达部分建筑的值，会导致错误
//
// 4. dist变量必须按层递增，不是每次出队递增
//    ✗ while(!q.empty()) { auto [x,y]=q.front(); q.pop(); dist++; ... }
//    ✓ while(!q.empty()) { int sz=q.size(); dist++; while(sz--) { ... } }
//    这是层序BFS的经典错误
//
// 【面试追问】
//
// Q1: 为什么从建筑出发而不是从空地出发？
// → 建筑数量通常远少于空地，BFS次数=出发点数×O(mn)，出发点越少越快。
//    极端情况：如果建筑数>空地数(虽然罕见)，从空地出发反而更快。
//
// Q2: 如果有建筑被障碍完全隔开，怎么高效检测？
// → 解法2中：某栋建筑BFS结束后，检查它访问到的空地数是否为0，
//    如果是则直接返回-1（不可能有空地到达所有建筑）。
//    解法3中：如果某轮BFS的result仍是INT_MAX，说明没有空地
//    被该建筑可达且同时被之前所有建筑可达，直接返回-1。
//
// Q3: 递减标记法为什么不需要visited数组？
// → 格子值在被访问后立刻从emptyLandMark改为emptyLandMark-1，
//    同一轮BFS中其他路径再遇到它时值已经变了，不等于emptyLandMark，
//    自然不会重复访问。一个值同时充当了"visited标记"和"可达计数"。
//
// Q4: 此题和286. Walls and Gates有什么联系？
// → 286是多源BFS找最近的一个源的距离，本题是多源BFS但要累加所有源的距离。
//    286可以所有门同时入队做一次BFS，本题必须每栋建筑单独BFS后累加。
// ============================================================
