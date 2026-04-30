/*
 * LeetCode 707: 设计链表 (Design Linked List)
 *
 * 【题目本质】
 * 从零实现链表数据结构，支持按索引的增删查操作。
 * 核心不在算法，而在指针操作的准确性和边界条件处理。
 *
 * 【解法总览】
 * 解法1: 单链表 + 虚拟头节点    — get O(n) / add O(n) / delete O(n) — 面试首选
 * 解法2: 双向链表 + 虚拟头尾节点 — get O(n/2) / add O(n/2) / delete O(n/2) — 加分项
 */

// ============================================================
// 解法1: 单链表 + 虚拟头节点
// 时间: get/add/delete 均 O(n)   空间: O(1) 额外（不含链表本身）
//
// 【思路】
// 核心问题：插入/删除需要找到"前驱节点"，但头节点没有前驱。
// 解决方案：加一个虚拟头节点(dummy head)，这样所有节点统一有前驱。
// 同时维护 size_ 变量，O(1) 判断 index 是否有效。
//
// 链表结构:
//   dummy -> node0 -> node1 -> node2 -> NULL
//     ↑
//   head_ (不存有效数据，val=0)
//
// "找 index 的前驱" = 从 dummy 出发走 index 步
//
// 插入过程 (在 index=1 处插入 val=2):
//
//   Before:  dummy -> 1 -> 3 -> NULL
//                     ↑
//                    prev (从 dummy 走 1 步)
//
//   Step1:   newNode(2)->next = prev->next (= 3)
//   Step2:   prev->next = newNode(2)
//
//   After:   dummy -> 1 -> 2 -> 3 -> NULL
//
// 删除过程 (删除 index=1):
//
//   Before:  dummy -> 1 -> 2 -> 3 -> NULL
//                     ↑
//                    prev
//
//   Step1:   toDelete = prev->next (= 2)
//   Step2:   prev->next = toDelete->next (= 3)
//   Step3:   delete toDelete
//
//   After:   dummy -> 1 -> 3 -> NULL
// ============================================================

struct SListNode {
    int val;
    SListNode* next;
    SListNode(int v) : val(v), next(nullptr) {}
};

class MyLinkedList {
public:
    MyLinkedList() {
        dummy_ = new SListNode(0);
        size_ = 0;
    }
    
    // 析构函数：防止内存泄漏（面试中提一嘴即可）
    ~MyLinkedList() {
        SListNode* cur = dummy_;
        while (cur) {
            SListNode* next = cur->next;
            delete cur;
            cur = next;
        }
    }
    
    int get(int index) {
        // index 无效直接返回 -1
        if (index < 0 || index >= size_) return -1;
        
        SListNode* cur = dummy_->next;  // 第 0 个真实节点
        for (int i = 0; i < index; i++) {
            cur = cur->next;
        }
        return cur->val;
    }
    
    void addAtHead(int val) {
        // 复用 addAtIndex，在位置 0 插入 = 在头部插入
        addAtIndex(0, val);
    }
    
    void addAtTail(int val) {
        // index == size 时追加到末尾，这是 addAtIndex 的合法输入
        addAtIndex(size_, val);
    }
    
    void addAtIndex(int index, int val) {
        // 注意：index > size_ 才拒绝，index == size_ 是追加到末尾
        if (index > size_) return;
        if (index < 0) index = 0;
        
        // 从 dummy 出发走 index 步，到达 index 的前驱
        SListNode* prev = dummy_;
        for (int i = 0; i < index; i++) {
            prev = prev->next;
        }
        
        SListNode* newNode = new SListNode(val);
        // 顺序很关键：先接后面，再改前驱的 next
        // 如果反过来，prev->next 被改掉后就找不到原来的后继了
        newNode->next = prev->next;
        prev->next = newNode;
        size_++;
    }
    
    void deleteAtIndex(int index) {
        if (index < 0 || index >= size_) return;
        
        // 同样找前驱
        SListNode* prev = dummy_;
        for (int i = 0; i < index; i++) {
            prev = prev->next;
        }
        
        SListNode* toDelete = prev->next;
        prev->next = toDelete->next;  // 跳过被删节点
        delete toDelete;              // 释放内存
        size_--;
    }

private:
    SListNode* dummy_;  // 虚拟头节点
    int size_;          // 链表有效节点数
};

// ============================================================
// 解法2: 双向链表 + 虚拟头尾节点
// 时间: get/add/delete 均 O(n)，但实际最多遍历 n/2
// 空间: O(1) 额外
//
// 【思路】
// 解法1 的痛点：即使 index 在链表尾部，也要从头遍历 n 步。
// 双向链表可以从两端遍历：index < size/2 从头走，否则从尾走。
// 用两个虚拟节点(dummy head + dummy tail)让首尾操作也无需特判。
//
// 双向链表结构:
//   dummyHead <-> node0 <-> node1 <-> node2 <-> dummyTail
//     ↑                                            ↑
//   head_                                        tail_
//
// getNode(index) 策略:
//   if index < size/2:  从 head_->next 往后走 index 步
//   else:               从 tail_->prev 往前走 (size-1-index) 步
//
// 插入 newNode 在 prev 和 succ 之间 (4条指针赋值):
//   
//   Before:  ... <-> prev <-> succ <-> ...
//   
//   Step1:   newNode->prev = prev
//   Step2:   newNode->next = succ
//   Step3:   prev->next = newNode     // 先设好 newNode 的指针
//   Step4:   succ->prev = newNode     // 再修改邻居的指针
//
//   After:   ... <-> prev <-> newNode <-> succ <-> ...
//
// 删除 node (已知 node):
//
//   Before:  ... <-> prev <-> node <-> succ <-> ...
//
//   Step1:   prev->next = succ
//   Step2:   succ->prev = prev
//   Step3:   delete node
//
//   After:   ... <-> prev <-> succ <-> ...
// ============================================================

struct DListNode {
    int val;
    DListNode* prev;
    DListNode* next;
    DListNode(int v) : val(v), prev(nullptr), next(nullptr) {}
};

class MyLinkedList2 {
public:
    MyLinkedList2() {
        head_ = new DListNode(0);   // 虚拟头
        tail_ = new DListNode(0);   // 虚拟尾
        head_->next = tail_;
        tail_->prev = head_;
        size_ = 0;
    }
    
    ~MyLinkedList2() {
        DListNode* cur = head_;
        while (cur) {
            DListNode* next = cur->next;
            delete cur;
            cur = next;
        }
    }
    
    int get(int index) {
        if (index < 0 || index >= size_) return -1;
        return getNode(index)->val;
    }
    
    void addAtHead(int val) {
        // 在虚拟头之后插入 = 在链表最前面插入
        addAfter(head_, val);
    }
    
    void addAtTail(int val) {
        // 在虚拟尾的前驱之后插入 = 在链表最后面追加
        addAfter(tail_->prev, val);
    }
    
    void addAtIndex(int index, int val) {
        if (index > size_) return;
        if (index < 0) index = 0;
        
        DListNode* prevNode;
        if (index == size_) {
            // 追加到末尾：前驱是 dummyTail 的前一个
            prevNode = tail_->prev;
        } else {
            // 找到 index 位置节点的前驱
            prevNode = getNode(index)->prev;
        }
        addAfter(prevNode, val);
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
    
    // 智能选择遍历方向：靠近头从头走，靠近尾从尾走
    DListNode* getNode(int index) {
        DListNode* cur;
        if (index < size_ / 2) {
            // 从头往后走
            cur = head_->next;
            for (int i = 0; i < index; i++) {
                cur = cur->next;
            }
        } else {
            // 从尾往前走 — 双向链表的核心优势
            cur = tail_->prev;
            for (int i = size_ - 1; i > index; i--) {
                cur = cur->prev;
            }
        }
        return cur;
    }
    
    // 工具函数：在 prev 之后插入新节点
    void addAfter(DListNode* prev, int val) {
        DListNode* newNode = new DListNode(val);
        DListNode* succ = prev->next;
        
        // 四条指针赋值：先设新节点的，再改邻居的
        newNode->prev = prev;
        newNode->next = succ;
        prev->next = newNode;
        succ->prev = newNode;
        
        size_++;
    }
    
    // 工具函数：删除指定节点
    void removeNode(DListNode* node) {
        DListNode* prev = node->prev;
        DListNode* succ = node->next;
        
        // 让前驱和后继直接相连，跳过 node
        prev->next = succ;
        succ->prev = prev;
        
        delete node;
        size_--;
    }
};

// ============================================================
// 【解法对比】
//
// | 维度         | 单链表 + dummy head  | 双向链表 + dummy head/tail |
// |-------------|---------------------|---------------------------|
// | 代码量       | ~40行，简洁          | ~70行，较长                 |
// | get 最坏     | O(n)               | O(n/2)                    |
// | 每次插入指针数 | 2条                 | 4条                        |
// | 每节点空间    | 1个next             | 2个(prev+next)             |
// | 面试推荐     | ⭐ 首选              | 追问优化时给出              |
//
// 选择策略:
// - 时间紧张/追求正确率 → 单链表
// - 被追问"如何优化尾部操作"或"get如何更快" → 双向链表
//
// ============================================================
// 【易错点】
//
// 1. addAtIndex 中 index == size 应该追加而非拒绝:
//    ✗ if (index >= size_) return;    // 漏掉了 index==size 的合法情况
//    ✓ if (index > size_) return;     // 只有严格大于才拒绝
//
// 2. 插入时指针赋值顺序错误:
//    ✗ prev->next = newNode;
//      newNode->next = prev->next;    // 此时 prev->next 已是 newNode!
//    ✓ newNode->next = prev->next;    // 先接后面
//      prev->next = newNode;          // 再改前驱
//
// 3. 忘记维护 size_:
//    ✗ 插入/删除后没有 size_++ / size_--
//      导致后续 index 有效性判断全部失效
//    ✓ 每次 add 必须 size_++，每次 delete 必须 size_--
//
// 4. 双向链表删除时只改了单向:
//    ✗ prev->next = succ;  // 漏掉了 succ->prev = prev
//    ✓ prev->next = succ;
//      succ->prev = prev;  // 双向都要更新!
//
// 5. get 中混淆"从 dummy 走几步"和"从第0个节点走几步":
//    ✗ cur = dummy_;  for (i=0; i<index; i++) cur=cur->next; return cur->val;
//      // 走了 index 步到达的是第 index-1 个节点（因为从 dummy 出发）
//    ✓ cur = dummy_->next;  // 先跳过 dummy，cur 就是第 0 个节点
//      for (i=0; i<index; i++) cur=cur->next;  return cur->val;
//
// ============================================================
// 【面试追问 — 递进链】
//
// Q1: 为什么要用虚拟头节点？
//   → 不用的话，addAtHead 和 deleteAtIndex(0) 要单独处理头指针更新。
//     虚拟头让所有节点有前驱，逻辑统一，不易出 bug。
//
// Q2: 如果频繁 addAtTail，单链表怎么优化？
//   → 维护 tail_ 指针，addAtTail 变 O(1)。
//     但 deleteAtTail 仍 O(n)（单链表找不到尾节点的前驱）。
//
// Q3: 如何让 addAtTail 和 deleteAtTail 都 O(1)？
//   → 双向链表 + 虚拟头尾。deleteAtTail 直接操作 dummyTail->prev。
//
// Q4: 这个设计和 LRU Cache 有什么关系？
//   → LRU = 双向链表（维护访问顺序）+ 哈希表（O(1) 定位节点）。
//     本题的 addAfter/removeNode 就是 LRU 的核心操作。
//
// ============================================================
// 【相关题型】
//
// - 206. 反转链表 — 复用指针操作基础（三指针法）
// - 21. 合并有序链表 — 复用 dummy head 技巧
// - 146. LRU 缓存 — 直接复用本题解法2的 addAfter/removeNode
// - 460. LFU 缓存 — 多条双向链表，本题是基础
// ============================================================
