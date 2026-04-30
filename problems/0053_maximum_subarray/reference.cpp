/*
 * LeetCode 53: 最大子数组和 (Maximum Subarray)
 *
 * 【题目本质】
 * 在所有连续子数组中找和最大的那个。核心是一个"延续 vs 重新开始"的决策问题。
 *
 * 【解法总览】
 * 解法1: 暴力枚举          — O(n²) / O(1)   — 枚举所有区间
 * 解法2: Kadane 算法 (DP)  — O(n)  / O(1)   — ⭐ 面试首选
 * 解法3: 前缀和            — O(n)  / O(1)   — 另一个视角
 * 解法4: 分治法            — O(n log n) / O(log n) — Follow-up
 */

// ============================================================
// 解法1: 暴力枚举 — 枚举所有 (i, j) 区间
// 时间: O(n²)  空间: O(1)
//
// 【思路】
// 最直觉的方式：固定起点 i，向右扩展终点 j，
// 每扩展一步累加 nums[j]，更新全局最大值。
// 瓶颈：每个起点都要扫到末尾，O(n²) 对 n=10^5 会超时。
// ============================================================
class Solution1 {
public:
    int maxSubArray(vector<int>& nums) {
        int n = nums.size();
        int result = INT_MIN;  // 不能初始化为 0，全负数组会错
        for (int i = 0; i < n; i++) {
            int sum = 0;
            for (int j = i; j < n; j++) {
                sum += nums[j];
                result = max(result, sum);
            }
        }
        return result;
    }
};

// ============================================================
// 解法2: Kadane 算法 (动态规划 + 空间优化) — ⭐ 面试首选
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 暴力的瓶颈：对每个起点都要遍历一遍。
// 换个角度：对每个"终点"i，以 nums[i] 结尾的最大子数组和是多少？
//
// 状态定义: dp[i] = 以 nums[i] 结尾的最大子数组和
// 转移方程: dp[i] = max(dp[i-1] + nums[i], nums[i])
//   - dp[i-1] > 0: 前面的子数组有正贡献，接上它
//   - dp[i-1] <= 0: 前面的子数组是累赘，从 nums[i] 重新开始
//
// dp[i] 只依赖 dp[i-1]，空间从 O(n) 优化到 O(1)。
//
// DP 填表过程:
// index:  0    1    2    3    4    5    6    7    8
// nums:  -2    1   -3    4   -1    2    1   -5    4
// dp:    -2    1   -2    4    3    5    6    1    5
//        重新  重新  接上  重新  接上  接上  接上  接上  接上
//        开始  开始       开始
//
// 全局答案 = max(所有 dp[i]) = 6，对应子数组 [4, -1, 2, 1]
// ============================================================
class Solution2 {
public:
    int maxSubArray(vector<int>& nums) {
        int curMax = nums[0];   // dp[i]: 以当前元素结尾的最大子数组和
        int result = nums[0];   // 全局最大
        
        for (int i = 1; i < (int)nums.size(); i++) {
            // 核心决策：前面累积 > 0 就接上，否则重新开始
            curMax = max(curMax + nums[i], nums[i]);
            result = max(result, curMax);
        }
        return result;
    }
};

// ============================================================
// 解法2b: Kadane 的等价写法 — 更直观地展示"重新开始"
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 如果前面的累积和变成负数，直接归零（相当于从下一个元素重新开始）。
// 这种写法需要 result 初始化为 INT_MIN。
// ============================================================
class Solution2b {
public:
    int maxSubArray(vector<int>& nums) {
        int curSum = 0;
        int result = INT_MIN;
        
        for (int num : nums) {
            if (curSum < 0) curSum = 0;  // 累赘，丢弃
            curSum += num;
            result = max(result, curSum);
        }
        return result;
    }
};

// ============================================================
// 解法3: 前缀和 — 转化为"差值最大化"问题
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 子数组 nums[i..j] 的和 = prefix[j+1] - prefix[i]
// 要最大化这个差值，对每个右端点 j，需要 j 之前最小的 prefix[i]。
//
// 这和 121.买卖股票 完全同构：
//   - 股票: maxProfit = price[j] - minPrice
//   - 本题: maxSum = prefixSum[j] - minPrefixSum
//
// 前缀和视角:
// nums:    -2   1  -3   4  -1   2   1  -5   4
// prefix: 0  -2  -1  -4   0  -1   1   2  -3   1
//                     ↑                ↑
//              minPrefix=-4     prefixSum=2
//              maxSum = 2 - (-4) = 6
// ============================================================
class Solution3 {
public:
    int maxSubArray(vector<int>& nums) {
        int result = INT_MIN;
        int prefixSum = 0;
        int minPrefix = 0;  // 对应空前缀，prefix[0] = 0
        
        for (int i = 0; i < (int)nums.size(); i++) {
            prefixSum += nums[i];
            // 先算结果，再更新 minPrefix（保证子数组至少包含一个元素）
            result = max(result, prefixSum - minPrefix);
            minPrefix = min(minPrefix, prefixSum);
        }
        return result;
    }
};

// ============================================================
// 解法4: 分治法 — Follow-up 要求
// 时间: O(n log n)  空间: O(log n) 递归栈
//
// 【思路】
// 类似归并排序的分治策略：
// 1. 把数组从中间一分为二
// 2. 最大子数组要么全在左半，要么全在右半，要么跨越中点
// 3. 前两种递归解决；第三种从中点向两边扩展求最大
// 4. 三者取最大
//
// 分治过程 (示意):
//                [-2, 1, -3, 4, -1, 2, 1, -5, 4]
//               /                                 \
//     [-2, 1, -3, 4]                    [-1, 2, 1, -5, 4]
//        左半最大=4                         右半最大=4
//     跨越中点: 左扩=4, 右扩=(-1+2+1)=2, 跨越和=6
//     全局最大 = max(4, 4, 6) = 6
// ============================================================
class Solution4 {
public:
    int maxSubArray(vector<int>& nums) {
        return solve(nums, 0, (int)nums.size() - 1);
    }
    
private:
    int solve(vector<int>& nums, int left, int right) {
        // base case: 只有一个元素
        if (left == right) return nums[left];
        
        int mid = left + (right - left) / 2;
        int leftMax  = solve(nums, left, mid);       // 全在左半
        int rightMax = solve(nums, mid + 1, right);   // 全在右半
        int crossMax = crossingMax(nums, left, mid, right); // 跨越中点
        
        return max({leftMax, rightMax, crossMax});
    }
    
    // 从 mid 向左扩展 + 从 mid+1 向右扩展，合并得到跨越中点的最大子数组和
    int crossingMax(vector<int>& nums, int left, int mid, int right) {
        // 从 mid 向左扩展
        int leftSum = INT_MIN;  // 必须 INT_MIN，因为至少要包含 nums[mid]
        int sum = 0;
        for (int i = mid; i >= left; i--) {
            sum += nums[i];
            leftSum = max(leftSum, sum);
        }
        
        // 从 mid+1 向右扩展
        int rightSum = INT_MIN;  // 至少要包含 nums[mid+1]
        sum = 0;
        for (int i = mid + 1; i <= right; i++) {
            sum += nums[i];
            rightSum = max(rightSum, sum);
        }
        
        return leftSum + rightSum;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法         | 时间       | 空间     | 适用场景               |
// |-------------|-----------|---------|----------------------|
// | 暴力枚举     | O(n²)     | O(1)    | 理解题意，小数据量       |
// | Kadane (DP) | O(n)      | O(1)    | ⭐ 面试首选            |
// | 前缀和       | O(n)      | O(1)    | 展示多角度思维          |
// | 分治法       | O(n logn) | O(logn) | Follow-up，展示分治能力 |
//
// Kadane vs 前缀和：本质等价。Kadane 的 "curMax < 0 就重新开始"
// 等价于前缀和的 "当前前缀和比之前的最小前缀和还小，就重置 minPrefix"。
//
// 分治法：时间不如 Kadane，但这个思想可以扩展到线段树上支持
// 区间查询（如 "任意区间的最大子数组和"），是进阶的基础。
//
// ============================================================
// 【易错点】
//
// 1. result 初始化为 0:
//    ✗ int result = 0;  → 全负数组如 [-3, -2, -1] 会错误返回 0
//    ✓ int result = nums[0]; 或 int result = INT_MIN;
//
// 2. 前缀和更新顺序颠倒:
//    ✗ minPrefix = min(minPrefix, prefixSum);  // 先更新
//       result = max(result, prefixSum - minPrefix);  // 再算
//    → 这允许 minPrefix == prefixSum，即子数组长度为 0
//    ✓ 先算 result，再更新 minPrefix
//
// 3. 分治法中 crossingMax 的 leftSum/rightSum 初始化:
//    ✗ int leftSum = 0; → 如果 mid 左边全负，leftSum 应为最大负数
//    ✓ int leftSum = INT_MIN; → 强制至少选一个元素
//
// 4. Kadane 的两种写法混淆:
//    写法A: curMax = max(curMax + num, num), result = max(result, curMax)
//           → 初始化 curMax = nums[0], result = nums[0], 从 i=1 开始
//    写法B: if (curMax < 0) curMax = 0; curMax += num; result = max(...)
//           → 初始化 curMax = 0, result = INT_MIN, 从 i=0 开始
//    两种不能混用！
//
// ============================================================
// 【面试追问（递进链）】
//
// Q1: "暴力 O(n²) 能过吗？瓶颈在哪？"
//     → n=10^5 时约 10^10 次操作，超时。瓶颈在于每个起点都要扫到末尾。
//
// Q2: "Kadane 的贪心决策为什么不会漏掉最优解？"
//     → 如果 dp[i-1] < 0，那么任何包含 dp[i-1] 对应子数组的更长子数组
//       一定不如从 nums[i] 重新开始的好。最优子数组的起点一定是某个
//       "前面累积为负，从此重新开始"的位置。
//
// Q3: "如果要返回最大子数组的起止下标怎么办？"
//     → 维护 start, end, tempStart。当 curMax < nums[i]（重新开始）时
//       tempStart = i；当 curMax > result 时 start = tempStart, end = i。
//
// Q4: "如果是环形数组呢？" (LeetCode 918)
//     → 答案 = max(普通 Kadane 最大和, 总和 - Kadane 最小子数组和)。
//       特判：全负数组时最小子数组 = 整个数组，此时只取第一种。
//
// Q5: "如果要支持动态修改某个元素后查询任意区间的最大子数组和？"
//     → 线段树，每个节点维护 (区间和, 最大前缀和, 最大后缀和, 最大子数组和)，
//       合并时用 crossingMax 的思路。这就是分治法的延伸。
// ============================================================
