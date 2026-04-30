/*
 * 280. Wiggle Sort
 *
 * 【题目本质】
 * 将数组重排为摆动序列: nums[0] <= nums[1] >= nums[2] <= nums[3] ...
 * 核心洞察: 只需保证局部条件，不需要全局排序。
 * 贪心: 遍历数组，违反当前位条件时交换相邻元素即可。
 *
 * 【解法总览】
 * Solution1: 贪心单次遍历 — O(n) 时间, O(1) 空间 [推荐]
 * Solution2: 排序后交换 — O(n log n) 时间, O(1) 空间
 */

// ===================== Solution 1: 贪心单次遍历 =====================
// 思路:
// - 偶数索引 i: 应满足 nums[i] <= nums[i+1]，不满足则交换
// - 奇数索引 i: 应满足 nums[i] >= nums[i+1]，不满足则交换
// 正确性: 交换修复当前条件，不会破坏前一个条件
// 时间 O(n), 空间 O(1)
class Solution1 {
public:
    void wiggleSort(vector<int>& nums) {
        for (int i = 0; i < (int)nums.size() - 1; i++) {
            if (i % 2 == 0) {
                // 偶数位: nums[i] <= nums[i+1]
                if (nums[i] > nums[i + 1]) {
                    swap(nums[i], nums[i + 1]);
                }
            } else {
                // 奇数位: nums[i] >= nums[i+1]
                if (nums[i] < nums[i + 1]) {
                    swap(nums[i], nums[i + 1]);
                }
            }
        }
    }
};

// ===================== Solution 2: 排序后交换 =====================
// 思路: 先排序得到非递减序列，然后从 index 1 开始每两个一组交换
// 排序后: [1, 2, 3, 4, 5, 6]
// 交换后: [1, 3, 2, 5, 4, 6]  --> 满足摆动条件
// 时间 O(n log n), 空间 O(1) (原地排序)
class Solution2 {
public:
    void wiggleSort(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        // 从第 2 个元素开始，每隔 2 个交换一对
        for (int i = 1; i + 1 < (int)nums.size(); i += 2) {
            swap(nums[i], nums[i + 1]);
        }
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 特点 |
 * |------|------|------|------|
 * | Solution1 贪心 | O(n) | O(1) | 最优，单次遍历 |
 * | Solution2 排序 | O(n log n) | O(1) | 简单直观 |
 *
 * 【正确性证明 (Solution1)】
 * 偶数位 i 交换时 (nums[i] > nums[i+1]):
 *   - 前一条件: nums[i-1] >= nums[i] (奇数位已满足)
 *   - 交换后: nums[i] 变小(原 nums[i+1])
 *   - nums[i-1] >= 原nums[i] > 新nums[i] → 前一条件仍成立 ✓
 *
 * 奇数位 i 交换时 (nums[i] < nums[i+1]):
 *   - 前一条件: nums[i-1] <= nums[i] (偶数位已满足)
 *   - 交换后: nums[i] 变大(原 nums[i+1])
 *   - nums[i-1] <= 原nums[i] < 新nums[i] → 前一条件仍成立 ✓
 *
 * 【易错点】
 * 1. unsigned 减法溢出
 *    ✗ i < nums.size() - 1 (空数组时 size()-1 = UINT_MAX)
 *    ✓ i < (int)nums.size() - 1
 *
 * 2. 奇偶判断条件写反
 *    ✗ 偶数位 nums[i] < nums[i+1] 交换
 *    ✓ 偶数位 nums[i] > nums[i+1] 交换 (偶数位应小)
 *
 * 3. 混淆 280 和 324
 *    ✗ 认为需要严格不等 (<, >)
 *    ✓ 280 是非严格 (<=, >=)，324 才是严格
 *
 * 【面试追问】
 * Q1: 为什么 O(n) 贪心就够了?
 *     --> 摆动排序只要求局部条件，不要求全局有序
 *     --> 每次交换修复当前位，不破坏前面
 *
 * Q2: Wiggle Sort II (324) 为什么不能用同样方法?
 *     --> 严格不等要求更强，简单交换可能导致相等元素无法满足
 *     --> 需要 O(n) 中位数 + 三路划分，或排序后穿插
 *
 * Q3: 如果要求最小交换次数呢?
 *     --> 当前方法最多 n-1 次交换，已是最优
 *     --> 每次交换至多修复一个违反
 */
