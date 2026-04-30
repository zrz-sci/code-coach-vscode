/*
 * 【题目本质】
 * 判断一组轴对齐矩形是否精确覆盖一个大矩形（无重叠、无空隙）。
 * 核心：面积守恒 + 顶点奇偶性验证。
 *
 * 【解法总览】
 * Solution1: 面积+顶点计数  O(n) / O(n) ⭐推荐
 * Solution2: 扫描线         O(n log n) / O(n)
 */

// ===================== Solution 1: 面积 + 顶点计数 =====================
// 思路：
// 1. 面积之和必须等于外接大矩形面积
// 2. 每个小矩形4个顶点，用set做toggle(出现偶数次删除)
//    最终set里恰好剩大矩形的4个角点
class Solution1 {
public:
    bool isRectangleCover(vector<vector<int>>& rectangles) {
        set<pair<int,int>> pts;
        int x1 = INT_MAX, y1 = INT_MAX, x2 = INT_MIN, y2 = INT_MIN;
        long area = 0;

        for (auto& r : rectangles) {
            x1 = min(x1, r[0]); y1 = min(y1, r[1]);
            x2 = max(x2, r[2]); y2 = max(y2, r[3]);
            area += (long)(r[2] - r[0]) * (r[3] - r[1]);

            pair<int,int> corners[] = {{r[0],r[1]},{r[0],r[3]},{r[2],r[1]},{r[2],r[3]}};
            for (auto& p : corners) {
                if (pts.count(p)) pts.erase(p);
                else pts.insert(p);
            }
        }

        if (area != (long)(x2 - x1) * (y2 - y1)) return false;
        return pts.size() == 4
            && pts.count({x1,y1}) && pts.count({x1,y2})
            && pts.count({x2,y1}) && pts.count({x2,y2});
    }
};

// ===================== Solution 2: 扫描线 =====================
// 思路：将矩形的左右边作为事件，按x排序
// 用有序集合维护当前活跃的y区间，检查无重叠无空隙
// 每个x事件点：移除右边到达的区间，添加左边开始的区间
// 检查活跃区间连续覆盖[y_min, y_max]
class Solution2 {
public:
    bool isRectangleCover(vector<vector<int>>& rectangles) {
        // 事件：(x, type, y1, y2)  type: 0=start, 1=end
        vector<array<int,4>> events;
        for (auto& r : rectangles) {
            events.push_back({r[0], 0, r[1], r[3]}); // 左边开始
            events.push_back({r[2], 1, r[1], r[3]}); // 右边结束
        }
        sort(events.begin(), events.end());

        multiset<pair<int,int>> active; // 活跃的y区间
        int y1 = INT_MAX, y2 = INT_MIN;
        for (auto& r : rectangles) {
            y1 = min(y1, r[1]);
            y2 = max(y2, r[3]);
        }

        int i = 0, n = events.size();
        while (i < n) {
            int curX = events[i][0];
            // 处理同一x坐标的所有事件
            while (i < n && events[i][0] == curX) {
                if (events[i][1] == 1) { // end
                    active.erase(active.find({events[i][2], events[i][3]}));
                } else { // start
                    active.insert({events[i][2], events[i][3]});
                }
                i++;
            }
            // 如果不是最后，检查active区间连续覆盖[y1,y2]
            if (i < n) {
                int prev = y1;
                for (auto& seg : active) {
                    if (seg.first != prev) return false; // 空隙
                    if (seg.first >= seg.second) return false;
                    prev = seg.second;
                }
                if (prev != y2) return false;
            }
        }
        return active.empty();
    }
};

/*
 * 【解法对比】
 * Solution1 顶点法：O(n)，代码简洁，面试首选
 * Solution2 扫描线：O(n log n)，通用性强，可扩展到求并集面积
 *
 * 【易错点】
 * 1. 面积必须用long，int会溢出（坐标±10^5 -> 面积4*10^10）
 * 2. 顶点法不仅要检查剩4个点，还要验证是正确的4个角
 * 3. 扫描线中同一x的事件需一起处理（先删后加避免误判）
 *
 * 【面试追问】
 * Q: 如果要求返回未覆盖的区域坐标呢？
 * A: 需要扫描线方法，在检测到空隙时记录(x, prev_y, gap_y)坐标。
 *    顶点法只能判断yes/no，无法定位具体缺失区域。
 */
