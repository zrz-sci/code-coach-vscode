# 305. 岛屿数量 II

## 核心思路

本题本质是：**动态连通性问题**——每次添加一个陆地后，快速回答"当前有多少个连通分量（岛屿）"。这是并查集（Union-Find）的经典应用场景。

## 思维链

1. **读完题第一反应**：每次加一块陆地后，重新跑一遍 BFS/DFS 统计岛屿数量？→ 每次操作 O(m×n)，k 次操作总计 O(k×m×n)，太慢。

2. **暴力解的瓶颈**：每次加完陆地后，重新扫描整个网格做 BFS/DFS 来数岛屿，大量重复计算——之前已经连通的区域又被重新遍历了。

3. **关键观察**：每次只加了**一个**格子，最多影响上下左右 4 个邻居。我们能不能**增量维护**岛屿数量，而不是每次重算？

4. **增量思路**：
   - 新加一个陆地 → 岛屿数 +1
   - 检查四个方向的邻居，如果邻居已经是陆地 → 尝试合并
   - 每次成功合并两个**不同**的岛屿 → 岛屿数 -1

5. **什么数据结构支持"合并"和"判断是否同一集合"？**→ 并查集（Union-Find）！`find` 判断归属，`union` 合并集合，均接近 O(1)。

6. **最终方案**：用并查集维护所有已加陆地的连通关系，每次操作最多 4 次 union，总时间 O(k × α(m×n)) ≈ O(k)。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力 BFS/DFS | 每次加完重新扫描整个网格 | O(k × m × n) | O(m × n) | 能说出即可 |
| 并查集（基础） | 增量维护连通分量计数 | O(k × m × n) 最坏 | O(m × n) | ⭐ 必须写出 |
| 并查集（路径压缩+按秩合并） | 优化 find/union 到接近 O(1) | O(k × α(mn)) | O(m × n) | 加分项 |

## 关键提示

1. **增量思维**：不要每次重算。新加一个格子时，岛屿数先 +1，然后看四周有几个已存在的陆地能合并进来，每合并一次 -1。

2. **二维坐标一维化**：并查集通常用一维数组，把 `(r, c)` 映射为 `r * n + c`。

3. **如何区分"水"和"陆地"**：初始时所有节点的 parent 设为 -1（表示水），只有被 addLand 操作过的才是有效节点。

4. **重复添加同一位置**：题目没说 positions 中的坐标不重复！如果某个位置已经是陆地，直接跳过，输出当前岛屿数。

5. **ASCII 示意图——示例演示**：
```
初始:           Op1(0,0):       Op2(0,1):       Op3(1,2):       Op4(2,1):
0 0 0           1 0 0           1 1 0           1 1 0           1 1 0
0 0 0           0 0 0           0 0 0           0 0 1           0 0 1
0 0 0           0 0 0           0 0 0           0 0 0           0 1 0
islands=0       islands=1       islands=1       islands=2       islands=3
                新加(0,0)       新加(0,1)       新加(1,2)       新加(2,1)
                无邻居陆地      与(0,0)合并     无邻居陆地      无邻居陆地
                +1=1            +1-1=1          +1=2            +1=3
```

## 解法详解

### 解法1: 暴力 BFS — O(k × m × n) / O(m × n)

**思考过程**：最直接的想法——每次加完一块陆地后，重新扫描整个网格用 BFS/DFS 统计有多少个岛屿。这和 LeetCode 200（岛屿数量）完全一样，只是重复做 k 次。

```cpp
class Solution {
public:
    vector<int> numIslands2(int m, int n, vector<vector<int>>& positions) {
        vector<vector<int>> grid(m, vector<int>(n, 0));
        vector<int> result;
        int dx[] = {0, 0, 1, -1};
        int dy[] = {1, -1, 0, 0};
        
        for (auto& pos : positions) {
            grid[pos[0]][pos[1]] = 1;
            
            // 每次都重新 BFS 计数（和 LC200 一样）
            vector<vector<bool>> visited(m, vector<bool>(n, false));
            int count = 0;
            for (int i = 0; i < m; i++) {
                for (int j = 0; j < n; j++) {
                    if (grid[i][j] == 1 && !visited[i][j]) {
                        count++;
                        // BFS 标记整个岛
                        queue<pair<int,int>> q;
                        q.push({i, j});
                        visited[i][j] = true;
                        while (!q.empty()) {
                            auto [x, y] = q.front(); q.pop();
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
```

**关键点**：这个解法完全正确但太慢。每次操作都扫描整个网格，做了大量重复工作。但它帮我们理解题意——我们需要一种方式来**增量维护**岛屿数量。

### 解法2: 并查集（路径压缩 + 按秩合并）— O(k × α(mn)) / O(m × n) ⭐ 面试首选

**从解法1优化**：解法1的瓶颈是每次加完陆地后重新遍历整个网格。但每次只加了一个格子，最多影响 4 个邻居。我们用并查集来追踪连通性，每次操作只需常数次 union。

**并查集核心操作**：
- `find(x)`：找 x 所属集合的代表（根节点），路径压缩使后续查找更快
- `union(x, y)`：合并两个集合，按秩合并保持树平衡

**增量维护逻辑**：
```
addLand(r, c):
  如果 (r,c) 已经是陆地 → 跳过
  count++                        // 先当作新岛屿
  对四个方向的邻居 (nr, nc):
    如果邻居已经是陆地:
      如果 find(r*n+c) != find(nr*n+nc):  // 不在同一个集合
        union(r*n+c, nr*n+nc)
        count--                  // 合并了两个岛，总数减1
```

```cpp
class Solution {
public:
    vector<int> parent;
    vector<int> rank_;
    int count;
    
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]); // 路径压缩
        }
        return parent[x];
    }
    
    void unite(int x, int y) {
        int rx = find(x), ry = find(y);
        if (rx == ry) return; // 已经在同一集合，不做操作
        // 按秩合并：矮树挂到高树下面
        if (rank_[rx] < rank_[ry]) swap(rx, ry);
        parent[ry] = rx;
        if (rank_[rx] == rank_[ry]) rank_[rx]++;
        count--; // 成功合并 → 岛屿数 -1
    }
    
    vector<int> numIslands2(int m, int n, vector<vector<int>>& positions) {
        parent.assign(m * n, -1); // -1 表示水
        rank_.assign(m * n, 0);
        count = 0;
        
        vector<int> result;
        int dx[] = {0, 0, 1, -1};
        int dy[] = {1, -1, 0, 0};
        
        for (auto& pos : positions) {
            int r = pos[0], c = pos[1];
            int id = r * n + c;
            
            // 处理重复添加：已经是陆地就跳过
            if (parent[id] != -1) {
                result.push_back(count);
                continue;
            }
            
            // 新建陆地
            parent[id] = id; // 自己是自己的根
            count++;
            
            // 检查四个方向，尝试合并
            for (int d = 0; d < 4; d++) {
                int nr = r + dx[d], nc = c + dy[d];
                int nid = nr * n + nc;
                if (nr >= 0 && nr < m && nc >= 0 && nc < n 
                    && parent[nid] != -1) { // 邻居是陆地
                    unite(id, nid);
                }
            }
            
            result.push_back(count);
        }
        return result;
    }
};
```

**关键点**：
- `parent[id] == -1` 表示水，`parent[id] >= 0` 表示陆地
- 路径压缩 + 按秩合并使得 find/union 的均摊时间为 O(α(n))，α 是反阿克曼函数，实际上可视为常数

### 解法3: 并查集（用 HashMap 替代数组）— O(k × α(mn)) / O(k)

**适用场景**：当 m × n 非常大但 positions 很少时（稀疏网格），用 HashMap 代替数组可以节省空间。

```cpp
class Solution {
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
            
            if (parent.count(id)) { // 已经是陆地
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
```

## 解法对比

| | 暴力 BFS | 并查集（数组） | 并查集（HashMap） |
|---|---|---|---|
| 时间 | O(k × m × n) | O(k × α(mn)) | O(k × α(mn)) 均摊 |
| 空间 | O(m × n) | O(m × n) | O(k) |
| 优势 | 容易理解 | 速度快 | 稀疏网格省空间 |
| 劣势 | 太慢 | 大网格浪费空间 | HashMap 常数大 |

**面试推荐**：解法2（并查集+数组），代码简洁，时间最优，是面试标准答案。

## 易错点

1. **忘记处理重复添加同一位置**：
   - ✗ 每次都 `count++` 然后合并 → 同一位置被加两次会多算岛屿
   - ✓ 先检查 `parent[id] != -1`，如果已经是陆地直接跳过

2. **parent 数组初始化为 0 而不是 -1**：
   - ✗ `parent.assign(m*n, 0)` → 无法区分"水"和"id=0 的陆地"
   - ✓ `parent.assign(m*n, -1)`，-1 表示水，只有被操作过的格子才设为 `parent[id] = id`

3. **union 时忘记检查 `rx == ry`**：
   - ✗ 直接 `parent[ry] = rx; count--;` → 同一集合的两个元素合并会多减 count
   - ✓ 先 `if (rx == ry) return;`

4. **二维坐标转一维时用错列数**：
   - ✗ `id = r * m + c`（用行数 m）
   - ✓ `id = r * n + c`（用列数 n）

## 面试追问

**Q1: 如果不用并查集，暴力解怎么做？瓶颈在哪？**
> 每次加完陆地后重新 BFS/DFS 扫描整个网格统计岛屿数。瓶颈是每次都重复遍历所有已知陆地，O(k × m × n) 太慢。

**Q2: 为什么并查集适合这个问题？路径压缩和按秩合并分别解决什么？**
> 并查集天然支持"合并两个集合"和"查询是否同一集合"——正好对应"两个相邻陆地合并为一个岛"和"判断邻居是否属于同一个岛"。路径压缩使 find 更快（树退化→扁平化），按秩合并保持树平衡（矮树挂高树下）。两者结合使 find/union 均摊 O(α(n))。

**Q3: 如果 positions 中有大量重复坐标怎么办？如果网格极其稀疏（m×n=10^9 但 k=100）怎么办？**
> 重复坐标：添加前检查是否已是陆地，O(1) 跳过。稀疏网格：用 HashMap 代替数组存 parent，空间从 O(m×n) 降到 O(k)。

**Q4: 这道题和 LC 200（岛屿数量）有什么关系？**
> LC 200 是静态问题（网格不变），一次 BFS/DFS 即可。LC 305 是动态问题（网格在变），需要并查集增量维护。如果面试官先问 200 再问 305，就是在测试你能否从"静态遍历"升级到"动态连通性维护"。

## 相关题型

- **200. 岛屿数量** — 静态版本，BFS/DFS 一次扫描即可。305 是其动态升级版。
- **547. 省份数量 / 朋友圈** — 同样是并查集求连通分量数，区别是输入是邻接矩阵而非网格。
- **128. 最长连续序列** — 也可以用并查集解决连通性问题（数字连续 = 相邻连接）。
- **721. 账户合并** — 并查集的另一个经典应用，合并有共同邮箱的账户。
- **323. 无向图中连通分量的数目** — 并查集模板题，比 305 简单（不需要动态添加节点）。