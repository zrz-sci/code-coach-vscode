// 747. Largest Number At Least Twice of Others
// 难度: Easy
// 标签: Array, Sorting

// ============================================================
// 解法一：一次遍历找最大和第二大 - 推荐
// 时间: O(n)  空间: O(1)
// ============================================================
// 核心优化：只需要比较最大值和第二大值
// 如果 maxVal >= 2 * secondMax，则对所有其他值也满足

class Solution {
public:
    int dominantIndex(vector<int>& nums) {
        int n = nums.size();
        int maxVal = -1, secondMax = -1;
        int maxIdx = 0;

        for (int i = 0; i < n; ++i) {
            if (nums[i] > maxVal) {
                secondMax = maxVal;  // 旧的最大变成第二大
                maxVal = nums[i];
                maxIdx = i;
            } else if (nums[i] > secondMax) {
                secondMax = nums[i];
            }
        }

        // maxVal >= 2 * secondMax 则满足条件
        // 注意 secondMax 可能为 -1（初始值），但 nums[i] >= 0
        // 所以实际上 secondMax >= 0（至少被更新了一次，因为 n >= 2）
        return (maxVal >= 2 * secondMax) ? maxIdx : -1;
    }
};

// ============================================================
// 解法二：朴素两遍遍历
// 时间: O(n)  空间: O(1)
// ============================================================
// 第一遍找最大值和下标，第二遍验证所有元素

class Solution {
public:
    int dominantIndex(vector<int>& nums) {
        int n = nums.size();

        // 第一遍：找最大值和下标
        int maxIdx = 0;
        for (int i = 1; i < n; ++i) {
            if (nums[i] > nums[maxIdx]) {
                maxIdx = i;
            }
        }

        // 第二遍：验证所有其他元素
        int maxVal = nums[maxIdx];
        for (int i = 0; i < n; ++i) {
            if (i != maxIdx && maxVal < 2 * nums[i]) {
                return -1;
            }
        }

        return maxIdx;
    }
};

// ============================================================
// 解法三：排序辅助（不修改原数组）
// 时间: O(n log n)  空间: O(n)
// ============================================================
// 排序后比较最大值和第二大值

class Solution {
public:
    int dominantIndex(vector<int>& nums) {
        int n = nums.size();

        // 先记录最大值的原始下标
        int maxIdx = max_element(nums.begin(), nums.end()) - nums.begin();

        // 创建排序副本
        vector<int> sorted_nums = nums;
        sort(sorted_nums.begin(), sorted_nums.end());

        // 比较最大和第二大
        if (sorted_nums[n - 1] >= 2 * sorted_nums[n - 2]) {
            return maxIdx;
        }
        return -1;
    }
};

// ============================================================
// 解法四：使用 STL 的简洁写法
// 时间: O(n)  空间: O(1)
// ============================================================

class Solution {
public:
    int dominantIndex(vector<int>& nums) {
        int n = nums.size();
        // 找最大值迭代器
        auto maxIt = max_element(nums.begin(), nums.end());
        int maxIdx = maxIt - nums.begin();
        int maxVal = *maxIt;

        for (int i = 0; i < n; ++i) {
            if (i != maxIdx && maxVal < 2 * nums[i]) {
                return -1;
            }
        }
        return maxIdx;
    }
};

// ============================================================
// 解法五：partial_sort 找 top-2（适用于更一般场景）
// 时间: O(n)  空间: O(n)
// ============================================================
// 使用 nth_element 找第二大，然后比较

class Solution {
public:
    int dominantIndex(vector<int>& nums) {
        int n = nums.size();
        int maxIdx = max_element(nums.begin(), nums.end()) - nums.begin();
        int maxVal = nums[maxIdx];

        // 找第二大值：遍历一次即可
        int secondMax = INT_MIN;
        for (int i = 0; i < n; ++i) {
            if (i != maxIdx) {
                secondMax = max(secondMax, nums[i]);
            }
        }

        return (maxVal >= 2 * secondMax) ? maxIdx : -1;
    }
};

// ============================================================
// 测试用例分析
// ============================================================
// 输入: nums = [3,6,1,0]
// maxVal=6, secondMax=3, maxIdx=1
// 6 >= 2*3 = 6 => true => 返回 1  正确
//
// 输入: nums = [1,2,3,4]
// maxVal=4, secondMax=3, maxIdx=3
// 4 >= 2*3 = 6 => false => 返回 -1  正确
//
// 边界: nums = [0, 0]  但题目保证最大元素唯一，不会出现
//
// 边界: nums = [1, 0]
// maxVal=1, secondMax=0, maxIdx=0
// 1 >= 2*0 = 0 => true => 返回 0  正确
//
// 边界: nums = [0, 1]
// maxVal=1, secondMax=0, maxIdx=1
// 1 >= 0 => true => 返回 1  正确
