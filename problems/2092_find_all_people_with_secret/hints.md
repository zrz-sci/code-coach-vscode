# 2092. 找出知晓秘密的所有专家

## 核心思路

本质上是一个**按时间顺序传播的连通性问题**：秘密沿着会议关系传播，但必须按时间先后顺序，且同一时刻的所有会议需要同时处理（秘密可以在同一时刻通过多场会议链式传播）。

## 思维链

1. **读完题第一反应**：秘密从 person 0 开始，按时间顺序经过会议传播给更多人。最朴素的做法——按时间排序所有会议，逐个处理，如果会议双方有一个知道秘密，就传给另一个。但问题在于：**同一时刻的会议可以链式传播**（A→B 和 B→C 同时发生，A 知道秘密，C 也应该知道），简单逐个处理可能遗漏。

2. **暴力解的瓶颈**：同一时刻的会议需要反复遍历直到没有新传播为止（类似 BFS 扩散），最坏情况下同一时刻有 O(m) 场会议，每轮只传播一个人，需要 O(m) 轮，总共 O(m²)。

3. **怎么突破瓶颈？** 同一时刻的会议构成一个图，我们需要找"与已知秘密的人连通的所有人"。这正是 **Union-Find（并查集）** 或 **BFS/DFS** 的经典场景——连通分量问题。

4. **Union-Find 方案**：按时间分组会议。对于每个时间点，把该时刻所有会议的两端 union 起来，然后检查哪些人和已知秘密的人在同一个连通分量。关键：**处理完一个时间点后，没有获得秘密的人要断开连接（reset）**，否则会影响后续时间点的判断。

5. **BFS 方案**：按时间分组会议。对于每个时间点，建立该时刻的邻接表，从所有已知秘密的参会者出发做 BFS/DFS，标记所有能到达的人。

6. **核心难点**：同一时刻的"瞬时传播"——不能简单地线性扫描，必须用连通性算法处理同一时刻的所有会议。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力模拟 | 按时间分组，每组反复扫描直到无新传播 | O(m² + m·log m) | O(n + m) | 能说出即可 |
| BFS 分组传播 | 按时间分组，每组建图 BFS | O(m·log m + n + m) | O(n + m) | ⭐ 必须写出 |
| Union-Find 分组 + 重置 | 按时间分组，每组 union 后 reset 未获密者 | O(m·log m + m·α(n)) | O(n + m) | ⭐ 必须写出 |

## 关键提示

1. **时间排序是第一步**：秘密只能从过去传到未来，所以必须按时间顺序处理会议。

2. **同一时刻必须整体处理**：示例3中 `[1,2,1]` 和 `[2,3,1]` 同时发生，专家2可以在 time=1 同时接收并转发秘密。这意味着同一时刻的会议不能逐个处理，要当作一个"批次"。

3. **Union-Find 的陷阱 — reset 操作**：如果某个时间点的一组人 union 在一起，但最终这个连通分量里没有任何人知道秘密，那这些边在后续不应该有任何效果。必须把这些人的 parent 重置回自己。

4. **BFS 的思路更直觉**：对每个时间批次，把会议构成图，从已知秘密的节点出发 BFS 扩散，能到的都标记为知道秘密。

5. **ASCII 示意图 — 示例3的传播过程**：
```
时间线:  t=0          t=1              t=2
         0--1         1--2  2--3       3--4
         (初始)       (链式传播!)      
         
知道秘密: {0,1}  →   {0,1,2,3}   →  {0,1,2,3,4}

t=1 的关键: 会议 [1,2] 和 [2,3] 同时发生
  建图: 1—2—3
  BFS 从 {1} 出发 → 到达 2 → 到达 3
  所以 2 和 3 都在 t=1 获得秘密
```

## 解法详解

### 解法1: 暴力模拟 — O(m² + m·log m) / O(n + m)

**思考过程**: 最直觉的做法——按时间分组，对每组会议反复扫描。每一轮如果发现某场会议有一方知道秘密，就传给另一方。重复直到某一轮没有新传播。

```cpp
class Solution {
public:
    vector<int> findAllPeople(int n, vector<vector<int>>& meetings, int firstPerson) {
        vector<bool> knows(n, false);
        knows[0] = true;
        knows[firstPerson] = true;
        
        // 按时间排序
        sort(meetings.begin(), meetings.end(), [](auto& a, auto& b) {
            return a[2] < b[2];
        });
        
        int m = meetings.size();
        int i = 0;
        while (i < m) {
            int curTime = meetings[i][2];
            int j = i;
            // 找到同一时间的所有会议 [i, j)
            while (j < m && meetings[j][2] == curTime) j++;
            
            // 反复扫描直到没有新传播
            bool changed = true;
            while (changed) {
                changed = false;
                for (int k = i; k < j; k++) {
                    int x = meetings[k][0], y = meetings[k][1];
                    if (knows[x] && !knows[y]) {
                        knows[y] = true;
                        changed = true;
                    } else if (knows[y] && !knows[x]) {
                        knows[x] = true;
                        changed = true;
                    }
                }
            }
            i = j;
        }
        
        vector<int> result;
        for (int p = 0; p < n; p++) {
            if (knows[p]) result.push_back(p);
        }
        return result;
    }
};
```

**关键点**: 最坏情况下同一时刻有 m 场会议形成一条链，每轮只传播一个人，需要 O(m) 轮 × O(m) 扫描 = O(m²)。面试中提到这个方案展示理解即可，然后说出瓶颈引出优化。

---

### 解法2: BFS 分组传播 — O(m·log m + n + m) / O(n + m) ⭐ 面试首选

**从解法1优化**: 解法1对同一时刻反复扫描的本质是在找"与已知秘密者连通的所有人"。直接对同一时刻的会议建图，从已知秘密的人出发做 BFS，一次就能找到所有可达的人。

```
// BFS 传播示意 (示例3, t=1):
//
// 同一时刻 t=1 的会议: [1,2], [2,3]
// 建图: 1 — 2 — 3
//
// 已知秘密的参会者: {1}
// BFS:  队列=[1] → 访问1, 邻居2入队
//       队列=[2] → 访问2, 邻居1(已访问)跳过, 邻居3入队
//       队列=[3] → 访问3, 邻居2(已访问)跳过
// 结果: 1,2,3 都知道秘密 ✓
```

```cpp
class Solution {
public:
    vector<int> findAllPeople(int n, vector<vector<int>>& meetings, int firstPerson) {
        vector<bool> knows(n, false);
        knows[0] = true;
        knows[firstPerson] = true;
        
        sort(meetings.begin(), meetings.end(), [](auto& a, auto& b) {
            return a[2] < b[2];
        });
        
        int m = meetings.size();
        int i = 0;
        while (i < m) {
            int curTime = meetings[i][2];
            int j = i;
            while (j < m && meetings[j][2] == curTime) j++;
            
            // 对这批会议建邻接表
            unordered_map<int, vector<int>> adj;
            queue<int> q;
            unordered_set<int> visited;
            
            for (int k = i; k < j; k++) {
                int x = meetings[k][0], y = meetings[k][1];
                adj[x].push_back(y);
                adj[y].push_back(x);
            }
            
            // BFS 起点: 这批会议中已经知道秘密的人
            for (auto& [node, _] : adj) {
                if (knows[node]) {
                    q.push(node);
                    visited.insert(node);
                }
            }
            
            while (!q.empty()) {
                int cur = q.front(); q.pop();
                for (int nei : adj[cur]) {
                    if (!visited.count(nei)) {
                        visited.insert(nei);
                        knows[nei] = true;
                        q.push(nei);
                    }
                }
            }
            
            i = j;
        }
        
        vector<int> result;
        for (int p = 0; p < n; p++) {
            if (knows[p]) result.push_back(p);
        }
        return result;
    }
};
```

**关键点**: 每个时间批次的边只会被遍历一次（BFS 的特性），所以所有批次加起来总边数是 O(m)，总时间是 O(m·log m) 排序 + O(m + n) BFS。

---

### 解法3: Union-Find 分组 + 重置 — O(m·log m + m·α(n)) / O(n) ⭐ 面试首选

**另一个视角**: 同一时刻的连通性问题用 Union-Find 也很自然。关键难点在于：**如果一个连通分量中没有人知道秘密，处理完后必须 reset**，否则这些人的"虚假连接"会污染后续时间点。

```
// Union-Find + Reset 示意 (示例2):
//
// 初始: knows = {0, 3}
//
// t=2: 会议 [1,2]
//   union(1, 2) → {1,2} 连通
//   检查: 1不知道, 2不知道 → 这个分量无秘密
//   reset: parent[1]=1, parent[2]=2  ← 关键! 断开连接
//
// t=3: 会议 [3,1], [0,3]
//   union(3, 1) → {3,1} 连通
//   union(0, 3) → {0,3,1} 连通
//   检查: 0知道 → 整个分量 {0,1,3} 都知道 ✓
//   (如果 t=2 没 reset, 2还连着1, 会错误地得到2也知道)
```

```cpp
class Solution {
public:
    vector<int> parent, rank_;
    
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    
    void unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return;
        if (rank_[px] < rank_[py]) swap(px, py);
        parent[py] = px;
        if (rank_[px] == rank_[py]) rank_[px]++;
    }
    
    vector<int> findAllPeople(int n, vector<vector<int>>& meetings, int firstPerson) {
        parent.resize(n);
        rank_.resize(n, 0);
        for (int i = 0; i < n; i++) parent[i] = i;
        
        // person 0 和 firstPerson 初始连通
        unite(0, firstPerson);
        
        sort(meetings.begin(), meetings.end(), [](auto& a, auto& b) {
            return a[2] < b[2];
        });
        
        int m = meetings.size();
        int i = 0;
        while (i < m) {
            int curTime = meetings[i][2];
            int j = i;
            while (j < m && meetings[j][2] == curTime) j++;
            
            // 记录本批次涉及的所有人
            vector<int> people;
            for (int k = i; k < j; k++) {
                int x = meetings[k][0], y = meetings[k][1];
                unite(x, y);
                people.push_back(x);
                people.push_back(y);
            }
            
            // 检查: 没有和 person 0 连通的人，要 reset
            for (int p : people) {
                if (find(p) != find(0)) {
                    // 这个人所在的连通分量没有秘密，断开
                    parent[p] = p;
                    rank_[p] = 0;
                }
            }
            
            i = j;
        }
        
        vector<int> result;
        for (int p = 0; p < n; p++) {
            if (find(p) == find(0)) result.push_back(p);
        }
        return result;
    }
};
```

**关键点**: reset 操作只重置本批次涉及的、且未获得秘密的人。这确保了 UF 结构在进入下一个时间点时是"干净"的。

## 解法对比

| | 暴力模拟 | BFS 分组 | Union-Find |
|---|---|---|---|
| 时间 | O(m² + m log m) | O(m log m + m + n) | O(m log m + m·α(n)) |
| 空间 | O(n) | O(n + m) 邻接表 | O(n) |
| 核心操作 | 反复扫描 | 建图 + BFS | union + reset |
| 代码复杂度 | 简单 | 中等 | 中等偏高 |
| 面试推荐 | 仅作思路铺垫 | ✅ 直觉清晰 | ✅ 考察 UF 功底 |

**选择建议**：
- BFS 方案逻辑更直觉，面试中更容易写对和解释清楚
- Union-Find 方案更考察数据结构功底，如果面试官追问"还有其他方法吗"可以给出
- 两者实际性能相近，BFS 空间稍大（邻接表），UF 空间更紧凑

## 易错点

1. **Union-Find 忘记 reset**：
   - ✗ 只 union 不 reset → 时间 t 的会议中未获秘密的人保持连通，后续时间可能错误传播
   - ✓ 每个时间批次处理完后，遍历涉及的人，与 person 0 不连通的重置 `parent[p]=p`

2. **Reset 的遍历对象搞错**：
   - ✗ 遍历所有 n 个人做 reset → O(n) per batch，总体可能 O(n·T)
   - ✓ 只遍历本批次 `people` 列表中的人 → 总体 O(m)

3. **BFS 起点遗漏**：
   - ✗ 只把会议中某一方已知秘密的人加入 BFS 起点
   - ✓ 必须遍历该批次邻接表中**所有节点**，检查 `knows[node]` 后加入起点。因为一个人可能在多场会议中出现

4. **排序后分组边界**：
   - ✗ 用 `meetings[j][2] == meetings[i][2]` 但 j 没有先初始化为 i
   - ✓ `int j = i; while (j < m && meetings[j][2] == curTime) j++;` 确保 j 从 i 开始

5. **Union-Find 路径压缩 vs reset 冲突**：
   - ✗ reset 时只写 `parent[p] = p`，但 p 的子节点的 parent 可能还指向 p 的旧根
   - ✓ 对本批次所有人都做 reset（因为路径压缩后，每个人直接指向根，reset 每个人就够了）

## 面试追问

**Q1: 暴力法的时间复杂度瓶颈在哪？**
> 同一时刻的会议需要反复扫描，最坏 O(m) 轮。本质是在做"连通分量的 BFS"但用了低效的扫描方式。

**Q2: 为什么 Union-Find 需要 reset？能不能不 reset？**
> 不能。如果 t=2 时 A 和 B union 但都不知秘密，不 reset 的话，t=5 时如果 B 和 C 开会且 C 知道秘密，A 也会被错误地标记为知道（因为 A 还和 B 连着）。但 A 和 B 在 t=2 的会议并不能让信息"穿越时间"传播。

**Q3: 如果会议数量非常大但时间范围很小，怎么优化？**
> 可以用 counting sort / bucket sort 按时间分桶，避免 O(m log m) 的排序开销，变成 O(m + T) 其中 T 是最大时间值（题目说 ≤ 10⁵）。

**Q4: 如果要求输出每个人最早知道秘密的时间呢？**
> BFS 方案天然支持——在标记 `knows[nei] = true` 时同时记录 `knowTime[nei] = curTime`。Union-Find 方案需要额外维护。

## 相关题型

- **547. 省份数量** — 基础 Union-Find 练习，没有时间维度，直接求连通分量数。本题可以复用 UF 模板，区别在于需要按时间分批 + reset。
- **1319. 连通网络的操作次数** — 同样是 Union-Find 求连通性，但不需要按时间分组处理。
- **839. 相似字符串组** — Union-Find 求连通分量，但判断"是否连接"的条件不同（字符串相似性）。
- **990. 等式方程的可满足性** — Union-Find + 后续验证不等式，和本题"先 union 再检查"的模式类似。
- **BFS 系列: 994. 腐烂的橘子 / 542. 01 矩阵** — 同样是"多源 BFS 按层扩散"的模式，本题是按时间分批的多源 BFS。