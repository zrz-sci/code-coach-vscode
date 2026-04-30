# 232. 用栈实现队列

## 核心思路

栈是"后进先出"(LIFO)，队列是"先进先出"(FIFO)——顺序正好相反。**两个栈倒一次，就像把一摞盘子翻了个面，底部变成了顶部**，LIFO 就变成了 FIFO。

## 思维链

1. **读完题第一反应**：栈和队列的出入顺序是反的。一个栈肯定不行，但题目给了两个栈，能不能用第二个栈把顺序"翻转"过来？

2. **最朴素的想法**：每次 `push` 时，先把栈里的元素全倒到另一个栈，放入新元素，再全倒回来。这样栈顶永远是"最早进来的"，`pop`/`peek` 就是 O(1)。但 `push` 每次都要搬两遍，O(n)。

3. **瓶颈在哪**：每次 push 都要来回倒，很多搬运是重复的。能不能"懒一点"，不急着搬？

4. **关键突破——延迟搬运**：用一个栈专门负责 push（输入栈），另一个专门负责 pop/peek（输出栈）。只有当输出栈空了，才把输入栈的元素一次性倒过去。这样每个元素**最多被搬运一次**，均摊 O(1)！

5. **为什么均摊 O(1)**：每个元素一生只经历：进输入栈（1次）→ 出输入栈进输出栈（1次）→ 出输出栈（1次），共 3 次操作。n 个元素总共 3n 次操作，均摊每次 O(1)。

6. **Follow-up 完美解决**：这个延迟搬运策略天然满足题目要求的"每个操作均摊 O(1)"。

## 解法概览

| 解法 | 思路 | push | pop/peek | 空间 | 面试 |
|------|------|------|----------|------|------|
| 暴力搬运 | push 时来回倒 | O(n) | O(1) | O(n) | 能说出即可 |
| 延迟搬运（双栈） | 输入栈+输出栈，惰性转移 | O(1) | 均摊 O(1) | O(n) | ⭐ 必须写出 |

## 关键提示

1. **想象两摞盘子**：往左边一摞放盘子（push），需要拿最底下的盘子时，把左边整摞翻到右边——底部变顶部，顺序就对了。

2. **什么时候搬？**：不是每次 pop 都搬！只有右边那摞（输出栈）空了，才从左边（输入栈）搬过来。如果右边还有盘子，直接拿就行。

3. **peek 和 pop 的关系**：peek 其实就是"看一眼 pop 要返回的值但不删除"。可以复用逻辑。

4. **过程图示**：
```
push(1), push(2), push(3):

  输入栈 inStack    输出栈 outStack
      [3]  ← top        (空)
      [2]
      [1]  ← bottom

pop() → 输出栈空，搬运！

  输入栈 inStack    输出栈 outStack
      (空)              [1]  ← top  ← 这就是队头！
                        [2]
                        [3]  ← bottom

pop() 返回 1，outStack 弹出顶部

push(4):             pop() 返回 2:
  inStack  outStack    inStack  outStack
   [4]      [2]         [4]      [3]
             [3]
```

## 解法详解

### 解法1: 暴力搬运 — push O(n) / pop O(1)

**思考过程**：最直觉的方式——始终维护一个栈，让栈顶就是队头（最早的元素）。每次 push 新元素时，先把所有元素搬到临时栈，再把新元素放底部，再搬回来。

```cpp
class MyQueue {
    stack<int> s1, s2; // s1 是主栈（栈顶=队头），s2 是临时栈
public:
    MyQueue() {}
    
    void push(int x) {
        // 把 s1 全部倒到 s2
        while (!s1.empty()) {
            s2.push(s1.top());
            s1.pop();
        }
        // 新元素放到 s1 底部（现在 s1 空了，直接放）
        s1.push(x);
        // 再把 s2 倒回 s1，这样新元素在最底下，旧元素在上面
        while (!s2.empty()) {
            s1.push(s2.top());
            s2.pop();
        }
    }
    
    int pop() {
        int front = s1.top(); // 栈顶就是队头
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
```

**关键点**：每次 push 搬了两趟（s1→s2→s1），n 个元素就是 O(n)。这很浪费——大量搬运是重复的。

---

### 解法2: 延迟搬运（双栈） — 均摊 O(1) ⭐ 面试首选

**从解法1优化**：解法1的瓶颈是每次 push 都搬运。核心洞察是：**不需要每次都搬，只在需要的时候搬一次就够了**。

用两个栈分工：
- `inStack`：所有 push 操作放这里
- `outStack`：所有 pop/peek 从这里取；只有 outStack 空了，才把 inStack 一次性倒过来

```cpp
class MyQueue {
    stack<int> inStack, outStack;
    
    // 核心：惰性转移，只有输出栈空了才搬
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
        inStack.push(x); // 永远往输入栈放，O(1)
    }
    
    int pop() {
        transfer(); // 确保输出栈有元素
        int front = outStack.top();
        outStack.pop();
        return front;
    }
    
    int peek() {
        transfer(); // 确保输出栈有元素
        return outStack.top();
    }
    
    bool empty() {
        return inStack.empty() && outStack.empty(); // 两个都空才是真空
    }
};
```

**为什么均摊 O(1)？**
- 每个元素最多被 push 进 inStack 一次、pop 出 inStack 一次、push 进 outStack 一次、pop 出 outStack 一次 = 4 次操作。
- n 个元素总共 4n 次操作，分摊到 n 次 push/pop 上，每次均摊 O(1)。
- 虽然某一次 pop 可能触发 O(k) 的搬运，但后续 k-1 次 pop 都是 O(1)，平均下来就是 O(1)。

## 解法对比

| | 解法1: 暴力搬运 | 解法2: 延迟搬运 |
|---|---|---|
| push | O(n)，每次来回倒 | O(1)，直接入栈 |
| pop/peek | O(1) | 均摊 O(1) |
| 核心思想 | 保持栈有序 | 惰性转移，按需搬运 |
| 面试 | 展示理解即可 | **必须写出** |

**什么时候选哪个？** 面试中直接写解法2。解法1只在面试官追问"最朴素的做法"时口述即可。

## 易错点

1. **`empty()` 只检查一个栈**：
   - ✗ `return inStack.empty();` — 忘了 outStack 里可能还有元素
   - ✓ `return inStack.empty() && outStack.empty();`

2. **每次 pop/peek 都无条件搬运**：
   - ✗ 每次都把 inStack 倒到 outStack — 这会打乱 outStack 里已有元素的顺序！
   - ✓ **只有 outStack 为空时才搬运**，这是"延迟搬运"的核心

3. **peek 和 pop 忘了先调 transfer()**：
   - ✗ 直接 `outStack.top()` — outStack 可能是空的，崩溃
   - ✓ 先 `transfer()` 确保 outStack 有元素

4. **搬运时方向搞反**：
   - 一定是 inStack → outStack（pop inStack 的 top，push 到 outStack），不能反过来

## 面试追问

**Q1: 最朴素的做法是什么？瓶颈在哪？**
> 每次 push 时把栈来回倒一遍，push 是 O(n)。瓶颈在于重复搬运——每次 push 都搬所有元素。

**Q2: 怎么做到均摊 O(1)？**
> 用两个栈分工，输入栈只管 push，输出栈只管 pop/peek。关键是"惰性转移"——只有输出栈空了才搬。每个元素一生最多搬一次，所以均摊 O(1)。

**Q3: 如果把 push 做成 O(n)、pop 做成 O(1)，和反过来 push O(1)、pop 均摊 O(1)，哪个更好？为什么？**
> 延迟搬运更好，因为均摊 O(1) 意味着 n 次操作总共 O(n)，而暴力搬运 n 次 push 总共 O(n²)。在大量 push 的场景下差距很大。

**Q4: 能否用一个栈实现队列？**
> 可以用递归模拟第二个栈——递归调用栈就是隐式的第二个栈。但本质上还是两个栈，只是一个是系统调用栈。

## 相关题型

- **225. 用队列实现栈** — 本题的"镜像题"。复用同样的"倒腾"思想，但方向反过来：用队列的 FIFO 模拟 LIFO。区别：队列版需要在 push 时把前面的元素重新入队到新元素后面。
- **155. 最小栈** — 同样是设计题+栈的变体，复用"辅助栈"的思想：一个栈存数据，一个栈存额外信息（这里是最小值 vs 这里是队列顺序）。
- **346. 数据流中的移动平均值** — 队列的经典应用，可以用本题实现的队列来做。