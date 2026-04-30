// LeetCode 732: My Calendar III
// 解法：差分 + 有序 map
// 每次 book O(n log n)，总 O(n^2 log n)，空间 O(n)

class MyCalendarThree {
public:
    MyCalendarThree() {}

    int book(int startTime, int endTime) {
        diff[startTime]++;
        diff[endTime]--;

        // 扫描前缀和，找最大重叠次数
        int maxK = 0, count = 0;
        for (auto& [time, delta] : diff) {
            count += delta;
            maxK = max(maxK, count);
        }
        return maxK;
    }

private:
    map<int, int> diff; // 差分数组
};
