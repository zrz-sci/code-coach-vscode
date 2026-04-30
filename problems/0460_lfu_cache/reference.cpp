/*
 * LeetCode 460: LFU Cache
 * 
 * 【题目本质】
 * 设计一个缓存数据结构，在 O(1) 时间内完成:
 * (1) 通过 key 查找 value
 * (2) 更新 key 的访问频率
 * (3) 淘汰"频率最低 + 最久未使用"的 key
 * 
 * 核心挑战: 同时维护"频率"和"时间顺序"两个维度。
 *
 * 【解法总览】
 * 解法1: 暴力 HashMap + 线性扫描          — get O(1) / put O(n)   — 理解题意
 * 解法2: 双 HashMap + 双向链表(分桶)       — get O(1) / put O(1)   — 面试首选 ⭐
 * 解法3: 双 HashMap + splice 优化          — get O(1) / put O(1)   — 代码更简洁
 *
 * 【整体架构图】
 *
 *   keyMap (HashMap):
 *   ┌─────────┬──────────────────────────┐
 *   │  key=1  │ val=10, freq=3, iter─────│──→ freqMap[3] 中的位置
 *   │  key=2  │ val=20, freq=1, iter─────│──→ freqMap[1] 中的位置
 *   │  key=3  │ val=30, freq=2, iter─────│──→ freqMap[2] 中的位置
 *   └─────────┴──────────────────────────┘
 *
 *   freqMap (HashMap of doubly-linked lists):
 *   freq=1:  [2] ←→ [5] ←→ [7]    头=最久未用(淘汰候选)  尾=最近使用
 *                                   ↑
 *   freq=2:  [3] ←→ [4]           minFreq=1 时, 淘汰 key=2
 *   freq=3:  [1] ←→ [6]
 *
 *   minFreq = 1  ← 直接定位到要淘汰的那个桶
 *
 * 【操作示例】(对应题目示例)
 *
 *   LFUCache(2)
 *   put(1,1): freqMap[1]=[1]           minFreq=1
 *   put(2,2): freqMap[1]=[1,2]         minFreq=1
 *   get(1):   freq of 1: 1→2
 *             freqMap[1]=[2]  freqMap[2]=[1]  minFreq=1(freq=1链表未空)
 *   put(3,3): 满了! 淘汰 freqMap[minFreq=1].front() = key 2
 *             删除2后: freqMap[1]=[3]  freqMap[2]=[1]  minFreq=1
 *   get(2):   不存在 → 返回 -1
 *   get(3):   freq of 3: 1→2
 *             freqMap[1]=[]   freqMap[2]=[1,3]  minFreq=2(freq=1空了且==minFreq)
 *   put(4,4): 满了! 淘汰 freqMap[minFreq=2].front() = key 1
 *             删除1后: freqMap[1]=[4]  freqMap[2]=[3]  minFreq=1
 *   get(1):   不存在 → 返回 -1
 *   get(3):   返回 3, freq 2→3
 *   get(4):   返回 4, freq 1→2
 */

// ============================================================
// 解法1: 暴力 — HashMap + 线性扫描淘汰
// 时间: get O(1), put O(n)  空间: O(n)
//
// 【思路】
// 用 HashMap 存 key → (value, freq, lastUsedTime)。
// get 是 O(1)（直接查 HashMap）。
// put 满了需要淘汰时，遍历所有 entry 找 freq 最小的，
// freq 相同则找 lastUsedTime 最小的 → O(n)。
//
// 这不满足题目 O(1) 要求，但帮助我们理解：
// 瓶颈在于"快速找到 freq 最小 + 最久未使用的 key"。
// ============================================================
class Solution1_LFUCache {
    struct Entry {
        int val, freq, time;
    };
    int cap, globalTime;
    unordered_map<int, Entry> cache;

public:
    Solution1_LFUCache(int capacity) : cap(capacity), globalTime(0) {}

    int get(int key) {
        if (cache.find(key) == cache.end()) return -1;
        cache[key].freq++;
        cache[key].time = globalTime++;
        return cache[key].val;
    }

    void put(int key, int value) {
        if (cap <= 0) return;

        if (cache.find(key) != cache.end()) {
            cache[key].val = value;
            cache[key].freq++;
            cache[key].time = globalTime++;
            return;
        }

        // 满了 → 线性扫描找淘汰目标 O(n)
        if ((int)cache.size() >= cap) {
            int evictKey = -1, minF = INT_MAX, minT = INT_MAX;
            for (auto& [k, e] : cache) {
                // 频率更小，或频率相同但时间更早（最久未使用）
                if (e.freq < minF || (e.freq == minF && e.time < minT)) {
                    minF = e.freq;
                    minT = e.time;
                    evictKey = k;
                }
            }
            cache.erase(evictKey);
        }

        cache[key] = {value, 1, globalTime++};
    }
};


// ============================================================
// 解法2: 双 HashMap + 双向链表 — O(1) 所有操作 ⭐ 面试首选
// 时间: get O(1), put O(1)  空间: O(n)
//
// 【思路】
// 解法1的瓶颈: 淘汰时需要 O(n) 扫描找最小频率。
// 
// 突破方法: "按频率分桶"
// - freqMap[f] 是一个双向链表，存储所有频率为 f 的 key
// - 链表内按使用时间排列: 头部=最久未用, 尾部=最近使用
// - 维护 minFreq 变量，O(1) 定位到淘汰桶
//
// 当 key 被访问（get/put已有key）:
//   freq 从 f 变为 f+1
//   → 从 freqMap[f] 中删除，加入 freqMap[f+1] 尾部
//   → 如果 freqMap[f] 空了 且 f == minFreq → minFreq++
//
// 淘汰:
//   → 直接删 freqMap[minFreq] 的头部节点
//
// 【为什么 minFreq 只会 +1 不会跳跃？】
// 当 freqMap[minFreq] 变空时，说明其中唯一的 key 被提升了频率。
// 但我们一次只提升一个 key，从 minFreq 到 minFreq+1。
// 所以 freqMap[minFreq+1] 一定非空，新的最小频率就是 minFreq+1。
// ============================================================
class LFUCache {
    // key 对应的信息
    struct Node {
        int val, freq;
        list<int>::iterator iter;  // 在 freqMap 链表中的位置
    };

    int cap, minFreq;
    unordered_map<int, Node> keyMap;           // key → Node
    unordered_map<int, list<int>> freqMap;     // freq → doubly-linked list of keys

    // 核心辅助函数: 将 key 的频率从 f 提升到 f+1
    void increaseFreq(int key) {
        auto& node = keyMap[key];
        int oldFreq = node.freq;
        node.freq++;

        // 第1步: 从旧频率链表中移除
        freqMap[oldFreq].erase(node.iter);

        // 第2步: 检查旧频率链表是否变空
        if (freqMap[oldFreq].empty()) {
            freqMap.erase(oldFreq);
            // 关键! 如果旧频率就是 minFreq，则 minFreq 需要 +1
            // 因为该频率下已经没有任何 key 了
            if (minFreq == oldFreq) {
                minFreq++;
            }
        }

        // 第3步: 加入新频率链表尾部（尾部 = 最近使用）
        freqMap[node.freq].push_back(key);
        // 更新迭代器: 指向新链表中的最后一个元素
        node.iter = prev(freqMap[node.freq].end());
    }

public:
    LFUCache(int capacity) : cap(capacity), minFreq(0) {}

    int get(int key) {
        auto it = keyMap.find(key);
        if (it == keyMap.end()) return -1;

        // 访问了一次 → 频率+1
        increaseFreq(key);
        return it->second.val;
    }

    void put(int key, int value) {
        // 边界: 容量为0时任何 put 都不生效
        if (cap <= 0) return;

        // Case 1: key 已存在 → 更新值 + 增加频率
        if (keyMap.find(key) != keyMap.end()) {
            keyMap[key].val = value;
            increaseFreq(key);  // put 已有 key 也算一次访问
            return;
        }

        // Case 2: key 不存在，需要插入新 key
        // 如果已满，先淘汰
        if ((int)keyMap.size() >= cap) {
            // 淘汰目标: freqMap[minFreq] 链表的头部（最久未使用）
            int evictKey = freqMap[minFreq].front();
            freqMap[minFreq].pop_front();
            if (freqMap[minFreq].empty()) {
                freqMap.erase(minFreq);
            }
            keyMap.erase(evictKey);
        }

        // 插入新 key，频率为 1
        // 新 key 的频率一定 ≤ 所有已有 key 的频率，所以 minFreq = 1
        minFreq = 1;
        freqMap[1].push_back(key);
        keyMap[key] = {value, 1, prev(freqMap[1].end())};
    }
};


// ============================================================
// 解法3: 双 HashMap + splice 优化 — 代码更简洁
// 时间: get O(1), put O(1)  空间: O(n)
//
// 【思路】
// 和解法2完全相同的架构，但利用 C++ list::splice() 的特性:
// splice 可以在 O(1) 时间内将一个节点从一个 list 移到另一个 list，
// 而且移动后迭代器仍然有效！
// 
// 这意味着:
// 1. 不需要手动 erase + push_back
// 2. 不需要更新 keyMap 中存储的迭代器
// 代码更简洁，不容易出 bug。
// ============================================================
class Solution3_LFUCache {
    int cap, minFreq;

    // 把 key, value, freq 都存在链表节点中
    struct KVF {
        int key, val, freq;
    };

    unordered_map<int, list<KVF>> freqMap;                  // freq → list<KVF>
    unordered_map<int, list<KVF>::iterator> keyMap;          // key → iterator

    // 将 key 对应的节点从旧频率链表 splice 到新频率链表
    void touch(list<KVF>::iterator it) {
        int oldFreq = it->freq;
        it->freq++;
        int newFreq = it->freq;

        // splice: 把 it 从 freqMap[oldFreq] 移到 freqMap[newFreq] 的尾部
        // 注意: splice 后 it 仍然有效，指向同一个节点
        freqMap[newFreq].splice(freqMap[newFreq].end(), freqMap[oldFreq], it);

        // 清理旧频率
        if (freqMap[oldFreq].empty()) {
            freqMap.erase(oldFreq);
            if (minFreq == oldFreq) {
                minFreq++;
            }
        }
    }

public:
    Solution3_LFUCache(int capacity) : cap(capacity), minFreq(0) {}

    int get(int key) {
        auto it = keyMap.find(key);
        if (it == keyMap.end()) return -1;

        auto listIter = it->second;
        touch(listIter);
        // 迭代器在 splice 后不失效，keyMap 不需要更新
        return listIter->val;
    }

    void put(int key, int value) {
        if (cap <= 0) return;

        // key 已存在
        auto it = keyMap.find(key);
        if (it != keyMap.end()) {
            it->second->val = value;
            touch(it->second);
            return;
        }

        // key 不存在，需要插入
        if ((int)keyMap.size() >= cap) {
            // 淘汰 minFreq 链表的头部
            auto& minList = freqMap[minFreq];
            keyMap.erase(minList.front().key);
            minList.pop_front();
            if (minList.empty()) {
                freqMap.erase(minFreq);
            }
        }

        // 插入新 key
        minFreq = 1;
        freqMap[1].push_back({key, value, 1});
        keyMap[key] = prev(freqMap[1].end());
    }
};


/*
 * ============================================================
 * 【解法对比】
 * 
 *            解法1(暴力)        解法2(分桶)           解法3(splice)
 * get:       O(1)              O(1)                 O(1)
 * put:       O(n) 淘汰时       O(1)                 O(1)
 * 代码量:     少                中等                  稍少
 * 面试:       说思路            ⭐ 首选写这个          提到即可
 * 
 * 解法2 vs 解法3:
 * - 架构完全一致，区别仅在于 splice vs 手动 erase+push_back
 * - splice 的好处: 迭代器不失效，不用更新 keyMap
 * - splice 的坏处: 不是所有人都熟悉这个 API，面试时可能需要解释
 * 
 * 与 LRU Cache (146题) 的对比:
 * - LRU: 一条链表 + 一个 HashMap → 只维护时间顺序
 * - LFU: 多条链表(按频率分桶) + 两个 HashMap + minFreq → 维护频率+时间
 * - LFU 的 increaseFreq 操作 = LRU 的 moveToTail，但多了频率桶间的迁移
 * 
 * ============================================================
 * 【易错点】
 * 
 * 1. minFreq 更新遗漏:
 *    ✗ increaseFreq 中忘记检查 freqMap[oldFreq].empty()
 *    ✓ 必须在旧频率链表变空且 oldFreq == minFreq 时执行 minFreq++
 *    原因: 否则淘汰时访问的是一个空链表
 * 
 * 2. put 已有 key 忘记增加频率:
 *    ✗ if (keyMap.count(key)) { keyMap[key].val = value; return; }
 *    ✓ if (keyMap.count(key)) { keyMap[key].val = value; increaseFreq(key); return; }
 *    原因: 题目明确说 put 操作也增加使用计数
 * 
 * 3. 新插入 key 后 minFreq 不重置为 1:
 *    ✗ 插入后 minFreq 还是旧值(比如 3)
 *    ✓ minFreq = 1; // 新 key 频率为1，一定是当前最小
 *    原因: 新 key 的频率为1，一定 ≤ 所有已有 key 的频率
 * 
 * 4. 链表方向搞反:
 *    ✗ 淘汰时 pop_back()（删除了最近使用的）
 *    ✓ 淘汰时 pop_front()（删除最久未使用的）
 *    约定: 新访问的 push_back 到尾部，淘汰从 front 取
 * 
 * 5. 迭代器存错:
 *    ✗ node.iter = freqMap[freq].end();
 *    ✓ node.iter = prev(freqMap[freq].end());
 *    原因: end() 是 past-the-end，不指向任何有效元素
 * 
 * 6. capacity 为 0 不特判:
 *    ✗ 直接比较 keyMap.size() >= cap，当 cap=0 时 size_t 比较出问题
 *    ✓ 函数开头 if (cap <= 0) return;
 * 
 * ============================================================
 * 【面试追问】
 * 
 * Q1: LRU 和 LFU 各自的优缺点？什么场景用哪个？
 *     LRU: 实现简单，适合访问有时间局部性的场景。
 *          缺点: 偶发的全表扫描会把热点数据冲掉。
 *     LFU: 保护高频数据，适合有稳定热点的场景。
 *          缺点: 历史高频但已过时的数据("缓存污染")很难淘汰。
 * 
 * Q2: 为什么 minFreq 只会 +1 不会跳跃？
 *     当 freqMap[minFreq] 变空时，是因为我们刚把其中的一个 key 
 *     从 minFreq 提升到了 minFreq+1。所以 freqMap[minFreq+1] 
 *     一定有至少一个元素，新的最小频率就是 minFreq+1。
 * 
 * Q3: 如果要支持 O(1) 的 delete(key) 操作，需要改什么？
 *     只需在 keyMap 中找到 key 的信息（包括频率和链表迭代器），
 *     然后从对应频率链表中删除即可。如果删除后该频率链表变空
 *     且等于 minFreq，需要遍历找新的 minFreq（这一步不是 O(1)），
 *     除非额外维护一个频率的有序集合。
 * 
 * Q4: 工业界如何解决 LFU 的"缓存污染"问题？
 *     - 频率衰减: 定期将所有 key 的频率减半
 *     - 窗口 LFU: 只统计最近时间窗口内的频率
 *     - LRU-K: 根据最近 K 次访问的时间来决定淘汰
 *     - ARC (Adaptive Replacement Cache): 自适应地在 LRU 和 LFU 之间平衡
 * ============================================================
 */
