# 1348. 推文计数 (Tweet Counts Per Frequency)

## 核心思路

**设计题：HashMap + 有序容器 + 二分查找**。用 `unordered_map<string, multiset<int>>` 存储每个推文名的所有发布时间戳。`recordTweet` 直接插入有序集合；`getTweetCountsPerFrequency` 根据频率（minute=60s, hour=3600s, day=86400s）将时间段 `[startTime, endTime]` 切分为若干 chunk，用 `lower_bound` / `upper_bound` 二分查找统计每个 chunk 中的推文数。

## 思维链

1. **读题审题**：需要实现两个操作：
   - `recordTweet(name, time)`: 记录一条推文在时间 time（秒）发布
   - `getTweetCountsPerFrequency(freq, name, start, end)`: 按频率把 `[start, end]` 切成若干时间窗口，统计每个窗口中该推文名出现的次数

2. **理解 chunk 切分规则——本题最易搞错的部分**：

```
给定 [startTime, endTime]，频率对应的秒数为 delta:

chunk 0: [startTime,                startTime + delta - 1]
chunk 1: [startTime + delta,        startTime + 2*delta - 1]
...
最后chunk: [startTime + k*delta,    endTime]  ← 可能不满 delta 秒!

具体例子: freq="minute" (delta=60), startTime=0, endTime=60:
  chunk 0: [0, 59]     ← 0 + 60 - 1 = 59
  chunk 1: [60, 60]    ← 最后一个 chunk 只有 1 秒!
  共 2 个 chunk

常见错误: 认为 [0,60] 只有 1 个 chunk [0,60]
         实际上 60 已经属于下一个 chunk!
```

3. **chunk 数量公式**：
```
delta = {"minute": 60, "hour": 3600, "day": 86400}[freq]
chunk 数量 = (endTime - startTime) / delta + 1

推导: 第一个 chunk 起点 startTime, 第二个 startTime+delta, ...
     最后一个 chunk 的起点 <= endTime
     startTime + k*delta <= endTime  →  k <= (endTime - startTime) / delta
     共 k+1 = (endTime - startTime) / delta + 1 个 chunk
```

4. **数据结构选择**：

```
方案A: unordered_map<string, multiset<int>>
  - recordTweet: O(log n) 自动排序插入
  - 每个窗口统计: lower_bound + upper_bound + distance = O(log n) ~ O(n)
  - 优点: 代码简洁, 天然有序, 允许重复
  - 缺点: multiset 的 distance 是 O(n)

方案B: unordered_map<string, vector<int>> + 查询时排序
  - recordTweet: O(1) push_back
  - getTweet: 先 sort O(n log n), 再二分 O(log n)
  - 优点: record 快; vector 上二分的 distance 是 O(1)
  - 缺点: 每次查询都排序

方案C: unordered_map<string, vector<int>> + 保持排序
  - recordTweet: O(n) 插入排序(需要移动元素)
  - 每个窗口统计: O(log n) 二分, distance O(1)
  - 优点: 查询高效
  - 缺点: 插入慢

数据量: 总操作 <= 10^4, 哪个方案都能通过
```

5. **内部数据结构图示**：
```
TweetCounts 对象:

tweets: unordered_map<string, multiset<int>>
┌─────────────┬────────────────────────┐
│   Key       │   Value (有序集合)     │
├─────────────┼────────────────────────┤
│  "tweet3"   │  {0, 10, 60, 120}     │
│  "tweet1"   │  {5, 100}             │
│  "hello"    │  {30, 30, 50}         │ ← multiset 允许重复
└─────────────┴────────────────────────┘
```

6. **完整示例演算**：

```
TweetCounts tc;

tc.recordTweet("tweet3", 0);    → tweets["tweet3"] = {0}
tc.recordTweet("tweet3", 60);   → tweets["tweet3"] = {0, 60}
tc.recordTweet("tweet3", 10);   → tweets["tweet3"] = {0, 10, 60}

tc.getTweetCountsPerFrequency("minute", "tweet3", 0, 59):
  delta = 60
  chunk 0: [0, min(0+59, 59)] = [0, 59]
    lower_bound(0) → 指向0, upper_bound(59) → 指向60
    count = 2  (0 和 10)
  0+60=60 > 59, 循环结束
  返回 [2] ✓

tc.getTweetCountsPerFrequency("minute", "tweet3", 0, 60):
  delta = 60
  chunk 0: [0, 59]
    lower_bound(0)→0, upper_bound(59)→指向60 → count=2
  chunk 1: [60, min(60+59, 60)] = [60, 60]
    lower_bound(60)→指向60, upper_bound(60)→指向end → count=1
  返回 [2, 1] ✓

tc.recordTweet("tweet3", 120);  → tweets["tweet3"] = {0, 10, 60, 120}

tc.getTweetCountsPerFrequency("hour", "tweet3", 0, 210):
  delta = 3600
  chunk 0: [0, min(0+3599, 210)] = [0, 210]
    lower_bound(0)→begin, upper_bound(210)→end → count=4  (全部)
  0+3600=3600 > 210, 循环结束
  返回 [4] ✓
```

7. **二分查找核心操作**：

```
sorted times = [0, 10, 60, 120]

统计 [0, 59] 内的元素数:
  lower_bound(0)  → index 0 (第一个 >= 0)
  upper_bound(59) → index 2 (第一个 > 59, 即 60 的位置)
  count = 2 - 0 = 2 ✓

统计 [60, 60] 内的元素数:
  lower_bound(60)  → index 2
  upper_bound(60)  → index 3 (第一个 > 60, 即 120 的位置)
  count = 3 - 2 = 1 ✓
```

## 解法概览

| 解法 | 数据结构 | recordTweet | getTweet | 推荐 |
|------|----------|-------------|----------|------|
| multiset + 二分 | `map<str, multiset<int>>` | O(log n) | O(chunks * log n) | 面试首选 |
| vector + 查询排序 | `map<str, vector<int>>` | O(1) | O(n log n) | 简单实现 |
| vector + 保持排序 | `map<str, vector<int>>` | O(n) 插入 | O(chunks * log n) | 查询多时 |

## 关键提示

- **提示1**: 频率映射：`"minute"→60, "hour"→3600, "day"→86400`。用 if-else 或 map。
- **提示2**: 最后一个窗口可能不满——用 `min(chunkStart + delta - 1, endTime)` 截断右边界。
  ```
  错误: 所有 chunk 都是 delta 大小
  正确: 最后一个 chunk 的右边界是 endTime, 可能 < chunkStart + delta - 1
  ```
- **提示3**: 整数溢出陷阱！`chunkStart + delta - 1` 当 chunkStart 接近 10^9 时可能超过 INT_MAX (2.1*10^9)。用 `long long` 做运算。
- **提示4**: `endTime - startTime <= 10^4`，所以 minute 频率最多 ceil(10001/60)=167 个 chunk。不会产生大量窗口。
- **提示5**: multiset 的 `distance(lower, upper)` 是 O(n)（双向迭代器），vector 的是 O(1)（随机访问迭代器）。对于本题数据量，两者都足够快。

## 解法详解

### 解法1: HashMap + multiset（推荐）

**数据结构**：`unordered_map<string, multiset<int>>`

- `multiset` 特性：自动排序（红黑树）、允许重复值、`lower_bound/upper_bound` O(log n)、`insert` O(log n)。

**recordTweet**: `tweets[name].insert(time)` — 一行搞定。

**getTweetCountsPerFrequency**:
1. 根据 freq 确定 delta
2. for 循环：`chunkStart` 从 `startTime` 开始，每次加 delta
3. 计算 `chunkEnd = min(chunkStart + delta - 1, endTime)`
4. 用 `lower_bound(chunkStart)` 和 `upper_bound(chunkEnd)` 统计

### 解法2: HashMap + vector + 查询时排序

**trade-off**: `recordTweet` 只是 `push_back` O(1)，但每次 `getTweet` 前要排序。

**优化**: 加 `dirty` 标志位，数据变脏时才重新排序。如果多次查询之间没有新的 record，可以复用排序结果。

### 解法3: HashMap + vector + 二分插入

每次 `recordTweet` 时用 `lower_bound` 找到插入位置，`insert` 保持数组有序。插入操作 O(n)（移动元素），但查询不需要排序。

## 完整代码

```cpp
class TweetCounts {
    unordered_map<string, multiset<int>> tweets;
public:
    TweetCounts() {}

    void recordTweet(string tweetName, int time) {
        tweets[tweetName].insert(time);
    }

    vector<int> getTweetCountsPerFrequency(string freq, string tweetName,
                                            int startTime, int endTime) {
        int delta;
        if (freq == "minute") delta = 60;
        else if (freq == "hour") delta = 3600;
        else delta = 86400;  // "day"

        vector<int> result;
        auto& times = tweets[tweetName];

        // 用 long long 避免 t + delta - 1 溢出
        for (long long t = startTime; t <= endTime; t += delta) {
            long long windowEnd = min(t + delta - 1, (long long)endTime);
            auto lo = times.lower_bound((int)t);
            auto hi = times.upper_bound((int)windowEnd);
            result.push_back((int)distance(lo, hi));
        }
        return result;
    }
};
```

```python
from collections import defaultdict
from bisect import insort, bisect_left, bisect_right

class TweetCounts:
    def __init__(self):
        self.tweets = defaultdict(list)

    def recordTweet(self, tweetName: str, time: int) -> None:
        insort(self.tweets[tweetName], time)

    def getTweetCountsPerFrequency(self, freq: str, tweetName: str,
                                    startTime: int, endTime: int) -> List[int]:
        delta = {"minute": 60, "hour": 3600, "day": 86400}[freq]
        times = self.tweets[tweetName]
        result = []
        t = startTime
        while t <= endTime:
            windowEnd = min(t + delta - 1, endTime)
            lo = bisect_left(times, t)
            hi = bisect_right(times, windowEnd)
            result.append(hi - lo)
            t += delta
        return result
```

## 面试追问

1. **"multiset 的 distance 是 O(n) 怎么办？"** → 改用 `vector<int>` + 保持排序。vector 的随机访问迭代器做减法是 O(1)。
2. **"如果 record 远多于 query？"** → 用 vector + 查询时排序（record 是 O(1)）。
3. **"如何支持删除推文？"** → multiset 直接 `erase(iterator)`。vector 需要 `find` + `erase`。
4. **"时间戳可以重复吗？"** → 可以！multiset 和 vector 都天然支持重复值。`lower_bound + upper_bound` 正确统计重复值。
5. **"endTime - startTime <= 10^4 有什么意义？"** → 限制了窗口数量上界。minute 最多 ~167 个窗口，整体查询效率可控。

## 相关题目

| 题目 | 关联 |
|------|------|
| 362. Design Hit Counter | 类似的时间窗口计数设计 |
| 981. Time Based Key-Value Store | HashMap + 二分查找时间戳 |
| 729. My Calendar I | 有序区间 + 二分查找 |
| 1396. Design Underground System | 设计题 + 时间统计 |

## 复杂度分析

- **recordTweet**: O(log n)（multiset 插入）或 O(1)（vector 追加）
- **getTweetCountsPerFrequency**: O(chunks * log n)，chunks = (endTime - startTime) / delta + 1 <= 168
- **空间**: O(N)，N 为总推文数
