/*
 * LeetCode 432: All O'one Data Structure
 *
 * 【题目本质】
 * 设计一个数据结构，维护字符串计数，所有操作（inc, dec, getMax, getMin）均 O(1)。
 * 核心挑战：同时需要 O(1) 修改计数和 O(1) 获取极值。
 *
 * 【解法总览】
 * 解法1: HashMap + 遍历          — inc/dec O(1), getMax/getMin O(n) — 不满足要求，仅作起点
 * 解法2: HashMap + 双向链表(分桶) — 全部 O(1)                      — 面试必须写出
 *
 * 【设计思路图】
 *
 * HashMap: key → Node*                  双向链表 (按count升序):
 *   "a" → [Node:1]                     [Sentinel:0] ↔ [1:{"a","c"}] ↔ [2:{"b"}] ↔ [5:{"d"}] ↔ [Sentinel:INF]
 *   "b" → [Node:2]                         head          ↑ min                       ↑ max          tail
 *   "c" → [Node:1]
 *   "d" → [Node:5]                    getMinKey() = head->next 的任意key = "a"
 *                                      getMaxKey() = tail->prev 的任意key = "d"
 *
 * 【为什么 O(1)?】
 * inc/dec 只让计数变化 ±1，目标桶一定是当前桶的相邻节点：
 *   - 如果相邻节点的 count 恰好是 target count → 直接移过去
 *   - 否则 → 在相邻位置新建一个节点
 * 双向链表在已知位置的插入/删除是 O(1)
 * HashMap 查找是 O(1)
 * unordered_set 的 insert/erase 是摊还 O(1)
 */

// ============================================================
// 解法1: HashMap + 遍历 (不满足 O(1) 要求，仅作为暴力起点)
// 时间: inc/dec O(1), getMax/getMin O(n)
// 空间: O(n)
//
// 【思路】
// 最直觉的做法：HashMap 存 key→count。
// 增减操作直接改 map，但获取极值需要遍历整个 map。
// 瓶颈：getMax/getMin 是 O(n)。
// ============================================================
class AllOne1 {
    unordered_map<string, int> count;
public:
    AllOne1() {}

    void inc(string key) {
        count[key]++;
    }

    void dec(string key) {
        if (--count[key] == 0) {
            count.erase(key);
        }
    }

    string getMaxKey() {
        string res = "";
        int maxVal = 0;
        for (auto& [k, v] : count) {
            if (v > maxVal) {
                maxVal = v;
                res = k;
            }
        }
        return res;
    }

    string getMinKey() {
        string res = "";
        int minVal = INT_MAX;
        for (auto& [k, v] : count) {
            if (v < minVal) {
                minVal = v;
                res = k;
            }
        }
        return res;
    }
};

// ============================================================
// 解法2: HashMap + 双向链表(分桶) — 全部 O(1)  ⭐ 面试必须写出
// 时间: 所有操作 O(1) 摊还
// 空间: O(n)
//
// 【思路】
// 解法1的瓶颈: getMax/getMin 需要遍历找极值。
// 我们需要一个有序结构，头尾就是极值，且增删 O(1)。
//
// 双向链表天然支持 O(1) 头尾访问和已知位置的增删。
// 关键观察: 计数只会 ±1 变化，所以目标桶一定是当前桶的邻居。
//
// 数据结构设计:
// 1. 链表节点(桶): 存一个 count 值 + 该 count 下所有 key 的集合
// 2. 链表按 count 从小到大排序
// 3. HashMap: key → 该 key 所在的桶(节点指针)
// 4. 哨兵: head(count=0) 和 tail(count=INT_MAX) 简化边界处理
//
// inc("hello") 操作示意:
//   假设 "hello" 当前在 count=2 的桶:
//
//   Before: [S:0] ↔ [1:{"a"}] ↔ [2:{"hello","b"}] ↔ [S:INF]
//                                   ↑ cur
//
//   Step1: 目标 count = 3, 检查 cur->next->count == 3?
//          cur->next = [S:INF], count=INF ≠ 3
//          → 在 cur 和 cur->next 之间新建 [3:{}]
//
//   Step2: 把 "hello" 加入 [3], 从 [2] 移除
//
//   After:  [S:0] ↔ [1:{"a"}] ↔ [2:{"b"}] ↔ [3:{"hello"}] ↔ [S:INF]
//                                                ↑ keyNode["hello"] 更新
//
// dec("b") 操作示意 (count 2→1):
//   Before: [S:0] ↔ [1:{"a"}] ↔ [2:{"b"}] ↔ [3:{"hello"}] ↔ [S:INF]
//                                  ↑ cur
//   目标 count=1, 检查 cur->prev->count == 1? 是!
//   把 "b" 加入 [1], 从 [2] 移除, [2] 为空 → 删除 [2]
//
//   After:  [S:0] ↔ [1:{"a","b"}] ↔ [3:{"hello"}] ↔ [S:INF]
//
// ============================================================
class AllOne {
    // 链表节点: 一个"桶"，存储同一计数下的所有 key
    struct Node {
        int count;
        unordered_set<string> keys;
        Node* prev;
        Node* next;
        Node(int c) : count(c), prev(nullptr), next(nullptr) {}
    };

    Node* head; // 哨兵头, count = 0
    Node* tail; // 哨兵尾, count = INT_MAX
    unordered_map<string, Node*> keyNode; // key → 所在桶

    // 在 prevNode 之后插入一个 count 值为 c 的新桶
    // 为什么参数是 prevNode: 我们总是知道"应该插在谁后面"
    // (inc 时插在 cur 后面, dec 时插在 cur->prev 后面)
    Node* addNodeAfter(Node* prevNode, int c) {
        Node* newNode = new Node(c);
        newNode->prev = prevNode;
        newNode->next = prevNode->next;
        prevNode->next->prev = newNode;
        prevNode->next = newNode;
        return newNode;
    }

    // 从链表中移除空桶
    // 前提: node 不是哨兵，且 node->keys 已为空
    void removeNode(Node* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
        delete node;
    }

public:
    AllOne() {
        head = new Node(0);
        tail = new Node(INT_MAX);
        head->next = tail;
        tail->prev = head;
    }

    void inc(string key) {
        if (keyNode.find(key) == keyNode.end()) {
            // === key 不存在，新插入，计数为 1 ===
            // 目标桶: count=1，应该紧跟在 head 后面
            if (head->next->count != 1) {
                // count=1 的桶不存在，新建
                addNodeAfter(head, 1);
            }
            head->next->keys.insert(key);
            keyNode[key] = head->next;
        } else {
            // === key 已存在，计数 c → c+1 ===
            Node* cur = keyNode[key];
            int newCount = cur->count + 1;
            // 目标桶在 cur 的右边（计数更大的方向）
            // 检查 cur->next 的 count 是否恰好是 newCount
            if (cur->next->count != newCount) {
                // 不存在，在 cur 后面新建
                addNodeAfter(cur, newCount);
            }
            // 把 key 移到目标桶
            cur->next->keys.insert(key);
            keyNode[key] = cur->next;
            // 从旧桶移除
            cur->keys.erase(key);
            if (cur->keys.empty()) {
                removeNode(cur); // 旧桶空了，删除
            }
        }
    }

    void dec(string key) {
        // 题目保证 key 存在
        Node* cur = keyNode[key];
        int newCount = cur->count - 1;

        if (newCount == 0) {
            // 计数降为 0，彻底从数据结构中删除
            keyNode.erase(key);
        } else {
            // 计数 c → c-1，目标桶在 cur 的左边（计数更小的方向）
            if (cur->prev->count != newCount) {
                // 不存在，在 cur->prev 后面新建（即 cur 前面）
                addNodeAfter(cur->prev, newCount);
            }
            cur->prev->keys.insert(key);
            keyNode[key] = cur->prev;
        }

        // 从旧桶移除（无论计数是否降为0，旧桶都要移除这个 key）
        cur->keys.erase(key);
        if (cur->keys.empty()) {
            removeNode(cur);
        }
    }

    string getMaxKey() {
        // 最大计数在链表尾部（tail->prev），哨兵 tail 的 count=INF
        if (tail->prev == head) return ""; // 链表空（只有哨兵）
        return *(tail->prev->keys.begin()); // 返回任意一个 key
    }

    string getMinKey() {
        // 最小计数在链表头部（head->next），哨兵 head 的 count=0
        if (head->next == tail) return ""; // 链表空
        return *(head->next->keys.begin());
    }

    // 析构函数：释放所有节点内存
    ~AllOne() {
        Node* cur = head;
        while (cur) {
            Node* next = cur->next;
            delete cur;
            cur = next;
        }
    }
};

// ============================================================
// 解法3: 使用 std::list 简化代码 (本质和解法2完全相同)
// 时间: 所有操作 O(1) 摊还
// 空间: O(n)
//
// 【思路】
// 和解法2相同的设计，但用 STL 的 std::list 代替手写双向链表。
// 优点：代码更短，不需要手动管理 prev/next 指针和内存。
// 注意：需要用 list::iterator 代替 Node*，理解 iterator 语义。
// ============================================================
class AllOne3 {
    struct Bucket {
        int count;
        unordered_set<string> keys;
    };

    list<Bucket> buckets; // 双向链表，按 count 升序
    unordered_map<string, list<Bucket>::iterator> keyBucket; // key → 所在桶的迭代器

public:
    AllOne3() {}

    void inc(string key) {
        if (keyBucket.find(key) == keyBucket.end()) {
            // key 不存在，插入 count=1
            // count=1 的桶应该在链表最前面
            if (buckets.empty() || buckets.front().count != 1) {
                buckets.push_front({1, {}});
            }
            buckets.front().keys.insert(key);
            keyBucket[key] = buckets.begin();
        } else {
            // key 已存在
            auto cur = keyBucket[key];
            int newCount = cur->count + 1;
            auto next = std::next(cur);
            // 检查下一个桶的 count 是否是 newCount
            if (next == buckets.end() || next->count != newCount) {
                // 在 cur 后面插入新桶
                next = buckets.insert(next, {newCount, {}});
            }
            next->keys.insert(key);
            keyBucket[key] = next;
            // 从旧桶移除
            cur->keys.erase(key);
            if (cur->keys.empty()) buckets.erase(cur);
        }
    }

    void dec(string key) {
        auto cur = keyBucket[key];
        int newCount = cur->count - 1;

        if (newCount == 0) {
            keyBucket.erase(key);
        } else {
            // 目标在 cur 前面
            auto prev = (cur == buckets.begin()) ? cur : std::prev(cur);
            if (cur == buckets.begin() || prev->count != newCount) {
                // 在 cur 前面插入新桶
                prev = buckets.insert(cur, {newCount, {}});
            }
            prev->keys.insert(key);
            keyBucket[key] = prev;
        }

        cur->keys.erase(key);
        if (cur->keys.empty()) buckets.erase(cur);
    }

    string getMaxKey() {
        if (buckets.empty()) return "";
        return *(buckets.back().keys.begin());
    }

    string getMinKey() {
        if (buckets.empty()) return "";
        return *(buckets.front().keys.begin());
    }
};

// ============================================================
// 【解法对比】
//
// | 维度         | 解法1(HashMap+遍历) | 解法2(手写双向链表) | 解法3(std::list) |
// |-------------|-------------------|--------------------|-----------------|
// | inc/dec     | O(1)              | O(1)               | O(1)            |
// | getMax/Min  | O(n)              | O(1)               | O(1)            |
// | 代码复杂度   | 极低              | 高（手写链表）       | 中等            |
// | 内存管理     | 自动              | 需手动 delete       | 自动            |
// | 面试推荐     | 起点              | 展示底层理解         | ⭐ 实战首选     |
//
// 解法2 和 解法3 本质完全相同，区别仅在于是否手写链表。
// 面试中建议先说清设计思路（解法2的核心逻辑），
// 写代码时可以用 std::list 加速（解法3），但要能解释底层原理。
//
// ============================================================
// 【易错点】
//
// 1. inc 新 key 时忘记特判:
//    ✗ 直接 `keyNode[key]->count + 1` → key 不在 map 中，空指针
//    ✓ 先 `if (keyNode.find(key) == keyNode.end())` 走新建逻辑
//
// 2. inc/dec 方向搞反:
//    ✗ inc 时检查 cur->prev 是否有 count+1 的桶
//    ✓ inc 检查 cur->next（计数增大往右），dec 检查 cur->prev（计数减小往左）
//
// 3. 忘记在移除 key 后检查桶是否为空:
//    ✗ 只做了 cur->keys.erase(key)，没检查 keys.empty()
//    ✓ 必须 `if (cur->keys.empty()) removeNode(cur);`
//    后果：空桶残留在链表中，getMin 会返回一个不存在的 count
//
// 4. dec 到 0 时仍然尝试找目标桶:
//    ✗ newCount=0 时还在链表中找 count=0 的桶
//    ✓ newCount==0 时直接 erase(key)，不找新桶
//
// 5. 解法3 的 iterator 失效:
//    ✗ erase(cur) 之后还使用 cur
//    ✓ erase 前先保存需要的信息，或把 erase 放在最后
//
// ============================================================
// 【面试追问】
//
// Q1(基础理解): "为什么不能用一个 map<int, set<string>> (红黑树) 来维护?"
//   → map 的插入/删除是 O(log n)，不满足 O(1)。
//     双向链表 + "计数只变化±1"的特性保证了目标位置一定是邻居，O(1)定位。
//
// Q2(优化追问): "unordered_set 的 insert/erase 最坏 O(n)，
//     如何保证严格 O(1)?"
//   → 题目要求"平均 O(1)"即可。unordered_set 摊还 O(1)。
//     如果要最坏 O(1)，可以用完美哈希或 cuckoo hashing，但实践中不必要。
//
// Q3(变体追问): "如果还要支持 getMedianKey()，返回计数中位数的key?"
//   → 维护一个"中间指针"指向链表中间桶。
//     每次 inc/dec 后，根据总 key 数量和中间指针的位置，O(1) 调整。
//     类似 LeetCode 295（数据流中位数）的双指针/双堆思路。
//
// Q4(设计追问): "这个结构线程安全吗？怎么改?"
//   → 不安全。可以加全局互斥锁（简单但并发度低），
//     或对 HashMap 和链表分别加读写锁，
//     或使用无锁数据结构（CAS 操作链表节点）。
// ============================================================
