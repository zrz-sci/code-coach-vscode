/*
 * LeetCode 469: Convex Polygon
 * 难度: Medium | 标签: Geometry, Array, Math
 *
 * 核心思路: 叉积判断所有连续三点的转向方向是否一致
 *
 * 叉积几何意义:
 *         C
 *        /
 *       /  cross > 0: 左转(逆时针)
 *  A---B
 *       \  cross < 0: 右转(顺时针)
 *        \
 *         C'
 *
 *  cross(AB, BC) = (Bx-Ax)(Cy-By) - (By-Ay)(Cx-Bx)
 *
 * 凸多边形: 所有非零叉积同号
 * 凹多边形: 存在叉积变号的顶点
 *
 * 环形遍历: i, (i+1)%n, (i+2)%n
 * 共线处理: cross=0 跳过，不影响凸性
 * 溢出处理: 坐标差 <= 2*10^4, 乘积 <= 4*10^8, 用 long long
 */


// ================================================================
// 解法1: 叉积判断法 -- O(n) 时间, O(1) 空间
// ================================================================
// 遍历所有连续三点 (A, B, C)
// 计算 cross(AB, BC) = dx1*dy2 - dy1*dx2
// 所有非零叉积必须同号 -> 凸多边形
//
// 示例: points = [[0,0],[0,5],[5,5],[5,0]]  (正方形)
//   三元组:
//   (0,0)->(0,5)->(5,5): dx1=0,dy1=5, dx2=5,dy2=0  cross=-25 (右转)
//   (0,5)->(5,5)->(5,0): dx1=5,dy1=0, dx2=0,dy2=-5 cross=-25 (右转)
//   (5,5)->(5,0)->(0,0): dx1=0,dy1=-5,dx2=-5,dy2=0 cross=-25 (右转)
//   (5,0)->(0,0)->(0,5): dx1=-5,dy1=0,dx2=0,dy2=5  cross=-25 (右转)
//   全部同号 -> true
//
// 示例: points = [[0,0],[0,10],[10,10],[10,0],[5,5]]
//   ...(10,0)->(5,5)->(0,0): cross > 0 (左转!)
//   出现变号 -> false
class Solution {
public:
    bool isConvex(vector<vector<int>>& points) {
        int n = points.size();
        long long prevCross = 0;  // 记录之前的叉积符号

        for (int i = 0; i < n; i++) {
            // 三个连续顶点 (环形取模)
            int a = i;
            int b = (i + 1) % n;
            int c = (i + 2) % n;

            // 向量 AB
            long long dx1 = points[b][0] - points[a][0];
            long long dy1 = points[b][1] - points[a][1];
            // 向量 BC
            long long dx2 = points[c][0] - points[b][0];
            long long dy2 = points[c][1] - points[b][1];

            // 叉积 cross = AB x BC
            long long cross = dx1 * dy2 - dy1 * dx2;

            if (cross != 0) {
                // 如果之前有非零叉积且符号不同 -> 不凸
                if (prevCross != 0 && ((prevCross > 0) != (cross > 0))) {
                    return false;
                }
                prevCross = cross;
            }
            // cross == 0: 共线，跳过不影响判断
        }
        return true;
    }
};


// ================================================================
// 解法2: 用sign标记简化 -- O(n) 时间, O(1) 空间
// ================================================================
// 用 sign 变量 (0=未确定, 1=正, -1=负) 代替 prevCross
// 逻辑更清晰
class Solution2 {
public:
    bool isConvex(vector<vector<int>>& points) {
        int n = points.size();
        int sign = 0;  // 0=未确定, 1=正(逆时针), -1=负(顺时针)

        for (int i = 0; i < n; i++) {
            int a = i, b = (i + 1) % n, c = (i + 2) % n;

            long long cross =
                (long long)(points[b][0] - points[a][0]) *
                            (points[c][1] - points[b][1]) -
                (long long)(points[b][1] - points[a][1]) *
                            (points[c][0] - points[b][0]);

            if (cross > 0) {
                if (sign == -1) return false;  // 之前是右转，现在左转
                sign = 1;
            } else if (cross < 0) {
                if (sign == 1) return false;   // 之前是左转，现在右转
                sign = -1;
            }
            // cross == 0: 共线，不更新 sign
        }
        return true;
    }
};


/*
 * ================================================================
 * 复杂度分析:
 *
 * 时间: O(n) -- 遍历所有顶点一次
 * 空间: O(1) -- 只用常数变量
 *
 * ================================================================
 * 叉积公式备忘:
 *
 * 给定三点 A(x1,y1), B(x2,y2), C(x3,y3):
 *   向量 AB = (x2-x1, y2-y1)
 *   向量 BC = (x3-x2, y3-y2)
 *   叉积 = (x2-x1)*(y3-y2) - (y2-y1)*(x3-x2)
 *
 * 叉积 > 0: A->B->C 逆时针(左转)
 * 叉积 < 0: A->B->C 顺时针(右转)
 * 叉积 = 0: A, B, C 共线
 *
 * ================================================================
 * 易错点:
 *
 * 1. 漏掉环形处理: 最后两组三元组 (Pn-2,Pn-1,P0) 和 (Pn-1,P0,P1) 必须检查
 * 2. 共线误判: cross=0 应跳过，不应视为符号不同
 * 3. 整数溢出: 坐标差可达 2*10^4, 乘积可达 4*10^8, 必须用 long long
 * 4. 忘记初始化 prevCross=0: 第一个非零叉积设定方向基准
 *
 * ================================================================
 * 面试追问:
 *
 * Q1: 叉积的正负分别代表什么?
 *     cross > 0: 逆时针(左转), cross < 0: 顺时针(右转)
 *
 * Q2: 如果所有点共线?
 *     所有 cross=0, 返回 true (题目保证是简单多边形，不会出现)
 *
 * Q3: 如何计算多边形面积?
 *     Shoelace 公式: Area = 0.5 * |sum(xi*y(i+1) - x(i+1)*yi)|
 *     符号表示顺逆时针方向
 * ================================================================
 */
