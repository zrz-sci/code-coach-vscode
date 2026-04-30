/*
 * 【题目本质】
 * 统计排序后满足 a + b + c < target 的三元组数量 —— 排序 + 双指针批量计数
 *
 * 【解法总览】
 * 解法1: 排序 + 双指针    O(n^2)      / O(1)  ⭐推荐
 * 解法2: 排序 + 二分搜索  O(n^2 logn) / O(1)
 */

// ===================== 解法1: 排序 + 双指针 =====================
// 思路:
//   排序后固定 nums[i]，用 left/right 双指针在 [i+1, n-1] 区间扫描
//   关键: 若 nums[i]+nums[left]+nums[right] < target
//         则 right 从 left+1 到当前 right 都满足（因为更小的 right 和更小）
//         一次性加 right - left 个
//
// 示例 [-2,0,1,3], target=2:
//   i=0 (-2): left=1(0), right=3(3) → sum=1<2 → count+=2, left=2
//             left=2(1), right=3(3) → sum=2>=2 → right=2, 退出
//   i=1 (0):  left=2(1), right=3(3) → sum=4>=2 → right=2, 退出
//   答案: 2
class Solution1 {
public:
    int threeSumSmaller(vector<int>& nums, int target) {
        int n = nums.size();
        if (n < 3) return 0;

        sort(nums.begin(), nums.end());
        int count = 0;

        for (int i = 0; i < n - 2; i++) {
            int left = i + 1, right = n - 1;
            while (left < right) {
                int sum = nums[i] + nums[left] + nums[right];
                if (sum < target) {
                    // 固定 left，right 可以是 left+1 到当前 right 中的任意值
                    count += right - left;
                    left++;
                } else {
                    right--;
                }
            }
        }
        return count;
    }
};

// ===================== 解法2: 排序 + 二分搜索 =====================
// 思路: 固定 i 和 j，二分查找 nums[j+1..n-1] 中最大的 k 使得
//       nums[i] + nums[j] + nums[k] < target
//       即 nums[k] < target - nums[i] - nums[j]
//       用 lower_bound 找到 remain 的位置，前面的元素都满足
class Solution2 {
public:
    int threeSumSmaller(vector<int>& nums, int target) {
        int n = nums.size();
        if (n < 3) return 0;

        sort(nums.begin(), nums.end());
        int count = 0;

        for (int i = 0; i < n - 2; i++) {
            for (int j = i + 1; j < n - 1; j++) {
                int remain = target - nums[i] - nums[j];
                // lower_bound 找第一个 >= remain 的位置
                auto it = lower_bound(nums.begin() + j + 1, nums.end(), remain);
                int k = it - nums.begin() - 1; // 最后一个 < remain 的位置
                if (k > j) {
                    count += k - j;
                }
            }
        }
        return count;
    }
};

/*
 * 【解法对比】
 * ┌────────────────┬────────────┬──────┬──────────────────┐
 * │ 解法           │ 时间       │ 空间 │ 特点             │
 * ├────────────────┼────────────┼──────┼──────────────────┤
 * │ 排序+双指针    │ O(n^2)     │ O(1) │ 最优，面试首选   │
 * │ 排序+二分      │ O(n^2logn) │ O(1) │ 思路直观         │
 * └────────────────┴────────────┴──────┴──────────────────┘
 *
 * 【易错点】
 * 1. count += right - left，不是 count++
 *    理由: 排序后固定 left，right 取 [left+1, right] 中任意值都满足
 * 2. 是严格小于 <，不是 <=
 * 3. 外层循环 i < n-2（至少留两个元素给 left/right）
 * 4. 二分写法中 lower_bound 找的是 >= remain 的位置，需要 -1
 *
 * 【面试追问】
 * Q1: 为什么可以排序？ → 只问数量，不问下标
 * Q2: 与 3Sum 双指针区别？ → 3Sum 找等于需去重，这里找小于需批量计数
 * Q3: 如何扩展到 kSum Smaller？ → 固定 k-2 个数，最内层双指针
 */
