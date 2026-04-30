/*
 * LeetCode 854: K-Similar Strings (相似度为 K 的字符串)
 * 难度: Hard
 * 标签: BFS, Hash Table, String
 *
 * 题目: 给定两个字母异位词 s1 和 s2 (仅含 {a,b,c,d,e,f}, 长度<=20),
 *       求最少交换次数使 s1 变成 s2
 *
 * 核心建模:
 *   每个字符串排列 = 图中的节点
 *   每次交换 = 一条边 (代价为1)
 *   求最少交换 = 无权图最短路径 → BFS
 *
 * 三个关键剪枝:
 *   1. 只修正第一个不匹配位 (减少等价路径)
 *   2. 只和 curr[j]==s2[i] 的位置交换 (保证有效)
 *   3. 只和 curr[j]!=s2[j] 的位置交换 (不破坏已匹配)
 */



// ============================================================
// 解法1: BFS + 贪心剪枝 (推荐)
// ============================================================
//
// BFS 状态图示例 (s1="abc", s2="bca"):
//
//   Layer 0:  "abc"
//              │  位置0: 'a'≠'b', 找 j 使 curr[j]='b' → j=1
//              └─ swap(0,1)
//   Layer 1:  "bac"
//              │  位置0: 'b'='b' ✓, 跳过
//              │  位置1: 'a'≠'c', 找 j 使 curr[j]='c' → j=2
//              └─ swap(1,2)
//   Layer 2:  "bca" == s2  ✓  return 2
//
// 时间: O(状态数 * n), 剪枝后远小于 O(n!)
// 空间: O(状态数)

class Solution {
public:
    int kSimilarity(string s1, string s2) {
        // 特判: 已经相等
        if (s1 == s2) return 0;

        int n = s1.size();
        unordered_set<string> visited;
        queue<string> q;

        q.push(s1);
        visited.insert(s1);
        int steps = 0;

        while (!q.empty()) {
            steps++;
            int layerSize = q.size();

            // 逐层遍历 (BFS 标准写法)
            while (layerSize--) {
                string curr = q.front();
                q.pop();

                // ---- 剪枝1: 找第一个不匹配位置 ----
                // 跳过已匹配的前缀, 保证搜索有序性
                int i = 0;
                while (i < n && curr[i] == s2[i]) i++;

                // ---- 剪枝2+3: 只和有效且错位的位置交换 ----
                for (int j = i + 1; j < n; j++) {
                    // curr[j] == s2[i]: j 位置的字符正好是 i 需要的
                    // curr[j] != s2[j]: j 位置本身也是错位的 (不破坏已匹配)
                    if (curr[j] != s2[i] || curr[j] == s2[j]) continue;

                    // 生成新状态
                    string next = curr;
                    swap(next[i], next[j]);

                    // 提前终止: 找到目标
                    if (next == s2) return steps;

                    // 去重入队
                    if (!visited.count(next)) {
                        visited.insert(next);
                        q.push(next);
                    }
                }
            }
        }

        // 对合法输入不会执行到这里
        return -1;
    }
};

/*
 * 详细 trace: s1 = "abac", s2 = "baca"
 *
 * Layer 0:
 *   curr = "abac"
 *   i=0: 'a' != 'b', 找 j>0 使 curr[j]=='b' 且 curr[j]!=s2[j]
 *     j=1: curr[1]='b'==s2[0]='b' ✓, curr[1]='b'!=s2[1]='a' ✓
 *       swap(0,1): "baac" → 加入队列
 *     j=2: curr[2]='a'!='b' → 跳过
 *     j=3: curr[3]='c'!='b' → 跳过
 *   steps = 1
 *
 * Layer 1:
 *   curr = "baac"
 *   i=0: 'b'='b' ✓ 跳过
 *   i=1: 'a'='a' ✓ 跳过
 *   i=2: 'a' != 'c', 找 j>2 使 curr[j]=='c' 且 curr[j]!=s2[j]
 *     j=3: curr[3]='c'==s2[2]='c' ✓, curr[3]='c'!=s2[3]='a' ✓
 *       swap(2,3): "baca" == s2! return 2  ✓
 */


// ============================================================
// 解法2: A* 搜索 (进阶优化)
// ============================================================
// 启发函数 h(state) = ceil(不匹配位置数 / 2)
// 一次最优交换最多同时修正2个位置, 所以 /2 是合法下界 (admissible)
// 优先队列按 f = g + h 排序, 优先展开"离目标更近"的状态
//
// 时间: 通常优于普通 BFS (展开更少状态)
// 空间: O(状态数)

class Solution_AStar {
public:
    int kSimilarity(string s1, string s2) {
        if (s1 == s2) return 0;

        int n = s1.size();

        // 启发函数: 不匹配位置数的一半 (上取整)
        auto heuristic = [&](const string& s) -> int {
            int mismatch = 0;
            for (int i = 0; i < n; i++) {
                if (s[i] != s2[i]) mismatch++;
            }
            return (mismatch + 1) / 2;
        };

        // 优先队列: (f=g+h, g, state), f 小的优先
        using State = tuple<int, int, string>;
        priority_queue<State, vector<State>, greater<State>> pq;

        unordered_map<string, int> dist;  // 到达每个状态的最短距离
        dist[s1] = 0;
        pq.push({heuristic(s1), 0, s1});

        while (!pq.empty()) {
            auto [f, g, curr] = pq.top();
            pq.pop();

            // 已有更短路径到达此状态, 跳过
            if (dist.count(curr) && dist[curr] < g) continue;

            // 找第一个不匹配位置
            int i = 0;
            while (i < n && curr[i] == s2[i]) i++;

            // 枚举有效交换
            for (int j = i + 1; j < n; j++) {
                if (curr[j] != s2[i] || curr[j] == s2[j]) continue;

                string next = curr;
                swap(next[i], next[j]);

                int newG = g + 1;

                if (next == s2) return newG;

                if (!dist.count(next) || newG < dist[next]) {
                    dist[next] = newG;
                    int newF = newG + heuristic(next);
                    pq.push({newF, newG, next});
                }
            }
        }

        return -1;
    }
};


// ============================================================
// 边界测试用例
// ============================================================
/*
 * s1="ab",  s2="ba"  → 1 (一次交换)
 * s1="abc", s2="bca" → 2 (两次交换: abc→bac→bca)
 * s1="abc", s2="abc" → 0 (已相等)
 * s1="aabb", s2="bbaa" → 2
 * s1="abcdef", s2="fedcba" → 需要多步
 *
 * 同类 BFS 状态空间搜索题:
 *   127. Word Ladder — 每次改一个字母
 *   752. Open the Lock — 每次转一位
 *   773. Sliding Puzzle — 滑动拼图
 *   847. Shortest Path Visiting All Nodes — BFS + 位掩码
 *
 * 【易错点】
 * 1. 忘记 curr[j] != s2[j] 条件: 交换会破坏已匹配位，搜索空间膨胀导致 TLE。
 * 2. 没有固定第一个不匹配位: 枚举所有 (i,j) 对产生大量等价状态，TLE。
 * 3. 忘记 visited 去重: 同一状态反复入队导致 MLE/TLE。
 * 4. 用 DFS 而非 BFS: DFS 不能保证最短路径，求最少步骤必须用 BFS。
 * 5. 字符串比较开销: 每次 next==s2 比较是 O(n)，可用哈希优化但通常不必要。
 *
 * 【面试追问】
 * Q1: 为什么固定修正第一个不匹配位不会漏解?
 *     → 位置 i 迟早需要被修正(它和目标不同)。先修正它还是先修正别的位置，
 *       总步数不变(因为修正操作独立)。固定顺序只是消除等价搜索路径。
 *
 * Q2: A* 搜索的启发函数为什么选 ceil(不匹配数/2)?
 *     → 一次最优交换最多同时修正 2 个位置，所以不匹配数/2 是
 *       合法下界 (admissible)，不会高估，保证 A* 最优性。
 *
 * Q3: 如果字符集更大(如 26 个字母)，算法还可行吗?
 *     → 可行但状态空间增大。核心优化仍是三个剪枝。瓶颈是字符串长度
 *       而非字符集大小，长度 <= 20 时仍可通过。
 */
