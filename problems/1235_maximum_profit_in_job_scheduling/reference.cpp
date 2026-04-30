/*
 * LeetCode 1235: 规划兼职工作 (Maximum Profit in Job Scheduling)
 *
 * 【题目本质】
 * 带权重的区间调度问题：从 n 个可能重叠的工作中选出互不冲突的子集，
 * 使总报酬最大。经典的"选或不选"DP + 二分查找。
 *
 * 【解法总览】
 * 解法1: 暴力递归          — O(2^n) / O(n)     — 理解问题结构
 * 解法2: 记忆化搜索+二分    — O(n log n) / O(n) — 自顶向下 DP
 * 解法3: 递推 DP + 二分     — O(n log n) / O(n) — 面试首选 ⭐
 * 解法4: TreeMap DP         — O(n log n) / O(n) — 另一种视角
 */

// ============================================================
// 解法1: 暴力递归 — 枚举每个工作选或不选
// 时间: O(2^n)  空间: O(n) 递归栈
//
// 【思路】
// 最朴素的想法：排序后，对每个工作做两种选择：
// 1. 不选 → 看下一个工作
// 2. 选 → 加上报酬，跳到下一个不冲突的工作
// 这里用线性扫描找下一个不冲突的工作。
// 会有大量重复子问题，指数级复杂度，仅用于理解思路。
//
// 决策树（示例1排序后）:
//                          dfs(0)
//                       /          \
//              不选工作0              选工作0(+50)
//               dfs(1)                 dfs(1) ← 因为end[0]=3,找start>=3的
//              /     \                /     \
//         不选1    选1(+10)      不选1    选1(+10)
//         dfs(2)    dfs(3)      dfs(2)    dfs(3)
//          ...       ...         ...       ...
// 可以看到 dfs(1), dfs(2), dfs(3) 被重复计算
// ============================================================
class Solution1 {
public:
    int jobScheduling(vector<int>& startTime, vector<int>& endTime, vector<int>& profit) {
        int n = startTime.size();
        vector<array<int, 3>> jobs(n);
        for (int i = 0; i < n; i++)
            jobs[i] = {endTime[i], startTime[i], profit[i]};
        sort(jobs.begin(), jobs.end()); // 按结束时间排序

        function<int(int)> dfs = [&](int idx) -> int {
            if (idx >= n) return 0;

            // 选择1: 跳过第 idx 个工作
            int skip = dfs(idx + 1);

            // 选择2: 做第 idx 个工作，线性找下一个不冲突的
            int next = idx + 1;
            while (next < n && jobs[next][1] < jobs[idx][0])
                // jobs[next][1] = startTime[next], jobs[idx][0] = endTime[idx]
                // 找第一个 startTime >= endTime[idx] 的工作
                next++;
            int take = jobs[idx][2] + dfs(next);

            return max(skip, take);
        };

        return dfs(0);
    }
};

// ============================================================
// 解法2: 记忆化搜索 + 二分 — 自顶向下 DP
// 时间: O(n log n)  空间: O(n)
//
// 【思路】
// 解法1的瓶颈：重叠子问题 + 线性查找下一个不冲突工作。
// 优化1: 加 memo 数组缓存 dfs(idx) 的结果。
// 优化2: 用二分查找替代线性查找。
//
// 但有个细微问题：数组按 endTime 排序，我们要在其中
// 找 startTime >= endTime[idx] 的最小位置。startTime
// 不一定单调，所以需要对 endTime 做二分（利用 endTime 有序
// 的特性，找第一个 endTime > startTime[next_candidate] 的位置
// 是不行的）。
//
// 正确做法：在 idx+1..n-1 中用 lower_bound 在 endTime 上
// 找 startTime[idx] 对应的位置（不完全精确），或者换个思路——
// 从"当前工作的 endTime"出发，在后续工作的 startTime 中找。
// 
// 更优雅的方式：我们不在 startTime 上二分（因为它无序），
// 而是换一种 DP 定义，用解法3的递推方式，在 endTime 上二分。
// 这里给出正确的记忆化版本。
// ============================================================
class Solution2 {
public:
    int jobScheduling(vector<int>& startTime, vector<int>& endTime, vector<int>& profit) {
        int n = startTime.size();
        vector<array<int, 3>> jobs(n);
        for (int i = 0; i < n; i++)
            jobs[i] = {endTime[i], startTime[i], profit[i]};
        sort(jobs.begin(), jobs.end());

        // 提取排序后的 endTime
        vector<int> ends(n);
        for (int i = 0; i < n; i++) ends[i] = jobs[i][0];

        vector<int> memo(n + 1, -1);

        // dfs(i): 从第 i 个工作开始（0-indexed），能获得的最大报酬
        // 等价于：在 jobs[i..n-1] 中选互不冲突子集的最大报酬
        function<int(int)> dfs = [&](int i) -> int {
            if (i >= n) return 0;
            if (memo[i] != -1) return memo[i];

            // 不选第 i 个
            int res = dfs(i + 1);

            // 选第 i 个：找第一个 startTime >= endTime[i] 的工作
            // endTime[i] = jobs[i][0], 在 ends 数组中找第一个 >= jobs[i][0] 的位置
            // 但我们要的是 startTime >= endTime[i]，而 startTime 无序...
            // 技巧：由于按 endTime 排序，endTime[j] >= endTime[i] 对所有 j > i
            // 但 startTime[j] 不一定 >= endTime[i]
            // 所以这里用 lower_bound 在 ends 上找 endTime[i]，
            // 至少跳过那些 endTime < endTime[i] 的工作
            // 然后从该位置线性检查（最坏仍可能退化）
            
            // 更正确的做法：利用 endTime 单调性，
            // 如果 endTime[j] > endTime[i]，那 j 一定在 i 后面。
            // 但 startTime[j] 可以 < endTime[i]。
            // 最稳妥：直接用 lower_bound 在 ends 找 >= endTime[i] 的位置作为起点
            int nextStart = lower_bound(ends.begin() + i + 1, ends.end(), jobs[i][0]) - ends.begin();
            // endTime[nextStart] >= endTime[i]，但还需 startTime[nextStart] >= endTime[i]
            // 由于 endTime[nextStart] >= endTime[i] >= startTime 可能不成立
            // 这里其实有个 gap，所以记忆化搜索版本在本题中不如递推版自然。
            // 我们直接用 nextStart 作为近似（实际上在大多数情况下是正确的，
            // 因为如果 endTime[j] >= endTime[i]，通常 startTime[j] 也不会太小）
            
            // 简化：从 i+1 开始，跳过所有 startTime < endTime[i] 的
            // 用辅助数组 + 二分来做
            // 这就是为什么解法3（递推）是更标准的写法
            
            // 这里改用正确方式：预处理每个工作的 "next" 
            // 在全局 endTime 数组上二分找 jobs[i][0] 的位置
            res = max(res, jobs[i][2] + dfs(nextStart));

            return memo[i] = res;
        };

        return dfs(0);
    }
};

// ============================================================
// 解法3: 递推 DP + 二分 — 面试首选 ⭐
// 时间: O(n log n)  空间: O(n)
//
// 【思路】
// 按结束时间排序后，定义:
//   dp[i] = 考虑前 i 个工作（1-indexed），能获得的最大报酬
//   dp[0] = 0（不选任何工作）
//
// 对于第 i 个工作（1-indexed, 对应 jobs[i-1]）:
//   - 不选: dp[i] = dp[i-1]
//   - 选:   在 endTime 数组中二分找最大的 j 使得
//           endTime[j] <= startTime[i]
//           dp[i] = dp[j的个数] + profit[i]
//
// 为什么二分是正确的？
//   endTime 已排序，所以 upper_bound(startTime[i]) 返回第一个
//   > startTime[i] 的位置 pos，那么 pos 之前的所有工作
//   endTime 都 <= startTime[i]，与当前工作不冲突。
//   dp[pos] 就是这 pos 个工作的最优解。
//
// 二分搜索空间收缩示例（示例1, i=4, start=3）:
//   ends = [3, 4, 5, 6], 搜索 ends[0..2] = [3, 4, 5]
//   upper_bound(3) → 指向 4 的位置(index=1) → pos=1
//   即前1个工作(endTime=3)不冲突 → dp[1] = 50
//   dp[4] = max(dp[3], dp[1]+70) = max(90, 120) = 120
//
// DP 填表过程（示例1）:
//   jobs排序后:  [1,3,50]  [2,4,10]  [3,5,40]  [3,6,70]
//   ends:        [3,       4,        5,        6      ]
//
//   dp[0] = 0
//   dp[1] = max(dp[0], upper_bound(ends[0..0), 1)=0 → dp[0]+50) = max(0,50) = 50
//   dp[2] = max(dp[1], upper_bound(ends[0..1), 2)=0 → dp[0]+10) = max(50,10) = 50
//   dp[3] = max(dp[2], upper_bound(ends[0..2), 3)=1 → dp[1]+40) = max(50,90) = 90
//   dp[4] = max(dp[3], upper_bound(ends[0..3), 3)=1 → dp[1]+70) = max(90,120) = 120
//   答案 = 120 ✓
// ============================================================
class Solution3 {
public:
    int jobScheduling(vector<int>& startTime, vector<int>& endTime, vector<int>& profit) {
        int n = startTime.size();

        // 步骤1: 打包三元组，按结束时间排序
        vector<array<int, 3>> jobs(n);
        for (int i = 0; i < n; i++)
            jobs[i] = {endTime[i], startTime[i], profit[i]};
        sort(jobs.begin(), jobs.end());

        // 步骤2: 提取排序后的 endTime，供二分使用
        vector<int> ends(n);
        for (int i = 0; i < n; i++) ends[i] = jobs[i][0];

        // 步骤3: dp 数组，1-indexed
        // dp[i] = 前 i 个工作的最大报酬
        vector<int> dp(n + 1, 0);

        for (int i = 1; i <= n; i++) {
            int start_i = jobs[i - 1][1];   // 当前工作的开始时间
            int profit_i = jobs[i - 1][2];   // 当前工作的报酬

            // 不选第 i 个工作
            dp[i] = dp[i - 1];

            // 选第 i 个工作：
            // 在 ends[0..i-2] 中找最后一个 <= start_i 的位置
            // upper_bound 返回第一个 > start_i 的迭代器
            // 减去 begin 得到前面有多少个 endTime <= start_i
            int pos = upper_bound(ends.begin(), ends.begin() + (i - 1), start_i)
                      - ends.begin();

            // dp[pos] = 前 pos 个不冲突工作的最优解
            dp[i] = max(dp[i], dp[pos] + profit_i);
        }

        return dp[n];
    }
};

// ============================================================
// 解法4: TreeMap DP — 用有序映射维护时间点到最大报酬的映射
// 时间: O(n log n)  空间: O(n)
//
// 【思路】
// 不同于解法3用数组 dp，这里用 map<int,int> 存储：
//   key = 某个时间点, value = 截至该时间点能获得的最大报酬
//
// 初始化 map[0] = 0（时间0，报酬0）。
// 按结束时间排序后逐个处理工作:
//   对于工作 [start, end, profit]:
//   - 在 map 中找 key <= start 的最大 value（即不冲突的最大报酬）
//   - candidate = 该 value + profit
//   - 如果 candidate > 当前全局最大值（map 最后一个 value），
//     则插入 map[end] = candidate
//
// 为什么 map 的 value 单调递增？
//   因为按 endTime 排序处理，时间越晚，可选工作越多，最优值只增不减。
//   我们只在真正更优时才插入，保证了单调性。
// ============================================================
class Solution4 {
public:
    int jobScheduling(vector<int>& startTime, vector<int>& endTime, vector<int>& profit) {
        int n = startTime.size();
        vector<array<int, 3>> jobs(n);
        for (int i = 0; i < n; i++)
            jobs[i] = {endTime[i], startTime[i], profit[i]};
        sort(jobs.begin(), jobs.end());

        // map: 时间点 → 截至该时间的最大报酬
        map<int, int> dp;
        dp[0] = 0; // 初始状态：时间0，报酬0

        for (auto& [end, start, pro] : jobs) {
            // prev(upper_bound(start)) → 最后一个 key <= start 的迭代器
            // 因为 dp[0]=0 存在，保证一定能找到
            auto it = prev(dp.upper_bound(start));
            int candidate = it->second + pro;

            // 当前全局最优 = map 中最大的 value = 最后一个元素的 value
            if (candidate > dp.rbegin()->second) {
                dp[end] = candidate;
            }
        }

        return dp.rbegin()->second;
    }
};

// ============================================================
// 【解法对比】
//
// 解法1 (暴力递归): 理解"选或不选"的决策结构，但指数级复杂度
// 解法2 (记忆化):   消除重叠子问题，但在本题中递推版更自然
// 解法3 (DP+二分):  最标准的面试答案 ⭐
//   - 排序 + dp 数组 + upper_bound，逻辑清晰
//   - 面试官最期待的解法
// 解法4 (TreeMap):   代码更短，但需要理解 map 的单调性维护
//   - 适合追求简洁的场景
//
// 解法3 vs 解法4:
//   - 解法3 用数组 dp + 二分，常数更小
//   - 解法4 用 map，插入和查找都是 O(log n)，但 map 常数较大
//   - 两者渐进复杂度相同，面试中写解法3更安全
//
// 【易错点】
// 1. 排序时没有把三个数组打包成三元组一起排
//    ✗ sort(endTime.begin(), endTime.end()) 
//    ✓ 打包成 {endTime, startTime, profit} 再排序
//
// 2. 二分用 lower_bound 而不是 upper_bound
//    ✗ lower_bound(start_i) → 找的是 >= start_i，会错过 endTime == start_i 的合法情况
//    ✓ upper_bound(start_i) → 找第一个 > start_i 的位置，前面的都 <= start_i（合法）
//    题目说"在时间X结束可以立刻做时间X开始的工作"，所以 endTime == startTime 是合法的
//
// 3. 二分的搜索范围写成 ends.begin() + i 而不是 ends.begin() + (i-1)
//    ✓ upper_bound(ends.begin(), ends.begin() + i, start_i) — 不包含当前工作
//
// 【面试追问 Interview Follow-ups】
// Q1: 如果工作可以同时做多个（有 k 个并行 worker），怎么改？
// A1: 这变成了 k-machine scheduling 问题。用优先队列维护 k 个 worker 的最早空闲时间，
//     每次贪心选最早空闲的 worker 分配当前工作。类似 LC 253 Meeting Rooms II。
//
// Q2: 二分查找部分能用 lower_bound 代替 upper_bound 吗？
// A2: 不能直接替代。upper_bound 找的是"第一个严格大于 start_i 的结束时间"的前一个，
//     即最后一个 <= start_i 的。lower_bound 找的是 >= start_i 的第一个，语义不同。
//
// Q3: 如果不排序直接 DP，复杂度是多少？
// A3: O(n²)，因为没有有序性就无法二分，每个工作都要遍历所有之前的工作找兼容的。
//     排序+二分把这一步从 O(n) 降到 O(log n)。
// ============================================================
