// LeetCode 628: Maximum Product of Three Numbers
// 方法：排序后比较两端组合
// 时间复杂度: O(n log n), 空间复杂度: O(1)

class Solution {
public:
    int maximumProduct(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        int n = nums.size();
        // 候选1: 最大的三个数
        int cand1 = nums[n-1] * nums[n-2] * nums[n-3];
        // 候选2: 最小的两个负数 × 最大的正数
        int cand2 = nums[0] * nums[1] * nums[n-1];
        return max(cand1, cand2);
    }
};
