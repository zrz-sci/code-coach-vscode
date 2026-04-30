/*
 * LeetCode 641: 设计循环双端队列 (Design Circular Deque)
 *
 * 标签: Design, Queue, Array, Linked List
 * 难度: Medium
 *
 * 三种解法:
 * 1. 固定数组 + 浪费一格法  — O(1) 所有操作, 面试首选
 * 2. 固定数组 + size 计数   — O(1) 所有操作, 不浪费空间
 * 3. 双向链表              — O(1) 所有操作, 展示链表功底
 */



// ============================================================
// 解法1: 固定数组 + 浪费一格法 — ⭐ 面试首选
//
// 核心设计:
//   数组大小 cap = k + 1 (多1格区分空满)
//   front 指向队头元素
//   rear  指向队尾元素的下一个空位
//   isEmpty: front == rear
//   isFull:  (rear + 1) % cap == front
//
// 为什么浪费一格？
//   cap=k 时: 空 → front==rear, 满 → front==rear → 无法区分!
//   cap=k+1: 空 → front==rear, 满 → (rear+1)%cap==front → 可区分
//
// 指针移动规则:
//   前进(+1): x = (x + 1) % cap
//   后退(-1): x = (x - 1 + cap) % cap  (加 cap 防 C++ 负数取模)
//
// insertFront: 先退 front 再写  (front 指向有效元素, 前面要腾位置)
// insertLast:  先写再进 rear   (rear 指向空位, 直接写然后前进)
//
// 结构图 (k=3, cap=4, 元素为 [3,1,2]):
//
//   ┌─────┬─────┬─────┬─────┐
//   │  1  │  2  │     │  3  │
//   └─────┴─────┴─────┴─────┘
//   idx: 0     1     2     3
//                 ↑     ↑
//                 R     F
//
//   逻辑顺序: Front→[3]→[1]→[2]←Rear
//   isEmpty: 3 != 2 → false
//   isFull: (2+1)%4=3 == front=3 → true
//   getFront: data[3] = 3
//   getRear: data[(2-1+4)%4] = data[1] = 2
// ============================================================
class MyCircularDeque_v1 {
    vector<int> data;
    int front, rear, cap;
public:
    // 构造: cap = k+1, 多出的 1 格用于区分空满
    MyCircularDeque_v1(int k) : data(k + 1), front(0), rear(0), cap(k + 1) {}

    /*
     * insertFront: 在队头插入元素
     *   1. 检查是否已满
     *   2. front 后退一步 (腾出位置)
     *   3. 写入 data[front]
     *
     * 注意: 先移后写! 因为 front 当前指向有效元素
     */
    bool insertFront(int value) {
        if (isFull()) return false;
        front = (front - 1 + cap) % cap; // 后退 (+cap 防负数)
        data[front] = value;
        return true;
    }

    /*
     * insertLast: 在队尾插入元素
     *   1. 检查是否已满
     *   2. 写入 data[rear] (rear 当前就是空位)
     *   3. rear 前进一步
     *
     * 注意: 先写后移! 因为 rear 当前指向空位
     */
    bool insertLast(int value) {
        if (isFull()) return false;
        data[rear] = value;
        rear = (rear + 1) % cap; // 前进
        return true;
    }

    /*
     * deleteFront: 删除队头
     *   front 前进一步即可, 不需要清零数据
     *   (下次写入会自然覆盖)
     */
    bool deleteFront() {
        if (isEmpty()) return false;
        front = (front + 1) % cap;
        return true;
    }

    /*
     * deleteLast: 删除队尾
     *   rear 后退一步即可
     */
    bool deleteLast() {
        if (isEmpty()) return false;
        rear = (rear - 1 + cap) % cap;
        return true;
    }

    /*
     * getFront: 读取队头
     *   front 直接指向队头元素
     */
    int getFront() {
        return isEmpty() ? -1 : data[front];
    }

    /*
     * getRear: 读取队尾
     *   rear 指向空位! 队尾在 rear 前一个位置
     *   (rear - 1 + cap) % cap
     */
    int getRear() {
        return isEmpty() ? -1 : data[(rear - 1 + cap) % cap];
    }

    bool isEmpty() {
        return front == rear;
    }

    bool isFull() {
        return (rear + 1) % cap == front;
    }
};


// ============================================================
// 解法2: 固定数组 + size 计数 — 不浪费空间
//
// 核心改动:
//   数组大小 = k (不需要 k+1)
//   用 size 变量区分空满: isEmpty → sz==0, isFull → sz==maxSz
//   每次 insert 时 sz++, delete 时 sz--
//
// 优点: 不浪费一格空间
// 缺点: 每次操作需维护 sz; 多线程环境 sz 需要原子操作
// ============================================================
class MyCircularDeque_v2 {
    vector<int> data;
    int front, rear, sz, maxSz;
public:
    MyCircularDeque_v2(int k) : data(k), front(0), rear(0), sz(0), maxSz(k) {}

    bool insertFront(int value) {
        if (sz == maxSz) return false;
        front = (front - 1 + maxSz) % maxSz;
        data[front] = value;
        sz++;
        return true;
    }

    bool insertLast(int value) {
        if (sz == maxSz) return false;
        data[rear] = value;
        rear = (rear + 1) % maxSz;
        sz++;
        return true;
    }

    bool deleteFront() {
        if (sz == 0) return false;
        front = (front + 1) % maxSz;
        sz--;
        return true;
    }

    bool deleteLast() {
        if (sz == 0) return false;
        rear = (rear - 1 + maxSz) % maxSz;
        sz--;
        return true;
    }

    int getFront() {
        return sz == 0 ? -1 : data[front];
    }

    int getRear() {
        return sz == 0 ? -1 : data[(rear - 1 + maxSz) % maxSz];
    }

    bool isEmpty() { return sz == 0; }
    bool isFull() { return sz == maxSz; }
};


// ============================================================
// 解法3: 双向链表 — 展示链表功底
//
// 核心设计:
//   每个节点: {val, prev, next}
//   dummy head + dummy tail 哨兵节点简化边界
//   insertFront → 在 head 之后插入
//   insertLast  → 在 tail 之前插入
//   deleteFront → 删除 head->next
//   deleteLast  → 删除 tail->prev
//
// 结构图:
//   DummyHead <-> [3] <-> [1] <-> [2] <-> DummyTail
//     哨兵         队头                队尾     哨兵
//
// 优点: 概念清晰, 无需取模运算
// 缺点: 每节点多 2 指针, 内存大, 缓存不友好
//        需要手动管理 new/delete
// ============================================================
class MyCircularDeque_v3 {
    struct Node {
        int val;
        Node* prev;
        Node* next;
        Node(int v) : val(v), prev(nullptr), next(nullptr) {}
    };

    Node* head; // 哨兵头
    Node* tail; // 哨兵尾
    int sz, maxSz;

    // 在 node 之后插入新节点
    void insertAfter(Node* node, int val) {
        Node* newNode = new Node(val);
        newNode->next = node->next;
        newNode->prev = node;
        node->next->prev = newNode;
        node->next = newNode;
    }

    // 删除指定节点
    void removeNode(Node* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
        delete node;
    }

public:
    MyCircularDeque_v3(int k) : sz(0), maxSz(k) {
        head = new Node(-1); // dummy head
        tail = new Node(-1); // dummy tail
        head->next = tail;
        tail->prev = head;
    }

    ~MyCircularDeque_v3() {
        Node* cur = head;
        while (cur) {
            Node* next = cur->next;
            delete cur;
            cur = next;
        }
    }

    bool insertFront(int value) {
        if (sz == maxSz) return false;
        insertAfter(head, value); // 在哨兵头之后插入
        sz++;
        return true;
    }

    bool insertLast(int value) {
        if (sz == maxSz) return false;
        insertAfter(tail->prev, value); // 在哨兵尾之前插入
        sz++;
        return true;
    }

    bool deleteFront() {
        if (sz == 0) return false;
        removeNode(head->next); // 删除哨兵头之后的第一个节点
        sz--;
        return true;
    }

    bool deleteLast() {
        if (sz == 0) return false;
        removeNode(tail->prev); // 删除哨兵尾之前的最后一个节点
        sz--;
        return true;
    }

    int getFront() { return sz == 0 ? -1 : head->next->val; }
    int getRear()  { return sz == 0 ? -1 : tail->prev->val; }
    bool isEmpty() { return sz == 0; }
    bool isFull()  { return sz == maxSz; }
};


// ============================================================
// 主解法 (提交用) — 解法1: 浪费一格法
// ============================================================
class MyCircularDeque {
    vector<int> data;
    int front, rear, cap;
public:
    MyCircularDeque(int k) : data(k + 1), front(0), rear(0), cap(k + 1) {}

    bool insertFront(int value) {
        if (isFull()) return false;
        front = (front - 1 + cap) % cap;
        data[front] = value;
        return true;
    }

    bool insertLast(int value) {
        if (isFull()) return false;
        data[rear] = value;
        rear = (rear + 1) % cap;
        return true;
    }

    bool deleteFront() {
        if (isEmpty()) return false;
        front = (front + 1) % cap;
        return true;
    }

    bool deleteLast() {
        if (isEmpty()) return false;
        rear = (rear - 1 + cap) % cap;
        return true;
    }

    int getFront() {
        return isEmpty() ? -1 : data[front];
    }

    int getRear() {
        return isEmpty() ? -1 : data[(rear - 1 + cap) % cap];
    }

    bool isEmpty() {
        return front == rear;
    }

    bool isFull() {
        return (rear + 1) % cap == front;
    }
};


// ============================================================
// 【面试追问】
// ============================================================
// Q1: 浪费一格法 vs size 计数法哪个更好？
//     → 浪费一格法无额外变量、多线程环境下无原子性问题
// Q2: 如果要支持动态扩容怎么办？
//     → 类似 vector 2 倍扩容: isFull 时分配 2*cap 新数组，
//       从 front 开始按逻辑顺序复制，重置 front=0, rear=size
// Q3: 多线程安全的循环队列怎么设计？
//     → Lock-Free Ring Buffer: 原子变量存 front/rear，CAS 操作无锁并发
//       Linux 内核的 kfifo 就是这种设计
//
// 【易错点】
// 1. C++ 负数取模: (front-1) % cap 可能为负，必须 (front-1+cap) % cap ✗
// 2. insertFront 先写后移 → 覆盖了当前头元素! 正确是先移后写 ✗
// 3. getRear 返回 data[rear] → rear 是空位! 正确是 data[(rear-1+cap)%cap] ✗
// 4. 忘记检查 isFull/isEmpty → insert 前必须检查满，delete/get 前必须检查空 ✗
// 5. cap 初始化为 k 而非 k+1 → 浪费一格法必须 k+1，否则空满无法区分 ✗
