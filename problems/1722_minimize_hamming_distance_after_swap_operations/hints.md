# 1722. 执行交换操作后的最小汉明距离

## 核心思路

交换操作具有**传递性**——如果 `a` 能和 `b` 交换，`b` 能和 `c` 交换，那么 `a`、`b`、`c` 三个位置的元素可以任意排列。本质上就是找出所有互相可达的下标组（连通分量），在每个组内贪心匹配 `source` 和 `target`。

## 思维链

1. **读完题第一反应**：交换可以做任意多次、任意顺序。如果两个下标直接有交换关系，它们的元素可以互换。但如果 0↔1、1↔2，那么 0、1、2 三个位置的元素能不能任意排列？——可以！因为通过多次交换，任何排列都能达成（类似冒泡排序的原理）。

2. **关键洞察**：交换关系是**传递**的。把下标看作图的节点，每个 `allowedSwap` 看作一条边，那么**同一个连通分量内的所有下标，其 `source` 值可以任意重排**。

3. **问题转化**：对每个连通分量，我们能把 `source` 中这些位置的值自由分配。为了最小化汉明距离，我们要让尽可能多的位置匹配 `target`。即：在每个连通分量内，数一下 `source` 和 `target` 有多少值是相同的（可以配对的），配对数越多，汉明距离越小。

4. **怎么找连通分量**？两种经典方法：
   - **并查集（Union-Find）**：边建边合并，最后查询每个节点所属集合
   - **DFS/BFS**：建邻接表，遍历找连通分量

5. **怎么计算每个分量的贡献**？对于分量内的下标集合，统计 `source` 值的多重集和 `target` 值的多重集，能配对的数量就是"不贡献汉明距离"的数量，剩下的就是贡献。

6. **最终答案** = `n` - 所有连通分量中能配对的总数。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力模拟 | 真的去做交换，尝试所有排列 | 指数级 | O(n) | 能说出即可 |
| Union-Find + 哈希计数 | 并查集找连通分量，哈希表统计匹配 | O(n·α(n)) ≈ O(n) | O(n) | ⭐ 必须写出 |
| DFS + 哈希计数 | DFS找连通分量，哈希表统计匹配 | O(n + E) | O(n + E) | ⭐ 必须写出 |

## 关键提示

1. **传递性是核心**：不要被"交换"操作迷惑，多次交换 = 同一连通分量内任意排列。想想冒泡排序——只要元素在同一个连通块里，你总能通过若干次相邻交换达成任意排列。

2. **图论建模**：下标是节点，`allowedSwaps` 是边。问题变成：在每个连通分量内，`source` 和 `target` 最多能匹配多少个？

3. **多重集匹配**：注意 `source` 和 `target` 中可能有重复值！不能用 set，要用 multiset 或 hashmap 计数。例如 source 分量内有 `[1,1,2]`，target 有 `[1,2,2]`，能匹配 `1` 和 `2` 各一个，匹配数 = 2。

4. **并查集 vs DFS**：两者都能找连通分量。并查集代码更短、思路更直接；DFS 需要建邻接表但不需要额外的数据结构知识。面试中推荐并查集。

5. **计算匹配数的技巧**：对于每个连通分量，用 hashmap 统计 source 值出现次数，然后遍历 target 值做消减。每成功消减一次就是一次匹配。

```
示例1可视化:
下标:     0   1   2   3
source:  [1,  2,  3,  4]
target:  [2,  1,  4,  5]

allowedSwaps: [0,1], [2,3]

连通分量:
  分量A: {0, 1}  source值={1,2}  target值={2,1}  → 完美匹配, 贡献0
  分量B: {2, 3}  source值={3,4}  target值={4,5}  → 只能匹配4, 贡献1

汉明距离 = 0 + 1 = 1
```

## 解法详解

### 解法1: Union-Find + 哈希计数 — O(n·α(n)) / O(n) ⭐ 面试首选

**思考过程**：交换操作的传递性 → 连通分量 → 并查集是找连通分量的利器。找到分量后，用 hashmap 对每个分量的 source 值计数，再用 target 值去匹配消减。

```cpp
class Solution {
public:
    vector<int> parent, rank_;
    
    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]); // 路径压缩
        return parent[x];
    }
    
    void unite(int a, int b) {
        int ra = find(a), rb = find(b);
        if (ra == rb) return;
        // 按秩合并
        if (rank_[ra] < rank_[rb]) swap(ra, rb);
        parent[rb] = ra;
        if (rank_[ra] == rank_[rb]) rank_[ra]++;
    }
    
    int minimumHammingDistance(vector<int>& source, vector<int>& target, 
                               vector<vector<int>>& allowedSwaps) {
        int n = source.size();
        parent.resize(n);
        rank_.resize(n, 0);
        for (int i = 0; i < n; i++) parent[i] = i;
        
        // 1. 建立并查集
        for (auto& sw : allowedSwaps) {
            unite(sw[0], sw[1]);
        }
        
        // 2. 按连通分量分组，统计每个分量中 source 值的出现次数
        unordered_map<int, unordered_map<int, int>> groupCount;
        // groupCount[root][value] = count
        for (int i = 0; i < n; i++) {
            groupCount[find(i)][source[i]]++;
        }
        
        // 3. 用 target 值去消减，每消减一次 = 一次成功匹配
        int match = 0;
        for (int i = 0; i < n; i++) {
            int root = find(i);
            if (groupCount[root][target[i]] > 0) {
                groupCount[root][target[i]]--;
                match++;
            }
        }
        
        return n - match;
    }
};
```

**关键点**：
- 路径压缩和按秩合并让并查集几乎 O(1) 每次操作
- `groupCount[root]` 用 root 节点作为分量的代表，确保同一分量的节点聚在一起
- 遍历 target 做消减时，顺序无所谓（同一分量内的值是一个池子）

### 解法2: DFS + 哈希计数 — O(n + E) / O(n + E)

**从另一个角度**：不用并查集，直接建图跑 DFS 找连通分量。对每个分量收集 source 值和 target 值，用 hashmap 匹配。

```cpp
class Solution {
public:
    int minimumHammingDistance(vector<int>& source, vector<int>& target, 
                               vector<vector<int>>& allowedSwaps) {
        int n = source.size();
        
        // 1. 建邻接表
        vector<vector<int>> adj(n);
        for (auto& sw : allowedSwaps) {
            adj[sw[0]].push_back(sw[1]);
            adj[sw[1]].push_back(sw[0]);
        }
        
        // 2. DFS 找连通分量
        vector<bool> visited(n, false);
        int hamming = 0;
        
        for (int i = 0; i < n; i++) {
            if (visited[i]) continue;
            
            // 收集当前连通分量的所有下标
            vector<int> component;
            // DFS 用栈
            stack<int> stk;
            stk.push(i);
            visited[i] = true;
            while (!stk.empty()) {
                int node = stk.top(); stk.pop();
                component.push_back(node);
                for (int nei : adj[node]) {
                    if (!visited[nei]) {
                        visited[nei] = true;
                        stk.push(nei);
                    }
                }
            }
            
            // 3. 统计这个分量中 source 值的频率
            unordered_map<int, int> srcCount;
            for (int idx : component) {
                srcCount[source[idx]]++;
            }
            
            // 4. 用 target 值消减，未匹配的就是汉明贡献
            int unmatched = 0;
            for (int idx : component) {
                if (srcCount[target[idx]] > 0) {
                    srcCount[target[idx]]--;
                } else {
                    unmatched++;
                }
            }
            hamming += unmatched;
        }
        
        return hamming;
    }
};
```

**关键点**：
- DFS 方式更直观——遍历到的节点就是同一个分量
- 不需要全局的 groupCount，每个分量独立处理，内存更清晰
- 时间 O(n + E) 其中 E = allowedSwaps.size()

## 解法对比

| | Union-Find | DFS |
|---|---|---|
| 时间 | O(n·α(n)) ≈ O(n) | O(n + E) |
| 空间 | O(n)（不需要邻接表） | O(n + E)（需要邻接表） |
| 代码量 | 需要写并查集模板 | 标准 DFS |
| 面试推荐 | ⭐ 更简洁，展示数据结构功底 | 也完全可以，更直观 |
| 关键区别 | 边处理完后按 root 分组 | 直接在遍历时收集分量 |

**选择建议**：如果你并查集模板很熟，用 Union-Find 更简洁；如果对图遍历更熟悉，用 DFS 一样好。面试中两种都可以，但并查集是这类"连通分量 + 分组操作"问题的标配思路。

## 易错点

1. **重复值处理**：
   - ✗ 用 `unordered_set` 存 source 值（丢失重复信息）
   - ✓ 用 `unordered_map<int,int>` 计数，因为同一个值可能出现多次
   - 例：source 分量 = {1,1,2}，target 分量 = {1,2,2}，set 会认为完全匹配，实际只匹配 2 个

2. **并查集忘记路径压缩**：
   - ✗ `return parent[x] == x ? x : find(parent[x]);`（没有更新 parent[x]）
   - ✓ `return parent[x] == x ? x : parent[x] = find(parent[x]);`
   - 不压缩会导致 find 退化为 O(n)，整体 O(n²)

3. **分组时用 `i` 而不是 `find(i)` 做 key**：
   - ✗ `groupCount[i][source[i]]++`
   - ✓ `groupCount[find(i)][source[i]]++`
   - 每个节点的代表是 `find(i)` 而不是 `i` 本身

4. **DFS 解法中忘记双向建边**：
   - ✗ 只加 `adj[a].push_back(b)`
   - ✓ 还要加 `adj[b].push_back(a)`，因为交换是双向的

## 面试追问

**Q1: 为什么同一连通分量内的元素可以任意排列？**
→ 因为在一个连通图上，任何排列都可以通过一系列相邻交换达成。这和冒泡排序的原理一样——只要两个位置之间存在一条"交换路径"，元素就可以在路径上逐步移动到目标位置。数学上，连通图上的相邻对换可以生成所有置换。

**Q2: 如果 allowedSwaps 非常多（接近 n²），有没有优化空间？**
→ 当边数很多时，大部分节点很快就连通了。并查集的按秩合并 + 路径压缩已经接近 O(1) 每次操作，不需要特别优化。但如果所有节点都在一个连通分量中，可以提前 break——直接对整个数组做一次全局匹配。

**Q3: 如果不是"交换"而是"单向移动"（只能把 a 的值移到 b，不能反过来），怎么做？**
→ 这就变成了**有向图**问题。不能简单用连通分量了，需要用**强连通分量（SCC）**。在一个 SCC 内部的值仍然可以任意排列，但不同 SCC 之间需要考虑拓扑顺序。问题会复杂很多。

**Q4: 时间复杂度的瓶颈在哪里？**
→ 并查集操作几乎 O(1)，瓶颈在 hashmap 的操作。总共 n 次插入 + n 次查询，每次 O(1) 期望，总 O(n)。如果用 `map`（有序）则是 O(n log n)。

## 相关题型

- **547. 省份数量** — 同样是找连通分量数，直接复用并查集/DFS 模板，但只需要计数分量个数，不需要分组操作
- **684. 冗余连接** — 并查集的另一个经典应用：在建边过程中检测环，复用同一个 Union-Find 模板
- **839. 相似字符串组** — 同样是"传递性 → 连通分量"的思路，区别在于边的定义不是直接给出，而是需要判断两个字符串是否相似
- **1202. 交换字符串中的元素** — 几乎是同一道题！区别是操作对象从两个数组变成了一个字符串内部排序，连通分量内的字符要排序后重新放回使字典序最小