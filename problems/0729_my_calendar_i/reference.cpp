// LeetCode 729: My Calendar I
// 解法：有序 map + 二分查找
// 每次 book O(log n)，总共 O(n log n)，空间 O(n)

class MyCalendar {
public:
    MyCalendar() {}

    bool book(int startTime, int endTime) {
        // 找到第一个 start >= startTime 的区间
        auto it = calendar.lower_bound(startTime);

        // 检查该区间是否与新区间重叠：it->first < endTime
        if (it != calendar.end() && it->first < endTime) {
            return false;
        }

        // 检查前一个区间是否与新区间重叠：prev->second > startTime
        if (it != calendar.begin()) {
            auto prev = prev(it);
            if (prev->second > startTime) {
                return false;
            }
        }

        calendar[startTime] = endTime;
        return true;
    }

private:
    map<int, int> calendar; // start -> end
};
