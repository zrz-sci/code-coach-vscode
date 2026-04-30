/*
 * LeetCode 547: Number of Provinces (省份数量)
 *
 * 【题目本质】
 * 给一个 n x n 的邻接矩阵，求无向图中连通分量的个数。
 * "省份" = 连通分量，"城市" = 节点，isConnected[i][j]=1 = 边。
 *
 * 【解法总览】
 * 解法1: DFS        — O(n^2) / O(n)            — 面试首选，代码最短
 * 解法2: BFS        — O(n^2) / O(n)            — DFS的等价替代
 * 解法3: Union-Find — O(n^2 * alpha(n)) / O(n) — 进阶，适合动态加边场景
 */

// ============================================================
// 解法1: DFS — 面试首选 ⭐
// 时间: O(n^2)  空间: O(n)
//
// 【思路】
// 维护 visited 数组。遍历每个城市 i:
//   如果未访问 → 从 i 出发 DFS，标记所有可达节点 → provinces++
//
// DFS 遍历过程 (画图理解):
//
// isConnected = [[1,1,0],[1,1,0],[0,0,1]]
//
// 图结构:   0 --- 1     2    (0和1相连, 2独立)
//
// i=0: 未访问 → DFS(0)
//   ┌─ DFS(0): mark visited[0]=T
//   │  j=1: g[0][1]=1, !visited[1] → DFS(1)
//   │  ┌─ DFS(1): mark visited[1]=T
//   │  │  j=0: visited[0]=T, 跳过
//   │  │  j=2: g[1][2]=0, 跳过
//   │  └─ return
//   │  j=2: g[0][2]=0, 跳过
//   └─ return
//   provinces = 1
//
// i=1: visited[1]=T → 跳过
//
// i=2: 未访问 → DFS(2)
//   ┌─ DFS(2): mark visited[2]=T
//   │  j=0: g[2][0]=0, 跳过
//   │  j=1: g[2][1]=0, 跳过
//   └─ return
//   provinces = 2
//
// 结果: 2 ✓
// ============================================================
class Solution1 {
public:
    int findCircleNum(vector<vector<int>>& isConnected) {
        int n = isConnected.size();
        vector<bool> visited(n, false);
        int provinces = 0;

        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                // 发现一个新的连通分量
                dfs(isConnected, visited, i, n);
                provinces++;
            }
        }
        return provinces;
    }

private:
    void dfs(vector<vector<int>>& g, vector<bool>& visited, int i, int n) {
        // 标记当前节点为已访问（必须在遍历邻居之前）
        visited[i] = true;
        // 遍历所有可能的邻居
        for (int j = 0; j < n; j++) {
            // 有边 且 未访问 → 递归DFS
            if (g[i][j] == 1 && !visited[j]) {
                dfs(g, visited, j, n);
            }
        }
    }
};

// ============================================================
// 解法2: BFS — DFS 的等价替代
// 时间: O(n^2)  空间: O(n)
//
// 【思路】
// 和 DFS 逻辑相同，只是用队列代替递归栈。
// 优势：不用担心递归栈溢出（n 最大 200，DFS 也不会溢出，但习惯上 BFS 更安全）。
//
// 注意：visited 标记应在 入队时 设置，而不是出队时。
//   如果出队时才标记，同一个节点可能被多次入队，浪费时间和空间。
// ============================================================
class Solution2 {
public:
    int findCircleNum(vector<vector<int>>& isConnected) {
        int n = isConnected.size();
        vector<bool> visited(n, false);
        int provinces = 0;

        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                // BFS 从节点 i 开始
                queue<int> q;
                q.push(i);
                visited[i] = true;  // 入队时标记

                while (!q.empty()) {
                    int node = q.front();
                    q.pop();
                    for (int j = 0; j < n; j++) {
                        if (isConnected[node][j] == 1 && !visited[j]) {
                            visited[j] = true;  // 入队时标记，不是出队时！
                            q.push(j);
                        }
                    }
                }
                provinces++;
            }
        }
        return provinces;
    }
};

// ============================================================
// 解法3: Union-Find (并查集)
// 时间: O(n^2 * alpha(n)) ≈ O(n^2)  空间: O(n)
//
// 【思路】
// 1. 初始化：每个城市是自己的根（n 个独立集合）
// 2. 遍历邻接矩阵上三角（j > i），对每条边做 union
// 3. 统计最终有多少个不同的根 = 连通分量数
//
// 为什么只遍历上三角？
//   isConnected[i][j] == isConnected[j][i]（无向图对称），
//   处理 (i,j) 和 (j,i) 是同一条边，只需一次。
//
// Union-Find 过程:
//   初始: parent=[0,1,2], 3个分量
//   (0,1): union(0,1) → parent=[0,0,2], 2个分量
//   (0,2): g[0][2]=0, 跳过
//   (1,2): g[1][2]=0, 跳过
//   统计 find(i)==i: {0,2} → 2个 ✓
//
// 路径压缩 + 按秩合并:
//   find 时路径压缩: 将路径上所有节点直接指向根
//   unite 时按秩合并: 矮树挂到高树下，保持树的平衡
//   这使得每次操作接近 O(1)
// ============================================================
class Solution3 {
public:
    int findCircleNum(vector<vector<int>>& isConnected) {
        int n = isConnected.size();
        vector<int> parent(n), rank_(n, 0);
        // 初始化：每个节点是自己的根
        iota(parent.begin(), parent.end(), 0);

        // 遍历邻接矩阵上三角
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                if (isConnected[i][j] == 1) {
                    unite(parent, rank_, i, j);
                }
            }
        }

        // 统计不同根的数量 = 连通分量数
        int provinces = 0;
        for (int i = 0; i < n; i++) {
            if (find(parent, i) == i) {
                provinces++;
            }
        }
        return provinces;
    }

private:
    // 带路径压缩的 find
    int find(vector<int>& parent, int x) {
        if (parent[x] != x) {
            parent[x] = find(parent, parent[x]);
        }
        return parent[x];
    }

    // 带按秩合并的 union
    void unite(vector<int>& parent, vector<int>& rank_, int x, int y) {
        int rx = find(parent, x);
        int ry = find(parent, y);
        if (rx == ry) return;  // 已在同一集合

        // 矮树挂到高树下
        if (rank_[rx] < rank_[ry]) swap(rx, ry);
        parent[ry] = rx;
        if (rank_[rx] == rank_[ry]) rank_[rx]++;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法        | 时间    | 空间 | 优势                            |
// |------------|--------|------|-------------------------------|
// | DFS        | O(n^2) | O(n) | 代码最短，最直观                  |
// | BFS        | O(n^2) | O(n) | 不用担心栈溢出                    |
// | Union-Find | O(n^2) | O(n) | 支持动态加边/查询连通性，通用模板    |
//
// 面试选择：默认写 DFS。如果面试官问"边是动态添加的"，用 Union-Find。
//
// ============================================================
// 【易错点】
//
// 1. 混淆邻接矩阵和网格:
//    ✗ 按网格做四方向 BFS/DFS (上下左右)
//    ✓ 节点 i 的邻居是 j=0..n-1 中 isConnected[i][j]==1 的
//
// 2. 忘记标记 visited:
//    ✗ DFS 中不设 visited[i]=true → 无限递归 (i→j→i→j...)
//    ✓ 进入 DFS 的第一行就标记
//
// 3. BFS 中出队时才标记 visited:
//    ✗ visited 在 pop 后设置 → 同一节点多次入队
//    ✓ visited 在 push 时设置
//
// 4. Union-Find 遍历全矩阵:
//    ✗ for i,j: g[i][j]==1 → union → 每条边处理两次(不出错但浪费)
//    ✓ 只遍历上三角 j > i
//
// ============================================================
// 【相关题型】
//
// - 200. 岛屿数量: 网格上的连通分量 (四方向flood fill)
// - 323. 无向图中连通分量的数目: 同一问题，输入是边列表
// - 684. 冗余连接: Union-Find 找成环的边
// - 990. 等式方程的可满足性: 等式做union，检查不等式矛盾
//
// ============================================================
// 【面试追问】
//
// Q1: 如果边是动态添加的, 每次加边后需要查询连通分量数, 怎么优化?
// → Union-Find (并查集)。维护一个 count 变量, 初始为 n。
//   每次 union 成功 (两个根不同) 就 count--。
//   查询 O(alpha(n)) 近似 O(1), 比每次重新 DFS 高效得多。
//
// Q2: 如果还需要支持"删边"操作呢?
// → 标准 Union-Find 不支持删边。两种思路:
//   (1) 离线处理: 把所有操作倒序, 删边变加边, 逆向用 Union-Find。
//   (2) 在线处理: Link-Cut Tree (LCT) 支持动态连通性,
//       但实现复杂, 面试基本不要求写出。
//
// Q3: 如果输入不是邻接矩阵而是边列表 (edges), 怎么改?
// → DFS: 先用 edges 建邻接表 adj[u].push_back(v), adj[v].push_back(u),
//   然后照常 DFS 遍历。
//   Union-Find: 直接遍历边列表 union(u, v) 即可, 甚至更简单。
//   参考 LC 323 (Number of Connected Components in an Undirected Graph)。
// ============================================================
