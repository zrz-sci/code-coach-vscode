/*
 * 【题目本质】
 * 判断一组区间是否存在重叠 —— 排序后线性扫描相邻区间即可
 *
 * 【解法总览】
 * 解法1: 排序 + 线性扫描  O(nlogn) / O(1)  ⭐推荐
 * 解法2: 暴力两两比较      O(n^2)   / O(1)
 */

// ===================== 解法1: 排序 + 线性扫描 =====================
// 思路: 按起始时间排序，检查每对相邻区间是否有重叠
// 排序后，若 prev.end > curr.start 则重叠
class Solution1 {
public:
    bool canAttendMeetings(vector<vector<int>>& intervals) {
        // 按起始时间排序（默认 vector 比较即按第一个元素排序）
        sort(intervals.begin(), intervals.end());

        // 线性扫描：检查相邻区间
        for (int i = 1; i < (int)intervals.size(); i++) {
            // 前一个会议的结束时间 > 后一个会议的开始时间 → 重叠
            if (intervals[i - 1][1] > intervals[i][0]) {
                return false;
            }
        }
        return true;
    }
};

// ===================== 解法2: 暴力两两比较 =====================
// 思路: 枚举所有区间对，检查是否有重叠
// 两个区间 [a,b) 和 [c,d) 重叠的充要条件: a < d && c < b
class Solution2 {
public:
    bool canAttendMeetings(vector<vector<int>>& intervals) {
        int n = intervals.size();
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                // 区间重叠判定: start1 < end2 && start2 < end1
                if (intervals[i][0] < intervals[j][1] &&
                    intervals[j][0] < intervals[i][1]) {
                    return false;
                }
            }
        }
        return true;
    }
};

/*
 * 【解法对比】
 * ┌────────────────┬───────────┬──────────┬────────────┐
 * │ 解法           │ 时间      │ 空间     │ 适用场景   │
 * ├────────────────┼───────────┼──────────┼────────────┤
 * │ 排序+线性扫描  │ O(nlogn)  │ O(1)     │ 面试首选   │
 * │ 暴力两两比较   │ O(n^2)    │ O(1)     │ 理解区间   │
 * └────────────────┴───────────┴──────────┴────────────┘
 *
 * 【易错点】
 * 1. 判定条件用 > 而非 >=：[1,5] 和 [5,10] 不算重叠
 * 2. 排序按 start 排序（vector 默认比较即可）
 * 3. 空数组应返回 true（没有会议当然能"参加"全部）
 * 4. 两区间重叠的通用判断: !(a >= d || c >= b) 即 a < d && c < b
 *
 * 【面试追问】
 * Q1: 最少需要几个会议室？ → LC 253，用最小堆维护各会议室最早结束时间
 * Q2: 如何输出所有冲突对？ → 排序后向后检查，直到不重叠即 break
 * Q3: 如果允许 k 个会议室呢？ → 堆大小限制为 k，超过则冲突
 */
