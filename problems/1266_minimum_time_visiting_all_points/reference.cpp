/*
 * LeetCode 1266: 访问所有点的最小时间 (Minimum Time Visiting All Points)
 *
 * 【题目本质】
 * 按顺序访问平面上的点，每秒可走水平/垂直/对角线一步。
 * 两点间最短时间 = 切比雪夫距离 = max(|dx|, |dy|)，累加所有相邻点对即可。
 *
 * 【解法总览】
 * 解法1: 切比雪夫距离（直观推导版）— O(n) / O(1)
 * 解法2: 切比雪夫距离（简洁版）      — O(n) / O(1)
 *
 * 两种写法本质相同，展示不同的理解角度。
 */

// ============================================================
// 解法1: 切比雪夫距离（直观推导版）
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 从 A(x1,y1) 到 B(x2,y2)：
//   dx = |x2 - x1|, dy = |y2 - y1|
//
// 对角线一步同时消耗水平和垂直各 1 个单位，所以：
//   - 先对角走 min(dx, dy) 步，把较短方向走完
//   - 再直走 |dx - dy| 步，走完剩余
//   - 总步数 = min(dx, dy) + |dx - dy| = max(dx, dy)
//
// 示意图（从 (1,1) 到 (3,4)）:
//
//   y=4  . . . B         dx = |3-1| = 2
//   y=3  . . ↗ ↑         dy = |4-1| = 3
//   y=2  . ↗ . .         对角走 min(2,3)=2 步: (1,1)→(2,2)→(3,3)
//   y=1  A . . .         直走 |2-3|=1 步: (3,3)→(3,4)
//        1 2 3 4         总计 max(2,3) = 3 步 ✓
//
// 从 (3,4) 到 (-1,0):
//   dx = |(-1)-3| = 4, dy = |0-4| = 4
//   全部对角走: max(4,4) = 4 步 ✓
//
// 总时间 = 3 + 4 = 7 ✓
// ============================================================
class Solution1 {
public:
    int minTimeToVisitAllPoints(vector<vector<int>>& points) {
        int totalTime = 0;
        for (int i = 1; i < (int)points.size(); i++) {
            int dx = abs(points[i][0] - points[i - 1][0]);
            int dy = abs(points[i][1] - points[i - 1][1]);
            
            // 对角走消耗 min(dx,dy) 步，直走消耗剩余差值
            // min(dx,dy) + abs(dx - dy) 化简就是 max(dx,dy)
            int diagonal = min(dx, dy);       // 对角线步数
            int straight = abs(dx - dy);      // 剩余直走步数
            totalTime += diagonal + straight;  // 等价于 max(dx, dy)
        }
        return totalTime;
    }
};

// ============================================================
// 解法2: 切比雪夫距离（简洁版）
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 知道公式后直接用 max(|dx|, |dy|)，代码更简洁。
// 和解法1完全等价，只是省去了中间变量。
// ============================================================
class Solution2 {
public:
    int minTimeToVisitAllPoints(vector<vector<int>>& points) {
        int totalTime = 0;
        for (int i = 1; i < (int)points.size(); i++) {
            // 切比雪夫距离：两个维度差值的较大者
            int dx = abs(points[i][0] - points[i - 1][0]);
            int dy = abs(points[i][1] - points[i - 1][1]);
            totalTime += max(dx, dy);
        }
        return totalTime;
    }
};

// ============================================================
// 【解法对比】
// 
// 解法1 和 解法2 本质完全相同：
//   min(dx,dy) + |dx-dy| ≡ max(dx,dy)
// 
// 解法1 更适合面试时讲解思路（展示"为什么是 max"），
// 解法2 更适合实际编码（一行搞定）。
//
// 【易错点】
// 1. 忘记取绝对值：
//    ✗ int dx = points[i][0] - points[i-1][0];  // 可能为负
//      → max(-4, -4) = -4，答案错误
//    ✓ int dx = abs(points[i][0] - points[i-1][0]);
//
// 2. 误用曼哈顿距离（dx + dy）：
//    ✗ totalTime += dx + dy;   // 没有利用对角线，答案偏大
//    ✓ totalTime += max(dx, dy);
//
// 3. 循环起始索引：
//    ✗ for (int i = 0; ...)  → points[i-1] 越界
//    ✓ for (int i = 1; ...)  → 正确地比较相邻点对
//
// 【面试追问】
// Q1: 为什么最短时间是 max(|dx|, |dy|) 而不是 |dx| + |dy|？
//     → 对角线一步同时消耗两个方向的距离，所以不需要分别走。
//       先对角走 min(dx,dy) 步，再直走 |dx-dy| 步 = max(dx,dy)。
//
// Q2: 如果不要求按顺序访问，求最短总时间？
//     → 变成旅行商问题(TSP)，NP-hard。
//       小 n 可用状压DP: dp[mask][i]，时间 O(2^n * n^2)。
//
// Q3: 切比雪夫距离和曼哈顿距离有什么数学关系？
//     → 坐标变换 (x,y) → (x+y, x-y) 可以互转。
//       切比雪夫距离在新坐标下等于曼哈顿距离，反之亦然。
//       这个技巧在 LeetCode 1131 等题中有应用。
// ============================================================
