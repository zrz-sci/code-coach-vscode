/*
 * LeetCode 502: IPO
 * 难度: Hard
 * 标签: Greedy, Array, Sorting, Heap (Priority Queue)
 *
 * =============================================
 * 解法一: 贪心 + 排序 + 大顶堆 (推荐)
 * =============================================
 *
 * 核心思路:
 *   每一轮从当前资本能启动的项目中, 选利润最大的执行。
 *   - 按 capital 升序排列项目 (索引排序)
 *   - 用指针逐步"解锁"能做的项目, 推入大顶堆
 *   - 每轮取堆顶 (利润最大) 执行, w += profit
 *
 * 贪心正确性 (交换论证):
 *   profit >= 0 → 资本单调递增 → 选利润最大后仍能选任何之前能选的项目
 *   而选利润较小后资本增长更少, 可能无法选高 capital 项目
 *   因此贪心策略不差于任何其他策略
 *
 * 时间: O(n log n) -- 排序 + 每个项目至多入堆出堆各一次
 * 空间: O(n) -- 索引数组 + 堆
 */

class Solution {
public:
    int findMaximizedCapital(int k, int w, vector<int>& profits, vector<int>& capital) {
        int n = profits.size();

        // ── 第一步: 按 capital 升序排列项目 ──
        // 使用索引排序: 不移动原始数组, 只排列索引
        vector<int> idx(n);
        iota(idx.begin(), idx.end(), 0);  // idx = {0, 1, 2, ..., n-1}
        sort(idx.begin(), idx.end(), [&](int a, int b) {
            return capital[a] < capital[b];
        });

        // ── 第二步: 大顶堆 + 指针扫描 ──
        // priority_queue<int> 默认是大顶堆 (max-heap)
        // 存储当前资本可以启动的项目的利润值
        priority_queue<int> maxHeap;
        int ptr = 0;  // 指向下一个待解锁的项目

        for (int i = 0; i < k; i++) {
            // (1) 将所有 capital <= w 的项目加入堆
            //     因为项目已按 capital 排序, ptr 只需单向移动
            while (ptr < n && capital[idx[ptr]] <= w) {
                maxHeap.push(profits[idx[ptr]]);
                ptr++;
            }

            // (2) 提前终止: 堆为空 → 没有能做的项目
            //     profit >= 0, w 不再增长, 后续轮次也不会解锁新项目
            if (maxHeap.empty()) break;

            // (3) 选利润最大的项目执行
            w += maxHeap.top();
            maxHeap.pop();
        }

        return w;
    }
};


/*
 * =============================================
 * 解法二: 使用 pair 排序 (写法更简洁)
 * =============================================
 *
 * 将 (capital, profit) 打包成 pair, pair 默认按 first 排序。
 * 逻辑完全相同, 只是排序方式不同。
 * 优点: 代码更直观, 不需要间接通过索引访问
 * 缺点: 需要额外空间存储 pair 数组
 */

class Solution_Pair {
public:
    int findMaximizedCapital(int k, int w, vector<int>& profits, vector<int>& capital) {
        int n = profits.size();

        // 打包 (capital, profit), pair 默认按 first 升序排序
        vector<pair<int, int>> projects(n);
        for (int i = 0; i < n; i++) {
            projects[i] = {capital[i], profits[i]};
        }
        sort(projects.begin(), projects.end());

        priority_queue<int> maxHeap;  // 大顶堆: 存利润
        int ptr = 0;

        for (int i = 0; i < k; i++) {
            // 解锁所有 capital <= w 的项目
            while (ptr < n && projects[ptr].first <= w) {
                maxHeap.push(projects[ptr].second);
                ptr++;
            }

            if (maxHeap.empty()) break;

            w += maxHeap.top();
            maxHeap.pop();
        }

        return w;
    }
};


/*
 * =============================================
 * 干跑示例 1: k=2, w=0, profits=[1,2,3], capital=[0,1,1]
 * =============================================
 *
 * 排序后 (按 capital):
 *   idx[0]=0: cap=0, profit=1
 *   idx[1]=1: cap=1, profit=2
 *   idx[2]=2: cap=1, profit=3
 *
 * 第 1 轮 (i=0): w=0
 *   - 解锁: cap[idx[0]]=0 <= 0 → push(1), ptr=1
 *   - cap[idx[1]]=1 > 0 → 停止
 *   - maxHeap = {1}
 *   - 取堆顶 1, w = 0 + 1 = 1
 *
 * 第 2 轮 (i=1): w=1
 *   - 解锁: cap[idx[1]]=1 <= 1 → push(2), ptr=2
 *           cap[idx[2]]=1 <= 1 → push(3), ptr=3
 *   - maxHeap = {3, 2}
 *   - 取堆顶 3, w = 1 + 3 = 4
 *
 * k=2 轮结束, return 4 ✓
 *
 *
 * =============================================
 * 干跑示例 2: k=3, w=0, profits=[1,2,3], capital=[0,1,2]
 * =============================================
 *
 * 排序后: idx[0]=0(cap=0), idx[1]=1(cap=1), idx[2]=2(cap=2)
 *
 * 第 1 轮: w=0
 *   - 解锁: cap=0 <= 0 → push(1), ptr=1
 *   - 取堆顶 1, w = 1
 *
 * 第 2 轮: w=1
 *   - 解锁: cap=1 <= 1 → push(2), ptr=2
 *   - maxHeap = {2}
 *   - 取堆顶 2, w = 3
 *
 * 第 3 轮: w=3
 *   - 解锁: cap=2 <= 3 → push(3), ptr=3
 *   - maxHeap = {3}
 *   - 取堆顶 3, w = 6
 *
 * return 6 ✓
 *
 *
 * =============================================
 * 边界情况分析
 * =============================================
 *
 * 1. k >= n: 最多做 n 个项目
 *    → ptr 到 n 后不再入堆, 堆清空后 break
 *
 * 2. 所有 capital > w: 第一轮 ptr=0 堆为空, 直接 break
 *    → return w (原始资本, 什么都没做)
 *
 * 3. 所有 capital = 0: 一次性全部入堆
 *    → 退化为 "取 top-k 最大值"
 *
 * 4. k = 0: for 循环不执行
 *    → return w
 *
 * 5. profit 有 0: 做了不亏也不赚
 *    → 贪心优先选大的, 0 只在没有更好选择时被选
 *    → 不影响正确性, 只浪费一次 k 的机会
 *
 *
 * =============================================
 * 易错点
 * =============================================
 *
 * ✗ 用小顶堆而非大顶堆 → 取出的是利润最小的, 贪心方向反了
 * ✓ priority_queue<int> 默认就是大顶堆
 *
 * ✗ 排序 capital 但没同步 profits → 索引错位, 数据对不上
 * ✓ 用索引排序 iota+sort 或 pair 排序
 *
 * ✗ ptr 每轮从 0 开始 → 应全局递增, 否则重复入堆
 * ✓ ptr 跨轮次保持, 因为资本只增不减
 *
 * ✗ 堆为空时继续取堆顶 → runtime error (UB)
 * ✓ 堆为空时 break
 *
 * ✗ 忘记排序 → 指针技巧失效, 必须每轮全量扫描
 * ✓ 必须先按 capital 排序
 *
 *
 * =============================================
 * 面试追问
 * =============================================
 *
 * Q1: 为什么贪心正确?
 *   交换论证 — profit>=0, 资本单调递增, 选利润最大不会比任何策略差
 *
 * Q2: 如果利润可以为负?
 *   贪心失效, 资本可能减少 → 需要 DP
 *
 * Q3: 如果项目有依赖关系?
 *   拓扑排序 + 贪心, 前置项目完成后才入堆
 */
