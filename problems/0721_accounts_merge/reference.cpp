/*
 * LeetCode 721: 账户合并 (Accounts Merge)
 * 
 * 【题目本质】
 * 给定多个账户（名称 + 邮箱列表），如果两个账户有共同邮箱则属于同一人。
 * 本质是求"连通分量"：邮箱是节点，同一账户的邮箱互相连通，
 * 最终把每个连通分量的所有邮箱收集起来。
 *
 * 【解法总览】
 * 解法1: DFS建图         — O(NK log NK) / O(NK) — 经典图遍历
 * 解法2: Union-Find(账户索引) — O(NK α(N)) / O(NK) — 面试首选 ⭐
 * 解法3: Union-Find(邮箱节点) — O(NK α(NK)) / O(NK) — 代码最简洁
 *
 * N = 账户数, K = 每个账户平均邮箱数
 */

// ============================================================
// 解法1: DFS 建图 — 邮箱为节点，同账户邮箱连边，DFS找连通分量
// 时间: O(NK log NK)  空间: O(NK)
//
// 【思路】
// 把每个邮箱看作图中的节点。同一个账户里的邮箱之间连边。
// 然后用 DFS（或 BFS）找所有连通分量，每个连通分量就是
// 一个合并后的账户。
//
// 为什么想到建图？因为"共同邮箱"就是边，找"属于同一人的所有邮箱"
// 就是找连通分量——这是图论的经典问题。
//
// 连边优化：同一账户内不需要两两连边 O(K²)，
// 只需每个邮箱和第一个邮箱连边 O(K)（星形拓扑）。
//
// 可视化示例:
//   accounts[0]: [John, a@, b@]     → a@—b@
//   accounts[1]: [John, c@]         → c@ (孤立)
//   accounts[2]: [John, a@, d@]     → a@—d@ (a@已存在,和accounts[0]连通)
//   accounts[3]: [Mary, e@]         → e@ (孤立)
//
//   图:  b@ — a@ — d@    c@    e@
//         连通分量1       分量2  分量3
//
//   结果: [John, a@, b@, d@], [John, c@], [Mary, e@]
// ============================================================
class Solution1 {
public:
    vector<vector<string>> accountsMerge(vector<vector<string>>& accounts) {
        unordered_map<string, int> emailToIdx;
        unordered_map<string, string> emailToName;
        unordered_map<int, vector<int>> graph;

        int idx = 0;
        for (auto& acc : accounts) {
            string name = acc[0];
            for (int i = 1; i < (int)acc.size(); i++) {
                if (!emailToIdx.count(acc[i])) {
                    emailToIdx[acc[i]] = idx++;
                }
                emailToName[acc[i]] = name;
                // 星形连接：每个邮箱和该账户的第一个邮箱连边
                if (i > 1) {
                    int u = emailToIdx[acc[1]], v = emailToIdx[acc[i]];
                    graph[u].push_back(v);
                    graph[v].push_back(u);
                }
            }
        }

        // 编号 → 邮箱的反向映射
        vector<string> idxToEmail(idx);
        for (auto& [email, id] : emailToIdx) {
            idxToEmail[id] = email;
        }

        // DFS 找连通分量
        vector<bool> visited(idx, false);
        vector<vector<string>> result;

        for (auto& [email, id] : emailToIdx) {
            if (visited[id]) continue;

            // 用栈模拟 DFS
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

            // 邮箱排序，前面加上名称
            sort(component.begin(), component.end());
            component.insert(component.begin(), emailToName[component[0]]);
            result.push_back(component);
        }

        return result;
    }
};


// ============================================================
// 解法2: Union-Find（账户索引作为节点）— 面试首选 ⭐
// 时间: O(NK · α(N))  空间: O(NK)
//
// 【思路】
// 解法1需要显式建图再DFS遍历，代码量大。
// 换一种思路：并查集天然适合"动态合并"。
//
// 核心观察：如果邮箱 X 同时出现在账户 i 和账户 j 中，
// 那么 i 和 j 一定是同一个人 → union(i, j)。
//
// 做法：
// 1. 遍历所有账户的所有邮箱
// 2. 用 emailToAccount 记录每个邮箱第一次出现在哪个账户
// 3. 如果邮箱已经出现过，union 当前账户和之前的账户
// 4. 最后按 find(i) 收集每个连通分量的邮箱
//
// 为什么并查集的节点是"账户索引"而不是"邮箱"？
// → 整数做节点效率更高（数组直接索引 vs 字符串哈希）
// → 收集结果时直接通过账户索引拿名称，不需要额外映射
//
// 可视化合并过程 (示例1):
//   处理 accounts[0]: [John, a@, b@]
//     a@ → 第一次出现 → emailToAccount[a@] = 0
//     b@ → 第一次出现 → emailToAccount[b@] = 0
//
//   处理 accounts[1]: [John, c@]
//     c@ → 第一次出现 → emailToAccount[c@] = 1
//
//   处理 accounts[2]: [John, a@, d@]
//     a@ → 已在账户0出现 → union(2, 0)  ← 关键合并！
//     d@ → 第一次出现 → emailToAccount[d@] = 2
//
//   处理 accounts[3]: [Mary, e@]
//     e@ → 第一次出现 → emailToAccount[e@] = 3
//
//   并查集状态: {0,2} 一组, {1} 一组, {3} 一组
//   收集:
//     根0 → 账户0的邮箱{a@,b@} + 账户2的邮箱{a@,d@} → {a@,b@,d@}
//     根1 → {c@}
//     根3 → {e@}
// ============================================================
class Solution2 {
public:
    vector<int> parent, rank_;

    int find(int x) {
        // 路径压缩：递归时顺便把 x 直接挂到根节点下
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    void unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return; // 已经在同一集合
        // 按秩合并：矮树挂到高树下，保持树的平衡
        if (rank_[px] < rank_[py]) swap(px, py);
        parent[py] = px;
        if (rank_[px] == rank_[py]) rank_[px]++;
    }

    vector<vector<string>> accountsMerge(vector<vector<string>>& accounts) {
        int n = accounts.size();
        parent.resize(n);
        rank_.resize(n, 0);
        for (int i = 0; i < n; i++) parent[i] = i;

        // 记录每个邮箱第一次出现在哪个账户
        unordered_map<string, int> emailToAccount;

        for (int i = 0; i < n; i++) {
            for (int j = 1; j < (int)accounts[i].size(); j++) {
                const string& email = accounts[i][j];
                if (emailToAccount.count(email)) {
                    // 邮箱已出现在另一个账户 → 合并两个账户
                    unite(i, emailToAccount[email]);
                } else {
                    // 邮箱第一次出现，记录属于账户 i
                    emailToAccount[email] = i;
                }
            }
        }

        // 按根节点收集所有邮箱（用 set 自动去重+排序）
        unordered_map<int, set<string>> rootToEmails;
        for (int i = 0; i < n; i++) {
            int root = find(i);
            // 必须对每个账户都 find，包括没有被 union 过的
            for (int j = 1; j < (int)accounts[i].size(); j++) {
                rootToEmails[root].insert(accounts[i][j]);
            }
        }

        // 组装最终结果
        vector<vector<string>> result;
        for (auto& [root, emails] : rootToEmails) {
            vector<string> merged;
            merged.push_back(accounts[root][0]); // 名称从根账户取
            for (const string& e : emails) {
                merged.push_back(e); // set 已排序
            }
            result.push_back(merged);
        }

        return result;
    }
};


// ============================================================
// 解法3: Union-Find（邮箱字符串直接作为节点）
// 时间: O(NK · α(NK))  空间: O(NK)
//
// 【思路】
// 解法2用账户索引做并查集节点，需要最后遍历所有账户收集邮箱。
// 另一种方式：直接让邮箱字符串做并查集节点。
//
// 同一账户内的每个邮箱都和第一个邮箱 union。
// 如果某个邮箱出现在多个账户中，它会自然地把这些账户连起来
// （因为它在不同账户中都和各自的第一个邮箱 union，传递性生效）。
//
// 优点：代码最简洁
// 缺点：字符串做哈希 key 比整数慢
// ============================================================
class Solution3 {
public:
    unordered_map<string, string> parent;

    string find(const string& x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]); // 路径压缩
        }
        return parent[x];
    }

    void unite(const string& x, const string& y) {
        string px = find(x), py = find(y);
        if (px != py) {
            parent[px] = py; // 简单合并（未按秩，但有路径压缩，足够）
        }
    }

    vector<vector<string>> accountsMerge(vector<vector<string>>& accounts) {
        unordered_map<string, string> emailToName;

        for (auto& acc : accounts) {
            for (int i = 1; i < (int)acc.size(); i++) {
                // 初始化：每个邮箱的 parent 指向自己
                if (!parent.count(acc[i])) {
                    parent[acc[i]] = acc[i];
                }
                emailToName[acc[i]] = acc[0];
                // 同一账户内，所有邮箱和第一个邮箱 union
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


// ============================================================
// 【解法对比】
//
// | 解法 | 核心思路 | UF节点类型 | 代码复杂度 | 推荐场景 |
// |------|----------|-----------|-----------|---------|
// | DFS建图 | 显式建邻接表+DFS | N/A | 高 | 不熟悉UF时 |
// | UF账户索引 | 合并账户索引 | 整数(快) | 中 | 面试首选⭐ |
// | UF邮箱节点 | 合并邮箱字符串 | 字符串(慢) | 低 | 竞赛/追求简洁 |
//
// 解法2 vs 解法3: 
//   解法2 的并查集用整数数组，find/unite 更快；收集时需遍历所有账户
//   解法3 的并查集用 unordered_map<string,string>，代码短但哈希开销大
//
// 【易错点】
// 1. ✗ 看到名称相同就合并：名称相同不代表同一人
//    ✓ 只有共同邮箱才合并
//
// 2. ✗ 收集结果时只遍历 emailToAccount 里记录的账户:
//      for (auto& [email, acc] : emailToAccount) { ... }
//    ✓ 必须遍历所有账户 i ∈ [0, n)，因为有些账户可能没触发 union
//      但自身邮箱仍需收集
//
// 3. ✗ 忘记排序输出邮箱（题目明确要求 ASCII 排序）
//    ✓ 用 set<string> 自动排序，或最后手动 sort
//
// 4. ✗ find 函数没有路径压缩:
//      int find(int x) { return parent[x] == x ? x : find(parent[x]); }
//    ✓ 加路径压缩:
//      int find(int x) { if(parent[x]!=x) parent[x]=find(parent[x]); return parent[x]; }
//    不压缩虽然正确，但最坏退化到 O(N) 每次查询
//
// 5. ✗ 在 unite 中忘记先 find 再比较:
//      void unite(int x, int y) { parent[x] = y; }
//    ✓ 必须比较根节点:
//      void unite(int x, int y) { int px=find(x),py=find(y); if(px!=py) parent[py]=px; }
//
// 【面试追问 — 递进链】
//
// Q1: 暴力思路？
// → 对每对账户 (i,j) 检查是否有共同邮箱，O(N² × K)。
//   瓶颈：对比次数太多，且合并时需要处理传递性。
//
// Q2: 为什么用 Union-Find 而不是 DFS？
// → 都能解。Union-Find 是"边读入边合并"，不需要先把图建完。
//   DFS 需要先建完邻接表再遍历。面试中 UF 代码更短更清晰。
//
// Q3: 如果数据量极大（百万账户），怎么优化？
// → 给邮箱分配整数ID，UF用整数数组（避免字符串哈希）。
//   按秩合并+路径压缩，单次操作 O(α(N)) ≈ O(1)。
//   分布式场景可按邮箱前缀分片，片内独立合并，再跨片合并。
//
// Q4: 如果需要动态添加账户并实时查询"这个邮箱属于谁"？
// → Union-Find 天然支持在线合并。维护 emailToRoot 映射，
//   新账户来了直接 union。查询时 find 到根，根对应的名称就是答案。
// ============================================================
