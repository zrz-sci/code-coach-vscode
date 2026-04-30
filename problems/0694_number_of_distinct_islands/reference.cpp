/*
 * LeetCode 694: 不同岛屿的数量 (Number of Distinct Islands)
 *
 * 【题目本质】
 * 给 m×n 的 01 网格，找出所有岛屿（4连通的1），统计"平移后形状不同"的岛屿个数。
 * 核心问题：如何给岛屿形状一个平移不变的唯一编码？
 *
 * 【解法总览】
 * 解法1: DFS + 相对坐标集合 — O(m·n·k·logk) / O(m·n) — 最直观
 * 解法2: DFS + 路径签名    — O(m·n)          / O(m·n) — 最简洁
 * 解法3: BFS + 相对坐标集合 — O(m·n·k·logk) / O(m·n) — 无递归栈风险
 *
 * 其中 k 是单个岛屿大小，最坏 k = m·n
 */

// ============================================================
// 解法1: DFS + 相对坐标集合
// 时间: O(m·n·k·logk)  空间: O(m·n)
//
// 【思路】
// "形状相同"= 平移后重合 = 所有格子相对于某个锚点的偏移量集合相同。
// 对每个岛屿：
//   1) DFS 收集所有格子坐标
//   2) 减去锚点(DFS起点)得到相对坐标
//   3) 排序后作为形状签名
//   4) 放入 set 去重
//
// 示例:
//   原始网格:                岛1的格子:         归一化(减去(0,0)):
//   1 1 0 0 0               (0,0)(0,1)          (0,0)(0,1)
//   1 1 0 0 0               (1,0)(1,1)          (1,0)(1,1)
//   0 0 0 1 1
//   0 0 0 1 1               岛2的格子:         归一化(减去(2,3)):
//                            (2,3)(2,4)          (0,0)(0,1)
//                            (3,3)(3,4)          (1,0)(1,1)
//   两个岛归一化后相同 → 形状相同 → 答案=1
// ============================================================
class Solution1 {
public:
    int numDistinctIslands(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        set<vector<pair<int,int>>> shapes;
        
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 1) {
                    vector<pair<int,int>> shape;
                    dfs(grid, i, j, i, j, shape);
                    // 排序保证相同形状无论DFS访问顺序如何，签名一致
                    sort(shape.begin(), shape.end());
                    shapes.insert(shape);
                }
            }
        }
        return shapes.size();
    }
    
private:
    void dfs(vector<vector<int>>& grid, int r, int c,
             int r0, int c0, vector<pair<int,int>>& shape) {
        int m = grid.size(), n = grid[0].size();
        if (r < 0 || r >= m || c < 0 || c >= n || grid[r][c] != 1) return;
        
        grid[r][c] = 0;  // 标记已访问，避免额外 visited 数组
        // 存储相对于锚点 (r0,c0) 的偏移 — 消除平移差异
        shape.push_back({r - r0, c - c0});
        
        dfs(grid, r + 1, c, r0, c0, shape);
        dfs(grid, r - 1, c, r0, c0, shape);
        dfs(grid, r, c + 1, r0, c0, shape);
        dfs(grid, r, c - 1, r0, c0, shape);
    }
};


// ============================================================
// 解法2: DFS + 路径签名（方向序列 + 回溯标记）
// 时间: O(m·n)  空间: O(m·n)
//
// 【思路】
// 解法1需要对坐标排序。能否在DFS过程中直接产生确定性签名？
// 
// 关键观察：如果两个岛屿形状相同，从各自"第一个被访问的格子"出发，
// 按固定的方向顺序（下上右左）做DFS，遍历路径完全相同。
// 
// 把每步的方向编码为字符：D(下) U(上) R(右) L(左) S(起点)
// 
// ★ 为什么必须加回溯标记 'B'？
//
//   岛A:     岛B:
//   1 0      1 1
//   1 1      1 0
//
//   不加回溯标记:
//     岛A从(0,0)出发: S → D(下到(1,0)) → R(右到(1,1))       = "SDR"
//     岛B从(0,0)出发: S → D(下到(1,0)) → (回到(0,0)) → R(右到(0,1)) 
//                     如果不记录回溯 = "SDR"  ← 冲突!!
//
//   加上回溯标记 'B':
//     岛A: "S D R B B B"
//     岛B: "S D B R B B"  ← 不同了！正确区分！
//
// 这和二叉树序列化要记录null节点才能唯一确定结构是同一个道理。
// ============================================================
class Solution2 {
public:
    int numDistinctIslands(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        set<string> shapes;
        
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 1) {
                    string path;
                    dfs(grid, i, j, 'S', path);
                    shapes.insert(path);
                }
            }
        }
        return shapes.size();
    }

private:
    void dfs(vector<vector<int>>& grid, int r, int c,
             char dir, string& path) {
        int m = grid.size(), n = grid[0].size();
        if (r < 0 || r >= m || c < 0 || c >= n || grid[r][c] != 1) return;
        
        grid[r][c] = 0;
        path += dir;  // 记录进入当前格子的方向
        
        // 四个方向顺序必须固定，这样相同形状产生相同字符串
        dfs(grid, r + 1, c, 'D', path);  // 下
        dfs(grid, r - 1, c, 'U', path);  // 上
        dfs(grid, r, c + 1, 'R', path);  // 右
        dfs(grid, r, c - 1, 'L', path);  // 左
        
        // ★ 回溯标记 — 没有这行，不同形状可能产生相同路径
        path += 'B';
    }
};


// ============================================================
// 解法3: BFS + 相对坐标集合
// 时间: O(m·n·k·logk)  空间: O(m·n)
//
// 【思路】
// 和解法1完全相同的编码思路，只是用BFS替代DFS。
// 好处：没有递归栈溢出风险（对于超大网格更安全）。
// BFS的访问顺序和DFS不同，但排序后的坐标集合是相同的，
// 所以结果正确。
//
// BFS扩展过程示例 (岛屿起点(0,0)):
//   队列:     [(0,0)]
//   弹出(0,0): shape加入(0,0), 入队邻居(0,1)(1,0)
//   队列:     [(0,1),(1,0)]
//   弹出(0,1): shape加入(0,1), 入队邻居(无新的)
//   弹出(1,0): shape加入(1,0), 入队邻居(1,1)
//   弹出(1,1): shape加入(1,1)
//   shape = [(0,0),(0,1),(1,0),(1,1)] — 已经是排序的
// ============================================================
class Solution3 {
public:
    int numDistinctIslands(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
        set<vector<pair<int,int>>> shapes;
        
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] != 1) continue;
                
                vector<pair<int,int>> shape;
                queue<pair<int,int>> q;
                q.push({i, j});
                // ★ 入队时就标记，而非出队时标记
                // 否则同一格子可能被多次入队，导致重复坐标和性能浪费
                grid[i][j] = 0;
                
                while (!q.empty()) {
                    auto [r, c] = q.front();
                    q.pop();
                    shape.push_back({r - i, c - j});  // 相对于起点(i,j)的偏移
                    
                    for (auto& d : dirs) {
                        int nr = r + d[0], nc = c + d[1];
                        if (nr >= 0 && nr < m && nc >= 0 && nc < n 
                            && grid[nr][nc] == 1) {
                            grid[nr][nc] = 0;  // 入队时标记
                            q.push({nr, nc});
                        }
                    }
                }
                
                sort(shape.begin(), shape.end());
                shapes.insert(shape);
            }
        }
        return shapes.size();
    }
};


// ============================================================
// 【解法对比】
//
// | 维度         | 解法1(DFS+坐标)  | 解法2(DFS+路径签名) | 解法3(BFS+坐标) |
// |-------------|------------------|---------------------|-----------------|
// | 编码方式     | 相对坐标排序      | 方向字符串           | 相对坐标排序     |
// | 需要排序?    | 是 O(k logk)     | 否                  | 是              |
// | 需要回溯标记? | 否               | 必须!               | 否              |
// | 递归栈风险   | 有               | 有                  | 无              |
// | 代码简洁度   | 中等             | 最简洁              | 稍长            |
//
// 面试推荐: 解法2(路径签名)代码最短思路最巧，解法1(坐标)最直观易证明。
//
// ============================================================
// 【易错点】
//
// 1. 路径签名漏掉回溯标记:
//    ✗ dfs结束后不加 path += 'B';
//    ✓ 必须在四方向递归后加 path += 'B';
//    原因: 不同形状的分支结构在没有回溯标记时可能产生相同前序序列
//
// 2. BFS中出队才标记(而非入队时标记):
//    ✗ auto [r,c] = q.front(); q.pop(); grid[r][c] = 0;  
//    ✓ grid[nr][nc] = 0; q.push({nr,nc});  // 入队时标记
//    原因: 出队才标记会导致同一格子被多个邻居重复入队
//
// 3. 相对坐标没排序就插入set:
//    ✗ shapes.insert(shape); // 未排序
//    ✓ sort(shape.begin(), shape.end()); shapes.insert(shape);
//    原因: BFS/DFS访问顺序不确定，不排序可能同形状产生不同序列
//
// 4. DFS方向顺序不固定(路径签名法):
//    ✗ 不同岛屿DFS时用不同的方向顺序
//    ✓ 所有DFS统一用 下上右左 (或任意固定顺序)
//    原因: 方向顺序不固定则相同形状产生不同路径字符串
//
// ============================================================
// 【面试追问 — 递进链】
//
// Q1(基础): "怎么判断两个岛屿形状相同？"
//   → 坐标归一化(减去锚点)后集合相同，或DFS路径签名相同。
//
// Q2(编码正确性): "路径签名为什么需要回溯标记？"
//   → 类比二叉树序列化: 不记录null节点，前序序列不能唯一确定树结构。
//     回溯标记区分了"从哪个分支返回"，是编码分支结构的关键。
//
// Q3(变体): "如果允许旋转和翻转呢？(LeetCode 711)"
//   → 对每个岛屿生成8种变换(4旋转×2翻转)的归一化坐标，
//     取字典序最小的作为"标准形"。
//     旋转90°: (r,c)→(c,-r)，翻转: (r,c)→(r,-c)。
//
// Q4(大规模): "网格很大时，set<string>的比较开销怎么办？"
//   → 对路径字符串算多项式哈希，用unordered_set<uint64_t>存储。
//     双哈希降低冲突概率。
// ============================================================
