# 721. 账户合并

## 核心思路

这道题本质上是一个**连通分量（Connected Components）**问题：每个邮箱是一个节点，同一个账户里的邮箱之间有边相连，最终要找出所有连通分量，每个连通分量对应一个合并后的账户。

## 思维链

1. **读完题第一反应**：需要把"有共同邮箱"的账户合并到一起。这就像是在找"哪些账户属于同一组"——典型的分组/连通性问题。

2. **暴力思路**：对每对账户 (i, j)，检查是否有共同邮箱。如果有，就把它们合并。但这需要 O(n²) 对比较，每次比较还需要遍历邮箱列表，效率很低。

3. **瓶颈在哪**：暴力法的瓶颈在于"怎么快速判断两个账户是否有共同邮箱"。如果我们换个角度——不是从"账户"出发找关系，而是从"邮箱"出发建立关系呢？

4. **关键洞察**：同一个账户里的所有邮箱一定属于同一个人。所以我们可以把同一账户里的邮箱"连接"起来。如果邮箱 A 出现在账户1和账户3中，那么账户1和账户3的所有邮箱就都连通了。

5. **选择工具**：找连通分量有两个经典工具——**DFS/BFS** 和 **Union-Find（并查集）**。两者都能解决，Union-Find 在"动态合并"场景下更自然。

6. **具体做法**：
   - 用哈希表记录每个邮箱第一次出现在哪个账户
   - 如果某个邮箱出现在多个账户中，就把这些账户合并（Union-Find）或建边（DFS）
   - 最后按连通分量收集所有邮箱，排序输出

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| DFS 建图 | 邮箱为节点建图，DFS 找连通分量 | O(NK · α(NK)) ≈ O(NK log(NK)) | O(NK) | 能说出即可 |
| Union-Find（按账户合并）| 邮箱映射到账户索引，用并查集合并账户 | O(NK · α(NK)) | O(NK) | ⭐ 必须写出 |
| Union-Find（邮箱直接合并）| 每个邮箱作为并查集节点 | O(NK · α(NK)) | O(NK) | 加分项 |

> N = 账户数，K = 每个账户平均邮箱数，NK = 总邮箱数（含重复）

## 关键提示

1. **不要被"名称"迷惑**：名称相同不代表是同一个人，名称不同一定不是同一个人（但题目保证同一人的所有账户名称相同）。判断是否同一人的唯一依据是**共同邮箱**。

2. **同一个账户里的邮箱怎么"连接"**：不需要两两连边！只需要把每个邮箱都和该账户的**第一个邮箱**连接即可——这是一个常见的优化，把 O(K²) 条边减少到 O(K) 条。

3. **Union-Find 的"节点"是什么**：可以让邮箱做节点（需要字符串映射），也可以让账户索引做节点（更简单）。两种都行，但后者代码更清晰。

4. **最后收集结果时别忘了排序**：题目要求每个账户的邮箱按 ASCII 排序。

5. **连通分量的可视化**：
```
账户0: [John, a@, b@]     账户1: [John, c@]
账户2: [John, a@, d@]     账户3: [Mary, e@]

邮箱 a@ 同时出现在账户0和账户2 → 合并账户0和账户2

连通分量:
  {账户0, 账户2} → 邮箱: {a@, b@, d@}  名称: John
  {账户1}        → 邮箱: {c@}           名称: John
  {账户3}        → 邮箱: {e@}           名称: Mary
```

## 解法详解

### 解法1: DFS 建图 — O(NK log(NK)) / O(NK)

**思考过程**：把问题建模成图。每个邮箱是一个节点，同一账户内的邮箱之间连边。然后用 DFS 找所有连通分量，每个连通分量就是一个合并后的账户。

```cpp
class Solution {
public:
    vector<vector<string>> accountsMerge(vector<vector<string>>& accounts) {
        // emailToIdx: 给每个邮箱分配一个唯一编号
        unordered_map<string, int> emailToIdx;
        // emailToName: 记录每个邮箱属于哪个人
        unordered_map<string, string> emailToName;
        // 邻接表
        unordered_map<int, vector<int>> graph;
        
        int idx = 0;
        for (auto& acc : accounts) {
            string name = acc[0];
            for (int i = 1; i < acc.size(); i++) {
                if (emailToIdx.find(acc[i]) == emailToIdx.end()) {
                    emailToIdx[acc[i]] = idx++;
                }
                emailToName[acc[i]] = name;
                // 同一账户内，把每个邮箱和第一个邮箱连边（星形连接，省边数）
                if (i > 1) {
                    int u = emailToIdx[acc[1]], v = emailToIdx[acc[i]];
                    graph[u].push_back(v);
                    graph[v].push_back(u);
                }
            }
        }
        
        // DFS 找连通分量
        vector<bool> visited(idx, false);
        // 反向映射：编号 → 邮箱字符串
        vector<string> idxToEmail(idx);
        for (auto& [email, id] : emailToIdx) {
            idxToEmail[id] = email;
        }
        
        vector<vector<string>> result;
        for (auto& [email, id] : emailToIdx) {
            if (!visited[id]) {
                // BFS/DFS 收集这个连通分量的所有邮箱
                vector<string> component;
                stack<int> stk;
                stk.push(id);
                visited[id] = true;
                while (!stk.empty()) {
                    int cur = stk.top(); stk.pop();
                    component.push_back(idxToEmail[cur]);
                    for (int nei : graph[cur]) {
                        if (!visited[nei]) {
                            visited[nei] = true;
                            stk.push(nei);
                        }
                    }
                }
                sort(component.begin(), component.end());
                component.insert(component.begin(), emailToName[component[0]]);
                result.push_back(component);
            }
        }
        return result;
    }
};
```

**关键点**：
- 同一账户内的邮箱只需要和第一个邮箱连边（星形拓扑），不需要两两连边
- 需要维护 emailToIdx 和 idxToEmail 两个映射，比较繁琐
- 排序复杂度 O(NK log(NK)) 是主要开销


### 解法2: Union-Find（按账户索引合并）— O(NK · α(NK)) / O(NK) ⭐ 面试首选

**从解法1优化**：DFS 需要显式建图，代码量大。Union-Find 天然适合"动态合并"——遇到一个邮箱已经出现过，就把当前账户和之前的账户合并。不需要建图！

```cpp
class Solution {
public:
    vector<int> parent, rank_;
    
    int find(int x) {
        // 路径压缩：让 x 直接指向根
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }
    
    void unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return;
        // 按秩合并：矮树挂到高树下
        if (rank_[px] < rank_[py]) swap(px, py);
        parent[py] = px;
        if (rank_[px] == rank_[py]) rank_[px]++;
    }
    
    vector<vector<string>> accountsMerge(vector<vector<string>>& accounts) {
        int n = accounts.size();
        parent.resize(n);
        rank_.resize(n, 0);
        // 初始化：每个账户是自己的根
        for (int i = 0; i < n; i++) parent[i] = i;
        
        // emailToAccount: 记录每个邮箱第一次出现在哪个账户
        unordered_map<string, int> emailToAccount;
        
        for (int i = 0; i < n; i++) {
            for (int j = 1; j < accounts[i].size(); j++) {
                const string& email = accounts[i][j];
                if (emailToAccount.count(email)) {
                    // 这个邮箱之前在账户 emailToAccount[email] 出现过
                    // 说明账户 i 和那个账户属于同一个人，合并！
                    unite(i, emailToAccount[email]);
                } else {
                    emailToAccount[email] = i;
                }
            }
        }
        
        // 按根节点收集每个连通分量的所有邮箱
        unordered_map<int, set<string>> rootToEmails;
        for (int i = 0; i < n; i++) {
            int root = find(i);
            for (int j = 1; j < accounts[i].size(); j++) {
                rootToEmails[root].insert(accounts[i][j]);
            }
        }
        
        // 组装结果
        vector<vector<string>> result;
        for (auto& [root, emails] : rootToEmails) {
            vector<string> merged;
            merged.push_back(accounts[root][0]); // 名称
            for (const string& e : emails) {     // set 自动排序
                merged.push_back(e);
            }
            result.push_back(merged);
        }
        return result;
    }
};
```

**关键点**：
- Union-Find 的节点是**账户索引**（0 到 n-1），不是邮箱字符串，这样实现更简单
- 遍历时用 `emailToAccount` 检测邮箱是否之前出现过，如果出现过就合并两个账户
- 收集结果时用 `set<string>` 自动去重+排序


### 解法3: Union-Find（邮箱直接作为节点）— O(NK · α(NK)) / O(NK)

**不同视角**：解法2是把"账户"作为并查集节点，这里把"邮箱"作为节点。同一账户里的邮箱两两属于同一组，只需要把每个邮箱和该账户的第一个邮箱 union 即可。

```cpp
class Solution {
public:
    unordered_map<string, string> parent;
    
    string find(const string& x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }
    
    void unite(const string& x, const string& y) {
        string px = find(x), py = find(y);
        if (px != py) parent[px] = py;
    }
    
    vector<vector<string>> accountsMerge(vector<vector<string>>& accounts) {
        // emailToName: 记录每个邮箱属于谁
        unordered_map<string, string> emailToName;
        
        for (auto& acc : accounts) {
            for (int i = 1; i < acc.size(); i++) {
                // 初始化：每个邮箱的 parent 是自己
                if (!parent.count(acc[i])) {
                    parent[acc[i]] = acc[i];
                }
                emailToName[acc[i]] = acc[0];
                // 同一账户内，把每个邮箱和第一个邮箱连接
                if (i > 1) {
                    unite(acc[i], acc[1]);
                }
            }
        }
        
        // 按根邮箱收集连通分量
        unordered_map<string, set<string>> rootToEmails;
        for (auto& [email, _] : parent) {
            rootToEmails[find(email)].insert(email);
        }
        
        // 组装结果
        vector<vector<string>> result;
        for (auto& [root, emails] : rootToEmails) {
            vector<string> merged;
            merged.push_back(emailToName[root]);
            for (const string& e : emails) {
                merged.push_back(e);
            }
            result.push_back(merged);
        }
        return result;
    }
};
```

**关键点**：
- 并查集的 parent 用 `unordered_map<string, string>` 实现，key 和 value 都是邮箱字符串
- 没有按秩合并，但有路径压缩，实际效率已经足够
- 代码更简洁，但字符串做 key 的哈希开销比整数大


## 解法对比

| | DFS 建图 | UF（账户索引）⭐ | UF（邮箱节点） |
|---|---|---|---|
| 核心思想 | 显式建图 + DFS 遍历 | 动态合并账户 | 动态合并邮箱 |
| 建模方式 | 邮箱→节点，同账户→边 | 账户→UF节点 | 邮箱→UF节点 |
| 代码复杂度 | 高（需要多个映射） | 中（推荐） | 低（最简洁） |
| 并查集节点类型 | N/A | 整数（快） | 字符串（慢一点） |
| 面试推荐 | 可以但不首选 | **首选** | 也可以 |

**什么时候选哪个**：
- 面试中推荐解法2，因为整数作为并查集节点效率高、写法清晰
- 如果对并查集不熟，DFS 建图也完全可以
- 解法3 代码最短，适合竞赛

## 易错点

1. **合并时只看名称相同就合并**
   - ✗ `if (accounts[i][0] == accounts[j][0]) unite(i, j);`
   - ✓ 只有**共同邮箱**才能合并，名称相同不代表同一人

2. **收集结果时没有对所有账户 find()**
   - ✗ 只对有过 union 的账户 find，忘了单独的账户
   - ✓ 必须对所有 i ∈ [0, n) 都做 find(i) 来收集邮箱

3. **忘记对邮箱排序**
   - ✗ 直接把收集到的邮箱放入结果
   - ✓ 用 `set<string>` 自动排序，或收集后手动 `sort`

4. **星形连接遗漏**
   - ✗ 同一账户里的邮箱只和上一个邮箱连接（链式），某些边缺失时可能断开
   - ✓ 都和第一个邮箱连接（星形），保证连通。注意：链式其实也正确（因为传递性），但星形更直观不易错

5. **并查集的 find 没有路径压缩**
   - ✗ `return parent[x] == x ? x : find(parent[x]);`（没有压缩）
   - ✓ `if (parent[x] != x) parent[x] = find(parent[x]); return parent[x];`

## 面试追问

**Q1：暴力思路是什么？瓶颈在哪？**
> 对每对账户检查是否有共同邮箱，有则合并。时间 O(n² × K)，瓶颈在于 n² 对比较和合并时的传递性处理。

**Q2：为什么选 Union-Find 而不是 DFS？有什么区别？**
> 两者都能找连通分量。Union-Find 的优势是"在线"的——边读入边合并，不需要先把图建完。DFS 需要先建完邻接表再遍历。面试中 Union-Find 代码更短、逻辑更清晰。时间复杂度相当（UF 有近乎 O(1) 的均摊复杂度）。

**Q3：如果账户数量极大（百万级），邮箱字符串很长，怎么优化？**
> 1. 先给每个邮箱分配整数 ID，并查集用整数操作（避免字符串哈希开销）
> 2. 按秩合并 + 路径压缩让 Union-Find 近乎 O(1)
> 3. 如果分布式场景，可以先按邮箱首字母分片（partition），每个分片内独立合并，再跨分片合并

**Q4：如果需要支持动态添加新账户并实时查询"这个邮箱属于谁"呢？**
> Union-Find 天然支持动态合并。维护一个 emailToRoot 映射，新账户来了就 union 相关邮箱。查询时 find 到根，根对应的名称就是答案。

## 相关题型

- **547. 省份数量** — 同样是找连通分量，但更简单（直接给了邻接矩阵）。学会本题的 Union-Find 模板可以直接复用。
- **684. 冗余连接** — Union-Find 检测环：如果 union 时发现两个节点已经在同一集合，说明这条边是冗余的。复用 find/unite 模板。
- **839. 相似字符串组** — 和本题几乎相同的框架：判断两个字符串是否"相似"（对应本题的"有共同邮箱"），然后用 Union-Find 合并。
- **1319. 连通网络的操作次数** — Union-Find 计算连通分量数，复用 find/unite 模板，额外统计冗余边数。