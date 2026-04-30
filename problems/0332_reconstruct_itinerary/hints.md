# 332. 重新安排行程

## 核心思路

这道题本质上是在一个**有向图**中找一条**欧拉路径（Eulerian Path）**——从 JFK 出发，恰好经过每条边（机票）一次，且在有多种选择时按字典序最小走。

## 思维链

1. **读完题第一反应**：这是一个图的遍历问题。每个机场是节点，每张机票是一条有向边。要求从 JFK 出发，走完所有边恰好一次。这就是经典的欧拉路径问题。

2. **最朴素的想法——回溯暴力搜索**：从 JFK 开始 DFS，每次选一张没用过的机票走，走到用完所有机票就是一个合法行程。为了字典序最小，先对目的地排序，找到第一个合法行程就返回。但回溯最坏情况下可能探索大量分支。

3. **暴力回溯的瓶颈**：当存在"死胡同"时，回溯需要撤销选择、尝试下一个目的地，时间复杂度可能很高。**核心问题是**：贪心地总选字典序最小的目的地可能走进死胡同（还没用完所有机票就无路可走了）。

4. **突破瓶颈——Hierholzer 算法**：这是找欧拉路径/回路的经典 O(E) 算法。核心洞察：**当你走进死胡同时，说明这个节点应该放在路径的末尾**。具体做法是 DFS 时每走一条边就删除它，当一个节点没有出边时就把它加入结果（逆序记录），最后把结果翻转。

5. **如何保证字典序最小**：对每个节点的邻接表按字典序排序（或用 `multiset`/最小堆），这样每次贪心选字典序最小的邻居。配合 Hierholzer 的"后序插入 + 翻转"策略，自然得到字典序最小的欧拉路径。

6. **为什么 Hierholzer 是对的**：直觉理解——如果从某个节点出发贪心走最小字典序的边走到了死胡同，说明这个节点之后不需要再经过其他地方了，所以它应该排在行程靠后的位置。把它"后插"到结果中，回溯后继续处理其他分支。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 回溯搜索 | DFS + 回溯，找第一个合法行程 | O(E!) 最坏 | O(E) | 能说出即可 |
| Hierholzer (multiset) | 欧拉路径经典算法，后序插入 + 翻转 | O(E log E) | O(E) | ⭐ 必须写出 |
| Hierholzer (priority_queue) | 同上，用最小堆替代 multiset | O(E log E) | O(E) | 加分项 |

## 关键提示

- **提示1**：把这道题建模成图。机场是节点，机票是有向边。问题变成：从 JFK 出发，走完所有边恰好一次。这是什么经典问题？

- **提示2**：如果你贪心地每次选字典序最小的目的地，可能走进死胡同（示例2就是这种情况）。想想：走进死胡同意味着什么？这个死胡同节点应该在行程的什么位置？

- **提示3**：Hierholzer 算法的核心操作只有两步：(1) 每走一条边就删掉它；(2) 当节点无出边时加入结果。最后翻转结果。

- **提示4（ASCII 图示——示例2 的图）**：
```
    JFK ──ATL──→ SFO
     │    ↑ ↙      
     └──→ ATL       
     │               
     └──→ SFO ──→ ATL

  邻接表 (排序后):
  JFK: [ATL, SFO]
  ATL: [JFK, SFO]
  SFO: [ATL]
```

- **提示5**：Hierholzer 在示例2上的执行过程：
```
  DFS(JFK) → 选ATL → DFS(ATL) → 选JFK → DFS(JFK) → 选SFO 
  → DFS(SFO) → 选ATL → DFS(ATL) → 选SFO → DFS(SFO) → 无出边, 加入结果[SFO]
  回溯ATL → 无出边, 加入[SFO, ATL]
  回溯SFO → 无出边, 加入[SFO, ATL, SFO]
  回溯JFK → 无出边, 加入[SFO, ATL, SFO, JFK]
  回溯ATL → 无出边, 加入[SFO, ATL, SFO, JFK, ATL]
  回溯JFK → 无出边, 加入[SFO, ATL, SFO, JFK, ATL, JFK]
  翻转 → [JFK, ATL, JFK, SFO, ATL, SFO] ✓
```

## 解法详解

### 解法1: 回溯搜索 — O(E!) / O(E)

**思考过程**：最直觉的方法。建图后从 JFK 开始 DFS，每次选一张没用过的机票（按字典序排好的），递归走下去。如果所有票都用完了，就找到了答案。如果走到死胡同还没用完，就回溯撤销这张票，尝试下一个目的地。

```cpp
class Solution {
public:
    vector<string> findItinerary(vector<vector<string>>& tickets) {
        // 建邻接表：每个出发地 → 按字典序排列的目的地列表
        unordered_map<string, vector<string>> graph;
        for (auto& t : tickets) {
            graph[t[0]].push_back(t[1]);
        }
        for (auto& [from, tos] : graph) {
            sort(tos.begin(), tos.end());
        }
        
        int n = tickets.size();
        vector<string> route = {"JFK"};
        // 记录每条边（邻接表中的每个位置）是否已使用
        unordered_map<string, vector<bool>> used;
        for (auto& [from, tos] : graph) {
            used[from].assign(tos.size(), false);
        }
        
        // 回溯：找到第一个合法行程立刻返回
        function<bool()> backtrack = [&]() -> bool {
            if ((int)route.size() == n + 1) return true; // 用完所有票
            
            string& curr = route.back();
            auto& neighbors = graph[curr];
            for (int i = 0; i < (int)neighbors.size(); i++) {
                if (used[curr][i]) continue;
                used[curr][i] = true;
                route.push_back(neighbors[i]);
                if (backtrack()) return true; // 找到就立刻返回
                route.pop_back();
                used[curr][i] = false;
            }
            return false;
        };
        
        backtrack();
        return route;
    }
};
```

**关键点**：
- 邻接表排序后，第一个找到的合法行程就是字典序最小的（因为我们总是先尝试字典序小的目的地）。
- 最坏情况（所有票的出发地相同）时间复杂度极高，但题目数据规模 ≤ 300，实际通常能过。

---

### 解法2: Hierholzer 算法 (multiset) — O(E log E) / O(E) ⭐ 面试首选

**从解法1优化**：解法1的瓶颈是回溯——走进死胡同后要撤销、重试。Hierholzer 算法巧妙地避免了回溯：**走进死胡同的节点直接放到结果末尾**，然后回溯继续处理剩余的边。最终翻转结果即可。

**为什么这样是对的？** 如果从某个节点出发没有出边了，说明在最终路径中，这个节点后面不会再有任何节点了——它就是路径的终点。把它放到结果的末尾（后序位置），然后回退处理前面的分支。所有节点都会被恰好在正确的时机加入结果。

```cpp
class Solution {
public:
    vector<string> findItinerary(vector<vector<string>>& tickets) {
        // 用 multiset 存邻接表，天然有序 + 支持删除
        unordered_map<string, multiset<string>> graph;
        for (auto& t : tickets) {
            graph[t[0]].insert(t[1]);
        }
        
        vector<string> result;
        
        // Hierholzer: DFS + 后序记录
        function<void(const string&)> dfs = [&](const string& node) {
            while (!graph[node].empty()) {
                // 取字典序最小的目的地
                string next = *graph[node].begin();
                // 删除这条边（走过就不能再走）
                graph[node].erase(graph[node].begin());
                dfs(next);
            }
            // 没有出边了 → 加入结果（后序位置）
            result.push_back(node);
        };
        
        dfs("JFK");
        
        // 后序记录是反的，翻转得到正序
        reverse(result.begin(), result.end());
        return result;
    }
};
```

**关键点**：
- `multiset` 自动排序 + 允许重复，`erase(begin())` 就是取最小并删除，O(log N)。
- 后序加入 + 翻转是 Hierholzer 算法的精髓，不要在前序位置加入（那样会得到错误结果）。

---

### 解法3: Hierholzer 算法 (priority_queue) — O(E log E) / O(E)

**和解法2 的区别**：用最小堆（`priority_queue`）代替 `multiset`。从堆顶 pop 就是取字典序最小的目的地。逻辑完全一样。

```cpp
class Solution {
public:
    vector<string> findItinerary(vector<vector<string>>& tickets) {
        // 用最小堆存邻接表
        unordered_map<string, priority_queue<string, vector<string>, greater<string>>> graph;
        for (auto& t : tickets) {
            graph[t[0]].push(t[1]);
        }
        
        vector<string> result;
        
        function<void(const string&)> dfs = [&](const string& node) {
            while (!graph[node].empty()) {
                string next = graph[node].top();
                graph[node].pop();
                dfs(next);
            }
            result.push_back(node);
        };
        
        dfs("JFK");
        reverse(result.begin(), result.end());
        return result;
    }
};
```

---

### 解法2b: Hierholzer 算法（迭代版，用栈模拟递归）

**为什么需要迭代版**：递归深度最大为边数+1（300+1），一般不会栈溢出。但面试官可能追问迭代写法，而且迭代版也展示了对 Hierholzer 的深入理解。

```cpp
class Solution {
public:
    vector<string> findItinerary(vector<vector<string>>& tickets) {
        unordered_map<string, multiset<string>> graph;
        for (auto& t : tickets) {
            graph[t[0]].insert(t[1]);
        }
        
        vector<string> result;
        stack<string> stk;
        stk.push("JFK");
        
        while (!stk.empty()) {
            string curr = stk.top();
            if (!graph[curr].empty()) {
                // 还有出边：取最小的，压栈继续走
                string next = *graph[curr].begin();
                graph[curr].erase(graph[curr].begin());
                stk.push(next);
            } else {
                // 死胡同：加入结果，回退
                result.push_back(curr);
                stk.pop();
            }
        }
        
        reverse(result.begin(), result.end());
        return result;
    }
};
```

## 解法对比

| | 回溯搜索 | Hierholzer (multiset) | Hierholzer (priority_queue) | Hierholzer (迭代) |
|---|---|---|---|---|
| 时间 | O(E!) 最坏 | O(E log E) | O(E log E) | O(E log E) |
| 空间 | O(E) | O(E) | O(E) | O(E) |
| 核心区别 | 暴力尝试所有可能 | 利用欧拉路径性质避免回溯 | 同左，数据结构不同 | 同左，用栈替代递归 |
| 适用场景 | 理解题意 | **面试首选** | 代码略短 | 怕栈溢出时 |

**关键区别**：回溯是"试错型"——走错了回头重来；Hierholzer 是"不会走错"——走到死胡同就直接放末尾，绝不回溯。这个质的飞跃来自对欧拉路径性质的理解。

## 易错点

1. **✗ 前序加入结果而非后序**
   ```cpp
   // 错误：在 DFS 开头就加入
   result.push_back(node);  // 然后继续 DFS
   ```
   **✓ 应该在节点无出边时（DFS 结尾）才加入**，最后翻转。前序加入会在遇到分支时把路径搞乱。

2. **✗ 用 `set` 而非 `multiset`**
   ```cpp
   unordered_map<string, set<string>> graph; // 错！同一条航线可能有多张票
   ```
   **✓ 用 `multiset`**，因为 `["JFK","ATL"]` 可能出现多次，`set` 会去重。

3. **✗ 忘记删除已走过的边**
   ```cpp
   // 错误：遍历但不删除
   for (auto& next : graph[node]) { dfs(next); }
   ```
   **✓ 走一条边必须立刻删除它**（`erase(begin())`），否则会重复走。

4. **✗ 迭代版中忘记在有出边时不 pop 栈顶**
   ```cpp
   // 错误：先 pop 再处理
   stk.pop();
   // 应该只在死胡同时才 pop
   ```
   **✓ 有出边时只压入下一个节点，不弹出当前节点**；死胡同时才弹出并加入结果。

5. **✗ 忘记 reverse**
   Hierholzer 的结果是后序（逆序），最后必须翻转。

## 面试追问

**Q1: 这道题的图论模型是什么？为什么保证有解？**
→ 这是有向图的欧拉路径问题。题目保证所有机票至少存在一种合理行程，即图是连通的且满足欧拉路径存在条件：最多一个节点出度比入度多1（起点 JFK），最多一个节点入度比出度多1（终点），其余节点出度=入度。

**Q2: 为什么贪心选字典序最小 + 后序插入就能得到字典序最小的行程？**
→ 因为我们总是优先走字典序最小的边。如果走到死胡同，说明当前节点的"后续路径"比其他分支的"后续路径"短（或为空），它应该被安排在行程靠后的位置。后序插入恰好实现了这一点。直觉：字典序小的选择优先执行，只有当它们导致死胡同时才被"推迟"到末尾。

**Q3: 如果不保证有解（可能无法用完所有票），怎么修改？**
→ 需要先检查欧拉路径/回路的存在性条件：
- 图必须弱连通（忽略方向后连通）
- 对于欧拉路径：恰好一个节点 out-in=1（起点），恰好一个 in-out=1（终点），其余 in=out
- 对于欧拉回路：所有节点 in=out
- 如果不满足，返回空。

**Q4: 迭代版和递归版有什么本质区别？**
→ 没有本质区别。迭代版用显式栈模拟递归调用栈。区别在于：递归版可能栈溢出（本题 ≤300 不会），迭代版需要自己管理"什么时候压栈、什么时候弹出"，写法需要注意——只在死胡同时才弹出并加入结果。

## 相关题型

- **753. 破解保险箱 (Cracking the Safe)** — 同样是欧拉路径/回路问题，复用 Hierholzer 算法框架。区别：节点是 n-1 位密码，边是 n 位密码，需要 De Bruijn 序列。
- **207/210. 课程表 I/II (Course Schedule)** — 同为有向图问题，但是拓扑排序（DAG），不是欧拉路径。复用"建邻接表 + DFS/BFS"的图建模技巧。
- **1584. 连接所有点的最小费用** — 图上的另一类经典问题（MST），对比学习图算法的不同应用场景。