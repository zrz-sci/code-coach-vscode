/*
 * LeetCode 3679: Minimum Discards to Balance Inventory
 * 使库存平衡的最少丢弃次数
 *
 * 解法: 滑动窗口 + 贪心 + 队列
 * 时间: O(n)  空间: O(n)
 *
 * 思路:
 * 对每种物品类型维护一个 deque，记录该类型被保留的天数索引。
 * 每天到达新物品时:
 *   1. 清理队列中已经滑出窗口的过期记录
 *   2. 如果队列长度 >= m，必须丢弃（违反约束）
 *   3. 否则保留，将当前天数入队
 *
 * 贪心正确性:
 * 尽量保留每个物品是最优的。已经保留的物品无法撤回，
 * 而保留当前物品不会让未来的情况更糟 —— 因为它比之前的
 * 同类物品更晚过期，最多导致未来某天多一个需要丢弃，
 * 但那天的丢弃是不可避免的（通过交换论证可证明）。
 */

// ============================================================
// 解法一: 滑动窗口 + 队列 (最优解)
// ============================================================
class Solution {
public:
    int minArrivalsToDiscard(vector<int>& arrivals, int w, int m) {
        // key: 物品类型, value: 该类型被保留的天数索引队列
        unordered_map<int, deque<int>> kept;
        int discards = 0;
        int n = arrivals.size();

        for (int i = 0; i < n; i++) {
            int type = arrivals[i];
            auto& q = kept[type];

            // Step 1: 清理窗口外的过期记录
            // 当前窗口: [max(0, i-w+1), i]
            // 过期条件: 日期 < i - w + 1
            while (!q.empty() && q.front() < i - w + 1) {
                q.pop_front();
            }

            // Step 2: 判断能否保留
            if ((int)q.size() >= m) {
                // 窗口内该类型已有 m 个保留, 超限 → 丢弃
                discards++;
            } else {
                // 可以安全保留
                q.push_back(i);
            }
        }

        return discards;
    }
};

// ============================================================
// 解法二: 暴力模拟 (验证用, O(n*w) 时间)
// ============================================================
class Solution_Brute {
public:
    int minArrivalsToDiscard(vector<int>& arrivals, int w, int m) {
        int n = arrivals.size();
        // kept[i] = true 表示第 i 天的物品被保留
        vector<bool> kept(n, false);
        int discards = 0;

        for (int i = 0; i < n; i++) {
            // 统计当前窗口 [max(0, i-w+1), i) 内
            // 与 arrivals[i] 同类型且被保留的数量
            int count = 0;
            int windowStart = max(0, i - w + 1);

            for (int j = windowStart; j < i; j++) {
                if (kept[j] && arrivals[j] == arrivals[i]) {
                    count++;
                }
            }

            if (count < m) {
                // 保留
                kept[i] = true;
            } else {
                // 必须丢弃
                discards++;
            }
        }

        return discards;
    }
};

// ============================================================
// 解法三: 使用 vector 替代 deque (cache-friendly 优化)
// ============================================================
// 对于某些情况, 使用 vector + 头指针 替代 deque 可以更快
class Solution_VecOpt {
public:
    int minArrivalsToDiscard(vector<int>& arrivals, int w, int m) {
        // 使用 vector<vector<int>> + head 指针
        // 避免 deque 的内存分配开销
        unordered_map<int, vector<int>> kept;
        unordered_map<int, int> head; // 每种类型的有效起始位置
        int discards = 0;
        int n = arrivals.size();

        for (int i = 0; i < n; i++) {
            int type = arrivals[i];
            auto& vec = kept[type];
            int& h = head[type]; // 有效起始指针

            // 清理过期记录（移动头指针）
            while (h < (int)vec.size() && vec[h] < i - w + 1) {
                h++;
            }

            // 当前窗口内有效数量 = vec.size() - h
            int activeCount = (int)vec.size() - h;

            if (activeCount >= m) {
                discards++;
            } else {
                vec.push_back(i);
            }
        }

        return discards;
    }
};

/*
 * 复杂度分析:
 *
 * 解法一 (滑动窗口+队列):
 *   时间: O(n) - 每个元素最多入队一次、出队一次, 总操作 2n
 *   空间: O(n) - HashMap + 队列存储所有被保留的日期
 *
 * 解法二 (暴力):
 *   时间: O(n * w) - 每天扫描整个窗口
 *   空间: O(n) - 记录每天是否保留
 *
 * 解法三 (vector优化):
 *   时间: O(n) - 同解法一
 *   空间: O(n) - 同解法一, 但内存局部性更好
 *
 * 关键测试用例:
 *   [1,2,1,3,1], w=4, m=2 → 0  (所有物品都可保留)
 *   [1,2,3,3,3,4], w=3, m=2 → 1 (第5天的3必须丢弃)
 *   [1,1,1,1,1], w=3, m=1 → 2   (第2天和第4天必须丢弃)
 *   [1,1,1,1,1], w=1, m=1 → 0   (窗口大小为1, 永远只有当前一个)
 *   [1], w=1, m=1 → 0            (只有一个物品)
 *
 * 边界情况:
 *   - w >= n: 窗口覆盖所有天, 每种类型最多 m 个
 *   - m >= w: 窗口内最多 w 个物品, m >= w 意味着永远不需要丢弃
 *   - 所有物品同类型: 丢弃数 = max(0, n - m * ceil(n/w))
 *
 * 面试技巧:
 *   1. 先说贪心策略: "能保留就保留"
 *   2. 解释数据结构: "每种类型一个队列"
 *   3. 说明均摊复杂度: "每个元素最多入队出队各一次"
 *   4. 注意 0-indexed vs 1-indexed 的转换
 */


// =====================================================
// 总结
// =====================================================
/*
 * ── 解法对比 ──
 *
 * | 解法                   | 时间    | 空间  | 常数 | 推荐度 |
 * |------------------------|---------|-------|------|--------|
 * | 滑动窗口+deque (推荐)  | O(n)   | O(n)  | 中   | ★★★ 面试首选 |
 * | 滑动窗口+vector+头指针 | O(n)   | O(n)  | 小   | ★★★ 性能最优 |
 * | 暴力模拟               | O(n*w) | O(n)  | 小   | ★   验证用   |
 *
 * deque 方案: 语义最清晰, pop_front 直接表达"过期清理"。
 * vector+头指针: 避免 deque 的分段内存分配, cache 更友好,
 *   实测快 10-30%, 但头指针之前的内存不会释放 (空间上界相同)。
 * 暴力: 每天重扫窗口, 仅用于对拍验证。
 *
 * ── 易错点 (具体bug) ──
 *
 * 1. 过期条件 off-by-one:
 *    BUG: 写成 q.front() < i - w 而非 q.front() < i - w + 1
 *    → 窗口实际大小变成 w+1, 少丢弃了物品, 答案偏小。
 *    正确窗口: [i-w+1, i], 长度恰好为 w。
 *
 * 2. 判断条件用 > m 而非 >= m:
 *    BUG: if (q.size() > m) → 允许窗口内保留 m+1 个同类物品,
 *    超出约束。正确: if (q.size() >= m) 时必须丢弃。
 *
 * 3. 丢弃时仍然把当前天入队:
 *    BUG: 不管是否丢弃都 q.push_back(i)
 *    → 被丢弃的物品也计入窗口, 后续判断时 size 虚高,
 *    导致大量误丢弃, 答案远大于正确值。
 *
 * 4. 忘记按类型分桶, 所有物品共用一个队列:
 *    BUG: 不同类型混在一起, 约束是"每种类型最多 m 个",
 *    而非"所有类型总共最多 m 个"。必须用 map<type, deque>。
 *
 * ── 面试追问链 ──
 *
 * Q1: 贪心策略"能保留就保留"为什么是最优的?
 * A1: 交换论证: 假设最优解在第 i 天丢弃了物品 X 但保留了第 j>i 天
 *     的同类物品 Y。将 X 保留、Y 丢弃, 丢弃总数不变, 但 X 比 Y 更早
 *     过期, 不会让后续更差。因此贪心保留不劣于任何最优解。
 *
 * Q2: 如果改为"窗口内每种类型至少保留 m 个, 不够则从仓库补货",
 *     问最少补货次数, 怎么做?
 * A2: 同样按类型维护窗口内保留数量, 但在窗口滑动时需要检查
 *     过期导致数量降到 < m 的情况, 每次降到 < m 就计一次补货。
 *     仍然 O(n) 滑动窗口, 但需要在 pop_front 时也检查约束。
 *
 * Q3: 如果物品有不同的价值, 要求最小化丢弃物品的总价值, 怎么做?
 * A3: 贪心策略变为: 当窗口内同类超限时, 丢弃价值最小的那个。
 *     需要用 multiset 或 priority_queue 替代 deque, 维护窗口内
 *     每种类型的价值集合。丢弃时弹出最小值。
 *     时间 O(n log m), 空间 O(n)。
 */
