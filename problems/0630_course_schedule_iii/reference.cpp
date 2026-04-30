// LeetCode 630: Course Schedule III
// 方法：贪心 + 最大堆
// 时间复杂度: O(n log n), 空间复杂度: O(n)

class Solution {
public:
    int scheduleCourse(vector<vector<int>>& courses) {
        // 按截止日期升序排序
        sort(courses.begin(), courses.end(), [](const auto& a, const auto& b) {
            return a[1] < b[1];
        });

        priority_queue<int> maxHeap; // 存已选课程的 duration
        int time = 0;

        for (auto& c : courses) {
            int duration = c[0], lastDay = c[1];
            // 尝试加入当前课程
            time += duration;
            maxHeap.push(duration);

            // 如果超过截止日期，移除已选课程中最长的
            if (time > lastDay) {
                time -= maxHeap.top();
                maxHeap.pop();
            }
        }

        return maxHeap.size();
    }
};
