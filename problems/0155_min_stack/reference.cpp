/*
 * LeetCode 155: 最小栈 (Min Stack)
 *
 * 【题目本质】
 * 普通栈的 push/pop/top 已经是 O(1)，难点在于让 getMin() 也是 O(1)。
 * 核心洞察：栈是 LIFO 结构，元素弹出顺序固定，因此可以为每个栈深度
 * 预先记录"当前状态的最小值"，pop 时自动恢复到上一个状态。
 *
 * 【解法总览】
 * 解法1: 暴力 getMin         — O(n) getMin / O(n) — 理解问题
 * 解法2: 双栈同步             — O(1) 全部 / O(n)  — 面试首选
 * 解法3: 单栈存 pair          — O(1) 全部 / O(n)  — 更简洁的写法
 * 解法4: 非严格递减辅助栈      — O(1) 全部 / O(n) 最坏, 平均更优 — 空间优化
 */

// ============================================================
// 解法1: 暴力 getMin — 每次遍历找最小值
// 时间: push/pop/top O(1), getMin O(n)
// 空间: O(n)
//
// 【思路】
// 最直觉的做法：push/pop/top 用标准栈操作，getMin 时遍历整个栈。
// 帮助我们理解瓶颈 — getMin 需要 O(n) 因为栈不知道最小值是谁。
// ============================================================
class MinStack1 {
    stack<int> stk;
public:
    MinStack1() {}

    void push(int val) {
        stk.push(val);
    }

    void pop() {
        stk.pop();
    }

    int top() {
        return stk.top();
    }

    int getMin() {
        // 瓶颈：必须遍历整个栈 — O(n)
        stack<int> tmp;
        int minVal = stk.top();
        while (!stk.empty()) {
            minVal = min(minVal, stk.top());
            tmp.push(stk.top());
            stk.pop();
        }
        // 倒回去恢复原栈
        while (!tmp.empty()) {
            stk.push(tmp.top());
            tmp.pop();
        }
        return minVal;
    }
};

// ============================================================
// 解法2: 双栈同步 — 辅助栈记录每层最小值 ⭐ 面试首选
// 时间: O(1) 所有操作
// 空间: O(n) — 辅助栈和主栈大小一致
//
// 【思路】
// 解法1的瓶颈: getMin 需要 O(n) 遍历。
// 突破方式: 用辅助栈 minStk 和主栈完全同步。
//   - push 时: minStk 压入 min(val, 当前最小值)
//   - pop  时: minStk 也同步弹出
//   - getMin: 直接读 minStk.top() → O(1)
//
// 为什么正确? 栈是 LIFO, 弹出顺序固定。minStk[i] 永远代表
// "当主栈有 i+1 个元素时, 所有元素的最小值"。pop 自动恢复。
//
// 操作过程示意:
//
// 操作          主栈 stk        辅助栈 minStk
// ──────────────────────────────────────────────
// push(-2)     [-2]            [-2]
// push(0)      [-2, 0]         [-2, -2]        ← min(0,-2) = -2
// push(-3)     [-2, 0, -3]     [-2, -2, -3]    ← min(-3,-2) = -3
// getMin()                      → -3 ✓
// pop()        [-2, 0]         [-2, -2]        ← 同步弹出
// top()         → 0 ✓
// getMin()                      → -2 ✓
// ============================================================
class MinStack2 {
    stack<int> stk;
    stack<int> minStk; // minStk.top() = 主栈所有元素中的最小值

public:
    MinStack2() {}

    void push(int val) {
        stk.push(val);
        // 关键: 辅助栈为空时直接压入, 否则取 min
        if (minStk.empty()) {
            minStk.push(val);
        } else {
            minStk.push(min(val, minStk.top()));
        }
    }

    void pop() {
        stk.pop();
        minStk.pop(); // 和主栈完全同步, 大小永远一致
    }

    int top() {
        return stk.top();
    }

    int getMin() {
        return minStk.top(); // O(1): 栈顶就是当前最小值
    }
};

// ============================================================
// 解法3: 单栈存 pair — 每个元素打包当时的最小值
// 时间: O(1) 所有操作
// 空间: O(n)
//
// 【思路】
// 和解法2本质完全一样, 只是把两个栈合并成一个栈,
// 每个元素是 {val, 截至此刻的最小值}。
// 代码更简洁, 面试中和解法2任选其一。
// ============================================================
class MinStack3 {
    // pair.first = 值, pair.second = 截至此刻的最小值
    stack<pair<int, int>> stk;

public:
    MinStack3() {}

    void push(int val) {
        if (stk.empty()) {
            stk.push({val, val});
        } else {
            // second 存的是 min(新值, 之前的最小值)
            stk.push({val, min(val, stk.top().second)});
        }
    }

    void pop() {
        stk.pop();
    }

    int top() {
        return stk.top().first;
    }

    int getMin() {
        return stk.top().second; // O(1)
    }
};

// ============================================================
// 解法4: 非严格递减辅助栈 — 空间优化
// 时间: O(1) 所有操作
// 空间: O(n) 最坏, 平均优于解法2
//
// 【思路】
// 解法2的辅助栈中有大量重复值。比如 push 了很多比当前最小值大的数,
// 辅助栈反复存同一个值。
//
// 优化: 辅助栈只在最小值"发生变化或相等"时才压入。
// - push: val <= minStk.top() 时才压入辅助栈
// - pop:  stk.top() == minStk.top() 时才弹出辅助栈
//
// ⚠️ 必须用 <= 不能用 <:
// 如果 push 了两次 -3, 辅助栈必须存两个 -3.
// 否则第一次 pop -3 后辅助栈就没有 -3 了, 但主栈里还有一个!
//
// 对比示意:
// 操作          主栈             辅助栈(非严格递减)
// ──────────────────────────────────────────────
// push(-2)     [-2]             [-2]
// push(0)      [-2, 0]          [-2]          ← 0 > -2, 不入
// push(-3)     [-2, 0, -3]      [-2, -3]      ← -3 <= -2, 入
// push(-3)     [-2, 0, -3, -3]  [-2, -3, -3]  ← -3 <= -3, 入 (<=!)
// pop(-3)      [-2, 0, -3]      [-2, -3]      ← -3 == -3, 同步弹
// pop(-3)      [-2, 0]          [-2]          ← -3 == -3, 同步弹
// getMin()                       → -2 ✓
// ============================================================
class MinStack4 {
    stack<int> stk;
    stack<int> minStk; // 非严格递减: 只存最小值变化的节点

public:
    MinStack4() {}

    void push(int val) {
        stk.push(val);
        // 只在 val <= 当前最小值时压入辅助栈
        // 注意: 必须是 <= 不是 <, 处理重复最小值
        if (minStk.empty() || val <= minStk.top()) {
            minStk.push(val);
        }
    }

    void pop() {
        // ⚠️ 必须先比较再弹出主栈!
        // 如果先 stk.pop() 再比较, 比较的就是下一个元素了
        if (stk.top() == minStk.top()) {
            minStk.pop();
        }
        stk.pop();
    }

    int top() {
        return stk.top();
    }

    int getMin() {
        return minStk.top();
    }
};

// ============================================================
// 【解法对比】
//
// | 解法 | getMin | 辅助空间 | 实现复杂度 | 面试推荐 |
// |------|--------|---------|-----------|---------|
// | 暴力 | O(n)   | 无      | 最简单     | 引出思路 |
// | 双栈同步 | O(1) | 2x主栈 | 简单     | ⭐首选  |
// | 单栈pair | O(1) | 2x主栈 | 最简洁   | 同等推荐 |
// | 递减辅助栈 | O(1) | ≤主栈 | 需注意<= | 追问优化 |
//
// 解法2 vs 解法3: 本质相同, 只是数据组织方式不同。pair 更简洁。
// 解法2 vs 解法4: 解法4节省了重复存储, 但最坏情况(递减序列)相同。
//
// 【易错点】
// 1. 辅助栈为空时访问 top():
//    ✗ minStk.push(min(val, minStk.top()))  ← 空栈 top() 未定义
//    ✓ if (minStk.empty()) minStk.push(val); else ...
//
// 2. 解法4 用 < 而不是 <=:
//    ✗ if (val < minStk.top()) minStk.push(val)
//      → push(-2), push(-2), pop() 后辅助栈没有 -2 了!
//    ✓ if (val <= minStk.top()) minStk.push(val)
//
// 3. 解法4 pop 顺序错误:
//    ✗ stk.pop(); if (stk.top() == minStk.top()) minStk.pop();
//      → 已经弹了, 比较的是下一个元素!
//    ✓ if (stk.top() == minStk.top()) minStk.pop(); stk.pop();
//
// 【面试追问】
// Q1(基础): 为什么一个变量记录最小值不够?
//    → pop 弹出最小值后无法恢复, 需要为每个栈状态记录最小值。
//
// Q2(优化): 辅助栈能否节省空间?
//    → 非严格递减辅助栈(解法4), 只在 val <= 最小值时压入。
//
// Q3(变体): 如果还要 O(1) getMax?
//    → 再加一个辅助栈维护最大值, 逻辑和 minStk 对称。
//
// Q4(进阶): 能否 O(1) 额外空间?
//    → 差值编码: 存 val-curMin, 负数说明产生了新最小值,
//      pop 时反推旧最小值。需用 long 防溢出。
// ============================================================
