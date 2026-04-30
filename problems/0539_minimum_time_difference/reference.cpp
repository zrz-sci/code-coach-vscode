/*
 * 【题目本质】
 * 将时间转为分钟数后，在环形数组中找最小相邻差值。
 * 一天 = 1440 分钟，需考虑跨午夜的环形差值。
 *
 * 【解法总览】
 * Solution1: 排序 + 遍历 — 排序后比较相邻差 + 首尾环形差（推荐）
 * Solution2: 桶排序 — O(n) 时间，利用时间值域有限的特性
 */

// 解法一：排序 + 遍历（推荐）
// 思路：转分钟数 → 排序 → 相邻差值最小值 → 别忘首尾环形差
// 时间 O(n log n)，空间 O(n)
class Solution1 {
public:
    int findMinDifference(vector<string>& timePoints) {
        if (timePoints.size() > 1440) return 0; // 鸽巢原理

        vector<int> minutes;
        for (auto& t : timePoints) {
            int h = stoi(t.substr(0, 2));
            int m = stoi(t.substr(3, 2));
            minutes.push_back(h * 60 + m);
        }
        sort(minutes.begin(), minutes.end());

        int minDiff = INT_MAX;
        for (int i = 1; i < minutes.size(); i++) {
            minDiff = min(minDiff, minutes[i] - minutes[i - 1]);
        }
        // 跨午夜：首尾环形差值
        minDiff = min(minDiff, 1440 - minutes.back() + minutes.front());

        return minDiff;
    }
};

// 解法二：桶排序
// 思路：1440个桶标记存在性，线性扫描找最小间隔
// 时间 O(n + 1440)，空间 O(1440)
class Solution2 {
public:
    int findMinDifference(vector<string>& timePoints) {
        vector<bool> seen(1440, false);
        for (auto& t : timePoints) {
            int mins = stoi(t.substr(0, 2)) * 60 + stoi(t.substr(3, 2));
            if (seen[mins]) return 0;
            seen[mins] = true;
        }

        int first = -1, last = -1, prev = -1;
        int minDiff = INT_MAX;
        for (int i = 0; i < 1440; i++) {
            if (!seen[i]) continue;
            if (first == -1) {
                first = i;
            } else {
                minDiff = min(minDiff, i - prev);
            }
            prev = i;
            last = i;
        }
        minDiff = min(minDiff, 1440 - last + first); // 环形
        return minDiff;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 特点 |
 * |------|------|------|------|
 * | 排序 | O(n log n) | O(n) | 通用，代码简洁 |
 * | 桶排序 | O(n+1440) | O(1440) | 利用值域有限，线性时间 |
 *
 * 【易错点】
 * 1. 必须考虑跨午夜的环形差值 1440 - back + front
 * 2. 鸽巢原理：n > 1440 直接返回 0
 * 3. 桶排序中重复时间直接返回 0
 *
 * 【面试追问】
 * Q1: 为什么排序后只看相邻？→ 非相邻差值 >= 相邻差值
 * Q2: 桶排序优劣？→ O(n) 但需 O(1440) 空间
 * Q3: 精确到秒？→ 桶扩大到 86400，方法不变
 */
