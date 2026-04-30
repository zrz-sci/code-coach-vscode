# 1801. 积压订单中的订单总数

## 核心思路

这道题本质上是**模拟一个订单簿（Order Book）的撮合引擎**：每来一笔订单，尝试和积压订单中"最优价格"的对手方匹配，匹配不了的加入积压。关键操作是"快速找到最优价格"——买方要找最低卖价，卖方要找最高买价。

## 思维链

1. **读完题第一反应**：按顺序处理每笔订单，需要维护两个积压池（buy 池和 sell 池），每次匹配时需要找到对手方的"最优价格"。
2. **最朴素的做法**：用两个列表存积压订单，每次匹配时遍历整个列表找最低卖价 / 最高买价。每笔订单匹配一次是 O(n)，总共 O(n²)。
3. **暴力解的瓶颈**：每次都要遍历整个列表去找极值——"找最小值 / 最大值"这个操作太慢了。
4. **突破瓶颈**：什么数据结构能 O(log n) 获取极值？**堆（Priority Queue）**！sell 池用**小顶堆**（最低价在堆顶），buy 池用**大顶堆**（最高价在堆顶）。
5. **核心逻辑**：来一笔 buy 订单 → 从 sell 小顶堆取堆顶，如果堆顶价 ≤ buy 价，匹配消耗，循环直到不能匹配 → 剩余 amount 加入 buy 大顶堆。sell 订单同理反过来。
6. **最后统计**：清空两个堆，累加所有剩余 amount，对 10⁹+7 取模。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力模拟 | 两个列表 + 线性查找极值 | O(n × m) | O(n) | 能说出即可 |
| 双堆模拟 | 大顶堆(buy) + 小顶堆(sell) | O(n log n) | O(n) | ⭐ 必须写出 |

> n = orders 总数，m = 积压订单池大小。双堆解法中每笔订单最多入堆出堆各一次，所以总操作 O(n log n)。

## 关键提示

1. **两个堆的方向不同**：buy 积压池要大顶堆（因为 sell 来了要找最高买价），sell 积压池要小顶堆（因为 buy 来了要找最低卖价）。
2. **匹配是 while 循环**：一笔订单可能和多个积压订单连续匹配（比如 amount=4 的 buy 可能先匹配 amount=2 的 sell，再匹配 amount=1 的 sell……），不要只匹配一次。
3. **amount 可能部分消耗**：堆顶订单的 amount 可能比当前订单大，此时只减少堆顶的 amount，不弹出。
4. **取模时机**：中间匹配过程不需要取模（amount 不会溢出 int，最大 10⁹），只在最后统计总数时取模。但注意总数累加时要取模（因为可能累加很多个 10⁹ 级别的数）。
5. **模拟过程示意**（以示例1为例）：

```
orders: [10,5,buy] [15,2,sell] [25,1,sell] [30,4,buy]

Step1: buy(10,5) → sell堆空 → 加入buy堆
  buy堆: {(10,5)}    sell堆: {}

Step2: sell(15,2) → buy堆顶价10 < 15 → 不匹配 → 加入sell堆
  buy堆: {(10,5)}    sell堆: {(15,2)}

Step3: sell(25,1) → buy堆顶价10 < 25 → 不匹配 → 加入sell堆
  buy堆: {(10,5)}    sell堆: {(15,2), (25,1)}

Step4: buy(30,4) → sell堆顶价15 ≤ 30 → 匹配!
  消耗 min(4,2)=2, buy剩余2, sell(15)耗尽弹出
  → sell堆顶价25 ≤ 30 → 匹配!
  消耗 min(2,1)=1, buy剩余1, sell(25)耗尽弹出
  → sell堆空 → 停止, 剩余1加入buy堆
  buy堆: {(10,5),(30,1)}    sell堆: {}

总计: 5 + 1 = 6 ✓
```

## 解法详解

### 解法1: 暴力模拟 — O(n × m) / O(n)

**思考过程**: 最直接的想法——用两个数组分别存 buy 和 sell 的积压订单，每次来新订单时线性扫描找到最优价格的对手方进行匹配。

```cpp
class Solution {
public:
    int getNumberOfBacklogOrders(vector<vector<int>>& orders) {
        const int MOD = 1e9 + 7;
        // buys[i] = {price, amount}, sells[i] = {price, amount}
        vector<pair<int,int>> buys, sells;
        
        for (auto& order : orders) {
            int price = order[0], amount = order[1], type = order[2];
            
            if (type == 0) { // buy 订单：找 sell 中最低价
                while (amount > 0 && !sells.empty()) {
                    // 线性查找最低价 sell — 这就是瓶颈！
                    int minIdx = 0;
                    for (int i = 1; i < sells.size(); i++) {
                        if (sells[i].first < sells[minIdx].first)
                            minIdx = i;
                    }
                    if (sells[minIdx].first > price) break; // 最低卖价都太高
                    
                    int matched = min(amount, sells[minIdx].second);
                    amount -= matched;
                    sells[minIdx].second -= matched;
                    if (sells[minIdx].second == 0)
                        sells.erase(sells.begin() + minIdx);
                }
                if (amount > 0) buys.push_back({price, amount});
            } else { // sell 订单：找 buy 中最高价
                while (amount > 0 && !buys.empty()) {
                    int maxIdx = 0;
                    for (int i = 1; i < buys.size(); i++) {
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
        return total;
    }
};
```

**关键点**: 每次匹配都要线性扫描找极值，积压订单多时会 TLE。这就是为什么需要堆。

---

### 解法2: 双堆模拟 — O(n log n) / O(n) ⭐ 面试首选

**从解法1优化**: 解法1的瓶颈是"每次线性查找极值"，堆天然支持 O(1) 取极值 + O(log n) 插入删除，完美匹配需求。

- **buy 积压池 → 大顶堆**：sell 来了要找最高买价，大顶堆堆顶就是最高价
- **sell 积压池 → 小顶堆**：buy 来了要找最低卖价，小顶堆堆顶就是最低价

```cpp
class Solution {
public:
    int getNumberOfBacklogOrders(vector<vector<int>>& orders) {
        const int MOD = 1e9 + 7;
        
        // buy积压: 大顶堆, 堆顶是最高买价 (pair默认按first降序)
        priority_queue<pair<int,int>> buyHeap;
        // sell积压: 小顶堆, 堆顶是最低卖价
        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> sellHeap;
        
        for (auto& order : orders) {
            int price = order[0], amount = order[1], type = order[2];
            
            if (type == 0) { // buy 订单
                // 尝试和 sell 积压中最低价匹配
                while (amount > 0 && !sellHeap.empty() 
                       && sellHeap.top().first <= price) {
                    auto [sellPrice, sellAmt] = sellHeap.top();
                    sellHeap.pop();
                    
                    int matched = min(amount, sellAmt);
                    amount -= matched;
                    sellAmt -= matched;
                    // 如果 sell 订单没用完，放回堆
                    if (sellAmt > 0) sellHeap.push({sellPrice, sellAmt});
                }
                // buy 还有剩余，加入 buy 积压
                if (amount > 0) buyHeap.push({price, amount});
                
            } else { // sell 订单
                // 尝试和 buy 积压中最高价匹配
                while (amount > 0 && !buyHeap.empty() 
                       && buyHeap.top().first >= price) {
                    auto [buyPrice, buyAmt] = buyHeap.top();
                    buyHeap.pop();
                    
                    int matched = min(amount, buyAmt);
                    amount -= matched;
                    buyAmt -= matched;
                    if (buyAmt > 0) buyHeap.push({buyPrice, buyAmt});
                }
                if (amount > 0) sellHeap.push({price, amount});
            }
        }
        
        // 统计积压订单总量
        long long total = 0;
        while (!buyHeap.empty()) {
            total = (total + buyHeap.top().second) % MOD;
            buyHeap.pop();
        }
        while (!sellHeap.empty()) {
            total = (total + sellHeap.top().second) % MOD;
            sellHeap.pop();
        }
        return total;
    }
};
```

**关键点**:
- `while` 循环确保一笔大 amount 的订单能连续匹配多个对手方
- 部分匹配后要把剩余的 push 回堆
- 统计时用 `long long` 避免溢出，每步取模

## 解法对比

| | 解法1 暴力 | 解法2 双堆 |
|---|---|---|
| 找极值 | O(m) 线性扫描 | O(1) 堆顶 |
| 插入/删除 | O(1) / O(m) | O(log n) |
| 总时间 | O(n × m) ≈ O(n²) | O(n log n) |
| 面试价值 | 说明理解题意 | **必须写出** |

## 易错点

1. **堆的方向搞反**：
   - ✗ buy 用小顶堆 → 取出的是最低买价，应该找最高买价才对
   - ✓ buy 用大顶堆（默认 `priority_queue`），sell 用小顶堆（`greater<>`）

2. **匹配只做一次而非循环**：
   - ✗ `if (sellHeap.top().first <= price)` 只匹配一次
   - ✓ `while (amount > 0 && !sellHeap.empty() && ...)` 循环匹配直到不能匹配

3. **部分匹配后忘记放回堆**：
   - ✗ 弹出后不管剩余量直接丢弃
   - ✓ `if (sellAmt > 0) sellHeap.push({sellPrice, sellAmt})` 剩余部分放回

4. **最后统计溢出**：
   - ✗ `int total`，累加多个 10⁹ 级数直接溢出
   - ✓ `long long total`，每步 `% MOD`

5. **匹配条件搞混**：
   - buy 来了找 sell：条件是 `sellPrice <= buyPrice`（卖价够低才能成交）
   - sell 来了找 buy：条件是 `buyPrice >= sellPrice`（买价够高才能成交）
   - 虽然逻辑等价，但弄反了 `<=` 和 `>=` 的方向会 WA

## 面试追问

**Q1: 为什么用两个堆？能不能只用一个数据结构？**
> buy 需要找最高价（大顶堆），sell 需要找最低价（小顶堆），方向相反，不能合并成一个堆。用 `std::map`（有序映射）也可以，buy 用 `rbegin()` 找最大 key，sell 用 `begin()` 找最小 key，效果等价，常数略大但代码可能更简洁。

**Q2: 如果 amount 极大（比如 10¹⁸），代码需要改什么？**
> 当前代码中 `amount` 和堆里的 amount 用 `int` 存储（最大 10⁹ 足够），但如果扩展到 10¹⁸ 需要改为 `long long`。匹配过程中的 `min(amount, sellAmt)` 不会溢出（结果 ≤ 两者较小值），主要改类型声明即可。

**Q3: 如果要支持"撤单"操作（删除某个特定价格的积压订单），数据结构怎么调整？**
> 堆不支持高效删除任意元素。可以改用 `std::map<int, long long>`（价格 → 总数量），buy 用 `rbegin()` 取最大价格，sell 用 `begin()` 取最小价格，删除任意价格是 O(log n)。或者使用"懒删除"的堆——标记已删除元素，取堆顶时跳过。

## 相关题型

- **253. 会议室 II** — 同样用堆模拟时间线上的资源分配，复用"堆维护当前状态"的思想
- **295. 数据流的中位数** — 双堆经典应用，和本题类似地维护两个方向不同的堆
- **1046. 最后一块石头的重量** — 堆模拟"每次取极值、消耗后放回"的模式，和本题的匹配逻辑类似
- **355. 设计推特** — 合并多个有序流用堆，设计题中堆的典型应用