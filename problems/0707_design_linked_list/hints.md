# 707. 设计链表

## 核心思路

这道题本质上是让你**从零实现链表数据结构**，考察你对链表基本操作（增删查）的理解以及指针操作的准确性。核心难点不在算法，而在**边界条件处理**和**指针操作顺序**。

## 思维链

1. **读完题第一反应**：需要实现一个链表类，支持 get/addAtHead/addAtTail/addAtIndex/deleteAtIndex 五个操作。最直接的想法是用单链表。
2. **单链表的痛点是什么？** 插入和删除需要找到"前一个节点"，如果要在头部操作，头节点没有"前一个节点"，需要特殊处理。
3. **怎么统一处理？** 引入**虚拟头节点（dummy head）**，这样所有节点（包括真正的第一个节点）都有前驱，插入/删除逻辑统一。
4. **能否更进一步？** 用**双向链表**，可以从两端遍历，`get` 和其他操作可以选择从 head 或 tail 出发，靠近尾部的节点不用从头遍历。
5. **维护 `size`**：维护一个链表长度变量，可以 O(1) 判断 index 是否有效，还能决定从头还是从尾开始遍历。

## 解法概览

| 解法 | 思路 | get | add/delete | 空间 | 面试 |
|------|------|-----|------------|------|------|
| 单链表 + dummy head | 维护虚拟头节点和 size | O(n) | O(n) | O(1) 额外 | ⭐ 必须写出 |
| 双向链表 + dummy head/tail | 双向遍历，可从头或尾出发 | O(n)，实际最多 n/2 | O(n)，实际最多 n/2 | O(1) 额外 | 加分项 |

## 关键提示

1. **使用虚拟头节点（dummy head）** — 不用它的话，`addAtHead` 和 `deleteAtIndex(0)` 需要单独处理头指针更新，极易出错。

2. **维护 `size` 变量** — 每次 add 时 `size++`，delete 时 `size--`。这样判断 index 是否越界只需 `index < 0 || index >= size`，不需要遍历到末尾。

3. **`addAtIndex` 的关键**：找到第 `index` 个节点的**前驱**，然后在前驱之后插入。有了 dummy head，第 0 个节点的前驱就是 dummy。

4. **指针操作顺序图**（以插入为例）：

```
// 在 index=1 处插入 val=2
//
// 插入前: dummy -> 1 -> 3 -> NULL
//                  ↑
//                 prev (index=0 的前驱就是 dummy，index=1 的前驱是节点1)
//
// Step1: newNode->next = prev->next     (newNode->next = 3)
// Step2: prev->next = newNode           (1->next = newNode)
//
// 插入后: dummy -> 1 -> 2 -> 3 -> NULL
```

5. **双向链表的额外注意点**：插入/删除时要同时维护 `prev` 和 `next` 两个方向的指针，共 4 条指针赋值语句，顺序必须正确。

## 解法详解

### 解法1: 单链表 + 虚拟头节点 — O(n) / O(1) ⭐ 面试首选

**思考过程**: 单链表是最基础的实现。关键技巧是加一个 dummy head，这样所有操作都统一为"找到前驱节点，然后操作"。

```
// 链表结构示意 (dummy head):
//
//   dummy -> node0 -> node1 -> node2 -> NULL
//     ↑
//   head_ (始终指向 dummy，不存储有效数据)
//
// 要 get(1):  从 dummy 走 1+1=2 步到 node1
// 要 add(1):  从 dummy 走 1 步到 node0 (前驱)，在其后插入
// 要 delete(1): 从 dummy 走 1 步到 node0 (前驱)，跳过 node1
```

```cpp
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int v) : val(v), next(nullptr) {}
};

class MyLinkedList {
public:
    MyLinkedList() {
        dummy_ = new ListNode(0);  // 虚拟头节点，不存有效数据
        size_ = 0;
    }
    
    int get(int index) {
        if (index < 0 || index >= size_) return -1;
        ListNode* cur = dummy_->next;  // 第0个真实节点
        for (int i = 0; i < index; i++) {
            cur = cur->next;
        }
        return cur->val;
    }
    
    void addAtHead(int val) {
        addAtIndex(0, val);  // 复用 addAtIndex，减少重复代码
    }
    
    void addAtTail(int val) {
        addAtIndex(size_, val);  // index == size 时追加到末尾
    }
    
    void addAtIndex(int index, int val) {
        if (index > size_) return;       // 严格大于 size，不插入
        if (index < 0) index = 0;        // 负数视为0
        ListNode* prev = dummy_;
        for (int i = 0; i < index; i++) {
            prev = prev->next;           // 走到 index 的前驱
        }
        ListNode* newNode = new ListNode(val);
        newNode->next = prev->next;      // 先接上后面的链
        prev->next = newNode;            // 再让前驱指向新节点
        size_++;
    }
    
    void deleteAtIndex(int index) {
        if (index < 0 || index >= size_) return;
        ListNode* prev = dummy_;
        for (int i = 0; i < index; i++) {
            prev = prev->next;           // 走到 index 的前驱
        }
        ListNode* toDelete = prev->next;
        prev->next = toDelete->next;     // 跳过被删节点
        delete toDelete;                 // 释放内存
        size_--;
    }

private:
    ListNode* dummy_;  // 虚拟头节点
    int size_;         // 链表长度
};
```

**关键点**：
- `addAtHead` 和 `addAtTail` 都复用 `addAtIndex`，避免重复逻辑
- 找前驱时从 `dummy_` 出发走 `index` 步，而不是 `index-1` 步（因为 dummy 本身就多占一位）
- 插入时先 `newNode->next = prev->next`，再 `prev->next = newNode`，顺序不能反


### 解法2: 双向链表 + 虚拟头尾节点 — O(n) / O(1)（加分项）

**从解法1优化**: 单链表的痛点是每次都要从头遍历。如果 `index` 接近链表末尾（比如 `index = size - 1`），单链表要走 n 步，而双向链表可以从尾部出发只走 1 步。用两个虚拟节点（dummy head 和 dummy tail）让首尾操作也统一。

```
// 双向链表结构示意:
//
//   dummyHead <-> node0 <-> node1 <-> node2 <-> dummyTail
//     ↑                                           ↑
//   head_                                        tail_
//
// 要 get(2):  index=2 > size/2? 如果是，从 tail_ 往前走
//             否则从 head_ 往后走
//
// 插入 node 在 prev 和 succ 之间:
//   Step1: node->prev = prev
//   Step2: node->next = succ
//   Step3: prev->next = node
//   Step4: succ->prev = node
//
// 删除 node (已知 node):
//   Step1: prev = node->prev
//   Step2: succ = node->next
//   Step3: prev->next = succ
//   Step4: succ->prev = prev
```

```cpp
struct DListNode {
    int val;
    DListNode* prev;
    DListNode* next;
    DListNode(int v) : val(v), prev(nullptr), next(nullptr) {}
};

class MyLinkedList {
public:
    MyLinkedList() {
        head_ = new DListNode(0);   // 虚拟头
        tail_ = new DListNode(0);   // 虚拟尾
        head_->next = tail_;
        tail_->prev = head_;
        size_ = 0;
    }
    
    int get(int index) {
        if (index < 0 || index >= size_) return -1;
        DListNode* cur = getNode(index);
        return cur->val;
    }
    
    void addAtHead(int val) {
        addAfter(head_, val);  // 在虚拟头之后插入
    }
    
    void addAtTail(int val) {
        addAfter(tail_->prev, val);  // 在虚拟尾的前驱之后插入
    }
    
    void addAtIndex(int index, int val) {
        if (index > size_) return;
        if (index < 0) index = 0;
        // 找到 index 位置的前驱节点
        DListNode* prev;
        if (index == size_) {
            prev = tail_->prev;       // 追加到末尾
        } else {
            prev = getNode(index)->prev;  // index 节点的前驱
        }
        addAfter(prev, val);
    }
    
    void deleteAtIndex(int index) {
        if (index < 0 || index >= size_) return;
        DListNode* node = getNode(index);
        removeNode(node);
    }

private:
    DListNode* head_;  // 虚拟头
    DListNode* tail_;  // 虚拟尾
    int size_;
    
    // 根据 index 靠近头还是尾，选择遍历方向
    DListNode* getNode(int index) {
        DListNode* cur;
        if (index < size_ / 2) {
            // 从头往后走
            cur = head_->next;
            for (int i = 0; i < index; i++) cur = cur->next;
        } else {
            // 从尾往前走 — 这就是双向链表的优势
            cur = tail_->prev;
            for (int i = size_ - 1; i > index; i--) cur = cur->prev;
        }
        return cur;
    }
    
    // 在 prev 之后插入新节点
    void addAfter(DListNode* prev, int val) {
        DListNode* newNode = new DListNode(val);
        DListNode* succ = prev->next;
        // 四条指针赋值，顺序很重要
        newNode->prev = prev;
        newNode->next = succ;
        prev->next = newNode;   // 先设好 newNode 的指针，再修改邻居的指针
        succ->prev = newNode;
        size_++;
    }
    
    // 删除指定节点
    void removeNode(DListNode* node) {
        DListNode* prev = node->prev;
        DListNode* succ = node->next;
        prev->next = succ;
        succ->prev = prev;
        delete node;
        size_--;
    }
};
```

**关键点**：
- `getNode` 根据 `index < size/2` 选择从头还是从尾遍历，最坏只需 n/2 步
- 插入/删除抽成 `addAfter` 和 `removeNode` 工具函数，所有 API 都复用
- 双虚拟节点让首尾操作无需特判

## 解法对比

| | 单链表 + dummy head | 双向链表 + dummy head/tail |
|---|---|---|
| 实现复杂度 | 简单，约 40 行 | 中等，约 70 行 |
| get 最坏 | O(n) | O(n/2) |
| 指针维护 | 每次操作 2 条指针 | 每次操作 4 条指针 |
| 空间 | 每节点 1 个 next 指针 | 每节点 2 个指针 (prev + next) |
| 面试推荐 | ⭐ 首选 — 简洁不易错 | 被追问"如何优化"时给出 |

**什么时候选哪个？**
- 面试时间紧张：写单链表，bug 少、好调试
- 面试官追问优化，或题目操作大量集中在尾部：双向链表

## 易错点

1. **`addAtIndex` 漏掉 `index == size` 的情况**
   - ✗ `if (index >= size_) return;` — 当 `index == size` 时应该追加到末尾，不是拒绝
   - ✓ `if (index > size_) return;` — 只有严格大于 size 才拒绝

2. **`addAtIndex` 漏掉 `index < 0` 的处理**
   - ✗ 不检查负数，直接 for 循环走 index 步 — 负数 index 不进循环，变成在 dummy 后面插入（等同于 addAtHead），但这恰好是题意要求的行为？其实题目说 `0 <= index`，但为安全起见可以 `if (index < 0) index = 0;`

3. **插入时指针赋值顺序错误**
   - ✗ `prev->next = newNode; newNode->next = prev->next;` — 此时 `prev->next` 已经被改成 `newNode`，所以 `newNode->next = newNode`，形成自环！
   - ✓ 先 `newNode->next = prev->next;`，再 `prev->next = newNode;`

4. **忘记维护 `size_`**
   - 每次 add 必须 `size_++`，每次 delete 必须 `size_--`
   - 否则后续的 index 有效性判断全部失效

5. **双向链表删除时忘记更新 `prev` 指针**
   - ✗ 只做了 `prev->next = succ;`，漏掉了 `succ->prev = prev;`
   - ✓ 必须双向都更新，否则从尾部遍历时会访问到已删除的节点

## 面试追问

**Q1: 为什么要用虚拟头节点？不用行不行？**
> 不用也可以，但每个操作（尤其是 `addAtHead` 和 `deleteAtIndex(0)`）都需要单独判断是否在修改头指针。虚拟头节点让所有节点都有前驱，插入/删除逻辑统一，代码更简洁、更不容易出 bug。

**Q2: 如果操作大量集中在尾部（如频繁 `addAtTail`），单链表怎么优化？**
> 可以维护一个 `tail_` 指针，`addAtTail` 变成 O(1)。但 `deleteAtIndex(size-1)` 仍然是 O(n)，因为单链表找不到尾节点的前驱。要解决这个问题，就需要升级到双向链表。

**Q3: 如果要求实现 `addAtTail` 和 `deleteAtTail` 都是 O(1)，怎么办？**
> 使用双向链表 + 虚拟头尾节点。`addAtTail` 在 `dummyTail->prev` 之后插入，`deleteAtTail` 删除 `dummyTail->prev`，都是 O(1)。这就是解法2。

**Q4: 如何处理内存泄漏？**
> 在析构函数中遍历链表，逐个 `delete` 每个节点。面试中提一嘴"实际工程中会加 destructor"即可，不需要写完整代码，但要意识到这个问题。

## 相关题型

- **206. 反转链表** — 复用单链表的指针操作基础，练习"三指针翻转"
- **21. 合并两个有序链表** — 复用 dummy head 技巧统一头节点处理
- **146. LRU 缓存** — 双向链表 + 哈希表的经典组合设计题，复用本题的 `addAfter` / `removeNode` 工具函数
- **460. LFU 缓存** — 在 LRU 基础上进一步，多个双向链表的设计