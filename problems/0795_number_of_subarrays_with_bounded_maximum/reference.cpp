/*
 * LeetCode 795: 区间子数组个数 (Number of Subarrays with Bounded Maximum)
 * 难度: Medium
 * 标签: Array, Two Pointers
 *
 * 【题目本质】
 * 统计最大值在 [left, right] 范围内的子数组个数。
 * 用减法拆解：count(<=right) - count(<=left-1)。
 *
 * 【解法总览】
 * 解法一: 减法 countAtMost(right) - countAtMost(left-1)  O(n) / O(1)
 * 解法二: 双指针一次遍历                                    O(n) / O(1)
 */


/*
 * ============================================================
 * 解法一: 减法原理 — countAtMost
 * ============================================================
 *
 * 思路:
 *   count(最大值 in [left,right])
 *   = count(最大值 <= right) - count(最大值 <= left-1)
 *
 *   countAtMost(bound): 遍历数组，维护连续 <= bound 的段长度 cur
 *   - nums[i] <= bound: cur++, count += cur
 *   - nums[i] > bound:  cur = 0
 *
 * 示例: nums=[2,1,4,3], left=2, right=3
 *
 *   countAtMost(3):
 *     x=2: cur=1, cnt=1  (子数组: [2])
 *     x=1: cur=2, cnt=3  (子数组: [1], [2,1])
 *     x=4: cur=0, cnt=3  (4>3, 段断裂)
 *     x=3: cur=1, cnt=4  (子数组: [3])
 *     返回 4
 *
 *   countAtMost(1):
 *     x=2: cur=0, cnt=0
 *     x=1: cur=1, cnt=1  (子数组: [1])
 *     x=4: cur=0, cnt=1
 *     x=3: cur=0, cnt=1
 *     返回 1
 *
 *   答案 = 4 - 1 = 3
 *   验证: [2], [2,1], [3]
 *
 * 时间: O(n), 空间: O(1)
 */
class Solution1 {
public:
    int numSubarrayBoundedMax(vector<int>& nums, int left, int right) {
        return countAtMost(nums, right) - countAtMost(nums, left - 1);
    }

private:
    int countAtMost(vector<int>& nums, int bound) {
        int count = 0, cur = 0;
        for (int x : nums) {
            if (x <= bound) {
                cur++;
                count += cur;
            } else {
                cur = 0;
            }
        }
        return count;
    }
};


/*
 * ============================================================
 * 解法二: 双指针 — 一次遍历
 * ============================================================
 *
 * 思路:
 *   维护两个位置:
 *   - j: 最近一个 > right 的位置 (段的左边界的前一个)
 *   - k: 最近一个在 [left, right] 的位置 (保证子数组最大值够大)
 *
 *   对于以 i 结尾的子数组:
 *   - 左端不能 <= j (否则包含 >right 的元素)
 *   - 左端不能 > k (否则所有元素 < left，最大值不够)
 *   - 有效左端: j+1 到 k，共 k-j 个 (当 k > j)
 *
 * 示例: nums=[2,1,4,3], left=2, right=3
 *   初始 j=-1, k=-1
 *   i=0, x=2: in [2,3] -> k=0.  k>j: count += 0-(-1) = 1
 *   i=1, x=1: < left.          k>j: count += 0-(-1) = 1. total=2
 *   i=2, x=4: > right -> j=2.  k<=j: count += 0. total=2
 *   i=3, x=3: in [2,3] -> k=3. k>j: count += 3-2 = 1. total=3
 *
 * 时间: O(n), 空间: O(1)
 */
class Solution2 {
public:
    int numSubarrayBoundedMax(vector<int>& nums, int left, int right) {
        int n = nums.size();
        int count = 0;
        int j = -1; // 最近一个 > right 的位置
        int k = -1; // 最近一个在 [left, right] 的位置

        for (int i = 0; i < n; i++) {
            if (nums[i] > right) {
                j = i;
            }
            if (nums[i] >= left && nums[i] <= right) {
                k = i;
            }
            // 以 i 结尾的有效子数组个数
            if (k > j) {
                count += k - j;
            }
        }
        return count;
    }
};


/*
 * ============================================================
 * 【解法对比】
 *
 * | 解法              | 时间 | 空间 | 特点                       |
 * |------------------|------|------|---------------------------|
 * | 减法原理 (解法一)   | O(n) | O(1) | 两次遍历，思路清晰易理解     |
 * | 双指针 (解法二)     | O(n) | O(1) | 一次遍历，面试可展示进阶思维  |
 *
 * 面试推荐: 先讲解法一的减法思路，再用解法二优化为一次遍历
 *
 * ============================================================
 * 【易错点】
 *
 * 1. 减法边界错误:
 *    ✗ countAtMost(right) - countAtMost(left)
 *    ✓ countAtMost(right) - countAtMost(left - 1)
 *    (left 本身应包含在有效范围内)
 *
 * 2. cur 更新顺序:
 *    ✗ count += cur; cur++; (先加旧值再更新)
 *    ✓ cur++; count += cur; (先更新再加新值)
 *
 * 3. 双指针初始化:
 *    ✗ j = 0, k = 0
 *    ✓ j = -1, k = -1 (表示尚未遇到对应元素)
 *
 * ============================================================
 * 【面试追问】
 *
 * Q1: 减法原理为什么正确？
 * → 设 A = {最大值 <= right 的子数组}, B = {最大值 <= left-1 的子数组}。
 *   B 是 A 的子集 (最大值 <= left-1 一定 <= right)。
 *   A - B = {最大值在 [left, right] 的子数组}，正是所求。
 *
 * Q2: cur 为什么等于以当前位置结尾的子数组个数？
 * → cur 记录当前连续 <= bound 的段长度。以位置 i 结尾、
 *   所有元素 <= bound 的子数组，左端可以从 i-cur+1 到 i，
 *   共 cur 个。遇到 > bound 的元素段断裂，cur 归 0。
 *
 * Q3: 和 LC 992 "Subarrays with K Different Integers" 的关系？
 * → 完全相同的减法思路。992 用 atMost(k) - atMost(k-1)
 *   将"恰好 k 个不同整数"转化为两个"最多"问题。
 *   这是子数组计数题的通用技巧。
 * ============================================================
 */
