/*
 * LeetCode 740: Delete and Earn (删除并获得点数)
 *
 * 【题目本质】
 * 选 nums[i] 得 nums[i] 点，但必须删掉所有 nums[i]-1 和 nums[i]+1。
 * 关键洞察：选了 v 就应该选所有 v → 转化为 House Robber。
 *
 * 【问题转化】
 * earn[v] = v * count(v) 表示选值 v 的总收益。
 * 选 v 不能同时选 v-1 和 v+1 → 在值域上做 House Robber DP。
 * dp[v] = max(dp[v-1], dp[v-2] + earn[v])
 *
 * 【解法总览】
 * 解法1: DP 数组版            — O(n+maxVal) / O(maxVal)   — 思路最清晰
 * 解法2: 滚动变量优化          — O(n+maxVal) / O(maxVal)   — 面试首选 ⭐
 * 解法3: HashMap + 排序        — O(n log n) / O(n)         — 值域稀疏时
 */

// ============================================================
// 解法1: DP 数组版 — 思路最清晰
// 时间: O(n + maxVal)  空间: O(maxVal)
//
// 【思路】
// Step 1: 统计 earn[v] = v * count(v)
// Step 2: 在值域 [1, maxVal] 上做 House Robber DP
//   dp[v] = max(
//     dp[v-1],               // 不选 v, 继承前面的最优解
//     dp[v-2] + earn[v]      // 选 v, 跳过 v-1
//   )
//
// DP 表推演:
// 示例1: nums = [2,2,3,3,3,4]
//   earn = [0, 0, 4, 9, 4]
//
//   v | earn[v] | dp[v-2] | dp[v-1] | dp[v] = max(dp[v-1], dp[v-2]+earn[v])
//   --|---------|---------|---------|------
//   1 |    0    |    0    |    0    | max(0, 0+0) = 0
//   2 |    4    |    0    |    0    | max(0, 0+4) = 4
//   3 |    9    |    0    |    4    | max(4, 0+9) = 9   ← 选3更好
//   4 |    4    |    4    |    9    | max(9, 4+4) = 9   ← 不选4
//
//   答案: dp[4] = 9 ✓ (选了所有的3: 3*3=9)
//
// 示例2: nums = [3,4,2]
//   earn = [0, 0, 2, 3, 4]
//
//   v | earn[v] | dp[v-2] | dp[v-1] | dp[v]
//   --|---------|---------|---------|------
//   1 |    0    |    0    |    0    | 0
//   2 |    2    |    0    |    0    | max(0, 0+2) = 2
//   3 |    3    |    0    |    2    | max(2, 0+3) = 3
//   4 |    4    |    2    |    3    | max(3, 2+4) = 6   ← 选4(4分)+选2(2分)
//
//   答案: dp[4] = 6 ✓ (选4删3, 再选2, 共6分)
// ============================================================
class Solution1 {
public:
    int deleteAndEarn(vector<int>& nums) {
        int maxVal = *max_element(nums.begin(), nums.end());

        // Step 1: 统计每个值的总收益
        // earn[v] = v * count(v)
        // 等效写法: 每遇到一个 x, earn[x] += x
        vector<int> earn(maxVal + 1, 0);
        for (int x : nums) {
            earn[x] += x;
        }

        // Step 2: House Robber DP
        vector<int> dp(maxVal + 1, 0);
        dp[1] = earn[1]; // base case
        for (int v = 2; v <= maxVal; v++) {
            dp[v] = max(dp[v - 1],             // 不选 v
                        dp[v - 2] + earn[v]);   // 选 v
        }

        return dp[maxVal];
    }
};

// ============================================================
// 解法2: 滚动变量优化 — 面试首选 ⭐
// 时间: O(n + maxVal)  空间: O(maxVal) [earn数组]
//
// 【空间优化】
// dp[v] 只依赖 dp[v-1] 和 dp[v-2], 用两个变量滚动代替数组。
//
// 优化前: dp = [0, 0, 4, 9, 9]        → O(maxVal) 数组
// 优化后: prev2, prev1, curr           → O(1) 变量
//
// 滚动过程 (nums = [2,2,3,3,3,4]):
//   初始: prev2=0, prev1=0
//   v=1: curr=max(0, 0+0)=0    → prev2=0, prev1=0
//   v=2: curr=max(0, 0+4)=4    → prev2=0, prev1=4
//   v=3: curr=max(4, 0+9)=9    → prev2=4, prev1=9
//   v=4: curr=max(9, 4+4)=9    → prev2=9, prev1=9
//   答案: prev1 = 9 ✓
//
// 注意: earn 数组的 O(maxVal) 空间仍然需要 (除非用 HashMap)
// ============================================================
class Solution2 {
public:
    int deleteAndEarn(vector<int>& nums) {
        int maxVal = *max_element(nums.begin(), nums.end());

        // Step 1: 统计 earn
        vector<int> earn(maxVal + 1, 0);
        for (int x : nums) {
            earn[x] += x;
        }

        // Step 2: House Robber with rolling variables
        int prev2 = 0, prev1 = 0;
        for (int v = 1; v <= maxVal; v++) {
            int curr = max(prev1, prev2 + earn[v]);
            prev2 = prev1;
            prev1 = curr;
        }

        return prev1;
    }
};

// ============================================================
// 解法3: HashMap + 排序 — 值域稀疏时更优
// 时间: O(n log n)  空间: O(n)
//
// 【思路】
// 当 maxVal 远大于 n 时 (如 nums=[1, 10000]), 解法1/2 的
// O(maxVal) 会浪费。用 HashMap 只存出现过的值, 排序后 DP。
//
// 关键区别: 不相邻的值之间没有冲突, 可以同时选。
//   相邻 key (差1): 做 House Robber 决策
//   不相邻 key (差>1): 直接累加, 没有冲突
//
// 示例: nums = [2,2,3,3,3,4]
//   earn = {2:4, 3:9, 4:4}
//   keys = [2, 3, 4] (已排序)
//
//   key=2: 和前一个不相邻(lastKey=-2) → curr = 0 + 4 = 4
//   key=3: 和 key=2 相邻 → curr = max(4, 0+9) = 9
//   key=4: 和 key=3 相邻 → curr = max(9, 4+4) = 9
//   答案: 9 ✓
// ============================================================
class Solution3 {
public:
    int deleteAndEarn(vector<int>& nums) {
        // 用有序 map 同时完成统计和排序
        map<int, int> earn;
        for (int x : nums) {
            earn[x] += x;
        }

        int prev2 = 0, prev1 = 0;
        int lastKey = -2; // 上一个处理的 key

        for (auto& [key, val] : earn) {
            int curr;
            if (key == lastKey + 1) {
                // 和上一个值相邻 → House Robber 决策
                curr = max(prev1, prev2 + val);
            } else {
                // 和上一个值不相邻 → 无冲突, 直接选
                curr = prev1 + val;
            }
            prev2 = prev1;
            prev1 = curr;
            lastKey = key;
        }

        return prev1;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法          | 时间          | 空间       | 适用场景           |
// |---------------|--------------|-----------|-------------------|
// | DP数组版      | O(n+maxVal)  | O(maxVal) | 通用              |
// | 滚动变量优化   | O(n+maxVal)  | O(maxVal) | 面试首选 ⭐        |
// | HashMap+排序  | O(n log n)   | O(n)      | 值域稀疏/maxVal很大 |
//
// maxVal <= 10^4 时, 解法1/2 的 O(maxVal) 完全可接受。
// 如果 maxVal 可达 10^9 (面试追问), 必须用解法3。
//
// ============================================================
// 【DP 空间优化推演】
//
// 原始 DP 数组:
//   dp[0] = 0
//   dp[1] = earn[1]
//   dp[2] = max(dp[1], dp[0] + earn[2])
//   dp[3] = max(dp[2], dp[1] + earn[3])
//   ...
//
// 观察依赖: dp[v] 只依赖 dp[v-1] 和 dp[v-2]
//
// 优化: 用 prev2 = dp[v-2], prev1 = dp[v-1] 两个变量
//
//   ┌─────────┐     ┌─────────┐     ┌──────┐
//   │ prev2   │ ──→ │ prev1   │ ──→ │ curr │
//   │(dp[v-2])│     │(dp[v-1])│     │(dp[v])│
//   └─────────┘     └─────────┘     └──────┘
//
// 每步更新:
//   curr = max(prev1, prev2 + earn[v])
//   prev2 = prev1
//   prev1 = curr
//
// 这个滚动技巧是所有 "只依赖前两个状态" 的 DP 的通用优化:
// Fibonacci、House Robber、Climbing Stairs 都用同样的手法。
//
// ============================================================
// 【易错点】
//
// 1. earn[v] 计算错误:
//    ✗ earn[x] = x       → 只记了值, 没有累加出现次数
//    ✗ earn[x]++          → 记了次数但不是总点数
//    ✓ earn[x] += x       → 每出现一次加 x, 等效于 x * count(x)
//
// 2. DP 遍历范围:
//    ✗ for (int i = 0; i < n; i++)  → 在原数组上做 DP, 不对!
//    ✓ for (int v = 1; v <= maxVal; v++) → 在值域上做 DP
//
// 3. HashMap 版忘记处理不相邻:
//    ✗ 所有 key 都用 max(prev1, prev2+earn[key])
//      → 不相邻的值无冲突, 应该直接累加!
//    ✓ 判断 key == lastKey+1 决定是冲突还是无冲突
//
// 4. 空数组/max_element 的 UB:
//    ✗ *max_element(nums.begin(), nums.end()) 对空数组是 UB
//    题目保证 nums.length >= 1, 但面试时提一句边界检查
//
// ============================================================
// 【面试追问 — 递进链】
//
// Q1: 为什么等价于 House Robber?
//     选 v 不能选 v-1/v+1 = 不能选相邻房子
//     earn[v] = 房子 v 的财宝值
//
// Q2: 如果 maxVal 很大 (如 10^9)?
//     HashMap + 排序, 只处理出现过的值, O(n log n)
//
// Q3: 选 v 后只删 v-1 (不删 v+1)?
//     限制变弱, 排序后贪心从大到小选
//
// Q4: 值可以为负?
//     负收益的值永远不选。DP 中 max 自然排除。
//
// Q5: House Robber 环形变体?
//     LeetCode 213: 分两次 DP (去掉首 / 去掉尾), 取 max。
//
// ============================================================
// 【相关题型】
//
// - 198. 打家劫舍 (House Robber):
//   本题的直接前置。完全相同的状态转移:
//   dp[i] = max(dp[i-1], dp[i-2] + nums[i])
//   理解 198 后本题只需加一层 "问题转化"。
//
// - 213. 打家劫舍 II:
//   环形 House Robber。如果本题值域是环形 (最大值和1相邻),
//   就需要这个技巧: 分两次 DP, 分别去掉首和尾。
//
// - 309. 最佳买卖股票时机含冷冻期:
//   也是 "选了当前就不能选相邻" 的 DP 模式。
//   买了之后有冷冻期, 和 "选 v 不能选 v-1" 类似。
//
// - 1388. 3n 块比萨:
//   更复杂的 "不能选相邻" DP。环形 + 只选 n 个。
//   核心仍是 House Robber 扩展。
// ============================================================
