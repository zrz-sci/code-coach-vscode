# 2192. 有向无环图中一个节点的所有祖先

## 核心思路

给定一个 DAG，对每个节点找出所有祖先（可达前驱）并升序返回。本题的核心是**图的可达性传递**——如果 u 是 v 的祖先，v 是 w 的祖先，那 u 也是 w 的祖先。两种主力解法：**从每个节点出发做 DFS/BFS 标记后代** 或 **拓扑排序按序传递祖先集合**。

## 思维链

1. **读完题第一反应**：对每个节点 v，我想知道"谁能到达 v"。最朴素的做法——对每个节点做一次反向 BFS/DFS？但"反向"意味着要建反图。或者换个思路：从每个节点 u 出发正向 DFS/BFS，把 u 加入所有它能到达的节点的祖先列表中。

2. **正向 BFS from each node**：对节点 u = 0, 1, ..., n-1，从 u 出发 BFS，所有能到达的节点 v，都将 u 加入 `ancestors[v]`。因为 u 从小到大遍历，且用 set 去重，最终自然有序。时间 O(n*(n+e))，空间 O(n^2)。

3. **能不能更聪明？** 利用 DAG 的拓扑性质：如果按拓扑序处理，节点 v 的祖先 = 所有入边父节点的祖先集合 + 这些父节点本身。即 `ancestors[v] = Union(ancestors[p] + {p})` for all p -> v。这避免了重复遍历。

4. **拓扑排序解法的细节**：先计算入度，BFS 拓扑排序，每当处理节点 u 时，对 u 的每条出边 u->v，执行 `ancestors[v] = ancestors[v] ∪ ancestors[u] ∪ {u}`。因为用 set，合并和去重都自动完成。

5. **反向图 + 逐节点 DFS**：建反图（边反向），对每个节点 v 从反图做 DFS/BFS，找到所有能到达的节点即为 v 的祖先。等价于方法 1 但从"被查询节点"出发。

6. **复杂度比较**：三种方法时间都是 O(n*(n+e)) 最坏（集合合并本身有 O(n) 开销），但拓扑排序利用了"祖先集合的累积性"，实际中常数更小。

## 拓扑结构图示

```
示例 1: n=8, edges = [[0,3],[0,4],[1,3],[2,4],[2,7],[3,5],[3,6],[3,7],[4,6]]

拓扑层次:

    Layer 0:  [0]  [1]  [2]     (入度 0, 无祖先)
               │    │    │╲
    Layer 1:  [3]       [4] ╲   (3: 祖先={0,1}, 4: 祖先={0,2})
             / │ ╲     /    │
    Layer 2: [5][6] [7]      │  (5: {0,1,3}, 6: {0,1,2,3,4})
                    ↑────────┘   (7: {0,1,2,3})

拓扑排序顺序(一种): 0 → 1 → 2 → 3 → 4 → 5 → 6 → 7

处理过程:
  处理 0: ancestors[3] ∪= {0}, ancestors[4] ∪= {0}
  处理 1: ancestors[3] ∪= {1}
  处理 2: ancestors[4] ∪= {2}, ancestors[7] ∪= {2}
  处理 3: ancestors[5] ∪= {0,1} ∪ {3} = {0,1,3}
          ancestors[6] ∪= {0,1} ∪ {3} = {0,1,3}
          ancestors[7] ∪= {0,1} ∪ {3} = {0,1,2,3}
  处理 4: ancestors[6] ∪= {0,2} ∪ {4} → {0,1,2,3,4}
```

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 正向 BFS from each node | 从 u 出发 BFS 标记后代 | O(n*(n+e)) | O(n^2) | 能说出 |
| 拓扑排序 + 集合传递 | 拓扑序依次合并祖先集合 | O(n*(n+e)) | O(n^2) | ⭐ 首选 |
| 反向图 + DFS | 反图上对每个节点 DFS | O(n*(n+e)) | O(n^2) | 替代方案 |

> n = 节点数, e = 边数。最坏 n^2 空间用于存储祖先集合。

## 关键提示

1. **为什么用 set 而不是 vector？** set 自动排序去重，合并操作方便。最终直接转 vector 输出即可。题目要求升序返回，set 天然有序。

2. **正向 BFS 的巧妙之处**：从 u=0 到 u=n-1 依次遍历，因为节点编号就是升序的，插入到 set 中自然保持有序。即使不用 set，用 vector + 最后排序也可以，但 set 更优雅。

3. **拓扑排序版本的核心操作**：`for each (u->v): ancestors[v].insert(ancestors[u].begin(), ancestors[u].end()); ancestors[v].insert(u);`。集合合并是 O(|ancestors[u]|)，总体 O(n^2) 最坏。

4. **不需要反图**：正向 BFS 解法直接用原图即可。拓扑排序也用原图。反图解法需要额外建反向邻接表。

5. **空间必然 O(n^2)**：考虑完全 DAG（链状），节点 n-1 有 n-1 个祖先，总计 O(n^2) 个祖先关系。这是问题本身的下界，无法优化。

6. **BFS vs DFS 都可以**：正向遍历时 BFS/DFS 效果相同，都是标记所有可达节点。BFS 用 queue，DFS 用递归或 stack。

## 解法详解

### 解法1: 正向 BFS from each node — O(n*(n+e)) / O(n^2)

**思考过程**：最直觉的方式——对每个节点 u，从 u 出发沿正向边 BFS，所有能到达的节点 v 都是 u 的后代，将 u 加入 `ancestors[v]`。

```
// 处理过程 (n=8 的例子)
// u=0: BFS 到达 3,4,5,6,7 → ancestors[3]∪={0},...,ancestors[7]∪={0}
// u=1: BFS 到达 3,5,6,7   → ancestors[3]∪={1},...,ancestors[7]∪={1}
// u=2: BFS 到达 4,6,7     → ancestors[4]∪={2},ancestors[6]∪={2},ancestors[7]∪={2}
// u=3: BFS 到达 5,6,7     → ancestors[5]∪={3},ancestors[6]∪={3},ancestors[7]∪={3}
// u=4: BFS 到达 6         → ancestors[6]∪={4}
// u=5,6,7: 无出边，skip
```

```cpp
class Solution {
public:
    vector<vector<int>> getAncestors(int n, vector<vector<int>>& edges) {
        // 建图（正向邻接表）
        vector<vector<int>> graph(n);
        for (auto& e : edges) {
            graph[e[0]].push_back(e[1]);
        }
        
        // 每个节点的祖先集合（set 自动排序去重）
        vector<set<int>> ancestorSets(n);
        
        // 从每个节点 u 出发 BFS，把 u 加入所有后代的祖先集合
        for (int u = 0; u < n; u++) {
            queue<int> q;
            vector<bool> visited(n, false);
            q.push(u);
            visited[u] = true;
            while (!q.empty()) {
                int cur = q.front(); q.pop();
                for (int next : graph[cur]) {
                    if (!visited[next]) {
                        visited[next] = true;
                        ancestorSets[next].insert(u); // u 是 next 的祖先
                        q.push(next);
                    }
                }
            }
        }
        
        // set 转 vector
        vector<vector<int>> result(n);
        for (int i = 0; i < n; i++) {
            result[i] = vector<int>(ancestorSets[i].begin(), ancestorSets[i].end());
        }
        return result;
    }
};
```

**关键点**：每轮 BFS 用独立的 visited 数组防止重复访问。u 从 0 到 n-1 遍历，但因为用 set，即使乱序插入也会自动排序。

### 解法2: 拓扑排序 + 集合传递 — O(n*(n+e)) / O(n^2) ⭐ 面试首选

**从解法1优化**：解法1 中，如果 u 能到达 v，v 能到达 w，那么 u 的 BFS 和 v 的 BFS 都会访问 w——存在重复工作。拓扑排序利用 DAG 的无环性，按拓扑序处理，每个节点的祖先集合由其所有前驱节点"传递"而来，避免重复遍历。

**状态转移**：`ancestors[v] = Union{ ancestors[p] ∪ {p} } for all edges p->v`

```
// 拓扑排序处理过程
// 入度: [0]=0, [1]=0, [2]=0, [3]=2, [4]=2, [5]=1, [6]=2, [7]=2
// 
// 初始队列: {0, 1, 2}
// 
// 处理 0: 出边→3,4
//   ancestors[3] ∪= {} ∪ {0} = {0},  indeg[3]=1
//   ancestors[4] ∪= {} ∪ {0} = {0},  indeg[4]=1
// 
// 处理 1: 出边→3
//   ancestors[3] ∪= {} ∪ {1} = {0,1}, indeg[3]=0 → 入队
// 
// 处理 2: 出边→4,7
//   ancestors[4] ∪= {} ∪ {2} = {0,2}, indeg[4]=0 → 入队
//   ancestors[7] ∪= {} ∪ {2} = {2},   indeg[7]=1
// 
// 处理 3: 出边→5,6,7
//   ancestors[5] ∪= {0,1} ∪ {3} = {0,1,3},     indeg[5]=0 → 入队
//   ancestors[6] ∪= {0,1} ∪ {3} = {0,1,3},     indeg[6]=1
//   ancestors[7] ∪= {0,1} ∪ {3} = {0,1,2,3},   indeg[7]=0 → 入队
// 
// 处理 4: 出边→6
//   ancestors[6] ∪= {0,2} ∪ {4} = {0,1,2,3,4}, indeg[6]=0 → 入队
```

```cpp
class Solution {
public:
    vector<vector<int>> getAncestors(int n, vector<vector<int>>& edges) {
        vector<vector<int>> graph(n);
        vector<int> indegree(n, 0);
        
        for (auto& e : edges) {
            graph[e[0]].push_back(e[1]);
            indegree[e[1]]++;
        }
        
        // 拓扑排序 (Kahn's BFS)
        queue<int> q;
        for (int i = 0; i < n; i++) {
            if (indegree[i] == 0) q.push(i);
        }
        
        vector<set<int>> ancestorSets(n);
        
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : graph[u]) {
                // v 的祖先 = u 的所有祖先 + u 本身
                ancestorSets[v].insert(ancestorSets[u].begin(), ancestorSets[u].end());
                ancestorSets[v].insert(u);
                if (--indegree[v] == 0) {
                    q.push(v);
                }
            }
        }
        
        // set 转 vector
        vector<vector<int>> result(n);
        for (int i = 0; i < n; i++) {
            result[i].assign(ancestorSets[i].begin(), ancestorSets[i].end());
        }
        return result;
    }
};
```

**关键点**：
- 拓扑排序保证处理 u 时，u 的所有祖先已经收集完毕，可以安全传递给后继。
- `set::insert(begin, end)` 进行批量插入，set 自动去重排序。
- 入度减到 0 才入队，确保一个节点的所有前驱都处理完后才处理它。

### 解法3: 反向图 + DFS — O(n*(n+e)) / O(n^2)

**完全不同的视角**：建反向图（所有边反转），对每个节点 v，在反图上做 DFS/BFS，所有能到达的节点就是 v 的祖先。

```cpp
class Solution {
public:
    vector<vector<int>> getAncestors(int n, vector<vector<int>>& edges) {
        // 建反向图
        vector<vector<int>> reverseGraph(n);
        for (auto& e : edges) {
            reverseGraph[e[1]].push_back(e[0]); // 反转边方向
        }
        
        vector<vector<int>> result(n);
        
        for (int v = 0; v < n; v++) {
            // 在反图上从 v 出发 BFS，找到所有 v 的祖先
            set<int> ancestors;
            vector<bool> visited(n, false);
            queue<int> q;
            q.push(v);
            visited[v] = true;
            while (!q.empty()) {
                int cur = q.front(); q.pop();
                for (int prev : reverseGraph[cur]) {
                    if (!visited[prev]) {
                        visited[prev] = true;
                        ancestors.insert(prev);
                        q.push(prev);
                    }
                }
            }
            result[v].assign(ancestors.begin(), ancestors.end());
        }
        
        return result;
    }
};
```

**关键点**：反图上从 v 出发能到达的所有节点，在原图中都能到达 v，因此都是 v 的祖先。

## 解法对比

| | 正向 BFS | 拓扑排序 | 反向图 DFS |
|--|---------|---------|-----------|
| 时间 | O(n*(n+e)) | O(n*(n+e)) | O(n*(n+e)) |
| 空间 | O(n^2) | O(n^2) | O(n^2+e) |
| 思路难度 | 低 | 中 | 低 |
| 代码难度 | 低 | 中 | 低 |
| 面试建议 | 可以 | ⭐首选 | 替代方案 |

**选择建议**：
- 面试首选拓扑排序：展示对 DAG 性质的理解，代码也不复杂
- 正向 BFS 更直觉，适合快速写出正确解
- 反向图需要额外空间建反图，但思路最直观（"反向找祖先"）

## 易错点

1. **忘记对结果排序**
   - X 用 vector 收集祖先后忘记 sort → 输出顺序不对
   - O 用 set 自动排序，或者最后对每个 vector 排序

2. **祖先集合传递不完整**
   - X 拓扑排序时只 `ancestors[v].insert(u)` 而忘记传递 `ancestors[u]` → 只有直接父节点
   - O 必须传递 `ancestors[u]` 的全部内容再加上 u 自身

3. **正向 BFS 时把自己加入祖先**
   - X 从 u 出发 BFS 时 `ancestorSets[u].insert(u)` → 自己成了自己的祖先
   - O BFS 到 next 时才 insert u，不要在起点 u 处 insert

4. **拓扑排序的入度初始化错误**
   - X 入度数组初始值非 0 或漏计某些边
   - O 遍历所有边 `indegree[to]++`

5. **visited 数组生命周期**
   - X 正向 BFS 时共用一个 visited → 第二个节点的 BFS 看不到已被标记的节点
   - O 每次 BFS 用独立的 visited 数组

## 面试追问

**Q1: 两种方法哪个更快？**
→ 理论时间复杂度相同 O(n*(n+e))。但拓扑排序避免了从每个节点重新遍历整个图，利用了"祖先集合的传递性"累积结果，实际中当图比较密集时拓扑排序常数更小。但集合合并操作（set insert range）本身也有 O(n) 开销，所以差异不大。

**Q2: 如果 n 很大（比如 10^5），这些方法还能用吗？**
→ 最坏情况下祖先集合总大小为 O(n^2)，n=10^5 时约 10^10——空间和时间都不可行。但题目约束 n<=1000，e<=2000，O(n^2) 完全没问题。如果 n 很大，需要考虑位图压缩（bitset）来存储祖先集合，空间 O(n^2/64)。

**Q3: 能用 bitset 优化吗？**
→ 可以。用 `bitset<N>` 代替 `set<int>` 存储祖先集合，合并操作变成 O(n/64) 的位运算 OR。总时间 O(n*e/64 + n^2/64)，空间 O(n^2/64)，大幅降低常数。

**Q4: 题目说是 DAG，如果不是 DAG 怎么办？**
→ 如果有环，拓扑排序会失败（队列清空后仍有节点未处理）。正向 BFS 和反向 DFS 仍然可以工作，但"祖先"的定义需要重新明确（环内节点互为祖先）。可以先用 Tarjan 算法缩点，将强连通分量缩成单个节点，得到 DAG 后再应用本题方法。

## 相关题型

- **LeetCode 207 课程表** — 拓扑排序基础题，判断 DAG 是否有环。本题是在拓扑排序基础上增加了集合传递。
- **LeetCode 210 课程表 II** — 输出拓扑排序序列，本题需要在拓扑排序过程中做更多事情（传递祖先）。
- **LeetCode 1462 课程表 IV** — 几乎相同的问题！判断 (u,v) 是否为祖先关系，本质是 DAG 可达性传递闭包。
- **LeetCode 802 找到最终的安全状态** — DAG 上的反向可达性分析，思路类似。
