# 1396. 设计地铁系统

## 核心思路

本题本质上是一个**数据结构设计题**：你需要设计两个哈希表，一个跟踪"正在旅途中"的乘客（id → 进站信息），另一个累计"每条路线"的总时间和次数（路线 → 统计数据），从而实现 O(1) 的 checkIn、checkOut 和 getAverageTime。

## 思维链

1. **读完题第一反应**：每个乘客 checkIn 记录进站信息，checkOut 时能算出本次旅程耗时，getAverageTime 需要某条路线的平均耗时。最朴素的做法是什么？——把每次完成的旅程（起点、终点、耗时）全部存到一个列表里，getAverageTime 时遍历所有旅程，筛选匹配的再求平均。

2. **暴力解的瓶颈在哪？**——checkIn 和 checkOut 都是 O(1)，但 getAverageTime 需要遍历所有历史旅程，是 O(N)，N 为总旅程数。如果查询频繁，这个代价太大。

3. **怎么突破瓶颈？**——我们不需要每次查询都重新扫描所有旅程。如果在 checkOut 时就按路线（startStation→endStation）分组累计 **总时间** 和 **次数**，那查询时直接 `总时间/次数` 就行，O(1)！

4. **需要什么数据结构？**
   - **哈希表1** `checkInMap`：`id → (stationName, time)`，记录当前在途乘客的进站信息。checkIn 时写入，checkOut 时读取并删除。
   - **哈希表2** `routeMap`：`"start→end" → (totalTime, count)`，累计每条路线的统计数据。checkOut 时更新，getAverageTime 时查询。

5. **路线的 key 怎么表示？**——用 `pair<string, string>` 或者拼接字符串 `start + "#" + end`（注意分隔符不能出现在站名中）。

6. **验证**：三个操作全是哈希表的查/插/改，全部 O(1)，空间 O(P + S²)，P 为乘客数，S 为站点数。

## 解法概览

| 解法 | 思路 | 时间(每次操作) | 空间 | 面试 |
|------|------|------|------|------|
| 暴力存旅程列表 | checkOut 存旅程到列表，getAvg 遍历筛选 | checkIn/Out O(1), getAvg O(N) | O(N) | 能说出即可 |
| 双哈希表 ⭐ | 在途表 + 路线统计表，增量维护 | 全部 O(1) | O(P + R) | ⭐ 必须写出 |

> N = 总旅程数，P = 同时在途乘客数，R = 不同路线数

## 关键提示

1. **checkIn 和 checkOut 是配对的**：一个乘客 checkIn 后一定会 checkOut，且中间不会再 checkIn。所以用 `id` 做 key，checkIn 存、checkOut 取，天然配对。

2. **不要存每次旅程的详情**：如果你发现自己在 getAverageTime 里遍历列表，想想能不能在 checkOut 时就把信息"折叠"进统计量（总时间 + 次数）。

3. **路线是有方向的**：`"A→B"` 和 `"B→A"` 是不同的路线，key 必须区分方向。

4. **字符串拼接做 key 时注意分隔符**：如果用 `start + end` 可能有歧义（"ab" + "c" vs "a" + "bc"），加一个特殊分隔符如 `"#"` 或用 `pair`。

5. **数据流示意**：
```
checkIn(45, "Leyton", 3)
  → checkInMap[45] = ("Leyton", 3)

checkOut(45, "Waterloo", 15)
  → 从 checkInMap[45] 取出 ("Leyton", 3)
  → 耗时 = 15 - 3 = 12
  → routeMap["Leyton#Waterloo"].totalTime += 12
  → routeMap["Leyton#Waterloo"].count += 1
  → 删除 checkInMap[45]

getAverageTime("Leyton", "Waterloo")
  → return routeMap["Leyton#Waterloo"].totalTime / count
```

## 解法详解

### 解法1: 暴力存旅程列表 — checkIn/Out O(1), getAvg O(N)

**思考过程**: 最直觉的做法——checkOut 完成一次旅程时，把 `(start, end, time)` 存到一个列表里。getAverageTime 时遍历列表，找到所有 start 和 end 匹配的旅程，算平均值。

```cpp
class UndergroundSystem {
    // id → (stationName, checkInTime)
    unordered_map<int, pair<string, int>> checkInMap;
    // 存储所有完成的旅程: (start, end, duration)
    vector<tuple<string, string, int>> trips;
    
public:
    UndergroundSystem() {}
    
    void checkIn(int id, string stationName, int t) {
        checkInMap[id] = {stationName, t};
    }
    
    void checkOut(int id, string stationName, int t) {
        auto& [startStation, startTime] = checkInMap[id];
        trips.emplace_back(startStation, stationName, t - startTime);
        checkInMap.erase(id);
    }
    
    double getAverageTime(string startStation, string endStation) {
        double totalTime = 0;
        int count = 0;
        // 瓶颈：每次查询都遍历所有历史旅程
        for (auto& [s, e, duration] : trips) {
            if (s == startStation && e == endStation) {
                totalTime += duration;
                count++;
            }
        }
        return totalTime / count;
    }
};
```

**关键点**: 
- 这个解法的瓶颈很明显：getAverageTime 是 O(N)，N 是所有历史旅程数量。如果系统运行很久，每次查询都很慢。
- 但 checkIn 和 checkOut 的设计思路是对的——用 `checkInMap` 暂存进站信息，这个思路在优化解法中保留。

---

### 解法2: 双哈希表（增量统计） — 全部 O(1) ⭐ 面试首选

**从解法1优化**: 解法1的瓶颈在 getAverageTime 需要遍历。**核心观察：平均值 = 总时间 / 次数**，我们不需要存每次旅程的详情，只需要在 checkOut 时累加 `totalTime` 和 `count`，查询时直接除就行。

```cpp
class UndergroundSystem {
    // 表1: 正在旅途中的乘客 —— id → (进站名, 进站时间)
    unordered_map<int, pair<string, int>> checkInMap;
    
    // 表2: 每条路线的统计数据 —— "start#end" → (总耗时, 旅程次数)
    unordered_map<string, pair<double, int>> routeMap;
    
public:
    UndergroundSystem() {}
    
    void checkIn(int id, string stationName, int t) {
        // 乘客进站，记录进站信息
        checkInMap[id] = {stationName, t};
    }
    
    void checkOut(int id, string stationName, int t) {
        // 取出该乘客的进站信息
        auto& [startStation, startTime] = checkInMap[id];
        
        // 构造路线 key（用 '#' 分隔避免歧义）
        string route = startStation + "#" + stationName;
        
        // 增量更新：累加耗时和次数
        routeMap[route].first += (t - startTime);
        routeMap[route].second += 1;
        
        // 进站信息用完即删（乘客已出站）
        checkInMap.erase(id);
    }
    
    double getAverageTime(string startStation, string endStation) {
        // 直接查表，O(1)
        string route = startStation + "#" + endStation;
        auto& [totalTime, count] = routeMap[route];
        return totalTime / count;
    }
};
```

**关键点**:
- `routeMap` 的 value 用 `pair<double, int>` 而不是 `pair<int, int>`：避免整数除法丢失精度。或者在 getAverageTime 中做 `(double)totalTime / count`。
- checkOut 后 `erase` 进站记录：题目保证乘客出站后才能再进站，erase 可省内存但不是必须的（不 erase 也能 AC，因为下次 checkIn 会覆盖）。

## 解法对比

| | 解法1 (暴力列表) | 解法2 (双哈希表) |
|---|---|---|
| checkIn | O(1) | O(1) |
| checkOut | O(1) | O(1) |
| getAverageTime | **O(N)** 遍历所有旅程 | **O(1)** 直接查表 |
| 空间 | O(N) 存所有旅程 | O(P + R) 只存在途+路线统计 |
| 核心区别 | "延迟计算"：查询时才筛选 | "增量维护"：更新时就聚合 |

**选哪个？** 面试中直接给解法2，但可以先口述解法1展示思路，然后说"但 getAvg 是 O(N)，可以在 checkOut 时增量维护统计量来优化到 O(1)"。

## 易错点

1. **路线 key 拼接歧义**
   - ✗ `string route = startStation + endStation;` → "Paradise" + "Cambridge" = "ParadiseCambridge"，但如果有站名 "ParadiseC" + "ambridge" 也会得到相同字符串！
   - ✓ `string route = startStation + "#" + endStation;` → 用不会出现在站名中的分隔符

2. **整数除法丢失精度**
   - ✗ `return totalTime / count;`（如果 totalTime 是 int）→ 结果被截断为整数
   - ✓ 把 totalTime 声明为 double，或返回时 `(double)totalTime / count`

3. **忘记处理 checkOut 时查不到 checkIn 记录**
   - 题目保证调用合法，所以不会出这个问题。但如果面试官追问"如果调用不合法怎么办"，你需要加检查。

4. **用 pair<string,string> 做 key 时忘记自定义哈希**
   - `unordered_map` 不支持 `pair` 作为 key（没有默认哈希）。要么拼接字符串，要么自己写哈希函数，要么用 `map`（O(log n)）。

## 面试追问

**Q1: 基础理解** — 为什么需要两个哈希表？一个行不行？
> 两个表存的东西本质不同：`checkInMap` 是**临时数据**（乘客在途状态，用完即删），`routeMap` 是**持久统计**（累计数据，越来越大）。用一个表也能做（比如全存旅程列表），但查询就不是 O(1) 了。

**Q2: 优化/扩展** — 如果还要支持 `getMedianTime(start, end)` 怎么办？
> 中位数无法通过 `(totalTime, count)` 增量维护，必须存每次旅程的耗时。可以给 routeMap 的 value 改成 `vector<int>` 或用两个堆（大顶堆 + 小顶堆）实时维护中位数，类似 LeetCode 295 数据流的中位数。

**Q3: 系统设计** — 如果这个系统有百万级并发用户，怎么处理？
> 单机哈希表不够了。需要分布式：(1) checkInMap 可以按 userId 分片到不同服务器；(2) routeMap 可以按路线 key 分片；(3) checkOut 时可能需要跨分片通信获取 checkIn 信息，或者把同一用户的 checkIn/checkOut 路由到同一分片；(4) getAverageTime 考虑用消息队列异步更新统计，接受最终一致性。

## 相关题型

- **LeetCode 355. 设计推特 (Design Twitter)** — 同样是多哈希表协作的设计题，复用"用不同哈希表存不同维度的数据"的思路
- **LeetCode 146. LRU 缓存 (LRU Cache)** — 经典设计题，哈希表 + 双向链表；复用"多数据结构组合实现 O(1) 操作"的设计模式
- **LeetCode 295. 数据流的中位数** — 如果本题追问中位数，直接复用 295 的双堆方案
- **LeetCode 588. 设计内存文件系统** — 另一道用哈希表/Trie 设计的系统题