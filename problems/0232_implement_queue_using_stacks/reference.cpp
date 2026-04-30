/*
 * LeetCode 232: 用栈实现队列 (Implement Queue using Stacks)
 *
 * 【题目本质】
 * 用两个 LIFO（后进先出）的栈，模拟一个 FIFO（先进先出）的队列。
 * 核心：两次"反转"等于"不反转"——元素从栈A倒到栈B，顺序就翻转了，
 * 恰好把 LIFO 变成了 FIFO。
 *
 * 【解法总览】
 * 解法1: 暴力搬运 — push O(n), pop/peek O(1), 空间 O(n)
 * 解法2: 延迟搬运(双栈分工) — push O(1), pop/peek 均摊 O(1), 空间 O(n) ⭐
 */

// ============================================================
// 解法1: 暴力搬运 — push 时来回倒，保证主栈栈顶=队头
// push: O(n)  pop/peek: O(1)  空间: O(n)
//
// 【思路】
// 最直觉的做法：让栈始终保持"栈顶=最早进入的元素（队头）"。
// 每次 push 新元素时:
//   1. 把主栈所有元素倒到临时栈（顺序翻转）
//   2. 新元素放入主栈（此时在最底部）
//   3. 把临时栈全部倒回主栈（顺序恢复，新元素在底部）
//
// 过程图示 (push 3 到 [1, 2]):
//
//  主栈 s1    临时 s2        操作           主栈 s1    临时 s2
//   [1]        (空)     → s1倒到s2 →        (空)       [2]
//   [2]                                                [1]
//
//                       → 3 放入 s1 →       [3]        [2]
//                                                      [1]
//
//                       → s2倒回s1 →        [1]        (空)
//                                           [2]
//                                           [3]
//   栈顶 = 1 = 队头 ✓
// ============================================================
class Solution1 {
public:
    class MyQueue {
        stack<int> s1, s2;
    public:
        MyQueue() {}

        void push(int x) {
            // 第一步：主栈 → 临时栈（清空主栈）
            while (!s1.empty()) {
                s2.push(s1.top());
                s1.pop();
            }
            // 第二步：新元素放入主栈底部
            s1.push(x);
            // 第三步：临时栈 → 主栈（恢复顺序，新元素在最底下）
            while (!s2.empty()) {
                s1.push(s2.top());
                s2.pop();
            }
        }

        int pop() {
            // 栈顶就是队头，直接弹出
            int front = s1.top();
            s1.pop();
            return front;
        }

        int peek() {
            return s1.top();
        }

        bool empty() {
            return s1.empty();
        }
    };
};

// ============================================================
// 解法2: 延迟搬运（双栈分工） — 均摊 O(1)  ⭐ 面试首选
// push: O(1)  pop/peek: 均摊 O(1)  空间: O(n)
//
// 【思路】
// 解法1 的瓶颈：每次 push 都要把所有元素来回搬运，大量重复劳动。
//
// 关键观察：不需要保持主栈有序！
// 用两个栈分工：
//   - inStack: 只负责接收 push，新元素直接入栈
//   - outStack: 只负责提供 pop/peek
//   - 只有当 outStack 空了，才把 inStack 一次性倒过来
//
// 为什么"倒一次"就够了？
//   inStack 里元素从底到顶是 [旧→新]
//   倒到 outStack 后从底到顶变成 [新→旧]
//   outStack 栈顶 = 最旧的元素 = 队头 ✓
//
// 完整过程图示:
//
//  push(1), push(2), push(3):
//    inStack     outStack
//      [3]        (空)
//      [2]
//      [1]
//
//  pop() → outStack 为空，触发搬运:
//    inStack     outStack
//     (空)         [1]  ← top = 队头
//                  [2]
//                  [3]
//  pop() 返回 1
//
//    inStack     outStack
//     (空)         [2]  ← top
//                  [3]
//
//  push(4):
//    inStack     outStack
//      [4]         [2]  ← top
//                  [3]
//
//  pop() → outStack 非空，不搬运！直接弹:
//  pop() 返回 2
//
//    inStack     outStack
//      [4]         [3]  ← top
//
//  pop() → outStack 非空，直接弹:
//  pop() 返回 3
//
//    inStack     outStack
//      [4]        (空)
//
//  pop() → outStack 为空，搬运:
//    inStack     outStack
//     (空)         [4]  ← top
//  pop() 返回 4
//
// 【均摊分析】
// 每个元素一生经历 4 次操作：
//   push 进 inStack (1次)
//   pop 出 inStack (1次, 搬运时)
//   push 进 outStack (1次, 搬运时)
//   pop 出 outStack (1次, 用户 pop 时)
// n 个元素总共 4n 次，均摊每次操作 O(1)。
// ============================================================
class Solution2 {
public:
    class MyQueue {
        stack<int> inStack;   // 输入栈：push 专用
        stack<int> outStack;  // 输出栈：pop/peek 专用

        // 惰性转移：只有输出栈空了才搬运
        // 为什么不能在 outStack 非空时搬运？
        // 因为 outStack 里现有元素的顺序是正确的（更早入队的在上面），
        // 如果此时把 inStack 的新元素倒进去，会插在老元素上面，
        // 导致新元素比老元素先出队 —— 违反 FIFO！
        void transfer() {
            if (outStack.empty()) {
                while (!inStack.empty()) {
                    outStack.push(inStack.top());
                    inStack.pop();
                }
            }
        }

    public:
        MyQueue() {}

        void push(int x) {
            // 新元素永远进输入栈，O(1)
            inStack.push(x);
        }

        int pop() {
            transfer();  // 确保输出栈有数据
            int front = outStack.top();
            outStack.pop();
            return front;
        }

        int peek() {
            transfer();  // 复用同样的搬运逻辑
            return outStack.top();
        }

        bool empty() {
            // 两个栈都空才是真正的空队列
            // 易错：只检查一个栈会导致误判
            return inStack.empty() && outStack.empty();
        }
    };
};

// ============================================================
// 【解法对比】
//
// | 操作      | 解法1(暴力搬运) | 解法2(延迟搬运) |
// |-----------|----------------|----------------|
// | push      | O(n) 每次倒两遍 | O(1) 直接入栈   |
// | pop/peek  | O(1)           | 均摊 O(1)       |
// | n次操作   | O(n²) 最坏     | O(n) 总共       |
// | 代码复杂度 | 简单           | 稍复杂(transfer)|
//
// 解法2 完美解决 Follow-up 的"均摊 O(1)"要求。
//
// 【易错点】
//
// 1. empty() 只检查一个栈:
//    ✗ return inStack.empty();
//      → 如果 inStack 空但 outStack 还有元素，误判为空
//    ✓ return inStack.empty() && outStack.empty();
//
// 2. transfer() 在 outStack 非空时也搬运:
//    ✗ void transfer() {  // 没有判断 outStack 是否为空
//          while (!inStack.empty()) { outStack.push(inStack.top()); ... }
//       }
//      → outStack 已有 [1,2]（1在顶），inStack 有 [3]
//        无条件搬运后 outStack 变成 [3,1,2]，pop 返回 3 而不是 1！
//    ✓ if (outStack.empty()) { ... } — 只有输出栈空了才搬
//
// 3. peek 和 pop 忘了调 transfer():
//    ✗ int pop() { return outStack.top(); } — outStack 可能为空，崩溃
//    ✓ 先 transfer()，再操作
//
// 【面试追问】
//
// Q1: 最朴素的方法是什么？时间复杂度？
// A1: push 时把栈来回倒一遍，push O(n)，pop O(1)。
//     n 次 push 总共 O(n²)。
//
// Q2: 怎么优化到均摊 O(1)？关键 insight 是什么？
// A2: "延迟搬运"——分输入栈和输出栈，只有输出栈空了才搬。
//     每个元素最多搬一次，所以均摊 O(1)。
//     关键 insight：outStack 非空时绝不能搬运，否则破坏顺序。
//
// Q3: 如果把 push 做成均摊 O(1)、pop 做成 O(n)，行不行？
// A3: 技术上可以（反过来，push 进一个栈，pop 时每次都倒过来取底部再倒回去），
//     但每次 pop 都是 O(n)，没有"均摊"效果。延迟搬运之所以能均摊，
//     是因为一次昂贵的搬运惠及后续多次 O(1) 的 pop。
//
// Q4: 能否只用一个栈 + 递归实现？
// A4: 可以。递归的调用栈充当第二个栈。pop 时递归到栈底取出元素再压回去。
//     但本质上还是两个栈（一个显式，一个是系统调用栈），且空间开销更大。
//
// 【相关题型】
// - 225. 用队列实现栈: 镜像题，复用"倒腾"思想，但方向反过来。
//   区别：队列版在 push 时把前面的元素出队再入队到新元素后面。
// - 155. 最小栈: 同样是"辅助数据结构"设计题，复用"一个存数据，
//   一个存辅助信息"的双栈思路。
// - 622. 设计循环队列: 队列设计的进阶版，用数组+双指针实现。
// ============================================================
