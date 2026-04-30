/*
 * LeetCode 332: 重新安排行程 (Reconstruct Itinerary)
 *
 * 【题目本质】
 * 在有向图中找从 JFK 出发、经过每条边恰好一次的欧拉路径，且要字典序最小。
 *
 * 【解法总览】
 * 解法1: 回溯搜索       — O(E!) / O(E)     — 暴力，理解题意
 * 解法2: Hierholzer(multiset,递归) — O(E log E) / O(E) — 面试首选
 * 解法3: Hierholzer(priority_queue) — O(E log E) / O(E) — 代码更短
 * 解法4: Hierholzer(迭代)     — O(E log E) / O(E) — 避免栈溢出
 *
 * 【Hierholzer 算法在示例2 上的执行过程】
 *
 *   图:
 *     JFK ──→ ATL (字典序小, 先选)
 *     JFK ──→ SFO
 *     ATL ──→ JFK
 *     ATL ──→ SFO
 *     SFO ──→ ATL
 *
 *   邻接表 (multiset, 有序):
 *     JFK: {ATL, SFO}
 *     ATL: {JFK, SFO}
 *     SFO: {ATL}
 *
 *   DFS 执行过程:
 *     dfs("JFK")
 *       取 ATL, 删除 JFK→ATL, 剩余 JFK:{SFO}
 *       dfs("ATL")
 *         取 JFK, 删除 ATL→JFK, 剩余 ATL:{SFO}
 *         dfs("JFK")
 *           取 SFO, 删除 JFK→SFO, 剩余 JFK:{}
 *           dfs("SFO")
 *             取 ATL, 删除 SFO→ATL, 剩余 SFO:{}
 *             dfs("ATL")
 *               取 SFO, 删除 ATL→SFO, 剩余 ATL:{}
 *               dfs("SFO")
 *                 SFO 无出边 → result += "SFO"    result: [SFO]
 *               ATL 无出边 → result += "ATL"       result: [SFO, ATL]
 *             SFO 无出边 → result += "SFO"         result: [SFO, ATL, SFO]
 *           JFK 无出边 → result += "JFK"            result: [SFO, ATL, SFO, JFK]
 *         ATL 无出边 → result += "ATL"              result: [SFO, ATL, SFO, JFK, ATL]
 *       JFK 无出边 → result += "JFK"                result: [SFO, ATL, SFO, JFK, ATL, JFK]
 *
 *   翻转 → [JFK, ATL, JFK, SFO, ATL, SFO] ✓
 */

// ============================================================
// 解法1: 回溯搜索 — 暴力尝试所有可能的行程
// 时间: O(E!) 最坏  空间: O(E)
//
// 【思路】
// 最直觉的方法: 从 JFK 出发, 每次选一张没用过的机票飞.
// 为了字典序最小, 先对每个机场的目的地排序, 找到的第一个
// 合法行程就是答案.
// 
// 瓶颈: 当贪心走字典序最小目的地走进死胡同时, 必须回溯
// 撤销选择, 最坏情况指数级别.
// ============================================================
class Solution1 {
public:
    vector<string> findItinerary(vector<vector<string>>& tickets) {
        // 建邻接表
        unordered_map<string, vector<string>> graph;
        for (auto& t : tickets) {
            graph[t[0]].push_back(t[1]);
        }
        // 排序: 保证先尝试字典序小的目的地
        for (auto& [from, tos] : graph) {
            sort(tos.begin(), tos.end());
        }
        
        int n = tickets.size();
        vector<string> route = {"JFK"};
        
        // 记录邻接表中每个位置(即每张票)是否已用
        unordered_map<string, vector<bool>> used;
        for (auto& [from, tos] : graph) {
            used[from].assign(tos.size(), false);
        }
        
        function<bool()> backtrack = [&]() -> bool {
            // 终止条件: 用完所有票, 路径长度 = 票数 + 1
            if ((int)route.size() == n + 1) return true;
            
            string& curr = route.back();
            auto& neighbors = graph[curr];
            for (int i = 0; i < (int)neighbors.size(); i++) {
                if (used[curr][i]) continue;
                
                // 做选择: 使用这张票
                used[curr][i] = true;
                route.push_back(neighbors[i]);
                
                // 因为邻接表已排序, 第一个找到的就是字典序最小的
                if (backtrack()) return true;
                
                // 撤销选择
                route.pop_back();
                used[curr][i] = false;
            }
            return false;
        };
        
        backtrack();
        return route;
    }
};

// ============================================================
// 解法2: Hierholzer 算法 (multiset, 递归) — 欧拉路径经典算法
// 时间: O(E log E)  空间: O(E)
//
// 【思路】
// 解法1 的瓶颈是回溯. Hierholzer 算法利用欧拉路径的性质
// 完全避免回溯:
//
// 核心洞察: 当 DFS 走到一个没有出边的节点(死胡同)时,
// 这个节点一定是当前路径的终点. 把它放到结果末尾(后序位置),
// 然后回退继续处理. 最终翻转结果就是正确路径.
//
// 为什么用 multiset:
// - 自动按字典序排序(每次取 begin() 就是最小的)
// - 允许重复(同一航线可能有多张票)
// - 支持 O(log N) 删除(走过的边不能再走)
//
// 决策过程 (以示例2为例):
//
//   JFK: {ATL, SFO}  → 选 ATL(最小)
//    └→ ATL: {JFK, SFO} → 选 JFK(最小)
//        └→ JFK: {SFO} → 选 SFO(唯一)
//            └→ SFO: {ATL} → 选 ATL(唯一)
//                └→ ATL: {SFO} → 选 SFO(唯一)
//                    └→ SFO: {} → 死胡同! → 后序加入
//                ATL: {} → 死胡同! → 后序加入
//            SFO: {} → 后序加入
//        JFK: {} → 后序加入
//    ATL: {} → 后序加入
//   JFK: {} → 后序加入
//
//   后序结果: [SFO, ATL, SFO, JFK, ATL, JFK]
//   翻转:    [JFK, ATL, JFK, SFO, ATL, SFO] ✓
// ============================================================
class Solution2 {
public:
    vector<string> findItinerary(vector<vector<string>>& tickets) {
        // 建邻接表: multiset 天然有序 + 允许重复
        unordered_map<string, multiset<string>> graph;
        for (auto& t : tickets) {
            graph[t[0]].insert(t[1]);
        }
        
        vector<string> result;
        
        // Hierholzer DFS: 走到死胡同就后序加入结果
        function<void(const string&)> dfs = [&](const string& node) {
            // 只要还有出边就继续走
            while (!graph[node].empty()) {
                // 取字典序最小的目的地
                string next = *graph[node].begin();
                // 走过就删除(每张票只能用一次)
                graph[node].erase(graph[node].begin());
                dfs(next);
            }
            // 关键: 无出边时才加入结果(后序位置)
            // 这保证了死胡同节点被正确放到路径末尾
            result.push_back(node);
        };
        
        dfs("JFK");
        
        // 后序记录是逆序的, 翻转得到正序
        reverse(result.begin(), result.end());
        return result;
    }
};

// ============================================================
// 解法3: Hierholzer 算法 (priority_queue) — 用最小堆替代 multiset
// 时间: O(E log E)  空间: O(E)
//
// 【思路】
// 和解法2 逻辑完全相同, 只是数据结构换成了最小堆(priority_queue).
// priority_queue 的 top()/pop() 等价于 multiset 的 *begin()/erase(begin()).
// 代码更短一些.
// ============================================================
class Solution3 {
public:
    vector<string> findItinerary(vector<vector<string>>& tickets) {
        // 最小堆: greater<string> 让字典序小的在顶部
        unordered_map<string, priority_queue<string, vector<string>, greater<string>>> graph;
        for (auto& t : tickets) {
            graph[t[0]].push(t[1]);
        }
        
        vector<string> result;
        
        function<void(const string&)> dfs = [&](const string& node) {
            while (!graph[node].empty()) {
                string next = graph[node].top();
                graph[node].pop(); // 走过就删除
                dfs(next);
            }
            result.push_back(node); // 后序加入
        };
        
        dfs("JFK");
        reverse(result.begin(), result.end());
        return result;
    }
};

// ============================================================
// 解法4: Hierholzer 算法 (迭代版, 用栈模拟递归)
// 时间: O(E log E)  空间: O(E)
//
// 【思路】
// 用显式栈模拟递归调用栈. 核心逻辑:
// - 栈顶节点有出边: 走最小的那条边, 压入下一个节点(不弹出当前节点!)
// - 栈顶节点无出边: 弹出并加入结果(相当于递归版的"后序加入")
//
// 关键细节: 有出边时不弹出当前节点, 因为回溯时还要检查它是否还有其他出边.
// 只有确认无出边后才弹出, 这和递归版中"while循环结束后才push_back"是一致的.
// ============================================================
class Solution4 {
public:
    vector<string> findItinerary(vector<vector<string>>& tickets) {
        unordered_map<string, multiset<string>> graph;
        for (auto& t : tickets) {
            graph[t[0]].insert(t[1]);
        }
        
        vector<string> result;
        stack<string> stk;
        stk.push("JFK");
        
        while (!stk.empty()) {
            string curr = stk.top();
            if (!graph[curr].empty()) {
                // 有出边: 取最小的, 删除, 压栈继续走
                // 注意: 不弹出 curr! 回溯时还要回到它
                string next = *graph[curr].begin();
                graph[curr].erase(graph[curr].begin());
                stk.push(next);
            } else {
                // 无出边(死胡同): 弹出并加入结果
                result.push_back(curr);
                stk.pop();
            }
        }
        
        // 和递归版一样, 后序结果需要翻转
        reverse(result.begin(), result.end());
        return result;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法 | 时间       | 空间 | 特点                    |
// |------|-----------|------|------------------------|
// | 回溯 | O(E!) 最坏 | O(E) | 直觉, 但有回溯开销       |
// | Hierholzer(multiset) | O(E log E) | O(E) | 面试首选, 优雅无回溯 |
// | Hierholzer(pq)       | O(E log E) | O(E) | 代码更短             |
// | Hierholzer(迭代)     | O(E log E) | O(E) | 无递归栈溢出风险       |
//
// 核心区别: 
//   回溯法 = 暴力尝试 + 撤销
//   Hierholzer = 利用欧拉路径性质, "后序加入 + 翻转" 避免一切回溯
//
// 面试中推荐解法2(multiset递归), 代码清晰易解释.
// 如果追问迭代写法, 给解法4.
//
// 【易错点】
//
// 1. ✗ 在 DFS 开头(前序)就加入结果:
//      result.push_back(node);  // 然后 while(...) dfs(next);
//    ✓ 必须在 DFS 结尾(后序, 即 while 循环结束后)才加入.
//    原因: 前序加入时, 当前节点后面的分支还没处理完,
//          会导致分支节点的顺序被打乱.
//
// 2. ✗ 用 set 而非 multiset:
//      set<string> 会对同一目的地去重, 丢失重复机票.
//    ✓ 用 multiset 或 priority_queue.
//
// 3. ✗ 遍历邻接表但不删除已走过的边:
//      for (auto& x : graph[node]) dfs(x);  // 会重复走!
//    ✓ 每走一条边必须立刻删除: erase(begin()) 或 pop().
//
// 4. ✗ 迭代版中有出边时弹出了栈顶:
//      stk.pop(); stk.push(next);  // 错! 回不来了
//    ✓ 有出边时只压入 next, 不弹出 curr.
//      只有无出边时才弹出 curr 并加入结果.
//
// 5. ✗ 忘记最后翻转 result:
//    Hierholzer 后序记录的是逆序, 必须 reverse.
//
// 【面试追问】
//
// Q1: 这道题的图论模型是什么？什么条件下保证有解？
//     → 有向图的欧拉路径. 存在条件: 图连通, 且最多一个节点
//       out-in=1(起点), 最多一个 in-out=1(终点), 其余 in=out.
//
// Q2: 为什么后序加入 + 翻转能得到正确的字典序最小路径？
//     → 因为我们总是贪心选字典序最小的边. 走到死胡同说明该节点
//       应排在路径末尾. 后序插入保证了: 越早走到的死胡同越靠后,
//       越晚处理的分支越靠前. 翻转后恰好是正确顺序.
//
// Q3: 如果数据量很大(百万级机票), multiset vs priority_queue 选哪个?
//     → priority_queue 更好. multiset 底层是红黑树, 内存碎片多,
//       缓存不友好. priority_queue 底层是 vector, 内存连续.
//       但两者渐进复杂度相同 O(E log E).
//
// Q4: 能否不用翻转, 直接正序构建结果?
//     → 可以用 deque, 每次 push_front 而不是 push_back.
//       但 push_front 在 vector 上是 O(N), 用 deque 虽然 O(1)
//       但常数大. 实际面试中 reverse 最简洁.
// ============================================================
