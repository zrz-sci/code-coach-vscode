/*
 * LeetCode 622: 设计循环队列 (Design Circular Queue)
 *
 * 【题目本质】
 * 实现一个固定容量的环形缓冲区（Ring Buffer），所有操作 O(1)，
 * 核心难点：用取模运算让数组首尾相连，正确区分空和满。
 *
 * 【解法总览】
 * 解法1: 数组 + count 计数   — O(1) / O(k) — 最直观，面试首选
 * 解法2: 数组 + 浪费一格     — O(1) / O(k) — 经典 OS 内核做法，并发友好
 * 解法3: 链表实现            — O(1) / O(k) — 面试追问变体
 */

// ============================================================
// 解法1: 数组 + count 计数 — 最直观，面试首选
// 时间: O(1) 每操作   空间: O(k)
//
// 【思路】
// 最自然的想法：数组存数据，head 指向队首，tail 指向下一个插入位置。
// 关键问题：head == tail 时到底是空还是满？
// 最简单的解决方案：用 count 变量记录当前元素个数。
// count==0 → 空，count==k → 满。干净利落，不容易出错。
//
// 环形数组示意（容量 k=4）:
//
//   索引:  0   1   2   3
//        +---+---+---+---+
//        | 5 |   | 3 | 4 |    head=2, tail=1, count=3
//        +---+---+---+---+
//          ↑       ↑
//         tail    head
//
//   逻辑顺序: 3(head) → 4 → 5 → _(tail, 下一个插入位置)
//   tail 在 head "前面"（取模意义下），这就是"循环"
//
// ============================================================
class Solution1 {
public:
    class MyCircularQueue {
        vector<int> data;
        int head;      // 队首元素索引
        int tail;      // 下一个要插入的位置索引
        int count;     // 当前元素个数
        int capacity;  // 队列容量 k
        
    public:
        MyCircularQueue(int k) : data(k), head(0), tail(0), count(0), capacity(k) {}
        
        bool enQueue(int value) {
            if (isFull()) return false;
            data[tail] = value;
            // 取模让 tail 从末尾绕回到开头，这是"循环"的核心
            tail = (tail + 1) % capacity;
            count++;
            return true;
        }
        
        bool deQueue() {
            if (isEmpty()) return false;
            // 不需要清除 data[head] 的值，逻辑上已经不在队列中了
            head = (head + 1) % capacity;
            count--;
            return true;
        }
        
        int Front() {
            return isEmpty() ? -1 : data[head];
        }
        
        int Rear() {
            // 易错点！tail 指向空位，队尾元素在 tail 的前一格
            // +capacity 防止 tail=0 时变成负数索引
            return isEmpty() ? -1 : data[(tail - 1 + capacity) % capacity];
        }
        
        bool isEmpty() {
            return count == 0;
        }
        
        bool isFull() {
            return count == capacity;
        }
    };
};

// ============================================================
// 解法2: 数组 + 浪费一格 — 经典 OS 内核做法
// 时间: O(1) 每操作   空间: O(k)
//
// 【思路】
// 解法1 用 count 变量来区分空和满，简单但有个缺点：
// 在多线程场景中，enQueue 和 deQueue 都要修改 count，需要加锁。
//
// 经典的替代方案：分配 k+1 大小的数组，但只存 k 个元素。
// - 空: head == tail
// - 满: (tail + 1) % (k+1) == head  （tail 的下一格就是 head）
//
// 这样生产者只改 tail，消费者只改 head，在单生产者单消费者场景下
// 天然线程安全（配合内存屏障），Linux kfifo 就是这么实现的。
//
// 示意（k=3, 数组大小=4）:
//
//   空:    [_, _, _, _]   head=0, tail=0, head==tail → 空
//   存3个: [1, 2, 3, _]  head=0, tail=3, (3+1)%4==0==head → 满
//                    ↑ 这个位置永远不存数据（"浪费"的一格）
// ============================================================
class Solution2 {
public:
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
        
        bool isEmpty() {
            // 不需要 count，纯靠指针位置判断
            return head == tail;
        }
        
        bool isFull() {
            // tail 的下一格是 head，说明中间已存满 k 个元素
            return (tail + 1) % cap == head;
        }
    };
};

// ============================================================
// 解法3: 链表实现 — 追问变体
// 时间: O(1) 每操作   空间: O(k)
//
// 【思路】
// 面试官可能追问"不用数组怎么做？"
// 用单向链表，维护 head 和 tail 指针。
// 优点：不需要预分配连续内存。
// 缺点：每个节点有指针开销，且需要手动管理内存。
//
// 链表结构示意:
//   head → [1] → [2] → [3] → head (tail.next 指回 head，形成环)
//                        ↑
//                       tail
//
//   Rear() 直接返回 tail->val，O(1)
//   Front() 直接返回 head->val，O(1)
// ============================================================
class Solution3 {
public:
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
            // 让 tail 指回 head，形成环（保持"循环"语义）
            tail->next = head;
            count++;
            return true;
        }
        
        bool deQueue() {
            if (isEmpty()) return false;
            if (count == 1) {
                // 易错点：只剩一个节点时，必须同时重置 head 和 tail
                // 如果只处理 head，tail 会变成悬挂指针
                delete head;
                head = tail = nullptr;
            } else {
                ListNode* toDelete = head;
                head = head->next;
                tail->next = head;  // 维持环形：tail 指向新的 head
                delete toDelete;
            }
            count--;
            return true;
        }
        
        int Front() {
            return isEmpty() ? -1 : head->val;
        }
        
        int Rear() {
            // 维护了 tail 指针，所以 O(1) 直接拿
            return isEmpty() ? -1 : tail->val;
        }
        
        bool isEmpty() { return count == 0; }
        bool isFull()  { return count == capacity; }
        
        // 析构函数防止内存泄漏
        ~MyCircularQueue() {
            while (!isEmpty()) deQueue();
        }
    };
};

// ============================================================
// 【解法对比】
//
// | 维度         | 数组+count    | 数组+浪费一格  | 链表         |
// |-------------|--------------|---------------|-------------|
// | 空间         | 恰好 k       | k+1           | 动态分配     |
// | 空/满判断    | count        | 指针位置       | count       |
// | 并发友好     | 需锁 count   | 单生产者单消费者无锁 | 需锁   |
// | 实现难度     | ★☆☆         | ★★☆           | ★★☆        |
// | 面试推荐     | ✅ 首选       | 追问时展示      | 链表追问时   |
//
// 面试建议：先写解法1（count方案），写完后主动提"还有浪费一格的方案，
// 在并发场景下更优"，展示知识广度。
//
// 【易错点】
//
// 1. Rear() 取错位置:
//    ✗ return data[tail];        // tail 是空位！
//    ✓ return data[(tail - 1 + capacity) % capacity];
//
// 2. 取模时负数:
//    ✗ (tail - 1) % capacity     // C++ 中 -1 % 3 = -1，不是 2
//    ✓ (tail - 1 + capacity) % capacity  // +capacity 保证非负
//
// 3. 浪费一格方案数组大小写错:
//    ✗ vector<int> data(k)       // 满和空无法区分
//    ✓ vector<int> data(k + 1)   // 故意多分配一格
//
// 4. 链表版只剩一个节点时 deQueue:
//    ✗ head = head->next; (tail 仍指向已 delete 的节点 → 悬挂指针)
//    ✓ count==1 时特判: delete head; head = tail = nullptr;
//
// 5. 链表版忘记在 enQueue 后维护 tail->next = head:
//    导致不是真正的环形链表，某些遍历逻辑会出错
//
// 【面试追问】
//
// Q1（基础理解）: 为什么叫"循环"队列？普通队列有什么问题？
//   → 普通队列出队后前面的空间浪费了（或需要 O(n) 搬移数据），
//     循环队列用取模让指针"绕回"开头，复用空间，所有操作 O(1)。
//
// Q2（优化/设计）: 两种区分空/满的方案各有什么优缺点？
//   → count 方案最简单直观，但多线程下 enQueue 和 deQueue 都改 count 需要锁。
//     浪费一格方案多用 1 格空间，但生产者只改 tail、消费者只改 head，
//     在单生产者单消费者场景下可以无锁，OS 内核级别用的就是这种。
//
// Q3（变体）: 如果要支持动态扩容怎么办？
//   → 类似 vector 的倍增策略：满时分配 2 倍新数组，从 head 开始按逻辑
//     顺序拷贝到新数组（不能直接 memcpy，因为环形数据可能跨越边界），
//     然后 head=0, tail=count。单次扩容 O(n)，均摊 O(1)。
// ============================================================
