/*
 * 【题目本质】
 * 对每个区间的 end 值，在所有 start 值中找 >= end 的最小值及其下标
 * 经典排序 + 二分查找问题
 *
 * 【解法总览】
 * 解法1: 排序 + 二分查找 ⭐ — O(n log n) / O(n) — 面试首选
 * 解法2: 有序 map — O(n log n) / O(n) — 代码更简洁
 */

// ============================================================
// 解法1: 排序 + 二分查找 ⭐
// 思路：(start, index) 排序后，对每个 end 做 lower_bound
// 时间 O(n log n)，空间 O(n)
// ============================================================
class Solution1 {
public:
    vector<int> findRightInterval(vector<vector<int>>& intervals) {
        int n = intervals.size();
        vector<pair<int, int>> starts; // (start_value, original_index)
        for (int i = 0; i < n; i++) {
            starts.push_back({intervals[i][0], i});
        }
        sort(starts.begin(), starts.end());

        vector<int> res(n);
        for (int i = 0; i < n; i++) {
            int target = intervals[i][1]; // end of current interval
            // 找第一个 start >= target
            auto it = lower_bound(starts.begin(), starts.end(),
                                  make_pair(target, INT_MIN));
            res[i] = (it == starts.end()) ? -1 : it->second;
        }
        return res;
    }
};

// ============================================================
// 解法2: 有序 map
// 思路：map<start, index> 天然有序，直接 lower_bound
// 时间 O(n log n)，空间 O(n)
// ============================================================
class Solution2 {
public:
    vector<int> findRightInterval(vector<vector<int>>& intervals) {
        map<int, int> startMap; // start -> original index
        int n = intervals.size();
        for (int i = 0; i < n; i++) {
            startMap[intervals[i][0]] = i;
        }

        vector<int> res(n);
        for (int i = 0; i < n; i++) {
            auto it = startMap.lower_bound(intervals[i][1]);
            res[i] = (it == startMap.end()) ? -1 : it->second;
        }
        return res;
    }
};

/*
 * 【解法对比】
 * 解法1: 排序 + 二分，更显式，面试更容易讲解
 * 解法2: 利用 map 内置有序性，代码更短，但 map 常数较大
 *
 * 【易错点】
 * 1. lower_bound 是 >=，upper_bound 是 >，本题需要 >=
 * 2. 排序后原始下标丢失 → 必须存 pair<start, index>
 * 3. lower_bound 对 pair 比较：先比 first 再比 second
 *    所以 target pair 的 second 用 INT_MIN 确保找到 first == target 的
 *
 * 【面试追问】
 * Q: start 不唯一怎么办？ A: 用 multimap，lower_bound 仍正确
 * Q: 动态插入区间？ A: 用 set/map 维护，每次 O(log n)
 * Q: 时间下界？ A: O(n log n)，n 次范围查询的最优
 */
