/*
 * LeetCode 57: 插入区间 (Insert Interval)
 *
 * 【题目本质】
 * 在已排序、无重叠的区间列表中插入一个新区间，合并所有重叠部分。
 * 核心：区间天然分成"左边不重叠"、"中间重叠需合并"、"右边不重叠"三段。
 *
 * 【解法总览】
 * 解法1: 三段式一次遍历 — O(n) / O(n) — 面试首选 ⭐
 * 解法2: 插入后合并(复用LC56) — O(n) / O(n) — 思路直觉
 * 解法3: 二分查找定位边界 — O(log n + n) / O(n) — 追问加分项
 */

// ============================================================
// 解法1: 三段式一次遍历 — 面试首选
// 时间: O(n)  空间: O(n) (结果数组)
//
// 【思路】
// 区间已按 start 排序且无重叠，新区间把原列表分成三段：
//   - 左段：end < newInterval.start → 完全在新区间左边，无交集
//   - 中段：start <= newInterval.end → 与新区间有交集，需要合并
//   - 右段：剩余的 → 完全在新区间右边，无交集
//
// 示例:
//   intervals: [1,2] [3,5] [6,7] [8,10] [12,16]
//   newInterval: [4,8]
//
//   左段: [1,2]  (end=2 < 4=newStart → 不重叠)
//   中段: [3,5] [6,7] [8,10]  (合并为 [min(4,3), max(8,10)] = [3,10])
//   右段: [12,16]
//
//   结果: [1,2] [3,10] [12,16]
// ============================================================
class Solution1 {
public:
    vector<vector<int>> insert(vector<vector<int>>& intervals, vector<int>& newInterval) {
        vector<vector<int>> result;
        int i = 0, n = intervals.size();

        // 阶段1: 左段 — 所有 end < newInterval.start 的区间
        // 为什么用严格小于：end == start 意味着端点相接，算重叠需合并
        while (i < n && intervals[i][1] < newInterval[0]) {
            result.push_back(intervals[i]);
            i++;
        }

        // 阶段2: 中段 — 所有与 newInterval 有交集的区间，逐个合并
        // 重叠条件：当前区间 start <= newInterval 的 end
        // 注意：newInterval 在合并过程中不断扩展，循环条件用的是最新值
        while (i < n && intervals[i][0] <= newInterval[1]) {
            newInterval[0] = min(newInterval[0], intervals[i][0]);
            newInterval[1] = max(newInterval[1], intervals[i][1]);
            i++;
        }
        // 合并完成，把最终的合并区间加入结果
        result.push_back(newInterval);

        // 阶段3: 右段 — 剩余区间全部在新区间右边
        while (i < n) {
            result.push_back(intervals[i]);
            i++;
        }

        return result;
    }
};

// ============================================================
// 解法2: 插入后合并 (复用 LeetCode 56 合并区间)
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 如果已经会做 LC56（合并区间），最直觉的做法：
// 1. 先把 newInterval 按 start 插入到正确位置（保持排序）
// 2. 然后对整个列表执行"合并区间"操作
//
// 优点：思路简单，代码可复用；缺点：需要 vector::insert (O(n) 移动)
// ============================================================
class Solution2 {
public:
    vector<vector<int>> insert(vector<vector<int>>& intervals, vector<int>& newInterval) {
        // 找到 newInterval 应该插入的位置（按 start 排序）
        auto it = intervals.begin();
        while (it != intervals.end() && (*it)[0] < newInterval[0]) {
            ++it;
        }
        intervals.insert(it, newInterval);

        // 合并区间 — 和 LC56 完全相同的逻辑
        vector<vector<int>> result;
        for (auto& interval : intervals) {
            if (result.empty() || result.back()[1] < interval[0]) {
                // 不重叠：上一个区间的 end < 当前区间的 start
                result.push_back(interval);
            } else {
                // 重叠：扩展上一个区间的 end
                result.back()[1] = max(result.back()[1], interval[1]);
            }
        }
        return result;
    }
};

// ============================================================
// 解法3: 二分查找定位边界
// 时间: O(log n + n)  空间: O(n)
//
// 【思路】
// 解法1 的阶段1和阶段3是线性扫描找边界。利用区间已排序的特点，
// 可以用二分查找更快定位：
//   - left: 第一个可能与 newInterval 重叠的区间索引
//           即第一个 end >= newInterval.start 的区间
//   - right: 最后一个可能与 newInterval 重叠的区间索引
//            即最后一个 start <= newInterval.end 的区间
//
// 二分过程示例:
//   intervals: [1,2] [3,5] [6,7] [8,10] [12,16]
//   newInterval: [4,8]
//
//   找 left (第一个 end >= 4):
//     ends = [2, 5, 7, 10, 16]
//     lo=0 hi=4 mid=2: ends[2]=7 >= 4 → left=2, hi=1
//     lo=0 hi=1 mid=0: ends[0]=2 < 4  → lo=1
//     lo=1 hi=1 mid=1: ends[1]=5 >= 4 → left=1, hi=0
//     结果: left=1 → intervals[1]=[3,5]
//
//   找 right (最后一个 start <= 8):
//     starts = [1, 3, 6, 8, 12]
//     lo=0 hi=4 mid=2: starts[2]=6 <= 8 → right=2, lo=3
//     lo=3 hi=4 mid=3: starts[3]=8 <= 8 → right=3, lo=4
//     lo=4 hi=4 mid=4: starts[4]=12 > 8 → hi=3
//     结果: right=3 → intervals[3]=[8,10]
//
//   合并 [left=1, right=3]: min(4,3)=3, max(8,10)=10 → [3,10]
//   结果: [1,2] + [3,10] + [12,16]
// ============================================================
class Solution3 {
public:
    vector<vector<int>> insert(vector<vector<int>>& intervals, vector<int>& newInterval) {
        int n = intervals.size();
        if (n == 0) return {newInterval};

        // 二分找 left: 第一个 intervals[i][1] >= newInterval[0]
        int lo = 0, hi = n - 1, left = n;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (intervals[mid][1] >= newInterval[0]) {
                left = mid;       // 可能是答案，继续往左找更小的
                hi = mid - 1;
            } else {
                lo = mid + 1;     // 一定不是答案，排除左半
            }
        }

        // 二分找 right: 最后一个 intervals[i][0] <= newInterval[1]
        lo = 0; hi = n - 1;
        int right = -1;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (intervals[mid][0] <= newInterval[1]) {
                right = mid;      // 可能是答案，继续往右找更大的
                lo = mid + 1;
            } else {
                hi = mid - 1;     // 一定不是答案，排除右半
            }
        }

        vector<vector<int>> result;

        // 左段: [0, left-1] 这些区间完全在 newInterval 左边
        for (int i = 0; i < left; i++) {
            result.push_back(intervals[i]);
        }

        // 合并段: [left, right] 的区间与 newInterval 合并
        if (left <= right) {
            // 取所有重叠区间和 newInterval 的最小 start、最大 end
            newInterval[0] = min(newInterval[0], intervals[left][0]);
            newInterval[1] = max(newInterval[1], intervals[right][1]);
        }
        // left > right 说明没有区间与 newInterval 重叠，直接插入即可
        result.push_back(newInterval);

        // 右段: [right+1, n-1] 这些区间完全在 newInterval 右边
        for (int i = right + 1; i < n; i++) {
            result.push_back(intervals[i]);
        }

        return result;
    }
};

// ============================================================
// 【解法对比】
//
// | 特性          | 解法1:三段式   | 解法2:插入+合并 | 解法3:二分查找 |
// |---------------|---------------|-----------------|---------------|
// | 时间          | O(n)          | O(n)            | O(log n + n)  |
// | 空间          | O(n)          | O(n)            | O(n)          |
// | 代码量        | 最少          | 中等            | 较多          |
// | 面试推荐      | ⭐ 首选       | 复用思路        | 追问加分      |
// | 核心优势      | 简洁直觉      | 复用LC56        | 展示二分能力  |
//
// 三种解法本质相同：找到重叠段、合并、拼接。
// 区别在于"找重叠段"的方式：线性扫描 vs 二分定位。
// 由于结果数组构建本身就是 O(n)，二分只优化了常数。
//
// ============================================================
// 【易错点】
//
// 1. 重叠判断用 < 还是 <=:
//    ✗ while (i < n && intervals[i][1] <= newInterval[0])
//      → [1,3] 和 [3,5]: end=3 <= start=3 判为不重叠，但它们共享端点3，应该合并
//    ✓ while (i < n && intervals[i][1] < newInterval[0])
//      → 严格小于才是真正的"完全不重叠"
//
// 2. 阶段2循环中 newInterval 未更新:
//    ✗ 把 newInterval 拷贝给 merged，合并时更新 merged
//      但循环条件仍用原始 newInterval → 可能提前退出，漏掉后续重叠区间
//    ✓ 直接在 newInterval 上修改，保证循环条件始终用最新的合并结果
//
// 3. 忘记 push 合并结果:
//    ✗ 阶段2只做合并但没有 result.push_back(newInterval)
//    ✓ 阶段2结束后必须把合并后的 newInterval 加入结果
//
// 4. 解法3中 left > right 的情况:
//    ✗ 没有处理 left > right（没有重叠区间），直接取 intervals[left] 越界
//    ✓ left > right 时跳过合并，直接 push newInterval
//
// ============================================================
// 【面试追问】
//
// Q1: 和 LeetCode 56 的关系？
// → 56 是给无序区间排序后合并所有重叠区间。
//   57 是已排序区间中插入一个新区间后合并。
//   57 可以看做 56 的特殊情况：已排序 + 只处理一个新区间。
//   解法2 直接复用 56 的合并逻辑。
//
// Q2: 如果频繁插入呢？每次 O(n) 太慢。
// → 用 std::map<int,int> (有序映射) 维护 {start: end}。
//   插入时 lower_bound 定位，合并相邻区间。
//   单次操作 O(log n + k)，k 是被合并的区间数。
//   这就是 LC715 Range Module 的思路。
//
// Q3: 能否原地修改 intervals，不用额外 O(n) 空间？
// → 可以用双指针：读指针遍历原数组，写指针标记结果位置。
//   合并段覆盖到 intervals[write]，最后 resize。
//   但 vector 缩减仍需移动元素，时间不变，只省结果数组空间。
// ============================================================
