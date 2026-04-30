/*
 * LeetCode 2092: 找出知晓秘密的所有专家
 *
 * 【题目本质】
 * 秘密按时间顺序通过会议传播，同一时刻的会议可以链式瞬时传播。
 * 本质是"按时间分批处理的连通性传播问题"。
 *
 * 【解法总览】
 * 解法1: 暴力模拟 — O(m² + m·log m) / O(n + m) — 最直觉但慢
 * 解法2: BFS 分组传播 — O(m·log m + m + n) / O(n + m) — 面试首选
 * 解法3: Union-Find 分组 + 重置 — O(m·log m + m·α(n)) / O(n) — UF 经典应用
 */

// ============================================================
// 解法1: 暴力模拟 — 按时间分组，每组反复扫描直到无新传播
// 时间: O(m² + m·log m)  空间: O(n)
//
// 【思路】
// 最直觉的做法：按时间排序，同一时刻的会议放一批，
// 反复扫描每批会议，如果双方有一个知道秘密就传给另一个。
// 重复直到该批没有新传播。
//
// 瓶颈：同一时刻如果会议形成长链 1-2-3-...-k，
// 每轮扫描只能传播一步，需要 O(k) 轮 × O(k) 扫描。
//
// 传播示意 (示例3, t=1):
//   会议: [1,2], [2,3]   已知: {1}
//   第1轮: 扫描[1,2]→2获得秘密, 扫描[2,3]→3获得秘密(如果先扫[2,3]则这轮没传到3)
//   可能需要第2轮: 扫描[2,3]→3获得秘密
//   第3轮: 无新传播，结束
// ============================================================
class Solution1 {
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
            // 找同一时间的所有会议 [i, j)
            while (j < m && meetings[j][2] == curTime) j++;
            
            // 反复扫描直到没有新传播 — 本质是低效的连通性搜索
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

// ============================================================
// 解法2: BFS 分组传播 — 按时间分组，每组建图 BFS 扩散
// 时间: O(m·log m + m + n)  空间: O(n + m)
//
// 【思路】
// 解法1的瓶颈是"反复扫描"来寻找连通性。
// 换个思路：对同一时刻的会议直接建邻接表，
// 从已知秘密的参会者出发做 BFS，一次性找到所有可达者。
//
// BFS 天然处理链式传播，不需要多轮扫描。
//
// 传播示意 (示例3, t=1):
//
//   同一时刻 t=1 的会议: [1,2], [2,3]
//   建图:  1 — 2 — 3
//
//   已知秘密的参会者: {1}
//   BFS:  
//     队列: [1]
//     弹出 1 → 邻居 2 入队, knows[2]=true
//     弹出 2 → 邻居 3 入队, knows[3]=true
//     弹出 3 → 无新邻居
//   结束. 一轮 BFS 搞定!
//
// 示例2 的关键 (t=2 不传播, t=3 传播):
//
//   t=2: 会议 [1,2]
//   建图: 1 — 2
//   已知秘密的参会者中在图里的: 无 (1不知道, 2不知道)
//   BFS 起点为空 → 无传播
//
//   t=3: 会议 [3,1], [0,3]
//   建图: 0 — 3 — 1
//   已知秘密的参会者: {0, 3}
//   BFS 从 {0, 3} 出发 → 到达 1
//   结果: 0, 1, 3 知道秘密 ✓
// ============================================================
class Solution2 {
public:
    vector<int> findAllPeople(int n, vector<vector<int>>& meetings, int firstPerson) {
        vector<bool> knows(n, false);
        knows[0] = true;
        knows[firstPerson] = true;
        
        // 排序：按时间升序，这样可以线性扫描并分组
        sort(meetings.begin(), meetings.end(), [](auto& a, auto& b) {
            return a[2] < b[2];
        });
        
        int m = meetings.size();
        int i = 0;
        while (i < m) {
            int curTime = meetings[i][2];
            int j = i;
            while (j < m && meetings[j][2] == curTime) j++;
            
            // 对本批次建邻接表（只包含本时刻的会议）
            unordered_map<int, vector<int>> adj;
            for (int k = i; k < j; k++) {
                int x = meetings[k][0], y = meetings[k][1];
                adj[x].push_back(y);
                adj[y].push_back(x);
            }
            
            // BFS 起点：本批次涉及的人中，已经知道秘密的
            queue<int> q;
            unordered_set<int> visited;
            for (auto& [node, _] : adj) {
                if (knows[node]) {
                    q.push(node);
                    visited.insert(node);
                }
            }
            
            // BFS 扩散
            while (!q.empty()) {
                int cur = q.front(); q.pop();
                for (int nei : adj[cur]) {
                    if (!visited.count(nei)) {
                        visited.insert(nei);
                        // 为什么在这里标记而不是最后统一标记？
                        // 因为这个人在后续批次中可能作为 BFS 起点
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

// ============================================================
// 解法3: Union-Find 分组 + 重置
// 时间: O(m·log m + m·α(n))  空间: O(n)
//
// 【思路】
// 同一时刻的"连通性"正是 Union-Find 的强项。
// 对每个时间批次：
//   1) union 所有该时刻的会议边
//   2) 检查哪些人和 person 0 同一分量 → 获得秘密
//   3) 不在 person 0 分量中的人 → reset parent 回自己
//
// 为什么需要 reset？
// 如果 t=2 时 A 和 B union 但都不知秘密，不 reset 的话，
// t=5 时如果 B 和知秘密的 C 开会，A 也会被错误连通。
// 但实际上 t=2 的会议不应该让 A 在 t=5 受益。
//
// Reset 示意 (示例2):
//
//   初始 UF: 0 和 3 连通. parent: [0,1,2,0,...]  (3→0)
//
//   t=2: 会议 [1,2]
//     union(1,2): parent: [0,1,1,0,...]  (2→1)
//     涉及的人: {1, 2}
//     find(1) = 1, find(0) = 0 → 1 和 0 不连通 → reset!
//     find(2) = 1, find(0) = 0 → 2 和 0 不连通 → reset!
//     reset 后: parent[1]=1, parent[2]=2
//     关键: 如果不 reset, 后面 union(3,1) 会导致 2 也连到 0
//
//   t=3: 会议 [3,1], [0,3]
//     union(3,1): 3→0, 1→1 → union(0,1) → parent[1]=0
//     union(0,3): 已经连通
//     涉及的人: {0, 1, 3}
//     find(0)=0=find(0) ✓, find(1)=0=find(0) ✓, find(3)=0=find(0) ✓
//     全部与 0 连通，不需要 reset
//
//   最终: find(p)==find(0) 的有: 0, 1, 3 → 答案 [0,1,3] ✓
// ============================================================
class Solution3 {
public:
    vector<int> parent, rnk;
    
    int find(int x) {
        // 路径压缩：让每个节点直接指向根
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    
    void unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return;
        // 按秩合并：矮树挂到高树下面
        if (rnk[px] < rnk[py]) swap(px, py);
        parent[py] = px;
        if (rnk[px] == rnk[py]) rnk[px]++;
    }
    
    vector<int> findAllPeople(int n, vector<vector<int>>& meetings, int firstPerson) {
        parent.resize(n);
        rnk.resize(n, 0);
        for (int i = 0; i < n; i++) parent[i] = i;
        
        // 初始: person 0 和 firstPerson 连通（时间 0 分享秘密）
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
            
            // 收集本批次涉及的所有人 + union
            vector<int> people;
            for (int k = i; k < j; k++) {
                int x = meetings[k][0], y = meetings[k][1];
                unite(x, y);
                people.push_back(x);
                people.push_back(y);
            }
            
            // Reset: 与 person 0 不连通的人，parent 归零
            // 为什么只遍历 people 而不是所有 n 个人？
            // 因为只有本批次参与的人可能被错误连接，
            // 其他人的状态不受本批次影响
            for (int p : people) {
                if (find(p) != find(0)) {
                    parent[p] = p;  // 断开连接
                    rnk[p] = 0;    // 秩也重置
                }
            }
            
            i = j;
        }
        
        // 最终检查：与 person 0 在同一连通分量的人都知道秘密
        vector<int> result;
        for (int p = 0; p < n; p++) {
            if (find(p) == find(0)) result.push_back(p);
        }
        return result;
    }
};

// ============================================================
// 【解法对比】
//
// 解法1 暴力: 简单直觉，但同一时刻链式传播需要多轮扫描 O(m²)
// 解法2 BFS:  一次 BFS 解决链式传播，直觉清晰，代码好写
//             每个时间批次建邻接表有额外空间开销
// 解法3 UF:   不需要建邻接表，空间更优，但 reset 逻辑是难点
//             适合熟练使用 Union-Find 的场景
//
// 面试推荐：解法2（BFS）最容易正确实现和清晰解释
// 如果面试官追问其他方法，给出解法3展示 UF 功底
//
// 【易错点】
// 1. Union-Find 忘记 reset:
//    ✗ union 完就不管了 → 跨时间的虚假连通导致错误传播
//    ✓ 每个时间批次处理完，遍历 people，不连通 0 的 reset
//
// 2. Reset 只重置 parent 忘了重置 rank:
//    ✗ parent[p]=p 但 rnk[p] 还是之前合并后的值
//    ✓ parent[p]=p; rnk[p]=0; 两个都要重置
//
// 3. BFS 起点遗漏:
//    ✗ 只检查会议的某一方是否知道秘密
//    ✓ 遍历邻接表所有 key（即本批次涉及的所有人），检查 knows[node]
//
// 4. 排序 lambda 的比较字段写错:
//    ✗ 按 meetings[i][0] 排序（按人排序）
//    ✓ 按 meetings[i][2] 排序（按时间排序）
//
// 5. Union-Find 中 find(p) != find(0) 误写为 parent[p] != parent[0]:
//    ✗ 没有路径压缩到根，parent 不一定直接指向根
//    ✓ 必须调用 find() 函数得到真正的根
//
// 【面试追问】
// Q1: 暴力法能不能通过？瓶颈在哪？
//     → 不能。同一时刻 O(m) 场会议形成链，需要 O(m) 轮扫描，
//       总体 O(m²) 在 m=10^5 时 TLE。瓶颈是"反复扫描找连通性"。
//
// Q2: Union-Find 为什么需要 reset？不 reset 会怎样？
//     → 不 reset 会导致跨时间的虚假连通。例如 t=2 时 A-B union
//       但都不知秘密，t=5 时 B 认识了知秘密的 C，A 会被错误传播。
//       时间是单向的，过去的无效会议不应该影响未来。
//
// Q3: 如果时间范围很小（如 ≤ 1000），能否避免排序？
//     → 可以用桶排序/counting sort 按时间分桶，O(m+T) 代替 O(m log m)。
//       T=max(time_i)。
//
// Q4: 如果不仅要知道谁知道秘密，还要输出每个人最早得知的时间？
//     → BFS 方案天然支持：在 knows[nei]=true 时同时记录 knowTime[nei]=curTime。
//       UF 方案需要额外维护一个数组记录时间。
// ============================================================
