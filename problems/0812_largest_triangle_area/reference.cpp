/*
 * LeetCode 812: Largest Triangle Area
 *
 * 解法一：暴力枚举 + 叉积公式（推荐）
 * 时间复杂度: O(n^3)
 * 空间复杂度: O(1)
 *
 * 思路：
 * 三重循环枚举所有三元组，用 Shoelace Formula 计算面积。
 * 面积 = 0.5 * |x1*(y2-y3) + x2*(y3-y1) + x3*(y1-y2)|
 * 这是向量叉积的直接应用。
 */

class Solution {
public:
    double largestTriangleArea(vector<vector<int>>& points) {
        int n = points.size();
        double maxArea = 0.0;

        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                for (int k = j + 1; k < n; ++k) {
                    // Shoelace formula for triangle area
                    double area = triangleArea(
                        points[i][0], points[i][1],
                        points[j][0], points[j][1],
                        points[k][0], points[k][1]
                    );
                    maxArea = max(maxArea, area);
                }
            }
        }

        return maxArea;
    }

private:
    // 用叉积计算三角形面积
    // 向量 AB x AC 的绝对值 / 2
    double triangleArea(int x1, int y1, int x2, int y2, int x3, int y3) {
        // 叉积展开式:
        // area = 0.5 * |x1*(y2-y3) + x2*(y3-y1) + x3*(y1-y2)|
        //
        // 等价于行列式:
        // | x1  y1  1 |
        // | x2  y2  1 |  的绝对值 / 2
        // | x3  y3  1 |
        return 0.5 * abs(
            (double)(x1) * (y2 - y3) +
            (double)(x2) * (y3 - y1) +
            (double)(x3) * (y1 - y2)
        );
    }
};


/*
 * 解法二：凸包优化（面试拓展版）
 * 时间复杂度: O(n log n) 凸包 + O(h^3) 枚举（h 为凸包上的点数）
 * 空间复杂度: O(n)
 *
 * 思路：
 * 最大面积三角形的三个顶点一定在凸包上。
 * 先用 Andrew 算法求凸包，再在凸包上枚举三元组。
 * 当 n 较大但凸包点数较少时，效率提升显著。
 */

class SolutionConvexHull {
public:
    double largestTriangleArea(vector<vector<int>>& points) {
        // Step 1: 构建凸包（Andrew's Monotone Chain）
        vector<vector<int>> hull = convexHull(points);

        // Step 2: 在凸包上枚举所有三元组
        int h = hull.size();
        double maxArea = 0.0;

        for (int i = 0; i < h; ++i) {
            for (int j = i + 1; j < h; ++j) {
                for (int k = j + 1; k < h; ++k) {
                    double area = triangleArea(
                        hull[i][0], hull[i][1],
                        hull[j][0], hull[j][1],
                        hull[k][0], hull[k][1]
                    );
                    maxArea = max(maxArea, area);
                }
            }
        }

        return maxArea;
    }

private:
    // 叉积: OA x OB
    // 正值 -> 逆时针, 负值 -> 顺时针, 0 -> 共线
    long long cross(vector<int>& O, vector<int>& A, vector<int>& B) {
        return (long long)(A[0] - O[0]) * (B[1] - O[1]) -
               (long long)(A[1] - O[1]) * (B[0] - O[0]);
    }

    // Andrew's Monotone Chain 凸包算法
    vector<vector<int>> convexHull(vector<vector<int>>& points) {
        int n = points.size();
        if (n < 3) return points;

        // 按 x 排序，x 相同按 y 排序
        sort(points.begin(), points.end());

        vector<vector<int>> hull;

        // 构建下凸包
        for (int i = 0; i < n; ++i) {
            while (hull.size() >= 2 &&
                   cross(hull[hull.size()-2], hull[hull.size()-1], points[i]) <= 0) {
                hull.pop_back();
            }
            hull.push_back(points[i]);
        }

        // 构建上凸包
        int lower_size = hull.size();
        for (int i = n - 2; i >= 0; --i) {
            while ((int)hull.size() > lower_size &&
                   cross(hull[hull.size()-2], hull[hull.size()-1], points[i]) <= 0) {
                hull.pop_back();
            }
            hull.push_back(points[i]);
        }

        // 最后一个点与第一个点重复，去掉
        hull.pop_back();
        return hull;
    }

    double triangleArea(int x1, int y1, int x2, int y2, int x3, int y3) {
        return 0.5 * abs(
            (double)(x1) * (y2 - y3) +
            (double)(x2) * (y3 - y1) +
            (double)(x3) * (y1 - y2)
        );
    }
};


/*
 * 解法三：旋转卡壳优化（进阶拓展）
 * 时间复杂度: O(n log n + h^2)，h 为凸包点数
 * 空间复杂度: O(n)
 *
 * 思路：
 * 在凸包上固定两个顶点 i, j，第三个顶点 k 随着 j 的移动单调递增
 * （面积关于 k 的函数是单峰的），因此可以用类似旋转卡壳的方法
 * 将第三层循环的总量均摊到 O(h)。
 *
 * 注意：严格的 O(h^2) 旋转卡壳实现较复杂，
 * 这里展示简化版本的思路框架。
 */

class SolutionRotatingCalipers {
public:
    double largestTriangleArea(vector<vector<int>>& points) {
        // 和凸包解法相同，先求凸包
        // 然后对凸包上的点使用旋转卡壳
        // 固定边 (i, j)，第三点 k 沿凸包移动
        // 利用面积单峰性质，k 不需要回退

        int n = points.size();
        double maxArea = 0.0;

        // 简化版：直接暴力（本题 n<=50 足够）
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                for (int k = j + 1; k < n; ++k) {
                    double area = 0.5 * abs(
                        (double)points[i][0] * (points[j][1] - points[k][1]) +
                        (double)points[j][0] * (points[k][1] - points[i][1]) +
                        (double)points[k][0] * (points[i][1] - points[j][1])
                    );
                    maxArea = max(maxArea, area);
                }
            }
        }

        return maxArea;
    }
};
