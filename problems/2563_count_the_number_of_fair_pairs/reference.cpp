/*
 * LeetCode 2563: Count the Number of Fair Pairs
 * 统计公平数对的数目
 *
 * 难度: Medium
 * 标签: Array, Two Pointers, Binary Search, Sorting
 *
 * 题意:
 *   给定数组 nums 和整数 lower, upper
 *   统计满足 0 <= i < j < n 且 lower <= nums[i]+nums[j] <= upper 的 (i,j) 对数
 *
 * 核心思路: 排序 + 区间分解 + 双指针
 *
 *   排序不影响答案(统计的是满足和约束的对的数量)
 *   区间分解: count(sum in [lower, upper])
 *           = count(sum <= upper) - count(sum <= lower-1)
 *
 * 双指针示意图:
 *
 *   countPairs(bound=6), 排序后: [0, 1, 4, 4, 5, 7]
 *
 *   L=0, R=5: 0+7=7 > 6     → R--
 *   L=0, R=4: 0+5=5 <= 6    → count+=4, L++   (0能和1,4,4,5配)
 *   L=1, R=4: 1+5=6 <= 6    → count+=3, L++   (1能和4,4,5配)
 *   L=2, R=4: 4+5=9 > 6     → R--
 *   L=2, R=3: 4+4=8 > 6     → R--
 *   L=2, R=2: L >= R 结束    count(6) = 7
 *
 *   countPairs(bound=2), 排序后: [0, 1, 4, 4, 5, 7]
 *   L=0, R=5: 0+7=7 > 2 → R--, ..., L=0,R=1: 0+1=1<=2 → count+=1
 *   count(2) = 1
 *
 *   答案 = 7 - 1 = 6 ✓
 */

// =====================================================
// 解法1: 排序 + 双指针 (推荐)
// 时间: O(n log n)   空间: O(1)
// =====================================================
class Solution {
public:
    long long countFairPairs(vector<int>& nums, int lower, int upper) {
        // 排序: 排序后仍能正确统计满足和约束的对数
        sort(nums.begin(), nums.end());

        // 区间分解: [lower, upper] = (-inf, upper] \ (-inf, lower-1]
        return countPairs(nums, upper) - countPairs(nums, lower - 1);
    }

private:
    // 统计排序后数组中 nums[i] + nums[j] <= bound 且 i < j 的对数
    long long countPairs(vector<int>& nums, int bound) {
        long long count = 0;
        int left = 0, right = (int)nums.size() - 1;

        while (left < right) {
            if (nums[left] + nums[right] <= bound) {
                // nums[left] 能和 nums[left+1], nums[left+2], ..., nums[right] 配对
                // 因为排序后 nums[left+1..right] 都 <= nums[right]
                // 所以 nums[left] + nums[k] <= nums[left] + nums[right] <= bound
                // 对于 k in [left+1, right]
                count += right - left;
                left++;
            } else {
                // nums[left] + nums[right] > bound
                // 当前 right 太大，缩小
                right--;
            }
        }

        return count;
    }
};

/*
 * 详细执行过程 — 示例1:
 *
 * nums = [0, 1, 7, 4, 4, 5], lower = 3, upper = 6
 * 排序后: [0, 1, 4, 4, 5, 7]
 *
 * === countPairs(bound=6) ===
 *
 *   迭代1: L=0, R=5
 *     nums[0]+nums[5] = 0+7 = 7 > 6
 *     → R-- (R=4)
 *
 *   迭代2: L=0, R=4
 *     nums[0]+nums[4] = 0+5 = 5 <= 6
 *     → count += 4-0 = 4  (0配1,4,4,5)
 *     → L++ (L=1)
 *
 *   迭代3: L=1, R=4
 *     nums[1]+nums[4] = 1+5 = 6 <= 6
 *     → count += 4-1 = 3  (1配4,4,5)
 *     → L++ (L=2)
 *
 *   迭代4: L=2, R=4
 *     nums[2]+nums[4] = 4+5 = 9 > 6
 *     → R-- (R=3)
 *
 *   迭代5: L=2, R=3
 *     nums[2]+nums[3] = 4+4 = 8 > 6
 *     → R-- (R=2)
 *
 *   L=2 >= R=2, 循环结束
 *   countPairs(6) = 7
 *
 * === countPairs(bound=2) ===
 *
 *   L=0, R=5: 0+7=7 > 2 → R--
 *   L=0, R=4: 0+5=5 > 2 → R--
 *   L=0, R=3: 0+4=4 > 2 → R--
 *   L=0, R=2: 0+4=4 > 2 → R--
 *   L=0, R=1: 0+1=1 <= 2 → count += 1, L++
 *   L=1 >= R=1, 结束
 *   countPairs(2) = 1
 *
 * 最终: 7 - 1 = 6 ✓
 */


// =====================================================
// 解法2: 排序 + 二分查找
// 时间: O(n log n)   空间: O(1)
// =====================================================
class Solution2 {
public:
    long long countFairPairs(vector<int>& nums, int lower, int upper) {
        sort(nums.begin(), nums.end());
        long long count = 0;
        int n = nums.size();

        for (int i = 0; i < n; i++) {
            // 对于固定的 nums[i], 需要找 j > i 使得:
            //   lower <= nums[i] + nums[j] <= upper
            // 即:
            //   lower - nums[i] <= nums[j] <= upper - nums[i]

            int lo = lower - nums[i]; // nums[j] 的下界
            int hi = upper - nums[i]; // nums[j] 的上界

            // 在 nums[i+1..n-1] 中用二分找满足 [lo, hi] 的元素个数
            // lower_bound: 第一个 >= lo 的位置
            auto left_it = lower_bound(
                nums.begin() + i + 1, nums.end(), lo
            );
            // upper_bound: 第一个 > hi 的位置
            auto right_it = upper_bound(
                nums.begin() + i + 1, nums.end(), hi
            );

            count += (right_it - left_it);
        }

        return count;
    }
};

/*
 * 二分查找解法的执行过程:
 *
 *   排序后: [0, 1, 4, 4, 5, 7],  lower=3, upper=6
 *
 *   i=0, nums[0]=0: 需要 3 <= nums[j] <= 6
 *     搜索 [1,4,4,5,7] 中 [3,6] 范围
 *     lower_bound(3) → 指向4(idx 2)
 *     upper_bound(6) → 指向7(idx 5)
 *     count += 5-2 = 3  (4,4,5)
 *
 *   i=1, nums[1]=1: 需要 2 <= nums[j] <= 5
 *     搜索 [4,4,5,7] 中 [2,5] 范围
 *     lower_bound(2) → 指向4(idx 2)
 *     upper_bound(5) → 指向7(idx 5)
 *     count += 5-2 = 3  (4,4,5)
 *
 *   i=2, nums[2]=4: 需要 -1 <= nums[j] <= 2
 *     搜索 [4,5,7] 中 [-1,2] 范围
 *     无满足的 → count += 0
 *
 *   i=3, i=4 类似都是 0
 *
 *   总计: 3+3+0+0+0 = 6 ✓
 */


// =====================================================
// 两种方法对比
// =====================================================
/*
 * | 特性           | 双指针                  | 二分查找                |
 * |----------------|-------------------------|-------------------------|
 * | 时间复杂度     | O(n log n + 2n)         | O(n log n + n log n)    |
 * | 双指针部分     | O(n) x 2 = O(n)        | O(n log n)              |
 * | 空间复杂度     | O(1)                    | O(1)                    |
 * | 代码简洁度     | 更简洁                  | 稍长                    |
 * | 理解难度       | 区间分解需要想清楚      | 直观，逐个处理          |
 * | 实际速度       | 更快(常数因子小)        | 稍慢                    |
 *
 * 面试建议: 优先写双指针方法(解法1)。如果对区间分解不熟悉，
 *           二分查找(解法2)更直观，也能通过。
 *
 * 易错点:
 * 1. 忘记排序 → 双指针和二分都失效
 * 2. 整数溢出 → 返回 long long, 注意 count 类型
 * 3. 区间分解减一 → countPairs(lower-1) 不是 countPairs(lower)
 * 4. 二分搜索范围 → 从 i+1 开始, 避免和自己配对
 * 5. 双指针终止 → left < right, 不是 left <= right
 */

// =====================================================
// 总结
// =====================================================
/*
 * 【解法对比】
 *
 * | 解法              | 时间            | 空间  | 特点                      |
 * |-------------------|-----------------|-------|---------------------------|
 * | 双指针+区间分解   | O(n log n)      | O(1)  | 最优，面试首选            |
 * | 二分查找          | O(n log n)      | O(1)  | 更直观，常数因子稍大     |
 *
 * 注: 两种解法瓶颈都在排序 O(n log n)，双指针部分 O(n) vs 二分部分 O(n log n)
 *
 * 【易错点】
 * 1. 区间分解: count([L,U]) = count(<=U) - count(<=L-1)，减的是 L-1 不是 L
 * 2. 返回类型必须是 long long — 最坏 C(n,2) ≈ 5*10^9，超出 int 范围
 * 3. 排序不影响答案 — 统计的是"满足和约束的对的数量"，与顺序无关
 * 4. 双指针中 count += right-left 而非 +1 — 固定 left 后，
 *    [left+1, right] 中所有元素都能与 left 配对
 * 5. 双指针终止条件是 left < right — 等号时同一个元素不能自我配对
 *
 * 【面试追问】
 * Q1: 为什么排序后统计不会多算或漏算？
 *     → 题目要求 i<j，排序后我们统计的是"无序对的数量"。
 *       对于任意满足条件的原始对 (i,j)，排序后它们的值仍然存在
 *       且会被双指针恰好计数一次(因为 left<right 保证不重复)。
 *       排序只改变下标映射，不改变满足和约束的对的总数。
 *
 * Q2: 如果约束改为 nums[i]-nums[j] 在 [lower, upper] 范围内，怎么做？
 *     → 差值约束不能直接排序+双指针(排序破坏 i<j 的差值关系)。
 *       需要用归并排序统计逆序对的变体，或用 BIT/线段树。
 *       在归并过程中，左半部分的元素天然是"先出现的"(i<j)，
 *       可以用双指针统计跨半部分的合法对。时间 O(n log n)。
 *
 * Q3: 如果数组不排序，能否做到优于 O(n^2)？
 *     → 可以。用平衡 BST(如 C++ 的 ordered set/BIT) 在线维护:
 *       从左到右扫描，对每个 nums[j]，查询已插入元素中在
 *       [lower-nums[j], upper-nums[j]] 范围内的个数。
 *       时间 O(n log n)，空间 O(n)。
 */
