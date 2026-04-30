/*
 * LeetCode 621: 任务调度器 (Task Scheduler)
 *
 * 【题目本质】
 * 给定任务频率和冷却间隔 n，求完成所有任务的最短时间。
 * 本质是：最高频任务决定了时间线的骨架，其他任务填入空隙，
 * 填不满的用 idle 补齐。
 *
 * 【解法总览】
 * 解法1: 贪心+堆模拟    — O(N) / O(1) — 直观模拟，可输出调度序列
 * 解法2: 数学公式(贪心)  — O(N) / O(1) — 面试首选，一步到位
 * 解法3: 贪心+排序模拟   — O(N) / O(1) — 用排序代替堆
 *
 * 注: N = tasks.length，由于最多26种任务，堆操作和排序都是常数级别
 */

// ============================================================
// 解法1: 贪心+堆模拟
// 时间: O(N)  空间: O(1)（最多26种任务）
//
// 【思路】
// 贪心策略：每个时间单位，选"当前可执行且剩余次数最多"的任务。
// 为什么优先高频？因为高频任务是产生 idle 的根源，早消耗少浪费。
//
// 实现：按 n+1 为一轮，每轮从最大堆中取最多 n+1 个不同任务执行，
// 频率减1后放回。一轮不满且后面还有任务，差额用 idle 补齐。
//
// 以 tasks=["A","A","A","B","B","B"], n=2 为例：
//
//   堆状态        取出         时间
//   {3,3}     →  A,B        → +3 (A B idle)  堆←{2,2}
//   {2,2}     →  A,B        → +3 (A B idle)  堆←{1,1}
//   {1,1}     →  A,B        → +2 (A B)       堆空，最后一轮不补idle
//   总时间 = 3 + 3 + 2 = 8
// ============================================================
class Solution1 {
public:
    int leastInterval(vector<char>& tasks, int n) {
        // 统计每种任务的频率
        vector<int> freq(26, 0);
        for (char c : tasks) freq[c - 'A']++;
        
        // 最大堆，存放非零频率
        priority_queue<int> maxHeap;
        for (int f : freq) {
            if (f > 0) maxHeap.push(f);
        }
        
        int time = 0;
        while (!maxHeap.empty()) {
            int cycle = n + 1;       // 每轮 n+1 个时间槽
            int taskCount = 0;       // 本轮实际执行的任务数
            vector<int> temp;        // 暂存本轮执行后仍有剩余的频率
            
            // 每轮取出最多 cycle 个不同任务
            for (int i = 0; i < cycle && !maxHeap.empty(); i++) {
                int cur = maxHeap.top();
                maxHeap.pop();
                if (cur - 1 > 0) {
                    temp.push_back(cur - 1); // 频率减1，还有剩余
                }
                taskCount++;
            }
            
            // 放回堆中
            for (int t : temp) maxHeap.push(t);
            
            // 关键：最后一轮不补 idle（堆空说明是最后一轮）
            // 非最后一轮需要补齐到 cycle（n+1）
            time += maxHeap.empty() ? taskCount : cycle;
        }
        return time;
    }
};

// ============================================================
// 解法2: 数学公式（贪心）— 面试首选 ⭐
// 时间: O(N)  空间: O(1)
//
// 【思路】
// 核心观察：出现次数最多的任务决定了时间线的"骨架"。
//
// 以 tasks=["A","A","A","B","B","B"], n=2 为例:
// maxFreq=3, countOfMax=2 (A和B都出现3次)
//
// 骨架按 n+1=3 为一组排列：
//
//   | A | B | idle |   ← 间隔段1 (长度 n+1 = 3)
//   | A | B | idle |   ← 间隔段2 (长度 n+1 = 3)
//   | A | B |          ← 最后段 (长度 = countOfMax = 2)
//
// 公式: (maxFreq - 1) * (n + 1) + countOfMax
//      = (3-1) * 3 + 2 = 8
//
// 但当任务种类很多、n 很小时，空位全部被填满甚至不够用：
//   tasks=["A","A","A","B","B","B","C","C","C","D","D","E"], n=2
//   公式 = 2*3+3 = 9，但 tasks.size() = 12
//   此时没有 idle，答案 = max(9, 12) = 12
//
// 为什么取 max？
// - 公式计算的是"高频任务约束下的最短时间"
// - tasks.size() 是"不管怎样每个任务至少占1个时间单位"
// - 真正的答案是两者中较大的那个
// ============================================================
class Solution2 {
public:
    int leastInterval(vector<char>& tasks, int n) {
        vector<int> freq(26, 0);
        for (char c : tasks) freq[c - 'A']++;
        
        // 找到最高频率
        int maxFreq = *max_element(freq.begin(), freq.end());
        
        // 统计出现次数等于最高频率的任务种类数
        // 注意：是 == maxFreq，不是 > 0
        int countOfMax = 0;
        for (int f : freq) {
            if (f == maxFreq) countOfMax++;
        }
        
        // (maxFreq-1) 个完整间隔段，每段长 n+1
        // 加上最后一段，长度为 countOfMax
        int formulaResult = (maxFreq - 1) * (n + 1) + countOfMax;
        
        // 答案不可能比任务总数少（每个任务至少占1个时间单位）
        return max(formulaResult, (int)tasks.size());
    }
};

// ============================================================
// 解法3: 贪心+排序模拟
// 时间: O(N)  空间: O(1)
//
// 【思路】
// 和堆模拟的思路一样，但用排序代替堆。
// 每轮将频率数组降序排序，取前 n+1 个非零频率的任务执行。
// 由于只有26种任务，排序 O(26log26) = O(1)。
//
// 优点：不需要额外的堆结构，代码更简洁
// 缺点：每轮都要排序（虽然是常数，但常数因子比堆稍大）
// ============================================================
class Solution3 {
public:
    int leastInterval(vector<char>& tasks, int n) {
        vector<int> freq(26, 0);
        for (char c : tasks) freq[c - 'A']++;
        
        sort(freq.begin(), freq.end(), greater<int>());
        
        int time = 0;
        while (freq[0] > 0) { // freq[0] 是当前最高频率，为0说明全部完成
            int taskCount = 0;
            
            // 每轮取前 n+1 个非零频率的任务
            for (int i = 0; i <= n && i < 26; i++) {
                if (freq[i] > 0) {
                    freq[i]--;
                    taskCount++;
                }
            }
            
            // 重新排序，保持降序
            sort(freq.begin(), freq.end(), greater<int>());
            
            // 最后一轮不补 idle
            time += (freq[0] > 0) ? (n + 1) : taskCount;
        }
        return time;
    }
};

// ============================================================
// 【解法对比】
//
// 解法1 (堆模拟) vs 解法2 (数学公式):
//   - 堆模拟：思路直观，可扩展为"输出具体调度序列"
//   - 数学公式：代码极短（5行核心逻辑），面试首选
//   - 两者时间复杂度相同，但公式法常数更小
//
// 解法1 (堆模拟) vs 解法3 (排序模拟):
//   - 本质相同，只是"选最高频"的实现方式不同
//   - 堆：O(log26) 取最大值；排序：O(26log26) 全排序
//   - 都是常数，排序写法可能更直观
//
// 面试推荐顺序：
//   先讲解法2（展示数学思维） → 被追问"输出序列"时切换到解法1
//
// 【易错点】
//
// 1. 忘记取 max:
//    ✗ return (maxFreq - 1) * (n + 1) + countOfMax;
//    ✓ return max((maxFreq - 1) * (n + 1) + countOfMax, (int)tasks.size());
//    原因：当任务种类 >> n+1 时，间隔段被完全填满不需要idle，
//          此时公式结果 < tasks.size()
//
// 2. countOfMax 统计错误:
//    ✗ if (f > 0) countOfMax++;  // 这是统计任务种类数！
//    ✓ if (f == maxFreq) countOfMax++;  // 只统计最高频的
//    原因：最后一段只放频率等于 maxFreq 的任务
//
// 3. 堆/排序模拟中最后一轮补 idle:
//    ✗ time += cycle;  // 每轮都补到 n+1
//    ✓ time += heap.empty() ? taskCount : cycle;  // 最后一轮只算实际任务
//    原因：最后一轮后面没有任何任务需要等冷却，不需要填 idle
//
// 4. 公式中 n+1 写成 n:
//    ✗ (maxFreq - 1) * n + countOfMax
//    ✓ (maxFreq - 1) * (n + 1) + countOfMax
//    原因：每个间隔段包含 1 个高频任务 + n 个间隔 = n+1
//
// 【面试追问（递进链）】
//
// Q1: 能用暴力模拟吗？复杂度是多少？
//     可以。逐个时间点扫描所有可执行任务，选频率最高的。
//     但总时间可能远大于 N（大量idle），每个时间点扫描26种任务。
//     整体 O(总时间 × 26)，最坏 O(N × n × 26)。
//
// Q2: 能否 O(1) 直接算出答案？用的什么贪心思想？
//     公式法。贪心思想是"最高频任务决定骨架"。
//     (maxFreq-1)*(n+1) + countOfMax 描述了骨架大小，
//     取 max 与 tasks.size() 处理无 idle 的情况。
//
// Q3: 如果需要输出具体的调度序列而不只是最短时间呢？
//     用堆模拟法，把堆中改为存 {频率, 任务字符}，
//     每轮取出的字符按顺序记录，不足 n+1 的用 "idle" 补。
//     最后一轮不补 idle。参考 LeetCode 358。
//
// Q4: 如果冷却时间 n 很大（比如 10^9），但任务种类很多，
//     公式法还能用吗？
//     能。公式法 O(N) 只取决于 tasks.length，和 n 的大小无关。
//     堆/排序模拟法则可能退化，因为要模拟大量 idle 轮次。
// ============================================================
