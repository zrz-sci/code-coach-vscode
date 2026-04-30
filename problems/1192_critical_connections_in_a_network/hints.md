# 1192. 查找集群内的关键连接

## 核心思路

本题本质上是在无向连通图中找**桥（Bridge）**——删除后会使图不连通的边。这是图论中经典的 Tarjan 算法应用。

## 思维链

1. **读完题第一反应**：对每条边，尝试删除它，然后用 BFS/DFS 检查图是否仍然连通。如果不连通，这条边就是关键连接（桥）。
2. **暴力解的瓶颈**：有 E 条边，每次删边后做一次 DFS/BFS 检查连通性需要 O(V+E)，总共 O(E×(V+E))。当 n 和 E 都达到 10^5 时，这就是 ~10^10，完全超时。
3. **突破瓶颈的关键观察**：一条边 (u, v) 是桥，当且仅当删除它后 v 那一侧的子树无法通过**其他路径（回边）**回到 u 或 u 的祖先。换句话说，如果 v 的子树中存在一条回边能连回 u 或 u 上方，那 (u,v) 就不是桥，因为有"备份路径"。
4. **Tarjan 算法的核心**：对图做一次 DFS，维护两个数组：`disc[u]`（发现时间戳）和 `low[u]`（u 及其子树能通过回边到达的最小时间戳）。如果 `low[v] > disc[u]`，说明 v 的子树完全无法回到 u 或更上方，(u,v) 就是桥。
5. **一次 DFS 搞定所有边**：不需要逐条边删除再检查，Tarjan 算法通过巧妙维护 low 值，在单次 O(V+E) 的 DFS 中找出所有桥。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力法 | 逐条删边 + BFS/DFS 检查连通性 | O(E×(V+E)) | O(V+E) | 能说出即可 |
| Tarjan 算法 | 一次 DFS 维护 disc/low 找桥 | O(V+E) | O(V+E) | ⭐ 必须写出 |

## 关键提示

1. **什么是桥？** 一条边 (u,v) 是桥 ⟺ 删除后图的连通分量数增加。等价地，在 DFS 树中，(u,v) 是桥 ⟺ v 的子树中没有任何回边能连到 u 或 u 的祖先。

2. **DFS 树 vs 原图**：对无向图做 DFS，所有边要么是"树边"（DFS 探索时走的边），要么是"回边"（连向已访问的祖先的边）。无向图不存在"前向边"或"交叉边"。

3. **disc 和 low 的含义**：
   ```
   disc[u] = u 被首次发现的时间戳
   low[u]  = u 及其子树中，通过回边能到达的最小 disc 值
   ```

4. **桥的判定条件**：对于树边 (u → v)，如果 `low[v] > disc[u]`，则 (u,v) 是桥。注意是严格大于！如果 `low[v] == disc[u]`，说明 v 的子树有回边恰好连到 u，那删除 (u,v) 后 v 仍可达 u，不是桥。

5. **图示理解**：
   ```
   示例: n=4, connections=[[0,1],[1,2],[2,0],[1,3]]
   
   原图:            DFS 树 (从0开始):
   0 --- 1          0 (disc=0)
   |   / |          |
   | /   |          1 (disc=1)
   2     3         / \
                  2    3 (disc=3)
                (disc=2)
                  |
                  回边: 2→0 (使 low[2]=0, low[1]=0)
   
   分析:
   - low[1]=0 (子树中2有回边到0)，low[1]=0 <= disc[0]=0 → 边(0,1)不是桥
   - low[2]=0 (2有回边到0)，low[2]=0 <= disc[1]=1 → 边(1,2)不是桥  
   - low[3]=3 (3没有回边)，low[3]=3 > disc[1]=1  → 边(1,3)是桥 ✓
   ```

## 解法详解

### 解法1: 暴力法 — O(E×(V+E)) / O(V+E)

**思考过程**: 最直接的想法——要判断一条边是不是桥，删掉它看图还连不连通就行。对每条边都这样试一遍。

```cpp
class Solution {
public:
    vector<vector<int>> criticalConnections(int n, vector<vector<int>>& connections) {
        vector<vector<int>> result;
        
        for (int i = 0; i < connections.size(); i++) {
            // 构建邻接表，但跳过第 i 条边
            vector<vector<int>> adj(n);
            for (int j = 0; j < connections.size(); j++) {
                if (j == i) continue;  // 删除第 i 条边
                adj[connections[j][0]].push_back(connections[j][1]);
                adj[connections[j][1]].push_back(connections[j][0]);
            }
            
            // BFS/DFS 检查连通性
            vector<bool> visited(n, false);
            queue<int> q;
            q.push(0);
            visited[0] = true;
            int count = 1;
            while (!q.empty()) {
                int u = q.front(); q.pop();
                for (int v : adj[u]) {
                    if (!visited[v]) {
                        visited[v] = true;
                        count++;
                        q.push(v);
                    }
                }
            }
            
            // 如果不能访问所有节点，说明这条边是桥
            if (count < n) {
                result.push_back(connections[i]);
            }
        }
        return result;
    }
};
```

**关键点**: 这个解法逻辑正确但会 TLE。每次都重建邻接表 + 做 BFS，n=10^5 时完全不可行。但它帮助我们理解"桥"的定义。

---

### 解法2: Tarjan 算法 — O(V+E) / O(V+E) ⭐ 面试首选

**从解法1优化**: 暴力法的瓶颈是"对每条边都做一次全图遍历"。Tarjan 的天才之处在于：只做一次 DFS，通过维护额外信息（disc 和 low 数组），就能判断每条边是否是桥。

**核心思想详解**：

```
DFS 树中的边分类（无向图）：
- 树边 (Tree Edge): DFS 探索时走的边
- 回边 (Back Edge): 连向已访问的祖先的边（无向图只有这两种）

一条树边 (u→v) 是桥的条件：
  v 的子树中没有任何回边能到达 u 或 u 的祖先
  即 low[v] > disc[u]

low[v] 的更新规则：
  初始: low[v] = disc[v]
  对 v 的每个邻居 w:
    - w 未访问 (树边): DFS(w), 然后 low[v] = min(low[v], low[w])
    - w 已访问且 w≠parent (回边): low[v] = min(low[v], disc[w])
```

**完整的 DFS 过程图示**：

```
示例: 0-1, 1-2, 2-0, 1-3，从节点0开始DFS

Step1: 访问0, disc[0]=0, low[0]=0
       邻居: 1(未访问), 2(未访问)
       先走 0→1

Step2: 访问1, disc[1]=1, low[1]=1, parent=0
       邻居: 0(parent跳过), 2(未访问), 3(未访问)
       先走 1→2

Step3: 访问2, disc[2]=2, low[2]=2, parent=1
       邻居: 0(已访问,非parent→回边!), 1(parent跳过)
       回边 2→0: low[2] = min(low[2], disc[0]) = min(2, 0) = 0
       返回到节点1

Step4: 回到1, low[1] = min(low[1], low[2]) = min(1, 0) = 0
       检查边(1,2): low[2]=0 <= disc[1]=1 → 不是桥 ✓
       继续走 1→3

Step5: 访问3, disc[3]=3, low[3]=3, parent=1
       邻居: 1(parent跳过)
       无其他邻居，返回到节点1

Step6: 回到1, low[1] = min(low[1], low[3]) = min(0, 3) = 0
       检查边(1,3): low[3]=3 > disc[1]=1 → 是桥! ✓
       返回到节点0

Step7: 回到0, low[0] = min(low[0], low[1]) = min(0, 0) = 0
       检查边(0,1): low[1]=0 <= disc[0]=0 → 不是桥 ✓

最终结果: [[1,3]]
```

```cpp
class Solution {
public:
    vector<vector<int>> criticalConnections(int n, vector<vector<int>>& connections) {
        // 建邻接表
        vector<vector<int>> adj(n);
        for (auto& conn : connections) {
            adj[conn[0]].push_back(conn[1]);
            adj[conn[1]].push_back(conn[0]);
        }
        
        vector<int> disc(n, -1);  // -1 表示未访问
        vector<int> low(n, 0);
        int timer = 0;
        vector<vector<int>> bridges;
        
        // 题目保证图连通，从节点0开始DFS即可覆盖全图
        // 用 lambda + 引用捕获，避免传太多参数
        function<void(int, int)> dfs = [&](int u, int parent) {
            disc[u] = low[u] = timer++;  // 初始时 low = disc
            
            for (int v : adj[u]) {
                if (v == parent) continue;  // 跳过来的那条边（避免走回头路）
                
                if (disc[v] == -1) {
                    // v 未访问 → 树边，递归探索
                    dfs(v, u);
                    // 回溯时，用子节点的 low 更新自己
                    low[u] = min(low[u], low[v]);
                    // 判断桥：v 的子树完全无法回到 u 或更上方
                    if (low[v] > disc[u]) {
                        bridges.push_back({u, v});
                    }
                } else {
                    // v 已访问 → 回边，用 v 的 disc 更新自己的 low
                    // 为什么用 disc[v] 而不是 low[v]？
                    // 两种都可以找桥，但用 disc[v] 是经典写法
                    // （用 low[v] 也正确，因为 low[v] <= disc[v]）
                    low[u] = min(low[u], disc[v]);
                }
            }
        };
        
        dfs(0, -1);
        return bridges;
    }
};
```

**关键点**:
- `low[v] > disc[u]` 是**严格大于**，因为等于意味着 v 子树有回边恰好到 u 本身，删除 (u,v) 后仍可达
- 跳过 parent 是为了避免把树边当回边处理（无向图中 u→v 和 v→u 是同一条边）

---

### 解法2变体: 迭代版 Tarjan（避免栈溢出）— O(V+E) / O(V+E)

**为什么需要迭代版**: 当 n=10^5 且图是一条链时，递归深度达 10^5，可能导致栈溢出。迭代版用显式栈模拟。

```cpp
class Solution {
public:
    vector<vector<int>> criticalConnections(int n, vector<vector<int>>& connections) {
        vector<vector<int>> adj(n);
        for (auto& conn : connections) {
            adj[conn[0]].push_back(conn[1]);
            adj[conn[1]].push_back(conn[0]);
        }
        
        vector<int> disc(n, -1), low(n, 0);
        vector<int> parent(n, -1);
        // idx[u] 记录 u 当前遍历到邻居列表的哪个位置
        vector<int> idx(n, 0);
        int timer = 0;
        vector<vector<int>> bridges;
        
        // 显式栈模拟 DFS
        stack<int> stk;
        stk.push(0);
        disc[0] = low[0] = timer++;
        
        while (!stk.empty()) {
            int u = stk.top();
            
            if (idx[u] < (int)adj[u].size()) {
                int v = adj[u][idx[u]++];  // 取下一个邻居
                
                if (v == parent[u]) continue;  // 跳过父边
                
                if (disc[v] == -1) {
                    // 树边：v 未访问
                    disc[v] = low[v] = timer++;
                    parent[v] = u;
                    stk.push(v);
                } else {
                    // 回边：v 已访问
                    low[u] = min(low[u], disc[v]);
                }
            } else {
                // u 的所有邻居都处理完了，回溯
                stk.pop();
                if (parent[u] != -1) {
                    // 等价于递归版的 "dfs(v,u) 返回后"
                    low[parent[u]] = min(low[parent[u]], low[u]);
                    if (low[u] > disc[parent[u]]) {
                        bridges.push_back({parent[u], u});
                    }
                }
            }
        }
        
        return bridges;
    }
};
```

## 解法对比

| | 暴力法 | Tarjan 递归版 | Tarjan 迭代版 |
|---|---|---|---|
| 时间 | O(E×(V+E)) | O(V+E) | O(V+E) |
| 空间 | O(V+E) | O(V+E) + 递归栈 | O(V+E) |
| 优势 | 易理解 | 代码简洁 | 不会栈溢出 |
| 劣势 | 严重超时 | 深图可能爆栈 | 代码较长 |
| 面试选择 | 口述思路 | ⭐ **首选** | 追问时提及 |

## 易错点

1. **跳过 parent 的处理不完整（重边情况）**：
   - ✗ `if (v == parent) continue;` — 如果图有重边（如两条 0→1 的边），这会把两条都跳过
   - ✓ 本题明确说"不存在重复连接"，所以简单跳过 parent 是安全的
   - 如果有重边，需要用边的编号来区分，只跳过来时那条具体的边

2. **low 更新时用 disc[v] 还是 low[v]**：
   - 对于回边 (u, v)，`low[u] = min(low[u], disc[v])` 和 `low[u] = min(low[u], low[v])` 在找**桥**时都正确
   - 但如果要找**割点（articulation point）**，只能用 `disc[v]`！所以养成用 `disc[v]` 的习惯更安全

3. **桥判定用 > 而非 >=**：
   - ✗ `if (low[v] >= disc[u])` — 这是割点的判定条件（对根节点还需额外处理）
   - ✓ `if (low[v] > disc[u])` — 桥的判定条件，严格大于

4. **忘记初始化 disc 为 -1**：
   - ✗ `vector<int> disc(n, 0)` — 节点0的 disc 就是0，无法区分"未访问"和"第一个访问"
   - ✓ `vector<int> disc(n, -1)` — 用 -1 表示未访问

## 面试追问

**Q1: 什么是桥？暴力法怎么做？复杂度多少？**
> 桥是删除后使图不连通的边。暴力法：逐条删边，做一次 BFS/DFS 检查连通性。O(E×(V+E))。

**Q2: Tarjan 算法的 low 值是什么含义？为什么 low[v] > disc[u] 说明 (u,v) 是桥？**
> low[v] 是 v 及其子树中，通过回边能到达的最小发现时间。如果 low[v] > disc[u]，说明 v 的子树中没有任何回边能到达 u 或 u 的祖先，删掉 (u,v) 后 v 的子树就和上面断开了。

**Q3: 如果要找割点（删除后图不连通的点）呢？和找桥有什么区别？**
> 割点判定：对非根节点 u，如果存在子节点 v 使得 `low[v] >= disc[u]`（注意是 >=）；对根节点，如果在 DFS 树中有 2 个以上子节点。区别：桥用 `>`，割点用 `>=`，且根要特判。

**Q4: 如果图有重边（两个节点之间有多条边），代码需要怎么改？**
> 不能简单用 `v == parent` 跳过，因为可能跳过了不该跳过的平行边。需要用边的编号来区分：只跳过"来时走的那条边"，不跳过平行边。可以用 `pair<int,int>` 邻接表存 (邻居, 边编号)。

**Q5: n 很大时递归会爆栈，怎么处理？**
> 用显式栈模拟 DFS（迭代版 Tarjan），用 idx 数组记录每个节点处理到邻居列表的哪个位置，在栈弹出时做回溯更新。

## 相关题型

- **LeetCode 207/210 课程表** — 同样是图的 DFS，但用于检测有向图环和拓扑排序。复用"邻接表 + DFS 遍历"框架。
- **LeetCode 323 无向图中连通分量的数目** — 更基础的图连通性问题，可作为本题的前置练习。复用邻接表 + DFS/BFS/并查集。
- **LeetCode 685 冗余连接 II** — 有向图中找删除后使图成为树的边。涉及并查集 + 边分类。
- **割点（Articulation Point）** — Tarjan 算法的姊妹问题，判定条件从 `low[v] > disc[u]` 变为 `low[v] >= disc[u]`（非根）或 DFS 子节点数 ≥ 2（根）。