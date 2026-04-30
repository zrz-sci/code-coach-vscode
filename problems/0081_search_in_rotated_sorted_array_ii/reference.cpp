/*
 * LeetCode 81: 搜索旋转排序数组 II (Search in Rotated Sorted Array II)
 *
 * 【题目本质】
 * 在含重复元素的旋转有序数组中判断 target 是否存在。
 * 核心挑战：重复元素可能让我们无法判断哪半边有序，导致二分失效。
 *
 * 【和第 33 题的关系】
 * 第 33 题：元素不重复 → nums[left] <= nums[mid] 能确定左半有序
 * 第 81 题：元素可重复 → nums[left] == nums[mid] == nums[right] 时无法判断
 *          → 多加一个 if 分支，收缩边界跳过重复
 *          → 最坏时间从 O(log n) 退化到 O(n)
 *
 * 【解法总览】
 * 解法1: 暴力遍历        — O(n) / O(1) — baseline
 * 解法2: 二分查找+去重   — O(log n) 平均, O(n) 最坏 / O(1) — 面试首选
 */

// ============================================================
// 解法1: 暴力遍历
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 最直接的方法：遍历数组，看 target 是否存在。
// 虽然简单，但它揭示了一个重要事实：
// 本题最坏情况下任何算法都是 O(n)，暴力法是正确的 baseline。
// ============================================================
class Solution1 {
public:
    bool search(vector<int>& nums, int target) {
        for (int num : nums) {
            if (num == target) return true;
        }
        return false;
    }
};

// ============================================================
// 解法2: 二分查找（处理重复元素） — 面试首选
// 时间: O(log n) 平均, O(n) 最坏  空间: O(1)
//
// 【思路】
// 旋转数组的性质：被 mid 分成两半，至少有一半是有序的。
// - 如果左半有序 (nums[left] <= nums[mid])，检查 target 是否在 [left, mid) 中
// - 如果右半有序 (nums[mid] <= nums[right])，检查 target 是否在 (mid, right] 中
//
// 重复元素的问题：
// 当 nums[left] == nums[mid] == nums[right] 时，
// 我们无法判断 mid 在旋转点的哪一侧。
//
// 例子:
//   [1, 3, 1, 1, 1]  left=0, mid=2, right=4
//        ↑               → 旋转点在左半边，右半边有序
//   [1, 1, 1, 3, 1]  left=0, mid=2, right=4
//              ↑         → 旋转点在右半边，左半边有序
//   两种情况 nums[left]==nums[mid]==nums[right]==1，无法区分！
//
// 解决方案：遇到这种情况，left++; right--; 线性跳过一个重复元素。
// 这不会漏掉 target，因为 nums[mid] 已经和 target 比较过了，
// 而 nums[left]==nums[mid]!=target，所以跳过 left 是安全的。
//
// 搜索空间收缩过程:
// nums = [2,5,6,0,0,1,2], target = 0
//
// Round1: [2, 5, 6, 0, 0, 1, 2]
//          L        M        R
//         nums[mid]=0 == target → return true!
//
// nums = [1,1,1,3,1], target = 3
//
// Round1: [1, 1, 1, 3, 1]
//          L     M     R
//         nums[L]==nums[M]==nums[R]==1, 无法判断
//         → left++, right--
//
// Round2: [1, 1, 3, 1]  (逻辑上 left=1, right=3)
//             L  M  R
//         nums[mid]=3 == target → return true!
//
// ★ 和第 33 题的代码差异：仅多了下面标注 ★ 的 if 分支 ★
// ============================================================
class Solution2 {
public:
    bool search(vector<int>& nums, int target) {
        int left = 0, right = (int)nums.size() - 1;
        
        while (left <= right) {
            int mid = left + (right - left) / 2;
            
            // 找到 target，直接返回
            if (nums[mid] == target) return true;
            
            // ★★★ 这是和第 33 题唯一的核心区别 ★★★
            // 无法判断哪边有序 → 线性收缩边界
            // 安全性：nums[mid] != target（已在上面排除）
            //        nums[left] == nums[mid] → nums[left] != target → 跳过安全
            //        nums[right] == nums[mid] → nums[right] != target → 跳过安全
            if (nums[left] == nums[mid] && nums[mid] == nums[right]) {
                left++;
                right--;
            }
            // 左半边有序: nums[left] <= nums[mid]
            // 注意用 <= 不是 <，因为 left 可能等于 mid（区间只剩两个元素）
            else if (nums[left] <= nums[mid]) {
                // target 在有序的左半边 [left, mid) 中？
                if (nums[left] <= target && target < nums[mid]) {
                    right = mid - 1;  // 在左半边找
                } else {
                    left = mid + 1;   // 在右半边找
                }
            }
            // 右半边有序: nums[mid] <= nums[right]
            else {
                // target 在有序的右半边 (mid, right] 中？
                if (nums[mid] < target && target <= nums[right]) {
                    left = mid + 1;   // 在右半边找
                } else {
                    right = mid - 1;  // 在左半边找
                }
            }
        }
        
        return false;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法 | 时间 | 空间 | 适用场景 |
// |------|------|------|----------|
// | 暴力 | O(n) | O(1) | baseline，最坏情况一样 |
// | 二分 | O(log n)~O(n) | O(1) | 面试必写，平均远快于暴力 |
//
// 关键区别：二分法在大部分输入下是 O(log n)，只有极端重复时退化。
// 面试中暴力法只能拿及格分，二分法才是考察重点。
//
// 【与系列题的关系】
// 33题(无重复)代码 → 81题(有重复)代码：
//   仅在 while 循环内加一个 if 分支处理 nums[left]==nums[mid]==nums[right]
//   其余代码完全不变
//
// 153题(找最小值,无重复) → 154题(找最小值,有重复)：
//   同样的套路：多一个 if 分支，遇到 nums[mid]==nums[right] 时 right--
//
// 【易错点】
//
// 1. 忘记先判断 nums[mid] == target 就进入收缩逻辑
//    ✗ 先收缩 left++/right-- 可能跳过 target 所在位置
//    ✓ 必须先检查 nums[mid] == target，确认 mid 不是答案后才能安全收缩
//    原因：收缩时 nums[left]==nums[mid]，如果 nums[mid]==target，
//         那跳过 left 虽然没跳过 mid，但如果同时 right-- 跳过的
//         nums[right] 也等于 target……等等，其实 mid 还在区间内所以
//         实际上是安全的。但为了代码清晰和不出 bug，先判断 mid 最稳妥。
//
// 2. 判断左半有序时用 < 而非 <=
//    ✗ if (nums[left] < nums[mid])
//       当 left==mid（区间仅两个元素）时，nums[left]==nums[mid]，
//       这个条件为 false，会错误地进入右半有序的分支
//    ✓ if (nums[left] <= nums[mid])
//       覆盖 left==mid 的边界情况
//
// 3. 收缩边界时只写 left++ 不写 right--
//    ✗ 功能正确但效率差：考虑 [1,1,1,1,1,1,2]，只 left++ 需要5步
//    ✓ left++ 和 right-- 同时做，两端收缩更快
//
// 4. target 的范围判断漏掉等号
//    ✗ if (nums[left] < target && target < nums[mid])
//       当 target == nums[left] 时会被错误排除
//    ✓ if (nums[left] <= target && target < nums[mid])
//       左闭右开，因为 nums[mid] 已经比较过了
//
// 【面试追问】
//
// Q1: 和第 33 题相比，代码上改了什么？为什么必须这样改？
// A1: 多了一个 if (nums[left]==nums[mid]&&nums[mid]==nums[right])
//     的分支，做 left++; right--。因为有重复时这三个值相等让我们
//     无法判断 mid 在旋转点的哪一侧，只能退化线性跳过。
//
// Q2: 最坏时间复杂度是多少？给一个具体的最坏输入。
// A2: O(n)。例如 nums=[1,1,1,...,1], target=2。每轮只能 left++; right--，
//     需要 n/2 轮。或者 nums=[1,1,1,2,1,1,1]，target=2，在找到 2 之前
//     可能需要线性扫描大部分元素。
//
// Q3: 能否设计一个算法在最坏情况下优于 O(n)？
// A3: 不能。信息论论证：考虑 n-1 个 1 和 1 个 2 组成的数组，
//     2 可以在 n 个位置中的任意一个。在检查某个位置之前，
//     你无法排除它。因此下界就是 Ω(n)。
//
// Q4: 收缩边界时能不能只 right-- 不 left++？
// A4: 可以，正确性不受影响。但同时做两边收缩效率更高。
//     关键是 nums[mid] != target 已经确认，而 nums[left]==nums[right]==nums[mid]，
//     所以 left 和 right 指向的值都不是 target，跳过它们是安全的。
// ============================================================
