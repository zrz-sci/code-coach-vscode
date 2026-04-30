/*
 * LeetCode 826: Most Profit Assigning Work (最大利润分配工作)
 *
 * 【题目本质】
 * 每个工人独立选择一个自己能胜任（难度 <= 能力）且利润最高的工作。
 * 多个工人可以选同一个工作。求总利润最大值。
 *
 * 【解法总览】
 * 解法1: 排序 + 双指针         — O(n log n + m log m) / O(n) — 面试首选 ⭐
 * 解法2: 排序 + 二分查找       — O((n+m) log n) / O(n) — 在线查询场景
 * 解法3: 桶映射 + 前缀最大值   — O(n + m + D) / O(D) — 值域小时最优
 */

// ============================================================
// 解法1: 排序 + 双指针 — 面试首选 ⭐
// 时间: O(n log n + m log m)  空间: O(n)
//
// 【思路】
// 核心观察：利润不一定随难度递增，所以不能简单取"难度最接近的工作"。
// 需要的是"难度 ≤ w 的所有工作中利润最大的"。
//
// 将工作按难度排序，工人按能力排序后，双指针同步扫描：
// - 工人能力递增，工作难度指针只前进
// - 维护扫过的工作中的最大利润 maxP
// - 每个工人的贡献就是当前的 maxP
//
// 示例1: difficulty=[2,4,6,8,10], profit=[10,20,30,40,50], worker=[4,5,6,7]
//
// 排序后 jobs: [(2,10),(4,20),(6,30),(8,40),(10,50)]
// 排序后 worker: [4, 5, 6, 7]
//
// 双指针扫描:
//   w=4: j扫到 {2,10}→maxP=10, {4,20}→maxP=20, stop(6>4) → +20
//   w=5: j不动(6>5) → +20
//   w=6: j扫到 {6,30}→maxP=30, stop(8>6) → +30
//   w=7: j不动(8>7) → +30
//   总利润 = 20+20+30+30 = 100 ✓
// ============================================================
class Solution1 {
public:
    int maxProfitAssignment(vector<int>& difficulty, vector<int>& profit, vector<int>& worker) {
        int n = difficulty.size();

        // 将 (difficulty, profit) 配对后按难度升序排序
        vector<pair<int,int>> jobs(n);
        for (int i = 0; i < n; i++) {
            jobs[i] = {difficulty[i], profit[i]};
        }
        sort(jobs.begin(), jobs.end());

        // 工人按能力升序排序
        sort(worker.begin(), worker.end());

        int totalProfit = 0;
        int j = 0;      // 工作指针，只前进不后退
        int maxP = 0;   // 难度 ≤ 当前工人能力 的所有工作中最大利润

        for (int w : worker) {
            // 将所有难度 ≤ w 的工作纳入考虑
            while (j < n && jobs[j].first <= w) {
                maxP = max(maxP, jobs[j].second);
                j++;
            }
            // 这个工人的最优选择就是 maxP
            // 如果 j 一步没动（所有工作都太难），maxP 保持上一个值或 0
            totalProfit += maxP;
        }

        return totalProfit;
    }
};

// ============================================================
// 解法2: 排序 + 二分查找
// 时间: O((n+m) log n)  空间: O(n)
//
// 【思路】
// 不排序 worker。对工作按难度排序后，构建前缀最大利润数组。
// 对每个工人，二分查找"难度 ≤ worker[j]"的最后一个位置，
// 直接查表得到该范围内的最大利润。
//
// 适用场景：工人是在线到达的（不能预排序），每来一个就查一次。
//
// 前缀最大利润构建:
//   jobs sorted: [(2,10), (4,20), (6,30), (8,40), (10,50)]
//   prefixMax:   [10,     20,     30,     40,     50]
//
// 对 worker=5: 二分找 difficulty<=5 → 位置1 → prefixMax[1]=20
// 对 worker=7: 二分找 difficulty<=7 → 位置2 → prefixMax[2]=30
// ============================================================
class Solution2 {
public:
    int maxProfitAssignment(vector<int>& difficulty, vector<int>& profit, vector<int>& worker) {
        int n = difficulty.size();

        vector<pair<int,int>> jobs(n);
        for (int i = 0; i < n; i++) {
            jobs[i] = {difficulty[i], profit[i]};
        }
        sort(jobs.begin(), jobs.end());

        // 构建前缀最大利润数组
        // prefixMax[i] = max(jobs[0..i].profit)
        vector<int> prefixMax(n);
        prefixMax[0] = jobs[0].second;
        for (int i = 1; i < n; i++) {
            prefixMax[i] = max(prefixMax[i - 1], jobs[i].second);
        }

        int totalProfit = 0;

        for (int w : worker) {
            // 二分查找：最后一个 difficulty <= w 的位置
            int lo = 0, hi = n - 1, pos = -1;
            while (lo <= hi) {
                int mid = lo + (hi - lo) / 2;
                if (jobs[mid].first <= w) {
                    pos = mid;
                    lo = mid + 1;
                } else {
                    hi = mid - 1;
                }
            }

            // pos == -1 表示没有任何工作难度 <= w，利润为 0
            if (pos >= 0) {
                totalProfit += prefixMax[pos];
            }
        }

        return totalProfit;
    }
};

// ============================================================
// 解法3: 桶映射 + 前缀最大值
// 时间: O(n + m + D)  空间: O(D)，D = max(difficulty, worker)
//
// 【思路】
// 当值域 D 不大时（本题 ≤ 10^5），直接用数组做桶：
//   bucket[d] = 难度恰好为 d 的所有工作中最大利润
// 然后做前缀 max：
//   bucket[d] = max(bucket[d], bucket[d-1])
// 查询时 worker[j] 的最大利润就是 bucket[worker[j]]
//
// 优点：避免排序，线性时间
// 限制：值域太大时空间浪费
// ============================================================
class Solution3 {
public:
    int maxProfitAssignment(vector<int>& difficulty, vector<int>& profit, vector<int>& worker) {
        // 找到值域上界（需要覆盖 worker 的最大值）
        int maxD = 0;
        for (int d : difficulty) maxD = max(maxD, d);
        for (int w : worker) maxD = max(maxD, w);

        // 桶：每个难度值对应的最大利润
        vector<int> bucket(maxD + 1, 0);
        for (int i = 0; i < (int)difficulty.size(); i++) {
            bucket[difficulty[i]] = max(bucket[difficulty[i]], profit[i]);
        }

        // 前缀最大值：bucket[d] 表示 难度 ≤ d 的最大利润
        for (int d = 1; d <= maxD; d++) {
            bucket[d] = max(bucket[d], bucket[d - 1]);
        }

        // 每个工人直接查表
        int totalProfit = 0;
        for (int w : worker) {
            totalProfit += bucket[w];
        }
        return totalProfit;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法 | 时间 | 空间 | 适用场景 |
// |------|------|------|----------|
// | 双指针 | O(n log n + m log m) | O(n) | 面试首选，最直观 |
// | 二分  | O((n+m) log n) | O(n) | 工人在线到达 |
// | 桶映射 | O(n + m + D) | O(D) | 值域小，追求线性 |
//
// 【易错点】
//
// 1. 利润不随难度单调递增:
//    ✗ 取难度最接近的工作的利润
//    ✓ 取难度 ≤ w 的所有工作中的最大利润（前缀 max）
//
// 2. 双指针中 maxP 不应重置:
//    ✗ 每个工人开始时 maxP = 0
//    ✓ maxP 是累计的，因为 worker 递增，之前能做的工作现在也能做
//
// 3. 二分查找边界:
//    ✗ 忘记处理 pos == -1 的情况（工人能力低于所有工作难度）
//    ✓ pos == -1 时利润为 0，不要访问 prefixMax[-1]
//
// 4. 桶映射忘记覆盖 worker 最大值:
//    ✗ maxD = max(difficulty)，但 worker 中有更大的值时越界
//    ✓ maxD = max(max(difficulty), max(worker))
//
// 【面试追问】
//
// Q1: "如果每个工作只能分配给一个工人？"
//   → 变成贪心 + 优先队列：按工人能力从大到小排序，每次从可做工作中
//     取利润最高的。用 max-heap 维护可用工作，已分配的 pop 掉。
//
// Q2: "如果工人到达是在线的，能否 O(log n) 单次查询？"
//   → 用解法2：预处理排序 + 前缀 max 后，每次二分查找 O(log n)。
//
// Q3: "值域很大（10^9）时桶排序还适用吗？"
//   → 不适用，空间 O(D) 太大。此时用排序 + 二分或双指针。
// ============================================================
