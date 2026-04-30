/*
 * LeetCode 329: 矩阵中的最长递增路径 (Longest Increasing Path in a Matrix)
 *
 * 【题目本质】
 * 矩阵中严格递增方向构成一个 DAG（有向无环图），本题就是求 DAG 上的最长路径。
 *
 * 【解法总览】
 * 解法1: 暴力 DFS         — O(指数级) / O(m·n)  — 最直觉，但超时
 * 解法2: DFS + 记忆化      — O(m·n) / O(m·n)    — ⭐ 面试首选
 * 解法3: 拓扑排序 BFS      — O(m·n) / O(m·n)    — 加分项，无递归栈风险
 */

// ============================================================
// 解法1: 暴力 DFS — 对每个格子出发 DFS 穷举所有递增路径
// 时间: O(m·n·4^(m·n)) 最坏指数级  空间: O(m·n) 递归栈
//
// 【思路】
// 最直觉的想法：枚举每个格子作为起点，DFS 往四个方向走，
// 只走值严格更大的格子，记录最长路径。
// 
// 关键观察：只走严格递增方向 → 不可能回到已访问的格子 → 无环
// 所以不需要 visited 数组。
//
// 问题：大量重复计算。从不同起点的 DFS 会反复计算同一个格子
// 出发的最长路径。
//
// 矩阵示意 (示例1):
//   9  9  4
//   6  6  8
//   2  1  1
//
// 从(2,1)=1出发: 1→2→6→9 长度4
// 从(2,0)=2出发: 2→6→9   长度3  ← 子路径6→9被重复计算了
// ============================================================
class Solution1 {
public:
    int m, n;
    int dx[4] = {0, 0, 1, -1};
    int dy[4] = {1, -1, 0, 0};
    
    int dfs(vector<vector<int>>& matrix, int i, int j) {
        int maxLen = 1; // 至少包含自己
        for (int d = 0; d < 4; d++) {
            int ni = i + dx[d], nj = j + dy[d];
            if (ni >= 0 && ni < m && nj >= 0 && nj < n
                && matrix[ni][nj] > matrix[i][j]) {
                maxLen = max(maxLen, 1 + dfs(matrix, ni, nj));
            }
        }
        return maxLen;
    }
    
    int longestIncreasingPath(vector<vector<int>>& matrix) {
        m = matrix.size(); n = matrix[0].size();
        int ans = 0;
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                ans = max(ans, dfs(matrix, i, j));
        return ans;
    }
};

// ============================================================
// 解法2: DFS + 记忆化 — ⭐ 面试首选
// 时间: O(m·n)  空间: O(m·n)
//
// 【思路】
// 解法1的瓶颈：同一个格子的 DFS 结果被反复计算。
// 
// 关键：因为图是 DAG（严格递增 → 无环），所以从 (i,j) 出发的
// 最长递增路径长度是固定的，不会因为"从哪里来"而改变。
// 这就是 DAG 上 DP 的子问题独立性。
//
// 优化：用 memo[i][j] 缓存结果。memo[i][j]=0 表示未计算。
// 合法结果最小为1（格子自身），所以0是安全的哨兵值。
//
// 每个格子最多计算一次，每次看4个邻居 → 总时间 O(4·m·n) = O(m·n)
//
// 记忆化过程:
//   dfs(2,1)=1 → 上(1,1)=6>1 → dfs(1,1) → 上(0,1)=9>6 → dfs(0,1)
//     dfs(0,1): 四周无更大值 → memo[0][1]=1, 返回1
//   dfs(1,1) = 1+1 = 2 → memo[1][1]=2
//   dfs(2,1) 继续: 左(2,0)=2>1 → dfs(2,0) → 上(1,0)=6>2 → dfs(1,0)
//     dfs(1,0): 上(0,0)=9>6 → dfs(0,0) → memo[0][0]=1
//     dfs(1,0) = 1+1 = 2 → memo[1][0]=2
//   dfs(2,0) = 1+2 = 3 → memo[2][0]=3
//   dfs(2,1) = max(1+2, 1+3) = 4 → memo[2][1]=4
//   后续再访问(1,0)时直接返回memo[1][0]=2，不再递归
// ============================================================
class Solution2 {
public:
    int m, n;
    int dx[4] = {0, 0, 1, -1};
    int dy[4] = {1, -1, 0, 0};
    vector<vector<int>> memo;
    
    int dfs(vector<vector<int>>& matrix, int i, int j) {
        // 已经计算过，直接返回缓存值
        if (memo[i][j] != 0) return memo[i][j];
        
        int maxLen = 1; // 至少包含自身
        for (int d = 0; d < 4; d++) {
            int ni = i + dx[d], nj = j + dy[d];
            if (ni >= 0 && ni < m && nj >= 0 && nj < n
                && matrix[ni][nj] > matrix[i][j]) {
                // 严格递增保证无环，不需要 visited
                maxLen = max(maxLen, 1 + dfs(matrix, ni, nj));
            }
        }
        
        memo[i][j] = maxLen; // 缓存"从(i,j)出发的最长递增路径长度"
        return maxLen;
    }
    
    int longestIncreasingPath(vector<vector<int>>& matrix) {
        m = matrix.size(); n = matrix[0].size();
        memo.assign(m, vector<int>(n, 0)); // 0 = 未计算
        
        int ans = 0;
        // 枚举每个格子作为起点，取最大值
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                ans = max(ans, dfs(matrix, i, j));
        return ans;
    }
};

// ============================================================
// 解法3: 拓扑排序 BFS — 无递归栈风险
// 时间: O(m·n)  空间: O(m·n)
//
// 【思路】
// 把矩阵看成 DAG：边从小值指向大值 (即 a→b 当且仅当 a<b 且相邻)。
// 求 DAG 上最长路径 = 拓扑排序后的最大层数。
//
// 1. 计算入度: indegree[i][j] = 有多少个严格更小的邻居指向(i,j)
// 2. 入度为0的格子入队（它们是路径起点/局部最小值）
// 3. BFS 逐层扩展，每层 layers++
// 4. 最终 layers = 最长路径长度
//
// 为什么层数 = 最长路径？
// 拓扑排序中，第k层的节点意味着它前面至少有k-1层。
// 最后一层处理完时，layers就是DAG的最长链长度。
//
// 入度计算 (示例1):
//   matrix:      indegree:
//   9 9 4        2 2 0     ← (0,2)=4, 邻居9>4和8>4，没有<4的
//   6 6 8        1 1 2
//   2 1 1        1 0 0     ← (2,1)=1和(2,2)=1, 没有更小的邻居
//
// BFS 层序扩展:
//   Level 1: (0,2)=4, (2,1)=1, (2,2)=1   入度为0
//   Level 2: (1,2)=8, (2,0)=2             入度变为0
//   Level 3: (1,0)=6, (1,1)=6
//   Level 4: (0,0)=9, (0,1)=9
//   答案 = 4 层
// ============================================================
class Solution3 {
public:
    int longestIncreasingPath(vector<vector<int>>& matrix) {
        int m = matrix.size(), n = matrix[0].size();
        int dx[4] = {0, 0, 1, -1};
        int dy[4] = {1, -1, 0, 0};
        
        // Step 1: 计算入度
        // indegree[i][j] = 有多少个邻居的值严格小于 matrix[i][j]
        // 即有多少条边"从更小的邻居指向我"
        vector<vector<int>> indegree(m, vector<int>(n, 0));
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                for (int d = 0; d < 4; d++) {
                    int ni = i + dx[d], nj = j + dy[d];
                    if (ni >= 0 && ni < m && nj >= 0 && nj < n
                        && matrix[ni][nj] < matrix[i][j]) {
                        // (ni,nj) 比我小，它指向我 → 我的入度+1
                        indegree[i][j]++;
                    }
                }
            }
        }
        
        // Step 2: 入度为0的入队（路径起点 = 局部最小值）
        queue<pair<int,int>> q;
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                if (indegree[i][j] == 0)
                    q.push({i, j});
        
        // Step 3: BFS 逐层扩展
        int layers = 0;
        while (!q.empty()) {
            int sz = q.size();
            layers++; // 每处理一层，最长路径+1
            for (int k = 0; k < sz; k++) {
                auto [i, j] = q.front(); q.pop();
                // 对所有 "我指向的更大邻居" 减少入度
                for (int d = 0; d < 4; d++) {
                    int ni = i + dx[d], nj = j + dy[d];
                    if (ni >= 0 && ni < m && nj >= 0 && nj < n
                        && matrix[ni][nj] > matrix[i][j]) {
                        if (--indegree[ni][nj] == 0) {
                            q.push({ni, nj}); // 入度变0，可以加入下一层
                        }
                    }
                }
            }
        }
        
        return layers;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法          | 时间     | 空间   | 递归风险 | 面试推荐 |
// |---------------|----------|--------|----------|----------|
// | 暴力 DFS      | 指数级   | O(m·n) | 有       | 说思路   |
// | DFS + 记忆化  | O(m·n)  | O(m·n) | 有       | ⭐首选   |
// | 拓扑排序 BFS  | O(m·n)  | O(m·n) | 无       | 加分项   |
//
// 解法2 vs 解法3:
// - 解法2 (DFS+Memo) 是自顶向下：从每个格子出发，递归求解子问题
// - 解法3 (拓扑BFS) 是自底向上：从局部最小值开始，逐层推进
// - 本质相同，都利用了 DAG 的无环性
// - 解法2 代码更短更直觉，面试首选
// - 解法3 无递归栈风险，适合超大矩阵
//
// ============================================================
// 【易错点】
//
// 1. ✗ 在 DFS 中加了 visited 数组:
//      visited[i][j] = true;
//      dfs(ni, nj);
//      visited[i][j] = false;
//    → ✓ 不需要 visited！严格递增保证无环。
//      如果加了 visited，从不同入口到达同一格子时会得到错误结果
//      （因为该格子被标记为 visited，无法再被其他路径访问）。
//
// 2. ✗ memo 初始化后忘记在 dfs 开头检查:
//      int dfs(...) { int maxLen = 1; /* 直接开始循环 */ }
//    → ✓ 必须先检查 if (memo[i][j] != 0) return memo[i][j];
//      否则没有记忆化效果，退化为暴力。
//
// 3. ✗ 返回值少加了1:
//      maxLen = max(maxLen, dfs(matrix, ni, nj)); // 少了 "1 +"
//    → ✓ maxLen = max(maxLen, 1 + dfs(matrix, ni, nj));
//      当前格子本身要算一步。
//
// 4. ✗ 拓扑排序入度方向搞反:
//      if (matrix[ni][nj] > matrix[i][j]) indegree[i][j]++;
//      // 这是在说"更大的邻居指向我" → 边方向反了
//    → ✓ if (matrix[ni][nj] < matrix[i][j]) indegree[i][j]++;
//      入度 = "有多少个更小的邻居指向我"（边从小指向大）
//
// 5. ✗ BFS 中处理邻居时方向搞反:
//      if (matrix[ni][nj] < matrix[i][j]) // 应该是 >
//    → ✓ if (matrix[ni][nj] > matrix[i][j])
//      BFS 扩展时，当前节点指向更大的邻居
//
// ============================================================
// 【面试追问】
//
// Q1: 为什么不需要 visited 数组？
// → 只走严格递增方向，值单调递增不可能回到已走过的格子，
//   所以天然无环。本质是在 DAG 上做搜索。
//
// Q2: 记忆化搜索中，从不同起点到达同一格子，memo 值会不会冲突？
// → 不会。memo[i][j] 表示"从(i,j)出发的最长路径"，这个值
//   只取决于(i,j)及其后续格子，与"谁先到达(i,j)"无关。
//   这是 DAG 上 DP 子问题独立性的体现。
//
// Q3: 如果矩阵 1000×1000，递归深度可能达 10^6，怎么避免栈溢出？
// → 方案1: 使用拓扑排序 BFS（解法3），完全迭代无递归。
//   方案2: 将 DFS 改为显式栈的迭代版本。
//   方案3: 系统层面增大栈大小（不推荐，不通用）。
//
// Q4: 如果要输出最长路径本身，怎么改？
// → 在 DFS 记忆化基础上，额外维护 next[i][j] = 下一步的坐标。
//   每次更新 maxLen 时同时记录是从哪个邻居得到的最优值。
//   最后从全局最优起点沿 next 链还原路径。
//
// Q5: 时间复杂度 O(m·n) 怎么严格证明？
// → 每个格子最多被 dfs() 实际计算一次（后续直接查 memo），
//   每次计算遍历4个邻居，所以总工作量 ≤ 4·m·n = O(m·n)。
//   外层双循环虽然调用 m·n 次 dfs，但大部分直接命中 memo 返回。
//
// ============================================================
// 【空间优化说明】
//
// 本题是矩阵上的记忆化搜索（DAG 上的 DP），memo 必须保留整个矩阵，
// 无法进行空间优化。
//
// 原因分析：
// - memo[i][j] = 从 (i,j) 出发的最长递增路径长度
// - 每个格子的结果依赖其四个方向上值更大的邻居，依赖关系形成 DAG
// - 这个 DAG 的拓扑顺序由矩阵元素的大小关系决定，不是简单的行/列顺序
// - 因此不能像网格 DP（如 0062 Unique Paths）那样逐行滚动
//
// 类比：拓扑排序 BFS（解法3）同样需要 O(m·n) 的 indegree 数组。
// 无论用 DFS+memo 还是拓扑 BFS，空间都是 O(m·n)，已是最优。
// ============================================================
