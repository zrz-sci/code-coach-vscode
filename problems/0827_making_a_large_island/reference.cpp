/*
 * LeetCode 827: 最大人工岛 (Making A Large Island)
 *
 * 【题目本质】
 * 在 n×n 的 0/1 网格中，最多将一个 0 翻转为 1，求能形成的最大岛屿面积。
 * 核心：预处理所有岛屿的编号和面积，然后枚举每个 0，看它能连接哪些不同的岛。
 *
 * 【解法总览】
 * 解法1: 暴力 BFS — O(n⁴) / O(n²) — 最直觉，会超时
 * 解法2: 岛屿编号 + 枚举 0 (DFS) — O(n²) / O(n²) — 面试首选 ⭐
 * 解法3: Union-Find — O(n²·α(n²)) / O(n²) — 并查集实现，加分项
 */

// ============================================================
// 解法1: 暴力 BFS — 对每个 0 翻转后重新算最大岛
// 时间: O(n⁴)  空间: O(n²)
//
// 【思路】
// 最直觉的做法：遍历每个 0，临时变成 1，然后 BFS 求包含该点的
// 连通分量面积。取所有尝试的最大值。
//
// 瓶颈：每个 0 都要 BFS 一遍整个网格，总共 O(n²) 个 0 × O(n²) BFS = O(n⁴)。
// n=500 时完全不可行，但面试中能说出这个思路证明理解了题意。
// ============================================================
class Solution1 {
public:
    int largestIsland(vector<vector<int>>& grid) {
        int n = grid.size(), ans = 0;
        bool hasZero = false;
        int dx[] = {0,0,1,-1}, dy[] = {1,-1,0,0};
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 0) {
                    hasZero = true;
                    grid[i][j] = 1; // 临时翻转
                    
                    // BFS 求包含 (i,j) 的连通分量大小
                    vector<vector<bool>> visited(n, vector<bool>(n, false));
                    queue<pair<int,int>> q;
                    q.push({i, j});
                    visited[i][j] = true;
                    int area = 0;
                    while (!q.empty()) {
                        auto [x, y] = q.front(); q.pop();
                        area++;
                        for (int d = 0; d < 4; d++) {
                            int nx = x+dx[d], ny = y+dy[d];
                            if (nx>=0 && nx<n && ny>=0 && ny<n 
                                && !visited[nx][ny] && grid[nx][ny]==1) {
                                visited[nx][ny] = true;
                                q.push({nx, ny});
                            }
                        }
                    }
                    ans = max(ans, area);
                    grid[i][j] = 0; // 恢复原状
                }
            }
        }
        // 全是1时没有0可翻，答案就是n*n
        return hasZero ? ans : n * n;
    }
};

// ============================================================
// 解法2: 岛屿编号 + 枚举 0 (DFS 着色) — 面试首选 ⭐
// 时间: O(n²)  空间: O(n²)
//
// 【思路】
// 暴力解的瓶颈：每次翻转 0 后都要重新计算岛屿面积，但岛屿本身不变！
// 
// 优化思路：两遍扫描
//   第一遍：DFS 给每个岛屿染上不同编号(从2开始)，记录 {编号: 面积}
//   第二遍：枚举每个 0，查看它四个邻居分别属于哪些不同岛屿，
//           面积之和 + 1(自身) 就是翻转该 0 后的岛屿大小
//
// 【关键】用 set 去重 —— 一个 0 的多个邻居可能属于同一个岛
//
// 示例:
//   原始网格:        着色后:         面积表:
//   1 1 0            2 2 0           {2: 3, 3: 1}
//   1 0 0     →      2 0 0
//   0 0 1            0 0 3
//
//   枚举 (0,2)=0:
//     右无，下(1,2)=0，左(0,1)=2，上无 → 邻居岛集合 {2}
//     合并面积 = area[2]+1 = 3+1 = 4
//
//   枚举 (1,1)=0:
//     上(0,1)=2, 右(1,2)=0, 下(2,1)=0, 左(1,0)=2 → {2}
//     合并面积 = 3+1 = 4
//
//   枚举 (2,0)=0:
//     上(1,0)=2, 右(2,1)=0, 下无, 左无 → {2}
//     合并面积 = 3+1 = 4
//
//   枚举 (2,1)=0:
//     上(1,1)=0, 右(2,2)=3, 下无, 左(2,0)=0 → {3}
//     合并面积 = 1+1 = 2
//
//   枚举 (1,2)=0:
//     上(0,2)=0, 右无, 下(2,2)=3, 左(1,1)=0 → {3}
//     合并面积 = 1+1 = 2
//
//   ans = max(3, 4, 4, 4, 2, 2) = 4
// ============================================================
class Solution2 {
public:
    int largestIsland(vector<vector<int>>& grid) {
        int n = grid.size();
        int dx[] = {0,0,1,-1}, dy[] = {1,-1,0,0};
        
        // ---- Step 1: DFS 着色 ----
        // 编号从2开始，避免和原始值0/1混淆
        unordered_map<int, int> area; // 岛屿编号 → 面积
        int colorId = 2;
        
        // DFS: 把 (x,y) 所在连通分量全部染成 color，返回面积
        function<int(int, int, int)> dfs = [&](int x, int y, int color) -> int {
            // 越界、海洋、已着色 → 终止
            if (x < 0 || x >= n || y < 0 || y >= n) return 0;
            if (grid[x][y] != 1) return 0; // 只处理未着色的陆地(值恰好为1)
            
            grid[x][y] = color; // 着色，同时起到 visited 的作用
            int size = 1;
            for (int d = 0; d < 4; d++) {
                size += dfs(x + dx[d], y + dy[d], color);
            }
            return size;
        };
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 1) { // 发现未着色的陆地
                    int size = dfs(i, j, colorId);
                    area[colorId] = size;
                    colorId++;
                }
            }
        }
        
        // ---- Step 2: 枚举每个 0，计算翻转后的最大面积 ----
        int ans = 0;
        
        // 用已有最大岛屿面积初始化(处理全1矩阵: 没有0可翻转)
        for (auto& [id, sz] : area) {
            ans = max(ans, sz);
        }
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 0) {
                    // 收集四个邻居所属的不同岛屿编号
                    unordered_set<int> neighbors;
                    for (int d = 0; d < 4; d++) {
                        int ni = i + dx[d], nj = j + dy[d];
                        if (ni >= 0 && ni < n && nj >= 0 && nj < n 
                            && grid[ni][nj] > 1) { // >1 表示已着色的岛屿
                            neighbors.insert(grid[ni][nj]);
                        }
                    }
                    int total = 1; // 翻转的这个0本身贡献面积1
                    for (int id : neighbors) {
                        total += area[id]; // 每个不同岛屿的面积只加一次
                    }
                    ans = max(ans, total);
                }
            }
        }
        
        return ans;
    }
};

// ============================================================
// 解法3: Union-Find (并查集) — 思路相同，实现方式不同
// 时间: O(n²·α(n²)) ≈ O(n²)  空间: O(n²)
//
// 【思路】
// 用并查集维护连通性:
//   Step1: 遍历网格，将所有相邻的 1 合并到同一个集合，并查集维护集合大小
//   Step2: 枚举每个 0，查它四个邻居的根节点(去重)，累加各集合大小+1
//
// 并查集 vs DFS着色:
// - 功能等价，都是"预处理连通分量+查表"
// - 并查集更适合动态场景(如陆地在线添加)
// - DFS着色对本题更直观
// ============================================================
class Solution3 {
public:
    vector<int> parent, rnk, sz;
    
    int find(int x) {
        // 路径压缩: 递归把沿途节点直接挂到根
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    
    void unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return; // 已在同一集合
        // 按秩合并: 小树挂到大树上
        if (rnk[a] < rnk[b]) swap(a, b);
        parent[b] = a;
        sz[a] += sz[b]; // 合并大小
        if (rnk[a] == rnk[b]) rnk[a]++;
    }
    
    int largestIsland(vector<vector<int>>& grid) {
        int n = grid.size();
        int total = n * n;
        parent.resize(total);
        rnk.resize(total, 0);
        sz.resize(total, 1);
        iota(parent.begin(), parent.end(), 0); // parent[i] = i
        
        // Step 1: 合并相邻的 1
        // 只向右和下合并，避免重复 unite（向上和左会在之前的格子处理过）
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 1) {
                    if (j + 1 < n && grid[i][j + 1] == 1)
                        unite(i * n + j, i * n + j + 1);
                    if (i + 1 < n && grid[i + 1][j] == 1)
                        unite(i * n + j, (i + 1) * n + j);
                }
            }
        }
        
        // Step 2: 枚举每个 0
        int ans = 0;
        
        // 初始化: 取已有最大集合的大小(处理全1情况)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (grid[i][j] == 1)
                    ans = max(ans, sz[find(i * n + j)]);
        
        int dirs[][2] = {{0,1},{0,-1},{1,0},{-1,0}};
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 0) {
                    // 收集四个邻居的根节点（去重）
                    unordered_set<int> roots;
                    for (auto& [di, dj] : dirs) {
                        int ni = i + di, nj = j + dj;
                        if (ni >= 0 && ni < n && nj >= 0 && nj < n 
                            && grid[ni][nj] == 1) {
                            roots.insert(find(ni * n + nj));
                        }
                    }
                    int merged = 1; // 翻转的0本身
                    for (int r : roots) {
                        merged += sz[r];
                    }
                    ans = max(ans, merged);
                }
            }
        }
        
        return ans;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法 | 时间 | 空间 | 核心思想 | 适用场景 |
// |------|------|------|----------|----------|
// | 暴力BFS | O(n⁴) | O(n²) | 每个0重新算 | 说明思路 |
// | DFS着色 | O(n²) | O(n²) | 预处理+查表 | 面试首选 ⭐ |
// | Union-Find | O(n²α) | O(n²) | 并查集维护 | 动态扩展 |
//
// 解法1→2 的关键优化: 岛屿不变，面积只需算一次，翻转0时O(1)查表
// 解法2 vs 3: 功能等价，DFS着色更直观，Union-Find更适合动态场景
//
// 【易错点】
// 1. 同一岛屿面积重复累加:
//    ✗ 直接加四个邻居面积 (左邻和上邻可能是同一个岛)
//    ✓ 用 set 收集不同编号/根节点，每个只加一次
//
// 2. 着色编号与原始值冲突:
//    ✗ colorId 从 1 开始 → grid[x][y] != 1 判断失效，已着色的也是1
//    ✓ colorId 从 2 开始，1 专门表示"未着色的陆地"
//
// 3. 全1矩阵时答案为0:
//    ✗ 只在枚举 0 的循环里更新 ans，没有 0 则 ans=0
//    ✓ 用所有岛屿最大面积初始化 ans
//
// 4. DFS 终止条件写成 grid[x][y]==0:
//    ✗ if (grid[x][y] == 0) return 0; → 已着色(值≥2)的格子会被重复访问
//    ✓ if (grid[x][y] != 1) return 0; → 只处理值恰好为1的未着色陆地
//
// 5. Union-Find 构建时用4方向:
//    ✗ 4方向会导致 unite(a,b) 和 unite(b,a) 重复调用（虽然不影响正确性但浪费）
//    ✓ 构建时只向右+向下(2方向)，查询时才用4方向
//
// 【面试追问】
// Q1: 暴力解的瓶颈在哪？
//     → O(n⁴)，每翻转一个0都要重新BFS整个网格，但岛屿结构不变，大量重复计算。
//
// Q2: 如何优化到 O(n²)？
//     → 预处理: DFS着色+记录面积。枚举0时O(1)查表合并相邻不同岛屿面积。
//
// Q3: 如果可以翻转 k 个 0 呢？
//     → k=1 有这个巧妙解法。k>1时问题复杂很多，可能需要BFS从岛屿边界扩展k步，
//       或者在小k时枚举组合。没有简洁的O(n²)通解。
//
// Q4: DFS 着色 vs Union-Find 各自优劣？
//     → DFS着色: 直观、代码简洁，适合静态一次性场景。
//       Union-Find: 适合动态添加陆地(如LeetCode 305)，支持增量合并。
//       本题两者复杂度相当，面试推荐DFS版本。
// ============================================================
