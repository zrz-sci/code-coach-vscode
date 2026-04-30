# 803. 打砖块

## 核心思路

本质是一个**逆向并查集**问题。正向模拟"打掉砖块后哪些会掉落"很难，因为要追踪连通性的变化。但如果**逆序还原**——先把所有砖块打掉，然后从最后一个 hit 开始逐个加回来——每次加回一个砖块时，用并查集检查有多少原本不连通于顶部的砖块因此变得连通于顶部，那些就是"会掉落"的砖块数量。

## 思维链

1. **读完题第一反应** → 每次打掉一个砖块后 BFS/DFS 检查连通性？O(hits * m * n) 太慢。
2. **关键困难** → 删除操作在并查集中很难处理（并查集只支持合并，不支持分裂）。
3. **逆向思维** → 如果把操作反过来——不是"删除"而是"添加"，那并查集就能用了！
4. **具体做法** → 先把所有 hits 的砖块都去掉，建立初始并查集状态。然后从最后一个 hit 逆序加回砖块，每次记录新增的与顶部连通的砖块数。
5. **虚拟节点** → 用一个虚拟节点代表"顶部"，所有第 0 行的砖块都与这个虚拟节点连通。这样检查某砖块是否"稳定"只需看它是否与虚拟节点连通。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 逆向并查集 | 逆序添加砖块 + Union-Find | O((m*n + hits) * α(m*n)) | O(m*n) | ⭐ 必须写出 |
| BFS/DFS 暴力 | 每次打掉后重新检查连通性 | O(hits * m * n) | O(m*n) | 仅作为暴力baseline |

## 关键提示

1. **逆向操作是本题的核心 trick**：并查集不支持 split/delete，但支持 union。把"删除"反转为"添加"就能用并查集了。
2. **虚拟顶部节点**：创建一个编号为 `m*n` 的虚拟节点，第 0 行的所有砖块都和它 union。判断砖块是否稳定 = 判断是否与虚拟节点同组。
3. **掉落数量的计算**：加回砖块前，记录与顶部连通的砖块数 `before`；加回并 union 后，记录新的数量 `after`。掉落数 = `after - before - 1`（减 1 因为加回的砖块本身不算掉落）。
4. **注意 hit 打的位置可能原本就没有砖块**：如果 `grid[r][c] == 0`，该 hit 不产生掉落（结果为 0）。
5. **标记技巧**：先遍历 hits，把被打的位置从 grid 中减去（标记为已打掉），然后基于修改后的 grid 建并查集。逆序恢复时再加回来。

## 解法详解

### 解法1: 逆向并查集 -- O((m*n + hits) * alpha(m*n)) / O(m*n) ⭐ 面试首选

**思考过程**: 并查集天然支持合并但不支持分裂，所以把"打掉砖块"反转为"添加砖块"。

```
操作流程:
1. 复制 grid，将所有 hits 位置标记为 0（先全部打掉）
2. 基于标记后的 grid 建立并查集（第0行砖块连虚拟顶部）
3. 逆序遍历 hits：
   a. 如果原始 grid 该位置就是 0 → result[i] = 0
   b. 否则：恢复砖块，与四周已有砖块 union
   c. 如果在第0行，额外和虚拟顶部 union
   d. result[i] = 新增连通到顶部的砖块数 - 1
```

```cpp
class Solution {
public:
    vector<int> hitBricks(vector<vector<int>>& grid, vector<vector<int>>& hits) {
        int m = grid.size(), n = grid[0].size();
        int top = m * n;  // 虚拟顶部节点

        // 并查集（带 size 的）
        vector<int> parent(m * n + 1), sz(m * n + 1, 1);
        iota(parent.begin(), parent.end(), 0);

        // 将 hits 位置标记（grid 值减 1，原来是1变0，原来是0变-1）
        auto g = grid;  // 工作副本
        for (auto& h : hits) {
            g[h[0]][h[1]]--;
        }

        // lambda: find + union
        function<int(int)> find = [&](int x) {
            while (parent[x] != x) x = parent[x] = parent[parent[x]];
            return x;
        };
        auto unite = [&](int a, int b) {
            a = find(a); b = find(b);
            if (a == b) return;
            if (sz[a] < sz[b]) swap(a, b);
            parent[b] = a;
            sz[a] += sz[b];
        };
        auto id = [&](int r, int c) { return r * n + c; };

        // 基于打掉所有 hits 后的 grid 建并查集
        for (int r = 0; r < m; r++) {
            for (int c = 0; c < n; c++) {
                if (g[r][c] != 1) continue;
                if (r == 0) unite(id(r, c), top);
                if (r > 0 && g[r-1][c] == 1) unite(id(r, c), id(r-1, c));
                if (c > 0 && g[r][c-1] == 1) unite(id(r, c), id(r, c-1));
            }
        }

        // 逆序恢复砖块
        vector<int> result(hits.size(), 0);
        int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};

        for (int i = hits.size() - 1; i >= 0; i--) {
            int r = hits[i][0], c = hits[i][1];
            if (grid[r][c] == 0) continue;  // 原本就没砖

            int before = sz[find(top)];  // 恢复前顶部连通的砖块数

            // 恢复砖块
            g[r][c] = 1;
            if (r == 0) unite(id(r, c), top);
            for (auto& d : dirs) {
                int nr = r + d[0], nc = c + d[1];
                if (nr >= 0 && nr < m && nc >= 0 && nc < n && g[nr][nc] == 1) {
                    unite(id(r, c), id(nr, nc));
                }
            }

            int after = sz[find(top)];
            result[i] = max(0, after - before - 1);  // 减1因为加回的砖块本身不算
        }

        return result;
    }
};
```

## 易错点

1. **没有处理 hit 位置原本就没有砖块的情况**：
   - ✗ 假设所有 hit 位置一定有砖块 → 可能多算
   - ✓ 检查原始 grid，如果 `grid[r][c] == 0`，该 hit 结果为 0

2. **掉落数量忘记减 1**：
   - ✗ `result[i] = after - before` → 把加回的砖块本身也算成了"掉落"
   - ✓ `result[i] = after - before - 1` → 减去加回的砖块本身

3. **并查集 size 统计错误**：
   - ✗ 用 rank 合并但不维护 size → 无法计算连通顶部的砖块数
   - ✓ 用带 size 的并查集，合并时累加 size

4. **标记 hits 时直接设为 0**：
   - ✗ `g[r][c] = 0` → 无法区分"原本就是0"和"被 hit 打掉"
   - ✓ `g[r][c]--` → 原本是1变0（被打），原本是0变-1（本来就没砖）

## 面试追问

**Q1: 为什么不能正向模拟（直接打掉后检查连通性）？**
> 正向模拟需要在每次打掉砖块后重新 BFS/DFS 检查连通性，每次 O(m*n)，总 O(hits * m * n)。对于 hits 达 4*10^4、grid 达 200*200 的规模，约 10^9 次操作，TLE。逆向并查集的 amortized 复杂度接近 O((m*n + hits))。

**Q2: 并查集为什么不支持删除？**
> 并查集通过路径压缩和按秩合并实现接近 O(1) 的 union/find。但删除一个节点需要"拆开"合并关系，会破坏树结构。没有已知的高效方法在支持路径压缩的并查集上做删除操作。

**Q3: 除了逆向并查集，还有其他方法吗？**
> 可以用 BFS 暴力但会 TLE。理论上也可以用 Link-Cut Tree 或其他动态连通性数据结构，但实现复杂度远高于逆向并查集。逆向并查集是这类"删除转添加"问题的标准技巧。

## 相关题型

- **130. 被围绕的区域** — 并查集应用，虚拟节点连接边界的 O，思路类似本题的虚拟顶部节点。
- **200. 岛屿数量** — 并查集基础应用，二维网格上的连通性问题。
- **305. 岛屿数量 II** — 动态添加陆地 + 并查集，和本题"逆序添加砖块"思路完全一致。
- **1202. 交换字符串中的元素** — 并查集分组后组内排序，并查集的另一种应用场景。
