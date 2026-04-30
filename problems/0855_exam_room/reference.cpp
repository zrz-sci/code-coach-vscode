/*
 * LeetCode 855: Exam Room (考场就座)
 *
 * 【题目本质】
 * 设计一个动态座位管理系统：每次 seat() 选使得到最近人距离最大化的位置，
 * leave() 移除一个座位。核心是动态维护间隔并找最大间隔。
 *
 * 【解法总览】
 * 解法1: 有序集合(set)遍历  — seat O(k), leave O(logk)  — 面试首选
 * 解法2: 堆 + 懒删除       — seat O(logk)均摊, leave O(logk) — 进阶优化
 */

// ============================================================
// 解法1: 有序集合遍历 — 面试首选 ⭐
// seat: O(k)  leave: O(log k)  k = 当前在座人数
//
// 【思路】
// 用 set<int> 维护所有有人的座位（自动排序）。
// seat() 时遍历 set 分析所有间隔，找到使距离最大的座位：
//   1. 左端间隔: 坐位置0，距离 = first_occupied
//   2. 中间间隔: 坐中点 prev+(cur-prev)/2，距离 = (cur-prev)/2
//   3. 右端间隔: 坐位置n-1，距离 = n-1-last_occupied
// leave() 直接从 set 中删除。
//
// 间隔类型示意 (n=10, 有人位置={2,5,8}):
//   左端: [_, _, 2] → 坐位置0, 距离=2
//   中间: [2, _, _, 5] → 坐位置3, 距离=1
//         [5, _, _, 8] → 坐位置6, 距离=1
//   右端: [8, _, _] → 坐位置9, 距离=1
//   最大距离=2, 选位置0
//
// 平局处理:
//   题目要求距离相同时选编号最小的座位。
//   从左到右遍历，用严格 > 比较：相同距离不更新，保留先遇到的（编号小）。
// ============================================================
class ExamRoom1 {
private:
    set<int> seated; // 有序集合维护有人的座位
    int n;           // 座位总数

public:
    ExamRoom1(int n) : n(n) {}

    int seat() {
        // 特殊情况：空房间，坐位置0
        if (seated.empty()) {
            seated.insert(0);
            return 0;
        }

        // ===== 分析所有间隔，找最大距离的座位 =====

        // 左端间隔：坐位置0，距离 = 第一个有人位置
        int maxDist = *seated.begin();
        int bestSeat = 0;

        // 中间间隔：相邻两个有人位置之间
        int prev = -1;
        for (int s : seated) {
            if (prev != -1) {
                // 间隔 [prev, s]，最优坐在中点
                int dist = (s - prev) / 2; // 整除天然偏左
                if (dist > maxDist) {       // 严格 > 保证平局选编号小
                    maxDist = dist;
                    bestSeat = prev + (s - prev) / 2;
                }
            }
            prev = s;
        }

        // 右端间隔：坐位置 n-1，距离 = n-1-最后有人位置
        int rightDist = n - 1 - *seated.rbegin();
        if (rightDist > maxDist) {
            bestSeat = n - 1;
        }

        // 落座
        seated.insert(bestSeat);
        return bestSeat;
    }

    void leave(int p) {
        seated.erase(p);
    }
};

// ============================================================
// 解法2: 堆 + 有序集合 — 优化 seat() 到 O(log k) 均摊
// seat: O(log k) 均摊  leave: O(log k)
//
// 【思路】
// 用最大堆存储所有间隔，每个间隔记录 {距离, 座位编号, 左端, 右端}。
// seat() 时取堆顶（最大距离的间隔），验证有效性后分裂为两个新间隔。
// leave() 时合并相邻间隔，旧间隔通过懒删除处理。
//
// 懒删除机制:
//   leave(p) 不从堆中删除包含 p 的间隔，只从 set 中删除 p 并添加新合并间隔。
//   seat() 取堆顶时验证间隔是否仍有效（两端点在 set 中且相邻）。
//   无效的间隔被丢弃，取下一个堆顶。
//
// 间隔定义:
//   (left, right) 表示 left 和 right 都有人，之间是空的。
//   特殊: left=-1 表示左墙，right=n 表示右墙。
//
// 距离计算:
//   left=-1:        dist = right (坐在位置0)
//   right=n:        dist = n-1-left (坐在位置n-1)
//   otherwise:      dist = (right-left)/2 (坐在中点)
//
// 示例: n=10
//   初始堆: [(-1, 10, dist=9, seat=0)]  ← 整个房间是一个空间隔
//   seat(): 取堆顶, 坐位置0
//     分裂: (-1, 0) [无效,长度0] 和 (0, 10, dist=9, seat=9)
//     → return 0, seated={0}
//   seat(): 堆顶 (0, 10, dist=9, seat=9), 坐位置9
//     分裂: (0, 9, dist=4, seat=4)
//     → return 9, seated={0,9}
// ============================================================
class ExamRoom2 {
private:
    int n;
    set<int> seated;

    // 间隔结构体
    struct Interval {
        int dist;  // 该间隔中最优座位到最近人的距离
        int seat;  // 该间隔中最优座位编号
        int left;  // 间隔左端有人位置（-1 表示左墙）
        int right; // 间隔右端有人位置（n 表示右墙）

        // 最大堆比较：距离大优先，距离相同座位编号小优先
        bool operator<(const Interval& o) const {
            if (dist != o.dist) return dist < o.dist;
            return seat > o.seat;
        }
    };

    priority_queue<Interval> pq;

    // 计算间隔的最优距离和座位
    Interval makeInterval(int left, int right) {
        int dist, seat;
        if (left == -1) {
            dist = right; // 坐位置0
            seat = 0;
        } else if (right == n) {
            dist = n - 1 - left; // 坐位置n-1
            seat = n - 1;
        } else {
            dist = (right - left) / 2;
            seat = left + (right - left) / 2;
        }
        return {dist, seat, left, right};
    }

    // 验证间隔是否仍然有效
    bool isValid(const Interval& iv) {
        // 检查两端点是否仍在 seated 中（或是墙壁）
        bool lOk = (iv.left == -1) || seated.count(iv.left);
        bool rOk = (iv.right == n) || seated.count(iv.right);
        if (!lOk || !rOk) return false;

        // 检查两端点在 seated 中是否相邻
        if (iv.left != -1 && iv.right != n) {
            auto it = seated.find(iv.left);
            if (next(it) == seated.end() || *next(it) != iv.right) {
                return false;
            }
        } else if (iv.left == -1 && !seated.empty()) {
            if (*seated.begin() != iv.right) return false;
        } else if (iv.right == n && !seated.empty()) {
            if (*seated.rbegin() != iv.left) return false;
        }
        return true;
    }

public:
    ExamRoom2(int n) : n(n) {
        // 初始：整个房间是一个空间隔 (-1, n)
        pq.push(makeInterval(-1, n));
    }

    int seat() {
        // 取堆顶，跳过无效间隔（懒删除）
        while (!pq.empty()) {
            Interval top = pq.top();
            pq.pop();

            if (!isValid(top)) continue;

            int s = top.seat;
            seated.insert(s);

            // 分裂间隔为两个子间隔
            if (top.left != s) {
                pq.push(makeInterval(top.left, s));
            }
            if (s != top.right) {
                pq.push(makeInterval(s, top.right));
            }

            return s;
        }
        return -1; // 不会到这里
    }

    void leave(int p) {
        auto it = seated.find(p);
        // 找左右邻居
        int left = (it == seated.begin()) ? -1 : *prev(it);
        int right = (next(it) == seated.end()) ? n : *next(it);

        seated.erase(it);

        // 合并左右间隔为一个大间隔
        pq.push(makeInterval(left, right));
        // 旧的 (left, p) 和 (p, right) 间隔留在堆中，
        // 下次 seat() 取到它们时会被 isValid 检测出无效并跳过
    }
};

// ============================================================
// 【解法对比】
//
// | 解法         | seat()      | leave()   | 实现难度 | 适用场景       |
// |-------------|-------------|-----------|--------|--------------|
// | set遍历      | O(k)        | O(log k)  | 简单    | k 小(<=10^4)  |
// | 堆+懒删除    | O(logk)均摊  | O(log k)  | 中等    | k 大时更优     |
//
// n 可达 10^9 但操作只有 10^4 次 → k 最大 10^4。
// set 遍历的 O(k) = O(10^4)，完全可以接受。面试首选 set 遍历。
//
// ============================================================
// 【易错点】
//
// 1. 首尾间隔距离不除2:
//    ✗ 所有间隔都 gap/2 → 坐端点的距离被低估
//    ✓ 左端距离=first, 右端距离=n-1-last, 中间距离=(gap)/2
//
// 2. 平局选编号大的:
//    ✗ dist >= maxDist 时更新 → 后面（编号大的）覆盖前面
//    ✓ dist > maxDist 严格大于 → 保留先遇到的（编号小的）
//
// 3. 空集合没特判:
//    ✗ 直接取 *seated.begin() → 未定义行为
//    ✓ if (seated.empty()) { insert(0); return 0; }
//
// 4. 堆解法忘记验证间隔有效性:
//    ✗ 直接用堆顶 → leave 后旧间隔还在堆中，导致错误结果
//    ✓ 每次取堆顶后验证两端仍然在 set 中且相邻
//
// ============================================================
// 【相关题型】
//
// - 849. Maximize Distance to Closest Person:
//   本题的静态版本(一次查询)。间隔分析逻辑完全相同。
//
// - 295. Find Median from Data Stream:
//   设计题，动态维护有序结构。
//
// - 729. My Calendar I:
//   区间管理设计题。
// ============================================================
