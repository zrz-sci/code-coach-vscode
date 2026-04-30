/*
 * LeetCode 368: 最大整除子集 (Largest Divisible Subset)
 *
 * 标签: Array, Math, Dynamic Programming, Sorting
 * 难度: Medium
 *
 * 三种解法:
 * 1. 排序 + DP + parent 回溯  — O(n^2) / O(n)  面试标准解法
 * 2. HashMap 约数枚举优化     — O(n * sqrt(max)) / O(n)
 * 3. 递归 + 记忆化 (自顶向下) — O(n^2) / O(n)
 */



// ============================================================
// 解法1: 排序 + DP + parent 回溯 — O(n^2) / O(n)  ⭐ 面试首选
//
// 核心思想: LIS 变种
//   排序后，整除检查变为单向: nums[i] % nums[j] (i>j)
//   dp[i] = 以 nums[i] 结尾的最长整除链长度
//   parent[i] = dp[i] 的前驱索引 (-1=无前驱)
//
// 转移方程:
//   dp[i] = max(dp[j] + 1)  对所有 j<i 且 nums[i] % nums[j] == 0
//
// 为什么排序后只检查链式整除就够？
//   整除传递性: a|b 且 b|c → a|c
//   排序后的链 a1 < a2 < ... < ak 中:
//   a1|a2, a2|a3 → a1|a3 (自动满足)
//   因此只需保证相邻元素整除，任意对都自动满足。
//
// DP 表 (nums = [1,2,3,4,8]):
//
// ┌───────┬─────┬─────┬─────┬─────┬─────┐
// │ index │  0  │  1  │  2  │  3  │  4  │
// ├───────┼─────┼─────┼─────┼─────┼─────┤
// │ nums  │  1  │  2  │  3  │  4  │  8  │
// │ dp    │  1  │  2  │  2  │  3  │  4  │
// │parent │ -1  │  0  │  0  │  1  │  3  │
// └───────┴─────┴─────┴─────┴─────┴─────┘
//
// 回溯: 4→3→1→0 → [8,4,2,1] → reverse → [1,2,4,8]
// ============================================================
class Solution_v1 {
public:
    vector<int> largestDivisibleSubset(vector<int>& nums) {
        // Step 1: 排序 — 将子集问题转化为子序列问题
        sort(nums.begin(), nums.end());
        int n = nums.size();

        // dp[i] = 以 nums[i] 结尾的最长整除链长度
        // 初始化 1: 每个元素自身是长度 1 的合法链
        vector<int> dp(n, 1);

        // parent[i] = dp[i] 从哪个索引 j 转移而来
        // -1 表示 nums[i] 是链的起点(无前驱)
        vector<int> parent(n, -1);

        // 全局最优的长度和位置
        int maxLen = 1, maxIdx = 0;

        // Step 2: DP 填表 — O(n^2) 双重循环
        for (int i = 1; i < n; i++) {
            for (int j = 0; j < i; j++) {
                // 转移条件: nums[i] 能被 nums[j] 整除
                // 且接在 j 后面能得到更长的链
                if (nums[i] % nums[j] == 0 && dp[j] + 1 > dp[i]) {
                    dp[i] = dp[j] + 1;
                    parent[i] = j;
                }
            }
            // 注意: 在外层循环更新全局最优
            // 此时 dp[i] 已确定最终值
            if (dp[i] > maxLen) {
                maxLen = dp[i];
                maxIdx = i;
            }
        }

        // Step 3: 路径回溯 — 沿 parent 链收集元素
        vector<int> result;
        for (int i = maxIdx; i != -1; i = parent[i]) {
            result.push_back(nums[i]);
        }
        // parent 链是从大到小收集的, 需要反转
        reverse(result.begin(), result.end());

        return result;
    }
};


// ============================================================
// 解法2: HashMap 约数枚举优化 — O(n * sqrt(max_val)) / O(n)
//
// 优化思路:
//   解法1 对每个 i 遍历所有 j < i → O(n) 每次
//   实际上只需检查 nums[i] 的约数中存在于数组中的那些
//   用 HashMap {值 → (链长, 前驱值)} 存储
//   枚举约数只需 O(sqrt(nums[i]))
//
// 适用: n 大但数值范围有限的场景
//   当 sqrt(max_val) < n 时，优于 O(n^2)
//
// 约数枚举技巧:
//   对 val 的约数 d, 成对出现: d 和 val/d
//   只需枚举 d 到 sqrt(val), 另一半用 val/d 得到
// ============================================================
class Solution_v2 {
public:
    vector<int> largestDivisibleSubset(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        int n = nums.size();

        // {值 → {链长度, 前驱值}}
        // 前驱值 = -1 表示无前驱
        unordered_map<int, pair<int, int>> info;

        int bestLen = 0, bestVal = -1;

        for (int i = 0; i < n; i++) {
            int val = nums[i];
            int curLen = 1, curPrev = -1;

            // 枚举 val 的所有约数
            for (int d = 1; (long long)d * d <= val; d++) {
                if (val % d != 0) continue;

                // 检查约数 d
                if (info.count(d) && info[d].first + 1 > curLen) {
                    curLen = info[d].first + 1;
                    curPrev = d;
                }

                // 检查配对约数 val/d
                int pd = val / d;
                if (pd != d && info.count(pd) && info[pd].first + 1 > curLen) {
                    curLen = info[pd].first + 1;
                    curPrev = pd;
                }
            }

            info[val] = {curLen, curPrev};

            if (curLen > bestLen) {
                bestLen = curLen;
                bestVal = val;
            }
        }

        // 沿值链回溯
        vector<int> result;
        for (int cur = bestVal; cur != -1; cur = info[cur].second) {
            result.push_back(cur);
        }
        reverse(result.begin(), result.end());

        return result;
    }
};


// ============================================================
// 解法3: 递归 + 记忆化 (自顶向下 DP)
//
// 与解法1本质相同，换成递归形式
// 有些人更习惯"从 i 出发能走多远"的思维方式
//
// 定义: dfs(i) = 从索引 i 开始, 能构成的最长整除链长度
// 转移: dfs(i) = 1 + max(dfs(j))  对所有 j>i 且 nums[j]%nums[i]==0
//
// 路径: next[i] 记录 i 的下一个元素索引, 正向收集无需反转
// ============================================================
class Solution_v3 {
public:
    vector<int> largestDivisibleSubset(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        int n = nums.size();

        vector<int> memo(n, -1);     // 记忆化: -1 表示未计算
        vector<int> next_idx(n, -1); // 下一个索引

        // 递归: 从 i 出发的最长链长度
        function<int(int)> dfs = [&](int i) -> int {
            if (memo[i] != -1) return memo[i];

            memo[i] = 1; // 至少包含自身
            for (int j = i + 1; j < n; j++) {
                if (nums[j] % nums[i] == 0) {
                    int len = 1 + dfs(j);
                    if (len > memo[i]) {
                        memo[i] = len;
                        next_idx[i] = j;
                    }
                }
            }
            return memo[i];
        };

        // 找最优起点
        int bestStart = 0;
        for (int i = 0; i < n; i++) {
            dfs(i);
            if (memo[i] > memo[bestStart]) bestStart = i;
        }

        // 正向收集 (无需反转)
        vector<int> result;
        for (int i = bestStart; i != -1; i = next_idx[i]) {
            result.push_back(nums[i]);
        }

        return result;
    }
};


// ============================================================
// 面试追问
//
// Q1: 能否用 O(n log n) 二分优化像 LIS 那样？
//   不行。整除关系不满足单调替换性质 (6 整除 2 和 3, 但 2 和 3
//   之间无整除关系), 无法维护类似 patience sorting 的 tail 数组。
//
// Q2: 如果允许重复元素怎么办？
//   题目保证 distinct。若有重复 (如 [1,1,2]), 重复元素互除 (a%a==0),
//   需特殊处理——多个相同值都可放入子集。
//
// Q3: 为什么不能用贪心？
//   贪心 (每次选最小的能整除的) 无法保证全局最优。
//   例如 [1,2,3,6,24]: 贪心走 1→2→6→24, 但 DP 能保证穷举所有路径。
// ============================================================
//
// 易错点
//
// 1. 忘记排序 — 不排序则 DP 方向不确定, 转移方程无法正确工作
// 2. dp 初始化为 0 而非 1 — 每个元素自身是合法子集 (长度 1)
//    ✗ dp[i] = 0    ✓ dp[i] = 1
// 3. 回溯路径忘记 reverse — parent 链从后往前, 输出需要从小到大
// 4. maxIdx 在内层 j 循环中更新 — 此时 dp[i] 尚未确定最终值
//    ✗ 内层更新 maxIdx   ✓ 外层更新 maxIdx
// 5. 只返回长度不返回子集 — 题目要求返回实际元素, 必须用 parent 恢复路径
// ============================================================
//
// 主解法 (提交用) — 解法1
// ============================================================
class Solution {
public:
    vector<int> largestDivisibleSubset(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        int n = nums.size();
        vector<int> dp(n, 1), parent(n, -1);
        int maxLen = 1, maxIdx = 0;

        for (int i = 1; i < n; i++) {
            for (int j = 0; j < i; j++) {
                if (nums[i] % nums[j] == 0 && dp[j] + 1 > dp[i]) {
                    dp[i] = dp[j] + 1;
                    parent[i] = j;
                }
            }
            if (dp[i] > maxLen) {
                maxLen = dp[i];
                maxIdx = i;
            }
        }

        vector<int> res;
        for (int i = maxIdx; i != -1; i = parent[i]) {
            res.push_back(nums[i]);
        }
        reverse(res.begin(), res.end());
        return res;
    }
};
