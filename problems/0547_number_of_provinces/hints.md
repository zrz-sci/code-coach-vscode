# 547. 省份数量

## 核心思路

经典的**连通分量计数**问题。输入是一个邻接矩阵表示的无向图，"省份"就是"连通分量"。问题转化为：图中有多少个连通分量？

## 思维链

1. **读完题第一反应** → n 个城市，`isConnected[i][j]=1` 表示城市 i 和 j 直接相连。"省份"就是一组直接或间接相连的城市。这不就是求**连通分量的个数**吗？
2. **建模** → `isConnected` 本身就是邻接矩阵！城市编号 0~n-1 就是节点，`isConnected[i][j]=1` 就是 i 和 j 之间有边。
3. **标准做法** → 求连通分量个数有三种经典方法：DFS、BFS、Union-Find。对于邻接矩阵，三者都适用。
4. **DFS 怎么做？** → 维护一个 `visited` 数组。遍历每个城市 i，如果 i 未访问过，就从 i 出发做 DFS，标记所有可达节点为已访问。每次触发新 DFS 就意味着发现了一个新连通分量，省份数 +1。
5. **Union-Find 怎么做？** → 遍历邻接矩阵上三角，对每条边 `(i,j)` 做 `union`，最后统计不同的根节点数。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| DFS | 遍历节点，未访问就DFS一次 | O(n^2) | O(n) | ⭐ 面试首选 |
| BFS | 遍历节点，未访问就BFS一次 | O(n^2) | O(n) | 变体 |
| Union-Find | 遍历边做union，数根节点 | O(n^2 * alpha(n)) | O(n) | 进阶/万能模板 |

## 关键提示

1. **邻接矩阵 vs 邻接表**：本题直接给了邻接矩阵，不需要自己建图。`isConnected[i][j]` 就能告诉你 i 和 j 是否相连。
2. **对称矩阵**：因为 `isConnected[i][j] == isConnected[j][i]`（无向图），Union-Find 只需遍历上三角（j > i），避免重复。
3. **DFS 遍历邻居的方式**：从节点 i 出发，邻居就是所有满足 `isConnected[i][j] == 1` 的 j（0 ≤ j < n）。这不同于邻接表，需要 O(n) 扫描整行。
4. **visited 数组是关键**：不能省略！没有 visited 会无限递归（i → j → i → j...）。
5. **复杂度分析**：虽然 DFS 看起来是 O(n) 个节点各遍历一行，但每个节点只被访问一次（visited 保证），所以总时间是 O(n^2)（每条边被访问两次）。

## DFS 递归过程可视化

```
输入: isConnected = [[1,1,0],[1,1,0],[0,0,1]]

邻接矩阵 → 图结构:
  0 --- 1     2
  (0和1相连)  (2独立)

DFS 过程:

visited = [F, F, F], provinces = 0

i=0: visited[0]=F → 启动 DFS(0)
  ┌─ DFS(0):
  │   visited[0] = true
  │   j=0: isConnected[0][0]=1, visited[0]=T → 跳过(已访问)
  │   j=1: isConnected[0][1]=1, visited[1]=F → 递归 DFS(1)
  │   ┌─ DFS(1):
  │   │   visited[1] = true
  │   │   j=0: isConnected[1][0]=1, visited[0]=T → 跳过
  │   │   j=1: isConnected[1][1]=1, visited[1]=T → 跳过
  │   │   j=2: isConnected[1][2]=0 → 跳过
  │   └─ 返回
  │   j=2: isConnected[0][2]=0 → 跳过
  └─ 返回
  provinces++ → provinces = 1
  visited = [T, T, F]

i=1: visited[1]=T → 跳过 (已在上一轮DFS中访问)

i=2: visited[2]=F → 启动 DFS(2)
  ┌─ DFS(2):
  │   visited[2] = true
  │   j=0: isConnected[2][0]=0 → 跳过
  │   j=1: isConnected[2][1]=0 → 跳过
  │   j=2: isConnected[2][2]=1, visited[2]=T → 跳过
  └─ 返回
  provinces++ → provinces = 2
  visited = [T, T, T]

最终: provinces = 2 ✓
```

## 解法详解

### 解法1: DFS — O(n^2) / O(n) ⭐ 面试首选

**思考过程**: 连通分量的标准求法——从每个未访问节点出发 DFS，能一次 DFS 到达的所有节点属于同一个连通分量。

```cpp
class Solution {
public:
    int findCircleNum(vector<vector<int>>& isConnected) {
        int n = isConnected.size();
        vector<bool> visited(n, false);
        int provinces = 0;

        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                dfs(isConnected, visited, i, n);
                provinces++;  // 每次新DFS = 发现一个新省份
            }
        }
        return provinces;
    }

private:
    void dfs(vector<vector<int>>& g, vector<bool>& visited, int i, int n) {
        visited[i] = true;
        for (int j = 0; j < n; j++) {
            if (g[i][j] == 1 && !visited[j]) {
                dfs(g, visited, j, n);
            }
        }
    }
};
```

**关键点**:
- 外层循环遍历所有节点，每个未访问的节点触发一次 DFS → 一个新连通分量。
- DFS 函数中，从节点 i 出发，检查 `g[i][j]==1 && !visited[j]` 来找未访问的邻居。
- `visited[i] = true` 必须在遍历邻居**之前**设置，否则会重复访问导致无限递归。
- 空间 O(n)：visited 数组 O(n) + 递归栈最深 O(n)（链式图的情况）。

---

### 解法2: BFS — O(n^2) / O(n)

**换个遍历方式**: DFS 用递归（栈），BFS 用队列，逻辑完全等价。

```cpp
class Solution {
public:
    int findCircleNum(vector<vector<int>>& isConnected) {
        int n = isConnected.size();
        vector<bool> visited(n, false);
        int provinces = 0;

        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                // BFS 从节点 i 出发
                queue<int> q;
                q.push(i);
                visited[i] = true;
                while (!q.empty()) {
                    int node = q.front(); q.pop();
                    for (int j = 0; j < n; j++) {
                        if (isConnected[node][j] == 1 && !visited[j]) {
                            visited[j] = true;
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
```

**关键点**:
- BFS 中 `visited[j] = true` 应在**入队时**设置（而不是出队时），避免同一节点被多次入队。
- 对于连通分量计数，DFS 和 BFS 效果完全一样。DFS 代码更短，BFS 不用担心栈溢出。

---

### 解法3: Union-Find (并查集) — O(n^2 * alpha(n)) / O(n)

**不同视角**: 把每个城市看作一个独立集合，扫描邻接矩阵上三角，对每条边 `(i,j)` 做 union。最后数有多少个不同的根。

```
Union-Find 过程:
isConnected = [[1,1,0],[1,1,0],[0,0,1]]

初始: parent = [0, 1, 2]  (每个节点是自己的根)

扫描上三角 (i < j):
  (0,1): isConnected[0][1]=1 → union(0,1) → parent = [0, 0, 2]
  (0,2): isConnected[0][2]=0 → 跳过
  (1,2): isConnected[1][2]=0 → 跳过

统计不同根的数量:
  find(0)=0, find(1)=0, find(2)=2
  不同根: {0, 2} → 2个

结果: 2 ✓
```

```cpp
class Solution {
public:
    int findCircleNum(vector<vector<int>>& isConnected) {
        int n = isConnected.size();
        vector<int> parent(n), rank_(n, 0);
        iota(parent.begin(), parent.end(), 0); // parent[i] = i

        // 遍历上三角，对每条边做 union
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                if (isConnected[i][j] == 1) {
                    unite(parent, rank_, i, j);
                }
            }
        }

        // 统计不同根的数量
        int provinces = 0;
        for (int i = 0; i < n; i++) {
            if (find(parent, i) == i) provinces++;
        }
        return provinces;
    }

private:
    int find(vector<int>& parent, int x) {
        if (parent[x] != x) {
            parent[x] = find(parent, parent[x]); // 路径压缩
        }
        return parent[x];
    }

    void unite(vector<int>& parent, vector<int>& rank_, int x, int y) {
        int rx = find(parent, x), ry = find(parent, y);
        if (rx == ry) return;
        // 按秩合并
        if (rank_[rx] < rank_[ry]) swap(rx, ry);
        parent[ry] = rx;
        if (rank_[rx] == rank_[ry]) rank_[rx]++;
    }
};
```

**关键点**:
- **路径压缩 + 按秩合并**使得每次 find/union 操作接近 O(1)（准确说是 O(alpha(n))，alpha 是反阿克曼函数，实际 ≤ 5）。
- 只遍历上三角 `j > i`：因为 `isConnected[i][j] == isConnected[j][i]`，避免重复 union。
- 统计根节点：`find(parent, i) == i` 的节点就是某个连通分量的根。

## 解法对比

|  | DFS | BFS | Union-Find |
|--|-----|-----|------------|
| 时间 | O(n^2) | O(n^2) | O(n^2 * alpha(n)) ≈ O(n^2) |
| 空间 | O(n) 栈+visited | O(n) 队列+visited | O(n) parent+rank |
| 代码量 | 最短 | 中等 | 最长 |
| 适用场景 | 通用 | 通用 | **动态加边/查询连通性** |
| 面试推荐 | ⭐ 首选 | 替代方案 | 追问"如果边是动态添加的" |

## 易错点

1. **混淆邻接矩阵和邻接表**：
   - 本题是 n x n 的邻接矩阵（节点是城市，不是网格中的点）
   - ✗ 把它当成网格做 flood fill（上下左右四方向）
   - ✓ 对于节点 i，遍历 `j = 0 to n-1` 检查 `isConnected[i][j]`

2. **忘记标记 visited 导致无限递归**：
   - ✗ DFS 中不设 `visited[i] = true` → i→j→i→j 无限循环
   - ✓ 进入 DFS 的第一行就标记 `visited[i] = true`

3. **Union-Find 遍历整个矩阵导致重复 union**：
   - ✗ `for i, for j: if g[i][j]==1 union(i,j)` → 每条边 union 两次（浪费但不出错）
   - ✓ 只遍历上三角 `j > i`，或者至少从 `j = i+1` 开始

4. **n 的理解错误**：
   - n 是城市数量（矩阵的行/列数），不是边的数量
   - `isConnected.size()` = n，矩阵是 n x n

## 面试追问

**Q1: DFS 和 BFS 哪个更好？有什么区别？**
> 对于这道题，两者时间空间复杂度完全一样。DFS 代码更短（递归天然用栈），BFS 在图很深时不会栈溢出。面试中任选一个即可。

**Q2: 如果边是动态添加的（每添加一条边就查询连通分量数），怎么办？**
> 用 Union-Find。初始 n 个连通分量，每次 union 成功（两个根不同）就分量数减 1。不需要每次重新遍历，单次操作 O(alpha(n))。

**Q3: 这道题和 200. 岛屿数量有什么区别？**
> 200 题的输入是网格（每个格子和上下左右四邻居相连），本题的输入是邻接矩阵（每个节点和 n-1 个节点中的部分相连）。核心思想都是数连通分量，但遍历邻居的方式不同。

**Q4: 能否用邻接表优化？**
> 可以。先遍历矩阵建邻接表，时间 O(n^2)。之后 DFS/BFS 只访问真实的边，时间 O(n + E)。当图稀疏时（E << n^2），邻接表版更快。但建表本身也是 O(n^2)，所以总时间不变。只有当邻接表直接给出时才有优势。

## 相关题型

- **200. 岛屿数量** — 网格上的连通分量计数（DFS/BFS flood fill）。和本题核心相同，但遍历方式不同（四方向 vs 邻接矩阵）。
- **323. 无向图中连通分量的数目** — 和本题完全等价，只是输入换成了边列表而非邻接矩阵。
- **684. 冗余连接** — Union-Find 应用：找到使图成环的那条边。
- **990. 等式方程的可满足性** — 把变量看作节点，等式 `a==b` 做 union，然后检查不等式是否矛盾。Union-Find 的经典应用。
