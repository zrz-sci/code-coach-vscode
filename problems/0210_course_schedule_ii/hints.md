# 210. 课程表 II

## 核心思路

这道题本质上是**有向图的拓扑排序**：给定课程之间的先修关系（有向边），找出一个合法的学习顺序，使得每门课的先修课都排在它前面；如果存在环（循环依赖），则无法完成所有课程。

## 思维链

1. **读完题第一反应**：课程之间有先后依赖，这就是一个有向图。"先修课必须在前面" → 这不就是拓扑排序吗？
2. **什么是拓扑排序？** 把有向无环图(DAG)的所有节点排成线性序列，使得每条有向边 `u→v` 中，`u` 都排在 `v` 前面。如果图有环，拓扑排序不存在。
3. **怎么实现拓扑排序？** 两种经典方法：
   - **BFS (Kahn 算法)**：找入度为 0 的节点（没有先修课的课程），先学它们，然后"删掉"它们，更新其他节点的入度，重复此过程。
   - **DFS (后序反转)**：对每个节点做 DFS，当一个节点的所有后续节点都访问完毕后，把它加入结果（后序）。最终反转结果就是拓扑排序。
4. **怎么判断有环？**
   - BFS：如果最终排序的节点数 < numCourses，说明有环（有些节点入度始终不为 0）。
   - DFS：如果在递归过程中访问到"正在访问中"的节点，说明存在环。
5. **与 207 题（课程表 I）的关系**：207 题只需要判断能否完成（是否有环），本题需要输出具体的顺序。代码上只多了一个"收集结果"的步骤。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| BFS (Kahn算法) | 入度表 + 队列，逐层剥离入度为0的节点 | O(V+E) | O(V+E) | ⭐ 必须写出 |
| DFS (后序反转) | 深度优先搜索，后序加入结果，最后反转 | O(V+E) | O(V+E) | ⭐ 必须写出 |

> V = numCourses (节点数)，E = prerequisites.length (边数)

## 关键提示

1. **建图方向**：`prerequisites[i] = [a, b]` 表示 `b→a`（先学 b 再学 a），所以邻接表中 `graph[b].push_back(a)`。注意不要建反了！

2. **BFS 入度的含义**：入度 = 还有多少先修课没学。入度为 0 = 可以学了。每次出队一门课，把它指向的课程入度减 1。

3. **DFS 三色标记法**：
   - 白色(0)：未访问
   - 灰色(1)：正在访问（在当前递归栈中）
   - 黑色(2)：已完成
   - 如果 DFS 遇到灰色节点 → 有环！

4. **ASCII 示意图 — 示例2的依赖图**：
   ```
   prerequisites = [[1,0],[2,0],[3,1],[3,2]]
   
   有向图:          入度:
     0 → 1 → 3      0: 入度0 ← 起点!
     |       ↑       1: 入度1
     └→ 2 ───┘       2: 入度1
                      3: 入度2
   
   BFS 过程:
   Step1: 入度0的入队 → queue=[0]
   Step2: 弹出0, result=[0], 1的入度→0, 2的入度→0 → queue=[1,2]
   Step3: 弹出1, result=[0,1], 3的入度→1 → queue=[2]
   Step4: 弹出2, result=[0,1,2], 3的入度→0 → queue=[3]
   Step5: 弹出3, result=[0,1,2,3] → 完成!
   ```

5. **与课程表 I (207题) 的区别**：207 题返回 `bool`，本题返回 `vector<int>`。代码改动：把 BFS 中弹出的节点依次加入结果数组，最后检查长度。

## 解法详解

### 解法1: BFS (Kahn算法) — O(V+E) / O(V+E) ⭐ 面试首选

**思考过程**：
- 哪些课可以先学？没有先修课的（入度为 0）。
- 学完这些课之后，又有哪些课的先修课全部满足了？入度变为 0 的。
- 这就是一个"逐层剥洋葱"的过程，天然适合 BFS。
- 如果最后有课程始终无法被剥到（入度始终 > 0），说明存在循环依赖。

```cpp
class Solution {
public:
    vector<int> findOrder(int numCourses, vector<vector<int>>& prerequisites) {
        // 1. 建图 + 统计入度
        vector<vector<int>> graph(numCourses);
        vector<int> indegree(numCourses, 0);
        for (auto& p : prerequisites) {
            // p[1] → p[0]：先学 p[1] 再学 p[0]
            graph[p[1]].push_back(p[0]);
            indegree[p[0]]++;
        }
        
        // 2. 把所有入度为 0 的节点入队（没有先修课，可以直接学）
        queue<int> q;
        for (int i = 0; i < numCourses; i++) {
            if (indegree[i] == 0) q.push(i);
        }
        
        // 3. BFS：每次取出一门可学的课，更新后续课程的入度
        vector<int> result;
        while (!q.empty()) {
            int course = q.front();
            q.pop();
            result.push_back(course);  // 这门课可以学了，加入结果
            
            for (int next : graph[course]) {
                indegree[next]--;  // "学完 course"等于去掉一条边
                if (indegree[next] == 0) {
                    q.push(next);  // 先修课全部学完了，可以学了
                }
            }
        }
        
        // 4. 检查是否所有课都能学完（有环则不能）
        if ((int)result.size() != numCourses) return {};
        return result;
    }
};
```

**关键点**：
- 入度数组是 Kahn 算法的核心数据结构，它记录每个节点"还差几个前驱没处理"。
- 最终 `result.size() == numCourses` 是判断有无环的关键条件。

---

### 解法2: DFS (后序反转) — O(V+E) / O(V+E) ⭐ 面试必知

**思考过程**：
- 拓扑排序的另一种思路：如果我对一个节点做 DFS，当它的所有"后继"都已经处理完毕时，把它加入结果。
- 这样得到的是"反向拓扑序"（后序遍历），最后反转一下就是正确的拓扑排序。
- 为什么？因为后序遍历保证：一个节点在它所有依赖的节点之后被记录，反转后就变成在所有依赖之前。
- 判环：用三色标记，遇到灰色（当前路径上的祖先）说明有环。

```cpp
class Solution {
public:
    vector<int> findOrder(int numCourses, vector<vector<int>>& prerequisites) {
        // 1. 建图
        vector<vector<int>> graph(numCourses);
        for (auto& p : prerequisites) {
            graph[p[1]].push_back(p[0]);
        }
        
        // 三色标记：0=未访问, 1=访问中(灰), 2=已完成(黑)
        vector<int> color(numCourses, 0);
        vector<int> result;
        bool hasCycle = false;
        
        // 2. 对每个未访问的节点发起 DFS
        for (int i = 0; i < numCourses && !hasCycle; i++) {
            if (color[i] == 0) {
                dfs(graph, i, color, result, hasCycle);
            }
        }
        
        if (hasCycle) return {};
        
        // 3. 后序遍历的结果需要反转
        reverse(result.begin(), result.end());
        return result;
    }
    
private:
    void dfs(vector<vector<int>>& graph, int node,
             vector<int>& color, vector<int>& result, bool& hasCycle) {
        if (hasCycle) return;
        
        color[node] = 1;  // 标记为"访问中"
        
        for (int next : graph[node]) {
            if (color[next] == 1) {
                // 遇到灰色节点 → 存在环!
                hasCycle = true;
                return;
            }
            if (color[next] == 0) {
                dfs(graph, next, color, result, hasCycle);
            }
            // color[next] == 2: 已完成，跳过
        }
        
        color[node] = 2;  // 标记为"已完成"
        result.push_back(node);  // 后序：所有后继都处理完了，再加自己
    }
};
```

**关键点**：
- `result.push_back(node)` 在 DFS **返回前**执行（后序位置），这保证了依赖关系的正确性。
- 三色标记是 DFS 判环的标准技巧：灰色表示"在当前递归栈中"，再次遇到灰色意味着走了一个回路。

---

## 解法对比

| | BFS (Kahn) | DFS (后序反转) |
|---|---|---|
| **直觉性** | 更直觉："先学没有前提的课" | 需要理解后序 + 反转 |
| **判环方式** | 最终计数 != numCourses | 三色标记遇到灰色节点 |
| **代码量** | 略少 | 需要递归 + 颜色数组 |
| **面试推荐** | ⭐ 首选，逻辑清晰好解释 | ⭐ 追问时展示 DFS 功底 |
| **扩展性** | 容易改成按层输出/并行调度 | 容易改成求最长路径等 |

**建议**：面试中先写 BFS 版本（逻辑最清晰），如果面试官追问"还有别的方法吗？"，再说 DFS 后序反转。

## 易错点

1. **建图方向搞反**：
   - ✗ `graph[p[0]].push_back(p[1])` — 这是反向图，会得到错误的拓扑序
   - ✓ `graph[p[1]].push_back(p[0])` — `[a, b]` 表示 b→a，b 是 a 的前提

2. **DFS 忘记反转结果**：
   - ✗ 直接返回后序遍历结果 → 得到的是反向拓扑序
   - ✓ `reverse(result.begin(), result.end())` 或者用栈/从后往前填充

3. **DFS 只用两色（visited bool数组）判环**：
   - ✗ 用 `visited[node] = true` — 无法区分"正在访问中"和"已完成"，会误判有环
   - ✓ 必须用三色标记：0(未访问) / 1(访问中) / 2(已完成)

4. **忘记处理孤立节点**：
   - 没有任何先修关系的课程，入度为 0，BFS 中会直接入队
   - DFS 中通过外层 for 循环保证每个节点都会被访问

5. **BFS 中忘记判断环**：
   - ✗ 直接返回 result，不检查长度
   - ✓ `if (result.size() != numCourses) return {};`

## 面试追问

**Q1: 这题和 207 题（课程表 I）有什么区别？代码改动大吗？**
> 207 只需返回 `true/false`（是否有环），本题需要输出具体顺序。代码改动极小：BFS 版本只需多一行 `result.push_back(course)`，最后返回 `result` 而不是 `result.size() == numCourses`。

**Q2: 如果要求输出所有合法的拓扑序，怎么做？**
> 用回溯法：每次从所有入度为 0 的节点中选一个（而不是随便出队一个），选完后更新入度，递归生成下一个位置。回溯时恢复入度。时间复杂度可以是指数级（合法拓扑序的数量本身可以是指数级）。

**Q3: 如果课程有并行度限制（每学期最多学 k 门课），最少需要几个学期？**
> 这就是 LeetCode 1136 (Parallel Courses)。用 BFS 分层，每层最多取 k 个入度为 0 的节点，层数就是学期数。如果 k 足够大，层数就是 DAG 的最长路径长度 + 1。

**Q4: BFS 和 DFS 哪个更适合面试？**
> BFS (Kahn) 更适合面试首选，因为：不需要递归、逻辑直觉（"先学没有前提的课"）、判环条件简单（计数）。DFS 作为第二种方法展示全面性。

## 相关题型

- **207. 课程表** — 本题的简化版，只需判断是否有环（返回 bool），不需要输出顺序。代码上删掉 result 数组，改成计数器即可。
- **269. 火星词典 (Alien Dictionary)** — 复用拓扑排序模板，难点在于从字符串比较中建图（提取字符间的先后关系）。
- **310. 最小高度树 (Minimum Height Trees)** — 复用 BFS 逐层剥离的思想，但从叶子节点（度为1）开始剥，而不是入度为0的节点。
- **1136. 并行课程 (Parallel Courses)** — 在本题基础上加分层约束，BFS 天然按层处理，每层代表一个学期。
- **329. 矩阵中的最长递增路径** — 可以建模为 DAG 求最长路径，用拓扑排序 + DP 求解。