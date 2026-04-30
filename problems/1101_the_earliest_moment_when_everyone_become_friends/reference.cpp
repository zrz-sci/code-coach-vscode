/*
 * LeetCode 1101: 彼此熟识的最早时间 (The Earliest Moment When Everyone Become Friends)
 *
 * 【题目本质】
 * 按时间顺序逐条处理"加边"操作，求所有 n 个节点第一次全连通的时刻。
 * 经典的"动态连通性"问题 → 并查集。
 *
 * 【解法总览】
 * 解法1: 暴力 BFS — O(m×(n+m)) / O(n+m) — 每加一条边就 BFS 检查连通性
 * 解法2: 排序 + 并查集 — O(m log m + m α(n)) / O(n) — 增量维护连通分量数 ⭐
 */

// ============================================================
// 解法1: 暴力 BFS — 每加一条边后检查全图连通性
// 时间: O(m × (n + m))  空间: O(n + m)
//
// 【思路】
// 最直觉的做法：把日志按时间排序，逐条加边到邻接表。
// 每加一条边后，从节点 0 出发 BFS，看能否访问到所有 n 个节点。
// 如果能 → 返回当前时间戳；全部处理完仍不能 → 返回 -1。
//
// 【为什么这样做？】
// "所有人都认识" = 图只有1个连通分量 = 从任意节点出发能到达所有节点。
// BFS 是最直接的检测方式。
//
// 【瓶颈】
// 每加一条边都要 O(n+E) 做一次 BFS，而边数 E 会累积到 m。
// 总计 O(m × (n+m))，大量重复遍历。
// ============================================================

// 合并过程可视化（示例1, n=6）:
//
// 初始: {0} {1} {2} {3} {4} {5}        分量数=6
//
// t=20190101: 加边(0,1) → BFS from 0: 访问 {0,1}，count=2 ≠ 6
// t=20190104: 加边(3,4) → BFS from 0: 访问 {0,1}，count=2 ≠ 6
// t=20190107: 加边(2,3) → BFS from 0: 访问 {0,1}，count=2 ≠ 6
// t=20190211: 加边(1,5) → BFS from 0: 访问 {0,1,5}，count=3 ≠ 6
// t=20190224: 加边(2,4) → BFS from 0: 访问 {0,1,5}，count=3 ≠ 6
// t=20190301: 加边(0,3) → BFS from 0: 访问 {0,1,2,3,4,5}，count=6 = n → 返回!

class Solution1 {
public:
    int earliestAcq(vector<vector<int>>& logs, int n) {
        sort(logs.begin(), logs.end());

        vector<vector<int>> adj(n);

        for (auto& log : logs) {
            int t = log[0], x = log[1], y = log[2];
            adj[x].push_back(y);
            adj[y].push_back(x);

            // BFS 检查是否从节点0可达所有节点
            vector<bool> visited(n, false);
            queue<int> q;
            q.push(0);
            visited[0] = true;
            int count = 1;

            while (!q.empty()) {
                int cur = q.front(); q.pop();
                for (int nei : adj[cur]) {
                    if (!visited[nei]) {
                        visited[nei] = true;
                        count++;
                        q.push(nei);
                    }
                }
            }

            if (count == n) return t;
        }

        return -1;
    }
};

// ============================================================
// 解法2: 排序 + 并查集 (Union-Find) — 面试首选 ⭐
// 时间: O(m log m + m α(n)) ≈ O(m log m)  空间: O(n)
//
// 【思路】
// 解法1的瓶颈：每次加边后都要 O(n+E) 检查连通性，无法利用之前的结果。
//
// 并查集能在近 O(1) 时间完成：
//   - union(x, y)：合并 x 和 y 所在的集合
//   - find(x)：找到 x 所在集合的代表元
//
// 关键优化：维护一个 components 计数器（初始为 n）。
// 每次 union 成功（两人原本不在同一集合），components 减 1。
// 当 components == 1 时，所有人在同一集合 → 返回当前时间戳。
//
// 【为什么要排序？】
// 我们要找"最早"时间，必须按时间从小到大处理。
// 第一次 components 降到 1 的时刻就是答案。
//
// 【并查集两个优化】
// 1. 路径压缩 (find 时把路径上的节点直接挂到根)
//    → 使后续 find 更快
// 2. 按秩合并 (矮树挂到高树上)
//    → 防止树退化成链
// 两个优化一起使得每次操作 O(α(n))，α(n) < 5 对任何实际输入。
// ============================================================

// 并查集树结构变化可视化（示例1）:
//
// 初始 parent: [0,1,2,3,4,5]  （每人是自己的根）
//
// union(0,1): parent[1]=0     树: 0←1
// union(3,4): parent[4]=3     树: 3←4
// union(2,3): parent[3]=2     树: 2←3←4  (路径压缩后4也直接指向2)
// union(1,5): parent[5]=0     树: 0←1, 0←5
// union(2,4): find(2)=2, find(4)=2 → 同一集合，跳过
// union(0,3): find(0)=0, find(3)=2 → parent[2]=0
//   现在所有节点的根都是 0 → components=1 → 返回 20190301

class Solution2 {
public:
    int earliestAcq(vector<vector<int>>& logs, int n) {
        // 按时间戳排序（vector 默认按首元素排序）
        sort(logs.begin(), logs.end());

        // 并查集初始化
        parent.resize(n);
        rank_.resize(n, 0);
        for (int i = 0; i < n; i++) parent[i] = i;

        int components = n;

        for (auto& log : logs) {
            int t = log[0], x = log[1], y = log[2];

            int rx = find(x);
            int ry = find(y);

            if (rx != ry) {
                // 按秩合并：将矮树挂到高树上
                // 为什么？防止树退化成链，保持 find 高效
                if (rank_[rx] < rank_[ry]) swap(rx, ry);
                parent[ry] = rx;
                if (rank_[rx] == rank_[ry]) rank_[rx]++;

                components--;

                // 提前终止：所有人已连通
                if (components == 1) return t;
            }
            // 如果 rx == ry，说明 x 和 y 已经认识，不做任何操作
        }

        // 处理完所有日志仍有多个分量 → 不可能全部认识
        return -1;
    }

private:
    vector<int> parent;
    vector<int> rank_;

    // 路径压缩：递归找根，同时把路径上所有节点直接指向根
    // 为什么这样做？下次 find 同一节点时直接 O(1) 返回
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]); // 路径压缩的关键一行
        }
        return parent[x];
    }
};

// ============================================================
// 解法2b: 并查集（迭代版 find）— 与解法2逻辑完全相同
// 有些面试官不喜欢递归，这里给出迭代写法
// ============================================================
class Solution2b {
public:
    int earliestAcq(vector<vector<int>>& logs, int n) {
        sort(logs.begin(), logs.end());

        vector<int> parent(n), rank_(n, 0);
        for (int i = 0; i < n; i++) parent[i] = i;

        int components = n;

        for (auto& log : logs) {
            int t = log[0], x = log[1], y = log[2];

            int rx = find(parent, x);
            int ry = find(parent, y);

            if (rx != ry) {
                if (rank_[rx] < rank_[ry]) swap(rx, ry);
                parent[ry] = rx;
                if (rank_[rx] == rank_[ry]) rank_[rx]++;
                if (--components == 1) return t;
            }
        }
        return -1;
    }

private:
    // 迭代版路径压缩（两遍扫描法）
    int find(vector<int>& parent, int x) {
        // 第一遍：找到根
        int root = x;
        while (root != parent[root]) root = parent[root];
        // 第二遍：路径压缩，把路径上所有节点直接指向根
        while (x != root) {
            int next = parent[x];
            parent[x] = root;
            x = next;
        }
        return root;
    }
};

// ============================================================
// 【解法对比】
//
// | 维度       | 暴力 BFS              | 排序 + 并查集          |
// |------------|----------------------|----------------------|
// | 时间       | O(m × (n+m))          | O(m log m + m α(n))   |
// | 空间       | O(n+m) 邻接表         | O(n) parent+rank      |
// | 核心思想    | 每次重新检查连通性      | 增量维护连通分量数      |
// | 面试推荐    | 说思路展示理解          | 必须能写出来 ⭐        |
//
// 并查集之所以高效，是因为它"记住"了之前的合并结果，
// 而 BFS 每次都从零开始检查。
//
// ============================================================
// 【易错点】
//
// 1. 忘记排序：
//    ✗ 直接遍历 logs → 可能先处理后发生的事件
//    ✓ sort(logs.begin(), logs.end()) 必须放在最前面
//
// 2. find 没做路径压缩：
//    ✗ int find(int x) { return parent[x] == x ? x : find(parent[x]); }
//      → 没有赋值回 parent[x]，树可能退化成链，find 变 O(n)
//    ✓ parent[x] = find(parent[x]); return parent[x];
//
// 3. union 时没检查 rx != ry 就减 components：
//    ✗ parent[ry] = rx; components--;  // 如果 rx==ry 就多减了
//    ✓ if (rx != ry) { parent[ry] = rx; components--; }
//
// 4. 按秩合并时 rank 更新条件写错：
//    ✗ 总是 rank_[rx]++  → rank 虚高
//    ✓ 只在 rank_[rx] == rank_[ry] 时才 rank_[rx]++
//       因为只有等高合并才会增加树高
//
// ============================================================
// 【面试追问 — 递进链】
//
// Q1: "这道题为什么不能用 DFS 或 BFS？"
//   → 可以用（解法1），但每次加边都要 O(n+E) 重新检查。
//     并查集的优势是"增量维护"，每次操作近 O(1)。
//
// Q2: "并查集的路径压缩和按秩合并分别有什么用？能只用一个吗？"
//   → 路径压缩使 find 路径变短（摊销近 O(1)）。
//     按秩合并防止树退化成链。
//     只用路径压缩就已经足够高效（摊销 O(log n)），
//     两者结合达到 O(α(n))。面试中通常只写路径压缩也能过。
//
// Q3: "如果需要支持'取消友谊'（删边）怎么办？"
//   → 并查集不支持删除。两种方案：
//     a) 离线处理：确定最终哪些边会被删，反向处理（删变加）
//     b) 在线处理：用 Link-Cut Tree（Splay 维护），但实现复杂
//
// Q4: "如果 n 很大（10^6），logs 也很多（10^7），需要什么优化？"
//   → 排序是瓶颈 O(m log m)。并查集部分已经是 O(m α(n)) ≈ O(m)。
//     可考虑：基数排序把排序降到 O(m)；或如果时间戳已有序则省去排序。
//
// ============================================================
