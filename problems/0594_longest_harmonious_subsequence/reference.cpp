// LeetCode 594: Longest Harmonious Subsequence
// -----------------------------------------------
// 解法一：哈希表计数 (推荐)
// 思路：和谐子序列只包含两种值 x 和 x+1
//       统计每个数字出现次数，枚举每对相邻值取最大和
// 时间 O(n)  空间 O(n)

class Solution {
public:
    int findLHS(vector<int>& nums) {
        unordered_map<int, int> count;
        for (int num : nums) {
            count[num]++;
        }

        int result = 0;
        for (auto& [val, cnt] : count) {
            // 只查 val+1 避免重复
            if (count.count(val + 1)) {
                result = max(result, cnt + count[val + 1]);
            }
        }
        return result;
    }
};


// -----------------------------------------------
// 解法二：排序 + 滑动窗口
// 思路：排序后相同值聚集，用双指针维护 max-min <= 1 的窗口
//       只有差恰好为 1 时更新答案
// 时间 O(n log n)  空间 O(1) (不算排序)

class Solution2 {
public:
    int findLHS(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        int result = 0, left = 0;

        for (int right = 0; right < (int)nums.size(); right++) {
            // 收缩左端：差 > 1 时移动
            while (nums[right] - nums[left] > 1) {
                left++;
            }
            // 注意：差必须恰好为 1，差为 0 不算和谐子序列
            if (nums[right] - nums[left] == 1) {
                result = max(result, right - left + 1);
            }
        }
        return result;
    }
};


// -----------------------------------------------
// 解法三：一遍哈希表 (边插入边查询)
// 思路：每插入一个元素，立即向两侧查询 num-1 和 num+1
//       可以在单次遍历中完成
// 时间 O(n)  空间 O(n)

class Solution3 {
public:
    int findLHS(vector<int>& nums) {
        unordered_map<int, int> count;
        int result = 0;

        for (int num : nums) {
            count[num]++;

            // 向左查：与 num-1 配对
            if (count.count(num - 1)) {
                result = max(result, count[num] + count[num - 1]);
            }
            // 向右查：与 num+1 配对
            if (count.count(num + 1)) {
                result = max(result, count[num] + count[num + 1]);
            }
        }
        return result;
    }
};
