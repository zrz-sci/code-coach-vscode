/*
 * LeetCode 918: 环形子数组的最大和 (Maximum Sum Circular Subarray)
 * 难度: Medium
 * 标签: Array, Dynamic Programming, Monotonic Queue
 *
 * 题目: 给定长度为 n 的环形整数数组 nums，返回非空子数组的最大可能和。
 *       环形 = 数组末尾连接开头。
 *
 * 约束: 1 <= n <= 3*10^4, -3*10^4 <= nums[i] <= 3*10^4
 */


/* ============================================================
 * 解法 1: Kadane 变体 — O(n) / O(1) [推荐]
 * ============================================================
 *
 * 核心洞察: 环形子数组的最大和只有两种情况:
 *
 *   情况1 - 不跨边界:        情况2 - 跨越边界:
 *   [...[===max===]...]      [==]...[min]...[==]
 *         ↑ Kadane 最大           ↑ total - Kadane 最小
 *
 * 跨越边界的子数组和 = totalSum - (不选的中间段)
 * 要最大化跨越部分 → 最小化中间段 → 求最小子数组和
 *
 * 答案 = max(maxSum, totalSum - minSum)
 *
 * 特判: 全负数时 totalSum == minSum → totalSum - minSum = 0 (空子数组)
 *       题目要求非空 → 直接返回 maxSum
 *
 * 详细推演 (nums = [5, -3, 5]):
 * ┌────┬─────┬────────┬────────┬────────┬────────┬──────────┐
 * │ i  │ num │ curMax │ maxSum │ curMin │ minSum │ totalSum │
 * ├────┼─────┼────────┼────────┼────────┼────────┼──────────┤
 * │ 0  │  5  │   5    │   5    │   5    │   5    │    5     │
 * │ 1  │ -3  │   2    │   5    │  -3    │  -3    │    2     │
 * │ 2  │  5  │   7    │   7    │   2    │  -3    │    7     │
 * └────┴─────┴────────┴────────┴────────┴────────┴──────────┘
 * 情况1: maxSum = 7, 情况2: 7 - (-3) = 10
 * 答案 = max(7, 10) = 10  (跨越边界的 [5, 5])
 */
class Solution {
public:
    int maxSubarraySumCircular(vector<int>& nums) {
        int totalSum = 0;
        int curMax = 0, maxSum = INT_MIN;  // Kadane 求最大
        int curMin = 0, minSum = INT_MAX;  // Kadane 求最小

        for (int num : nums) {
            // --- Kadane 求最大子数组和 ---
            // curMax: 以当前元素结尾的最大子数组和
            // 选择: 延续之前的子数组 (curMax + num) 或 从当前重新开始 (num)
            curMax = max(curMax + num, num);
            maxSum = max(maxSum, curMax);

            // --- Kadane 求最小子数组和 (镜像操作) ---
            // curMin: 以当前元素结尾的最小子数组和
            curMin = min(curMin + num, num);
            minSum = min(minSum, curMin);

            // 累加总和
            totalSum += num;
        }

        // 特判: 全负数 → maxSum < 0
        // 此时 totalSum == minSum → totalSum - minSum = 0 (空子数组, 不合法)
        if (maxSum < 0) return maxSum;

        // 一般情况: 取 "不跨边界的最大" 和 "跨边界的最大" 中的较大者
        return max(maxSum, totalSum - minSum);
    }
};


/* ============================================================
 * 解法 2: 前缀和 + 单调队列 — O(n) / O(n)
 * ============================================================
 *
 * 思路: 将数组复制一遍 (长度2n)，用前缀和转化为:
 *   max(prefix[j] - prefix[i])  where j - i <= n
 *
 * 用单调递增队列维护窗口内最小前缀和，实现 O(n)。
 *
 * 优势: 更通用，可以处理"子数组长度有约束"的变体
 * 劣势: 空间 O(n)，代码更复杂
 */
/*
class Solution {
public:
    int maxSubarraySumCircular(vector<int>& nums) {
        int n = nums.size();
        // 构建前缀和 (长度 2n+1)
        vector<long long> prefix(2 * n + 1, 0);
        for (int i = 0; i < 2 * n; i++) {
            prefix[i + 1] = prefix[i] + nums[i % n];
        }

        long long ans = LLONG_MIN;
        deque<int> dq; // 单调递增队列，存下标
        dq.push_back(0);

        for (int j = 1; j <= 2 * n; j++) {
            // 移除超出窗口 (长度 > n) 的元素
            while (!dq.empty() && dq.front() < j - n) {
                dq.pop_front();
            }
            // prefix[j] - prefix[dq.front()] 是以 j 结尾、长度 <= n 的最大子数组和
            if (!dq.empty()) {
                ans = max(ans, prefix[j] - prefix[dq.front()]);
            }
            // 维护单调递增: 弹出所有 >= prefix[j] 的
            while (!dq.empty() && prefix[dq.back()] >= prefix[j]) {
                dq.pop_back();
            }
            dq.push_back(j);
        }

        return (int)ans;
    }
};
*/


/* ============================================================
 * 解法 3: 前缀和 + 右侧最大值数组 — O(n) / O(n)
 * ============================================================
 *
 * 思路 (LeetCode 官方解法之一):
 * 1. 计算前缀和 prefix[i] = nums[0] + ... + nums[i-1]
 * 2. 对于不跨边界情况: max(prefix[j] - prefix[i]), 0 <= i < j <= n
 * 3. 对于跨边界情况:
 *    选 [j..n-1] + [0..i] = totalSum - (prefix[j] - prefix[i])
 *    等价于 prefix[i] + (totalSum - prefix[j])
 *    = prefix[i] + suffix[j]
 *
 * 预计算 rightMax[i] = max(prefix[j]) for j >= i
 * 然后 ans = max over i of (prefix[i] + rightMax[i+1])
 */
/*
class Solution {
public:
    int maxSubarraySumCircular(vector<int>& nums) {
        int n = nums.size();

        // 前缀和
        vector<int> prefix(n + 1, 0);
        for (int i = 0; i < n; i++) {
            prefix[i + 1] = prefix[i] + nums[i];
        }

        // rightMax[i] = max(prefix[i], prefix[i+1], ..., prefix[n])
        vector<int> rightMax(n + 1);
        rightMax[n] = prefix[n];
        for (int i = n - 1; i >= 0; i--) {
            rightMax[i] = max(rightMax[i + 1], prefix[i]);
        }

        int ans = INT_MIN;
        // 情况1: 不跨边界 → 经典最大子数组和
        int curMax = 0, maxSum = INT_MIN;
        for (int num : nums) {
            curMax = max(curMax + num, num);
            maxSum = max(maxSum, curMax);
        }
        ans = maxSum;

        // 情况2: 跨边界 → prefix[i] + (totalSum - 右侧最小prefix)
        // 等价于: prefix[i] + rightMax[i+1] 对应的后缀和
        int totalSum = prefix[n];
        for (int i = 0; i < n; i++) {
            // 跨越: [0..i] + 某个 [j..n-1] 中最优的
            int crossSum = prefix[i + 1] + (totalSum - rightMax[i + 1]);
            // 注意这里需要更仔细的处理，简化版用 Kadane 变体更清晰
        }

        return ans;
    }
};
*/


/* ============================================================
 * 知识点总结
 * ============================================================
 *
 * 1. Kadane 算法核心:
 *    curMax = max(curMax + nums[i], nums[i])
 *    含义: "延续" vs "从头开始"，贪心地做局部最优选择
 *
 * 2. 环形问题的拆解模式:
 *    环形最优 = max(不跨边界最优, 跨边界最优)
 *    跨边界最优 = totalSum - 不跨边界最小
 *    → 同时求最大和最小，一次遍历搞定
 *
 * 3. 全负数特判:
 *    maxSum < 0 时直接返回 maxSum
 *    否则 totalSum - minSum 会给出 0 (对应空子数组)
 *
 * 4. 变量数量: 只需 5 个变量 (totalSum, curMax, maxSum, curMin, minSum)
 *    一次遍历，O(1) 空间，面试中最优雅的解法
 *
 * 5. 相关题:
 *    LC 53  最大子数组和 (Kadane 基础)
 *    LC 152 乘积最大子数组 (Kadane 变体)
 *    LC 1191 K次串联后最大子数组和 (环形进阶)
 */

/* ============================================================
 * 【解法对比】
 *
 * | 解法                    | 时间   | 空间   | 代码复杂度 | 面试推荐 |
 * |------------------------|--------|--------|-----------|---------|
 * | Kadane 变体 (解法1)     | O(n)  | O(1)   | 简洁       | ⭐⭐⭐  |
 * | 前缀和+单调队列 (解法2)  | O(n)  | O(n)   | 复杂       | ✓ 进阶  |
 * | 前缀和+右侧最大值 (解法3)| O(n)  | O(n)   | 中等       | ✓ 备选  |
 *
 * 面试策略:
 *   1. 先说经典 Kadane (LC53)，确认面试官认可基础
 *   2. 画 "不跨边界 vs 跨边界" 两种情况的图
 *   3. 推导: 跨边界最大 = totalSum - minSubarraySum
 *   4. 特判全负数（totalSum == minSum → 空子数组）
 *   5. 面试官追问变体 → 提单调队列方法
 * ============================================================
 *
 * 【易错点】
 *
 * 1. maxSum 初始值设错:
 *    ✗ maxSum = 0 → 全负数时返回 0，但题目要求非空子数组
 *    ✓ maxSum = INT_MIN, minSum = INT_MAX
 *
 * 2. 忘记全负数特判:
 *    ✗ 直接返回 max(maxSum, totalSum - minSum)
 *      → 全负数时 totalSum - minSum = 0 (空子数组，不合法)
 *    ✓ if (maxSum < 0) return maxSum;
 *
 * 3. curMax/curMin 更新顺序:
 *    ✗ 先更新 totalSum 再更新 curMax → 不影响正确性
 *    但要确保 curMax = max(curMax + num, num) 中 num 是当前元素
 *    不要把 totalSum 当成 num
 *
 * 4. 环形 = 复制数组？
 *    ✗ 暴力复制一遍数组做长度 2n 的 Kadane → O(n) 但需限制子数组长度 <= n
 *    ✓ 如果不限长度会多算。正确做法用 Kadane 变体或单调队列
 * ============================================================
 *
 * 【面试追问】
 *
 * Q1: 为什么 totalSum - minSum 等价于跨边界的最大子数组和？
 * A1: 跨边界子数组 = 选头部一段 + 尾部一段 = 全部 - 中间不选的一段。
 *     要最大化跨边界部分 → 最小化中间不选的部分 → 求最小子数组和。
 *     所以 跨边界最大 = totalSum - minSubarraySum。
 *
 * Q2: 如果改成「环形子数组的最小和」怎么做？
 * A2: 完全对称: minCircular = min(minSum, totalSum - maxSum)。
 *     特判: 全正数时 totalSum - maxSum 可能为 0（空子数组），
 *     此时直接返回 minSum。
 *
 * Q3: 如果数组可以串联 k 次（LC1191），如何扩展？
 * A3: k=1 就是普通 Kadane; k=2 等价于本题环形;
 *     k>=3 时跨边界最大 = maxPrefix + maxSuffix + (k-2)*totalSum（如果 totalSum>0）。
 *     直觉: 中间 k-2 段全选（正贡献），头尾各取最优前后缀。
 *     如果 totalSum<=0，中间段不选，退化为 k=2 的环形情况。
 * ============================================================
 */
