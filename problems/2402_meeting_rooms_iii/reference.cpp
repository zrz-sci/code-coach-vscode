/*
 * LeetCode 2402: 会议室 III (Meeting Rooms III)
 * 
 * 【题目本质】
 * 事件驱动模拟：按会议开始时间顺序处理，每次分配编号最小的空闲房间，
 * 无空闲则延期到最早结束的房间。统计每个房间使用次数，返回最大次数的最小编号。
 *
 * 【解法总览】
 * 解法1: 暴力模拟 — O(m·n) / O(n) — 直觉做法，n≤100 可以过
 * 解法2: 双堆模拟 — O(m·log n) / O(n) — 面试期望解法 ⭐
 */

// ============================================================
// 解法1: 暴力模拟 — 每场会议扫描所有房间
// 时间: O(m·n)  空间: O(n)   其中 m = meetings.size()
//
// 【思路】
// 最直觉的模拟：维护每个房间的结束时间 roomEnd[i]。
// 对每场会议：
//   1) 先看有没有 roomEnd[i] <= start 的空闲房间，取编号最小的
//   2) 没有空闲的话，找 roomEnd[i] 最小的（即最早结束的），延期到那个时间
// 
// 为什么从暴力开始？因为模拟逻辑最清晰，不容易出错，
// 而且本题 n ≤ 100，暴力 O(m·n) ≈ 10^7 完全能过。
// ============================================================

// 模拟过程（示例1）:
// n=2, meetings = [[0,10],[1,5],[2,7],[3,4]] (已按start排序)
//
// roomEnd = [0, 0]  count = [0, 0]
//
// Meeting [0,10): 扫描 → room 0 空闲(end=0 <= 0) → roomEnd=[10,0] count=[1,0]
// Meeting [1, 5): 扫描 → room 0 占用(end=10>1), room 1 空闲(end=0<=1) → roomEnd=[10,5] count=[1,1]
// Meeting [2, 7): 扫描 → 无空闲 → 最早结束 room 1(end=5) → 延期到5, end=5+5=10 → roomEnd=[10,10] count=[1,2]
// Meeting [3, 4): 扫描 → 无空闲 → 最早结束 room 0(end=10, 编号小) → 延期到10, end=10+1=11 → roomEnd=[11,10] count=[2,2]
//
// count = [2,2] → 最大值=2，最小编号=0 → 返回 0

class Solution1 {
public:
    int mostBooked(int n, vector<vector<int>>& meetings) {
        sort(meetings.begin(), meetings.end());
        
        vector<long long> roomEnd(n, 0); // 每个房间的结束时间，long long 防溢出
        vector<int> count(n, 0);
        
        for (auto& m : meetings) {
            long long start = m[0], dur = m[1] - m[0];
            
            // 优先找空闲房间（编号从小到大扫描，第一个空闲的就是编号最小的）
            int chosen = -1;
            for (int i = 0; i < n; i++) {
                if (roomEnd[i] <= start) {
                    chosen = i;
                    break;
                }
            }
            
            if (chosen == -1) {
                // 没有空闲房间 → 找最早结束的（结束时间相同取编号小的）
                // 因为从小到大遍历，第一个最小 roomEnd 就满足"编号最小"
                long long earliest = LLONG_MAX;
                for (int i = 0; i < n; i++) {
                    if (roomEnd[i] < earliest) {
                        earliest = roomEnd[i];
                        chosen = i;
                    }
                }
                // 延期：从 earliest 开始，持续 dur
                roomEnd[chosen] = earliest + dur;
            } else {
                // 正常安排
                roomEnd[chosen] = start + dur;
            }
            
            count[chosen]++;
        }
        
        // max_element 返回迭代器，减去 begin 得到下标
        return max_element(count.begin(), count.end()) - count.begin();
    }
};


// ============================================================
// 解法2: 双堆模拟 — 空闲堆 + 占用堆 ⭐ 面试首选
// 时间: O(m·log n)  空间: O(n)
//
// 【思路】
// 解法1 每次 O(n) 扫描所有房间，瓶颈在两个操作：
//   1) "找编号最小的空闲房间" → 用小顶堆（按房间号）
//   2) "找最早结束的占用房间" → 用小顶堆（按结束时间,房间号）
//
// 两个堆各司其职：
//   idle: 小顶堆，存空闲房间号 → top 就是编号最小的空闲房间
//   busy: 小顶堆，存 (结束时间, 房间号) → top 就是最早结束的房间
//
// 每个房间在任何时刻只存在于其中一个堆里（不会重复分配）。
// 处理新会议前，先"释放"：把 busy 中结束时间 <= start 的房间移到 idle。
//
// 为什么两个堆不能合并成一个？
// 因为空闲房间按"编号"排优先级，占用房间按"结束时间"排优先级，
// 排序标准不同，必须分开维护。
// ============================================================

// 双堆状态变化（示例2）:
// n=3, meetings = [[1,20],[2,10],[3,5],[4,9],[6,8]]
//
// 初始: idle={0,1,2}  busy={}
//
// Meeting [1,20): 释放:无 → idle={0,1,2} 非空 → 取room 0
//   idle={1,2}  busy={(20,0)}  count=[1,0,0]
//
// Meeting [2,10): 释放:无 → idle={1,2} 非空 → 取room 1
//   idle={2}  busy={(10,1),(20,0)}  count=[1,1,0]
//
// Meeting [3,5):  释放:无 → idle={2} 非空 → 取room 2
//   idle={}  busy={(5,2),(10,1),(20,0)}  count=[1,1,1]
//
// Meeting [4,9):  释放:无(5>4) → idle={} 空! → 取busy top (5,2)
//   延期到t=5, 持续5, 结束t=10 → busy={(10,1),(10,2),(20,0)}  count=[1,1,2]
//
// Meeting [6,8):  释放:无(10>6) → idle={} 空! → 取busy top (10,1)
//   延期到t=10, 持续2, 结束t=12 → busy={(10,2),(12,1),(20,0)}  count=[1,2,2]
//
// count=[1,2,2] → 最大值=2，最小编号=1 → 返回 1

class Solution2 {
public:
    int mostBooked(int n, vector<vector<int>>& meetings) {
        sort(meetings.begin(), meetings.end());
        
        // 空闲房间堆：小顶堆，房间号小的优先
        priority_queue<int, vector<int>, greater<int>> idle;
        
        // 占用房间堆：小顶堆，(结束时间, 房间号) 小的优先
        // 结束时间用 long long —— 多次延期后可能超 int 范围
        priority_queue<pair<long long, int>,
                       vector<pair<long long, int>>,
                       greater<pair<long long, int>>> busy;
        
        for (int i = 0; i < n; i++) idle.push(i);
        
        vector<int> count(n, 0);
        
        for (auto& m : meetings) {
            long long start = m[0], end = m[1];
            long long dur = end - start;
            
            // 释放：把所有结束时间 <= start 的房间归还到空闲堆
            // 为什么是 <=？半闭区间 [s,e) 中 e 时刻房间已空闲
            while (!busy.empty() && busy.top().first <= start) {
                idle.push(busy.top().second);
                busy.pop();
            }
            
            int room;
            if (!idle.empty()) {
                // 有空闲房间 → 取编号最小的
                room = idle.top();
                idle.pop();
                busy.push({start + dur, room});
            } else {
                // 无空闲房间 → 取最早结束的房间，会议延期
                auto [freeTime, r] = busy.top();
                busy.pop();
                room = r;
                // 延期：从 freeTime 开始，持续 dur（不是 end！）
                busy.push({freeTime + dur, room});
            }
            
            count[room]++;
        }
        
        return max_element(count.begin(), count.end()) - count.begin();
    }
};


// ============================================================
// 【解法对比】
//
// | 维度         | 解法1 暴力模拟   | 解法2 双堆模拟      |
// |-------------|-----------------|-------------------|
// | 时间         | O(m·n)          | O(m·log n)        |
// | 空间         | O(n)            | O(n)              |
// | 代码量       | 短，逻辑直接      | 中等，需管理两个堆  |
// | 适用范围     | n ≤ 100 够用     | n 很大时也高效      |
// | 面试         | 讲思路用         | 需要写出来         |
//
// 本题 n ≤ 100，两种解法都能 AC。
// 但面试考的是堆的使用能力，解法2 是期望答案。
//
// ============================================================
// 【易错点】
//
// 1. long long 溢出：
//    ✗ pair<int, int> 存结束时间 → 10^5 个会议延期叠加可达 5×10^10
//    ✓ pair<long long, int> 存结束时间
//
// 2. 释放条件写错：
//    ✗ busy.top().first < start  → 半闭区间，end==start 时已空闲
//    ✓ busy.top().first <= start
//
// 3. 延期结束时间用了 end 而不是 dur：
//    ✗ busy.push({freeTime + end, room})   → end 是原始结束时间，不是持续时间
//    ✓ busy.push({freeTime + dur, room})   → dur = end - start 才是持续时间
//
// 4. 忘记排序：
//    题目要求"原开始时间更早的会议优先获得房间"
//    必须 sort(meetings.begin(), meetings.end())
//
// 5. max_element 返回迭代器不是值：
//    ✗ return max_element(count.begin(), count.end());  // 编译可能过但语义错
//    ✓ return max_element(count.begin(), count.end()) - count.begin();
//
// ============================================================
// 【面试追问】
//
// Q1（基础理解）: 暴力法怎么做？本题约束下能过吗？
//   → O(m·n), n≤100 约 10^7，能过。但面试需要展示堆的方案。
//
// Q2（优化）: 为什么用两个堆而不是一个？
//   → 空闲房间按"编号"优先、占用房间按"结束时间"优先，排序标准不同。
//     一个堆只能维护一种排序顺序。
//
// Q3（变体）: 如果同一时间有多场会议开始怎么办？
//   → 题目保证 start 互不相同。如果允许相同，只需排序时加第二关键字(如按end排)，
//     逻辑不变——仍是按排序后的顺序逐个分配。
//
// Q4（变体）: 如果会议可以被取消，怎么修改？
//   → 普通堆不支持删除中间元素。方案1: 懒删除（标记，pop 时跳过）。
//     方案2: 用 std::set 代替堆，支持 O(log n) 任意位置删除。
// ============================================================
