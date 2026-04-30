# 1202. 交换字符串中的元素

## 核心思路

本质上在问：**给定一组"可传递"的交换关系，同一个连通分量内的所有位置可以任意排列，求字典序最小的结果。** 关键洞察是交换具有传递性——如果 0↔1、1↔2，那么 0、1、2 三个位置的字符可以任意重排。

## 思维链

1. **读完题第一反应**：每对 (a, b) 可以无限次交换。暴力模拟？但交换次序太多，不知道怎么交换才能达到最小——模拟路线太复杂。

2. **关键观察——传递性**：如果位置 0 和位置 1 能交换，位置 1 和位置 2 能交换，那么通过多次操作，位置 0、1、2 上的字符可以**任意排列**（冒泡排序思想：任何相邻可交换的序列都能排成任意顺序）。

3. **问题转化**：把每对 (a, b) 看作一条边，所有索引构成一个图。同一个**连通分量**内的位置可以任意重排。要字典序最小，就把每个连通分量内的字符**排序后**按位置从小到大依次填入。

4. **怎么找连通分量？**
   - 方法一：**DFS/BFS** — 建邻接表，遍历找连通分量。
   - 方法二：**并查集 (Union-Find)** — 天然适合"合并+查询连通性"的场景，且实现简洁。

5. **具体操作**：
   - 用并查集把所有 pair 合并
   - 按根节点分组，收集每组的索引和对应字符
   - 每组内字符排序，索引排序，最小字符放最小索引

6. **复杂度分析**：并查集操作近 O(1)，分组 O(n)，排序 O(n log n)，总体 O(n log n + m·α(n))。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| DFS/BFS | 建图 + 遍历找连通分量 + 组内排序 | O(n log n + m) | O(n + m) | 能说出即可 |
| 并查集 | Union-Find 合并 + 分组排序 | O(n log n + m·α(n)) | O(n) | ⭐ 必须写出 |

## 关键提示

1. **想想传递性**：`[0,1]` 和 `[1,2]` 意味着 0、1、2 三个位置可以任意排列。这不就是连通分量吗？

2. **ASCII 示意图 — 示例2的连通关系**：
   ```
   pairs = [[0,3],[1,2],[0,2]]
   
   索引:  0 --- 3       0 和 3 连通
          |             0 和 2 连通（传递到 1 也连通）
          2 --- 1       
   
   连通分量: {0, 1, 2, 3} — 所有位置在同一组
   字符:     d, c, a, b  → 排序后 a, b, c, d
   位置:     0, 1, 2, 3  → 排序后 0, 1, 2, 3
   结果:     s[0]='a', s[1]='b', s[2]='c', s[3]='d' → "abcd"
   ```

3. **为什么组内排序就能得到最小？** 因为连通分量内可以任意重排，要字典序最小，显然最小字符放最小位置。

4. **并查集 vs DFS**：并查集代码更短、面试更常考；DFS 需要建邻接表，代码稍长但思路直观。

5. **易错方向**：不要试图模拟交换过程，那是 NP 级别的搜索。抓住"传递性 → 连通分量"这个核心。

## 解法详解

### 解法1: DFS 找连通分量 — O(n log n + m) / O(n + m)

**思考过程**: 把索引看作图的节点，pairs 是边，用 DFS 找出每个连通分量，然后对每个分量内的字符排序填回。

```cpp
class Solution {
public:
    string smallestStringWithSwaps(string s, vector<vector<int>>& pairs) {
        int n = s.size();
        // 建邻接表
        vector<vector<int>> adj(n);
        for (auto& p : pairs) {
            adj[p[0]].push_back(p[1]);
            adj[p[1]].push_back(p[0]);
        }
        
        vector<bool> visited(n, false);
        string result = s;
        
        for (int i = 0; i < n; i++) {
            if (visited[i]) continue;
            
            // DFS 收集当前连通分量的所有索引
            vector<int> indices;
            stack<int> stk;
            stk.push(i);
            visited[i] = true;
            while (!stk.empty()) {
                int node = stk.top(); stk.pop();
                indices.push_back(node);
                for (int nei : adj[node]) {
                    if (!visited[nei]) {
                        visited[nei] = true;
                        stk.push(nei);
                    }
                }
            }
            
            // 收集这些位置上的字符
            vector<char> chars;
            for (int idx : indices) chars.push_back(s[idx]);
            
            // 索引排序、字符排序，最小字符填最小位置
            sort(indices.begin(), indices.end());
            sort(chars.begin(), chars.end());
            
            for (int k = 0; k < indices.size(); k++) {
                result[indices[k]] = chars[k];
            }
        }
        return result;
    }
};
```

**关键点**: DFS 用栈实现避免递归栈溢出（n 可达 10^5）。收集完连通分量后，必须对**索引和字符都排序**再配对。

---

### 解法2: 并查集 (Union-Find) — O(n log n + m·α(n)) / O(n) ⭐ 面试首选

**从解法1优化**: DFS 需要建邻接表（O(m) 空间），并查集更轻量，直接在 pairs 上合并，无需邻接表。面试中手写并查集是加分项。

```cpp
class Solution {
public:
    vector<int> parent, rank_;
    
    int find(int x) {
        // 路径压缩：让 x 直接指向根，下次查询 O(1)
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    
    void unite(int x, int y) {
        int rx = find(x), ry = find(y);
        if (rx == ry) return;
        // 按秩合并：矮树挂到高树下，保持树平衡
        if (rank_[rx] < rank_[ry]) swap(rx, ry);
        parent[ry] = rx;
        if (rank_[rx] == rank_[ry]) rank_[rx]++;
    }
    
    string smallestStringWithSwaps(string s, vector<vector<int>>& pairs) {
        int n = s.size();
        parent.resize(n);
        rank_.resize(n, 0);
        for (int i = 0; i < n; i++) parent[i] = i;
        
        // 合并所有 pair
        for (auto& p : pairs) {
            unite(p[0], p[1]);
        }
        
        // 按根节点分组：root → 该组所有索引
        unordered_map<int, vector<int>> groups;
        for (int i = 0; i < n; i++) {
            groups[find(i)].push_back(i);
        }
        
        // 每组内：字符排序后填回最小位置
        string result = s;
        for (auto& [root, indices] : groups) {
            // indices 已经是按顺序遍历的，天然有序
            // 但为保险起见（哈希表不保证顺序）还是排序
            sort(indices.begin(), indices.end());
            
            vector<char> chars;
            for (int idx : indices) chars.push_back(s[idx]);
            sort(chars.begin(), chars.end());
            
            for (int k = 0; k < indices.size(); k++) {
                result[indices[k]] = chars[k];
            }
        }
        return result;
    }
};
```

**并查集过程图解（示例3: s="cba", pairs=[[0,1],[1,2]]）**：
```
初始:  parent = [0, 1, 2]   每个节点是自己的根
       
合并(0,1): parent = [0, 0, 2]    0←1
合并(1,2): find(1)=0, find(2)=2
           parent = [0, 0, 0]    0←1, 0←2

分组: root=0 → indices=[0,1,2]
      chars = ['c','b','a'] → 排序 → ['a','b','c']
      indices = [0,1,2]
      result = "abc"
```

## 解法对比

| | DFS/BFS | 并查集 |
|---|---|---|
| 核心操作 | 建图 + 遍历 | 合并 + 查询根 |
| 空间 | O(n + m) 邻接表 | O(n) parent数组 |
| 代码量 | 稍多（邻接表+DFS） | 适中（模板化） |
| 面试偏好 | 通用 | **更受欢迎**（考察并查集） |
| 适用拓展 | 适合需要遍历路径的场景 | 适合只需判断连通性的场景 |

两种解法时间复杂度本质相同，瓶颈都在**排序** O(n log n)。

## 易错点

1. **忘记传递性，尝试贪心模拟交换**
   - ✗ 对每个 pair 直接交换一次，以为就能得到最小
   - ✓ 必须理解同一连通分量内可以任意重排

2. **并查集 find 没有路径压缩导致 TLE**
   - ✗ `int find(int x) { while (parent[x] != x) x = parent[x]; return x; }`
   - ✓ `int find(int x) { if (parent[x] != x) parent[x] = find(parent[x]); return parent[x]; }`
   - 没有路径压缩，最坏 O(n) 每次查询，总体可能 O(n²)

3. **分组后忘记对 indices 排序**
   - ✗ 直接把排序后的字符按收集顺序填回
   - ✓ 索引和字符都排序，确保最小字符在最小索引位置
   - 用 `unordered_map` 分组时，遍历顺序不确定，更容易出错

4. **parent 初始化错误**
   - ✗ `parent.resize(n, 0);` — 所有节点指向0，一开始就全连通了
   - ✓ `for (int i = 0; i < n; i++) parent[i] = i;` — 每个节点初始指向自己

## 面试追问

**Q1: 暴力能做吗？瓶颈在哪？**
> 暴力的话可以尝试模拟所有交换顺序，但这是指数级的。关键洞察是"任意多次交换 = 同一连通分量内任意重排"，问题从"搜索最优交换序列"变成"分组 + 排序"。

**Q2: 为什么连通分量内的字符可以任意重排？**
> 类比冒泡排序：如果一组位置中任意相邻两个都能交换，经过足够多次交换后可以达到任意排列。连通分量保证了任意两个位置之间都存在交换路径（可能经过中间节点），所以可以把任意字符"传递"到任意位置。

**Q3: 如果 pairs 是动态增加的（在线查询），每次加一条边后都要返回当前最小字符串，怎么办？**
> 并查集天然支持增量式合并。每次新加一条边：
> - 如果两端已在同一组，结果不变
> - 如果不同组，合并两组，重新对合并后的组排序填回
> - 优化：维护每组字符的有序结构（如 multiset），合并时用归并，避免重新排序

## 相关题型

- **547. 省份数量** — 同样是并查集/DFS 找连通分量的模板题，区别：547 只需要计数连通分量个数，本题需要对每个分量内部做排序操作。
- **684. 冗余连接** — 并查集判断"加哪条边会形成环"，复用同样的 Union-Find 模板，区别：684 重点在检测合并冲突，本题重点在分组后处理。
- **721. 账户合并** — 用并查集合并同一用户的邮箱，合并后排序输出。和本题几乎同构：连通分量内收集元素 → 排序 → 输出。
- **839. 相似字符串组** — 同样是构建连通关系后求分组，区别在于"边"的定义不同（字符串相似性 vs 显式给定的 pairs）。