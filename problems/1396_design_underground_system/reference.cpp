/*
 * LeetCode 1396: 设计地铁系统 (Design Underground System)
 *
 * 【题目本质】
 * 设计一个系统，支持乘客进站/出站，并能 O(1) 查询任意路线的平均耗时。
 * 核心在于"增量维护统计量"——在 checkOut 时就按路线聚合，而非查询时遍历。
 *
 * 【解法总览】
 * 解法1: 暴力存旅程列表   — checkIn/Out O(1), getAvg O(N) / O(N) — 最直觉
 * 解法2: 双哈希表(增量统计) — 全部 O(1) / O(P+R)             — 最优 ⭐
 *
 * 数据流示意:
 *
 *   checkIn(45, "Leyton", 3)
 *     checkInMap: { 45: ("Leyton", 3) }
 *
 *   checkIn(27, "Leyton", 10)
 *     checkInMap: { 45: ("Leyton", 3), 27: ("Leyton", 10) }
 *
 *   checkOut(45, "Waterloo", 15)
 *     → 取出 checkInMap[45] = ("Leyton", 3)
 *     → 耗时 = 15 - 3 = 12
 *     → routeMap["Leyton#Waterloo"] = (12, 1)
 *     checkInMap: { 27: ("Leyton", 10) }
 *
 *   checkOut(27, "Waterloo", 20)
 *     → 取出 checkInMap[27] = ("Leyton", 10)
 *     → 耗时 = 20 - 10 = 10
 *     → routeMap["Leyton#Waterloo"] = (22, 2)   // 12+10=22, 次数2
 *     checkInMap: {}
 *
 *   getAverageTime("Leyton", "Waterloo")
 *     → routeMap["Leyton#Waterloo"] = (22, 2)
 *     → return 22.0 / 2 = 11.0
 */

// ============================================================
// 解法1: 暴力存旅程列表
// 时间: checkIn O(1), checkOut O(1), getAverageTime O(N)
// 空间: O(N)  N = 总旅程数
//
// 【思路】
// 最直觉的做法：checkOut 完成旅程时，把 (起点, 终点, 耗时) 存入列表。
// getAverageTime 时遍历列表，筛选匹配路线的旅程求平均。
//
// 瓶颈：getAverageTime 每次都要遍历全部历史旅程。
// 如果有 10万次旅程记录，每次查询都是 O(10万)。
// ============================================================
class UndergroundSystem1 {
    // id → (进站名, 进站时间)
    unordered_map<int, pair<string, int>> checkInMap;
    // 所有完成的旅程: (起点站, 终点站, 耗时)
    vector<tuple<string, string, int>> trips;

public:
    UndergroundSystem1() {}

    void checkIn(int id, string stationName, int t) {
        checkInMap[id] = {stationName, t};
    }

    void checkOut(int id, string stationName, int t) {
        auto& [startStation, startTime] = checkInMap[id];
        // 旅程完成，记录到列表
        trips.emplace_back(startStation, stationName, t - startTime);
        checkInMap.erase(id);
    }

    double getAverageTime(string startStation, string endStation) {
        double totalTime = 0;
        int count = 0;
        // 瓶颈在这里：每次查询都遍历所有历史旅程
        for (auto& [s, e, duration] : trips) {
            if (s == startStation && e == endStation) {
                totalTime += duration;
                count++;
            }
        }
        return totalTime / count;
    }
};

// ============================================================
// 解法2: 双哈希表 — 增量维护统计量 ⭐ 面试首选
// 时间: checkIn O(1), checkOut O(1), getAverageTime O(1)
// 空间: O(P + R)  P = 同时在途乘客数, R = 不同路线数
//
// 【思路】
// 从解法1的瓶颈出发：getAverageTime 每次遍历是因为"查询时才聚合"。
// 
// 关键观察：平均值 = 总时间 / 次数。
// 我们不需要存每次旅程的详情，只需要在 checkOut 时按路线累加
// totalTime 和 count。查询时直接 totalTime / count 就是 O(1)。
//
// 两个哈希表分工明确：
// - checkInMap (临时): 存"正在旅途中"的乘客进站信息，用完即删
// - routeMap   (持久): 存每条路线的累计统计数据，只增不减
//
// 为什么用字符串拼接做 key 而不是 pair<string,string>？
// 因为 unordered_map 不支持 pair 作为 key（没有默认哈希函数），
// 拼接字符串加分隔符是最简单的方案。
// ============================================================
class UndergroundSystem {
    // 表1: 在途乘客 —— id → (进站名, 进站时间)
    unordered_map<int, pair<string, int>> checkInMap;

    // 表2: 路线统计 —— "起点#终点" → (累计总耗时, 旅程次数)
    // 用 double 存 totalTime 避免后续除法时的整数截断
    unordered_map<string, pair<double, int>> routeMap;

public:
    UndergroundSystem() {}

    void checkIn(int id, string stationName, int t) {
        // 记录乘客的进站信息
        // 题目保证同一乘客不会重复 checkIn（未 checkOut 前不会再 checkIn）
        checkInMap[id] = {stationName, t};
    }

    void checkOut(int id, string stationName, int t) {
        // 取出进站信息
        auto& [startStation, startTime] = checkInMap[id];

        // 构造路线 key
        // 为什么用 '#' 分隔？
        // 避免 "ab"+"c" 和 "a"+"bc" 拼出相同字符串 "abc" 的歧义
        string route = startStation + "#" + stationName;

        // 增量更新统计量：这就是优化的关键！
        // 不需要存每次旅程详情，只维护总和与计数
        routeMap[route].first  += (t - startTime);  // 累加耗时
        routeMap[route].second += 1;                 // 累加次数

        // 乘客已出站，删除进站记录释放空间
        // (不删也能 AC，下次 checkIn 会覆盖，但删除更规范)
        checkInMap.erase(id);
    }

    double getAverageTime(string startStation, string endStation) {
        // O(1) 查表
        string route = startStation + "#" + endStation;
        auto& [totalTime, count] = routeMap[route];
        return totalTime / count;
    }
};

// ============================================================
// 【解法对比】
//
// | 操作           | 解法1 (暴力列表) | 解法2 (双哈希表) |
// |----------------|------------------|------------------|
// | checkIn        | O(1)             | O(1)             |
// | checkOut       | O(1)             | O(1)             |
// | getAverageTime | O(N) 遍历全部    | O(1) 查表        |
// | 空间           | O(N)             | O(P + R)         |
//
// 核心区别: 解法1是"延迟计算"(查询时遍历), 解法2是"增量维护"(更新时聚合)
// 这是设计题的经典思维: 能在写入时预计算的，就不要留到读取时再算。
//
// ============================================================
// 【易错点】
//
// 1. 路线 key 拼接歧义:
//    ✗ route = startStation + endStation;
//      "Paradise" + "Cambridge" = "ParadiseCambridge"
//      如果有 "ParadiseC" + "ambridge" 也得到 "ParadiseCambridge"!
//    ✓ route = startStation + "#" + endStation;
//
// 2. 整数除法精度丢失:
//    ✗ pair<int, int> 存 (totalTime, count)，return totalTime / count;
//      → 22 / 3 = 7 (整数除法截断!)
//    ✓ totalTime 用 double 存储，或者 return (double)totalTime / count;
//
// 3. 用 pair<string,string> 做 unordered_map 的 key:
//    ✗ unordered_map<pair<string,string>, ...> routeMap;
//      → 编译错误! pair 没有默认哈希函数
//    ✓ 拼接字符串做 key，或自定义哈希:
//      struct PairHash {
//          size_t operator()(const pair<string,string>& p) const {
//              return hash<string>()(p.first) ^ (hash<string>()(p.second) << 32);
//          }
//      };
//
// 4. checkOut 后不删除 checkInMap 中的记录:
//    虽然不影响正确性（下次 checkIn 会覆盖），但如果面试官问
//    "乘客量很大但同时在途的人不多"的场景，不删除会浪费内存。
//
// ============================================================
// 【面试追问 — 递进链】
//
// Q1 (基础理解): 为什么需要两个哈希表？能不能只用一个？
//    → checkInMap 存"临时状态"(在途)，routeMap 存"持久统计"(聚合结果)。
//      用一个也能做（如暴力列表），但 getAvg 就不是 O(1)。
//      两个表的设计体现了"关注点分离"的原则。
//
// Q2 (功能扩展): 如果还需要支持 getMedianTime(start, end)？
//    → 中位数无法通过 (totalTime, count) 增量维护。
//      方案A: routeMap 的 value 改为 vector<int> 存所有耗时，
//              查询时排序或用 nth_element，O(N log N) 或 O(N)。
//      方案B: 用双堆(大顶堆+小顶堆)实时维护中位数，
//              checkOut 时 O(log N) 插入，查询 O(1)。
//              参考 LeetCode 295。
//
// Q3 (系统设计): 百万级并发，单机放不下怎么办？
//    → (1) checkInMap 按 userId 分片到不同节点
//      (2) routeMap 按路线 key 分片
//      (3) checkOut 需要跨分片获取 checkIn 信息 —— 可以用一致性哈希
//          把同一用户的 in/out 路由到同一节点
//      (4) getAvg 可接受最终一致性 —— 用消息队列异步更新统计
// ============================================================
