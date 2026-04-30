/*
 * LeetCode 850: Rectangle Area II (矩形面积 II)
 *
 * 【题目本质】
 * 给定多个轴对齐矩形，求它们覆盖的总面积（重叠区域只算一次）。
 * 核心问题：如何高效去重计算重叠矩形的并集面积。
 *
 * 【解法总览】
 * 解法1: 坐标压缩 + 逐格检查 — O(n^3) / O(n^2) — 最直观，n<=200 可过
 * 解法2: 扫描线 + 区间合并   — O(n^2 log n) / O(n) — 面试推荐
 */

// ============================================================
// 解法1: 坐标压缩 + 逐格检查
// 时间: O(n^3)  空间: O(n^2)
//
// 【思路】
// 坐标压缩：把所有矩形的 x/y 坐标去重排序，将平面划分为 O(n^2) 个
// 小矩形格子。每个格子要么完全在某个矩形内部，要么完全不在。
// 遍历每个格子，检查它是否被任意一个输入矩形覆盖。
//
// 坐标压缩示意:
//   原始矩形: [0,0,2,2], [1,0,2,3], [1,0,3,1]
//   x坐标集合: {0, 1, 2, 3}  → 3个x区间: [0,1), [1,2), [2,3)
//   y坐标集合: {0, 1, 2, 3}  → 3个y区间: [0,1), [1,2), [2,3)
//   共 3x3=9 个小格子，逐个检查
//
// 为什么可以逐格检查？
//   坐标压缩保证了每个格子要么被完全覆盖，要么完全不被覆盖。
//   因为矩形边界恰好是划分线，格子内部不可能部分覆盖。
//
// 复杂度分析:
//   n 个矩形 → 最多 2n 个 x 坐标, 2n 个 y 坐标
//   → O(n^2) 个格子，每个格子检查 n 个矩形
//   → O(n^3)，n=200 时约 8*10^6，完全可过
// ============================================================
class Solution1 {
public:
    int rectangleArea(vector<vector<int>>& rectangles) {
        const int MOD = 1e9 + 7;

        // 收集并去重所有 x, y 坐标
        vector<int> xs, ys;
        for (auto& r : rectangles) {
            xs.push_back(r[0]);
            xs.push_back(r[2]);
            ys.push_back(r[1]);
            ys.push_back(r[3]);
        }
        sort(xs.begin(), xs.end());
        xs.erase(unique(xs.begin(), xs.end()), xs.end());
        sort(ys.begin(), ys.end());
        ys.erase(unique(ys.begin(), ys.end()), ys.end());

        long long ans = 0;

        // 遍历每个离散化后的小格子 [xs[i], xs[i+1]) x [ys[j], ys[j+1])
        for (int i = 0; i + 1 < (int)xs.size(); i++) {
            for (int j = 0; j + 1 < (int)ys.size(); j++) {
                int x1 = xs[i], x2 = xs[i + 1];
                int y1 = ys[j], y2 = ys[j + 1];

                // 检查这个格子是否被任意矩形完全覆盖
                for (auto& r : rectangles) {
                    if (r[0] <= x1 && x2 <= r[2] && r[1] <= y1 && y2 <= r[3]) {
                        // 被覆盖，计入面积（用 long long 防溢出）
                        ans += (long long)(x2 - x1) * (y2 - y1);
                        ans %= MOD;
                        break;  // 只算一次，不重复计算
                    }
                }
            }
        }

        return (int)ans;
    }
};

// ============================================================
// 解法2: 扫描线 + 区间合并 ⭐ 面试推荐
// 时间: O(n^2 log n)  空间: O(n)
//
// 【思路】
// 沿 x 轴扫描。在每个离散化的 x 区间 [xs[i], xs[i+1]) 内：
//   1. 收集在该 x 区间活跃的矩形的 y 区间
//   2. 合并这些 y 区间（去重）
//   3. 合并后的总 y 覆盖长度 * (xs[i+1]-xs[i]) = 该区间的面积
//
// "活跃"的定义:
//   矩形 [x1,y1,x2,y2] 在 x 区间 [a,b) 中活跃，
//   当且仅当 x1 <= a 且 b <= x2 (即 x1 <= a < b <= x2)
//   实际实现中只需检查 r[0] <= xs[i] && xs[i] < r[2]
//
// 区间合并:
//   把 y 区间排序后，依次合并重叠区间。
//   维护当前合并区间 [curL, curR):
//   - 新区间 [l, r) 与 [curL, curR) 重叠 (l <= curR) → 扩展 curR
//   - 不重叠 (l > curR) → 结算旧区间，开始新区间
//
// 示例: rectangles = [[0,0,2,2],[1,0,2,3],[1,0,3,1]]
//   xs = [0, 1, 2, 3]
//   x区间[0,1): 活跃rect0 → y区间[(0,2)]
//     合并后 y 覆盖=2, 面积=1*2=2
//   x区间[1,2): 活跃rect0,1,2 → y区间[(0,2),(0,3),(0,1)]
//     排序: [(0,1),(0,2),(0,3)]
//     合并: [0,3), 覆盖=3, 面积=1*3=3
//   x区间[2,3): 活跃rect2 → y区间[(0,1)]
//     合并后 y 覆盖=1, 面积=1*1=1
//   总面积 = 2+3+1 = 6 ✓
// ============================================================
class Solution2 {
public:
    int rectangleArea(vector<vector<int>>& rectangles) {
        const int MOD = 1e9 + 7;

        // 收集所有 x 坐标并去重
        vector<int> xs;
        for (auto& r : rectangles) {
            xs.push_back(r[0]);
            xs.push_back(r[2]);
        }
        sort(xs.begin(), xs.end());
        xs.erase(unique(xs.begin(), xs.end()), xs.end());

        long long ans = 0;

        // 扫描每个 x 区间
        for (int i = 0; i + 1 < (int)xs.size(); i++) {
            int xLeft = xs[i];
            int xWidth = xs[i + 1] - xs[i];

            // 收集在此 x 区间内活跃的 y 区间
            vector<pair<int, int>> yIntervals;
            for (auto& r : rectangles) {
                // 矩形在 [xLeft, xLeft + xWidth) 内活跃
                if (r[0] <= xLeft && xLeft + xWidth <= r[2]) {
                    yIntervals.push_back({r[1], r[3]});
                }
            }

            // 合并 y 区间，计算总覆盖长度
            sort(yIntervals.begin(), yIntervals.end());
            long long yLen = 0;
            int curL = -1, curR = -1;
            for (auto& [l, r] : yIntervals) {
                if (l >= curR) {
                    // 不重叠：结算旧区间，开始新区间
                    if (curR > curL) yLen += curR - curL;
                    curL = l;
                    curR = r;
                } else {
                    // 重叠：扩展当前区间
                    curR = max(curR, r);
                }
            }
            // 结算最后一个区间
            if (curR > curL) yLen += curR - curL;

            // 面积 = x宽度 * y覆盖长度
            ans = (ans + (long long)(xWidth % MOD) * (yLen % MOD)) % MOD;
        }

        return (int)ans;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法         | 时间         | 空间    | 适用场景          |
// |-------------|-------------|--------|-----------------|
// | 坐标压缩+逐格 | O(n^3)      | O(n^2) | n<=200, 最直观   |
// | 扫描线+合并   | O(n^2 logn) | O(n)   | 面试推荐          |
// | 线段树+扫描线  | O(n logn)   | O(n)   | n很大时(竞赛)     |
//
// ============================================================
// 【易错点】
//
// 1. long long 溢出:
//    ✗ int area = (x2-x1)*(y2-y1);  → 10^9 * 10^9 溢出
//    ✓ long long area = (long long)(x2-x1) * (y2-y1);
//
// 2. 取模时机:
//    ✗ 全部累加完再取模 → 中间 long long 也可能溢出
//    ✓ 每步加法后取模
//
// 3. 区间合并初始值:
//    ✗ curR = 0 → 第一个区间 [0,x) 会和初始值合并
//    ✓ curL = curR = -1 并在结算时检查 curR > curL
//
// ============================================================
// 【相关题型】
//
// - 218. The Skyline Problem:
//   扫描线经典，沿 x 扫描建筑轮廓线。
//
// - 391. Perfect Rectangle:
//   判断矩形能否恰好拼成完美矩形。
//
// - 223. Rectangle Area:
//   两个矩形并集面积 = 本题 n=2 的简化版。
// ============================================================
