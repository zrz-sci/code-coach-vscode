/*
 * LeetCode 815: 公交路线 (Bus Routes)
 *
 * 【题目本质】
 * 给定多条公交线路（每条线路覆盖若干站点），求从 source 站到 target 站
 * 最少需要乘坐几趟公交车。本质是"线路级 BFS 求最短换乘次数"。
 *
 * 【解法总览】
 * 解法1: 线路级 BFS（显式建图）— O(N²×L) / O(N²+S) — 直觉清晰
 * 解法2: 站点 BFS + 线路扩展   — O(N×L) / O(N×L) — 面试首选 ⭐
 *
 * 其中 N = routes.length, L = 平均线路长度, S = 总站点数
 */

// ============================================================
// 解法1: 线路级 BFS（显式建图）
// 时间: O(N²×L)  空间: O(N² + S)
//
// 【思路】
// 将每条公交线路看作图中的一个节点。
// 如果两条线路共享至少一个站点，就在它们之间连一条边（可以换乘）。
// 从所有包含 source 的线路出发做 BFS，找到包含 target 的线路就停。
// BFS 的层数就是乘坐的公交车数量。
//
// 为什么要先排序？
// 排序后可以用双指针 O(L1+L2) 判断两条线路是否共享站点，
// 否则用暴力或 set 会更慢。
//
// BFS 过程示意:
//   routes = [[1,2,7], [3,6,7]]
//   线路0 和 线路1 共享站点7 → graph[0]={1}, graph[1]={0}
//
//   source=1 在线路0 → 起点 = {线路0}
//   target=6 在线路1 → 终点 = {线路1}
//
//   Layer1: 线路0 → 邻居 线路1 (在终点集合中) → 返回 2
// ============================================================
class Solution1 {
public:
    int numBusesToDestination(vector<vector<int>>& routes, int source, int target) {
        if (source == target) return 0;
        
        int n = routes.size();
        
        // 排序每条线路，方便双指针判断是否有公共站
        for (auto& r : routes) sort(r.begin(), r.end());
        
        // 建立线路间邻接表
        vector<vector<int>> graph(n);
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                if (hasCommon(routes[i], routes[j])) {
                    graph[i].push_back(j);
                    graph[j].push_back(i);
                }
            }
        }
        
        // 找出包含 source / target 的线路
        unordered_set<int> srcRoutes, tgtRoutes;
        for (int i = 0; i < n; i++) {
            if (binary_search(routes[i].begin(), routes[i].end(), source))
                srcRoutes.insert(i);
            if (binary_search(routes[i].begin(), routes[i].end(), target))
                tgtRoutes.insert(i);
        }
        
        // BFS
        queue<int> q;
        vector<bool> visited(n, false);
        for (int r : srcRoutes) {
            if (tgtRoutes.count(r)) return 1; // source 和 target 在同一线路
            q.push(r);
            visited[r] = true;
        }
        
        int buses = 1;
        while (!q.empty()) {
            buses++;
            int sz = q.size();
            while (sz--) {
                int cur = q.front(); q.pop();
                for (int nxt : graph[cur]) {
                    if (visited[nxt]) continue;
                    if (tgtRoutes.count(nxt)) return buses;
                    visited[nxt] = true;
                    q.push(nxt);
                }
            }
        }
        return -1;
    }
    
private:
    // 双指针判断两个已排序数组是否有公共元素
    bool hasCommon(const vector<int>& a, const vector<int>& b) {
        int i = 0, j = 0;
        while (i < (int)a.size() && j < (int)b.size()) {
            if (a[i] == b[j]) return true;
            else if (a[i] < b[j]) i++;
            else j++;
        }
        return false;
    }
};


// ============================================================
// 解法2: 站点 BFS + 线路扩展 — 面试首选 ⭐
// 时间: O(N×L)  空间: O(N×L)
//
// 【思路】
// 解法1 的瓶颈在于 O(N²) 的线路间建图。
// 观察：两条线路能换乘，是因为它们共享某个站点。
// 所以可以用 stopToRoutes[stop] = {route_list} 作为隐式图：
//   - 遍历当前线路的所有站点
//   - 对每个站点，找到经过该站的所有其他线路（就是可换乘线路）
//
// 核心思想："站点"是连接不同线路的桥梁。
// 通过 visitedStop 保证每个站点只用于发现新线路一次，
// 这使得总时间复杂度为 O(总站点数) 而非 O(N²)。
//
// BFS 扩展过程:
//   routes = [[1,2,7], [3,6,7]], source=1, target=6
//   stopToRoutes: {1:[0], 2:[0], 7:[0,1], 3:[1], 6:[1]}
//
//   初始: source=1 → 线路0 入队, buses=1
//   Layer1: 处理线路0
//     站1(已访问), 站2(新，无新线路), 站7(新，发现线路1→入队)
//   Layer2: 处理线路1, buses=2
//     站3(新), 站6 == target → 返回 2 ✓
// ============================================================
class Solution2 {
public:
    int numBusesToDestination(vector<vector<int>>& routes, int source, int target) {
        // 特殊情况: source 就是 target，不需要坐任何车
        if (source == target) return 0;
        
        int n = routes.size();
        
        // step1: 建立 站点 → 经过该站的线路列表
        // 这是整个算法的核心数据结构，相当于隐式建图
        unordered_map<int, vector<int>> stopToRoutes;
        for (int i = 0; i < n; i++) {
            for (int stop : routes[i]) {
                stopToRoutes[stop].push_back(i);
            }
        }
        
        // 如果 source 不在任何线路中，不可能到达
        if (stopToRoutes.find(source) == stopToRoutes.end()) return -1;
        
        // step2: BFS 初始化
        // 队列中放线路编号，BFS 每一层代表多坐一趟车
        vector<bool> visitedRoute(n, false);
        unordered_set<int> visitedStop;
        queue<int> q;
        
        // 所有经过 source 的线路是起始层（坐第1趟车）
        visitedStop.insert(source);
        for (int routeIdx : stopToRoutes[source]) {
            q.push(routeIdx);
            visitedRoute[routeIdx] = true;
        }
        
        int buses = 1; // 当前层代表坐了几趟车
        
        // step3: BFS 逐层扩展
        while (!q.empty()) {
            int sz = q.size();
            while (sz--) {
                int curRoute = q.front(); q.pop();
                
                // 遍历当前线路经过的所有站点
                for (int stop : routes[curRoute]) {
                    // 找到目标！当前 buses 就是答案
                    if (stop == target) return buses;
                    
                    // 该站已处理过 → 通过它能发现的线路已经入队了
                    if (visitedStop.count(stop)) continue;
                    visitedStop.insert(stop);
                    
                    // 通过这个站点发现可换乘的新线路
                    for (int nextRoute : stopToRoutes[stop]) {
                        if (visitedRoute[nextRoute]) continue;
                        visitedRoute[nextRoute] = true;
                        q.push(nextRoute);
                    }
                }
            }
            buses++; // 进入下一层 = 需要多坐一趟车
        }
        
        return -1; // 无法到达 target
    }
};


// ============================================================
// 【解法对比】
//
// 解法1 (显式建图):
//   优点 — 思路直观，图的结构明确
//   缺点 — O(N²) 建图，N=500 时需要 25万次线路比较
//   适用 — N 较小时
//
// 解法2 (隐式建图):
//   优点 — 总时间 O(总站点数)，不需要两两比较线路
//   缺点 — 需要理解"站点作为桥梁"的双层遍历
//   适用 — 通用，面试首选
//
// 两者的 BFS 核心逻辑相同（按线路层级扩展），
// 区别仅在于"发现可换乘线路"的方式：
//   解法1: 预处理邻接表 graph[cur] → next
//   解法2: 实时通过 stopToRoutes 发现 next
//
// ============================================================
// 【易错点】
//
// 1. 忘记 source == target 特判
//    ✗ 直接进 BFS，如果 source 不在任何线路中返回 -1（但答案是0）
//    ✓ 函数开头 if (source == target) return 0;
//
// 2. buses 计数错位
//    ✗ int buses = 0; 然后在 while 开头 buses++
//      导致处理第一层线路时 buses=1（看起来对），
//      但如果在入队新线路时就检查 target，buses 还没 +1
//    ✓ buses=1 从第一层开始，层结束后 buses++
//       在遍历站点时检查 stop==target，此时 buses 值正确
//
// 3. 不标记 visitedStop 导致 TLE
//    ✗ 只标记 visitedRoute，每个站点会被多条线路重复处理
//       最坏情况：所有线路共享同一个站点，每处理一条新线路
//       都要通过这个站再扫一遍所有线路 → O(N²)
//    ✓ visitedStop 确保每个站只用一次，总处理量 = O(总站点数)
//
// 4. 在解法1中用 routes[i].count(x) 判断元素是否存在
//    ✗ vector 没有 count 成员函数（不是 set）
//    ✓ 先排序，然后用 binary_search(begin, end, x)
//
// ============================================================
// 【面试追问】
//
// Q1: 为什么 BFS 能保证找到"最少乘坐公交车数量"？
// A1: BFS 逐层扩展，第 k 层处理的线路表示"恰好坐 k 趟车能到达"。
//     因为 BFS 先处理距离近的节点，第一次到达 target 的层数就是最短路径。
//     这和 BFS 在无权图上求最短路径的原理完全一样。
//
// Q2: visitedStop 的作用是什么？去掉会怎样？
// A2: visitedStop 避免同一个站点被多次用于"发现新线路"。
//     去掉后功能上仍然正确（visitedRoute 保证不重复入队），
//     但时间复杂度退化：热门换乘站被反复扫描，最坏变成 O(N × S)。
//
// Q3: 如果要输出具体的换乘方案（线路序列+换乘站点），怎么改？
// A3: 维护 parent[routeIdx] = {prevRoute, transferStop}。
//     BFS 入队时记录"从哪条线路、通过哪个站"到达当前线路。
//     找到 target 后沿 parent 链回溯，还原完整换乘路径。
//
// Q4: 如果线路有方向性（A→B→C 不等于 C→B→A），怎么处理？
// A4: 此题线路是循环的，无方向性。如果有方向，需要判断
//     "在线路上从某站是否能到达另一站"，即站点在线路中的顺序关系。
//     可以存储每条线路的有序站点列表，换乘时检查方向可达性。
// ============================================================
