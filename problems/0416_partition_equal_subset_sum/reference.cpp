/*
 * LeetCode 416: 分割等和子集 (Partition Equal Subset Sum)
 *
 * 【题目本质】
 * 能否从数组中选若干数使和 = totalSum / 2？ → 0/1 背包判定问题。
 *
 * 【解法总览】
 * 解法1: 暴力递归(DFS)    — O(2^n) / O(n)             — 最直觉，理解"选/不选"
 * 解法2: 记忆化搜索        — O(n×target) / O(n×target) — 消除重叠子问题
 * 解法3: 二维 DP(0/1背包)  — O(n×target) / O(n×target) — 翻译成递推表
 * 解法4: 一维 DP(空间优化) — O(n×target) / O(target)   — 面试首选
 */

// ============================================================
// 解法1: 暴力递归 — 每个元素选或不选，枚举所有子集
// 时间: O(2^n)  空间: O(n) 递归栈
//
// 【思路】
// 最朴素的想法：对每个数做"选/不选"决策，递归到底看能否凑出 target。
// 这帮助我们建立直觉：每个元素的二元决策 → 0/1 背包结构。
//
// 决策树（nums = [1,5,11,5], target = 11）:
//                    remain=11
//                   /         \
//          选1: r=10         不选1: r=11
//              /     \            /      \
//        选5: r=5  不选5: r=10 选5: r=6  不选5: r=11
//           /  \       ...       ...       ...
//      选11:  不选11
//      r=-6✗  r=5
//            / \
//       选5: 不选5:
//       r=0✓  r=5✗    ← [1,5,5] 凑出11
// ============================================================
class Solution1 {
public:
    bool canPartition(vector<int>& nums) {
        int sum = 0;
        for (int x : nums) sum += x;
        if (sum % 2 != 0) return false; // 奇数不可能分成两个相等的整数和
        return dfs(nums, 0, sum / 2);
    }

private:
    bool dfs(vector<int>& nums, int index, int remain) {
        if (remain == 0) return true;    // 恰好凑出 target
        if (index >= (int)nums.size() || remain < 0) return false;

        // 核心：选当前数 or 不选当前数
        return dfs(nums, index + 1, remain - nums[index])
            || dfs(nums, index + 1, remain);
    }
};

// ============================================================
// 解法2: 记忆化搜索 — 暴力递归 + memo
// 时间: O(n × target)  空间: O(n × target)
//
// 【思路】
// 解法1的瓶颈：大量重叠子问题。
// 状态 (index, remain) 可能从不同路径重复到达。
// 加一个二维 memo 数组缓存，把 2^n 降到 n × target。
// ============================================================
class Solution2 {
public:
    bool canPartition(vector<int>& nums) {
        int sum = 0;
        for (int x : nums) sum += x;
        if (sum % 2 != 0) return false;

        int target = sum / 2;
        // memo[i][j]: -1=未计算, 0=false, 1=true
        vector<vector<int>> memo(nums.size(), vector<int>(target + 1, -1));
        return dfs(nums, 0, target, memo);
    }

private:
    bool dfs(vector<int>& nums, int idx, int remain, vector<vector<int>>& memo) {
        if (remain == 0) return true;
        if (idx >= (int)nums.size() || remain < 0) return false;
        if (memo[idx][remain] != -1) return memo[idx][remain]; // 已算过直接返回

        bool res = dfs(nums, idx + 1, remain - nums[idx], memo)
                || dfs(nums, idx + 1, remain, memo);
        memo[idx][remain] = res;
        return res;
    }
};

// ============================================================
// 解法3: 二维 DP（0/1 背包递推） — 从记忆化翻译成填表
// 时间: O(n × target)  空间: O(n × target)
//
// 【思路】
// dp[i][j] = 用前 i 个数（nums[0..i-1]）能否恰好凑出和 j
// 转移:
//   不选第 i 个: dp[i][j] = dp[i-1][j]
//   选第 i 个:   dp[i][j] |= dp[i-1][j - nums[i-1]]  (需要 j >= nums[i-1])
//
// dp 填表过程 (nums = [1,5,11,5], target = 11):
//
//        j:  0  1  2  3  4  5  6  7  8  9  10  11
// i=0 (无):  T  F  F  F  F  F  F  F  F  F   F   F
// i=1 (1) :  T  T  F  F  F  F  F  F  F  F   F   F
// i=2 (5) :  T  T  F  F  F  T  T  F  F  F   F   F
// i=3 (11):  T  T  F  F  F  T  T  F  F  F   F   T  ← 选了11凑出11
// i=4 (5) :  T  T  F  F  F  T  T  F  F  F   T   T
//
// 依赖关系: dp[i][j] 依赖 dp[i-1][j](正上方) 和 dp[i-1][j-num](左上方)
// 填表方向: i 从上到下, j 从左到右或从右到左均可
// ============================================================
class Solution3 {
public:
    bool canPartition(vector<int>& nums) {
        int sum = 0;
        for (int x : nums) sum += x;
        if (sum % 2 != 0) return false;

        int n = nums.size(), target = sum / 2;
        vector<vector<bool>> dp(n + 1, vector<bool>(target + 1, false));

        // base case: 凑出和 0 永远可以（不选任何数）
        for (int i = 0; i <= n; i++) dp[i][0] = true;

        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= target; j++) {
                dp[i][j] = dp[i - 1][j]; // 不选 nums[i-1]
                if (j >= nums[i - 1]) {
                    // 选 nums[i-1]，从 dp[i-1][j-nums[i-1]] 转移
                    dp[i][j] = dp[i][j] || dp[i - 1][j - nums[i - 1]];
                }
            }
        }
        return dp[n][target];
    }
};

// ============================================================
// 解法4: 一维 DP（空间优化） — 面试首选 ⭐
// 时间: O(n × target)  空间: O(target)
//
// 【思路】
// 解法3中 dp[i][j] 只依赖 dp[i-1][...] (上一行)，
// 所以可以用一维数组滚动更新，从 O(n×target) 空间降到 O(target)。
//
// 关键：内层循环必须【倒序】遍历 j！
// 原因：倒序保证更新 dp[j] 时用到的 dp[j-num] 还是"上一行"的旧值。
//       正序的话 dp[j-num] 已经被本轮更新过，相当于同一物品选了多次 → 完全背包。
//
// 倒序 vs 正序的区别（假设 num=3, dp = [T,F,F,T,F,F,...]）:
//   正序: j=3: dp[3]|=dp[0]=T, j=6: dp[6]|=dp[3]=T ← dp[3]已被更新! 3被选了两次!
//   倒序: j=6: dp[6]|=dp[3]=T(旧值), j=3: dp[3]|=dp[0]=T ← 每个数只选一次 ✓
// ============================================================
class Solution4 {
public:
    bool canPartition(vector<int>& nums) {
        int sum = 0;
        for (int x : nums) sum += x;
        if (sum % 2 != 0) return false; // 奇数 → 不可能

        int target = sum / 2;

        // 提前剪枝：如果最大元素 > target，不可能
        for (int x : nums) {
            if (x > target) return false;
        }

        vector<bool> dp(target + 1, false);
        dp[0] = true; // 空集的和 = 0

        for (int num : nums) {
            // 0/1 背包核心：倒序遍历，保证每个 num 只被选一次
            for (int j = target; j >= num; j--) {
                // dp[j] = dp[j] || dp[j - num]
                // 不选num(dp[j]保留旧值) or 选num(dp[j-num]是旧值因为倒序)
                dp[j] = dp[j] || dp[j - num];
            }
            // 提前终止：已经能凑出 target
            if (dp[target]) return true;
        }
        return dp[target];
    }
};

// ============================================================
// 【解法对比】
//
// | 解法 | 时间 | 空间 | 特点 |
// |------|------|------|------|
// | 暴力递归 | O(2^n) | O(n) | 理解决策结构 |
// | 记忆化 | O(n×T) | O(n×T) | 消除重叠子问题 |
// | 二维DP | O(n×T) | O(n×T) | 递推，便于理解填表 |
// | 一维DP | O(n×T) | O(T) | 面试首选，空间最优 |
//
// 面试中推荐直接写解法4（一维DP），但要能解释：
//   1. 为什么是 0/1 背包
//   2. 为什么倒序遍历
//   3. dp[0] = true 的含义
//
// 【易错点】
// 1. 忘记判奇数：sum 为奇数时 target 不是整数，直接 false。
//    ✗ int target = sum / 2; // 直接开始DP
//    ✓ if (sum % 2 != 0) return false; // 先判断
//
// 2. 一维DP正序遍历 → 变成完全背包:
//    ✗ for (int j = num; j <= target; j++) — 同一个数可能被选多次
//    ✓ for (int j = target; j >= num; j--) — 0/1背包必须倒序
//
// 3. dp初始化错误:
//    ✗ dp[0] = false → 永远无法转移出 true
//    ✓ dp[0] = true  → "和为0通过不选任何数达到"
//
// 4. 没有处理单个元素超过target的情况:
//    虽然不会导致WA(循环自然跳过)，但提前return false效率更高。
//
// 【面试追问】
//
// Q1: 这道题本质是什么类型的问题？
// → 0/1 背包判定版。n 个物品，重量 nums[i]，容量 target = sum/2，
//   问能否恰好装满。
//
// Q2: 如果改成"分两组使差最小"？（→ LeetCode 1049）
// → 同样的 0/1 背包，dp 求出所有可达的和 S1 (S1 ≤ sum/2)，
//   取最大的 S1，答案 = sum - 2*S1。
//   代码改动：最后遍历 dp 找最大的 true 位置，而不是只看 dp[target]。
//
// Q3: 如果每个元素可以使用无限次？（→ 完全背包）
// → 内层循环改为正序：for (int j = num; j <= target; j++)
//   这就是 0/1 背包和完全背包的唯一代码区别。
//
// Q4: 如果要求输出具体的分割方案？
// → 额外记录路径：用 parent[j] 记录到达状态 j 时选了哪个数，
//   最后从 dp[target] 回溯即可。
// ============================================================
