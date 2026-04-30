/*
 * LeetCode 494: 目标和 (Target Sum)
 *
 * 【题目本质】
 * 每个数前加 + 或 -，本质是将数组分成两个子集 P 和 Q，
 * 使得 P - Q = target。通过 P + Q = sum 推出 P = (sum + target) / 2，
 * 转化为经典 0/1 背包计数问题：从 nums 中选若干数使和恰好为 P 的方案数。
 *
 * 【解法总览】
 * 解法1: 回溯 DFS          — O(2^n) / O(n)          — 最直觉
 * 解法2: 记忆化搜索         — O(n × sum) / O(n × sum) — 过渡理解
 * 解法3: 0/1 背包 DP(二维)  — O(n × bagSize) / O(n × bagSize) — 理解填表
 * 解法4: 0/1 背包 DP(一维)  — O(n × bagSize) / O(bagSize)     — 面试首选
 */

// ============================================================
// 解法1: 回溯 DFS — 枚举每个数选 + 或 -
// 时间: O(2^n)  空间: O(n) 递归栈深度
//
// 【思路】
// 最直觉的做法：每个数字有两种选择，递归枚举所有 2^n 种组合。
// n=20 时 2^20 ≈ 10^6，刚好能过，但没有利用重叠子问题。
//
// 决策树 (nums = [1,1,1,1,1], target = 3):
//                          remain=3
//                    -1 /          \ +1 (减去nums[0]或加上)
//                 remain=2        remain=4
//              -1 /    \ +1    -1 /    \ +1
//           remain=1  remain=3  ...    ...
//           ...       (多条路径到达相同的 (i, remain))
// ============================================================
class Solution1 {
public:
    int findTargetSumWays(vector<int>& nums, int target) {
        return dfs(nums, 0, target);
    }

private:
    int dfs(vector<int>& nums, int i, int remain) {
        // 所有数字都已分配符号
        if (i == nums.size()) {
            return remain == 0 ? 1 : 0;
        }
        // 选 + 号: 消耗 nums[i]，remain 减小
        // 选 - 号: 增加 nums[i]，remain 增大
        return dfs(nums, i + 1, remain - nums[i])
             + dfs(nums, i + 1, remain + nums[i]);
    }
};

// ============================================================
// 解法2: 记忆化搜索 — DFS + memo 消除重复计算
// 时间: O(n × totalSum)  空间: O(n × totalSum)
//
// 【思路】
// 解法1中不同路径会到达相同的 (i, remain) 状态。
// 例如 nums=[1,1,...], 走 +1-1 和 -1+1 后 remain 相同。
// 用 memo[(i, remain)] 缓存结果，避免重复递归。
// remain 可正可负，范围 [-sum, sum]，用 map 方便处理。
// ============================================================
class Solution2 {
public:
    int findTargetSumWays(vector<int>& nums, int target) {
        unordered_map<int, unordered_map<int, int>> memo;
        return dfs(nums, 0, target, memo);
    }

private:
    int dfs(vector<int>& nums, int i, int remain,
            unordered_map<int, unordered_map<int, int>>& memo) {
        if (i == nums.size()) {
            return remain == 0 ? 1 : 0;
        }
        // 查缓存
        if (memo.count(i) && memo[i].count(remain)) {
            return memo[i][remain];
        }
        int result = dfs(nums, i + 1, remain - nums[i], memo)
                   + dfs(nums, i + 1, remain + nums[i], memo);
        memo[i][remain] = result; // 存缓存
        return result;
    }
};

// ============================================================
// 解法3: 0/1 背包 DP (二维) — 数学变换 + 标准填表
// 时间: O(n × bagSize)  空间: O(n × bagSize)
//
// 【思路】
// 数学变换:
//   P - Q = target, P + Q = sum → P = (sum + target) / 2
//   问题变为: 从 nums 选若干数使和 = bagSize 的方案数
//
// 状态定义: dp[i][j] = 用前 i 个数凑出和为 j 的方案数
// 转移方程: dp[i][j] = dp[i-1][j]                      (不选第i个)
//                     + dp[i-1][j-nums[i-1]]            (选第i个, 需 j >= nums[i-1])
// 初始化:   dp[0][0] = 1 (空集和为0, 1种方案)
// 遍历方向: i 从 1 到 n, j 从 0 到 bagSize (依赖上一行)
//
// 填表过程 (nums = [1,1,1,1,1], bagSize = 4):
//        j: 0  1  2  3  4
// i=0:      1  0  0  0  0
// i=1(1):   1  1  0  0  0    dp[1][1] = dp[0][0] = 1
// i=2(1):   1  2  1  0  0    dp[2][2] = dp[1][1] + dp[1][2] = 1
// i=3(1):   1  3  3  1  0    dp[3][3] = dp[2][2] + dp[2][3] = 3
// i=4(1):   1  4  6  4  1    dp[4][4] = dp[3][3] + dp[3][4] = 4 (不对?)
//                             实际 dp[4][3] = dp[3][3]+dp[3][2] = 1+3 = 4
// i=5(1):   1  5 10 10  5    dp[5][4] = dp[4][4]+dp[4][3] = 1+4 = 5 ✓
// ============================================================
class Solution3 {
public:
    int findTargetSumWays(vector<int>& nums, int target) {
        int sum = 0;
        for (int num : nums) sum += num;

        // 边界: bagSize 必须非负且为整数
        if ((sum + target) < 0 || (sum + target) % 2 != 0) return 0;

        int bagSize = (sum + target) / 2;
        int n = nums.size();

        vector<vector<int>> dp(n + 1, vector<int>(bagSize + 1, 0));
        dp[0][0] = 1; // 空集和为0 → 1种方案

        for (int i = 1; i <= n; i++) {
            for (int j = 0; j <= bagSize; j++) {
                // 不选第 i 个数
                dp[i][j] = dp[i - 1][j];
                // 选第 i 个数 (前提: 容量够)
                if (j >= nums[i - 1]) {
                    dp[i][j] += dp[i - 1][j - nums[i - 1]];
                }
            }
        }
        return dp[n][bagSize];
    }
};

// ============================================================
// 解法4: 0/1 背包 DP (一维空间优化) — 面试首选 ⭐
// 时间: O(n × bagSize)  空间: O(bagSize)
//
// 【思路】
// 解法3中 dp[i][j] 只依赖 dp[i-1][j] 和 dp[i-1][j-num]，
// 即只依赖上一行。用一维数组替代，但必须倒序遍历 j。
//
// 为什么倒序？
// 正序时 dp[j-num] 已在本轮被更新，包含了"选了当前 num"的结果，
// 导致 num 被重复选取（变成完全背包）。
// 倒序时 dp[j-num] 还是上一轮的值，保证每个 num 只选一次。
//
// 一维滚动过程 (nums = [1,1,1,1,1], bagSize = 4):
//   初始:    [1, 0, 0, 0, 0]
//   num=1:   j=4: dp[4]+=dp[3]=0  j=3: dp[3]+=dp[2]=0
//            j=2: dp[2]+=dp[1]=0  j=1: dp[1]+=dp[0]=1
//            → [1, 1, 0, 0, 0]
//   num=1:   j=4: dp[4]+=dp[3]=0  j=3: dp[3]+=dp[2]=0
//            j=2: dp[2]+=dp[1]=1  j=1: dp[1]+=dp[0]=1
//            → [1, 2, 1, 0, 0]
//   num=1:   → [1, 3, 3, 1, 0]
//   num=1:   → [1, 4, 6, 4, 1]
//   num=1:   → [1, 5, 10, 10, 5]
//   dp[4] = 5 ✓
// ============================================================
class Solution4 {
public:
    int findTargetSumWays(vector<int>& nums, int target) {
        int sum = 0;
        for (int num : nums) sum += num;

        // 关键边界判断:
        // 1. sum + target < 0: target 太小(负数), 不可能凑出
        // 2. (sum + target) % 2 != 0: 无法整除, 不存在整数解
        if ((sum + target) < 0 || (sum + target) % 2 != 0) return 0;

        int bagSize = (sum + target) / 2;

        // dp[j] = 从已考虑的数中选若干个, 和恰好为 j 的方案数
        vector<int> dp(bagSize + 1, 0);
        dp[0] = 1; // 和为 0 → 空集 → 1 种方案

        for (int num : nums) {
            // 0/1 背包: 倒序遍历, 保证每个 num 只用一次
            for (int j = bagSize; j >= num; j--) {
                // 不选 num: dp[j] 保持不变 (隐含)
                // 选 num:   dp[j] += dp[j - num]
                dp[j] += dp[j - num];
            }
        }
        return dp[bagSize];
    }
};

// ============================================================
// 【解法对比】
//
// | 解法     | 时间          | 空间          | 适用场景        |
// |----------|---------------|---------------|-----------------|
// | DFS      | O(2^n)        | O(n)          | n很小, 快速验证  |
// | 记忆化   | O(n×sum)      | O(n×sum)      | 过渡理解         |
// | 二维DP   | O(n×bagSize)  | O(n×bagSize)  | 理解填表过程      |
// | 一维DP   | O(n×bagSize)  | O(bagSize)    | 面试首选 ⭐      |
//
// 核心演进:
// DFS → 记忆化: 加缓存消除重复子问题
// 记忆化 → DP: 数学变换 (P=(sum+target)/2) 简化状态空间
// 二维DP → 一维DP: dp[i] 只依赖 dp[i-1], 滚动数组优化
//
// ============================================================
// 【易错点】
//
// 1. 漏判 (sum + target) < 0:
//    ✗ if ((sum + target) % 2 != 0) return 0;
//    ✓ if ((sum + target) < 0 || (sum + target) % 2 != 0) return 0;
//    当 target = -2, nums = [1] 时, sum + target = -1, bagSize 为负导致 UB。
//
// 2. 一维 DP 正序遍历:
//    ✗ for (int j = num; j <= bagSize; j++)   // 完全背包!
//    ✓ for (int j = bagSize; j >= num; j--)   // 0/1 背包
//    正序导致同一个 num 被多次选取。
//
// 3. dp[0] 初始化为 0:
//    ✗ dp[0] = 0  → 所有 dp 值永远为 0
//    ✓ dp[0] = 1  → 空集和为 0 是一种合法方案
//
// 4. 二维 DP 中 j 从 nums[i-1] 开始:
//    ✗ for (int j = nums[i-1]; j <= bagSize; j++)
//       漏掉 j < nums[i-1] 时的 dp[i][j] = dp[i-1][j]
//    ✓ for (int j = 0; j <= bagSize; j++)
//       不选的情况对所有 j 都要处理
//
// ============================================================
// 【面试追问 — 递进链】
//
// Q1: 暴力 DFS 时间多少？能通过吗？
//     → O(2^n), n=20 时 ~10^6 可以。但 n=40 就不行 (10^12)。
//
// Q2: 怎么优化？为什么有重叠子问题？
//     → 不同符号选择路径到达同一 (i, remain)。记忆化缓存。
//
// Q3: 能否进一步简化状态空间？
//     → 数学变换: P = (sum+target)/2, 转为 0/1 背包。
//        remain 从 [-sum, sum] 缩减到 [0, bagSize]。
//
// Q4: 一维 DP 为什么倒序？正序会变成什么？
//     → 正序 = 完全背包 (物品可重复选)。倒序保证每个物品只选一次。
//
// Q5: 如果 nums 有大量 0 呢？
//     → 每个 0 选 +/- 不影响和, 但方案数翻倍。
//        DP 自然处理: dp[j] += dp[j-0] = dp[j], 即翻倍。
//
// ============================================================
// 【相关题型】
//
// 416. 分割等和子集:
//   → 同样的 0/1 背包, dp 类型从 int(方案数) 变成 bool(能否凑出)
//   → dp[j] += dp[j-num] 变成 dp[j] = dp[j] || dp[j-num]
//
// 518. 零钱兑换 II:
//   → 完全背包计数, 唯一区别: j 正序遍历 (硬币可重复用)
//   → 对比本题一维 DP: 仅遍历方向不同 (正序 vs 倒序)
//
// 474. 一和零:
//   → 二维 0/1 背包 (两个容量: 0的个数和1的个数)
//
// 1049. 最后一块石头的重量 II:
//   → 同样的数学变换思路, 把问题转为"选子集使和接近 sum/2"
// ============================================================
