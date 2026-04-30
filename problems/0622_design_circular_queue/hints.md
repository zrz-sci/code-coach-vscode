# 622. 设计循环队列

## 核心思路

本质上是实现一个**固定大小的环形缓冲区（Ring Buffer）**：用取模运算让数组的尾部"接上"头部，从而复用已出队元素腾出的空间。

## 思维链

1. **读完题第一反应**：循环队列就是一个固定容量的队列，出队后腾出的空间可以被再次利用。最朴素的想法——用数组 + 两个指针（head/tail）来管理。

2. **普通队列的瓶颈**：如果用普通数组实现队列，head 和 tail 不断右移，即使前面的空间已经空了也无法利用，最终 tail 越界就"假满"了。

3. **怎么突破**：让指针"绕回来"！当 tail 到达数组末尾时，让它回到索引 0。这正好是取模运算做的事：`nextIndex = (currentIndex + 1) % capacity`。

4. **空/满怎么区分**：head == tail 时到底是空还是满？两种经典方案：
   - **方案 A**：多分配一个位置（容量 k+1），满的条件变成 `(tail+1) % (k+1) == head`，空的条件是 `tail == head`。
   - **方案 B**：用一个额外的 `count` 变量记录当前元素个数，count==0 是空，count==k 是满。

5. **还有别的实现吗**？可以用链表实现，天然不需要预分配大小，但需要额外维护节点数。

6. **选哪个**？数组 + count 最直观；数组 + 多一格空间更经典（操作系统内核的环形缓冲区就这么做）；链表实现作为面试加分项。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 数组 + count 计数 | 用 count 区分空/满 | O(1) 每操作 | O(k) | ⭐ 必须写出 |
| 数组 + 浪费一格 | 容量 k+1，空/满靠指针位置区分 | O(1) 每操作 | O(k) | 加分项 |
| 链表实现 | 单向链表 + count | O(1) 每操作 | O(k) | 能说出即可 |

## 关键提示

1. **取模是核心操作**：任何时候指针移动都用 `(index + 1) % capacity`，千万别自己 if/else 判断边界。

2. **Rear() 不是 tail 指向的位置**：tail 通常指向**下一个要写入的位置**（空位），所以队尾元素是 `(tail - 1 + capacity) % capacity`。这是最容易写错的地方！

3. **空/满判断是设计的灵魂**：先确定你选哪种方案（count 还是浪费一格），所有操作都围绕这个决定展开。

4. **ASCII 图示理解环形**：
```
容量 k=3, 数组大小=3, 用 count 方案:

初始:       enQueue(1):   enQueue(2):   enQueue(3):   deQueue():    enQueue(4):
[_, _, _]   [1, _, _]     [1, 2, _]     [1, 2, 3]     [_, 2, 3]     [4, 2, 3]
 ↑           ↑              ↑              ↑               ↑            ↑
h,t         h  t           h     t        h  t(绕回0)     h  t(不变)   t  h
count=0     count=1        count=2        count=3        count=2      count=3
                                          isFull!                     tail绕回来了!
```

5. **链表实现的关键**：需要同时维护 head 和 tail 指针。Rear() 直接返回 tail->val，不需要遍历。

## 解法详解

### 解法1: 数组 + count 计数 — O(1) / O(k) ⭐ 面试首选

**思考过程**：用固定大小数组存元素，head 指向队首，tail 指向下一个插入位置。用 count 变量追踪当前元素个数，这样空（count==0）和满（count==k）的判断一目了然，不需要在指针关系上动脑筋。

```cpp
class MyCircularQueue {
    vector<int> data;
    int head;    // 队首元素的索引
    int tail;    // 下一个插入位置的索引
    int count;   // 当前元素个数
    int capacity;
public:
    MyCircularQueue(int k) : data(k), head(0), tail(0), count(0), capacity(k) {}
    
    bool enQueue(int value) {
        if (isFull()) return false;
        data[tail] = value;
        tail = (tail + 1) % capacity;  // 取模实现"绕回"
        count++;
        return true;
    }
    
    bool deQueue() {
        if (isEmpty()) return false;
        head = (head + 1) % capacity;  // head 前进，旧元素自然被"逻辑删除"
        count--;
        return true;
    }
    
    int Front() {
        return isEmpty() ? -1 : data[head];
    }
    
    int Rear() {
        // tail 指向空位，队尾是 tail 的前一个位置
        // (tail - 1 + capacity) % capacity 防止 tail=0 时减成负数
        return isEmpty() ? -1 : data[(tail - 1 + capacity) % capacity];
    }
    
    bool isEmpty() { return count == 0; }
    bool isFull()  { return count == capacity; }
};
```

**关键点**：
- `Rear()` 中 `(tail - 1 + capacity) % capacity` 的 `+ capacity` 是为了防止 `tail=0` 时得到负数索引。
- `deQueue` 不需要真的清除数组中的值，只需移动 head 指针即可。

---

### 解法2: 数组 + 浪费一格 — O(1) / O(k)

**从解法1优化**：解法1用 count 变量区分空和满。另一种经典方法是：分配 k+1 大小的数组，但只存 k 个元素。这样 `head == tail` 一定是空，`(tail+1) % (k+1) == head` 一定是满。**不需要 count 变量**，在并发场景下更友好（只需一个读指针一个写指针，无需额外锁 count）。

```cpp
class MyCircularQueue {
    vector<int> data;
    int head;
    int tail;
    int cap;  // = k + 1，数组实际大小
public:
    MyCircularQueue(int k) : data(k + 1), head(0), tail(0), cap(k + 1) {}
    
    bool enQueue(int value) {
        if (isFull()) return false;
        data[tail] = value;
        tail = (tail + 1) % cap;
        return true;
    }
    
    bool deQueue() {
        if (isEmpty()) return false;
        head = (head + 1) % cap;
        return true;
    }
    
    int Front() {
        return isEmpty() ? -1 : data[head];
    }
    
    int Rear() {
        return isEmpty() ? -1 : data[(tail - 1 + cap) % cap];
    }
    
    // head == tail 表示空（因为有浪费的一格，满时 tail 和 head 之间隔一格）
    bool isEmpty() { return head == tail; }
    
    // tail 的下一个位置是 head，说明中间已经存了 k 个元素
    bool isFull()  { return (tail + 1) % cap == head; }
};
```

**关键点**：
- 数组大小是 `k+1` 而不是 `k`，这一格是"故意浪费"的。
- 这种方法在操作系统内核的环形缓冲区（如 Linux kfifo）中被广泛使用，因为**生产者只改 tail，消费者只改 head**，天然线程安全（单生产者单消费者场景）。

---

### 解法3: 链表实现 — O(1) / O(k)

**不同视角**：不用数组，用单向链表。好处是不需要预分配连续内存，动态分配节点。需要维护 head、tail 指针和 count。

```cpp
class MyCircularQueue {
    struct ListNode {
        int val;
        ListNode* next;
        ListNode(int v) : val(v), next(nullptr) {}
    };
    
    ListNode* head;
    ListNode* tail;
    int count;
    int capacity;
    
public:
    MyCircularQueue(int k) : head(nullptr), tail(nullptr), count(0), capacity(k) {}
    
    bool enQueue(int value) {
        if (isFull()) return false;
        ListNode* node = new ListNode(value);
        if (isEmpty()) {
            head = tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
        // 链表版的"循环"：tail 指向 head，形成环
        // （这里可以不连，因为我们用 count 管理空/满，但保持环形语义）
        tail->next = head;
        count++;
        return true;
    }
    
    bool deQueue() {
        if (isEmpty()) return false;
        if (count == 1) {
            // 只剩一个节点
            delete head;
            head = tail = nullptr;
        } else {
            ListNode* toDelete = head;
            head = head->next;
            tail->next = head;  // 维持环形
            delete toDelete;
        }
        count--;
        return true;
    }
    
    int Front() {
        return isEmpty() ? -1 : head->val;
    }
    
    int Rear() {
        return isEmpty() ? -1 : tail->val;
    }
    
    bool isEmpty() { return count == 0; }
    bool isFull()  { return count == capacity; }
    
    ~MyCircularQueue() {
        // 防止内存泄漏
        while (!isEmpty()) deQueue();
    }
};
```

**关键点**：
- 链表版的 `Rear()` 直接返回 `tail->val`，O(1)，这是维护 tail 指针的意义。
- 需要注意内存管理，`deQueue` 时要 `delete` 节点，析构函数要清理所有节点。

## 解法对比

| | 数组+count | 数组+浪费一格 | 链表 |
|---|---|---|---|
| 空间 | 恰好 k | k+1 | 动态分配，最多 k 个节点 |
| 空/满判断 | count == 0 / count == k | head==tail / (tail+1)%cap==head | count |
| 并发友好 | 需要锁 count | 单生产者单消费者无锁 | 需要锁 |
| 实现复杂度 | 最简单 | 稍复杂（取模多一层） | 需要内存管理 |
| **面试推荐** | ✅ 首选 | 追问时展示 | 追问链表实现时 |

## 易错点

1. **Rear() 计算错误**
   - ✗ `data[tail]` — tail 指向的是**空位**，不是队尾元素！
   - ✓ `data[(tail - 1 + capacity) % capacity]` — 队尾是 tail 的前一格

2. **取模时负数索引**
   - ✗ `data[(tail - 1) % capacity]` — 当 tail=0 时，`-1 % capacity` 在 C++ 中是 -1（未定义行为/负数），不是你期望的 capacity-1
   - ✓ `data[(tail - 1 + capacity) % capacity]` — 先加 capacity 保证非负

3. **浪费一格方案忘记数组多分配一格**
   - ✗ `vector<int> data(k)` — 满时 tail 和 head 重合，和空无法区分
   - ✓ `vector<int> data(k + 1)` — 故意浪费一格来区分空和满

4. **链表版 deQueue 只剩一个元素时没有处理 tail**
   - ✗ 只 `head = head->next`，但 tail 仍指向已删除的节点 → 悬挂指针
   - ✓ `count == 1` 时特殊处理：`head = tail = nullptr`

## 面试追问

**Q1: 基础理解 — 为什么叫"循环"队列？和普通队列有什么区别？**
> 普通队列出队后前面的空间浪费了（或者需要 O(n) 搬移），循环队列用取模运算让指针"绕回"数组开头，复用已出队的空间。所有操作保持 O(1)。

**Q2: 优化追问 — 如果在多线程环境下使用这个循环队列，怎么保证线程安全？**
> 方案一：所有操作加互斥锁（最简单但性能差）。方案二：用"浪费一格"的方案，在**单生产者单消费者**场景下，生产者只写 tail、消费者只读 head，利用原子操作可以实现无锁（lock-free）。这就是为什么 Linux kfifo 选择浪费一格的设计。

**Q3: 变体 — 如果要求支持动态扩容怎么办？**
> 类似 `vector` 的策略：当满时分配一个 2 倍大小的新数组，把旧数据按 head→tail 的逻辑顺序复制到新数组（注意不是直接 memcpy，因为环形数据可能跨越数组边界），然后重置 head=0, tail=count。扩容操作 O(n)，但均摊仍是 O(1)。

## 相关题型

- **641. 设计循环双端队列** — 复用本题的环形数组框架，区别：增加 `insertFront` 和 `deleteLast` 操作，需要让 head 往前移（`head = (head - 1 + cap) % cap`），本质是双向移动指针。
- **346. 数据流中的移动平均值** — 复用本题的环形缓冲区，固定窗口大小 = 队列容量，新数据进来时覆盖最旧的数据，维护窗口和。
- **1670. 设计前中后队列** — 更复杂的设计题，需要两个双端队列拼接，但核心的"环形数组+取模"思想相同。