/*
 * LeetCode 908: Smallest Range I
 *
 * 解法1: 数学公式 (面试首选)
 * 时间: O(n)  空间: O(1)
 *
 * 核心: answer = max(0, maxVal - minVal - 2*k)
 * max 最多减 k，min 最多加 k，差值缩小 2k，但不能为负。
 */

// ==================== 解法1: STL max/min_element ====================

class Solution {
public:
    int smallestRangeI(vector<int>& nums, int k) {
        // 找到数组的最大值和最小值
        int maxVal = *max_element(nums.begin(), nums.end());
        int minVal = *min_element(nums.begin(), nums.end());

        // 原始差距 - 可缩小的量(2k)，但不能小于 0
        // max 往下调 k: max - k
        // min 往上调 k: min + k
        // 新差距 = (max - k) - (min + k) = max - min - 2k
        return max(0, (maxVal - minVal) - 2 * k);
    }
};

// ==================== 解法2: 单次遍历 ====================

/*
 * 时间: O(n)  空间: O(1)
 * 只遍历一次同时找 max 和 min，比解法1少一次遍历
 */

class Solution_OnePass {
public:
    int smallestRangeI(vector<int>& nums, int k) {
        int maxVal = nums[0];
        int minVal = nums[0];

        // 单次遍历同时更新最大值和最小值
        for (int i = 1; i < (int)nums.size(); i++) {
            if (nums[i] > maxVal) maxVal = nums[i];
            if (nums[i] < minVal) minVal = nums[i];
        }

        // 计算最小可能差值
        int gap = maxVal - minVal;
        int shrink = 2 * k; // 总共能缩小的量

        // 如果能缩小的量 >= 原始差距，差值可以为 0
        // 否则差值 = 原始差距 - 可缩小量
        return (gap <= shrink) ? 0 : gap - shrink;
    }
};

// ==================== 解法3: 排序法 ====================

/*
 * 时间: O(n log n)  空间: O(1)
 * 排序后首尾就是 min 和 max
 * 代码简洁但时间复杂度不如线性遍历
 */

class Solution_Sort {
public:
    int smallestRangeI(vector<int>& nums, int k) {
        sort(nums.begin(), nums.end());

        // 排序后: nums.front() = min, nums.back() = max
        int gap = nums.back() - nums.front();
        return max(0, gap - 2 * k);
    }
};

// ==================== 解法4: minmax_element (C++ STL) ====================

/*
 * 时间: O(n)  空间: O(1)
 * 使用 minmax_element 一次遍历找到 min 和 max
 * 比分别调用 min_element + max_element 少约 25% 比较次数
 */

class Solution_MinMax {
public:
    int smallestRangeI(vector<int>& nums, int k) {
        // minmax_element 返回 pair<iterator, iterator>
        // first -> min, second -> max
        // 只需 ~1.5n 次比较 (vs 分别调用需要 2n 次)
        auto [itMin, itMax] = minmax_element(nums.begin(), nums.end());
        return max(0, *itMax - *itMin - 2 * k);
    }
};

/*
 * ==================== 数学推导 ====================
 *
 * 设原始数组: nums = [a_1, a_2, ..., a_n]
 * 约束: 每个 a_i 可以变成 a_i + x_i, 其中 -k <= x_i <= k
 *
 * 目标: 最小化 max(a_i + x_i) - min(a_i + x_i)
 *
 * 最优策略:
 * - 让最大值尽量小: max 变成 max(a) - k (取 x_max = -k)
 * - 让最小值尽量大: min 变成 min(a) + k (取 x_min = +k)
 * - 中间值随便调，不影响答案
 *
 * 新的"分数" = (max(a) - k) - (min(a) + k)
 *             = max(a) - min(a) - 2k
 *
 * 但分数不能为负 (因为相同元素差值为0):
 * answer = max(0, max(a) - min(a) - 2k)
 *
 * 为什么中间元素 a_m (min < a_m < max) 不影响?
 * - 调整后的 a_m 范围: [a_m - k, a_m + k]
 * - 因为 min(a) <= a_m <= max(a)
 *   所以 min(a) + k <= a_m + k 且 a_m - k <= max(a) - k
 * - 即 a_m 的调整范围一定和 [min(a)+k, max(a)-k] 重叠
 * - 中间元素总能调到目标范围内，不会成为新的 max 或 min
 *
 * ==================== 与 Smallest Range II (LC 910) 对比 ====================
 *
 * | 特征           | 908 (本题)          | 910                    |
 * |---------------|---------------------|------------------------|
 * | 操作          | +x, x in [-k, k]   | +k 或 -k (二选一)      |
 * | 自由度        | 高(连续区间)         | 低(只有两个选择)        |
 * | 解法          | O(n) 公式           | O(n log n) 排序+枚举   |
 * | 难度          | Easy                | Medium                 |
 * | 核心区别      | 可以取中间值         | 必须走极端(+k或-k)     |
 *
 * 910 的关键:
 * 排序后，存在一个分界点 i，使得:
 * - nums[0..i] 全部 +k
 * - nums[i+1..n-1] 全部 -k
 * 枚举 i，答案 = min over all i of:
 *   max(nums[i]+k, nums[n-1]-k) - min(nums[0]+k, nums[i+1]-k)
 *
 * ==================== 复杂度总结 ====================
 *
 * | 解法             | 时间       | 空间  | 备注              |
 * |-----------------|-----------|-------|-------------------|
 * | 数学公式         | O(n)      | O(1)  | 面试首选           |
 * | 单次遍历         | O(n)      | O(1)  | 最优              |
 * | minmax_element   | O(n)      | O(1)  | C++ STL 最佳实践  |
 * | 排序法           | O(n logn) | O(1)  | 不推荐            |
 */
