/*
 * 【题目本质】
 * 设计数据结构：动态插入整数，随时输出当前所有数字组成的不相交区间列表。
 * 核心是用有序容器维护区间集合，插入时定位 + 合并相邻区间。
 *
 * 【解法总览】
 * 解法1: 有序Map维护区间    addNum O(log n), getIntervals O(k)  ⭐推荐
 * 解法2: Set存所有数字      addNum O(log n), getIntervals O(n)   简单直接
 */

// ===================== Solution 1: 有序 Map 维护区间 =====================
// 思路：map<int,int> 存 {start -> end}
//   addNum 时用 upper_bound 定位，检查前驱和后继区间是否可合并
//   4 种情况：前合并、后合并、前后都合并、新建
// addNum: O(log n), getIntervals: O(区间数)
class SummaryRanges1 {
    map<int, int> intervals;  // start -> end，有序

public:
    SummaryRanges1() {}

    void addNum(int val) {
        // 找第一个 start > val 的区间
        auto it = intervals.upper_bound(val);

        bool mergeLeft = false, mergeRight = false;
        auto prevIt = it;

        // 检查前一个区间
        if (it != intervals.begin()) {
            --prevIt;
            if (prevIt->second >= val) return;  // val 已在前区间内，忽略
            if (prevIt->second + 1 == val) mergeLeft = true;
        }

        // 检查后一个区间
        if (it != intervals.end() && it->first == val + 1) {
            mergeRight = true;
        }

        if (mergeLeft && mergeRight) {
            // 前后都能合并：扩展前区间的 end，删除后区间
            prevIt->second = it->second;
            intervals.erase(it);
        } else if (mergeLeft) {
            // 只与前区间合并
            prevIt->second = val;
        } else if (mergeRight) {
            // 只与后区间合并：新建 [val, it->second]，删除旧后区间
            int end = it->second;
            intervals.erase(it);
            intervals[val] = end;
        } else {
            // 不合并：新建 [val, val]
            intervals[val] = val;
        }
    }

    vector<vector<int>> getIntervals() {
        vector<vector<int>> res;
        res.reserve(intervals.size());
        for (auto& [s, e] : intervals) {
            res.push_back({s, e});
        }
        return res;
    }
};

// ===================== Solution 2: Set 存所有数字 =====================
// 思路：用 set<int> 存所有插入的数字（自动去重 + 有序）
//   getIntervals 时遍历 set，将连续数字归为一个区间
// addNum: O(log n), getIntervals: O(n)
// 优点：代码极其简单
// 缺点：getIntervals 每次都要遍历所有数字，数据量大时慢
class SummaryRanges2 {
    set<int> nums;

public:
    SummaryRanges2() {}

    void addNum(int val) {
        nums.insert(val);
    }

    vector<vector<int>> getIntervals() {
        vector<vector<int>> res;
        if (nums.empty()) return res;

        auto it = nums.begin();
        int start = *it, end = *it;

        for (++it; it != nums.end(); ++it) {
            if (*it == end + 1) {
                end = *it;  // 连续，扩展当前区间
            } else {
                res.push_back({start, end});  // 不连续，保存当前区间
                start = end = *it;            // 开始新区间
            }
        }
        res.push_back({start, end});  // 别忘最后一个区间

        return res;
    }
};

/*
 * 【解法对比】
 * | 解法 | addNum | getIntervals | 优势 | 劣势 |
 * |------|--------|-------------|------|------|
 * | Solution1 | O(log n) | O(k) | 查询快，k 为区间数 | 合并逻辑复杂 |
 * | Solution2 | O(log n) | O(n) | 代码极简 | 查询遍历所有数字 |
 *
 * Follow-up 答案：大量合并时区间数 k << n，Solution1 的 getIntervals
 * 只需 O(k) 而 Solution2 需要 O(n)，因此 Solution1 更优。
 *
 * 【易错点】
 * 1. 重复数字：val 已在某区间 [start, end] 内时必须跳过
 * 2. 合并后区间时，需要删除旧区间再插入新的，注意 iterator 失效
 * 3. Solution1 中 mergeRight 时要先保存 end 再 erase，否则 it 失效
 * 4. Solution2 中 getIntervals 结束后别忘 push_back 最后一个区间
 * 5. 空数据流时 getIntervals 应返回空数组
 *
 * 【面试追问】
 * Q1: 大量合并时哪种更优？ → Map 方案，区间数 k << n 时 getIntervals O(k)
 * Q2: 能否用并查集？ → 可以，union(val, val-1) 和 union(val, val+1)
 * Q3: 如果 value 范围很小(≤10^4)，有无更简单方案？ → 布尔数组 + 遍历
 */
