// LeetCode 632: Smallest Range Covering Elements from K Lists
// 方法：最小堆
// 时间复杂度: O(N log k), 空间复杂度: O(k)

class Solution {
public:
    vector<int> smallestRange(vector<vector<int>>& nums) {
        // 最小堆: {值, 列表索引, 列表内索引}
        auto cmp = [](const auto& a, const auto& b) {
            return a[0] > b[0];
        };
        priority_queue<vector<int>, vector<vector<int>>, decltype(cmp)> minHeap(cmp);

        int curMax = INT_MIN;
        // 初始化：每个列表的第一个元素入堆
        for (int i = 0; i < nums.size(); i++) {
            minHeap.push({nums[i][0], i, 0});
            curMax = max(curMax, nums[i][0]);
        }

        int ansL = 0, ansR = INT_MAX;

        while (true) {
            auto top = minHeap.top();
            minHeap.pop();
            int curMin = top[0], listIdx = top[1], elemIdx = top[2];

            // 更新最优区间
            if (curMax - curMin < ansR - ansL) {
                ansL = curMin;
                ansR = curMax;
            }

            // 该列表已到末尾，无法继续覆盖所有列表
            if (elemIdx + 1 >= nums[listIdx].size()) break;

            // 移动该列表指针
            int nextVal = nums[listIdx][elemIdx + 1];
            minHeap.push({nextVal, listIdx, elemIdx + 1});
            curMax = max(curMax, nextVal);
        }

        return {ansL, ansR};
    }
};
