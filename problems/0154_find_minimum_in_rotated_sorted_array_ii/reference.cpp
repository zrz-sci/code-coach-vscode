/*
 * 【题目本质】
 * 在含重复元素的旋转排序数组中找最小值。
 * 核心是"二分 + 处理重复元素导致的无法判断情况"。
 *
 * 【解法总览】
 * 1. 二分搜索(与right比较): 平均 O(log n), 最坏 O(n), O(1) 空间
 * 2. 线性扫描:              O(n), O(1) 空间
 *
 * 【旋转数组结构图示】
 *
 *   无重复旋转:              有重复旋转:
 *     |     /|                 |     /|
 *     |    / |                 |----/ |
 *     |   /  |                 |  /   |
 *     |  /   |    /|           | /    |----
 *     | /    |   / |           |/     |  /|
 *     |/     |  /  |           |      | / |
 *     +------+--+--→           +------+---→
 *            断崖                     断崖
 *
 *   nums[mid] vs nums[right] 的三种情况:
 *   mid < right → 最小值在 [left, mid]
 *   mid > right → 最小值在 (mid, right]
 *   mid == right → 无法判断，right--
 */

// ===================== 解法1: 二分搜索 =====================
// 思路: 用 nums[mid] 与 nums[right] 比较确定搜索方向
// 为什么不与 left 比? 因为旋转后左端可能是最大值，无法正确收缩
class Solution1 {
public:
    int findMin(vector<int>& nums) {
        int left = 0, right = nums.size() - 1;

        while (left < right) {
            int mid = left + (right - left) / 2;

            if (nums[mid] < nums[right]) {
                // mid 在最小值的右侧(或就是最小值)
                // 为什么 right = mid 不是 mid-1? 因为 mid 可能就是最小值
                right = mid;
            } else if (nums[mid] > nums[right]) {
                // mid 在最小值的左侧，最小值一定在 mid 右边
                // 为什么 left = mid+1? 因为 mid 比 right 大，一定不是最小值
                left = mid + 1;
            } else {
                // nums[mid] == nums[right]: 关键! 无法判断最小值在哪边
                // 为什么 right-- 安全? 因为 nums[mid] == nums[right]，
                // 即使 right 位置是最小值，mid 位置有相同值不会丢失
                right--;
            }
        }

        return nums[left]; // left == right 时即为答案
    }
};

// ===================== 解法2: 线性扫描 =====================
// 思路: 直接遍历找最小值，作为兜底方案
// 最坏情况下二分也是O(n)，此解法实现更简单
class Solution2 {
public:
    int findMin(vector<int>& nums) {
        int minVal = nums[0];
        for (int i = 1; i < (int)nums.size(); i++) {
            if (nums[i] < minVal) {
                minVal = nums[i];
            }
        }
        return minVal;
    }
};

/*
 * 【解法对比】
 * | 解法 | 平均时间 | 最坏时间 | 空间 | 适用场景 |
 * |------|----------|----------|------|----------|
 * | 解法1 | O(log n) | O(n) | O(1) | 面试首选 |
 * | 解法2 | O(n)     | O(n) | O(1) | 兜底验证 |
 *
 * 【易错点】
 * 1. 比较 nums[mid] 和 nums[left] → 反例 [3,1,2]
 * 2. right = mid - 1 → 可能跳过最小值
 * 3. 相等时 left = mid + 1 → 可能跳过最小值
 * 4. 循环条件 left <= right → 可能死循环
 *
 * 【面试追问】
 * Q1: 与153题区别? → 增加相等分支 right--，最坏退化O(n)
 * Q2: 为什么不比较left? → 旋转后left可能最大，无法正确收缩
 * Q3: 搜索特定值(LC81)? → 同框架，有序半边判断target范围
 */
