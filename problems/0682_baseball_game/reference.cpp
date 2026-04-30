/*
 * LeetCode 682: Baseball Game
 *
 * 标签: Stack, Array, Simulation
 * 难度: Easy
 *
 * 纯模拟题。用 vector 当栈，按规则处理四种操作:
 *   数字 → 入栈
 *   "C"  → 弹出栈顶 (删除上一次得分)
 *   "D"  → 栈顶*2 入栈
 *   "+"  → 栈顶两元素之和入栈
 * 最后对栈中所有元素求和。
 */

// ==================== 解法一: vector 模拟栈 (标准写法) ====================
// 时间: O(n)  空间: O(n)
//
// 模拟示例 1: ops = ["5","2","C","D","+"]
//
//   "5" → stk = [5]
//   "2" → stk = [5, 2]
//   "C" → pop → stk = [5]
//   "D" → push 5*2=10 → stk = [5, 10]
//   "+" → push 5+10=15 → stk = [5, 10, 15]
//   sum = 5 + 10 + 15 = 30
//
class Solution {
public:
    int calPoints(vector<string>& operations) {
        vector<int> stk; // 用 vector 当栈 (方便访问倒数第二个元素)

        for (const string& op : operations) {
            if (op == "C") {
                // 移除上一次得分
                stk.pop_back();
            }
            else if (op == "D") {
                // 记录上一次得分的两倍
                stk.push_back(stk.back() * 2);
            }
            else if (op == "+") {
                // 记录前两次得分之和
                int n = stk.size();
                stk.push_back(stk[n - 1] + stk[n - 2]);
            }
            else {
                // 数字 (含负数): 直接入栈
                stk.push_back(stoi(op));
            }
        }

        // 返回所有有效得分的总和
        int sum = 0;
        for (int score : stk) {
            sum += score;
        }
        return sum;
    }
};


// ==================== 解法二: running sum 优化 ====================
// 时间: O(n)  空间: O(n)
//
// 在操作过程中维护总和, push 时加, pop 时减
// 省去最后的遍历求和 (虽然总体仍是 O(n))
//
// 模拟示例 2: ops = ["5","-2","4","C","D","9","+","+"]
//
//   "5"  → stk=[5],            sum=5
//   "-2" → stk=[5,-2],         sum=3
//   "4"  → stk=[5,-2,4],       sum=7
//   "C"  → pop 4, stk=[5,-2],  sum=3
//   "D"  → push -4, stk=[5,-2,-4],  sum=-1
//   "9"  → stk=[5,-2,-4,9],    sum=8
//   "+"  → push -4+9=5, stk=[5,-2,-4,9,5],     sum=13
//   "+"  → push 9+5=14, stk=[5,-2,-4,9,5,14],   sum=27
//
class Solution_RunningSum {
public:
    int calPoints(vector<string>& operations) {
        vector<int> stk;
        int sum = 0;

        for (const string& op : operations) {
            if (op == "C") {
                sum -= stk.back();   // 从总和中减去
                stk.pop_back();
            }
            else if (op == "D") {
                int val = stk.back() * 2;
                stk.push_back(val);
                sum += val;          // 加到总和
            }
            else if (op == "+") {
                int n = stk.size();
                int val = stk[n - 1] + stk[n - 2];
                stk.push_back(val);
                sum += val;          // 加到总和
            }
            else {
                int val = stoi(op);
                stk.push_back(val);
                sum += val;          // 加到总和
            }
        }

        return sum;
    }
};


// ==================== 解法三: 使用 accumulate (简洁写法) ====================
// 时间: O(n)  空间: O(n)
//
// 利用 <numeric> 的 accumulate 做最后求和, 代码更简洁
class Solution_Accumulate {
public:
    int calPoints(vector<string>& operations) {
        vector<int> stk;

        for (const auto& op : operations) {
            if (op == "C")
                stk.pop_back();
            else if (op == "D")
                stk.push_back(stk.back() * 2);
            else if (op == "+")
                stk.push_back(stk.back() + stk[stk.size() - 2]);
            else
                stk.push_back(stoi(op));
        }

        return accumulate(stk.begin(), stk.end(), 0);
    }
};


// ==================== 解法四: 使用 std::stack (对比) ====================
// 不推荐: std::stack 无法访问倒数第二个元素
// 需要额外 pop + push 操作来处理 "+"
//
// 展示为什么 vector 更好:
class Solution_StdStack {
public:
    int calPoints(vector<string>& operations) {
        stack<int> stk;

        for (const string& op : operations) {
            if (op == "C") {
                stk.pop();
            }
            else if (op == "D") {
                stk.push(stk.top() * 2);
            }
            else if (op == "+") {
                // 需要额外操作才能访问倒数第二个
                int top1 = stk.top(); stk.pop();
                int top2 = stk.top();
                stk.push(top1);          // 放回
                stk.push(top1 + top2);   // 新分数
            }
            else {
                stk.push(stoi(op));
            }
        }

        // std::stack 没有迭代器, 需要逐个 pop 求和
        int sum = 0;
        while (!stk.empty()) {
            sum += stk.top();
            stk.pop();
        }
        return sum;
    }
};


/*
 * ┌─────────────────────┬──────────┬────────┬────────────────────────────┐
 * │ 解法                │ 时间     │ 空间   │ 说明                       │
 * ├─────────────────────┼──────────┼────────┼────────────────────────────┤
 * │ vector 模拟栈       │ O(n)     │ O(n)   │ 标准写法, 面试首选         │
 * │ running sum 优化    │ O(n)     │ O(n)   │ 省去最后遍历               │
 * │ accumulate 简洁版   │ O(n)     │ O(n)   │ 代码最短                   │
 * │ std::stack (不推荐) │ O(n)     │ O(n)   │ "+"操作不方便              │
 * └─────────────────────┴──────────┴────────┴────────────────────────────┘
 *
 * 为什么 vector 优于 std::stack?
 *   1. "+" 操作需要访问倒数第二个元素
 *      vector: stk[stk.size()-2]  → O(1), 一行代码
 *      stack:  pop → peek → push → push  → 多步操作, 容易出错
 *
 *   2. 最后求和
 *      vector: for 循环或 accumulate
 *      stack:  只能 while pop 求和
 *
 * stoi() 要点:
 *   - stoi("123")  = 123
 *   - stoi("-2")   = -2    (自动处理负号)
 *   - stoi("0")    = 0
 *   - 如果字符串不是数字会抛 invalid_argument 异常
 *     (本题保证输入合法, 不需要额外处理)
 *
 * 面试建议:
 *   这是 Easy 题, 重点展示:
 *   1. 清晰的代码结构 (if-else if-else)
 *   2. 选择 vector 而非 stack 的理由
 *   3. 对边界情况的认知 (负数、"C" 后操作)
 *
 *
 * ======================== 总结 ========================
 *
 * 【解法对比】
 *   - vector 模拟栈: 面试首选，代码清晰，"+" 操作一行搞定
 *   - running sum: 实际性能相同，但展示了"增量维护"的思维
 *   - accumulate: 代码最短，适合竞赛
 *   - std::stack: 反面教材，说明为什么要选对数据结构
 *
 * 【易错点】
 *   1. "+" 操作时误用 stk.top() + stk.top()，忘了要访问倒数第二个元素
 *   2. 忘记负数字符串 "-2" 也要用 stoi 处理（stoi 自动支持负号）
 *   3. 用 std::stack 后发现无法随机访问，临场换数据结构浪费时间
 *   4. "C" 操作后立刻做 "D" 或 "+" — 题目保证操作合法，不需要判空
 *      但面试中提一句"题目保证合法"体现严谨性
 *
 * 【面试追问】
 *   Q1: 如果操作序列不保证合法（比如空栈时执行"C"），你怎么处理？
 *       → 每次 pop_back / back 前检查 stk.empty()，不合法时抛异常或跳过
 *
 *   Q2: 如果新增一个操作 "M" 表示取栈中所有元素的中位数并入栈，怎么做？
 *       → 方案1: 维护一个额外的 sorted multiset，O(log n) 取中位数
 *       → 方案2: 双堆（max-heap + min-heap）经典中位数流结构
 *       → 注意 "C" 撤销时也要从辅助结构中移除
 *
 *   Q3: 如果需要支持 undo/redo（撤销任意历史操作），数据结构怎么设计？
 *       → Command 模式: 每个操作封装成 {execute, undo} 对象
 *       → 用两个栈: undoStack 和 redoStack
 *       → 这是编辑器撤销/重做的经典设计模式
 */
