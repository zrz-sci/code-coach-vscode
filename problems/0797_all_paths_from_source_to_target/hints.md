# 797. 所有可能的路径

## 核心思路

在一个 **DAG (有向无环图)** 中，从节点 0 到节点 n-1 找出所有路径。因为图无环，不需要 visited 数组，直接用 **DFS/回溯** 枚举所有路径即可。这是最经典的图上回溯问题之一。

## 思维链

1. **读完题第一反应**：从 0 出发，找到所有能到达 n-1 的路径。"所有路径" → 回溯/DFS 枚举。

2. **为什么不用 BFS？** BFS 适合求"最短路径"，而本题要"所有路径"。DFS 天然适合路径枚举——走到头再回退，尝试其他分支。

3. **DAG 的关键特性**：无环意味着 DFS 不会死循环，所以不需要 visited 数组。每条边只会被遍历一次（在一条路径内不会重复经过同一节点）。

4. **回溯模板**：维护一个 `path` 数组，DFS 进入时 push，DFS 退出时 pop。当到达 n-1 时将当前 path 的副本加入结果。

5. **复杂度分析**：最坏情况下（完全图），从 0 到 n-1 的路径数可达 2^(n-2) 条（每个中间节点选择"经过"或"不经过"），每条路径最长 n 个节点。所以时间复杂度 O(2^n * n)，但 n <= 15，完全可接受。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| DFS/回溯 | 从 0 出发，递归枚举所有路径 | O(2^n * n) | O(n) 栈深 | 必须写出 |
| BFS | 用队列存储完整路径 | O(2^n * n) | O(2^n * n) | 不推荐 |

> 注：n = 节点数，最大 15

## 关键提示

1. **提示1**：这是一个 DAG——你不需要 visited 数组！想想为什么？（因为无环，DFS 不会死循环。）

2. **提示2**：回溯的核心三步：(1) 选择——把下一个节点加入 path；(2) 递归——继续 DFS；(3) 撤销——从 path 中移除（pop_back）。

3. **提示3**：递归终止条件是什么？当前节点 == n-1 时，把 path 的副本加入结果集。注意不需要 return 后再继续——因为 n-1 是终点节点，graph[n-1] = []，自然不会有后续邻居。

4. **提示4**：初始化 path 时，节点 0 就应该已经在 path 里。别忘了把起点加进去！

5. **提示5**：n <= 15 意味着什么？最多 2^13 = 8192 条路径，这个量级的暴力枚举完全没问题。

```
DFS 路径枚举示意（graph = [[1,2],[3],[3],[]]）：

图结构:
  0 ──→ 1 ──→ 3
  |           ↑
  └──→ 2 ────┘

DFS 过程（path 变化）：

dfs(0)
  path = [0]
  │
  ├─ dfs(1)                    ← 选择 0→1
  │   path = [0, 1]
  │   │
  │   └─ dfs(3)                ← 选择 1→3
  │       path = [0, 1, 3]
  │       node == n-1，收集路径！ result = [[0,1,3]]
  │       返回，path 回退到 [0, 1]
  │   返回，path 回退到 [0]
  │
  └─ dfs(2)                    ← 选择 0→2
      path = [0, 2]
      │
      └─ dfs(3)                ← 选择 2→3
          path = [0, 2, 3]
          node == n-1，收集路径！ result = [[0,1,3],[0,2,3]]
          返回，path 回退到 [0, 2]
      返回，path 回退到 [0]

最终结果: [[0,1,3], [0,2,3]]
```

## 解法详解

### 解法1: DFS/回溯 — O(2^n * n) / O(n)

**思考过程**：
- "所有路径" → DFS 枚举，不是求最短路径
- DAG 无环 → 不需要 visited
- 维护一个 path 数组做回溯
- 到达 n-1 就收集当前路径

```
// 回溯模板:
//
// dfs(node):
//   if node == target:
//       收集 path 的副本
//       return
//   for next in neighbors(node):
//       path.push(next)       ← 选择
//       dfs(next)             ← 递归
//       path.pop()            ← 撤销选择
```

```cpp
class Solution {
public:
    vector<vector<int>> allPathsSourceTarget(vector<vector<int>>& graph) {
        vector<vector<int>> result;
        vector<int> path = {0};  // 起点 0 先放入路径
        dfs(graph, 0, path, result);
        return result;
    }

private:
    void dfs(vector<vector<int>>& graph, int node, vector<int>& path,
             vector<vector<int>>& result) {
        // 到达终点，收集路径
        if (node == (int)graph.size() - 1) {
            result.push_back(path);  // 拷贝当前路径
            return;
        }

        // 遍历所有邻居
        for (int next : graph[node]) {
            path.push_back(next);        // 选择
            dfs(graph, next, path, result); // 递归
            path.pop_back();             // 撤销选择（回溯）
        }
    }
};
```

**关键点**：
- `path = {0}`：起点 0 在初始化时就放入 path
- `result.push_back(path)`：注意这里是拷贝，不是引用
- `path.pop_back()`：回溯的核心——探索完一个分支后恢复状态

---

### 解法2: BFS（不推荐，仅作对比）

**思考过程**：BFS 也可以枚举路径，但需要在队列中存储完整路径（而不是单个节点），空间开销大得多。

```cpp
class Solution {
public:
    vector<vector<int>> allPathsSourceTarget(vector<vector<int>>& graph) {
        vector<vector<int>> result;
        int n = graph.size();
        queue<vector<int>> q;
        q.push({0});  // 初始路径只有节点 0

        while (!q.empty()) {
            vector<int> path = q.front();
            q.pop();
            int node = path.back();  // 路径的最后一个节点

            if (node == n - 1) {
                result.push_back(path);  // 到达终点
                continue;
            }

            for (int next : graph[node]) {
                vector<int> newPath = path;  // 拷贝路径（代价高！）
                newPath.push_back(next);
                q.push(newPath);
            }
        }
        return result;
    }
};
```

**为什么不推荐**：
- 每次入队都要**拷贝整条路径**，时间和空间浪费巨大
- DFS 回溯只需 O(n) 额外空间（一条路径），BFS 需要同时存储所有"正在进行中"的路径

## 解法对比

| | DFS/回溯 | BFS |
|---|---|---|
| **空间** | O(n) 只存一条路径 | O(2^n * n) 同时存所有路径 |
| **实现** | 简洁，标准回溯模板 | 需要拷贝路径，代码冗长 |
| **适用** | 枚举所有路径的首选 | 求最短路径时才用 |
| **面试** | 首选 | 不推荐 |

## 易错点

1. **忘记初始化 path 包含节点 0**
   ```cpp
   // 错误: vector<int> path; dfs(0, ...);  → 结果中缺少起点 0
   // 正确: vector<int> path = {0};
   ```

2. **不需要 visited 但加了 visited**
   ```cpp
   // 不必要: visited[node] = true; ... visited[node] = false;
   // DAG 无环，天然不会重复访问
   // 但注意：如果题目说"有向图"而没说"无环"，就需要 visited！
   ```

3. **result.push_back(path) 拷贝问题**
   ```cpp
   // 注意: path 是引用，push_back 时会自动拷贝
   // 如果用 result.push_back(&path) 或存指针，后续回溯会破坏数据
   ```

4. **终止条件放错位置**
   ```cpp
   // 错误: 在 for 循环内检查 next == n-1
   //   这样如果 0 本身就是终点（n=1），会漏掉
   // 正确: 在 dfs 函数开头检查 node == n-1
   ```

## 面试追问

**Q1: 为什么不需要 visited 数组？**
→ 因为题目保证图是 DAG（有向无环图）。DFS 中不可能回到已经访问过的节点，所以不存在死循环风险。如果图可能有环，就必须加 visited。

**Q2: 时间复杂度怎么分析？**
→ 一个 n 节点的 DAG，从 0 到 n-1 最多有 2^(n-2) 条路径（考虑中间的 n-2 个节点，每个可以选择在路径中或不在）。每条路径长度最多 n。所以最坏情况是 O(2^n * n)。

**Q3: 如果图有环怎么办？**
→ 需要加 visited 数组。在 DFS 进入时标记，退出时取消标记（回溯式 visited），这样同一条路径上不会重复走同一个节点，但不同路径可以经过同一个节点。

**Q4: 如果只需要路径数量而不需要具体路径？**
→ 可以用 DP 或记忆化搜索。定义 `dp[i]` = 从节点 i 到 n-1 的路径数。转移：`dp[i] = sum(dp[j]) for j in graph[i]`，基础情况 `dp[n-1] = 1`。时间 O(V+E)，空间 O(V)。

## 相关题型

- **113. 路径总和 II** — 同样是 DFS 回溯收集所有路径，只是在二叉树上而非 DAG。复用"path push/pop + 终止条件收集"的模板。
- **78. 子集** — 回溯枚举所有子集，核心的"选择→递归→撤销"模板完全一致。
- **39. 组合总和** — 回溯枚举所有组合，同样的回溯框架，区别在终止条件和剪枝策略。
- **210. 课程表 II** — 同样是 DAG 上的问题，但求的是拓扑排序而非所有路径。
