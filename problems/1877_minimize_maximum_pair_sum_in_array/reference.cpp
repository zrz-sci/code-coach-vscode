/*
 * LeetCode 1877: Minimize Maximum Pair Sum in Array
 * 数组中最大数对和的最小值
 *
 * 难度: Medium
 * 标签: Greedy, Array, Two Pointers, Sorting
 *
 * 题意:
 *   给定偶数长度的数组 nums，将元素分成 n/2 对，
 *   使得所有数对和中的最大值尽可能小。返回最小化后的最大对和。
 *
 * 核心思路: 排序 + 贪心(最大配最小) + 双指针
 *
 * 贪心可视化:
 *
 *   排序后: [2, 3, 4, 4, 5, 6]
 *
 *   最大配最小:             顺序配对:
 *   (2,6) = 8              (2,3) = 5
 *   (3,5) = 8              (4,4) = 8
 *   (4,4) = 8              (5,6) = 11
 *   max = 8 ✓              max = 11 ✗
 *
 *   大配小 → 每对和被"均衡化"
 *   大配大 → 产生极端大的对和
 *
 * 贪心正确性(交换论证):
 *   对于 a < b < c < d:
 *     方案1: (a,c) (b,d) → max = b+d
 *     方案2: (a,d) (b,c) → max = max(a+d, b+c) <= b+d
 *   交换为"更外层配对"永远不会更差
 */

// =====================================================
// 解法: 排序 + 双指针 (贪心)
// 时间: O(n log n)   空间: O(1)
// =====================================================
class Solution {
public:
    int minPairSum(vector<int>& nums) {
        // 步骤1: 排序 — 为贪心配对做准备
        sort(nums.begin(), nums.end());

        // 步骤2: 双指针 — 最大配最小
        int res = 0;
        int left = 0, right = (int)nums.size() - 1;

        while (left < right) {
            // 当前配对: nums[left](当前最小) + nums[right](当前最大)
            int pairSum = nums[left] + nums[right];

            // 记录所有配对中的最大和
            res = max(res, pairSum);

            left++;
            right--;
        }

        return res;
    }
};

/*
 * 执行过程 — 示例1:
 *
 *   nums = [3, 5, 2, 3]
 *   排序后: [2, 3, 3, 5]
 *
 *   迭代1: left=0, right=3
 *     pairSum = 2 + 5 = 7
 *     res = max(0, 7) = 7
 *     left=1, right=2
 *
 *   迭代2: left=1, right=2
 *     pairSum = 3 + 3 = 6
 *     res = max(7, 6) = 7
 *     left=2, right=1  → 循环结束
 *
 *   返回 7
 *
 *
 * 执行过程 — 示例2:
 *
 *   nums = [3, 5, 4, 2, 4, 6]
 *   排序后: [2, 3, 4, 4, 5, 6]
 *
 *   迭代1: left=0, right=5
 *     pairSum = 2 + 6 = 8
 *     res = 8
 *
 *   迭代2: left=1, right=4
 *     pairSum = 3 + 5 = 8
 *     res = 8
 *
 *   迭代3: left=2, right=3
 *     pairSum = 4 + 4 = 8
 *     res = 8
 *
 *   返回 8
 */


// =====================================================
// 贪心正确性的严格证明
// =====================================================
/*
 * 定理: 排序后首尾配对使最大对和最小。
 *
 * 证明(交换论证):
 *
 * 设排序后 a[0] <= a[1] <= ... <= a[n-1]
 *
 * 假设最优方案 S* 中存在 i < j < k < l 使得:
 *   - (a[i], a[k]) 和 (a[j], a[l]) 是两对  (不是首尾对称配)
 *
 * 考虑交换为 (a[i], a[l]) 和 (a[j], a[k]):
 *   max(a[i]+a[l], a[j]+a[k])
 *   ≤ max(a[j]+a[l], a[j]+a[l])    [因为 a[i]≤a[j] 且 a[k]≤a[l]]
 *   = a[j] + a[l]
 *   ≤ max(a[i]+a[k], a[j]+a[l])    [原方案中这两对的最大值]
 *
 * 所以交换后不会更差。反复交换可到达首尾配对方案。
 * 因此首尾配对方案是最优的。   □
 */


// =====================================================
// 变种: 如果需要输出具体配对方案
// =====================================================
class SolutionWithPairs {
public:
    int minPairSum(vector<int>& nums) {
        int n = nums.size();
        sort(nums.begin(), nums.end());

        int res = 0;
        vector<pair<int,int>> pairs; // 记录配对方案

        int left = 0, right = n - 1;
        while (left < right) {
            int pairSum = nums[left] + nums[right];
            res = max(res, pairSum);

            pairs.push_back({nums[left], nums[right]});

            left++;
            right--;
        }

        // 输出配对方案
        // for (auto& [a, b] : pairs) {
        //     cout << "(" << a << ", " << b << ") = " << a+b << endl;
        // }

        return res;
    }
};


// =====================================================
// 拓展思考: 如果不用排序，使用计数排序优化
// 时间: O(n + maxVal)   空间: O(maxVal)
// =====================================================
/*
 * 当 nums[i] 的值域较小时(如 <= 10^5)，可以用计数排序
 * 将时间从 O(n log n) 降为 O(n + maxVal)。
 *
 * 思路:
 *   1. 用 count[v] 记录值 v 出现的次数
 *   2. 双指针: left 从最小值开始，right 从最大值开始
 *   3. 每次消耗 count[left] 和 count[right]，模拟配对过程
 */
class SolutionCountingSort {
public:
    int minPairSum(vector<int>& nums) {
        // 找值域范围
        int maxVal = *max_element(nums.begin(), nums.end());
        int minVal = *min_element(nums.begin(), nums.end());

        // 计数
        vector<int> count(maxVal + 1, 0);
        for (int x : nums) count[x]++;

        int res = 0;
        int left = minVal, right = maxVal;

        while (left <= right) {
            // 跳过计数为0的值
            while (left <= right && count[left] == 0) left++;
            while (left <= right && count[right] == 0) right--;

            if (left > right) break;

            // 配对 left 和 right
            res = max(res, left + right);

            if (left == right) {
                // 相同值自我配对，消耗2个
                count[left] -= 2;
            } else {
                count[left]--;
                count[right]--;
            }
        }

        return res;
    }
};

/*
 * 三种方案对比:
 *
 * | 方案           | 时间          | 空间       | 适用场景              |
 * |----------------|---------------|------------|-----------------------|
 * | 排序+双指针    | O(n log n)    | O(1)       | 通用，首选            |
 * | 输出配对方案   | O(n log n)    | O(n)       | 需要知道具体配对      |
 * | 计数排序       | O(n + maxVal) | O(maxVal)  | 值域较小时更快        |
 *
 * 易错点:
 * 1. 忘记排序 → 双指针无意义
 * 2. res初始化为0而非INT_MIN: 因为nums[i]>=1, 所以对和>=2, 0做初值没问题
 * 3. 计数排序中 left==right 时要消耗2个计数
 * 4. 返回最大对和，不是对和之和
 */

// =====================================================
// 总结
// =====================================================
/*
 * 【解法对比】
 *
 * | 解法             | 时间           | 空间       | 适用场景              |
 * |------------------|----------------|------------|-----------------------|
 * | 排序+双指针(推荐)| O(n log n)     | O(1)       | 通用首选              |
 * | 输出配对方案     | O(n log n)     | O(n)       | 需要具体配对信息      |
 * | 计数排序         | O(n + maxVal)  | O(maxVal)  | 值域小时更快          |
 *
 * 【易错点】
 * 1. 忘记排序 — 双指针首尾配对的前提是数组有序，不排序则无贪心性质
 * 2. 求的是"最大对和的最小值"，不是"所有对和之和" — 只需跟踪 max
 * 3. res 初始化为 0 是安全的 — 因为 nums[i]>=1，任何对和>=2
 * 4. 计数排序中 left==right 时要消耗 2 个计数 — 相同值自我配对
 * 5. 数组长度保证为偶数 — 不需要处理奇数情况
 *
 * 【面试追问】
 * Q1: 贪心正确性的核心论证是什么？一句话概括？
 *     → "最大配最小"使得每对和被均衡化；交换论证证明任何非首尾
 *       配对都可以调整为首尾配对且不变差。核心不等式:
 *       对 a<=b<=c<=d，max(a+d, b+c) <= max(a+c, b+d) = b+d。
 *
 * Q2: 如果要把数组分成 n/3 组（每组3个），最小化最大组和，怎么做？
 *     → 排序后贪心策略变为: 最大元素与两个最小元素配对。
 *       具体: 排序后取 (a[0],a[1],a[n-1]), (a[2],a[3],a[n-2]), ...
 *       正确性需更细致的交换论证，但大方向相同 — 大配小均衡化。
 *
 * Q3: 如果不是分组而是任意二元配对（不要求全配），求 k 对的最小最大和？
 *     → 二分答案。对于候选最大和 T，贪心检查: 排序后双指针统计
 *       有多少对满足 nums[i]+nums[j] <= T。若 >=k 对则 T 可行。
 *       时间 O(n log n + n log S)，S 是和的值域。
 */
