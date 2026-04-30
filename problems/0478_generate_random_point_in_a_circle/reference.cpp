// LeetCode 478: Generate Random Point in a Circle
// 核心：均匀随机生成圆内点
//
// 关键数学：
// - 面积微元 dA = r * dr * dtheta
// - CDF: F(r) = r^2 / R^2
// - 逆变换: r = R * sqrt(U), U ~ Uniform(0, 1)
//
// 两种方法：极坐标变换法 和 拒绝采样法

// ==================== 解法1: 极坐标变换法（推荐）====================
// 角度均匀随机 [0, 2pi)
// 半径 r = R * sqrt(U) 保证面积均匀
// 时间 O(1)，空间 O(1)
class Solution {
    double rad, xc, yc;
    mt19937 gen;
    uniform_real_distribution<double> dist;

public:
    Solution(double radius, double x_center, double y_center)
        : rad(radius), xc(x_center), yc(y_center),
          gen(random_device{}()), dist(0.0, 1.0) {}

    vector<double> randPoint() {
        // 角度：在 [0, 2*pi) 上均匀分布
        double theta = 2.0 * M_PI * dist(gen);

        // 半径：使用 sqrt 保证面积均匀分布
        // 直觉：外圈面积大，需要更多点分布在外圈
        // 数学：r 的 PDF f(r) = 2r/R^2, CDF F(r) = r^2/R^2
        //       逆变换 r = R * sqrt(U)
        double r = rad * sqrt(dist(gen));

        // 极坐标转直角坐标，加上圆心偏移
        return {xc + r * cos(theta), yc + r * sin(theta)};
    }
};

// ==================== 解法2: 拒绝采样法 ====================
// 在外接正方形内均匀随机生成，判断是否在圆内
// 接受率 = pi*R^2 / (2R)^2 = pi/4 ~ 78.5%
// 期望采样次数 = 4/pi ~ 1.27
// 时间 O(1) 期望，空间 O(1)
class Solution_Reject {
    double rad, xc, yc;
    mt19937 gen;
    uniform_real_distribution<double> dist;

public:
    Solution_Reject(double radius, double x_center, double y_center)
        : rad(radius), xc(x_center), yc(y_center),
          gen(random_device{}()), dist(-1.0, 1.0) {}

    vector<double> randPoint() {
        while (true) {
            // 在 [-R, R] x [-R, R] 正方形内均匀采样
            double x = dist(gen) * rad;
            double y = dist(gen) * rad;

            // 检查点是否在圆内（含圆周）
            if (x * x + y * y <= rad * rad) {
                // 加上圆心偏移
                return {xc + x, yc + y};
            }
            // 不在圆内则重新采样
        }
    }
};

// ==================== 错误示范：半径均匀随机（不均匀！） ====================
// 以下方法是错误的，会导致点集中在圆心附近
// 原因：半径小的环带面积小，半径大的环带面积大
//       均匀随机半径会给小环带分配过多的点
class Solution_Wrong {
    double rad, xc, yc;

public:
    Solution_Wrong(double radius, double x_center, double y_center)
        : rad(radius), xc(x_center), yc(y_center) {}

    vector<double> randPoint() {
        double theta = 2.0 * M_PI * ((double)rand() / RAND_MAX);
        // 错误！不应该直接 rad * rand()
        double r = rad * ((double)rand() / RAND_MAX);  // BUG: 缺少 sqrt
        return {xc + r * cos(theta), yc + r * sin(theta)};
    }
};

// ==================== 复杂度分析 ====================
// 极坐标法: 时间 O(1)，空间 O(1) — 每次调用确定性完成
// 拒绝采样: 时间 O(1) 期望，最坏无穷（但概率极低），空间 O(1)
//
// 面试关注点：
// 1. 为什么需要 sqrt？ — 面积与 r^2 成正比
// 2. 拒绝采样效率 — 4/pi ~ 1.27 次期望
// 3. 随机数质量 — 推荐 mt19937 + uniform_real_distribution
