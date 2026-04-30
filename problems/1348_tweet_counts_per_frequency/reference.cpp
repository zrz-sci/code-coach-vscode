/*
 * LeetCode 1348: Tweet Counts Per Frequency (推文计数)
 *
 * 设计题: 实现推文时间记录系统
 *   - recordTweet(name, time): 记录推文发布时间
 *   - getTweetCountsPerFrequency(freq, name, start, end): 按频率分窗口统计
 *
 * 数据结构:
 *   tweets: unordered_map<string, multiset<int>>
 *   ┌─────────────┬────────────────────────────┐
 *   │   Key       │   Value (有序多重集合)     │
 *   ├─────────────┼────────────────────────────┤
 *   │  "tweet3"   │  {0, 10, 60, 120}          │
 *   │  "tweet1"   │  {5, 100}                  │
 *   └─────────────┴────────────────────────────┘
 *
 * 窗口切分 (freq="minute", start=0, end=60):
 *   ┌──────────────┐┌──────────────┐
 *   │  [0, 59]     ││  [60, 60]    │
 *   │  *0  *10     ││  *60         │
 *   │  count=2     ││  count=1     │
 *   └──────────────┘└──────────────┘
 *   结果: [2, 1]
 *
 * 三种解法:
 *   解法1: HashMap + multiset (推荐, 代码简洁, 天然有序)
 *   解法2: HashMap + vector + 查询时排序 (record 快, query 慢)
 *   解法3: HashMap + vector + 二分插入 (record 慢, query 快)
 */



// ===================== 解法1: HashMap + multiset (推荐) =====================
//
// multiset 特性:
//   - 红黑树实现, 自动排序
//   - 允许重复值 (同一时刻可发多条推文)
//   - insert: O(log n)
//   - lower_bound / upper_bound: O(log n)
//
// recordTweet:  O(log n)
// getTweetCountsPerFrequency: O(chunks * log n)
// 其中 chunks <= (endTime - startTime) / 60 + 1 <= 168

class TweetCounts {
    unordered_map<string, multiset<int>> tweets;

public:
    TweetCounts() {}

    // 记录推文: 直接插入有序集合
    void recordTweet(string tweetName, int time) {
        tweets[tweetName].insert(time);
    }

    // 按频率统计推文数
    vector<int> getTweetCountsPerFrequency(string freq, string tweetName,
                                            int startTime, int endTime) {
        // Step 1: freq → delta (秒)
        int delta;
        if (freq == "minute") delta = 60;
        else if (freq == "hour") delta = 3600;
        else delta = 86400;  // "day"

        vector<int> result;
        auto& times = tweets[tweetName];

        // Step 2: 按 chunk 遍历 [startTime, endTime]
        // 【注意】用 long long 避免 t + delta - 1 溢出!
        // 当 t ≈ 10^9, delta = 86400 时, t + delta - 1 可能超 INT_MAX
        for (long long t = startTime; t <= endTime; t += delta) {
            // 当前窗口: [t, min(t + delta - 1, endTime)]
            // 最后一个窗口可能不满 delta 秒
            long long windowEnd = min(t + delta - 1, (long long)endTime);

            // Step 3: 二分查找统计窗口内推文数
            // lower_bound(t): 第一个 >= t 的位置
            // upper_bound(windowEnd): 第一个 > windowEnd 的位置
            // 差值 = [t, windowEnd] 内的元素数
            auto lo = times.lower_bound((int)t);
            auto hi = times.upper_bound((int)windowEnd);
            int count = (int)distance(lo, hi);

            result.push_back(count);
        }

        return result;
    }
};


// ===================== 解法2: HashMap + vector + 查询时排序 =====================
//
// recordTweet: O(1) — push_back
// getTweet: O(n log n + chunks * log n) — 排序后二分
//
// 适用: recordTweet 频率 >> getTweet 频率
// 优化: dirty 标志, 只在数据变脏时重新排序

class TweetCounts_SortOnQuery {
    unordered_map<string, vector<int>> tweets;
    unordered_map<string, bool> dirty;

public:
    TweetCounts_SortOnQuery() {}

    void recordTweet(string tweetName, int time) {
        tweets[tweetName].push_back(time);
        dirty[tweetName] = true;
    }

    vector<int> getTweetCountsPerFrequency(string freq, string tweetName,
                                            int startTime, int endTime) {
        int delta = (freq == "minute") ? 60 : (freq == "hour") ? 3600 : 86400;

        auto& times = tweets[tweetName];
        // 只在数据变脏时重新排序
        if (dirty[tweetName]) {
            sort(times.begin(), times.end());
            dirty[tweetName] = false;
        }

        vector<int> result;
        for (long long t = startTime; t <= endTime; t += delta) {
            long long windowEnd = min(t + delta - 1, (long long)endTime);
            // vector 上的二分: 随机访问迭代器, 减法 O(1)
            int lo = (int)(lower_bound(times.begin(), times.end(), (int)t) - times.begin());
            int hi = (int)(upper_bound(times.begin(), times.end(), (int)windowEnd) - times.begin());
            result.push_back(hi - lo);
        }
        return result;
    }
};


// ===================== 解法3: HashMap + vector + 二分插入 =====================
//
// recordTweet: O(n) — lower_bound 找位置 + insert 移动元素
// getTweet: O(chunks * log n) — 数组始终有序, 二分 O(log n)
//
// 适用: getTweet 频率 >> recordTweet 频率

class TweetCounts_SortedInsert {
    unordered_map<string, vector<int>> tweets;

public:
    TweetCounts_SortedInsert() {}

    void recordTweet(string tweetName, int time) {
        auto& times = tweets[tweetName];
        auto pos = lower_bound(times.begin(), times.end(), time);
        times.insert(pos, time);  // O(n): 需要移动后续元素
    }

    vector<int> getTweetCountsPerFrequency(string freq, string tweetName,
                                            int startTime, int endTime) {
        int delta = (freq == "minute") ? 60 : (freq == "hour") ? 3600 : 86400;
        auto& times = tweets[tweetName];

        vector<int> result;
        for (long long t = startTime; t <= endTime; t += delta) {
            long long windowEnd = min(t + delta - 1, (long long)endTime);
            int lo = (int)(lower_bound(times.begin(), times.end(), (int)t) - times.begin());
            int hi = (int)(upper_bound(times.begin(), times.end(), (int)windowEnd) - times.begin());
            result.push_back(hi - lo);
        }
        return result;
    }
};


/*
 * ==================== 手动演算 ====================
 *
 * TweetCounts tc;
 *
 * tc.recordTweet("tweet3", 0);   → tweets["tweet3"] = {0}
 * tc.recordTweet("tweet3", 60);  → tweets["tweet3"] = {0, 60}
 * tc.recordTweet("tweet3", 10);  → tweets["tweet3"] = {0, 10, 60}
 *
 * tc.getTweetCountsPerFrequency("minute", "tweet3", 0, 59):
 *   delta=60
 *   t=0: window=[0,59]
 *     lower_bound(0)→0, upper_bound(59)→指向60
 *     count=2  (元素 0 和 10)
 *   t=60: 60>59, 结束
 *   返回 [2] ✓
 *
 * tc.getTweetCountsPerFrequency("minute", "tweet3", 0, 60):
 *   delta=60
 *   t=0: window=[0,59]  → count=2  (0, 10)
 *   t=60: window=[60,60] → count=1  (60)
 *   t=120: 120>60, 结束
 *   返回 [2, 1] ✓
 *
 * tc.recordTweet("tweet3", 120); → tweets["tweet3"] = {0, 10, 60, 120}
 *
 * tc.getTweetCountsPerFrequency("hour", "tweet3", 0, 210):
 *   delta=3600
 *   t=0: window=[0, min(3599, 210)]=[0,210]
 *     count=4  (0, 10, 60, 120 全部在 [0,210] 内)
 *   t=3600: 3600>210, 结束
 *   返回 [4] ✓
 *
 *
 * ==================== 易错点汇总 ====================
 *
 * 1. 最后一个窗口截断:
 *    windowEnd = min(t + delta - 1, endTime)
 *    不能总用 t + delta - 1, 否则最后窗口超出 endTime
 *
 * 2. 整数溢出:
 *    t + delta - 1 当 t ≈ 10^9, delta = 86400 时超 INT_MAX
 *    解决: 循环变量 t 用 long long
 *
 * 3. multiset 的 distance 是 O(n):
 *    双向迭代器的 distance 需要逐个移动
 *    改用 vector + 排序, 则 distance 是减法 O(1)
 *
 * 4. 空推文名:
 *    若从未 recordTweet, tweets[name] 是空集
 *    所有窗口 count=0, 不会出错
 *
 * 5. 同一时间多条推文:
 *    multiset 允许重复, vector 天然允许
 *    lower_bound + upper_bound 正确处理重复值
 *
 *
 * ==================== 面试追问 ====================
 *
 * Q1: multiset 的 distance 是 O(n) 怎么办？
 *     → 改用 vector<int> + 保持排序，随机访问迭代器减法 O(1)
 * Q2: 如果 record 远多于 query？
 *     → 用 vector + 查询时排序（record O(1)）
 * Q3: 如何支持删除推文？
 *     → multiset 直接 erase(iterator)；vector 需 find + erase
 * Q4: 时间戳可以重复吗？
 *     → 可以! multiset 和 vector 都天然支持重复值
 */
