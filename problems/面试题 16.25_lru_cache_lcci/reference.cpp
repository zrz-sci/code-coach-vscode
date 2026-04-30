// ============================================================
// 面试题 16.25. LRU 缓存 (LRU Cache LCCI, 同主站146)
// 难度: Medium | 标签: Design, Hash Table, Linked List, Doubly-Linked List
// ============================================================

// ------------------------------------------------------------
// 解法1: 手写双向链表 + 哈希表 (面试标准答案)
// get O(1), put O(1), 空间 O(capacity)
//
// 思考过程:
//   1. O(1) 查找 -> 哈希表 (key -> 链表节点指针)
//   2. O(1) 插入/删除/移动 -> 双向链表
//   3. 哨兵节点(dummy head/tail)消除边界判断
//   4. 操作分解: addToHead / removeNode / moveToHead / removeTail
//
// 数据结构图:
//   dummy_head <-> [最近使用] <-> ... <-> [最久未使用] <-> dummy_tail
//
// 操作示例 (capacity=2):
//   put(1,1): head <-> [1:1] <-> tail
//   put(2,2): head <-> [2:2] <-> [1:1] <-> tail
//   get(1):   head <-> [1:1] <-> [2:2] <-> tail  (1移到头)
//   put(3,3): head <-> [3:3] <-> [1:1] <-> tail  (淘汰2)
// ------------------------------------------------------------
struct DLinkedNode {
    int key, value;          // 必须存 key! 淘汰时需要从 map 中删除
    DLinkedNode* prev;
    DLinkedNode* next;
    DLinkedNode() : key(0), value(0), prev(nullptr), next(nullptr) {}
    DLinkedNode(int k, int v) : key(k), value(v), prev(nullptr), next(nullptr) {}
};

class Solution1 {
    unordered_map<int, DLinkedNode*> cache;
    DLinkedNode* head;       // 哨兵头节点
    DLinkedNode* tail;       // 哨兵尾节点
    int capacity;
    int size;

    // 基本操作1: 在头部后面插入节点
    // 注意指针赋值顺序: 先设新节点指针, 再改周围节点
    void addToHead(DLinkedNode* node) {
        node->prev = head;
        node->next = head->next;
        head->next->prev = node;
        head->next = node;
    }

    // 基本操作2: 从链表中摘除节点(不释放内存)
    void removeNode(DLinkedNode* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    // 组合操作: 移到头部 = 摘除 + 插入头部
    void moveToHead(DLinkedNode* node) {
        removeNode(node);
        addToHead(node);
    }

    // 组合操作: 删除尾部节点(LRU淘汰), 返回被删节点以便处理
    DLinkedNode* removeTail() {
        DLinkedNode* node = tail->prev;  // 尾部哨兵的前一个
        removeNode(node);
        return node;  // 返回给调用方, 用于 cache.erase(key) 和 delete
    }

public:
    Solution1(int capacity) : capacity(capacity), size(0) {
        // 初始化哨兵: head <-> tail
        head = new DLinkedNode();
        tail = new DLinkedNode();
        head->next = tail;
        tail->prev = head;
    }

    int get(int key) {
        if (!cache.count(key)) return -1;
        DLinkedNode* node = cache[key];
        moveToHead(node);       // 标记为最近使用
        return node->value;
    }

    void put(int key, int value) {
        if (cache.count(key)) {
            // key 已存在: 更新值 + 移到头部
            DLinkedNode* node = cache[key];
            node->value = value;    // 别忘了更新值!
            moveToHead(node);
        } else {
            // key 不存在: 新建节点, 插入头部
            DLinkedNode* node = new DLinkedNode(key, value);
            cache[key] = node;
            addToHead(node);
            size++;
            // 超容量: 淘汰尾部(最久未使用)
            if (size > capacity) {
                DLinkedNode* removed = removeTail();
                cache.erase(removed->key);  // 用节点存的key删除map条目
                delete removed;              // 释放内存
                size--;
            }
        }
    }
};

// ------------------------------------------------------------
// 解法2: STL list + 哈希表 (代码简洁版)
// get O(1), put O(1), 空间 O(capacity)
//
// 思考过程:
//   1. C++ std::list 就是双向链表, 无需手写
//   2. 哈希表: key -> list<pair>::iterator
//   3. list::splice: O(1)移动节点, 迭代器不失效
//   4. 自动内存管理, 无需手动 delete
// ------------------------------------------------------------
class Solution2 {
    int cap;
    list<pair<int,int>> lru;  // front=最近使用, back=最久未使用
    unordered_map<int, list<pair<int,int>>::iterator> cache;

public:
    Solution2(int capacity) : cap(capacity) {}

    int get(int key) {
        auto it = cache.find(key);
        if (it == cache.end()) return -1;
        // splice: 将节点从当前位置移到 begin(), O(1), 迭代器不失效
        lru.splice(lru.begin(), lru, it->second);
        return it->second->second;  // pair.second = value
    }

    void put(int key, int value) {
        auto it = cache.find(key);
        if (it != cache.end()) {
            it->second->second = value;    // 更新 value
            lru.splice(lru.begin(), lru, it->second);
        } else {
            // 超容量先淘汰尾部
            if ((int)cache.size() >= cap) {
                int oldKey = lru.back().first;
                lru.pop_back();
                cache.erase(oldKey);
            }
            lru.push_front({key, value});
            cache[key] = lru.begin();
        }
    }
};

// ------------------------------------------------------------
// 解法3: STL list 精简版 (最少代码)
// 思考过程: 与Solution2相同, 更紧凑的写法
// ------------------------------------------------------------
class Solution3 {
    int cap;
    list<pair<int,int>> dll;
    unordered_map<int, list<pair<int,int>>::iterator> mp;

public:
    Solution3(int capacity) : cap(capacity) {}

    int get(int key) {
        if (!mp.count(key)) return -1;
        dll.splice(dll.begin(), dll, mp[key]);
        return mp[key]->second;
    }

    void put(int key, int value) {
        if (mp.count(key)) {
            mp[key]->second = value;
            dll.splice(dll.begin(), dll, mp[key]);
            return;
        }
        if ((int)mp.size() >= cap) {
            mp.erase(dll.back().first);
            dll.pop_back();
        }
        dll.push_front({key, value});
        mp[key] = dll.begin();
    }
};

/*
 * ============================================================
 * 解法对比:
 * ============================================================
 * | 解法         | 时间  | 空间        | 代码量 | 面试推荐 |
 * |-------------|-------|------------|-------|---------|
 * | 手写双向链表  | O(1)  | O(capacity)| ~60行 | 首选    |
 * | STL list    | O(1)  | O(capacity)| ~30行 | 追问展示 |
 * | 精简版       | O(1)  | O(capacity)| ~25行 | 快速实现 |
 *
 * 面试策略: 先手写链表展示底层功底, 再提及 STL list+splice
 *
 * ============================================================
 * 易错点:
 * ============================================================
 * 1. 节点没存 key -> 淘汰时无法从 map 中删除 key
 *    struct Node { int value; ... }; // 缺 key!
 *
 * 2. put 已存在的 key 忘记更新 value
 *    只做了 moveToHead, 没更新 node->value = value
 *
 * 3. 删除尾部节点后没从 map 中删除
 *    cache.erase(removed->key) 缺失 -> 悬挂指针
 *
 * 4. 哨兵初始化时未互连
 *    缺 head->next=tail; tail->prev=head -> 第一次操作崩溃
 *
 * 5. addToHead 指针赋值顺序错误
 *    先改 head->next 会导致原 head->next 丢失
 *    正确: 先设新节点指针, 再改周围节点
 *
 * ============================================================
 * 面试追问:
 * ============================================================
 * Q1: 为什么双向链表不用单向?
 *     -> 删除已知节点需要前驱指针, 单链表要O(n)遍历
 *
 * Q2: 线程安全?
 *     -> 互斥锁(简单) / 分段锁(高性能) / 读写锁(读多写少)
 *
 * Q3: LRU vs LFU?
 *     -> LFU按频率淘汰, 需要频率计数+每个频率一个链表+min_freq
 *
 * Q4: Redis的LRU?
 *     -> 近似LRU: 随机采样N个key, 淘汰其中最久未使用的
 */
