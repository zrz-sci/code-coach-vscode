/*
 * LeetCode 752: 打开转盘锁 (Open the Lock)
 *
 * 【题目本质】
 * 把每个4位密码看作图上的节点，每次拨动一位轮盘是一条边。
 * 从 "0000" 到 target 的最少旋转次数 = 无权图最短路径 = BFS。
 *
 * 【解法总览】
 * 解法1: 单向 BFS            — O(10^4 × 4) / O(10^4) — 标准面试解法
 * 解法2: 双向 BFS            — O(10^4 × 4) / O(10^4) — 常数优化，加分项
 * 解法3: A* 搜索 (了解即可)   — 依赖启发函数 / O(10^4) — 进阶
 */

// ============================================================
// 解法1: 单向 BFS — 无权图最短路径的标准模板
// 时间: O(N × D) 其中 N = 10^4 (状态总数)，D = 4 (每个状态的维度)
// 空间: O(N) visited 集合
//
// 【思路】
// 从 "0000" 出发，每个状态有 4×2 = 8 个邻居（4个轮盘各±1）。
// BFS 逐层扩展，第一次到达 target 时的层数即为答案。
// deadends 视为不可达节点，直接预先放入 visited。
//
// BFS 扩展过程:
//   层0: {"0000"}
//          ↓ 拨动4个轮盘各 ±1
//   层1: {"1000","9000","0100","0900","0010","0090","0001","0009"}
//          ↓ 排除 visited 和 deadends，继续扩展
//   层2: {...}
//          ↓
//   ...直到 target 出现
// ============================================================
class Solution1 {
public:
    int openLock(vector<string>& deadends, string target) {
        // 把 deadends 塞进 visited，一举两得：既标记死亡状态，又避免后续单独判断
        unordered_set<string> visited(deadends.begin(), deadends.end());
        
        // 边界1：起点 "0000" 本身是 deadend → 无法开始
        if (visited.count("0000")) return -1;
        // 边界2：起点就是目标 → 0步
        if (target == "0000") return 0;
        
        queue<string> q;
        q.push("0000");
        visited.insert("0000");
        int steps = 0;
        
        while (!q.empty()) {
            steps++;
            int sz = q.size(); // 当前层的节点数，逐层处理
            for (int i = 0; i < sz; i++) {
                string curr = q.front();
                q.pop();
                
                // 生成8个邻居：4个轮盘 × 2个方向(+1/-1)
                for (int j = 0; j < 4; j++) {
                    for (int d = -1; d <= 1; d += 2) {
                        string next = curr;
                        // +10 防止 C++ 负数取模的问题
                        // 例如 '0' - '0' + (-1) = -1, (-1)%10 在 C++ 中是 -1 而非 9
                        next[j] = (curr[j] - '0' + d + 10) % 10 + '0';
                        
                        // 第一次到达 target → BFS 保证是最短路径
                        if (next == target) return steps;
                        
                        if (!visited.count(next)) {
                            visited.insert(next);
                            q.push(next);
                        }
                    }
                }
            }
        }
        return -1; // 遍历完所有可达状态仍未到 target
    }
};

// ============================================================
// 解法2: 双向 BFS — 从两端同时搜索，相遇即停止
// 时间: O(N × D)，但实际搜索的状态远少于单向 BFS
// 空间: O(N)
//
// 【思路】
// 单向 BFS 从起点向外"扇形"扩展，搜索空间 ≈ 8^d (d 是深度)。
// 双向 BFS 从起点和终点同时搜索，各扩展 d/2 层就相遇：
//
//   单向:  起点 ──────────────────→ 终点    搜索空间 ≈ 8^d
//   双向:  起点 ──────→ ←────── 终点         搜索空间 ≈ 2 × 8^(d/2)
//
// 关键技巧：每轮扩展较小的那一端集合，保持两端规模平衡。
// 用 set 代替 queue，因为需要快速判断"是否在对面集合中"。
// ============================================================
class Solution2 {
public:
    int openLock(vector<string>& deadends, string target) {
        unordered_set<string> visited(deadends.begin(), deadends.end());
        
        if (visited.count("0000")) return -1;
        if (target == "0000") return 0;
        
        // front_set: 从起点扩展的前沿
        // back_set:  从终点扩展的前沿
        unordered_set<string> front_set, back_set;
        front_set.insert("0000");
        back_set.insert(target);
        // 两端的初始节点都标记为已访问
        visited.insert("0000");
        visited.insert(target);
        
        int steps = 0;
        
        while (!front_set.empty() && !back_set.empty()) {
            steps++;
            
            // 核心：总是扩展较小的集合 → 保持两端规模平衡
            // 这是双向 BFS 性能优势的关键
            if (front_set.size() > back_set.size()) {
                swap(front_set, back_set);
            }
            
            unordered_set<string> next_set; // 扩展后的新前沿
            
            for (const string& curr : front_set) {
                for (int j = 0; j < 4; j++) {
                    for (int d = -1; d <= 1; d += 2) {
                        string next = curr;
                        next[j] = (curr[j] - '0' + d + 10) % 10 + '0';
                        
                        // 邻居在对面的前沿中 → 两端相遇！
                        if (back_set.count(next)) return steps;
                        
                        if (!visited.count(next)) {
                            visited.insert(next);
                            next_set.insert(next);
                        }
                    }
                }
            }
            
            front_set = next_set; // 用新前沿替换旧前沿
        }
        
        return -1;
    }
};

// ============================================================
// 解法3: A* 搜索 (了解即可) — 启发式引导的 BFS
// 时间: 取决于启发函数质量，最优情况远好于普通 BFS
// 空间: O(N)
//
// 【思路】
// A* 搜索用 f(n) = g(n) + h(n) 选择下一个扩展的节点：
//   g(n) = 从起点到 n 的实际步数
//   h(n) = 从 n 到 target 的启发式估计（下界）
//
// 启发式函数 h(state):
//   h = Σ min(|si - ti|, 10 - |si - ti|)  对 i = 0..3
//   即每位数字到目标位的最小旋转距离之和。
//   这个 h 是 admissible 的（不高估实际距离），保证 A* 找到最优解。
//
// 例如 h("1234", "5678"):
//   位0: min(|1-5|, 10-4) = min(4,6) = 4
//   位1: min(|2-6|, 10-4) = min(4,6) = 4
//   位2: min(|3-7|, 10-4) = min(4,6) = 4
//   位3: min(|4-8|, 10-4) = min(4,6) = 4
//   h = 16
// ============================================================
class Solution3 {
public:
    int openLock(vector<string>& deadends, string target) {
        unordered_set<string> visited(deadends.begin(), deadends.end());
        
        if (visited.count("0000")) return -1;
        if (target == "0000") return 0;
        
        // 启发式函数：每位到 target 的最小旋转距离之和
        auto heuristic = [&](const string& s) -> int {
            int h = 0;
            for (int i = 0; i < 4; i++) {
                int diff = abs(s[i] - target[i]);
                h += min(diff, 10 - diff); // 正转和反转取较小值
            }
            return h;
        };
        
        // 优先队列：(f = g + h, g, state)，f 最小的优先
        using State = tuple<int, int, string>; // (f, g, state)
        priority_queue<State, vector<State>, greater<State>> pq;
        
        pq.push({heuristic("0000"), 0, "0000"});
        visited.insert("0000");
        
        while (!pq.empty()) {
            auto [f, g, curr] = pq.top();
            pq.pop();
            
            // 到达目标
            if (curr == target) return g;
            
            for (int j = 0; j < 4; j++) {
                for (int d = -1; d <= 1; d += 2) {
                    string next = curr;
                    next[j] = (curr[j] - '0' + d + 10) % 10 + '0';
                    
                    if (!visited.count(next)) {
                        visited.insert(next);
                        int new_g = g + 1;
                        int new_f = new_g + heuristic(next);
                        pq.push({new_f, new_g, next});
                    }
                }
            }
        }
        
        return -1;
    }
};

// ============================================================
// 【解法对比】
//
// | 维度         | 单向 BFS        | 双向 BFS              | A* 搜索            |
// |-------------|-----------------|----------------------|-------------------|
// | 实现难度     | ⭐ 简单          | ⭐⭐ 中等             | ⭐⭐⭐ 较难         |
// | 实际性能     | 可能遍历大量状态   | 搜索空间指数级缩小      | 启发函数好时最快     |
// | 面试推荐     | 首选，必须能写    | 追问优化时写           | 了解即可           |
// | 数据结构     | queue + set     | 两个 set              | priority_queue+set |
//
// 面试策略：先写单向 BFS 拿下基本分，面试官问优化再说双向 BFS。
//
// ============================================================
// 【易错点】
//
// 1. C++ 负数取模问题:
//    ✗ next[j] = (curr[j]-'0' + d) % 10 + '0'
//      当 curr[j]='0', d=-1 时: (-1) % 10 = -1 (C++), 得到字符 '/' 而非 '9'
//    ✓ next[j] = (curr[j]-'0' + d + 10) % 10 + '0'
//      +10 保证被除数非负: (0 - 1 + 10) % 10 = 9 ✓
//
// 2. 忘记判断 "0000" 在 deadends 中:
//    ✗ 直接 q.push("0000") → 死亡状态被当作合法起点
//    ✓ if (visited.count("0000")) return -1; 提前返回
//
// 3. 忘记判断 target == "0000":
//    ✗ BFS 至少执行一步后才比较 → target="0000" 时错误返回 -1
//    ✓ if (target == "0000") return 0; 特判
//
// 4. 双向 BFS 中 visited 不共享:
//    ✗ front 和 back 各自维护 visited → 同一个状态可能被两端都扩展
//    ✓ 共享同一个 visited 集合，初始时 "0000" 和 target 都要 insert
//
// 5. 邻居检查顺序错误:
//    ✗ 先 insert visited 再判断是否在 back_set 中 → 相遇点被 visited 拦截
//    ✓ 先判断 back_set.count(next)，再判断 visited
//
// ============================================================
// 【面试追问】
//
// Q1: 为什么用 BFS 而不是 DFS？
// → 无权图最短路径。BFS 逐层扩展，保证第一次到达时路径最短。
//   DFS 可能找到路径但不是最短的，需要遍历所有路径取最小值，
//   在 10000 个状态的图上效率极差。
//
// Q2: 单向 BFS 最坏访问多少状态？双向 BFS 为什么更快？
// → 最坏 10000 个状态。双向 BFS 从两端搜索，各自只走一半深度。
//   如果最短路径长度为 d，单向搜索空间 ~8^d，双向 ~2×8^(d/2)。
//   例如 d=10: 8^10 ≈ 10^9 vs 2×8^5 ≈ 65536，差几个数量级。
//
// Q3: 如果锁有 n 位，每位 k 种字符，复杂度？
// → 状态空间 k^n，每个状态 2n 个邻居。BFS: O(k^n × n)。
//   当 n 和 k 较大时状态爆炸，需要 A*/IDA* 等启发式搜索。
//
// Q4: A* 的启发函数怎么设计？为什么是 admissible 的？
// → h(s) = Σ min(|si-ti|, k-|si-ti|)。
//   每位至少要转 min(顺转,逆转) 次，而且位与位之间独立（每次只转一位），
//   所以 h 是实际距离的下界，即 admissible。
// ============================================================
