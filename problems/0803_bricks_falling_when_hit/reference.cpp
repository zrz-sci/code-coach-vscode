/*
 * LeetCode 803: Bricks Falling When Hit (打砖块)
 *
 * 【题目本质】
 * 二维网格中的砖块通过顶部保持稳定。依次打掉指定位置的砖块，每次打击后
 * 计算有多少砖块因失去与顶部的连通性而掉落。
 * 核心难点：并查集不支持删除 → 逆向操作，把"删除"变成"添加"。
 *
 * 【解法总览】
 * 解法1: 逆向并查集 — O((m*n+hits)*α) / O(m*n) — 标准做法
 * 解法2: BFS 暴力    — O(hits*m*n) / O(m*n)    — 会 TLE，仅作参考
 */

// ============================================================
// 解法1: 逆向并查集 — 标准做法 ⭐
// 时间: O((m*n + hits) * α(m*n))  空间: O(m*n)
//
// 【思路】
// 核心 trick：并查集只支持 union（合并），不支持 split（拆分）。
// 如果正向模拟"打掉砖块"，需要拆分连通分量，并查集做不到。
// 但如果把操作反过来——先把所有砖块都打掉，再逐个加回来——
// 就变成了 union 操作，并查集完美适用。
//
// 算法步骤：
// 1. 复制 grid，把所有 hits 对应位置的砖块标记为已打掉
// 2. 基于标记后的 grid 建立并查集（第0行砖块连虚拟顶部节点）
// 3. 从最后一个 hit 逆序恢复：
//    a. 记录恢复前与顶部连通的砖块数 before
//    b. 恢复砖块，与四周已有砖块 union
//    c. 记录恢复后与顶部连通的砖块数 after
//    d. 该 hit 导致的掉落数 = after - before - 1
//       （减1因为恢复的砖块本身不算掉落）
//
// 虚拟顶部节点的作用：
//   编号 m*n 的虚拟节点代表"顶部"。第0行的所有砖块都 union 到它。
//   判断砖块是否稳定 = 判断它的 find() 是否等于 find(top)。
//   统计稳定砖块数 = size[find(top)]。
//
// 示例: grid = [[1,0,0,0],[1,1,1,0]], hits = [[1,0]]
//
// 步骤1: 标记 hits → g = [[1,0,0,0],[0,1,1,0]]
// 步骤2: 建并查集
//   (0,0)=1 → union(0, top)
//   (1,1)=1, (1,2)=1 → union((1,1),(1,2))
//   注意 (1,1) 和 (1,2) 没有连到顶部（上方都是0）
//   顶部连通: {(0,0)} → size[find(top)] = 2 (含虚拟节点)
//
// 步骤3: 逆序恢复 hit[0]=(1,0)
//   before = size[find(top)] = 2
//   恢复 g[1][0] = 1
//   union((1,0), 上方(0,0)) → (1,0)连入顶部组
//   union((1,0), 右方(1,1)) → (1,1),(1,2)也连入顶部组
//   after = size[find(top)] = 5  (虚拟 + (0,0) + (1,0) + (1,1) + (1,2))
//   result[0] = 5 - 2 - 1 = 2 ✓
// ============================================================
class Solution1 {
public:
    vector<int> hitBricks(vector<vector<int>>& grid, vector<vector<int>>& hits) {
        int m = grid.size(), n = grid[0].size();
        int top = m * n; // 虚拟顶部节点编号

        // ---- 并查集数据结构 ----
        vector<int> parent(m * n + 1);
        vector<int> sz(m * n + 1, 1); // 每个连通分量的大小
        iota(parent.begin(), parent.end(), 0); // parent[i] = i

        // find with path compression
        function<int(int)> find = [&](int x) -> int {
            while (parent[x] != x) {
                parent[x] = parent[parent[x]]; // 路径压缩
                x = parent[x];
            }
            return x;
        };

        // union by size
        auto unite = [&](int a, int b) {
            a = find(a);
            b = find(b);
            if (a == b) return;
            if (sz[a] < sz[b]) swap(a, b);
            parent[b] = a;
            sz[a] += sz[b];
        };

        // 二维坐标 → 一维编号
        auto id = [&](int r, int c) { return r * n + c; };

        // ---- 步骤1: 标记所有 hits 为已打掉 ----
        auto g = grid; // 工作副本
        for (auto& h : hits) {
            g[h[0]][h[1]]--; // 1→0(被打), 0→-1(原本就没砖)
        }

        // ---- 步骤2: 基于标记后的 grid 建并查集 ----
        for (int r = 0; r < m; r++) {
            for (int c = 0; c < n; c++) {
                if (g[r][c] != 1) continue;
                // 第0行连虚拟顶部
                if (r == 0) unite(id(r, c), top);
                // 与上方砖块连接
                if (r > 0 && g[r-1][c] == 1) unite(id(r, c), id(r-1, c));
                // 与左方砖块连接
                if (c > 0 && g[r][c-1] == 1) unite(id(r, c), id(r, c-1));
            }
        }

        // ---- 步骤3: 逆序恢复砖块 ----
        vector<int> result(hits.size(), 0);
        int dirs[4][2] = {{0,1}, {0,-1}, {1,0}, {-1,0}};

        for (int i = (int)hits.size() - 1; i >= 0; i--) {
            int r = hits[i][0], c = hits[i][1];

            // 原本就没有砖块 → 不产生掉落
            if (grid[r][c] == 0) continue;

            // 记录恢复前顶部组的大小
            int before = sz[find(top)];

            // 恢复砖块
            g[r][c] = 1;

            // 如果在第0行，连虚拟顶部
            if (r == 0) unite(id(r, c), top);

            // 与四周已有砖块 union
            for (auto& d : dirs) {
                int nr = r + d[0], nc = c + d[1];
                if (nr >= 0 && nr < m && nc >= 0 && nc < n && g[nr][nc] == 1) {
                    unite(id(r, c), id(nr, nc));
                }
            }

            // 计算掉落数量
            int after = sz[find(top)];
            // 减1：恢复的砖块本身不算掉落
            result[i] = max(0, after - before - 1);
        }

        return result;
    }
};

// ============================================================
// 解法2: BFS 暴力 — 每次打击后检查连通性
// 时间: O(hits * m * n)  空间: O(m*n)
//
// 【思路】
// 每次打掉一个砖块后，从顶部(第0行)所有砖块开始 BFS/DFS，
// 标记所有连通的砖块为"稳定"。没被标记的砖块就是掉落的。
//
// 简单但太慢：hits 可达 4*10^4，grid 可达 200*200 = 4*10^4，
// 总计 ≈ 1.6 * 10^9 次操作，会 TLE。
//
// 这里仅作为 baseline 参考，不适合面试提交。
// ============================================================
class Solution2 {
public:
    vector<int> hitBricks(vector<vector<int>>& grid, vector<vector<int>>& hits) {
        int m = grid.size(), n = grid[0].size();
        vector<int> result(hits.size());
        int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};

        for (int i = 0; i < (int)hits.size(); i++) {
            int r = hits[i][0], c = hits[i][1];
            if (grid[r][c] == 0) {
                result[i] = 0;
                continue;
            }
            grid[r][c] = 0; // 打掉

            // BFS 从顶部标记所有稳定砖块
            vector<vector<bool>> stable(m, vector<bool>(n, false));
            queue<pair<int,int>> q;
            for (int j = 0; j < n; j++) {
                if (grid[0][j] == 1) {
                    stable[0][j] = true;
                    q.push({0, j});
                }
            }
            while (!q.empty()) {
                auto [cr, cc] = q.front(); q.pop();
                for (auto& d : dirs) {
                    int nr = cr+d[0], nc = cc+d[1];
                    if (nr>=0 && nr<m && nc>=0 && nc<n
                        && grid[nr][nc]==1 && !stable[nr][nc]) {
                        stable[nr][nc] = true;
                        q.push({nr, nc});
                    }
                }
            }

            // 统计不稳定砖块（掉落）
            int fallen = 0;
            for (int x = 0; x < m; x++) {
                for (int y = 0; y < n; y++) {
                    if (grid[x][y] == 1 && !stable[x][y]) {
                        grid[x][y] = 0; // 掉落后消失
                        fallen++;
                    }
                }
            }
            result[i] = fallen;
        }
        return result;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法        | 时间              | 空间    | 优势                    | 劣势              |
// |------------|-------------------|---------|------------------------|-------------------|
// | 逆向并查集  | O((mn+h)*α)       | O(mn)   | 高效，标准面试解法        | 思维难度高         |
// | BFS 暴力   | O(h*mn)           | O(mn)   | 简单直接                | TLE，仅作baseline  |
//
// ============================================================
// 【易错点】
//
// 1. hit 位置原本就没有砖块:
//    ✗ 不检查 grid[r][c]，直接当有砖块处理
//    ✓ 检查 grid[r][c]==0 时跳过，result[i]=0
//
// 2. 掉落数量忘记减1:
//    ✗ result[i] = after - before → 把恢复的砖块本身也算掉落
//    ✓ result[i] = after - before - 1 → 恢复的砖块不算
//
// 3. 标记 hits 方式不正确:
//    ✗ g[r][c] = 0 → 无法区分原本0和被打的1
//    ✓ g[r][c]-- → 原本1变0(需恢复)，原本0变-1(不需恢复)
//
// 4. 并查集 size 在虚拟节点上的含义:
//    ✗ 以为 size[find(top)] 就是砖块数
//    ✓ size[find(top)] 包含虚拟节点自身，实际砖块数 = size - 1
//       但计算 after - before 时虚拟节点抵消，所以不影响差值
//
// ============================================================
// 【面试追问 — 递进链】
//
// Q1: 为什么用逆向而非正向并查集？
//     并查集只支持 union（合并），不支持 split（拆分）。
//     打掉砖块是"拆分"操作，无法直接用并查集。
//     逆向后变成"添加"操作，即 union，并查集完美适用。
//
// Q2: 虚拟顶部节点的作用是什么？
//     避免遍历第0行的所有砖块来判断是否与顶部连通。
//     所有第0行砖块都 union 到虚拟顶部节点，判断稳定性只需
//     一次 find 操作：find(brick) == find(top)。
//
// Q3: 如果砖块有不同的重量，掉落时需要按重量排序？
//     并查集需要额外维护每个分量的总重量而非简单的 size。
//     其他逻辑不变，只是 before/after 的统计从 size 变成 weight。
//
// ============================================================
// 【相关题型】
//
// - 305. 岛屿数量 II:
//   动态添加陆地+并查集。和本题思路完全一致——
//   都是"逐步添加+union"而不是"逐步删除"。
//
// - 130. 被围绕的区域:
//   虚拟节点连接边界O的技巧。和本题的虚拟顶部节点思路一致。
//
// - 200. 岛屿数量:
//   并查集在二维网格上的基础应用。本题是其进阶版。
// ============================================================
