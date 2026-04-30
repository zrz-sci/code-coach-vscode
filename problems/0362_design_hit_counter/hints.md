# 362. 敲击计数器

## 核心思路
本质上是一个**滑动窗口计数问题**：维护一个时间窗口为 300 秒的数据结构，支持在窗口内添加元素和统计窗口内的元素总数。

## 思维链

1. **读完题第一反应**：每次 hit 记录一个时间戳，getHits 时数一数过去 300 秒内有多少条记录。最朴素的做法——用一个列表存所有时间戳，查询时遍历整个列表，跳过超时的。

2. **暴力的瓶颈在哪？**：列表会无限增长。如果系统运行很久，即使过期的时间戳也还在内存中，getHits 遍历时要跳过大量过期数据。

3. **怎么突破？——队列淘汰过期数据**：时间戳是单调递增的，过期的一定在队列头部。用队列存时间戳，每次查询前把队头过期的弹出。这样队列最多存 300 秒内的记录。

4. **进阶追问：如果每秒敲击次数极大怎么办？**：队列中会有大量相同时间戳。可以用固定长度为 300 的循环数组，每个槽位对应一秒，存 `(时间戳, 计数)` 对。用 `timestamp % 300` 定位槽位，如果时间戳变了就重置。

5. **还有什么思路？**：也可以用有序列表 + 二分查找，getHits 时二分找到 `timestamp - 299` 的位置，用尾部减去该位置就是计数。

## 解法概览
| 解法 | 思路 | hit() | getHits() | 空间 | 面试 |
|------|------|-------|-----------|------|------|
| 队列 | 存所有时间戳，弹出过期的 | O(1) | 均摊 O(1)，最坏 O(n) | O(n) | ⭐ 必须写出 |
| 循环数组 | 固定 300 长度，按秒聚合 | O(1) | O(300) = O(1) | O(300) = O(1) | ⭐ 进阶必须写出 |
| 有序列表 + 二分 | 存所有时间戳，二分查窗口起点 | O(1) | O(log n) | O(n) | 能说出即可 |

## 关键提示

1. **时间戳单调递增**是关键条件——这意味着过期数据一定在"前面"，可以用队列 FIFO 的特性淘汰。

2. **"过去 300 秒"的边界**：timestamp=300 时，时间戳 1 是否应该被计入？条件是 `timestamp - hit_time < 300`，即 `hit_time > timestamp - 300`。时间戳 1 刚好在 (300-300, 300] = (0, 300] 内，所以计入。但 timestamp=301 时，时间戳 1 就在 (1, 301] 内的边界外（1 不 > 1），不计入。

3. **循环数组的核心**：`timestamp % 300` 会产生碰撞（比如 timestamp=1 和 timestamp=301 都映射到槽 1）。所以每个槽需要额外记录"这个槽属于哪个时间戳"，如果不匹配就重置。

4. **进阶思考**：如果每秒有百万次 hit，队列方案要存百万个元素；循环数组方案只需要累加计数，空间恒定 O(300)。

5. 示意图——循环数组的工作方式：
```
时间轴:    1   2   3   ...  300  301
槽位索引:  1   2   3   ...  0    1
           ↑                      ↑
           这两个映射到同一个槽位！
           
timestamp=1  → slots[1] = {time:1, count:1}
timestamp=301 → slots[1]: time=1 ≠ 301, 重置为 {time:301, count:1}
```

## 解法详解

### 解法1: 队列 — hit O(1) / getHits 均摊 O(1) ⭐ 面试首选

**思考过程**：hit 产生的时间戳是单调递增的，所以队列中的时间戳天然有序。查询时只需要把队头过期的弹出，剩下的就是有效窗口内的。队列的大小就是答案。

```cpp
class HitCounter {
    queue<int> q;
public:
    HitCounter() {}
    
    void hit(int timestamp) {
        q.push(timestamp);
    }
    
    int getHits(int timestamp) {
        // 弹出所有过期的时间戳（距今超过300秒）
        while (!q.empty() && q.front() <= timestamp - 300) {
            q.pop();
        }
        return q.size();
    }
};
```

**关键点**：
- 判断条件是 `q.front() <= timestamp - 300`，不是 `< 300`。比如 timestamp=300 时，时间戳 1 满足 `1 > 300-300=0`，所以保留。
- 如果每秒大量 hit，队列会非常长，这是此解法的局限。

### 解法2: 循环数组（固定空间）— hit O(1) / getHits O(300) ⭐ 进阶首选

**从解法1优化**：解法1的瓶颈是同一秒内多次 hit 会产生大量重复时间戳。循环数组把同一秒的 hit 聚合成一个计数，空间固定为 300。

```cpp
class HitCounter {
    // times[i]: 槽位 i 上次被写入的时间戳
    // hits[i]:  槽位 i 的累计计数
    vector<int> times, hits;
public:
    HitCounter() : times(300, 0), hits(300, 0) {}
    
    void hit(int timestamp) {
        int idx = timestamp % 300;
        if (times[idx] != timestamp) {
            // 这个槽位是旧数据，重置
            times[idx] = timestamp;
            hits[idx] = 1;
        } else {
            // 同一秒内再次 hit，累加
            hits[idx]++;
        }
    }
    
    int getHits(int timestamp) {
        int total = 0;
        for (int i = 0; i < 300; i++) {
            // 只统计 300 秒窗口内的有效槽位
            if (timestamp - times[i] < 300) {
                total += hits[i];
            }
        }
        return total;
    }
};
```

**关键点**：
- `times` 数组解决了循环碰撞问题：通过记录时间戳来判断这个槽位的数据是否过期。
- getHits 恒定遍历 300 个槽位，不受 hit 总量影响。

### 解法3: 有序列表 + 二分查找 — hit O(1) / getHits O(log n)

**思路**：存所有时间戳到有序列表中（因为时间戳单调递增，直接 push_back 就是有序的）。getHits 时，用二分查找找到 `timestamp - 299` 的第一个位置，从该位置到末尾的长度就是窗口内的 hit 数。

```cpp
class HitCounter {
    vector<int> timestamps;
public:
    HitCounter() {}
    
    void hit(int timestamp) {
        timestamps.push_back(timestamp);
    }
    
    int getHits(int timestamp) {
        // 找到第一个 >= timestamp - 299 的位置
        int target = timestamp - 299;
        auto it = lower_bound(timestamps.begin(), timestamps.end(), target);
        // 从 it 到 end 的元素都在窗口内
        return timestamps.end() - it;
    }
};
```

**关键点**：
- 不清理过期数据，所以空间会持续增长。适合调用次数不多的场景。
- 二分查找的目标是 `timestamp - 299`，因为窗口是 `[timestamp-299, timestamp]`，共 300 秒。

## 解法对比

| | 队列 | 循环数组 | 二分查找 |
|---|---|---|---|
| **hit()** | O(1) | O(1) | O(1) |
| **getHits()** | 均摊 O(1)，最坏 O(n) | O(300) | O(log n) |
| **空间** | O(n)，n 为窗口内 hit 数 | O(300) 固定 | O(N)，N 为总 hit 数 |
| **大量同秒 hit** | 空间爆炸 | 完美应对 | 空间爆炸 |
| **适用场景** | 一般面试首选 | 高并发/Follow-up | 需要快速区间查询 |

**面试推荐**：先写队列解法展示基本理解，被追问 Follow-up（大量并发）时切换到循环数组。

## 易错点

1. **窗口边界判断错误**
   - ✗ `q.front() < timestamp - 300` → timestamp=300 时会保留时间戳 0（如果存在），多算
   - ✓ `q.front() <= timestamp - 300` → 正确排除 300 秒之前的

2. **循环数组判断过期条件错误**
   - ✗ `timestamp - times[i] <= 300` → 会把刚好 300 秒前的也算进来
   - ✓ `timestamp - times[i] < 300` → 窗口是 (timestamp-300, timestamp]

3. **二分查找目标写错**
   - ✗ `lower_bound(..., timestamp - 300)` → 窗口变成 301 秒
   - ✓ `lower_bound(..., timestamp - 299)` → 窗口刚好 300 秒（从 timestamp-299 到 timestamp）

4. **循环数组忘记重置计数**
   - ✗ 当 `times[idx] != timestamp` 时只更新 `times[idx]`，忘记把 `hits[idx]` 重置为 1
   - ✓ 必须同时重置 `hits[idx] = 1`

## 面试追问

**Q1（基础理解）**：最简单的实现方式是什么？时间空间复杂度？
> 用队列存时间戳，hit O(1)，getHits 弹出过期的后返回 size，均摊 O(1)。空间 O(n)。

**Q2（Follow-up）**：如果每秒有百万次 hit，你的方案能应对吗？
> 不行，队列会存百万个相同时间戳。改用循环数组，长度固定 300，每个槽位存 (时间戳, 计数)。空间 O(300)，不受 hit 频率影响。

**Q3（并发追问）**：如果是多线程环境呢？
> 循环数组方案中，每个槽位的读写需要加锁或使用原子操作。可以对 300 个槽位分别加细粒度锁（分段锁），减少竞争。队列方案需要全局锁，并发性更差。

**Q4（变体）**：如果要支持任意时间窗口（不只是 300 秒）怎么办？
> 循环数组大小改为窗口大小 W，但 W 很大时不合适。此时可回到队列方案，或使用有序列表 + 二分查找。

## 相关题型
- **933. 最近的请求次数 (Number of Recent Calls)** — 几乎同一道题，窗口从 300 秒变为 3000 毫秒，可以直接复用队列方案。
- **346. 数据流中的移动平均值 (Moving Average from Data Stream)** — 同样是固定窗口内维护统计量，复用循环数组/队列思路，区别是需要维护窗口内的和而不是计数。
- **1429. 第一个唯一数字 (First Unique Number)** — 数据流设计题，复用"队列 + 辅助数据结构"的设计模式。