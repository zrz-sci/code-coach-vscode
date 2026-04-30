/*
 * 379. Design Phone Directory
 *
 * 【题目本质】
 * 号码池管理器：支持 O(1) 分配号码、O(1) 查询可用性、O(1) 回收号码。
 * 考察数据结构设计能力，核心是如何组合 Queue/Set/Array 实现三个操作均 O(1)。
 *
 * 【解法总览】
 * Solution1: Queue + HashSet — 队列取号+哈希判重              所有操作 O(1)
 * Solution2: 纯 HashSet — 只用一个 set 维护可用号码           所有操作 O(1) 均摊
 *
 * 【设计要点】
 * - get(): 取出一个可用号码 → 需要快速获取任意可用元素
 * - check(): 判断号码是否可用 → 需要 O(1) 查询
 * - release(): 将号码归还池 → 需要防止重复归还
 */

// ===================== Solution1: Queue + HashSet（推荐）=====================
// queue 维护可用号码池（保证 get O(1)）
// unordered_set 记录已使用号码（保证 check O(1)）
// 所有操作 O(1)
class Solution1 {
    // PhoneDirectory 实现
    queue<int> available;          // 可用号码队列
    unordered_set<int> used;       // 已使用号码集合
    int maxNum;

public:
    void init(int maxNumbers) {
        maxNum = maxNumbers;
        for (int i = 0; i < maxNumbers; i++) {
            available.push(i);
        }
    }

    int get() {
        if (available.empty()) return -1;
        int num = available.front();
        available.pop();
        used.insert(num);
        return num;
    }

    bool check(int number) {
        // 可用 = 未被使用
        return used.find(number) == used.end();
    }

    void release(int number) {
        if (used.count(number)) {
            used.erase(number);
            available.push(number);
        }
        // 已经可用的号码不做任何操作（防止重复入队）
    }
};

// ===================== Solution2: 纯 HashSet =====================
// 只维护一个可用号码 set
// get: 取 begin() 并删除；check: count()；release: insert()
// 所有操作 O(1) 均摊
class Solution2 {
    unordered_set<int> available;  // 可用号码集合

public:
    void init(int maxNumbers) {
        for (int i = 0; i < maxNumbers; i++) {
            available.insert(i);
        }
    }

    int get() {
        if (available.empty()) return -1;
        int num = *available.begin();
        available.erase(available.begin());
        return num;
    }

    bool check(int number) {
        return available.count(number) > 0;
    }

    void release(int number) {
        available.insert(number);  // unordered_set 自动去重
    }
};

/*
 * 【解法对比】
 * | 解法     | get  | check | release | 空间 | 优点               | 缺点             |
 * |----------|------|-------|---------|------|--------------------|------------------|
 * | Solution1| O(1) | O(1)  | O(1)   | O(n) | get 确定性 O(1)     | 两个数据结构同步   |
 * | Solution2| O(1)*| O(1)  | O(1)   | O(n) | 单一数据结构更简洁   | get 均摊O(1)      |
 *
 * 【易错点】
 * 1. release 重复调用：不检查就入队会导致同一号码多次出现在队列中
 * 2. get 空队列：忘记检查 empty() 直接 front() 导致 UB
 * 3. check 正反逻辑：check(num) 返回 true 表示号码可用，不是已使用
 * 4. 初始化遗漏：忘记把 0~maxNumbers-1 全部加入可用池
 * 5. release 越界号码：虽然题目保证范围，但工程上应加边界检查
 *
 * 【面试追问】
 * Q1: maxNumbers 很大（如 10^9）时如何优化？
 * A1: 惰性初始化。不在构造时生成所有号码，维护 nextAvailable 指针，
 *     get 优先从回收池取，没有则用 nextAvailable++。
 *     初始化从 O(n) 降为 O(1)，空间也只分配已使用/回收的量。
 *
 * Q2: 如何实现线程安全版本？
 * A2: 方案一：互斥锁保护 get/check/release；
 *     方案二：使用 concurrent_queue + atomic 变量实现无锁版本；
 *     关键是 get 和 release 的原子性，check 可以允许弱一致。
 *
 * Q3: 如果要求 get 返回最小可用号码怎么办？
 * A3: 将 queue 换成 priority_queue(min-heap) 或 ordered set，
 *     get 变为 O(log n)。或者用 Bitset + __builtin_ctz 找最低位 1，
 *     可做到 O(n/64) ≈ O(1)。
 */
