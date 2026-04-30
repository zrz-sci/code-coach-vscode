# 1472. 设计浏览器历史记录

## 核心思路

本质上是设计一个**支持"当前位置"概念的线性历史列表**：可以在任意位置插入（并截断后续历史），也可以前后移动指针。关键在于 `visit` 时要清除前进历史。

## 思维链

1. **读完题第一反应**：浏览器历史就像一个线性的页面序列，有一个"当前位置"指针。后退就是指针左移，前进就是指针右移，访问新页面就是在当前位置后插入并删掉后面的所有页面。

2. **想到什么数据结构？** 有两种直觉：
   - **数组 + 指针**：用一个数组存所有页面，一个 `cur` 表示当前位置，一个 `end` 表示有效历史的末尾。visit 时在 cur+1 处写入新 url，然后把 end 更新到 cur。这样不需要真的删除后面的元素。
   - **双栈**：一个栈存"后退历史"，一个栈存"前进历史"。visit 时清空前进栈。back 时从后退栈弹出压入前进栈，forward 反过来。

3. **哪个更简单？** 数组方案三个操作都是 O(1)（visit 是均摊 O(1)），back/forward 只需移动指针。双栈方案 back/forward 是 O(steps)，因为要逐个弹出压入。

4. **还有没有其他方案？** 双向链表也可以，但实现更复杂，没有额外优势。面试中数组方案最简洁。

5. **核心难点**：visit 时"清除前进历史"不需要真的擦数据，只需要把 `end = cur` 即可，后面的数据虽然还在数组里但逻辑上已经无效了。

## 解法概览

| 解法 | 思路 | 时间 (visit/back/forward) | 空间 | 面试 |
|------|------|---------------------------|------|------|
| 数组 + 双指针 | 数组存页面，cur/end 标记位置 | O(1) / O(1) / O(1) | O(n) | ⭐ 必须写出 |
| 双栈 | 后退栈 + 前进栈 | O(1) / O(steps) / O(steps) | O(n) | 能说出即可 |
| 双向链表 | 链表节点存页面，指针移动 | O(1) / O(steps) / O(steps) | O(n) | 加分项（展示链表功底） |

## 关键提示

1. **visit 清除前进历史**：不需要真的 `erase` 或 `clear`，只需要调整一个边界变量。想想"逻辑删除"和"物理删除"的区别。

2. **back/forward 的步数可能超过实际能走的距离**：用 `min(steps, 可走距离)` 来限制，千万别忘了。

3. **双栈方案中，visit 后前进栈必须清空**：这是题目明确要求的——"清除所有前进历史"。

4. 示意图——数组方案的状态变化：
```
初始:  [leetcode]          cur=0, end=0
visit: [leetcode, google]  cur=1, end=1
visit: [leetcode, google, facebook]  cur=2, end=2
visit: [leetcode, google, facebook, youtube]  cur=3, end=3
back(1):  cur=2  → "facebook"
back(1):  cur=1  → "google"
forward(1): cur=2 → "facebook"
visit("linkedin"):  
       [leetcode, google, facebook, linkedin]  cur=3, end=3
       ↑ youtube 被逻辑覆盖了！
forward(2): cur 已在 end，无法前进 → "linkedin"
back(2):  cur=1 → "google"
back(7):  cur=max(0, 1-7)=0 → "leetcode"
```

## 解法详解

### 解法1: 数组 + 双指针 — O(1) / O(n) ⭐ 面试首选

**思考过程**: 浏览历史是线性的，最自然的存储方式就是数组。关键观察：visit 时不需要真的删除后面的元素，只要标记"有效范围到哪里"就行。这就是 `cur`（当前位置）和 `end`（有效历史末尾）两个指针的作用。

```cpp
class BrowserHistory {
    vector<string> history;
    int cur;  // 当前页面在 history 中的下标
    int end_;  // 有效历史的最后一个位置
public:
    BrowserHistory(string homepage) {
        history.push_back(homepage);
        cur = 0;
        end_ = 0;
    }
    
    void visit(string url) {
        cur++;
        // 如果 cur 位置已有旧数据，直接覆盖；否则 push_back
        if (cur < (int)history.size()) {
            history[cur] = url;
        } else {
            history.push_back(url);
        }
        // 关键：前进历史全部失效，end 设为 cur
        end_ = cur;
    }
    
    string back(int steps) {
        // 最多退到下标 0
        cur = max(0, cur - steps);
        return history[cur];
    }
    
    string forward(int steps) {
        // 最多进到 end_
        cur = min(end_, cur + steps);
        return history[cur];
    }
};
```

**关键点**:
- `visit` 中覆盖 vs push_back 的判断：如果之前历史数组很长，cur+1 可能还在数组范围内，直接覆盖即可。
- `end_ = cur` 是"逻辑删除"前进历史的精髓，O(1) 完成。

---

### 解法2: 双栈 — O(1) visit / O(steps) back,forward

**思考过程**: 后退和前进天然对应"两个方向"，很容易联想到双栈。backStack 存后退能到的页面，forwardStack 存前进能到的页面，`current` 记录当前页面。

```cpp
class BrowserHistory {
    stack<string> backStack;     // 后退历史
    stack<string> forwardStack;  // 前进历史
    string current;
public:
    BrowserHistory(string homepage) {
        current = homepage;
    }
    
    void visit(string url) {
        backStack.push(current);    // 当前页面进入后退历史
        current = url;
        // 清除所有前进历史
        while (!forwardStack.empty()) forwardStack.pop();
    }
    
    string back(int steps) {
        while (steps > 0 && !backStack.empty()) {
            forwardStack.push(current);  // 当前页面存入前进栈
            current = backStack.top();
            backStack.pop();
            steps--;
        }
        return current;
    }
    
    string forward(int steps) {
        while (steps > 0 && !forwardStack.empty()) {
            backStack.push(current);     // 当前页面存入后退栈
            current = forwardStack.top();
            forwardStack.pop();
            steps--;
        }
        return current;
    }
};
```

**关键点**:
- back/forward 每步都要把 current 转移到另一个栈，时间与 steps 成正比。
- visit 时清空 forwardStack 的 while 循环最坏 O(n)。

---

### 解法3: 双向链表 — O(1) visit / O(steps) back,forward

**思考过程**: 链表天然支持"在中间插入并截断后续"。每个节点存一个 url，有 prev 和 next 指针。visit 时在当前节点后插入新节点，next 设为 nullptr（截断前进历史）。

```cpp
class BrowserHistory {
    struct Node {
        string url;
        Node* prev;
        Node* next;
        Node(string u) : url(u), prev(nullptr), next(nullptr) {}
    };
    Node* current;
public:
    BrowserHistory(string homepage) {
        current = new Node(homepage);
    }
    
    void visit(string url) {
        Node* newNode = new Node(url);
        current->next = newNode;  // 截断：旧的 next 链被丢弃
        newNode->prev = current;
        current = newNode;
        // 注意：这里旧的前进链表节点没有 delete，实际面试可以提
        // 生产代码应该遍历旧 next 链逐个 delete 避免内存泄漏
    }
    
    string back(int steps) {
        while (steps > 0 && current->prev) {
            current = current->prev;
            steps--;
        }
        return current->url;
    }
    
    string forward(int steps) {
        while (steps > 0 && current->next) {
            current = current->next;
            steps--;
        }
        return current->url;
    }
    
    // 析构函数应该释放所有节点（面试中可以提到但不必写完）
};
```

**关键点**:
- visit 的截断是 O(1)（只修改指针），但存在内存泄漏。面试中主动提到这一点是加分项。
- back/forward 仍然是 O(steps)，和双栈一样。

---

## 解法对比

| | 数组+双指针 | 双栈 | 双向链表 |
|---|---|---|---|
| visit | O(1) | O(1) 均摊，清栈最坏 O(n) | O(1) 但有内存泄漏 |
| back | O(1) | O(steps) | O(steps) |
| forward | O(1) | O(steps) | O(steps) |
| 代码复杂度 | 最简单 | 中等 | 最复杂 |
| 面试推荐 | ⭐ 首选 | 展示栈的理解 | 展示链表功底 |

**选择建议**: 面试中优先写数组方案，三个操作全是 O(1)，代码最短。如果面试官追问"用链表怎么实现"或"用栈怎么实现"，再给出对应方案。

## 易错点

1. **visit 后忘记更新 end**
   - ✗ `history[++cur] = url;` （忘了 `end_ = cur`）
   - ✓ `history[++cur] = url; end_ = cur;`
   - 不更新 end，forward 可能走到已经被"逻辑删除"的旧页面

2. **数组越界：cur 超出 history.size()**
   - ✗ `history[cur] = url;`（如果 cur == history.size()，这是越界访问）
   - ✓ 先判断 `cur < history.size()` 再覆盖，否则 `push_back`

3. **back 时 cur 减成负数**
   - ✗ `cur -= steps; return history[cur];`
   - ✓ `cur = max(0, cur - steps); return history[cur];`

4. **双栈方案中 back/forward 忘记移动 current**
   - ✗ 只弹出 backStack 但不更新 current
   - ✓ 每一步都要 `forwardStack.push(current); current = backStack.top(); backStack.pop();`

## 面试追问

**Q1: 三种数据结构各有什么优缺点？什么场景选哪个？**
> 数组方案所有操作 O(1)，最优。双栈方案 back/forward 是 O(steps)，但概念清晰。双向链表支持 O(1) 截断但有内存管理问题。面试中选数组。

**Q2: 如果要支持"查看第 k 个历史记录"这样的随机访问操作呢？**
> 数组天然支持 O(1) 随机访问。双栈和链表都不支持，需要转成数组或加索引。这进一步证明数组方案的优势。

**Q3: 如果浏览历史非常长，内存是瓶颈怎么办？**
> 可以限制历史长度（如只保留最近 N 条），用环形数组(circular buffer)实现。超过 N 条时覆盖最老的记录。这是 LRU 思想的变体。

**Q4: 如果要支持多标签页呢？**
> 每个标签页维护独立的 BrowserHistory 实例。用 HashMap<tabId, BrowserHistory> 管理。

## 相关题型

- **155. 最小栈 (Min Stack)** — 同样是设计题，复用"辅助数据结构维护额外信息"的思路。区别：Min Stack 用辅助栈维护最小值，本题用双指针/双栈维护位置。
- **146. LRU 缓存** — 同为设计题，复用"多种数据结构组合"的设计思想。LRU = HashMap + 双向链表，本题的链表方案可以作为热身。
- **706. 设计哈希映射** — 基础设计题，练习"选择底层数据结构实现接口"的能力。