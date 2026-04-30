/*
 * LeetCode 362: 敲击计数器 (Design Hit Counter)
 *
 * 【题目本质】
 * 设计一个数据结构，支持在时间流中记录事件(hit)和查询过去 300 秒内的事件总数(getHits)。
 * 本质是一个固定大小滑动窗口的计数问题。
 *
 * 【解法总览】
 * 解法1: 队列        — hit O(1) / getHits 均摊 O(1) / 空间 O(n)   — 最直觉
 * 解法2: 循环数组     — hit O(1) / getHits O(300)    / 空间 O(300) — Follow-up 最优
 * 解法3: 二分查找     — hit O(1) / getHits O(log n)  / 空间 O(N)   — 另一种思路
 *
 * 【示例演示】
 *   操作序列:  hit(1), hit(2), hit(3), getHits(4), hit(300), getHits(300), getHits(301)
 *
 *   队列状态变化:
 *   hit(1):      [1]
 *   hit(2):      [1, 2]
 *   hit(3):      [1, 2, 3]
 *   getHits(4):  窗口(4-300, 4] = (-296, 4], 全部有效 → 返回 3
 *                [1, 2, 3]
 *   hit(300):    [1, 2, 3, 300]
 *   getHits(300):窗口(0, 300], 全部有效 → 返回 4
 *                [1, 2, 3, 300]
 *   getHits(301):窗口(1, 301], 时间戳 1 不满足 1 > 1，弹出
 *                [2, 3, 300] → 返回 3
 */

// ============================================================
// 解法1: 队列 — 存所有时间戳，淘汰过期的
// hit: O(1)  getHits: 均摊 O(1)，最坏 O(n)  空间: O(n)
//
// 【思路】
// 时间戳单调递增 → 队列中的元素天然有序。
// 过期的时间戳一定在队头 → 每次查询前把队头过期的弹出。
// 队列剩余的大小就是答案。
//
// 为什么"均摊 O(1)"？
// 每个时间戳最多入队一次、出队一次，所以 n 次操作总共最多 2n 次。
// 但单次 getHits 最坏可能弹出所有元素（比如很久没查过突然查一次）。
// ============================================================
class HitCounter1 {
    queue<int> q;
public:
    HitCounter1() {}
    
    void hit(int timestamp) {
        q.push(timestamp);
    }
    
    int getHits(int timestamp) {
        // 弹出过期的时间戳
        // 窗口: (timestamp - 300, timestamp]
        // 过期条件: front <= timestamp - 300 （即 front 在窗口左边界或更左）
        while (!q.empty() && q.front() <= timestamp - 300) {
            q.pop();
        }
        return q.size();
    }
};

// ============================================================
// 解法2: 循环数组 — 固定 300 空间，按秒聚合计数
// hit: O(1)  getHits: O(300)  空间: O(300) = O(1)
//
// 【思路】
// 解法1的瓶颈：如果同一秒有百万次 hit，队列存百万个相同时间戳，浪费空间。
// 
// 观察：窗口最多覆盖 300 个不同的秒。
// 用长度 300 的数组，timestamp % 300 定位槽位。
// 每个槽位记录 (时间戳, 计数)。
//
// 关键问题：不同时间戳可能映射到同一个槽位（如 1 和 301 都 % 300 = 1）。
// 解决方案：额外存时间戳，如果不匹配说明是旧数据，直接重置。
//
// 循环数组示意：
//   index:  0    1    2    3    ... 299
//   times:  300  1    2    3    ... 299
//   hits:   1    1    1    1    ... 0
//
//   hit(301) → idx = 301 % 300 = 1
//     times[1] = 1 ≠ 301 → 重置: times[1]=301, hits[1]=1
//   
//   getHits(301) → 遍历 0..299，只累加 301 - times[i] < 300 的槽位
//     times[1]=301: 301-301=0 < 300 ✓  累加 hits[1]=1
//     times[2]=2:   301-2=299 < 300 ✓  累加 hits[2]=1
//     times[3]=3:   301-3=298 < 300 ✓  累加 hits[3]=1
//     times[0]=300: 301-300=1 < 300 ✓  累加 hits[0]=1
//     总计 = 4? 不对，应该是 3。
//     哦等等，times[1] 被重置了，原来的 hit(1) 被覆盖了。
//     所以 idx=1 只有 hits=1（来自 hit(301)），没有 hit(1) 了。
//     但上面 getHits(301) 应返回 3 (hit(2), hit(3), hit(300))
//     实际：hit(2)→idx=2, hit(3)→idx=3, hit(300)→idx=0
//     以及 hit(301) 如果有的话... 但示例中没有 hit(301)
//     
// 重新按示例走一遍：
//   hit(1):   idx=1, times[1]=1, hits[1]=1
//   hit(2):   idx=2, times[2]=2, hits[2]=1
//   hit(3):   idx=3, times[3]=3, hits[3]=1
//   getHits(4): 遍历300个槽, times[1]=1: 4-1=3<300 ✓ +1
//               times[2]=2: 4-2=2<300 ✓ +1, times[3]=3: 4-3=1<300 ✓ +1
//               其余 times[i]=0: 4-0=4<300 ✓ 但 hits[i]=0, 加0
//               → 返回 3 ✓
//   hit(300): idx=0, times[0]=300, hits[0]=1
//   getHits(300): times[1]=1: 300-1=299<300 ✓ +1
//                 times[2]=2: 300-2=298<300 ✓ +1
//                 times[3]=3: 300-3=297<300 ✓ +1
//                 times[0]=300: 300-300=0<300 ✓ +1
//                 → 返回 4 ✓
//   getHits(301): times[1]=1: 301-1=300, 300<300? NO ✗
//                 times[2]=2: 301-2=299<300 ✓ +1
//                 times[3]=3: 301-3=298<300 ✓ +1
//                 times[0]=300: 301-300=1<300 ✓ +1
//                 → 返回 3 ✓
// ============================================================
class HitCounter2 {
    vector<int> times;  // times[i]: 槽位 i 最近一次被写入的时间戳
    vector<int> hits;   // hits[i]:  槽位 i 的有效 hit 计数
public:
    HitCounter2() : times(300, 0), hits(300, 0) {}
    
    void hit(int timestamp) {
        int idx = timestamp % 300;
        if (times[idx] != timestamp) {
            // 槽位存的是旧数据（不同的时间戳），重置
            // 为什么直接重置而不是累加？因为旧数据已经过期或被新时间戳覆盖
            times[idx] = timestamp;
            hits[idx] = 1;
        } else {
            // 同一秒内多次 hit，累加
            hits[idx]++;
        }
    }
    
    int getHits(int timestamp) {
        int total = 0;
        for (int i = 0; i < 300; i++) {
            // 判断槽位数据是否在窗口内
            // 窗口: (timestamp - 300, timestamp]
            // 等价于: timestamp - times[i] < 300
            if (timestamp - times[i] < 300) {
                total += hits[i];
            }
        }
        return total;
    }
};

// ============================================================
// 解法3: 有序列表 + 二分查找
// hit: O(1)  getHits: O(log n)  空间: O(N) N为总hit数
//
// 【思路】
// 时间戳单调递增 → push_back 后列表天然有序。
// getHits 需要统计 [timestamp-299, timestamp] 范围内的数量。
// 用 lower_bound 找到第一个 >= timestamp-299 的位置，
// 从该位置到 end 的距离就是答案。
//
// 为什么是 timestamp-299 而不是 timestamp-300？
// 窗口 = 过去 300 秒 = [timestamp-299, timestamp]（包含两端，共 300 秒）
// 等价于：hit_time > timestamp - 300
// lower_bound 找 >= timestamp-299，因为 > timestamp-300 等价于 >= timestamp-299
// （时间戳是整数）
//
// 缺点：不清理过期数据，空间持续增长。
// 
// 二分搜索过程示例（timestamps = [1, 2, 3, 300], query getHits(301)）:
//   target = 301 - 299 = 2
//   lower_bound 找第一个 >= 2 的位置:
//
//   lo=0                    hi=3
//   [1,     2,     3,     300]
//   Round1: mid=(0+3)/2=1, val=2 >= 2 → hi=mid=1
//   lo=0   hi=1
//   Round2: mid=(0+1)/2=0, val=1 < 2 → lo=mid+1=1
//   lo==hi=1 → 找到位置 1 (值为2)
//   答案 = size - pos = 4 - 1 = 3 ✓ (窗口内: [2, 3, 300])
//
// 二分搜索过程示例2（timestamps = [1, 1, 2, 3, 300], query getHits(300)）:
//   target = 300 - 299 = 1
//   lower_bound 找第一个 >= 1 的位置:
//
//   lo=0                         hi=4
//   [1,     1,     2,     3,     300]
//   Round1: mid=2, val=2 >= 1 → hi=2
//   lo=0          hi=2
//   [1,     1,     2]
//   Round2: mid=1, val=1 >= 1 → hi=1
//   lo=0   hi=1
//   Round3: mid=0, val=1 >= 1 → hi=0
//   lo==hi=0 → 找到位置 0 (值为1)
//   答案 = 5 - 0 = 5 ✓ (全部在窗口内)
//
// 二分搜索过程示例3（timestamps = [1, 2, 3, 300], query getHits(303)）:
//   target = 303 - 299 = 4
//   lower_bound 找第一个 >= 4 的位置:
//
//   lo=0                    hi=3
//   [1,     2,     3,     300]
//   Round1: mid=1, val=2 < 4 → lo=2
//   lo=2          hi=3
//   Round2: mid=2, val=3 < 4 → lo=3
//   lo==hi=3 → 找到位置 3 (值为300)
//   答案 = 4 - 3 = 1 ✓ (窗口内只剩 [300])
// ============================================================
class HitCounter3 {
    vector<int> timestamps;
public:
    HitCounter3() {}
    
    void hit(int timestamp) {
        timestamps.push_back(timestamp);
    }
    
    int getHits(int timestamp) {
        // 窗口左边界（含）
        int target = timestamp - 299;
        // 找第一个 >= target 的位置
        auto it = lower_bound(timestamps.begin(), timestamps.end(), target);
        return timestamps.end() - it;
    }
};

// ============================================================
// 【解法对比】
//
// |          | 队列(解法1)    | 循环数组(解法2)    | 二分查找(解法3)  |
// |----------|---------------|-------------------|-----------------|
// | hit()    | O(1)          | O(1)              | O(1) 均摊       |
// | getHits()| 均摊O(1)      | O(300)            | O(log n)        |
// | 空间     | O(窗口内hit数) | O(300) 固定        | O(总hit数)      |
// | 高并发hit | ✗ 空间爆炸    | ✓ 完美应对         | ✗ 空间爆炸      |
// | 实现难度  | 简单          | 中等               | 简单            |
//
// 面试策略:
// - 先写解法1（队列），展示基本设计能力
// - 被问 Follow-up 时切换到解法2（循环数组），展示工程思维
// - 解法3 作为补充，展示二分查找的灵活运用
//
// 【易错点】
//
// 1. 窗口边界: 
//    ✗ q.front() < timestamp - 300   → 会保留刚好 300 秒前的（多算）
//    ✓ q.front() <= timestamp - 300  → 正确排除
//    原因: 窗口是 (timestamp-300, timestamp]，左边界不含
//
// 2. 循环数组过期判断:
//    ✗ timestamp - times[i] <= 300   → 刚好 300 秒前的被算进来
//    ✓ timestamp - times[i] < 300    → 严格小于 300
//
// 3. 循环数组忘记重置 hits:
//    ✗ if (times[idx] != timestamp) { times[idx] = timestamp; hits[idx]++; }
//      这样旧的计数会被保留并继续累加
//    ✓ if (times[idx] != timestamp) { times[idx] = timestamp; hits[idx] = 1; }
//      必须重置为 1
//
// 4. 二分查找目标:
//    ✗ lower_bound(..., timestamp - 300) → 窗口变成 301 秒
//    ✓ lower_bound(..., timestamp - 299) → 窗口刚好 300 秒
//    因为整数域中 > x 等价于 >= x+1
//
// 5. 初始化 times 数组为 0:
//    这不会导致 bug 因为 getHits 时 timestamp >= 1，
//    而 timestamp - 0 = timestamp >= 1 > 0，
//    只有当 timestamp < 300 时 0 才"看起来有效"，
//    但 hits[i] 初始化也为 0，所以加了 0 不影响结果。
//
// 【面试追问 — 递进链】
//
// Q1（基础）: 最简单的实现？复杂度？
//   → 队列存时间戳，hit O(1)，getHits 弹出过期后返回 size，均摊 O(1)。
//
// Q2（Follow-up）: 每秒百万次 hit，你的方案扛得住吗？
//   → 队列会存百万个重复时间戳。改用循环数组，按秒聚合，空间固定 O(300)。
//
// Q3（多线程）: 并发环境怎么处理？
//   → 循环数组每个槽位独立，可以用分段锁（每个槽位一把锁），降低竞争。
//     队列方案需要全局锁，并发性差。
//     也可以用 atomic 操作实现无锁版本（CAS 更新计数）。
//
// Q4（变体）: 如果窗口大小不固定，用户可以查询任意时间范围内的 hit 数？
//   → 循环数组不再适用（数组大小不确定）。
//     可以用有序列表 + 二分查找，getHits(from, to) 做两次二分。
//     或用分桶策略（按分钟/小时聚合）+ 精细粒度数据的组合。
// ============================================================
