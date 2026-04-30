/*
 * LeetCode 759: Employee Free Time
 * 参考解法：排序 + 合并区间
 *
 * 思路：将所有员工的区间扁平化后按 start 排序，
 *       扫描过程中维护当前最远 end，发现间隙即为空闲时间。
 * 时间 O(N log N)，空间 O(N)
 */

/*
// Definition for an Interval.
class Interval {
public:
    int start;
    int end;
    Interval() {}
    Interval(int _start, int _end) {
        start = _start;
        end = _end;
    }
};
*/

class Solution {
public:
    vector<Interval> employeeFreeTime(vector<vector<Interval>> schedule) {
        // 1. 扁平化所有区间
        vector<Interval> all;
        for (auto& emp : schedule) {
            for (auto& iv : emp) {
                all.push_back(iv);
            }
        }

        // 2. 按 start 排序
        sort(all.begin(), all.end(), [](const Interval& a, const Interval& b) {
            return a.start < b.start;
        });

        // 3. 扫描找间隙
        vector<Interval> res;
        int maxEnd = all[0].end;

        for (int i = 1; i < all.size(); i++) {
            if (all[i].start > maxEnd) {
                res.push_back(Interval(maxEnd, all[i].start));
            }
            maxEnd = max(maxEnd, all[i].end);
        }

        return res;
    }
};
