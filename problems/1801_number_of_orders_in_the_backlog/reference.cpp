/*
 * LeetCode 1801: 积压订单中的订单总数 (Number of Orders in the Backlog)
 *
 * 【题目本质】
 * 模拟订单簿撮合引擎：每笔订单尝试和对手方"最优价格"匹配，
 * 关键是快速获取 buy 积压中的最高价 和 sell 积压中的最低价。
 *
 * 【解法总览】
 * 解法1: 暴力模拟 — O(n×m) / O(n) — 线性查找极值，直觉但慢
 * 解法2: 双堆模拟 — O(n log n) / O(n) — 大顶堆(buy)+小顶堆(sell)，面试首选
 */

// ============================================================
// 解法1: 暴力模拟 — 两个列表 + 线性扫描极值
// 时间: O(n × m)  空间: O(n)
//   n = orders数量, m = 积压订单最大规模
//
// 【思路】
// 最直觉的做法：用两个 vector 分别存 buy/sell 积压订单。
// 每来一笔订单，线性扫描对手方列表找极值。
// 瓶颈很明显：每次线性查找是 O(m)，当积压很多时极慢。
// 这个解法用来理解题意，面试中说完思路即可转向堆优化。
// ============================================================

// 模拟过程 (示例1):
//
// orders: [10,5,buy] [15,2,sell] [25,1,sell] [30,4,buy]
//
// Step1: buy(10,5) → sells空 → buys: [(10,5)]
// Step2: sell(15,2) → buys最高价10 < 15 → 不匹配 → sells: [(15,2)]
// Step3: sell(25,1) → buys最高价10 < 25 → 不匹配 → sells: [(15,2),(25,1)]
// Step4: buy(30,4) → sells最低价15 ≤ 30 → 匹配2笔 → 剩余2
//                   → sells最低价25 ≤ 30 → 匹配1笔 → 剩余1
//                   → sells空 → buys: [(10,5),(30,1)]
// 结果: 5 + 1 = 6 ✓

class Solution1 {
public:
    int getNumberOfBacklogOrders(vector<vector<int>>& orders) {
        const int MOD = 1e9 + 7;
        vector<pair<int,int>> buys, sells; // {price, amount}
        
        for (auto& order : orders) {
            int price = order[0], amount = order[1], type = order[2];
            
            if (type == 0) { // buy 订单：和 sell 中最低价匹配
                while (amount > 0 && !sells.empty()) {
                    // 线性查找最低价 sell — O(m) 瓶颈
                    int minIdx = 0;
                    for (int i = 1; i < (int)sells.size(); i++) {
                        if (sells[i].first < sells[minIdx].first)
                            minIdx = i;
                    }
                    if (sells[minIdx].first > price) break;
                    
                    int matched = min(amount, sells[minIdx].second);
                    amount -= matched;
                    sells[minIdx].second -= matched;
                    if (sells[minIdx].second == 0)
                        sells.erase(sells.begin() + minIdx);
                }
                if (amount > 0) buys.push_back({price, amount});
            } else { // sell 订单：和 buy 中最高价匹配
                while (amount > 0 && !buys.empty()) {
                    int maxIdx = 0;
                    for (int i = 1; i < (int)buys.size(); i++) {
                        if (buys[i].first > buys[maxIdx].first)
                            maxIdx = i;
                    }
                    if (buys[maxIdx].first < price) break;
                    
                    int matched = min(amount, buys[maxIdx].second);
                    amount -= matched;
                    buys[maxIdx].second -= matched;
                    if (buys[maxIdx].second == 0)
                        buys.erase(buys.begin() + maxIdx);
                }
                if (amount > 0) sells.push_back({price, amount});
            }
        }
        
        long long total = 0;
        for (auto& [p, a] : buys) total = (total + a) % MOD;
        for (auto& [p, a] : sells) total = (total + a) % MOD;
        return (int)total;
    }
};

// ============================================================
// 解法2: 双堆模拟 — 大顶堆(buy) + 小顶堆(sell) ⭐ 面试首选
// 时间: O(n log n)  空间: O(n)
//
// 【思路】
// 解法1的瓶颈：每次线性扫描找极值 O(m)。
// 堆可以 O(1) 取极值 + O(log n) 插入/删除，完美解决。
//
// 为什么两个堆方向不同？
// - buy 积压池: sell来了要匹配"出价最高的买家" → 大顶堆
// - sell 积压池: buy来了要匹配"报价最低的卖家" → 小顶堆
//
// 匹配过程是 while 循环：一笔 amount=100 的 buy 可能
// 连续匹配多个小额 sell，直到 amount 耗尽或无法匹配。
// 部分匹配后，对手方剩余量要放回堆。
//
// 堆中存 pair<price, amount>:
//   buyHeap: max-heap by price (默认 priority_queue)
//   sellHeap: min-heap by price (用 greater<>)
// ============================================================

// 匹配流程可视化:
//
// 来一笔 buy(price=30, amount=4):
//
//   sellHeap (小顶堆):
//   ┌─────────┐
//   │ (15, 2) │ ← 堆顶 = 最低卖价
//   │ (25, 1) │
//   └─────────┘
//
//   Round1: sellTop=(15,2), 15 ≤ 30 → 匹配 min(4,2)=2
//           buy剩余=2, sell(15)耗尽弹出
//   Round2: sellTop=(25,1), 25 ≤ 30 → 匹配 min(2,1)=1
//           buy剩余=1, sell(25)耗尽弹出
//   Round3: sellHeap空 → 停止循环
//   buy剩余1 → push到buyHeap

class Solution2 {
public:
    int getNumberOfBacklogOrders(vector<vector<int>>& orders) {
        const int MOD = 1e9 + 7;
        
        // buy积压: 大顶堆（默认），堆顶 = 最高买价
        priority_queue<pair<int,int>> buyHeap;
        // sell积压: 小顶堆，堆顶 = 最低卖价
        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> sellHeap;
        
        for (auto& order : orders) {
            int price = order[0], amount = order[1], type = order[2];
            
            if (type == 0) { // buy 订单
                // 循环匹配: 只要sell堆非空 且 最低卖价 ≤ 当前买价
                while (amount > 0 && !sellHeap.empty()
                       && sellHeap.top().first <= price) {
                    auto [sellPrice, sellAmt] = sellHeap.top();
                    sellHeap.pop();
                    
                    // 本次能匹配的量 = 两者amount的较小值
                    int matched = min(amount, sellAmt);
                    amount -= matched;
                    sellAmt -= matched;
                    
                    // 部分匹配: sell订单没用完，放回堆
                    if (sellAmt > 0)
                        sellHeap.push({sellPrice, sellAmt});
                }
                // buy还有剩余，加入buy积压
                if (amount > 0)
                    buyHeap.push({price, amount});
                
            } else { // sell 订单
                // 循环匹配: 只要buy堆非空 且 最高买价 ≥ 当前卖价
                while (amount > 0 && !buyHeap.empty()
                       && buyHeap.top().first >= price) {
                    auto [buyPrice, buyAmt] = buyHeap.top();
                    buyHeap.pop();
                    
                    int matched = min(amount, buyAmt);
                    amount -= matched;
                    buyAmt -= matched;
                    
                    if (buyAmt > 0)
                        buyHeap.push({buyPrice, buyAmt});
                }
                if (amount > 0)
                    sellHeap.push({price, amount});
            }
        }
        
        // 统计所有积压订单总量
        long long total = 0;
        while (!buyHeap.empty()) {
            total = (total + buyHeap.top().second) % MOD;
            buyHeap.pop();
        }
        while (!sellHeap.empty()) {
            total = (total + sellHeap.top().second) % MOD;
            sellHeap.pop();
        }
        return (int)total;
    }
};

// ============================================================
// 解法3: 有序映射 (std::map) — 和双堆等价的另一种实现
// 时间: O(n log n)  空间: O(n)
//
// 【思路】
// 用 map<int, int> 代替堆:
//   buyMap: 价格→数量, rbegin()取最高价
//   sellMap: 价格→数量, begin()取最低价
// 好处: 同价格的订单自动合并，且支持任意价格的删除(撤单场景)
// 和堆解法复杂度相同，但在面试追问"如何支持撤单"时更灵活
// ============================================================

class Solution3 {
public:
    int getNumberOfBacklogOrders(vector<vector<int>>& orders) {
        const int MOD = 1e9 + 7;
        
        // key=price, value=total amount at that price
        map<int,int> buyMap, sellMap;
        
        for (auto& order : orders) {
            int price = order[0], amount = order[1], type = order[2];
            
            if (type == 0) { // buy
                // sellMap.begin() 是最低卖价
                while (amount > 0 && !sellMap.empty()) {
                    auto it = sellMap.begin(); // 最低价
                    if (it->first > price) break;
                    
                    int matched = min(amount, it->second);
                    amount -= matched;
                    it->second -= matched;
                    if (it->second == 0) sellMap.erase(it);
                }
                if (amount > 0) buyMap[price] += amount;
                
            } else { // sell
                // buyMap.rbegin() 是最高买价, 但用 prev(end()) 更方便操作
                while (amount > 0 && !buyMap.empty()) {
                    auto it = prev(buyMap.end()); // 最高价
                    if (it->first < price) break;
                    
                    int matched = min(amount, it->second);
                    amount -= matched;
                    it->second -= matched;
                    if (it->second == 0) buyMap.erase(it);
                }
                if (amount > 0) sellMap[price] += amount;
            }
        }
        
        long long total = 0;
        for (auto& [p, a] : buyMap) total = (total + a) % MOD;
        for (auto& [p, a] : sellMap) total = (total + a) % MOD;
        return (int)total;
    }
};

// ============================================================
// 【解法对比】
//
// | 操作     | 解法1(列表)  | 解法2(堆)    | 解法3(map)   |
// |----------|-------------|-------------|-------------|
// | 取极值   | O(m) 扫描   | O(1) 堆顶   | O(1) begin  |
// | 插入     | O(1)        | O(log n)    | O(log n)    |
// | 删除极值 | O(m) erase  | O(log n)    | O(log n)    |
// | 删除任意 | O(m)        | 不直接支持  | O(log n) ✓  |
// | 同价合并 | 手动处理    | 不合并      | 自动合并 ✓  |
//
// 解法2(堆)是标准面试解法，代码最自然。
// 解法3(map)在需要"撤单"等扩展操作时更优。
//
// ============================================================
// 【易错点】
//
// 1. 堆方向搞反:
//    ✗ buy用小顶堆 → 取出最低买价，应该取最高
//    ✓ buy用大顶堆(默认priority_queue), sell用小顶堆(greater<>)
//
// 2. 只匹配一次不循环:
//    ✗ if (...) { 匹配一次; }
//    ✓ while (amount > 0 && !heap.empty() && 条件) { ... }
//    一笔大额订单可能吃掉多个小额对手方
//
// 3. 部分匹配后忘记放回:
//    ✗ pop后不管剩余量
//    ✓ if (sellAmt > 0) sellHeap.push({sellPrice, sellAmt})
//
// 4. 最终统计用int溢出:
//    ✗ int total; total += amount;  // 累加多个10^9溢出
//    ✓ long long total; total = (total + amount) % MOD;
//
// 5. 匹配条件写反:
//    buy来了找sell: sellPrice <= buyPrice (卖价够低)
//    sell来了找buy: buyPrice >= sellPrice (买价够高)
//    虽然数学等价，但在代码中弄混了 <= 和 >= 的主语会出bug
//
// ============================================================
// 【面试追问】
//
// Q1: 为什么用两个堆而不是一个？
// → buy要找"最高价"(大顶堆), sell要找"最低价"(小顶堆),
//   方向相反，无法用同一个堆。
//
// Q2: 能否用 map 代替堆？有什么优势？
// → 可以(解法3)。优势: 同价格订单自动合并减少条目数，
//   且支持 O(log n) 删除任意价格(撤单场景)。
//   堆不支持高效删除任意元素。
//
// Q3: 如果要实时返回"当前最优买价/卖价"(bid/ask)，怎么做？
// → 就是堆顶/map端点。buyHeap.top().first 是最优买价(bid),
//   sellHeap.top().first 是最优卖价(ask)。O(1) 查询。
//   这正是真实交易系统的 Order Book 结构。
//
// Q4: 如果订单量极大(10^8)且需要持久化，还用堆吗？
// → 实际交易系统用红黑树(map/set)或跳表，因为需要:
//   - 范围查询(某个价格区间有多少订单)
//   - 删除任意元素(撤单)
//   - 持久化友好
//   堆只适合"取极值"场景。
// ============================================================
