/*
 * LeetCode 3107: Minimum Operations to Make Median of Array Equal to K
 * (使数组中位数等于 K 的最少操作数)
 * 难度: Medium | 标签: Greedy, Array, Sorting
 *
 * 题目:
 *   给定数组 nums 和整数 k, 每次操作可以将任一元素 +1 或 -1。
 *   求使中位数等于 k 的最少操作次数。
 *   中位数: 排序后中间元素 (偶数长度取较大者, 即下标 n/2)。
 *
 * 核心思路:
 *   排序后中位数在 m = n/2 位置。
 *   - k <= nums[m]: 从 m 向左扫, 所有 > k 的减到 k
 *   - k >  nums[m]: 从 m 向右扫, 所有 < k 的加到 k
 *   只处理一侧即可, 另一侧已满足约束。
 *
 * 图解 (nums = [2,5,6,8,5], k = 4):
 *   排序: [2, 5, 5, 6, 8]   m=2, nums[m]=5
 *   k=4 < 5, 从 m 向左扫:
 *     i=2: 5>4, ops += 5-4 = 1
 *     i=1: 5>4, ops += 5-4 = 1
 *     i=0: 2<=4, break
 *   答案 = 2
 *
 * 图解 (nums = [2,5,6,8,5], k = 7):
 *   排序: [2, 5, 5, 6, 8]   m=2, nums[m]=5
 *   k=7 > 5, 从 m 向右扫:
 *     i=2: 5<7, ops += 7-5 = 2
 *     i=3: 6<7, ops += 7-6 = 1
 *     i=4: 8>=7, break
 *   答案 = 3
 *
 * 易错: 差值累加最大 2*10^14, 必须 long long!
 */


// ================================================================
// 解法一: 排序 + 单侧贪心 -- O(n log n) 时间, O(1) 空间 [推荐]
// ================================================================
// 思路:
//   1. 排序数组
//   2. 中位数在 m = n/2
//   3. 根据 k 与 nums[m] 的关系, 向一侧扫描累加差值
//
// 为什么只需处理一侧:
//   k < nums[m] (需要减小中位数):
//     - 左侧可能有 > k 的元素, 必须减到 k
//       否则排序后它们会占据中位数位置, 导致中位数仍 > k
//     - 右侧 >= nums[m] > k, 已经 >= k, 不需动
//   k > nums[m] (需要增大中位数):
//     - 右侧可能有 < k 的元素, 必须加到 k
//     - 左侧 <= nums[m] < k, 已经 <= k, 不需动
//
// 为什么可以 break:
//   排序后数组单调不减。
//   从 m 向左扫, 遇到 <= k 就能停 (更左的一定更小)
//   从 m 向右扫, 遇到 >= k 就能停 (更右的一定更大)
class Solution {
public:
    long long minOperationsToMakeMedianK(vector<int>& nums, int k) {
        sort(nums.begin(), nums.end());
        int n = nums.size();
        int m = n / 2;  // 中位数位置 (偶数长度取较大者)
        long long ops = 0;

        if (k <= nums[m]) {
            // 情况 A: 需要减小中位数 (及左侧大于 k 的元素)
            for (int i = m; i >= 0; i--) {
                if (nums[i] > k) {
                    ops += (long long)(nums[i] - k);
                    // 注意: (long long) 防止 int 溢出
                } else {
                    break;  // 排序后更左的一定 <= nums[i] <= k
                }
            }
        } else {
            // 情况 B: 需要增大中位数 (及右侧小于 k 的元素)
            for (int i = m; i < n; i++) {
                if (nums[i] < k) {
                    ops += (long long)(k - nums[i]);
                } else {
                    break;  // 排序后更右的一定 >= nums[i] >= k
                }
            }
        }

        return ops;
    }
};


// ================================================================
// 解法二: nth_element + 分区贪心 -- O(n) 期望时间, O(1) 空间
// ================================================================
// 用 nth_element 找中位数 (O(n) 期望), 然后遍历相关元素。
//
// nth_element 保证:
//   - nums[m] 是排序后应该在 m 位置的值
//   - [0, m) 的元素都 <= nums[m]
//   - [m+1, n) 的元素都 >= nums[m]
//   - 但两侧内部不一定有序!
//
// 因此不能 break, 必须全扫相应半边。
class Solution2 {
public:
    long long minOperationsToMakeMedianK(vector<int>& nums, int k) {
        int n = nums.size();
        int m = n / 2;

        nth_element(nums.begin(), nums.begin() + m, nums.end());

        long long ops = 0;

        if (k <= nums[m]) {
            // 遍历 [0, m], 所有 > k 的需要减到 k
            for (int i = 0; i <= m; i++) {
                if (nums[i] > k) {
                    ops += (long long)(nums[i] - k);
                }
                // 不能 break! nth_element 后内部无序
            }
        } else {
            // 遍历 [m, n), 所有 < k 的需要加到 k
            for (int i = m; i < n; i++) {
                if (nums[i] < k) {
                    ops += (long long)(k - nums[i]);
                }
            }
        }

        return ops;
    }
};


// ================================================================
// 解法三: 统一写法 (不分情况) -- O(n log n) 时间
// ================================================================
// 排序后分三部分处理:
//   - 中位数本身: |nums[m] - k|
//   - 左侧 [0, m-1]: 如果 > k 则减到 k
//   - 右侧 [m+1, n-1]: 如果 < k 则加到 k
//
// 优点: 不需要 if-else 分支, 代码更简洁
// 缺点: 不能 break (左右分开遍历, 没有从中位数出发)
class Solution3 {
public:
    long long minOperationsToMakeMedianK(vector<int>& nums, int k) {
        sort(nums.begin(), nums.end());
        int n = nums.size();
        int m = n / 2;
        long long ops = 0;

        // 处理中位数本身
        ops += abs((long long)nums[m] - k);

        // 处理左侧 [0, m-1]: 如果 > k, 需要减到 k
        for (int i = 0; i < m; i++) {
            if (nums[i] > k) {
                ops += (long long)(nums[i] - k);
            }
        }

        // 处理右侧 [m+1, n-1]: 如果 < k, 需要加到 k
        for (int i = m + 1; i < n; i++) {
            if (nums[i] < k) {
                ops += (long long)(k - nums[i]);
            }
        }

        return ops;
    }
};


/*
 * ================================================================
 * 复杂度分析:
 *
 * | 解法               | 时间           | 空间    |
 * |-------------------|--------------|--------|
 * | 排序 + 单侧贪心    | O(n log n)   | O(1)   |
 * | nth_element + 贪心 | O(n) 期望     | O(1)   |
 * | 统一写法           | O(n log n)   | O(1)   |
 *
 * ================================================================
 * 易错点:
 *
 * 1. 数据类型溢出:
 *    nums[i] 和 k 最大 10^9, 差值累加可达 2*10^14
 *    必须用 long long!
 *    ops += (long long)(nums[i] - k);  // 显式转换
 *
 * 2. 中位数下标:
 *    m = n/2 (偶数取较大者)
 *    n=5: m=2, n=6: m=3
 *    不是 (n-1)/2!
 *
 * 3. 解法一可以 break (排序后单调)
 *    解法二不能 break (nth_element 后内部无序)
 *    解法三不能 break (左右分开遍历)
 *
 * 4. k == nums[m] 时:
 *    进入情况 A, 循环条件 nums[m] > k 不满足, 直接 break
 *    ops = 0, 正确!
 *
 * ================================================================
 * 贪心正确性:
 *
 *   目标: 让排序后下标 m 的值 == k
 *
 *   排序后 nums[m] == k 等价于:
 *     至少 m+1 个元素 <= k  (这些在 m 左边或等于 m)
 *     至少 n-m 个元素 >= k  (这些在 m 右边或等于 m)
 *
 *   k < nums[m] 时:
 *     右侧已经 >= nums[m] > k, 满足 >= k
 *     左侧可能有 > k 的, 减到 k 保证 <= k
 *     修改右侧元素: 减小 -> 可能跑到左边挤走左侧元素, 增加操作
 *                  增大 -> 浪费操作
 *     所以只处理左侧是最优的
 *
 *   k > nums[m] 时: 对称推理
 *
 * ================================================================
 * 面试追问:
 *
 * Q1: 为什么只处理一侧?
 *     排序后中位数在固定位置 m。k < nums[m] 时, 右侧 >= nums[m] > k,
 *     已满足 >= k 约束, 修改它们不会使总操作减少。只需处理左侧。
 *
 * Q2: 能否 O(n) 解决?
 *     可以。nth_element O(n) 期望找中位数, 再遍历半边。
 *     但面试中排序方案更稳。
 *
 * Q3: 如果操作是 +2 或 -2?
 *     操作次数变为 ceil(|x - k| / 2), 思路不变, 修改代价计算即可。
 *
 * Q4: 如果数组动态增删元素?
 *     用两个堆 (max-heap + min-heap) 维护中位数, 类似 LC 295。
 *
 * ================================================================
 * 相关题型:
 *
 * 462. Minimum Moves to Equal Array Elements II:
 *   所有元素变成中位数, 类似的绝对值距离思想
 *
 * 295. Find Median from Data Stream:
 *   动态维护中位数 (双堆)
 *
 * 2033. Minimum Operations to Make a Uni-Value Grid:
 *   网格版本, 选中位数最优
 *
 * 2448. Minimum Cost to Make Array Equal:
 *   带权版本, 加权中位数
 * ================================================================
 */
