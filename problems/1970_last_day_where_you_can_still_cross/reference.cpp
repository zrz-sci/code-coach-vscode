/*
 * LeetCode 1970: 你能穿过矩阵的最后一天 (Last Day Where You Can Still Cross)
 * 
 * 【题目本质】
 * 陆地逐渐被水淹没，找从顶行到底行陆地连通的最后一天。
 * 核心是"连通性随时间单调变化" → 二分答案 或 逆向并查集。
 *
 * 【解法总览】
 * 解法1: 暴力 BFS         — O((RC)²) / O(RC)       — 每天检查连通性
 * 解法2: 二分 + BFS        — O(RC·log(RC)) / O(RC)  — 面试首选
 * 解法3: 逆向并查集         — O(RC·α(RC)) / O(RC)   — 最优，加分项
 */

// ============================================================
// 解法1: 暴力 BFS — 每天重建矩阵并 BFS 检查连通性
// 时间: O((RC)²)  空间: O(RC)
//
// 【思路】
// 最朴素想法：对每一天 d=1,2,...,n，构建前 d 天淹完的矩阵，
// 然后从第一行所有陆地出发 BFS，看能否到达最后一行。
// 找最大的 d 使得连通。
//
// 瓶颈：每天独立做一次完整 BFS，总 O(n × RC)，
// 其中 n = RC，所以总 O((RC)²)，数据范围 2×10⁴ 时约 4×10⁸，会超时。
// 
// 这个解法目的是展示理解题意，引出二分优化。
// ============================================================
class Solution1 {
public:
    int latestDayToCross(int row, int col, vector<vector<int>>& cells) {
        int n = cells.size();
        // 从最后一天往前找，第一个能连通的天就是答案
        for (int day = n; day >= 1; day--) {
            vector<vector<int>> grid(row, vector<int>(col, 0));
            for (int i = 0; i < day; i++) {
                grid[cells[i][0] - 1][cells[i][1] - 1] = 1; // 1-based → 0-based
            }
            if (canCross(grid, row, col)) return day;
        }
        return 0;
    }
    
    bool canCross(vector<vector<int>>& grid, int row, int col) {
        queue<pair<int,int>> q;
        // 将第一行所有陆地作为 BFS 起点
        for (int j = 0; j < col; j++) {
            if (grid[0][j] == 0) {
                q.push({0, j});
                grid[0][j] = 1; // 用水标记"已访问"，避免额外 visited 数组
            }
        }
        int dx[] = {0, 0, 1, -1};
        int dy[] = {1, -1, 0, 0};
        while (!q.empty()) {
            auto [x, y] = q.front(); q.pop();
            if (x == row - 1) return true; // 到达最后一行
            for (int d = 0; d < 4; d++) {
                int nx = x + dx[d], ny = y + dy[d];
                if (nx >= 0 && nx < row && ny >= 0 && ny < col 
                    && grid[nx][ny] == 0) {
                    grid[nx][ny] = 1; // 标记已访问
                    q.push({nx, ny});
                }
            }
        }
        return false;
    }
};

// ============================================================
// 解法2: 二分 + BFS — 面试首选 ⭐
// 时间: O(RC·log(RC))  空间: O(RC)
//
// 【思路】
// 关键观察：答案具有单调性。
// 设 f(d) = "前d天淹完后，顶行到底行是否连通"
// 随着 d 增大，水越来越多，f(d) 从 true 变为 false，且一旦 false 不会再 true。
// 这就是经典的"二分答案"模型：找最后一个 f(d) = true 的 d。
//
// 二分搜索空间收缩过程 (以示例3为例, row=3, col=3):
//   cells = [[1,2],[2,1],[3,3],[2,2],[1,1],[1,3],[2,3],[3,2],[3,1]]
//   总共9天
//
//   天数: 1  2  3  4  5  6  7  8  9
//   能过: ✓  ✓  ✓  ✗  ✗  ✗  ✗  ✗  ✗
//
//   Round1: lo=1, hi=9, mid=5 → check(5)=false → hi=4
//   Round2: lo=1, hi=4, mid=2 → check(2)=true  → ans=2, lo=3
//   Round3: lo=3, hi=4, mid=3 → check(3)=true  → ans=3, lo=4
//   Round4: lo=4, hi=4, mid=4 → check(4)=false → hi=3
//   lo=4 > hi=3, 返回 ans=3 ✓
// ============================================================
class Solution2 {
public:
    int latestDayToCross(int row, int col, vector<vector<int>>& cells) {
        int lo = 1, hi = cells.size();
        int ans = 0;
        
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (canCross(row, col, cells, mid)) {
                ans = mid;       // mid天还能过，记录并尝试更大的
                lo = mid + 1;
            } else {
                hi = mid - 1;    // mid天已不能过，缩小到左半
            }
        }
        return ans;
    }
    
    bool canCross(int row, int col, vector<vector<int>>& cells, int day) {
        // 构建前 day 天淹完的矩阵
        vector<vector<int>> grid(row, vector<int>(col, 0));
        for (int i = 0; i < day; i++) {
            grid[cells[i][0] - 1][cells[i][1] - 1] = 1;
        }
        
        // BFS: 从第一行所有陆地出发
        queue<pair<int,int>> q;
        int dx[] = {0, 0, 1, -1};
        int dy[] = {1, -1, 0, 0};
        
        for (int j = 0; j < col; j++) {
            if (grid[0][j] == 0) {
                q.push({0, j});
                grid[0][j] = 1; // 直接在 grid 上标记已访问
            }
        }
        
        while (!q.empty()) {
            auto [x, y] = q.front(); q.pop();
            // 到达最后一行 → 连通
            if (x == row - 1) return true;
            for (int d = 0; d < 4; d++) {
                int nx = x + dx[d], ny = y + dy[d];
                if (nx >= 0 && nx < row && ny >= 0 && ny < col 
                    && grid[nx][ny] == 0) {
                    grid[nx][ny] = 1;
                    q.push({nx, ny});
                }
            }
        }
        return false;
    }
};

// ============================================================
// 解法3: 逆向并查集 — 最优解，加分项
// 时间: O(RC·α(RC)) ≈ O(RC)  空间: O(RC)
//
// 【思路】
// 核心洞察：并查集擅长"合并"（加边），不擅长"断开"（删边）。
// 正向过程是"不断淹没陆地"→ 删除节点/边 → 难。
// 逆向过程是"不断恢复陆地"→ 添加节点并与邻居合并 → 正是并查集的强项！
//
// 具体做法：
// 1. 初始状态：全是水
// 2. 从 cells 的最后一天开始，逐天恢复陆地
// 3. 恢复一格后，与四邻域的已恢复陆地合并
// 4. 用虚拟节点 topVirtual / bottomVirtual 分别连接第一行和最后一行
// 5. 当 topVirtual 和 bottomVirtual 首次连通时，当前天数就是答案
//
// 为什么答案是 i（0-based index）？
// cells[0..i-1] 是前 i 天淹没的格子（天数 1-based 是第1到第i天）
// 恢复 cells[i] 意味着 cells[i] 没被淹 → 等价于只有前 i 天的水
// 此时连通 → 前 i 天淹完后仍可穿越 → 答案是 i
//
//  虚拟节点示意:
//  
//     topVirtual(n)
//     /    |    \
//   [0,0] [0,1] [0,2]    ← 第一行陆地恢复时union到topVirtual
//     |  \  |  /  |
//   [1,0] [1,1] [1,2]    ← 与四邻的已有陆地union
//     |  /  |  \  |
//   [2,0] [2,1] [2,2]    ← 最后一行陆地恢复时union到bottomVirtual
//     \    |    /
//     bottomVirtual(n+1)
//
//  当 find(topVirtual) == find(bottomVirtual) → 上下连通！
// ============================================================
class Solution3 {
public:
    vector<int> parent, rnk;
    
    int find(int x) {
        // 路径压缩：让 x 直接指向根
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }
    
    void unite(int a, int b) {
        int pa = find(a), pb = find(b);
        if (pa == pb) return;
        // 按秩合并：矮树挂在高树下，保持树高尽量小
        if (rnk[pa] < rnk[pb]) swap(pa, pb);
        parent[pb] = pa;
        if (rnk[pa] == rnk[pb]) rnk[pa]++;
    }
    
    int latestDayToCross(int row, int col, vector<vector<int>>& cells) {
        int n = row * col;
        int topVirtual = n;         // 虚拟节点：代表"顶行整体"
        int bottomVirtual = n + 1;  // 虚拟节点：代表"底行整体"
        
        parent.resize(n + 2);
        rnk.resize(n + 2, 0);
        iota(parent.begin(), parent.end(), 0); // parent[i] = i
        
        // 初始全是水
        vector<vector<int>> grid(row, vector<int>(col, 1));
        
        int dx[] = {0, 0, 1, -1};
        int dy[] = {1, -1, 0, 0};
        
        // 从最后一天开始逆序恢复
        for (int i = n - 1; i >= 0; i--) {
            int r = cells[i][0] - 1;  // 1-based → 0-based
            int c = cells[i][1] - 1;
            grid[r][c] = 0;           // 恢复为陆地
            
            int idx = r * col + c;    // 二维坐标转一维编号
            
            // 与四个方向已有的陆地合并
            for (int d = 0; d < 4; d++) {
                int nr = r + dx[d], nc = c + dy[d];
                if (nr >= 0 && nr < row && nc >= 0 && nc < col
                    && grid[nr][nc] == 0) {
                    unite(idx, nr * col + nc);
                }
            }
            
            // 第一行的陆地连到 topVirtual
            if (r == 0) unite(idx, topVirtual);
            // 最后一行的陆地连到 bottomVirtual
            if (r == row - 1) unite(idx, bottomVirtual);
            
            // 检查：顶部和底部是否连通
            if (find(topVirtual) == find(bottomVirtual)) {
                // 恢复cells[i]后连通 → 等价于前i天淹完后仍可穿越
                // cells是1-indexed天数: cells[0]是第1天, cells[i]是第i+1天
                // 恢复cells[i]意味着第i+1天的水没了 → 只有前i天的水
                // 答案就是 i
                return i;
            }
        }
        
        return 0; // 理论上day=0全陆地一定连通，不会到这里
    }
};

// ============================================================
// 【解法对比】
//
// | 解法 | 时间 | 空间 | 核心技巧 | 面试场景 |
// |------|------|------|----------|----------|
// | 暴力BFS | O((RC)²) | O(RC) | 模拟 | 说思路即可 |
// | 二分+BFS | O(RC·log(RC)) | O(RC) | 二分答案 | ⭐面试首选 |
// | 逆向并查集 | O(RC·α(RC)) | O(RC) | 逆向思维+UF | 加分项 |
//
// 二分+BFS vs 逆向并查集:
// - 二分+BFS: 思路直接，"答案有单调性→二分"是常见模式，代码好写
// - 逆向并查集: 更优的时间复杂度，展示对并查集的深入理解
// - 两者都值得掌握，因为代表了两种不同的思维范式
//
// 【易错点】
// 1. 坐标转换: cells 是 1-based，grid 是 0-based
//    ✗ grid[cells[i][0]][cells[i][1]] → 越界
//    ✓ grid[cells[i][0]-1][cells[i][1]-1]
//
// 2. 二分返回值: 不能返回 lo 或 mid，因为循环结束时它们可能不是答案
//    ✗ return mid; (mid 可能在最后一轮 check 失败)
//    ✓ 用 ans 变量记录最后一个 check=true 的 mid
//
// 3. BFS 中入队时标记 vs 出队时标记:
//    ✗ 出队时标记 → 同一节点被重复入队，浪费时间甚至死循环
//    ✓ 入队时立即标记 grid[nx][ny] = 1
//
// 4. 并查集天数计算:
//    逆序恢复 cells[i] 后连通 → 答案是 i（不是 i-1 也不是 i+1）
//    因为此时等价于只有前 i 天的水存在
//
// 5. 并查集忘记处理虚拟节点:
//    ✗ 只与四邻合并，不连虚拟节点 → find 检查时永远不会连通
//    ✓ 第一行连 topVirtual，最后一行连 bottomVirtual
//
// 【面试追问】
// Q1: 暴力解的瓶颈是什么？
//     → 每天独立 BFS，没利用"只变了一格"的增量信息，O((RC)²)
//
// Q2: 为什么能二分？
//     → 水只增不减，连通性单调递减（能过→不能过），存在唯一分界点
//
// Q3: 能否比 O(RC·logRC) 更快？
//     → 逆向并查集 O(RC·α(RC))，避免了二分的 log 因子
//
// Q4: 如果要从左到右穿越呢？
//     → 虚拟节点改为 leftVirtual 连第一列，rightVirtual 连最后一列
//
// Q5: 如果允许8方向（含对角线）呢？
//     → dx/dy 扩展为8个方向，其他逻辑不变
// ============================================================
