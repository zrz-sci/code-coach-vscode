/*
 * LeetCode 1353: 最多可以参加的会议数目
 * Maximum Number of Events That Can Be Attended
 *
 * 难度: Medium
 * 标签: Greedy, Array, Sorting, Heap (Priority Queue)
 *
 * =============================================
 * 解法: 贪心 + 排序 + 小顶堆 (最早截止时间优先)
 * =============================================
 *
 * 核心思路:
 *   逐天枚举, 每天从当前可参加的所有会议中,
 *   选择截止日期最早 (endDay 最小) 的那个参加。
 *
 * 贪心正确性 (Earliest Deadline First):
 *   截止早的会议"快过期了", 必须优先参加;
 *   截止晚的会议还有时间, 以后再参加。
 *   交换论证: 将任何最优解中的选择替换为 EDF 选择, 不会减少总数。
 *
 * 算法步骤:
 *   1. 按 startDay 排序
 *   2. 逐天枚举 day = 1, 2, ...
 *      (a) startDay == day 的会议入堆 (push endDay)
 *      (b) 清除过期会议 (endDay < day)
 *      (c) 参加堆顶会议 (endDay 最小)
 *
 * 时间: O(n log n + D), D = max(endDay) <= 10^5
 * 空间: O(n)
 */

class Solution {
public:
    int maxEvents(vector<vector<int>>& events) {
        int n = events.size();

        // ── 第一步: 按 startDay 排序 ──
        // startDay 相同时, endDay 小的排前面 (pair 默认比较)
        sort(events.begin(), events.end());

        // ── 第二步: 小顶堆 + 逐天枚举 ──
        // 小顶堆: 存 endDay, 堆顶 = 截止最早的会议 = 最紧急的
        // C++ 小顶堆: priority_queue<int, vector<int>, greater<int>>
        priority_queue<int, vector<int>, greater<int>> minHeap;

        int ptr = 0;    // 指向下一个待入堆的会议
        int count = 0;  // 已参加的会议总数

        // 枚举每一天
        for (int day = 1; day <= 100000; day++) {
            // (a) 将所有 startDay == day 的会议入堆
            //     因为已按 startDay 排序, ptr 单向递增
            while (ptr < n && events[ptr][0] == day) {
                minHeap.push(events[ptr][1]);  // 入堆: endDay
                ptr++;
            }

            // (b) 清除过期会议: endDay < day → 已无法参加
            while (!minHeap.empty() && minHeap.top() < day) {
                minHeap.pop();  // 丢弃过期会议
            }

            // (c) 参加截止最早的会议 (堆顶)
            if (!minHeap.empty()) {
                minHeap.pop();  // 参加并从堆中移除
                count++;
            }

            // (d) 提前终止: 所有会议已入堆且堆已空
            if (ptr >= n && minHeap.empty()) break;
        }

        return count;
    }
};


/*
 * =============================================
 * 解法二: 优化版 — 跳过空闲天
 * =============================================
 *
 * 当堆为空且还有未处理的会议时, 直接跳到下一个会议的 startDay,
 * 避免在没有会议的天空转。
 *
 * 适用于: 会议稀疏分布在很大天数范围内的场景
 *         (如只有 [1,1] 和 [99999,100000])
 *
 * 时间: O(n log n)  (不再依赖 D)
 * 空间: O(n)
 */

class Solution_Optimized {
public:
    int maxEvents(vector<vector<int>>& events) {
        int n = events.size();
        sort(events.begin(), events.end());

        priority_queue<int, vector<int>, greater<int>> minHeap;
        int ptr = 0;
        int count = 0;
        int day = 1;

        while (ptr < n || !minHeap.empty()) {
            // 跳过空闲天: 堆空时直接跳到下一个会议开始日
            if (minHeap.empty() && ptr < n) {
                day = max(day, events[ptr][0]);
            }

            // (a) 加入所有 startDay <= day 的会议
            //     注意这里用 <= (因为跳过天后可能跨过了多个 startDay)
            while (ptr < n && events[ptr][0] <= day) {
                minHeap.push(events[ptr][1]);
                ptr++;
            }

            // (b) 清除过期
            while (!minHeap.empty() && minHeap.top() < day) {
                minHeap.pop();
            }

            // (c) 参加最紧急的
            if (!minHeap.empty()) {
                minHeap.pop();
                count++;
            }

            day++;  // 推进到下一天
        }

        return count;
    }
};


/*
 * =============================================
 * 干跑示例 1: events = [[1,2],[2,3],[3,4]]
 * =============================================
 *
 * 排序后: [[1,2],[2,3],[3,4]]
 *
 * Day 1: push endDay=2. minHeap={2}. 参加 pop 2. count=1.
 * Day 2: push endDay=3. minHeap={3}. 参加 pop 3. count=2.
 * Day 3: push endDay=4. minHeap={4}. 参加 pop 4. count=3.
 *         ptr=3 && 堆空 → break.
 *
 * return 3 ✓
 *
 * =============================================
 * 干跑示例 2: events = [[1,2],[2,3],[3,4],[1,2]]
 * =============================================
 *
 * 排序后: [[1,2],[1,2],[2,3],[3,4]]
 *          A      D      B      C
 *
 * Day 1: push 2, 2 (A和D). minHeap={2,2}.
 *         参加: pop 2. count=1. minHeap={2}.
 *
 * Day 2: push 3 (B). minHeap={2,3}.
 *         清除过期: 堆顶=2 >= 2, 不过期.
 *         参加: pop 2 (会议D, 今天最后机会!). count=2. minHeap={3}.
 *
 * Day 3: push 4 (C). minHeap={3,4}.
 *         参加: pop 3 (会议B). count=3. minHeap={4}.
 *
 * Day 4: 无新会议. minHeap={4}.
 *         参加: pop 4 (会议C). count=4. minHeap={}.
 *         ptr=4 && 堆空 → break.
 *
 * return 4 ✓
 *
 * =============================================
 * 贪心关键验证: Day 2 如果不选会议D(end=2)?
 * =============================================
 *
 * 假设 Day 2 选了会议B(end=3) 而非 D(end=2):
 *   Day 3: 堆中只有 {4} (D 已过期被清除)
 *          参加 C. count=3.
 *   Day 4: 堆空.
 *   总计: 3 个 (比贪心的 4 个少!)
 *
 * → 证实: 优先选截止最早的 (EDF) 确实更优
 *
 * =============================================
 * 边界情况
 * =============================================
 *
 * 1. 所有会议同一天 [[5,5],[5,5],[5,5]]:
 *    Day 5: push 5,5,5. minHeap={5,5,5}.
 *    参加 1 个. count=1.
 *    Day 6: 清除过期 5<6, pop 两个.
 *    堆空 + ptr=3 → break. return 1
 *
 * 2. 一个超长会议 [[1,100000]]:
 *    Day 1: push 100000. 参加. count=1. break. return 1
 *
 * 3. 完全不重叠 [[1,1],[3,3],[5,5]]:
 *    (优化版跳过 Day 2, Day 4)
 *    Day 1: 参加. Day 3: 参加. Day 5: 参加. return 3
 *
 * =============================================
 * 面试追问
 * =============================================
 *
 * Q1: 每个会议有不同权重，最大化总权重？
 *     → 不能贪心，变成带权区间调度 (LC 1751)，用 DP + 排序 + 二分
 *
 * Q2: 为什么不用大顶堆（选 endDay 最大的）？
 *     → 选截止晚的会导致截止早的过期，交换论证证明 EDF 不劣于最优解
 *
 * Q3: 如果每天可以参加 k 个会议？
 *     → 每天 pop k 个堆顶，count += 实际 pop 个数
 *
 * =============================================
 * 易错点
 * =============================================
 *
 * ✗ 用大顶堆而非小顶堆 → 应选 endDay 最小（最紧急）
 * ✗ 忘记清除过期会议 → 堆中 endDay < day 的必须 pop
 * ✗ 枚举范围用 n 而非 max(endDay) → 天数与会议数无关
 * ✗ 不跳过空闲天 → 稀疏分布时退化为 O(D) 空转
 */
