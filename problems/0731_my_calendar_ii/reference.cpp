// LeetCode 731: My Calendar II
// 解法：差分 + 有序 map
// 每次 book O(n log n)，总 O(n^2 log n)，空间 O(n)

class MyCalendarTwo {
public:
    MyCalendarTwo() {}

    bool book(int startTime, int endTime) {
        // 尝试加入差分
        diff[startTime]++;
        diff[endTime]--;

        // 扫描前缀和，检查是否有三重预订
        int count = 0;
        for (auto& [time, delta] : diff) {
            count += delta;
            if (count >= 3) {
                // 撤销
                diff[startTime]--;
                diff[endTime]++;
                // 清理零值
                if (diff[startTime] == 0) diff.erase(startTime);
                if (diff[endTime] == 0) diff.erase(endTime);
                return false;
            }
        }
        return true;
    }

private:
    map<int, int> diff; // 差分数组
};
