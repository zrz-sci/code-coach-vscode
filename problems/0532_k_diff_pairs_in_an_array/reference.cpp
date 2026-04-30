/*
 * 【题目本质】
 * 在数组中找出所有唯一的 k-diff 数对 (a,b)，满足 |a-b|=k
 * 核心难点：去重 + k=0 特殊处理
 *
 * 【解法总览】
 * 解法一：哈希表计数 ⭐推荐     — O(n) / O(n)
 * 解法二：排序 + 双指针          — O(n log n) / O(1)
 * 解法三：排序 + 二分查找        — O(n log n) / O(1)
 */

// ===================== 解法一：哈希表计数（推荐） =====================
// 思路：统计频次，对每个不重复的num检查num+k是否存在
//       k=0时特判：频次>=2才算一对
class Solution {
public:
    int findPairs(vector<int>& nums, int k) {
        unordered_map<int, int> freq;
        for (int num : nums) freq[num]++;

        int result = 0;
        for (auto& [num, cnt] : freq) {
            if (k > 0 && freq.count(num + k)) {
                // k>0: 检查 num+k 是否存在
                result++;
            } else if (k == 0 && cnt >= 2) {
                // k=0: 同一个数出现至少2次
                result++;
            }
        }
        return result;
    }
};

// ===================== 解法二：排序 + 双指针 =====================
// 思路：排序后用两个指针维护差值恰好为k的数对
class Solution2 {
public:
    int findPairs(vector<int>& nums, int k) {
        sort(nums.begin(), nums.end());
        int n = nums.size(), result = 0;
        int left = 0, right = 1;

        while (left < n && right < n) {
            // 保证 left != right
            if (left == right || nums[right] - nums[left] < k) {
                right++;
            } else if (nums[right] - nums[left] > k) {
                left++;
            } else {
                // 找到一对：nums[right] - nums[left] == k
                result++;
                left++;
                // 跳过重复的left值，保证数对唯一
                while (left < n && nums[left] == nums[left - 1]) left++;
            }
        }
        return result;
    }
};

// ===================== 解法三：排序 + 二分查找 =====================
// 思路：排序后对每个不重复的元素，二分查找 num+k 是否存在
class Solution3 {
public:
    int findPairs(vector<int>& nums, int k) {
        sort(nums.begin(), nums.end());
        int n = nums.size(), result = 0;

        for (int i = 0; i < n; i++) {
            // 跳过重复元素
            if (i > 0 && nums[i] == nums[i - 1]) continue;
            // 在 i+1 之后的范围查找 nums[i]+k
            if (binary_search(nums.begin() + i + 1, nums.end(), nums[i] + k)) {
                result++;
            }
        }
        return result;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 优势 |
 * |------|------|------|------|
 * | 哈希表计数 | O(n) | O(n) | 最优时间，代码简洁 |
 * | 排序+双指针 | O(nlogn) | O(1) | 无额外空间 |
 * | 排序+二分 | O(nlogn) | O(1) | 思路直观 |
 *
 * 【易错点】
 * 1. k=0 特殊处理：需要找出现次数>=2的元素
 * 2. 只查 num+k 不查 num-k，避免重复计数
 * 3. 双指针中 left==right 时要移动 right
 * 4. 找到一对后跳过所有相同的 left 保证去重
 *
 * 【面试追问】
 * Q1: 为什么只查 num+k？
 *     → 遍历所有元素时，(a,a+k) 会被 a 找到，不会遗漏
 * Q2: k=0 的时间复杂度？
 *     → 仍为 O(n)，只是判断条件不同
 * Q3: 如何输出具体数对？
 *     → 在 result++ 处收集 {num, num+k}
 */
