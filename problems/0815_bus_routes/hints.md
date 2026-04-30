# 815. 公交路线

## 核心思路

这道题本质上是一个**最短路径问题**，但不是在"车站"层面做 BFS，而是在**"公交线路"层面**做 BFS —— 每坐一趟公交车算"一步"，求从 source 到 target 的最少步数。

## 思维链

1. **读完题第一反应**：从 source 出发，能坐哪些公交车？坐了之后能到哪些站？从那些站又能换乘哪些车？→ 这是一个典型的 BFS 逐层扩展问题。

2. **直觉做法 — 站点级 BFS**：把每个车站看成图的节点，同一条公交线路上的站两两连边，然后从 source 做 BFS 到 target。但这样边数可能是 O(Σroute_length²)，站点编号高达 10⁶，图太大了。

3. **瓶颈在哪**：站点级 BFS 的问题是图太稠密（同一条线路上所有站两两连通），而且我们要最少的"换乘次数"，不是最少的"经过站数"。

4. **关键转换 — 线路级 BFS**：既然求的是最少乘坐的公交车数量，不如把**公交线路**当作图的节点。两条线路如果共享至少一个站点，它们之间就有边（可以换乘）。从包含 source 的所有线路出发 BFS，找到包含 target 的线路就停。

5. **建图方式**：用哈希表 `stop → [route_indices]` 记录每个站被哪些线路经过。两条线路通过共享站点建立邻接关系。BFS 时按线路层级扩展，每扩一层代表多坐一趟车。

6. **优化**：BFS 过程中标记已访问的线路（避免重复坐），也标记已访问的站点（一个站被处理过后，通过它能换乘的线路都已入队）。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 站点级 BFS（暴力） | 同一线路站点两两连边，BFS | O(Σlen²) | O(Σlen²) | 能说出即可 |
| 线路级 BFS | 线路为节点，共享站点即有边 | O(N² × L) | O(N² + S) | ⭐ 必须写出 |
| 站点 BFS + 线路扩展 | 站点队列，但按线路批量扩展 | O(N × L) | O(N × L) | ⭐ 面试首选 |

> N = routes.length (≤500), L = 单条线路平均长度, S = 总站点数 (≤10⁵)

## 关键提示

1. **建模转换**：这道题不是求"最少经过几个站"，而是"最少坐几趟车"。想想什么应该作为 BFS 的"一层"？

2. **哈希表预处理**：`stop_to_routes[stop] = {route0, route2, ...}` —— 知道每个站能上哪些车，是换乘的关键。

3. **BFS 的起点不是一个**：source 可能被多条线路经过，所有经过 source 的线路都是 BFS 的起始层（距离=1）。

4. **去重的对象**：需要标记**已访问的线路**（防止重复坐同一趟车），也可以标记**已访问的站点**（该站能换乘的线路都已处理）。

5. **特殊情况**：`source == target` 时直接返回 0，不需要坐任何车。

```
示意图 (示例1):
routes = [[1,2,7], [3,6,7]], source=1, target=6

站点-线路映射:
  站1 → {线路0}
  站2 → {线路0}
  站7 → {线路0, 线路1}  ← 换乘点！
  站3 → {线路1}
  站6 → {线路1}

BFS (线路级):
  Layer 0: source=1
  Layer 1: 线路0 (经过站1) → 解锁站 {1,2,7}
  Layer 2: 线路1 (经过站7，与线路0共享) → 解锁站 {3,6,7}
           发现 target=6 在线路1中 → 返回 2
```

## 解法详解

### 解法1: 站点级 BFS（暴力思路）— 理解问题

**思考过程**：最直觉的想法 —— 把每个站当节点，同一条线路上的站互相连通。然后从 source BFS 到 target，BFS 的层数就是经过的站数。但我们要的是"乘坐公交车数量"，不是站数，所以需要额外记录换乘信息。

**问题**：同一条线路上的站两两连边，边数可达 O(Σlen²)。如果一条线路有 10⁵ 个站，光建图就 O(10¹⁰)，完全不可行。站点编号高达 10⁶，数组也开不了那么大（用 map 会更慢）。

**结论**：这个思路帮我们理解问题，但不可行。必须换个角度建图。

### 解法2: 线路级 BFS — O(N² × L) / O(N² + S)

**从解法1优化**：既然求的是"最少坐几趟车"，不如直接以**线路**为节点做 BFS。两条线路如果共享站点就连边。先预处理线路间的邻接关系，再 BFS。

```cpp
class Solution {
public:
    int numBusesToDestination(vector<vector<int>>& routes, int source, int target) {
        if (source == target) return 0;
        
        int n = routes.size();
        
        // 预处理：对每条线路的站点排序，方便后续判断两条线路是否共享站点
        for (auto& r : routes) sort(r.begin(), r.end());
        
        // 建立线路之间的邻接表：如果两条线路共享至少一个站点，则连边
        vector<vector<int>> graph(n);
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                if (hasCommonStop(routes[i], routes[j])) {
                    graph[i].push_back(j);
                    graph[j].push_back(i);
                }
            }
        }
        
        // 找出包含 source 和 target 的线路
        unordered_set<int> sourceRoutes, targetRoutes;
        for (int i = 0; i < n; i++) {
            if (binary_search(routes[i].begin(), routes[i].end(), source))
                sourceRoutes.insert(i);
            if (binary_search(routes[i].begin(), routes[i].end(), target))
                targetRoutes.insert(i);
        }
        
        // BFS: 从所有包含 source 的线路开始
        queue<int> q;
        vector<bool> visited(n, false);
        for (int r : sourceRoutes) {
            // 如果同一条线路同时包含 source 和 target，只需坐1趟
            if (targetRoutes.count(r)) return 1;
            q.push(r);
            visited[r] = true;
        }
        
        int buses = 1;
        while (!q.empty()) {
            buses++;
            int sz = q.size();
            while (sz--) {
                int cur = q.front(); q.pop();
                for (int next : graph[cur]) {
                    if (visited[next]) continue;
                    if (targetRoutes.count(next)) return buses;
                    visited[next] = true;
                    q.push(next);
                }
            }
        }
        return -1;
    }
    
private:
    // 判断两个已排序数组是否有公共元素（双指针 O(L1+L2)）
    bool hasCommonStop(const vector<int>& a, const vector<int>& b) {
        int i = 0, j = 0;
        while (i < a.size() && j < b.size()) {
            if (a[i] == b[j]) return true;
            else if (a[i] < b[j]) i++;
            else j++;
        }
        return false;
    }
};
```

**复杂度分析**：
- 建图 O(N² × L)：N 条线路两两比较，每次比较 O(L)
- BFS O(N²)：最多 N 个节点，N² 条边
- 空间 O(N² + S)

**关键点**：当 N 较小 (≤500) 但站点很多时，这种方法很好。但当 N 较大时，O(N²) 建图成为瓶颈。

### 解法3: 站点 BFS + 线路扩展 — O(N × L) / O(N × L) ⭐ 面试首选

**思考过程**：解法2 的瓶颈在于 O(N²) 的线路间建图。能不能跳过显式建图？

关键观察：站点是线路之间的"桥梁"。我们用 `stop → routes` 的映射，BFS 时：
- 从当前线路的所有站点出发
- 对每个站点，找到经过该站的所有其他线路
- 这些线路就是可以换乘的下一趟车

这等价于在站点和线路之间做"双层 BFS"：外层遍历线路，内层遍历站点，通过站点发现新线路。

```cpp
class Solution {
public:
    int numBusesToDestination(vector<vector<int>>& routes, int source, int target) {
        if (source == target) return 0;
        
        int n = routes.size();
        
        // step1: 建立 站点 → 线路列表 的映射
        unordered_map<int, vector<int>> stopToRoutes;
        for (int i = 0; i < n; i++) {
            for (int stop : routes[i]) {
                stopToRoutes[stop].push_back(i);
            }
        }
        
        // step2: BFS，队列里放的是线路编号
        // visited_route[i] 标记线路 i 是否已入队
        // visited_stop 标记站点是否已处理（避免重复通过同一个站找线路）
        vector<bool> visitedRoute(n, false);
        unordered_set<int> visitedStop;
        queue<int> q;
        
        // 将 source 所在的所有线路入队（第1趟车）
        for (int routeIdx : stopToRoutes[source]) {
            q.push(routeIdx);
            visitedRoute[routeIdx] = true;
        }
        visitedStop.insert(source);
        
        int buses = 1; // 坐第1趟车
        while (!q.empty()) {
            int sz = q.size();
            while (sz--) {
                int curRoute = q.front(); q.pop();
                
                // 遍历当前线路的所有站点
                for (int stop : routes[curRoute]) {
                    if (stop == target) return buses; // 找到目标！
                    
                    if (visitedStop.count(stop)) continue; // 该站已处理过
                    visitedStop.insert(stop);
                    
                    // 通过这个站点，找到可以换乘的所有线路
                    for (int nextRoute : stopToRoutes[stop]) {
                        if (visitedRoute[nextRoute]) continue;
                        visitedRoute[nextRoute] = true;
                        q.push(nextRoute);
                    }
                }
            }
            buses++; // 进入下一层 = 多坐一趟车
        }
        return -1;
    }
};
```

**为什么正确？**
- BFS 第 k 层代表"坐第 k 趟车能到达的所有站点"
- 对每条被访问的线路，遍历其所有站点，发现新的可换乘线路
- `visitedStop` 保证每个站点只用于发现新线路一次 → 总工作量为 O(总站点数)
- `visitedRoute` 保证每条线路只处理一次 → 不会重复遍历

**复杂度分析**：
- 时间 O(N × L)：每条线路遍历一次，每个站点处理一次，总计 O(Σroutes[i].length) = O(总站点数)
- 空间 O(N × L)：stopToRoutes 存储所有站点-线路关系

```
BFS 扩展过程 (示例1):
routes = [[1,2,7], [3,6,7]], source=1, target=6

stopToRoutes: {1:[0], 2:[0], 7:[0,1], 3:[1], 6:[1]}

Layer 1 (buses=1):
  入队: 线路0 (因为 source=1 在线路0中)
  处理线路0: 站点 {1,2,7}
    站1: 已访问，跳过
    站2: 新站，stopToRoutes[2]={0}，线路0已访问，无新线路
    站7: 新站，stopToRoutes[7]={0,1}，线路1未访问 → 入队线路1
  目标6未找到

Layer 2 (buses=2):
  处理线路1: 站点 {3,6,7}
    站7: 已访问，跳过
    站3: 新站，无新线路
    站6: == target → 返回 2 ✓
```

## 解法对比

| | 解法2: 线路级 BFS | 解法3: 站点BFS+线路扩展 |
|---|---|---|
| 建图 | 显式建 N×N 邻接表 | 隐式通过 stopToRoutes |
| 时间瓶颈 | O(N²×L) 两两比较线路 | O(总站点数)，无需两两比较 |
| 空间 | O(N²) 邻接表 | O(总站点数) 哈希表 |
| 适用场景 | N 很小 (≤几百) 时简洁 | 通用，面试首选 |
| 代码复杂度 | 需要排序+双指针判交集 | 更直觉，容易写对 |

## 易错点

1. **忘记 `source == target` 特判**：
   - ✗ 直接开始 BFS，可能 source 不在任何线路中也返回 -1
   - ✓ 开头 `if (source == target) return 0;`

2. **buses 计数位置搞错**：
   - ✗ 在 while 循环开头 `buses++`，导致多算一层
   - ✓ BFS 初始时 `buses=1`（坐第一趟车），层遍历结束后 `buses++`

3. **只标记 visitedRoute 不标记 visitedStop**：
   - ✗ 同一个站点反复用于发现新线路 → 时间退化
   - ✓ `visitedStop` 保证每个站点只处理一次，这是 O(总站点数) 复杂度的关键

4. **BFS 中先 buses++ 再检查 target**：
   - ✗ 在入队新线路时检查 target → 此时 buses 还没+1，返回值不对
   - ✓ 在遍历当前线路站点时检查 `stop == target`，此时 buses 就是正确答案

5. **stopToRoutes 中 source 不存在**：
   - 如果 source 不在任何线路中且 source ≠ target，BFS 队列为空，直接返回 -1。代码自然处理了这种情况。

## 面试追问

**Q1 (基础理解)**: 为什么不能直接在站点层面做 BFS？
> 两个问题：① 同一线路的站两两连边，边数可达 O(L²)，建图太慢；② BFS 层数是经过的站数，不是乘坐的车数，需要额外处理。以线路为节点做 BFS 天然对应"乘坐车数"。

**Q2 (优化)**: 解法2 的 O(N²) 建图能否避免？
> 可以。解法3 用 `stopToRoutes` 映射隐式建图，通过"站点"作为中间桥梁发现可换乘线路，总复杂度降到 O(总站点数)。

**Q3 (变体)**: 如果要求输出具体的换乘方案（坐了哪些线路、在哪些站换乘），怎么改？
> BFS 时额外维护一个 `parent` 数组，记录每条线路是从哪条线路、通过哪个站点换乘过来的。找到 target 后回溯 parent 链即可还原路径。

**Q4 (扩展)**: 如果每条线路有不同的票价，求最小花费到达 target？
> BFS 不再适用（边权不同），改用 Dijkstra：优先队列按累计花费排序，每次取最小花费的线路扩展。

## 相关题型

- **127. 单词接龙** — 同样的"层级 BFS 求最短变换"模式。本题中"线路"对应"单词"，"共享站点"对应"只差一个字母"。可以复用"双层 BFS + visited 去重"的模板。
- **1091. 二进制矩阵中的最短路径** — 标准 BFS 求最短路径，但在网格上。复用 BFS 逐层扩展的框架。
- **864. 获取所有钥匙的最短路径** — BFS + 状态压缩，类似本题的"多起点 BFS"模式。
- **752. 打开转盘锁** — BFS 求最短步数，可以练习"多起点 BFS"和"双向 BFS"优化。