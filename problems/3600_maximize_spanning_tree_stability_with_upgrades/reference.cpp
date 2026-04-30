/*
 * LeetCode 3600: 升级后最大生成树稳定性
 * Maximize Spanning Tree Stability with Upgrades
 *
 * 难度: Hard
 * 标签: Greedy, Union Find, Graph, Binary Search, Minimum Spanning Tree
 *
 * =============================================
 * 解法: 枚举候选阈值 + Union-Find (贪心验证)
 * =============================================
 *
 * 核心思路:
 *   "最大化最小边权" → 枚举/二分答案。
 *   设阈值 T, check(T) 判断能否构建稳定性 >= T 的生成树。
 *
 * check(T) 贪心策略:
 *   1. must 边: s < T → 不可行; 否则加入 UF
 *   2. optional 边 s >= T → 直接可用 (不消耗升级)
 *   3. optional 边 s < T 且 2*s >= T → 消耗一次升级
 *   4. 优先使用不需升级的边 (节省配额)
 *   5. 检查: 连通 + 升级次数 <= k
 *
 * 时间: O(E^2 * alpha(V)) -- 枚举法; O(E * logE * alpha(V)) -- 二分法
 * 空间: O(V + E)
 */
class Solution {
public:
    // ======== Union-Find 模板 ========
    vector<int> parent, rnk;

    void init(int n) {
        parent.resize(n);
        rnk.assign(n, 0);
        iota(parent.begin(), parent.end(), 0); // parent[i] = i
    }

    int find(int x) {
        // 路径分裂式压缩 (迭代, 比递归更快且无栈溢出风险)
        while (parent[x] != x) {
            parent[x] = parent[parent[x]]; // 跳过一层
            x = parent[x];
        }
        return x;
    }

    bool unite(int x, int y) {
        x = find(x);
        y = find(y);
        if (x == y) return false; // 已连通, 加此边会成环
        // 按秩合并: 矮树挂在高树下
        if (rnk[x] < rnk[y]) swap(x, y);
        parent[y] = x;
        if (rnk[x] == rnk[y]) rnk[x]++;
        return true; // 成功合并
    }

    int maxStability(int n, vector<vector<int>>& edges, int k) {
        // ========================================
        // Step 0: 预检查 must 边是否成环
        // ========================================
        // 这一步必须在最外层做，不能放在 check 内部
        init(n);
        for (auto& e : edges) {
            if (e[3] == 1) { // must 边
                if (!unite(e[0], e[1])) {
                    // must 边的两端已连通 → 加入会成环
                    // must 边又必须加入 → 矛盾 → 无合法生成树
                    return -1;
                }
            }
        }

        // ========================================
        // Step 1: 收集所有候选阈值
        // ========================================
        // 最优稳定性 = 某条选中边的 (原始或升级后) 强度
        // 所以答案一定在这些值中
        set<int> valSet;
        for (auto& e : edges) {
            valSet.insert(e[2]);             // 原始强度 s
            if (e[3] == 0) {
                valSet.insert(e[2] * 2);     // optional 边升级后 2*s
            }
        }

        // ========================================
        // Step 2: 定义 check(T) 函数
        // ========================================
        auto check = [&](int T) -> bool {
            // 每次 check 必须重新初始化 UF
            // (不同 T 下可用边集完全不同)
            init(n);
            int edgesUsed = 0;
            int upgrades = 0;

            // (a) 加入所有 must 边
            for (auto& e : edges) {
                if (e[3] == 1) {
                    if (e[2] < T) return false;
                    // must 边不能升级, 强度不够 → 此 T 不可行
                    unite(e[0], e[1]);
                    // must 边保证不成环 (Step 0 已验证)
                    edgesUsed++;
                }
            }

            // (b) 优先加入不需升级的 optional 边 (s >= T)
            // 贪心: 能免费用的先用, 节省升级配额
            for (auto& e : edges) {
                if (e[3] == 0 && e[2] >= T) {
                    if (unite(e[0], e[1])) {
                        edgesUsed++;
                    }
                }
            }

            // (c) 再加入需要升级的 optional 边 (s < T && 2*s >= T)
            // 这些边消耗升级配额
            for (auto& e : edges) {
                if (e[3] == 0 && e[2] < T && 2 * e[2] >= T) {
                    if (unite(e[0], e[1])) {
                        edgesUsed++;
                        upgrades++;
                    }
                }
            }

            // 检查: 边数够 (连通) + 升级数不超 k
            return edgesUsed >= n - 1 && upgrades <= k;
        };

        // ========================================
        // Step 3: 枚举所有候选值, 取最大可行值
        // ========================================
        // 为什么用枚举而非二分?
        // check(T) 的可行性不严格单调 (k 配额可能让中间某些值恰好可行)
        // 枚举更安全, 在 E <= 10^5 下可接受
        int ans = -1;
        for (int v : valSet) {
            if (check(v)) {
                ans = v; // 更新最大可行值
            }
        }
        return ans;
    }
};


/*
 * =============================================
 * 解法变体: 二分搜索候选值 (更快但需信任单调性)
 * =============================================
 *
 * 如果相信 check(T) 在候选值上大致单调递减,
 * 可以用二分搜索加速:
 */
class Solution_BinarySearch {
public:
    vector<int> parent, rnk;
    void init(int n) {
        parent.resize(n); rnk.assign(n, 0);
        iota(parent.begin(), parent.end(), 0);
    }
    int find(int x) {
        while (parent[x] != x) { parent[x] = parent[parent[x]]; x = parent[x]; }
        return x;
    }
    bool unite(int x, int y) {
        x = find(x); y = find(y);
        if (x == y) return false;
        if (rnk[x] < rnk[y]) swap(x, y);
        parent[y] = x;
        if (rnk[x] == rnk[y]) rnk[x]++;
        return true;
    }

    int maxStability(int n, vector<vector<int>>& edges, int k) {
        init(n);
        for (auto& e : edges)
            if (e[3] == 1)
                if (!unite(e[0], e[1])) return -1;

        set<int> valSet;
        for (auto& e : edges) {
            valSet.insert(e[2]);
            if (e[3] == 0) valSet.insert(e[2] * 2);
        }
        vector<int> cands(valSet.begin(), valSet.end()); // 升序

        auto check = [&](int T) -> bool {
            init(n);
            int used = 0, upgrades = 0;
            for (auto& e : edges)
                if (e[3] == 1) {
                    if (e[2] < T) return false;
                    unite(e[0], e[1]); used++;
                }
            for (auto& e : edges)
                if (e[3] == 0 && e[2] >= T)
                    if (unite(e[0], e[1])) used++;
            for (auto& e : edges)
                if (e[3] == 0 && e[2] < T && 2 * e[2] >= T)
                    if (unite(e[0], e[1])) { used++; upgrades++; }
            return used >= n - 1 && upgrades <= k;
        };

        // 二分: 找最大的 T 使 check(T) == true
        int lo = 0, hi = (int)cands.size() - 1, ans = -1;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (check(cands[mid])) {
                ans = cands[mid];
                lo = mid + 1;   // 尝试更大的
            } else {
                hi = mid - 1;   // 尝试更小的
            }
        }
        return ans;
    }
};


/*
 * =============================================
 * 面试追问
 * =============================================
 *
 * Q1: 枚举 vs 二分?
 *   枚举更安全 — check(T) 不严格单调 (k 配额可能使某些中间值恰好可行)
 *   二分更快但依赖单调性假设, 建议面试中先写枚举再提二分优化
 *
 * Q2: 贪心为什么优先免费边?
 *   交换论证: 把升级用在 s >= T 的边是浪费, 不如留给 s < T 的关键边
 *
 * Q3: must 边成环检测为什么放最外层?
 *   must 边成环与阈值 T 无关, 是全局不可解条件, 提前判断避免重复检测
 *
 * =============================================
 * 干跑验证
 * =============================================
 *
 * 示例 1: n=3, edges=[[0,1,2,1],[1,2,3,0]], k=1
 *   Step 0: must边 [0,1,2,1] → unite(0,1) OK. 无环.
 *   候选值: {2, 3, 6}
 *
 *   check(2): must边 s=2 >= 2 ✓, unite(0,1). optional边 s=3 >= 2 → unite(1,2).
 *             edgesUsed=2 >= 2, upgrades=0 <= 1 → true
 *   check(3): must边 s=2 < 3 → false
 *   check(6): must边 s=2 < 6 → false
 *
 *   最大可行值 = 2 ✓
 *
 * ─────────────────────────────────────────────
 *
 * 示例 2: n=3, edges=[[0,1,4,0],[1,2,3,0],[0,2,1,0]], k=2
 *   Step 0: 无 must 边. 无环.
 *   候选值: {1, 2, 3, 4, 6, 8}
 *
 *   check(1): 所有边 s >= 1, 免费选 [0,1](s=4) 和 [1,2](s=3), 连通.
 *             true, upgrades=0
 *   check(2): 同上. true
 *   check(3): [0,1] s=4>=3 免费, [1,2] s=3>=3 免费, [0,2] s=1<3 2*1=2<3 不可用
 *             edgesUsed=2, true
 *   check(4): [0,1] s=4>=4 免费, [1,2] s=3<4 2*3=6>=4 升级(1次), [0,2]不可用
 *             edgesUsed=2, upgrades=1<=2, true
 *   check(6): [0,1] s=4<6 2*4=8>=6 升级(1次), [1,2] s=3<6 2*3=6>=6 升级(2次)
 *             edgesUsed=2, upgrades=2<=2, true
 *   check(8): [0,1] s=4<8 2*4=8>=8 升级(1次), [1,2] s=3<8 2*3=6<8 不可用
 *             [0,2] s=1<8 不可用. edgesUsed=1 < 2 → false
 *
 *   最大可行值 = 6 ✓
 *
 * ─────────────────────────────────────────────
 *
 * 示例 3: n=3, edges=[[0,1,1,1],[1,2,1,1],[2,0,1,1]], k=0
 *   Step 0: must边 [0,1] unite OK. must边 [1,2] unite OK.
 *           must边 [2,0] → find(2)==find(0), 已连通 → 成环!
 *   return -1 ✓
 */

// 【易错点】
// ✗ 忘记检查 must-edge 是否成环 → 如果必选边本身有环，答案为 -1
// ✗ 二分边界设错 → lo 应为 0，hi 应为 max_weight，不是 n
// ✗ Union-Find 忘记路径压缩 → 超时
