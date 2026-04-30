# 636. 函数的独占时间

## 核心思路

这道题本质上是**模拟调用栈的执行过程**：用栈跟踪当前正在执行的函数，每当有新函数开始或当前函数结束时，计算栈顶函数在这段时间内消耗的"独占时间"。

## 思维链

1. **读完题第一反应**：这就是一个调用栈的模拟。函数 start 就是入栈，end 就是出栈。关键在于计算每个函数"自己"消耗的时间，不包括调用子函数的时间。

2. **怎么理解"独占时间"**：如果函数 A 调用了函数 B，那么 B 执行期间的时间不算 A 的。也就是说，只有**栈顶函数**在"消耗"时间。

3. **关键观察**：在任意时刻，只有栈顶函数在运行。每当发生一个事件（start 或 end），我们可以算出**上一个事件到当前事件之间**，栈顶函数消耗了多少时间。

4. **怎么用栈实现**：
   - 维护一个栈，存当前正在执行的函数 ID
   - 维护一个 `prevTime` 记录上一个事件的时间戳
   - 遇到 `start`：当前栈顶函数从 `prevTime` 到 `curTime` 这段时间都在运行 → 累加给栈顶函数；然后新函数入栈
   - 遇到 `end`：栈顶函数从 `prevTime` 到 `curTime` 这段时间在运行 → 累加给栈顶函数；然后出栈

5. **时间戳的细节**：`start` 表示时间单元的"开始"，`end` 表示时间单元的"末尾"。所以 `end:5` 意味着函数执行完了时间单元 5，下一个可用时间是 6。这个细节决定了时间差的计算方式。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 栈模拟（标准） | 用栈跟踪当前执行函数，每个事件更新栈顶函数时间 | O(L) | O(n) | ⭐ 必须写出 |
| 栈模拟（简化时间处理） | 将 end 时间 +1 统一为"下一个开始时间"，简化计算 | O(L) | O(n) | 加分项 |

> L = logs.length, n = 函数数量

## 关键提示

1. **只有栈顶函数在消耗时间** — 这是理解本题的核心。每次事件发生时，把"上次事件到现在"的时间差加给栈顶函数。

2. **start 和 end 的时间语义不同** — `start:3` 意味着从时间 3 的开头开始；`end:5` 意味着时间 5 的末尾结束（即占用了时间 5 整个单元）。所以遇到 end 时，时间差要 +1。

3. **prevTime 的更新规则** — 遇到 start 事件，prevTime 更新为当前时间戳；遇到 end 事件，prevTime 更新为当前时间戳 **+1**（因为 end 消耗完了这个时间单元）。

4. **ASCII 示意图**：
```
示例1: n=2, logs=["0:start:0","1:start:2","1:end:5","0:end:6"]

时间轴:  0   1   2   3   4   5   6
        |---|---|---|---|---|---|---|
函数0:  [===]                   [=]     独占 = 2 + 1 = 3
函数1:          [===============]       独占 = 4

栈变化:
  事件          栈(底→顶)     prevTime   动作
  0:start:0     [0]           0          函数0入栈
  1:start:2     [0,1]         2          栈顶0累加 2-0=2, 函数1入栈
  1:end:5       [0]           6          栈顶1累加 5-2+1=4, 函数1出栈
  0:end:6       []            7          栈顶0累加 6-6+1=1, 函数0出栈
```

5. **递归调用不影响逻辑** — 同一函数递归调用自身只是在栈中多出现一次，逻辑完全一样。

## 解法详解

### 解法1: 栈模拟 — O(L) / O(n) ⭐ 面试首选

**思考过程**：题目描述的就是一个调用栈。我们只需要模拟这个过程：维护一个栈，每次事件发生时，计算从上次事件到当前事件之间栈顶函数消耗的时间。

关键在于处理 start 和 end 的时间语义差异：
- `start:t` → 函数从时间 t 的开头开始执行
- `end:t` → 函数在时间 t 的末尾结束执行（即消耗了整个时间单元 t）

所以：
- 遇到 start：时间差 = `curTime - prevTime`（不含 curTime，因为 curTime 开始属于新函数）
- 遇到 end：时间差 = `curTime - prevTime + 1`（含 curTime，因为 curTime 整个单元都属于当前函数）

```cpp
class Solution {
public:
    vector<int> exclusiveTime(int n, vector<string>& logs) {
        vector<int> result(n, 0);
        stack<int> stk;  // 存函数ID，栈顶是当前正在执行的函数
        int prevTime = 0;
        
        for (const string& log : logs) {
            // 解析日志: "id:start/end:timestamp"
            int firstColon = log.find(':');
            int lastColon = log.rfind(':');
            int id = stoi(log.substr(0, firstColon));
            string type = log.substr(firstColon + 1, lastColon - firstColon - 1);
            int time = stoi(log.substr(lastColon + 1));
            
            if (type == "start") {
                // 在新函数开始之前，栈顶函数一直在运行
                // 从 prevTime 到 time-1 这段时间都属于栈顶函数
                if (!stk.empty()) {
                    result[stk.top()] += time - prevTime;
                }
                stk.push(id);
                prevTime = time;  // 新函数从 time 开始
            } else {
                // end: 栈顶函数从 prevTime 到 time(含) 都在运行
                result[stk.top()] += time - prevTime + 1;
                stk.pop();
                prevTime = time + 1;  // 下一个可用时间是 time+1
            }
        }
        
        return result;
    }
};
```

**关键点**：
- `prevTime` 在 start 事件后更新为 `time`，在 end 事件后更新为 `time + 1`
- 这是因为 end 消耗完了当前时间单元，下一个函数只能从 `time + 1` 开始

### 解法2: 统一时间处理 — O(L) / O(n)

**从解法1简化**：解法1需要分别处理 start 和 end 的时间差计算（一个不 +1，一个 +1）。如果我们把 end 的时间戳统一 +1（变成"下一个可用时间点"），就可以统一处理。

```cpp
class Solution {
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
            
            // 统一处理：end 的时间 +1，变成"下一个时间点的开始"
            if (type == "end") time++;
            
            // 现在不管 start 还是 end，时间差的计算都是 time - prevTime
            if (!stk.empty()) {
                result[stk.top()] += time - prevTime;
            }
            
            if (type == "start") {
                stk.push(id);
            } else {
                stk.pop();
            }
            
            prevTime = time;
        }
        
        return result;
    }
};
```

**核心简化**：将 `end:5` 理解为 `start_of_next:6`，这样所有事件都是"从 prevTime 到 curTime 的时间差给栈顶函数"，不再需要分别处理 +1 的问题。

## 解法对比

| | 解法1（标准） | 解法2（统一时间） |
|---|---|---|
| 时间差计算 | start 和 end 分别处理 | 统一为 `time - prevTime` |
| prevTime 更新 | start: `time`, end: `time+1` | 统一为 `time`（end 已提前 +1） |
| 代码简洁度 | 逻辑清晰，更好理解 | 更简洁，但需要理解"统一化"思想 |
| 面试推荐 | ⭐ 首选（好解释） | 如果面试官问能否简化 |

## 易错点

1. **end 时间差忘记 +1**
   - ✗ `result[stk.top()] += time - prevTime;`（end 事件）
   - ✓ `result[stk.top()] += time - prevTime + 1;`
   - 原因：`end:5` 表示时间 5 的**末尾**结束，函数占用了整个时间单元 5，所以包含 5 本身

2. **prevTime 在 end 后更新错误**
   - ✗ `prevTime = time;`（end 事件后）
   - ✓ `prevTime = time + 1;`
   - 原因：时间 5 已经被当前函数消耗完了，下一个可用时间是 6

3. **start 时没有检查栈是否为空**
   - ✗ 直接 `result[stk.top()] += ...`
   - ✓ 先 `if (!stk.empty())` 再访问栈顶
   - 原因：第一个 start 事件时栈为空，没有"之前正在运行的函数"

4. **字符串解析错误**
   - ✗ 用 `find(':')` 找两次冒号位置（可能找到同一个）
   - ✓ 用 `find(':')` 和 `rfind(':')` 分别从前和从后找
   - 原因：日志格式是 `id:type:timestamp`，有两个冒号

## 面试追问

**Q1: 为什么用栈而不是其他数据结构？**
→ 函数调用天然符合"后进先出"：最后被调用的函数一定最先返回（嵌套调用）。栈完美匹配这个行为。实际上操作系统的调用栈就是这么工作的。

**Q2: 如果日志不是按时间排序的，怎么处理？**
→ 先按时间戳排序，start 事件优先于同时间的 end 事件（题目保证不会有同时间的相同类型事件，但如果放宽约束就需要排序）。排序后再用同样的栈模拟。

**Q3: 如果要求输出每个函数每次调用的时间（而非总独占时间），怎么改？**
→ 栈中不只存函数 ID，还存每次调用的起始时间和已累积的独占时间。end 时计算这次调用的独占时间并输出/记录。需要在栈中存 `{id, accumulatedTime}` 的结构。

**Q4: 时间复杂度能否优于 O(L)？**
→ 不能。每条日志都必须读取一次，所以 O(L) 是下界。

## 相关题型

- **71. 简化路径** — 同样用栈处理"嵌套"结构（目录路径），复用栈匹配/模拟的思想
- **394. 字符串解码** — 栈处理嵌套结构，遇到 `[` 入栈、`]` 出栈并合并结果，和本题的 start/end 配对思路类似
- **735. 小行星碰撞** — 栈模拟过程，每个事件影响栈顶元素，复用"事件驱动的栈模拟"模式
- **1249. 移除无效括号** — 栈匹配配对结构，start/end 的配对本质就是括号匹配