/*
 * LeetCode 359: 日志速率限制器 (Logger Rate Limiter)
 * 
 * 【题目本质】
 * 设计一个带"冷却时间"的消息过滤器：相同消息在 10 秒内只能打印一次。
 * 核心操作是 O(1) 地查询某消息上次打印时间并决定是否允许打印。
 *
 * 【解法总览】
 * 解法1: 哈希表记录时间戳 — O(1) / O(n)    — 最直觉，面试首选
 * 解法2: 队列+集合(滑动窗口) — O(1)均摊 / O(m) — 空间优化，面试加分
 */

// ============================================================
// 解法1: 哈希表记录时间戳
// 时间: O(1) 每次调用
// 空间: O(n) n = 所有不同消息的数量（永不释放）
//
// 【思路】
// 问自己：对于每条消息，我需要知道什么？→ 它上次什么时候被打印的。
// "根据消息字符串快速查找一个时间值" → 天然是哈希表 key→value。
// 
// 小技巧：存"下次允许打印的最早时间"比存"上次打印时间"更方便。
// 这样判断时直接 timestamp >= nextAllowed 即可，不用做减法。
//
// 时间线示意:
//   t=1:  "foo" → 不在表中 → 打印 ✓, 记录 nextAllowed["foo"]=11
//   t=2:  "bar" → 不在表中 → 打印 ✓, 记录 nextAllowed["bar"]=12
//   t=3:  "foo" → 3 < 11   → 拒绝 ✗
//   t=8:  "bar" → 8 < 12   → 拒绝 ✗
//   t=10: "foo" → 10 < 11  → 拒绝 ✗
//   t=11: "foo" → 11 >= 11 → 打印 ✓, 更新 nextAllowed["foo"]=21
// ============================================================
class Logger {
    // message → 下次允许打印的最早时间戳
    unordered_map<string, int> nextAllowed;
    
public:
    Logger() {}
    
    bool shouldPrintMessage(int timestamp, string message) {
        auto it = nextAllowed.find(message);
        
        // 消息首次出现，或已过冷却期
        if (it == nextAllowed.end() || timestamp >= it->second) {
            // 更新下次允许时间为 t + 10
            nextAllowed[message] = timestamp + 10;
            return true;
        }
        
        // 仍在冷却期内，拒绝打印
        return false;
    }
};

// ============================================================
// 解法1-变体: 存上次打印时间（等价写法，用于对比理解）
// 时间: O(1) 每次调用
// 空间: O(n)
//
// 【思路】
// 和解法1完全等价，只是换一种存储方式：
// 存 lastPrintTime 而非 nextAllowed，判断时做减法。
// 两种写法面试都可以，看个人习惯。
// ============================================================
class Logger1b {
    // message → 上次打印的时间戳
    unordered_map<string, int> lastPrintTime;
    
public:
    Logger1b() {}
    
    bool shouldPrintMessage(int timestamp, string message) {
        auto it = lastPrintTime.find(message);
        
        // 首次出现，或距上次打印已过 10 秒
        if (it == lastPrintTime.end() || timestamp - it->second >= 10) {
            lastPrintTime[message] = timestamp; // 记录本次打印时间
            return true;
        }
        return false;
    }
};

// ============================================================
// 解法2: 队列 + 集合（滑动窗口）
// 时间: O(1) 均摊（每条消息最多入队出队各一次）
// 空间: O(m) m = 最近10秒窗口内的不同消息数
//
// 【思路】
// 解法1的问题：哈希表只增不减，如果消息种类极多（比如日志含唯一ID），
// 内存会无限增长。而实际上超过10秒的记录就没用了。
//
// 优化思路：只保留"最近10秒"的消息记录。
// - 队列(queue)：按时间顺序存 (timestamp, message)，方便从头部清理过期的
// - 集合(set)：存当前窗口内的消息，用于 O(1) 判断是否重复
//
// 每次新消息来时：
// 1. 清理队头所有过期条目（timestamp <= 当前时间 - 10）
// 2. 检查 message 是否在集合中
// 3. 如果不在 → 打印，加入队列和集合
//    如果在 → 拒绝
//
// 滑动窗口示意 (当前 timestamp = 11):
//   队列: [(2,"bar"), (11,"foo")]   ← t=1的"foo"已过期被清理
//   集合: {"bar", "foo"}
//   
//   清理前队列: [(1,"foo"), (2,"bar")]
//   t=1 的 "foo": 1 <= 11-10=1 → 过期，移除
//   t=2 的 "bar": 2 > 1 → 不过期，停止清理
// ============================================================
class Logger2 {
    queue<pair<int, string>> q;  // 按时间顺序的 (timestamp, message)
    set<string> msgSet;          // 当前窗口内的消息集合
    
public:
    Logger2() {}
    
    bool shouldPrintMessage(int timestamp, string message) {
        // 步骤1: 清理过期消息
        // 为什么是 <= timestamp - 10？
        // 因为在时间 t 打印的消息，到 t+10 时可以再次打印
        // 所以 t 时刻的记录在 t+10 时已过期
        while (!q.empty() && q.front().first <= timestamp - 10) {
            msgSet.erase(q.front().second);
            q.pop();
        }
        
        // 步骤2: 检查消息是否在当前窗口内
        if (msgSet.count(message)) {
            return false; // 10秒内已打印过，拒绝
        }
        
        // 步骤3: 允许打印，加入窗口
        q.push({timestamp, message});
        msgSet.insert(message);
        return true;
    }
};

// ============================================================
// 【解法对比】
//
// | 维度       | 解法1(哈希表)         | 解法2(队列+集合)        |
// |-----------|----------------------|------------------------|
// | 实现难度   | 极简(5行核心代码)      | 中等(需维护两个结构)      |
// | 查询时间   | O(1)                 | O(1) 均摊              |
// | 空间       | O(全部消息种类) 只增不减 | O(10秒窗口内消息) 自动清理 |
// | 适用场景   | 消息种类有限           | 消息种类极多需要控制内存   |
// | 面试推荐   | ⭐ 先写这个           | 作为 follow-up 补充     |
//
// 【易错点】
//
// 1. 边界条件 >= vs >:
//    ✗ timestamp > nextAllowed[message]  → t=11 时 "foo" 不能打印(11>11为false)
//    ✓ timestamp >= nextAllowed[message] → t=11 时 "foo" 可以打印(11>=11为true)
//    题目说"直到 t+10 之前不打印"，即 t+10 时刻可以打印。
//
// 2. 首次消息处理——依赖默认值的隐患:
//    ✗ 直接写 if (timestamp >= map[message]) — 对于不存在的key，
//      map[message] 会插入默认值 0，恰好 timestamp >= 0 成立，"碰巧正确"。
//      但这引入了副作用(插入了一个多余的 key)，且代码意图不清晰。
//    ✓ 先用 find()/count() 判断是否存在，再做时间比较。
//
// 3. 解法2清理条件:
//    ✗ q.front().first < timestamp - 10  → t=1的消息在t=11时: 1 < 1 为 false，不清理
//      导致 t=11 时 "foo" 仍在集合中，返回 false（错误！）
//    ✓ q.front().first <= timestamp - 10 → t=1的消息在t=11时: 1 <= 1 为 true，正确清理
//
// 【面试追问】
//
// Q1(基础): 为什么选哈希表而不是数组来存消息记录？
// → 消息是字符串，无法直接做数组索引。哈希表支持 O(1) 的字符串查找。
//
// Q2(空间优化): 如果系统运行很久，消息种类有数百万，怎么控制内存？
// → 用解法2的滑动窗口方案，只保留近10秒的记录。
//    或者在解法1基础上，定期遍历哈希表删除过期条目（但遍历是 O(n)）。
//
// Q3(并发): 多线程环境下怎么保证正确性？
// → "查询+更新"必须是原子操作。可用 mutex 加锁，或用读写锁提高并发度。
//    更高级的方案：ConcurrentHashMap + CAS 操作。
//
// Q4(变体): 如果时间戳不保证单调递增（可能乱序到达），怎么处理？
// → 解法2的队列方案失效（依赖时间有序）。
//    解法1 仍可用，但需改逻辑：只在新时间戳更大时才更新。
//    if (timestamp >= nextAllowed[msg]) { nextAllowed[msg] = timestamp + 10; return true; }
//    如果 timestamp < nextAllowed[msg] 但消息确实过期了（旧时间戳的迟到消息），
//    需要根据业务需求决定是否打印。
// ============================================================
