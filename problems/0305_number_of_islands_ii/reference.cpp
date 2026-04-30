/*
 * LeetCode 305: 岛屿数量 II (Number of Islands II)
 *
 * 【题目本质】
 * 动态连通性问题：每次添加一个陆地节点，快速回答当前有多少连通分量（岛屿）。
 * 这是并查集（Union-Find）的经典应用场景。
 *
 * 【解法总览】
 * 解法1: 暴力BFS         — O(k×m×n) / O(m×n) — 每次重新扫描，最直觉
 * 解法2: 并查集(数组版)   — O(k×α(mn)) / O(m×n) — 面试标准答案 ⭐
 * 解法3: 并查集(HashMap版) — O(k×α(mn)) / O(k) — 稀疏网格优化
 *
 * 【示例演示】
 * m=3, n=3, positions=[[0,0],[0,1],[1,2],[2,1]]
 *
 *   Op1: addLand(0,0)      Op2: addLand(0,1)      Op3: addLand(1,2)      Op4: addLand(2,1)
 *   1 0 0                  1 1 0                  1 1 0                  1 1 0
 *   0 0 0                  0 0 0                  0 0 1                  0 0 1
 *   0 0 0                  0 0 0                  0 0 0                  0 1 0
 *   count: 0+1=1           count: 1+1-1=1         count: 1+1=2           count: 2+1=3
 *          (无邻居)                (与(0,0)合并)          (无邻居陆地)           (无邻居陆地)
 */

// ============================================================
// 解法1: 暴力BFS — 每次加完陆地后重新扫描整个网格
// 时间: O(k × m × n)  空间: O(m × n)
//
// 【思路】
// 最直觉的做法：和 LeetCode 200 完全一样，只不过每次 addLand 后
// 都重新做一遍 BFS 计数。暴力但正确。
//
// 【瓶颈】
// 每次操作都要遍历整个网格，已经遍历过的陆地被反复重算。
// 当 k 和 m×n 都是 10^4 时，总操作量达到 10^8，勉强能过但不优雅。
// ============================================================
class Solution1 {
public:
    vector<int> numIslands2(int m, int n, vector<vector<int>>& positions) {
        vector<vector<int>> grid(m, vector<int>(n, 0));
        vector<int> result;
        int dx[] = {0, 0, 1, -1};
        int dy[] = {1, -1, 0, 0};

        for (auto& pos : positions) {
            grid[pos[0]][pos[1]] = 1;

            // 每次都重新 BFS 计数（完全复用 LC200 的逻辑）
            vector<vector<bool>> visited(m, vector<bool>(n, false));
            int count = 0;
            for (int i = 0; i < m; i++) {
                for (int j = 0; j < n; j++) {
                    if (grid[i][j] == 1 && !visited[i][j]) {
                        count++;
                        queue<pair<int, int>> q;
                        q.push({i, j});
                        visited[i][j] = true;
                        while (!q.empty()) {
                            auto [x, y] = q.front();
                            q.pop();
                            for (int d = 0; d < 4; d++) {
                                int nx = x + dx[d], ny = y + dy[d];
                                if (nx >= 0 && nx < m && ny >= 0 && ny < n
                                    && grid[nx][ny] == 1 && !visited[nx][ny]) {
                                    visited[nx][ny] = true;
                                    q.push({nx, ny});
                                }
                            }
                        }
                    }
                }
            }
            result.push_back(count);
        }
        return result;
    }
};

// ============================================================
// 解法2: 并查集（路径压缩 + 按秩合并）— 面试标准答案 ⭐
// 时间: O(k × α(m×n))  空间: O(m × n)
//
// 【思路】
// 解法1的瓶颈：每次加一个格子就重新扫描整个网格。
// 关键观察：每次只加了一个格子，最多影响上下左右4个邻居。
// 
// 增量维护策略：
//   1. 新加陆地 (r,c) → count++（先当作孤岛）
//   2. 检查四个邻居，如果邻居已经是陆地且和 (r,c) 不在同一集合
//      → union 它们，count--
//   3. 最终 count 就是当前岛屿数
//
// 为什么用并查集？
// - "合并两个集合" = 两块相邻陆地合并为一个岛
// - "查询是否同一集合" = 两块陆地是否已在同一个岛上
// - 路径压缩 + 按秩合并 → find/union 均摊 O(α(n)) ≈ O(1)
//
// 【关键实现细节】
// - parent[id] = -1 表示水，parent[id] >= 0 表示陆地
// - 二维坐标 (r,c) 映射为一维 id = r * n + c（注意是乘列数 n，不是 m）
//
// 【并查集结构示意 - 示例操作后】
//
//   Op1: (0,0)加入，parent[0]=0, count=1
//        集合: {0}
//
//   Op2: (0,1)加入，parent[1]=1, count=2
//        发现邻居(0,0)是陆地，union(1,0) → count=1
//        集合: {0,1}（根为0或1）
//
//   Op3: (1,2)加入，parent[5]=5, count=2
//        邻居(0,2)水, (1,1)水, (2,2)水, (1,3)越界 → 无合并
//        集合: {0,1}, {5}
//
//   Op4: (2,1)加入，parent[7]=7, count=3
//        邻居(1,1)水, (2,0)水, (2,2)水 → 无合并
//        集合: {0,1}, {5}, {7}
// ============================================================
class Solution2 {
public:
    vector<int> parent;
    vector<int> rank_;
    int count;

    // 路径压缩：递归地把路径上的所有节点直接连到根
    // 为什么：避免树退化为链表，保证后续 find 更快
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    // 按秩合并：矮树挂到高树下面
    // 为什么：保持树的高度尽量小，配合路径压缩达到 O(α(n))
    void unite(int x, int y) {
        int rx = find(x), ry = find(y);
        if (rx == ry) return; // 已经在同一集合，不合并，不减 count
        if (rank_[rx] < rank_[ry]) swap(rx, ry);
        parent[ry] = rx;
        if (rank_[rx] == rank_[ry]) rank_[rx]++;
        count--; // 两个不同的岛合并了，总岛屿数 -1
    }

    vector<int> numIslands2(int m, int n, vector<vector<int>>& positions) {
        parent.assign(m * n, -1); // -1 表示水（不是0！因为0是有效id）
        rank_.assign(m * n, 0);
        count = 0;

        vector<int> result;
        int dx[] = {0, 0, 1, -1};
        int dy[] = {1, -1, 0, 0};

        for (auto& pos : positions) {
            int r = pos[0], c = pos[1];
            int id = r * n + c; // 注意：乘的是列数 n，不是行数 m

            // 【易错点】处理重复添加：已经是陆地就跳过
            if (parent[id] != -1) {
                result.push_back(count);
                continue;
            }

            // 新建陆地：自己是自己的根
            parent[id] = id;
            count++; // 先当作一个新岛屿

            // 检查四个方向的邻居
            for (int d = 0; d < 4; d++) {
                int nr = r + dx[d], nc = c + dy[d];
                int nid = nr * n + nc;
                // 邻居在网格内 && 邻居是陆地（parent != -1）
                if (nr >= 0 && nr < m && nc >= 0 && nc < n
                    && parent[nid] != -1) {
                    unite(id, nid); // 内部会判断是否同一集合
                }
            }

            result.push_back(count);
        }
        return result;
    }
};

// ============================================================
// 解法3: 并查集（HashMap 版）— 稀疏网格空间优化
// 时间: O(k × α(m×n))  空间: O(k)
//
// 【思路】
// 解法2 用 O(m×n) 的数组，当网格很大但实际操作很少时浪费空间。
// 用 HashMap 只存已添加的陆地节点，空间降为 O(k)。
//
// 【与解法2的区别】
// - parent 从 vector 变成 unordered_map
// - 判断"是否是陆地"从 parent[id] != -1 变成 parent.count(id) > 0
// - 其余逻辑完全相同
//
// 【适用场景】
// m, n 很大（比如 10^5），但 k 很小（比如 100）。
// 本题约束 m×n <= 10^4，所以数组版就够了，HashMap 版是进阶拓展。
// ============================================================
class Solution3 {
public:
    unordered_map<int, int> parent;
    unordered_map<int, int> rank_;
    int count = 0;

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    void unite(int x, int y) {
        int rx = find(x), ry = find(y);
        if (rx == ry) return;
        if (rank_[rx] < rank_[ry]) swap(rx, ry);
        parent[ry] = rx;
        if (rank_[rx] == rank_[ry]) rank_[rx]++;
        count--;
    }

    vector<int> numIslands2(int m, int n, vector<vector<int>>& positions) {
        vector<int> result;
        int dx[] = {0, 0, 1, -1};
        int dy[] = {1, -1, 0, 0};

        for (auto& pos : positions) {
            int r = pos[0], c = pos[1];
            int id = r * n + c;

            // 用 count() 判断是否已经添加过（HashMap 特有写法）
            if (parent.count(id)) {
                result.push_back(count);
                continue;
            }

            parent[id] = id;
            rank_[id] = 0;
            count++;

            for (int d = 0; d < 4; d++) {
                int nr = r + dx[d], nc = c + dy[d];
                int nid = nr * n + nc;
                if (nr >= 0 && nr < m && nc >= 0 && nc < n
                    && parent.count(nid)) {
                    unite(id, nid);
                }
            }

            result.push_back(count);
        }
        return result;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法       | 时间           | 空间      | 适用场景          |
// |-----------|----------------|-----------|-------------------|
// | 暴力BFS   | O(k × m × n)  | O(m × n)  | 只适合理解题意     |
// | 并查集数组 | O(k × α(mn))  | O(m × n)  | 面试标准答案 ⭐    |
// | 并查集Map  | O(k × α(mn))  | O(k)      | 稀疏网格进阶拓展   |
//
// 解法1 vs 解法2: 本质区别是"每次重算" vs "增量维护"。
//   并查集的妙处在于每次操作只看4个邻居，不需要遍历整个网格。
//
// 解法2 vs 解法3: 空间优化。数组版 O(m×n) 一次性分配；
//   HashMap 版只存有效节点 O(k)，但有哈希表常数开销。
//   本题 m×n ≤ 10^4，数组版更快更简洁。
//
// ============================================================
// 【易错点】
//
// 1. parent 初始化为 0 而不是 -1:
//    ✗ parent.assign(m*n, 0)  → (0,0) 的 id=0，无法区分"水"和"id=0的陆地"
//    ✓ parent.assign(m*n, -1)  → -1 明确表示"这个位置是水"
//
// 2. 忘记处理重复添加同一位置:
//    ✗ 直接 count++，不检查是否已是陆地 → 同一位置加两次多算一个岛
//    ✓ if (parent[id] != -1) { result.push_back(count); continue; }
//
// 3. union 时不检查是否同一集合就减 count:
//    ✗ void unite(x,y) { parent[find(y)] = find(x); count--; }
//      → 如果 x 和 y 已在同一集合，count 被多减了
//    ✓ if (rx == ry) return; // 同一集合不做任何操作
//
// 4. 坐标映射写错:
//    ✗ id = r * m + c  （用行数 m，当 m != n 时 id 会冲突或越界）
//    ✓ id = r * n + c  （用列数 n，这是行优先存储的标准映射）
//
// 5. find 函数没做路径压缩导致 TLE:
//    ✗ int find(int x) { while(parent[x]!=x) x=parent[x]; return x; }
//      → 最坏 O(n) 每次查找
//    ✓ 递归路径压缩 或 迭代路径压缩(parent[x]=find(parent[x]))
//
// ============================================================
// 【面试追问 — 递进链】
//
// Q1 (基础理解): "暴力解怎么做？时间复杂度？瓶颈在哪？"
//   → 每次 BFS 重新扫描，O(k×m×n)。瓶颈：重复遍历已知连通区域。
//
// Q2 (优化): "每次只加一个格子，能不能增量维护？"
//   → 用并查集。新加陆地 count++，检查4个邻居，不同集合就 union 并 count--。
//      时间 O(k×α(mn))。
//
// Q3 (深入): "路径压缩和按秩合并各自的作用？不做会怎样？"
//   → 不做路径压缩：find 可能退化为 O(n)（链状树）。
//     不做按秩合并：合并后树可能变高。
//     两者结合：均摊 O(α(n))，α 是反阿克曼函数（对实际输入 ≤ 4）。
//
// Q4 (变体): "如果还支持 removeLand 操作呢？"
//   → 并查集不支持高效删除。可以考虑离线处理（时间倒流，删除变添加），
//     或者用更复杂的数据结构（Link-Cut Tree）。
//
// Q5 (系列题串联): "和 LC 200 的关系？"
//   → LC 200 是静态版：网格不变，一次 BFS/DFS 即可。
//     LC 305 是动态版：网格在变，需要并查集增量维护连通性。
//     面试官从 200 追问到 305，考的是从"静态遍历"到"动态连通性"的思维升级。
//
// ============================================================
// 【相关题型】
//
// - 200. 岛屿数量: 静态版，BFS/DFS 一次扫描。305 是其动态升级版。
// - 547. 省份数量: 并查集求连通分量数，输入是邻接矩阵而非网格。
//   复用: 完全相同的 union-find 模板，区别在于输入格式和邻居定义。
// - 323. 无向图中连通分量的数目: 并查集基础模板题，比305简单（不需动态添加）。
// - 721. 账户合并: 并查集应用，合并有共同邮箱的账户。
//   复用: union-find 框架，区别在于需要用 map 将字符串映射到 id。
// - 128. 最长连续序列: 可用并查集解决，"连续数字"等价于"相邻连接"。
// ============================================================
