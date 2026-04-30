// LeetCode 757: Set Intersection Size At Least Two
// 解法：贪心 + 排序
// 时间复杂度：O(n log n)  空间复杂度：O(1)

// ===================== 解法一：贪心 + 排序（推荐） =====================

class Solution {
public:
    int intersectionSizeTwo(vector<vector<int>>& intervals) {
        // 步骤1：排序
        // 按右端点升序排序，右端点相同则按左端点降序排序
        sort(intervals.begin(), intervals.end(), [](const vector<int>& a, const vector<int>& b) {
            if (a[1] != b[1]) return a[1] < b[1];  // 右端点升序
            return a[0] > b[0];                      // 左端点降序
        });

        // 步骤2：维护已选集合中最大的两个值
        int p1 = -1;  // 次大值
        int p2 = -1;  // 最大值
        int result = 0;

        // 步骤3：遍历每个区间进行贪心选择
        for (const auto& interval : intervals) {
            int left = interval[0];
            int right = interval[1];

            if (left > p2) {
                // 情况A：已选的两个最大值都不在当前区间内
                // 需要新选2个点，选区间右端的两个值
                p1 = right - 1;
                p2 = right;
                result += 2;
            } else if (left > p1) {
                // 情况B：只有 p2 在当前区间内
                // 需要再选1个点，选区间右端点
                p1 = p2;         // 原来的 p2 变成次大值
                p2 = right;      // 新选的 right 成为最大值
                result += 1;
            }
            // 情况C：p1 和 p2 都在 [left, right] 内，无需操作
        }

        return result;
    }
};


// ===================== 解法二：用集合模拟（直观但稍慢） =====================
// 更直观的实现方式，用 vector 记录已选的所有点
// 适合理解思路，但效率略低

class Solution2 {
public:
    int intersectionSizeTwo(vector<vector<int>>& intervals) {
        // 排序：右端点升序，右端点相同则左端点降序
        sort(intervals.begin(), intervals.end(), [](const vector<int>& a, const vector<int>& b) {
            if (a[1] != b[1]) return a[1] < b[1];
            return a[0] > b[0];
        });

        vector<int> selected;  // 已选点的有序列表

        for (const auto& interval : intervals) {
            int left = interval[0];
            int right = interval[1];

            // 计算当前区间内有多少个已选的点
            int count = 0;
            // 由于 selected 是有序的，可以从末尾往前数
            for (int i = (int)selected.size() - 1; i >= 0 && selected[i] >= left; i--) {
                count++;
                if (count >= 2) break;  // 已经够了
            }

            // 根据缺少的数量在右端补充
            int need = 2 - count;
            for (int i = 0; i < need; i++) {
                // 选 right, right-1, ... 但要避免重复
                int val = right - i;
                // 检查是否已选过（右端点可能已被选过）
                if (count > 0 && !selected.empty() && selected.back() == val) {
                    // 当前值已经被选了，往左偏移
                    val--;
                }
                selected.push_back(val);
            }
            // 保持有序
            sort(selected.begin(), selected.end());
        }

        return selected.size();
    }
};


// ===================== 解法三：更清晰的贪心实现 =====================
// 用 largest 和 second_largest 变量名更直观

class Solution3 {
public:
    int intersectionSizeTwo(vector<vector<int>>& intervals) {
        int n = intervals.size();

        // 排序策略：右端点升序，右端点相同则左端点降序
        sort(intervals.begin(), intervals.end(), [](const vector<int>& a, const vector<int>& b) {
            return a[1] < b[1] || (a[1] == b[1] && a[0] > b[0]);
        });

        int result = 0;
        int second = -1;  // 已选集中第二大的值
        int first = -1;   // 已选集中最大的值

        for (int i = 0; i < n; i++) {
            int lo = intervals[i][0];
            int hi = intervals[i][1];

            // 检查已选的两个最大值有多少落入 [lo, hi]
            bool firstIn = (first >= lo && first <= hi);
            bool secondIn = (second >= lo && second <= hi);

            if (firstIn && secondIn) {
                // 两个都在区间内，无需新增
                continue;
            } else if (firstIn) {
                // 只有最大值在区间内，需新增1个点
                // 选 hi（右端点）以最大化复用
                second = first;
                first = hi;
                result++;
            } else {
                // 两个都不在区间内，需新增2个点
                second = hi - 1;
                first = hi;
                result += 2;
            }
        }

        return result;
    }
};


// ===================== 关键细节说明 =====================
//
// 1. 排序策略的重要性：
//    - 右端点升序：从左到右处理区间，让选的点值尽量小
//    - 左端点降序（右端点相同时）：让更宽的区间先处理
//    这两条保证了贪心选择的最优性
//
// 2. 只维护两个变量的正确性：
//    - 排序后，后续区间的右端点 >= 当前区间的右端点
//    - 只有已选集中最大的两个值才有可能落入后续区间
//    - 更小的值不可能落入后续区间的范围内（因为后续左端点可能更大）
//
// 3. 选右端点的贪心直觉：
//    - 选值尽量大的点，让它更容易被后续区间（范围更靠右）复用
//    - 这与"最少会议室"等经典贪心问题异曲同工
//
// 4. 三种情况的判断：
//    情况A (left > p2): 需要2个新点 -> result += 2
//    情况B (left > p1): 需要1个新点 -> result += 1
//    情况C (left <= p1): 无需新点   -> result += 0
