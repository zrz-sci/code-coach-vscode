/*
 * LeetCode 895: Maximum Frequency Stack (最大频率栈)
 *
 * 【题目本质】
 * 数据结构设计：维护一个栈，pop 时弹出频率最高的元素，
 * 频率相同时弹出最近 push 的。核心思想：按频率分桶，
 * 每个频率维护一个栈，pop 时从最高频率栈弹出。
 *
 * 【解法总览】
 * 解法1: 频率分桶      — O(1) push/pop  — 面试首选
 * 解法2: 优先队列      — O(logn) push/pop — 简洁备选
 */

// ============================================================
// 解法1: 频率分桶 — 按频率建栈组，O(1) 操作（⭐ 面试首选）
// push: O(1)  pop: O(1)  空间: O(n)
//
// 【数据结构】
// - freq: unordered_map<int, int>
//   记录每个值的当前频率。freq[val] = val 已被 push 的次数（减去被 pop 的次数）。
//
// - group: unordered_map<int, stack<int>>
//   频率分桶。group[f] 是一个栈，存储"当前频率为 f 时被 push 的元素"。
//   关键: 一个元素被 push 3 次后，它同时存在于 group[1], group[2], group[3] 中！
//
// - maxFreq: int
//   当前所有元素中的最大频率。pop 时直接从 group[maxFreq] 弹出。
//
// 【push(val) 流程】
// 1. freq[val]++                         // 频率加 1
// 2. group[freq[val]].push(val)          // 压入新频率对应的栈
// 3. maxFreq = max(maxFreq, freq[val])   // 更新最大频率
//
// 【pop() 流程】
// 1. val = group[maxFreq].top()          // 取最高频率栈的栈顶
// 2. group[maxFreq].pop()                // 弹出
// 3. freq[val]--                         // 频率减 1
// 4. if group[maxFreq] 为空: maxFreq--   // 降级
// 5. return val
//
// 【关键洞察: 为什么 maxFreq 只减 1 就够了？】
// 设弹出的元素 val 之前频率为 f = maxFreq。
// val 在频率从 f-1 升到 f 时，被 push 到了 group[f-1] 和 group[f]。
// 弹出 val 后 group[f] 可能为空，但 val 在 group[f-1] 中的记录仍在。
// 因此 group[f-1] 一定非空，maxFreq 减 1 即可。
//
// 【完整示例】
//
// push(5): freq[5]=1, group[1]=[5],       maxFreq=1
// push(7): freq[7]=1, group[1]=[5,7],     maxFreq=1
// push(5): freq[5]=2, group[2]=[5],       maxFreq=2
// push(7): freq[7]=2, group[2]=[5,7],     maxFreq=2
// push(4): freq[4]=1, group[1]=[5,7,4],   maxFreq=2
// push(5): freq[5]=3, group[3]=[5],       maxFreq=3
//
// 状态:
//   group[1] = [5, 7, 4]  (底→顶)
//   group[2] = [5, 7]
//   group[3] = [5]
//   maxFreq = 3
//
// pop(): group[3].top()=5, freq[5]=2, group[3]空 → maxFreq=2  → return 5
// pop(): group[2].top()=7, freq[7]=1, group[2]非空 → maxFreq=2 → return 7
// pop(): group[2].top()=5, freq[5]=1, group[2]空 → maxFreq=1   → return 5
// pop(): group[1].top()=4, freq[4]=0, group[1]非空 → maxFreq=1 → return 4
//
// 输出: [5, 7, 5, 4] ✓
// ============================================================
class FreqStack1 {
public:
    // val → 当前频率
    unordered_map<int, int> freq;
    // 频率 → 该频率的元素栈
    unordered_map<int, stack<int>> group;
    // 当前最大频率
    int maxFreq = 0;

    FreqStack1() {}

    void push(int val) {
        // 1. 增加频率
        int f = ++freq[val];
        // 2. 压入对应频率的栈
        group[f].push(val);
        // 3. 更新最大频率
        maxFreq = max(maxFreq, f);
    }

    int pop() {
        // 1. 从最高频率栈弹出栈顶元素
        int val = group[maxFreq].top();
        group[maxFreq].pop();
        // 2. 频率减 1
        freq[val]--;
        // 3. 如果该频率栈空了，降级
        if (group[maxFreq].empty()) {
            maxFreq--;
        }
        return val;
    }
};

// ============================================================
// 解法2: 优先队列 — max-heap 按 (频率, 时间戳) 排序
// push: O(log n)  pop: O(log n)  空间: O(n)
//
// 【思路】
// 用 max-heap 存储三元组 (频率, 时间戳, 值)。
// tuple 的默认比较是字典序:
//   - 先比频率: 频率大的优先
//   - 频率相同比时间戳: 时间戳大的（更近push的）优先
//
// push(val):
//   freq[val]++, 插入 (freq[val], timestamp++, val) 到堆
//
// pop():
//   堆顶元素即为答案，弹出后 freq[val]--
//
// 注意: 不需要删除堆中的旧记录，因为每次 push 创建新记录，
// pop 只弹出堆顶（频率最大、时间最近的那次 push）。
// 旧记录的频率较小，自然排在后面。
//
// 示例:
// push(5): heap = [(1,0,5)]
// push(7): heap = [(1,1,7), (1,0,5)]
// push(5): heap = [(2,2,5), (1,1,7), (1,0,5)]
// pop():   弹出 (2,2,5), freq[5]=1 → return 5
//          heap = [(1,1,7), (1,0,5)]
// pop():   弹出 (1,1,7), freq[7]=0 → return 7
// ============================================================
class FreqStack2 {
public:
    unordered_map<int, int> freq;
    // (频率, 时间戳, 值) 的大顶堆
    priority_queue<tuple<int, int, int>> pq;
    int timestamp = 0;

    FreqStack2() {}

    void push(int val) {
        freq[val]++;
        // 每次 push 都创建新的堆条目
        pq.push({freq[val], timestamp++, val});
    }

    int pop() {
        // 堆顶: 频率最大、时间最新的元素
        auto [f, t, val] = pq.top();
        pq.pop();
        freq[val]--;
        return val;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法       | push    | pop     | 空间  | 特点                |
// |-----------|---------|---------|------|---------------------|
// | 频率分桶   | O(1)   | O(1)   | O(n) | ⭐面试首选，最优     |
// | 优先队列   | O(logn) | O(logn) | O(n) | 代码更短，次优       |
//
// 面试建议:
//   1. 先分析 pop 的需求: 频率最高 → 最近 push 的
//   2. 提出优先队列方案（快速写出）
//   3. 分析瓶颈: push/pop 是 O(logn)，能否做到 O(1)？
//   4. 给出频率分桶方案（最优解）
//   5. 解释为什么 maxFreq 只减 1 是正确的（核心追问点）
//
// 【易错点】
// 1. pop 后忘记 freq[val]--:
//    ✗ 只从 group 弹出，freq 不变 → 下次 push(val) 时频率计算错误
//    ✓ pop 后必须 freq[val]--
//
// 2. maxFreq 不管是否为空都减:
//    ✗ 每次 pop 无条件 maxFreq-- → 可能跳过仍有元素的频率层
//    ✓ 只在 group[maxFreq] 为空时才 maxFreq--
//
// 3. push 时忘记 maxFreq = max(maxFreq, f):
//    ✗ maxFreq 不更新 → pop 时永远从旧的最大频率弹出
//    ✓ 每次 push 都更新 maxFreq
//
// 4. 优先队列方案中 pop 后试图删除堆中旧记录:
//    ✗ 在堆中搜索并删除旧条目 → 破坏了堆结构或 O(n)
//    ✓ 不需要删除: 旧条目频率较低，自然排在后面，不会影响结果
//
// 5. 频率分桶中 group 用 vector 而不是 map:
//    ✗ group 用 vector<stack<int>>，但不知道最大频率 → 可能越界
//    ✓ 用 unordered_map<int, stack<int>>，自动处理任意频率
//
// 【面试追问 Interview Follow-ups】
// Q1: 为什么 group[maxFreq-1] 一定非空？（关键证明）
// A1: 设刚弹出的元素 val 之前频率为 maxFreq = f。
//     val 的频率从 f-1 变到 f 时，val 被 push 到了 group[f]。
//     但那次 push 之前，val 也被 push 到了 group[f-1]（频率为 f-1 时的那次 push）。
//     group[f-1] 中 val 的记录没有被删除（只有 group[f] 中的被 pop 了）。
//     所以 group[f-1] 至少包含 val 的一条记录，一定非空。
//
// Q2: 空间复杂度能否优化？
// A2: O(n) 已是最优，因为需要存储所有 push 过的元素信息。
//     频率分桶方案中，总的 group 条目数 = 所有 push 次数 = O(n)。
//
// Q3: 如果 pop 后 freq[val] 变成 0，需要从 freq 中删除吗？
// A3: 不需要。freq[val]=0 表示 val 目前不在栈中，
//     下次 push(val) 时 freq[val] 自然从 0 变成 1。
//     保留不影响正确性，删除也可以（节省常数空间）。
// ============================================================
