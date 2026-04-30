# 851. 喧闹和富有

## 核心思路

本质是一个**有向图上的信息传播**问题：richer 关系构成 DAG（有向无环图），对每个节点需要找到其所有"比自己富"的祖先节点中（包含自身）quietness 最小的那个人。用 **DFS + 记忆化**或**拓扑排序**从富人向穷人传播最安静人的信息。

## 思维链

1. **读完题第一反应** → richer[i] = [a, b] 表示 a 比 b 有钱。要对每个人 x 找出所有比 x 富（或等富）的人中最安静的那个。
2. **这是图问题吗？** → 是。把"a 比 b 富"建边 a → b（或 b → a，取决于遍历方向）。richer 关系无环（题目保证逻辑一致），所以是 DAG。
3. **建图方向怎么选？** → 建 b → a 的反向边（穷人指向富人）。从 x 出发 DFS 遍历所有比 x 富的人，找最安静的。但这样每个节点都 DFS 一次太慢。
4. **优化？** → 记忆化！如果已经算过某个人的答案，直接复用。或者反过来：建 a → b（富人指向穷人），用拓扑排序从最富的人开始，逐层向下传播"当前最安静人"。
5. **两种等价做法**：DFS 记忆化（自顶向下）和 拓扑排序 BFS（自底向上/从入度为0出发）。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| DFS + 记忆化 | 从每个人出发 DFS 找比自己富的所有人 | O(V+E) | O(V+E) | ⭐ 推荐 |
| 拓扑排序 BFS | 从最富的人开始逐层传播最安静人信息 | O(V+E) | O(V+E) | ⭐ 推荐 |
| 暴力 BFS/DFS（无记忆化） | 对每个人独立搜索 | O(V*(V+E)) | O(V+E) | 不推荐 |

## 关键提示

1. **建图方向决定遍历方式**：
   - 建 `richer → poorer`（a → b）：从入度为0（最富）的节点拓扑排序向下传播。
   - 建 `poorer → richer`（b → a）：从每个节点 DFS 向上搜索所有更富的人。
2. **DFS 记忆化的关键**：`answer[x] != -1` 表示已算过，直接返回。由于图是 DAG，不会有环导致死循环。
3. **拓扑排序传播逻辑**：处理节点 u 时，对其所有出边 u → v（u 比 v 富），如果 `quiet[answer[u]] < quiet[answer[v]]`，更新 `answer[v] = answer[u]`。
4. **初始化**：`answer[i] = i`（每个人初始认为自己是"比自己富的人中最安静的"）。
5. **quiet 数组的含义**：`quiet[i]` 是第 i 个人的安静程度，值越小越安静。所有值唯一。

## 解法详解

### 解法1: DFS + 记忆化 — O(V+E) / O(V+E) ⭐

**思考过程**: 对每个人 x，DFS 找出所有比 x 富的人（通过反向边），取 quiet 最小的。记忆化避免重复计算。

```
示例: richer=[[1,0],[2,1],[3,1],[3,7],[4,3],[5,3],[6,3]]
      quiet=[3,2,5,4,6,1,7,0]

建图 (poorer → richer): 0→1, 1→2, 1→3, 7→3, 3→4, 3→5, 3→6

DFS(0): → DFS(1) → DFS(2): 叶子, answer[2]=2
                  → DFS(3) → DFS(4): 叶子, answer[4]=4
                            → DFS(5): 叶子, answer[5]=5
                            → DFS(6): 叶子, answer[6]=6
                    answer[3]: min quiet among {3,4,5,6} → quiet={4,6,1,7} → person 5
                    answer[3]=5
         answer[1]: min quiet among {1,2,3的答案} → quiet={2,5,1} → person 5
         answer[1]=5
  answer[0]: min quiet among {0, 1的答案} → quiet={3, 1} → person 5
  answer[0]=5 ✓
```

```cpp
class Solution {
public:
    vector<int> loudAndRich(vector<vector<int>>& richer, vector<int>& quiet) {
        int n = quiet.size();
        vector<vector<int>> graph(n); // poorer → richer
        for (auto& r : richer)
            graph[r[1]].push_back(r[0]); // b → a (a比b富)

        vector<int> answer(n, -1);
        for (int i = 0; i < n; i++)
            dfs(i, graph, quiet, answer);
        return answer;
    }

    int dfs(int u, vector<vector<int>>& graph,
            vector<int>& quiet, vector<int>& answer) {
        if (answer[u] != -1) return answer[u];
        answer[u] = u; // 初始：自己
        for (int v : graph[u]) {
            int candidate = dfs(v, graph, quiet, answer);
            if (quiet[candidate] < quiet[answer[u]])
                answer[u] = candidate;
        }
        return answer[u];
    }
};
```

---

### 解法2: 拓扑排序 BFS — O(V+E) / O(V+E)

**不同视角**: 建 `richer → poorer` 的图，从最富的人（入度为0）开始 BFS，逐层向下传播最安静人信息。

```cpp
class Solution {
public:
    vector<int> loudAndRich(vector<vector<int>>& richer, vector<int>& quiet) {
        int n = quiet.size();
        vector<vector<int>> graph(n); // richer → poorer
        vector<int> indegree(n, 0);
        for (auto& r : richer) {
            graph[r[0]].push_back(r[1]); // a → b
            indegree[r[1]]++;
        }
        vector<int> answer(n);
        iota(answer.begin(), answer.end(), 0); // answer[i] = i

        queue<int> q;
        for (int i = 0; i < n; i++)
            if (indegree[i] == 0) q.push(i);

        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : graph[u]) {
                if (quiet[answer[u]] < quiet[answer[v]])
                    answer[v] = answer[u]; // 传播更安静的人
                if (--indegree[v] == 0) q.push(v);
            }
        }
        return answer;
    }
};
```

## 易错点

1. **建图方向搞反**：
   - 错: `richer[i]=[a,b]` 建成 `a→b` 后用 DFS → 搜到的是比自己穷的人
   - 对: DFS 用反向图 `b→a`（穷→富），拓扑排序用正向图 `a→b`（富→穷）

2. **记忆化标记搞混**：
   - 错: `answer[u] = 0` 作为未访问标记 → 人0会被误认为已访问
   - 对: `answer[u] = -1` 作为未访问标记

3. **比较 quiet 时用人的编号而非 quiet 值**：
   - 错: `if (candidate < answer[u])` → 比较的是人的编号
   - 对: `if (quiet[candidate] < quiet[answer[u]])` → 比较安静程度

4. **忘记初始化 answer[i] = i**：
   - "比自己富或等富的人"包含自己，所以初始答案是自己

## 面试追问

**Q1: 为什么图一定是 DAG？**
> 题目说"richer 关系逻辑一致"，意味着不会出现 a 比 b 富且 b 比 a 富。财富是严格偏序关系，所以无环。

**Q2: DFS 记忆化和拓扑排序哪个更好？**
> 复杂度相同，都是 O(V+E)。DFS 记忆化代码更短；拓扑排序更显式地展示了信息流方向，面试中更容易讲清楚。

**Q3: 如果 richer 关系不是 DAG（有环），怎么办？**
> 需要先用 Tarjan 或 Kosaraju 求强连通分量，缩点后变成 DAG，同一 SCC 内的人互相等富，取 SCC 内最安静的人。

**Q4: 时间复杂度为什么是 O(V+E) 而不是 O(V^2)？**
> 每个节点只被 DFS 访问一次（记忆化后直接返回），每条边也只被遍历一次。总工作量 = 节点数 + 边数 = O(V+E)。

## 相关题型

- **210. Course Schedule II** — 拓扑排序的经典题，同样建图 + BFS/DFS 处理 DAG。
- **802. Find Eventual Safe States** — 在有向图中找安全节点，需要反向图 + 拓扑排序。
- **1203. Sort Items by Groups Respecting Dependencies** — 双层拓扑排序，更复杂的 DAG 信息传播。
- **207. Course Schedule** — 判断 DAG 是否存在环，拓扑排序基础。
