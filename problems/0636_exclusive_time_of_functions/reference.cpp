/*
 * LeetCode 636: 函数的独占时间 (Exclusive Time of Functions)
 *
 * 【题目本质】
 * 模拟单线程 CPU 的调用栈执行过程，计算每个函数"自己"消耗的时间（不含调子函数的时间）。
 * 核心思想：任意时刻只有栈顶函数在运行，每次事件发生时结算上一段时间给栈顶函数。
 *
 * 【解法总览】
 * 解法1: 栈模拟（标准）— O(L) / O(n) — 分别处理 start/end 的时间差
 * 解法2: 栈模拟（统一时间）— O(L) / O(n) — 将 end 时间 +1 统一处理
 *
 * 其中 L = logs.length, n = 函数数量
 */

/*
 * 【时间轴可视化 — 示例1】
 *
 *   n=2, logs=["0:start:0","1:start:2","1:end:5","0:end:6"]
 *
 *   时间轴:  0   1   2   3   4   5   6
 *           |---|---|---|---|---|---|---|
 *   函数0:  [===]                   [=]     独占 = 2 + 1 = 3
 *   函数1:          [===============]       独占 = 4
 *
 *   事件处理过程:
 *   ┌──────────────┬──────────┬──────────┬──────────────────────────┐
 *   │    事件       │ 栈(底→顶)│ prevTime │ 动作                      │
 *   ├──────────────┼──────────┼──────────┼──────────────────────────┤
 *   │ 0:start:0    │ [0]      │ 0        │ 栈空，无累加。0入栈       │
 *   │ 1:start:2    │ [0,1]    │ 2        │ res[0]+=2-0=2。1入栈     │
 *   │ 1:end:5      │ [0]      │ 6        │ res[1]+=5-2+1=4。1出栈   │
 *   │ 0:end:6      │ []       │ 7        │ res[0]+=6-6+1=1。0出栈   │
 *   └──────────────┴──────────┴──────────┴──────────────────────────┘
 *   结果: res = [3, 4]
 */

/*
 * 【递归调用可视化 — 示例2】
 *
 *   n=1, logs=["0:start:0","0:start:2","0:end:5","0:start:6","0:end:6","0:end:7"]
 *
 *   时间轴:  0   1   2   3   4   5   6   7
 *           |---|---|---|---|---|---|---|---|
 *   函数0:  [===]                       [=]   ← 第1次调用的独占部分
 *   函数0(递归1):  [===============]          ← 第2次调用
 *   函数0(递归2):                  [=]        ← 第3次调用
 *
 *   栈变化:  [0] → [0,0] → [0] → [0,0] → [0] → []
 *   独占总计: 2 + 4 + 1 + 1 = 8
 */

// ============================================================
// 解法1: 栈模拟（标准）— 分别处理 start 和 end 的时间语义
// 时间: O(L)  空间: O(n) — L是日志数，n是函数数
//
// 【思路】
// 核心观察：在任意时刻，只有栈顶函数在"运行"。
// 每当一个事件发生时，从上次事件到现在的时间都属于栈顶函数。
//
// start 事件：新函数要开始了
//   → 先把 [prevTime, curTime-1] 这段时间加给当前栈顶函数
//   → 然后新函数入栈
//   → prevTime 更新为 curTime
//
// end 事件：栈顶函数要结束了
//   → 把 [prevTime, curTime] 这段时间加给栈顶函数（注意含 curTime）
//   → 栈顶出栈
//   → prevTime 更新为 curTime + 1（curTime 已被消耗）
//
// 为什么 start 时间差是 time-prevTime，end 是 time-prevTime+1？
// 因为 start:3 表示"从时间3的开头开始"，时间3本身属于新函数
// 而 end:5 表示"在时间5的末尾结束"，时间5本身属于当前函数
// ============================================================
class Solution1 {
public:
    vector<int> exclusiveTime(int n, vector<string>& logs) {
        vector<int> result(n, 0);
        stack<int> stk;  // 存函数ID，模拟调用栈
        int prevTime = 0;
        
        for (const string& log : logs) {
            // 解析 "id:type:timestamp"
            int firstColon = log.find(':');
            int lastColon = log.rfind(':');
            int id = stoi(log.substr(0, firstColon));
            string type = log.substr(firstColon + 1, lastColon - firstColon - 1);
            int time = stoi(log.substr(lastColon + 1));
            
            if (type == "start") {
                // 新函数开始前，栈顶函数从 prevTime 到 time-1 都在运行
                // 时间差 = time - prevTime（不含 time，time 属于新函数）
                if (!stk.empty()) {
                    result[stk.top()] += time - prevTime;
                }
                stk.push(id);
                prevTime = time;  // 新函数从 time 开始
            } else {
                // 栈顶函数从 prevTime 到 time（含）都在运行
                // 时间差 = time - prevTime + 1（含 time 本身）
                result[stk.top()] += time - prevTime + 1;
                stk.pop();
                prevTime = time + 1;  // time 已消耗完，下一个可用时间是 time+1
            }
        }
        
        return result;
    }
};

// ============================================================
// 解法2: 栈模拟（统一时间处理）— 将 end 时间 +1 统一化
// 时间: O(L)  空间: O(n)
//
// 【思路】
// 解法1 需要对 start 和 end 分别处理时间差（差1的问题），容易出错。
//
// 核心简化：将 end:t 理解为 "下一段从 t+1 开始"
// 具体做法：遇到 end 事件时，先将 time++
// 这样无论 start 还是 end，时间差都统一为 time - prevTime
//
// 这其实是把时间语义统一了：
//   start:3 → "从时间3开始" → time=3
//   end:5   → "结束后下一刻是6" → time=6（原time+1）
//
// 统一后，每次事件处理都是：
//   1. 栈顶函数累加 time - prevTime
//   2. 处理入栈/出栈
//   3. prevTime = time
// ============================================================
class Solution2 {
public:
    vector<int> exclusiveTime(int n, vector<string>& logs) {
        vector<int> result(n, 0);
        stack<int> stk;
        int prevTime = 0;
        
        for (const string& log : logs) {
            int firstColon = log.find(':');
            int lastColon = log.rfind(':');
            int id = stoi(log.substr(0, firstColon));
            string type = log.substr(firstColon + 1, lastColon - firstColon - 1);
            int time = stoi(log.substr(lastColon + 1));
            
            // 关键：end 时间 +1，统一为"下一段的起始时间"
            if (type == "end") time++;
            
            // 统一处理：从 prevTime 到 time 的时间差给栈顶函数
            if (!stk.empty()) {
                result[stk.top()] += time - prevTime;
            }
            
            // 入栈或出栈
            if (type == "start") {
                stk.push(id);
            } else {
                stk.pop();
            }
            
            prevTime = time;  // 统一更新
        }
        
        return result;
    }
};

// ============================================================
// 【解法对比】
//
// 解法1 vs 解法2:
// - 本质相同，都是栈模拟，时间空间复杂度一样
// - 解法1：逻辑直观，分别处理 start/end，好理解好解释
// - 解法2：代码更简洁，将 end+1 统一时间语义，但需要解释为什么+1
// - 面试建议：先写解法1（清晰），面试官追问简化时再给解法2
//
// 【易错点】
//
// 1. end 时间差少了 +1：
//    ✗ result[stk.top()] += time - prevTime;  // end事件
//    ✓ result[stk.top()] += time - prevTime + 1;
//    原因：end:5 表示时间5整个单元都被消耗，不是"在5之前结束"
//
// 2. end 后 prevTime 更新错误：
//    ✗ prevTime = time;    // end事件后
//    ✓ prevTime = time + 1;
//    原因：时间5已被当前函数用完，下一个可用起点是6
//
// 3. 第一个 start 事件时未检查栈空：
//    ✗ result[stk.top()] += time - prevTime;  // 栈空时 top() 未定义行为！
//    ✓ if (!stk.empty()) result[stk.top()] += time - prevTime;
//    原因：程序的第一个事件一定是 start，此时栈为空
//
// 4. 字符串解析只用 find(':') 找两个冒号：
//    ✗ int p1 = log.find(':'); int p2 = log.find(':', p1+1);
//    ✓ int p1 = log.find(':'); int p2 = log.rfind(':');
//    两种都能工作，但 rfind 更直观不易出错
//
// 【面试追问】
//
// Q1(基础): "为什么用栈？能不能不用栈？"
// → 函数调用天然是后进先出（LIFO）：最后调用的最先返回。
//   栈完美模拟调用栈。不用栈的话无法高效跟踪"当前正在运行的函数"，
//   因为一个函数 end 后需要恢复之前被暂停的函数。
//
// Q2(优化): "如果日志量非常大（10^8级别），怎么优化？"
// → 算法本身已经是 O(L) 线性，无法降低。但可以优化常数：
//   - 避免 substr 创建临时字符串，改用 sscanf 或手动解析
//   - 用数组代替 stack<int>（避免动态分配）
//   - 如果日志是流式的，可以边读边处理，不需要全部存储
//
// Q3(变体): "如果要计算每个函数的'包含时间'（含子函数调用时间）呢？"
// → 更简单。只需记录每个函数的 start 时间，end 时减去 start 时间 +1
//   累加到结果。这不需要栈来扣除子函数时间。
//   或者：包含时间 = 独占时间 + 所有直接子调用的包含时间。
//
// Q4(变体): "如果有多线程（多个 CPU 并行执行），怎么改？"
// → 每个线程维护一个独立的栈。日志需要包含线程 ID。
//   按时间排序所有日志，每个线程独立处理自己的 start/end。
//   同一函数可能在不同线程中并行执行，独占时间要累加所有线程的。
// ============================================================
