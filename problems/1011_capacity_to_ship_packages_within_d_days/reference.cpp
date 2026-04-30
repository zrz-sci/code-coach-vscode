/*
 * LeetCode 1011: 在 D 天内送达包裹的能力
 *
 * 【题目本质】
 * 在连续答案空间（运载能力）上，找满足"D 天内送完"的最小值。
 * 运载能力越大 → 需要天数越少，具有单调性 → 二分答案。
 *
 * 【解法总览】
 * 解法1: 线性枚举       — O(n × S) / O(1) — 最直觉，但超时
 * 解法2: 二分答案+贪心   — O(n × log S) / O(1) — 最优，面试必会
 *
 * 其中 S = sum(weights) - max(weights)，即答案空间大小
 */

// ============================================================
// 解法1: 线性枚举 — 从最小可能运载能力逐个尝试
// 时间: O(n × S)  空间: O(1)
//
// 【思路】
// 运载能力最小是 max(weights)（否则最重包裹装不上），
// 最大是 sum(weights)（一天全装完）。
// 从 max 开始逐个试，第一个能在 days 天内送完的就是答案。
//
// 这个解法的价值在于：验证函数 canShip() 在所有解法中复用。
// 瓶颈：答案空间可达 2.5×10^7，逐个试太慢。
// ============================================================
class Solution1 {
public:
    int shipWithinDays(vector<int>& weights, int days) {
        int maxW = *max_element(weights.begin(), weights.end());
        int sumW = accumulate(weights.begin(), weights.end(), 0);

        for (int cap = maxW; cap <= sumW; cap++) {
            if (canShip(weights, days, cap)) {
                return cap;
            }
        }
        return sumW;
    }

private:
    // 贪心验证：给定运载能力 cap，能否在 days 天内送完？
    // 策略：从左到右扫，当前天能装就装，装不下就开新天
    // 为什么贪心正确？包裹顺序固定，提前结束某天只会让后面需要更多天
    bool canShip(vector<int>& weights, int days, int cap) {
        int needDays = 1;   // 至少需要1天（不是0！）
        int curLoad = 0;    // 当前这一天已装重量

        for (int w : weights) {
            if (curLoad + w > cap) {
                // 装不下了，开新的一天
                needDays++;
                curLoad = 0;
            }
            curLoad += w;
        }
        return needDays <= days;
    }
};

// ============================================================
// 解法2: 二分答案 + 贪心验证 — 面试首选 ⭐
// 时间: O(n × log S)  空间: O(1)
//
// 【思路】
// 关键观察：运载能力 cap ↑ → 需要天数 ↓（单调性）
// 这意味着答案空间可以二分！
//
// 搜索空间: [max(weights), sum(weights)]
// 验证条件: canShip(weights, days, mid) → 是否 ≤ days 天
// 目标: 找满足条件的最小 cap（左边界二分）
//
// 二分搜索空间收缩过程 (示例1: weights=[1..10], days=5):
//
//   答案空间: [10 ................................ 55]
//
//   Round1: left=10, right=55, mid=32
//           needDays=2 ≤ 5 ✓ → right=32
//           [10 .................. 32]
//
//   Round2: left=10, right=32, mid=21
//           needDays=3 ≤ 5 ✓ → right=21
//           [10 .......... 21]
//
//   Round3: left=10, right=21, mid=15
//           needDays=5 ≤ 5 ✓ → right=15
//           [10 .... 15]
//
//   Round4: left=10, right=15, mid=12
//           needDays=7 > 5  ✗ → left=13
//              [13 . 15]
//
//   Round5: left=13, right=15, mid=14
//           needDays=6 > 5  ✗ → left=15
//                 [15]
//
//   left == right == 15 → 答案!
// ============================================================
class Solution2 {
public:
    int shipWithinDays(vector<int>& weights, int days) {
        // 下界：至少能装下最重的一个包裹
        // 如果 cap < max(weights)，那个包裹永远装不上船
        int left = *max_element(weights.begin(), weights.end());

        // 上界：一天全送完，运载能力 = 总重量
        int right = accumulate(weights.begin(), weights.end(), 0);

        // 左边界二分：找满足 canShip 的最小 cap
        while (left < right) {
            int mid = left + (right - left) / 2;

            if (canShip(weights, days, mid)) {
                // mid 可行，但不一定是最小的
                // 保留 mid 在范围内（right = mid 而非 mid-1）
                right = mid;
            } else {
                // mid 不可行（天数超了），需要更大的运载能力
                // mid 已确认不可行，可以排除（left = mid+1）
                left = mid + 1;
            }
        }
        // 循环结束: left == right，即最小可行运载能力
        return left;
    }

private:
    // 和解法1完全相同的贪心验证函数
    bool canShip(vector<int>& weights, int days, int cap) {
        int needDays = 1;
        int curLoad = 0;

        for (int w : weights) {
            if (curLoad + w > cap) {
                needDays++;
                curLoad = 0;
            }
            curLoad += w;
        }
        return needDays <= days;
    }
};

// ============================================================
// 【解法对比】
//
// | 维度     | 解法1 线性枚举    | 解法2 二分答案     |
// |----------|-------------------|--------------------|
// | 时间     | O(n × S) ≈ 10^12 | O(n × logS) ≈ 10^6|
// | 空间     | O(1)              | O(1)               |
// | 验证函数 | 相同              | 相同               |
// | 核心差异 | 逐个遍历答案空间  | 二分跳跃答案空间   |
//
// 两种解法唯一的区别是"如何遍历答案空间"。
// 验证函数（贪心模拟装船）完全一样。
// 面试必须写解法2。
//
// ============================================================
// 【易错点】
//
// 1. 下界写错:
//    ✗ int left = 1;
//    ✓ int left = *max_element(weights.begin(), weights.end());
//    原因：cap 小于最重包裹时，那个包裹永远装不上，canShip 永远 false
//
// 2. needDays 初始化为 0:
//    ✗ int needDays = 0;  → 最后一批货的那天没计入
//    ✓ int needDays = 1;  → 开始就是第1天，每次"装不下"才 ++
//
// 3. right = mid - 1:
//    ✗ right = mid - 1;  → mid 可能就是答案，跳过了
//    ✓ right = mid;      → 保留 mid，因为它可能是最小可行值
//    配合 while(left < right) 使用，不会死循环
//
// 4. 开新天后忘记重置 curLoad:
//    ✗ needDays++; curLoad += w;  → curLoad 累加了上一天的
//    ✓ needDays++; curLoad = 0; curLoad += w;
//    （或简写为 curLoad = w）
//
// 5. canShip 中没处理单个包裹超重:
//    其实不需要特判，因为 left 的下界已经 >= max(weights)，
//    二分过程中 mid 永远 >= max(weights)，不会出现单包超重。
//    但如果下界写错了（比如 left=1），这里就会出 bug。
//
// ============================================================
// 【面试追问】
//
// Q1: "为什么验证函数用贪心是对的？不需要 DP 最优分配？"
//     包裹顺序固定，每天是连续子数组。"能装就装"是最优的——
//     提前结束某天（少装），只会让后面需要更多天。
//
// Q2: "这道题和 LeetCode 410 有什么关系？"
//     完全等价！410 是"将数组分成 K 段，最小化最大段和"。
//     本题的 days=K，运载能力=最大段和。代码一模一样。
//
// Q3: "如果包裹可以重排顺序呢？"
//     变成 Bin Packing（装箱问题），NP-hard，不能简单贪心。
//     近似算法：排序后用 First Fit Decreasing 等启发式方法。
//
// Q4: "如果运载能力是浮点数（比如最小化距离），怎么改？"
//     二分条件改为 while(right - left > 1e-6)，
//     mid 用浮点除法，其余逻辑不变。参考 LeetCode 774。
//
// Q5: "能否用 DP 做？复杂度是多少？"
//     dp[i][j] = 前 i 个包裹分 j 天的最小运载能力。
//     转移枚举分割点: dp[i][j] = min over k { max(dp[k][j-1], sum[k+1..i]) }
//     时间 O(n^2 × days)，比二分慢，但也是正确解法。
// ============================================================
