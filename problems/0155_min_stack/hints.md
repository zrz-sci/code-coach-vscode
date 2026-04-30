# 155. 最小栈

## 核心思路

本质问题：普通栈的 push/pop/top 都是 O(1)，但"获取最小值"需要遍历整个栈才能知道。**如何用额外信息记住每个状态下的最小值，使 getMin 也变成 O(1)？**

## 思维链

1. **读完题第一反应**：栈的 push/pop/top 本身就是 O(1)，唯一的难点在 `getMin()`。最朴素的做法是每次调用 getMin 时遍历整个栈找最小值 → O(n)。

2. **暴力解的瓶颈**：getMin 需要 O(n) 遍历。题目要求所有操作都是 O(1)。能不能用一个变量记录当前最小值？

3. **单变量为什么不够？** 假设用 `curMin` 记录最小值，push 时可以更新 `curMin = min(curMin, val)`。但 pop 时如果弹出的正好是最小值，`curMin` 该变成什么？我们不知道"次小值"是多少！

4. **关键洞察**：栈有一个重要特性 — **元素弹出的顺序是固定的**。当栈有 n 个元素时，最小值是确定的；弹出栈顶后，剩余 n-1 个元素的最小值也是确定的。所以我们可以**为每个栈状态都记录一个最小值**。

5. **具体方案**：用一个辅助栈（或将最小值和元素一起存储），让每个"栈深度"都对应一个最小值。push 时，辅助栈存 `min(val, 当前最小值)`；pop 时，辅助栈同步弹出。这样 getMin 只需看辅助栈栈顶。

6. **进一步优化**：辅助栈中有很多重复的最小值，能否只在最小值变化时才压入？→ 引出"非严格递减辅助栈"的优化。

## 解法概览

| 解法 | 思路 | 时间(每操作) | 空间 | 面试 |
|------|------|-------------|------|------|
| 暴力 getMin | 每次遍历栈找最小 | O(n) getMin | O(n) | 能说出即可 |
| 双栈同步 | 辅助栈同步记录每层最小值 | O(1) 全部 | O(n) | ⭐ 必须写出 |
| 单栈存 pair | 每个元素和当时的最小值打包存 | O(1) 全部 | O(n) | ⭐ 必须写出 |
| 非严格递减辅助栈 | 辅助栈只在最小值变化时压入 | O(1) 全部 | O(n) 最坏,平均更优 | 加分项 |

## 关键提示

1. **栈的"状态"是单调的**：每次 push 增加一个元素，pop 减少一个元素。想想看，能不能为每个状态快照记住"此刻的最小值"？

2. **pop 是 getMin 的难点**：如果弹出的恰好是最小值，你需要知道"没有它的时候，最小值是谁"。这个信息必须提前保存。

3. **辅助栈和主栈完全同步**：主栈 push 时辅助栈也 push，主栈 pop 时辅助栈也 pop。辅助栈栈顶永远是当前状态的最小值。

4. **ASCII 示意图** — 模拟题目示例的操作过程：
```
操作               主栈 stk        辅助栈 minStk       getMin
push(-2)          [-2]            [-2]                -2
push(0)           [-2, 0]         [-2, -2]            -2
push(-3)          [-2, 0, -3]     [-2, -2, -3]        -3
pop()             [-2, 0]         [-2, -2]            -2
top() → 0
getMin() → -2
```

5. **思考 follow-up**：能否不用辅助栈，只用 O(1) 额外空间？→ 可以用差值编码技巧（存 val - curMin），但需要处理溢出，面试中不常考。

## 解法详解

### 解法1: 暴力 getMin — O(n) getMin / O(n) 空间

**思考过程**: 最直觉的做法。push/pop/top 用标准栈操作，getMin 时遍历栈内所有元素找最小值。

```cpp
class MinStack {
    stack<int> stk;
public:
    MinStack() {}
    
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
        // 把栈倒出来找最小值，再倒回去 — O(n)
        stack<int> tmp;
        int minVal = stk.top();
        while (!stk.empty()) {
            minVal = min(minVal, stk.top());
            tmp.push(stk.top());
            stk.pop();
        }
        while (!tmp.empty()) {
            stk.push(tmp.top());
            tmp.pop();
        }
        return minVal;
    }
};
```

**关键点**: getMin 是 O(n)，不满足题目要求的 O(1)。但这个解法帮我们理解了问题 — **瓶颈在于 getMin 不知道当前栈的最小值，需要遍历**。

---

### 解法2: 双栈同步 — O(1) 全部 / O(n) ⭐ 面试首选

**从解法1优化**: 解法1的瓶颈是 getMin 需要遍历。如果我们用一个辅助栈，和主栈完全同步地记录"截至当前深度的最小值"，getMin 就变成了 O(1) 读栈顶。

**为什么正确**：栈是 LIFO 结构，元素的弹出顺序和压入顺序相反。当我们 pop 时，辅助栈也 pop，自动恢复到上一个状态的最小值 — 不需要任何额外计算。

```
// 双栈同步示意图:
//
// 操作          主栈 stk       辅助栈 minStk       说明
// ─────────────────────────────────────────────────────────
// push(-2)     [-2]           [-2]                min(-2) = -2
// push(0)      [-2, 0]        [-2, -2]            min(0, -2) = -2
// push(-3)     [-2, 0, -3]    [-2, -2, -3]        min(-3, -2) = -3
// getMin()                                         minStk.top() = -3 ✓
// pop()        [-2, 0]        [-2, -2]            同步弹出
// top()                                            stk.top() = 0 ✓
// getMin()                                         minStk.top() = -2 ✓
```

```cpp
class MinStack {
    stack<int> stk;
    stack<int> minStk; // minStk.top() = 当前栈中所有元素的最小值
public:
    MinStack() {}
    
    void push(int val) {
        stk.push(val);
        // 关键：辅助栈压入 "val 和当前最小值中更小的那个"
        // 如果辅助栈为空，直接压 val
        if (minStk.empty()) {
            minStk.push(val);
        } else {
            minStk.push(min(val, minStk.top()));
        }
    }
    
    void pop() {
        stk.pop();
        minStk.pop(); // 和主栈完全同步
    }
    
    int top() {
        return stk.top();
    }
    
    int getMin() {
        return minStk.top(); // O(1)，因为栈顶就是当前最小值
    }
};
```

**关键点**: 辅助栈和主栈**必须完全同步** push/pop，这样两者的大小永远一致，getMin 直接读栈顶即可。

---

### 解法3: 单栈存 pair — O(1) 全部 / O(n)

**换个角度**: 不用两个栈，而是把"当前值"和"当前最小值"打包成一个 pair 存在同一个栈里。本质和解法2完全一样，只是数据组织方式不同。

```cpp
class MinStack {
    // 每个元素存 {值, 截至此刻的最小值}
    stack<pair<int,int>> stk;
public:
    MinStack() {}
    
    void push(int val) {
        if (stk.empty()) {
            stk.push({val, val});
        } else {
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
        return stk.top().second;
    }
};
```

**关键点**: pair 的 second 扮演了解法2中辅助栈栈顶的角色。代码更简洁，但本质思路完全一样。

---

### 解法4: 非严格递减辅助栈 — O(1) 全部 / 空间优化

**从解法2优化空间**: 解法2的辅助栈中有大量重复值（比如连续 push 了很多比当前最小值大的数，辅助栈不断重复压入同一个最小值）。能否只在最小值**发生变化**（或相等）时才压入辅助栈？

**核心规则**:
- push 时：只有当 `val <= minStk.top()` 时才压入辅助栈
- pop 时：只有当 `stk.top() == minStk.top()` 时才弹出辅助栈

**为什么用 `<=` 而不是 `<`**：如果两次 push 了相同的最小值，我们需要在辅助栈中记录两次，否则第一次 pop 就会把唯一的记录删掉。

```
// 非严格递减辅助栈示意:
//
// 操作          主栈             辅助栈(只存递减)     说明
// ──────────────────────────────────────────────────────────
// push(-2)     [-2]             [-2]                -2 入辅助栈
// push(0)      [-2, 0]          [-2]                0 > -2, 不入辅助栈
// push(-3)     [-2, 0, -3]      [-2, -3]            -3 <= -2, 入辅助栈
// getMin()                                           -3 ✓
// pop(-3)      [-2, 0]          [-2]                -3 == minStk.top(), 同步弹出
// getMin()                                           -2 ✓
```

```cpp
class MinStack {
    stack<int> stk;
    stack<int> minStk; // 非严格递减：只在新值 <= 栈顶时压入
public:
    MinStack() {}
    
    void push(int val) {
        stk.push(val);
        // 只有 val <= 当前最小值时才压入辅助栈
        if (minStk.empty() || val <= minStk.top()) {
            minStk.push(val);
        }
    }
    
    void pop() {
        // 如果弹出的值等于当前最小值，辅助栈也要弹
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
```

**关键点**: `<=` 不能写成 `<`。如果 push 两次 -3，辅助栈必须存两个 -3，否则第一次 pop -3 后辅助栈就空了。

---

## 解法对比

| | 解法1 暴力 | 解法2 双栈同步 | 解法3 单栈pair | 解法4 递减辅助栈 |
|---|---|---|---|---|
| getMin 时间 | O(n) | **O(1)** | **O(1)** | **O(1)** |
| 空间 | O(n) | O(n) 固定2倍 | O(n) 固定2倍 | O(n) 最坏,平均更优 |
| 代码复杂度 | 最简单 | 简单 | 最简洁 | 需注意 <= |
| 面试推荐 | 引出思路 | **首选** | 同等推荐 | 追问时使用 |

- 解法2和解法3本质完全相同，只是数据存储方式不同。面试中任选其一即可。
- 解法4在辅助栈空间上有优化，但最坏情况（递减序列）仍然是 O(n)。面试官追问"能否优化空间"时可以提出。

## 易错点

1. **辅助栈为空时直接访问 top()**
   - ✗ `minStk.push(min(val, minStk.top()))` — 如果 minStk 为空，top() 未定义行为
   - ✓ 先判断 `if (minStk.empty()) minStk.push(val); else minStk.push(min(val, minStk.top()));`

2. **解法4中用 `<` 而不是 `<=`**
   - ✗ `if (val < minStk.top()) minStk.push(val);` — push(-2), push(-2), pop() 后辅助栈为空
   - ✓ `if (val <= minStk.top()) minStk.push(val);` — 相等时也要压入

3. **解法4中 pop 时先弹主栈再比较**
   - ✗ `stk.pop(); if (stk.top() == minStk.top()) ...` — 已经弹出了，比较的是下一个元素！
   - ✓ `if (stk.top() == minStk.top()) minStk.pop(); stk.pop();` — 先比较再弹

4. **用 `int` 的 `INT_MIN` / `INT_MAX` 做初始化**
   - 本题不需要特殊初始化，用 `empty()` 判断即可。滥用 INT_MIN/INT_MAX 容易在边界值（如 push(INT_MIN)）时出错。

## 面试追问

**Q1: 基础理解 — 为什么不能用一个变量记录最小值？**

> 因为 pop 操作可能弹出当前最小值。一个变量无法恢复到"上一个状态的最小值"。栈的特性是 LIFO，我们需要为每个栈深度都维护一个最小值。

**Q2: 优化空间 — 辅助栈能否不和主栈完全同步？**

> 可以。使用"非严格递减辅助栈"（解法4），只在新值 ≤ 当前最小值时才压入辅助栈。注意必须是"≤"而非"<"，处理重复最小值的情况。

**Q3: 进阶变体 — 如果还要求 O(1) 的 getMax 呢？**

> 再加一个辅助栈维护最大值即可。maxStk 的逻辑和 minStk 对称：push 时压入 `max(val, maxStk.top())`，pop 时同步弹出。

**Q4: 极限挑战 — 能否只用 O(1) 额外空间（不用辅助栈）？**

> 可以用"差值编码"：栈中不存原值，而是存 `val - curMin`。当存入负数时说明 val 成了新的最小值，pop 时可以反推出旧的最小值。但需要用 long 防溢出，面试中解释思路即可，不常要求实现。

## 相关题型

- **716. Max Stack** — 复用同样的辅助栈思路，但 `popMax()` 需要删除非栈顶元素，因此需要更复杂的数据结构（双向链表 + 有序map/heap）
- **232. 用栈实现队列** — 同为栈的设计题，练习用两个栈模拟队列的 FIFO 行为
- **225. 用队列实现栈** — 反过来，用队列模拟栈的 LIFO 行为
- **239. 滑动窗口最大值** — 复用"维护一个单调结构来快速获取极值"的思路，从栈推广到了双端队列（单调队列）