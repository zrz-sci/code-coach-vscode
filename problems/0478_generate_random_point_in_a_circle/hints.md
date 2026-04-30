# 478. 在圆内随机生成点

## 核心思路
使用极坐标变换法，角度均匀随机、半径取 R*sqrt(rand) 以保证面积均匀分布，或使用拒绝采样在外接正方形内随机生成并过滤。

## 思维链
1. **理解均匀性**: "均匀"指圆内任意等面积区域被选中的概率相同，不能简单地对半径均匀随机
2. **错误方法排除**: 直接 r = R * rand() 会导致点集中在圆心附近，因为靠近圆心的环带面积小
3. **极坐标法推导**: 面积微元 dA = r*dr*d_theta，CDF F(r) = r^2/R^2，反函数法得 r = R*sqrt(U)
4. **拒绝采样法**: 在 [-R, R] x [-R, R] 正方形内均匀采样，若 x^2+y^2 <= R^2 则接受
5. **效率对比**: 极坐标法每次 O(1) 确定性；拒绝采样期望 4/pi ~ 1.27 次采样

## 解法概览

| 方法 | 时间 | 空间 | 适用场景 | 推荐 |
|------|------|------|----------|------|
| 极坐标变换 | O(1) | O(1) | 确定性方法 | ⭐ |
| 拒绝采样 | O(1) 期望 | O(1) | 概念简单 | ⭐ |

## 关键提示
1. r = R * sqrt(rand()) 是关键，不能直接 r = R * rand()，否则分布不均匀
2. 面积与 r^2 成正比，所以半径的 CDF 是 r^2/R^2，逆变换得 sqrt
3. 拒绝采样的接受率 = 圆面积/正方形面积 = pi/4 ~ 78.5%
4. 要加上圆心偏移 (x_center, y_center)
5. 使用 (double)rand()/RAND_MAX 或更好的随机数生成器

## 解法详解

### 解法1: 极坐标变换法 ⭐

**思路**: 角度 theta 在 [0, 2*pi) 均匀随机，半径 r = R * sqrt(U) 其中 U~Uniform(0,1)。这保证了面积均匀分布。

**数学推导**:
- 圆内均匀分布要求 f(r, theta) = 1/(pi*R^2)
- 边缘分布 f(r) = 2r/R^2，对应 CDF F(r) = r^2/R^2
- 逆变换法: r = R * sqrt(U)

```cpp
class Solution {
    double rad, xc, yc;
    mt19937 gen;
    uniform_real_distribution<double> dist;
public:
    Solution(double radius, double x_center, double y_center)
        : rad(radius), xc(x_center), yc(y_center),
          gen(random_device{}()), dist(0.0, 1.0) {}
    
    vector<double> randPoint() {
        double theta = 2.0 * M_PI * dist(gen);      // 角度均匀
        double r = rad * sqrt(dist(gen));             // 半径: sqrt 保证面积均匀
        return {xc + r * cos(theta), yc + r * sin(theta)};
    }
};
```

### 解法2: 拒绝采样法 ⭐

**思路**: 在圆的外接正方形 [-R, R] x [-R, R] 内均匀生成点，判断是否落在圆内。接受率约 78.5%。

```cpp
class Solution {
    double rad, xc, yc;
    mt19937 gen;
    uniform_real_distribution<double> dist;
public:
    Solution(double radius, double x_center, double y_center)
        : rad(radius), xc(x_center), yc(y_center),
          gen(random_device{}()), dist(-1.0, 1.0) {}
    
    vector<double> randPoint() {
        while (true) {
            double x = dist(gen) * rad;   // [-R, R]
            double y = dist(gen) * rad;   // [-R, R]
            if (x * x + y * y <= rad * rad) {
                return {xc + x, yc + y};
            }
        }
    }
};
```

## 易错点
- ✗ 半径直接均匀随机 r = R * rand()，导致点集中在圆心 → ✓ 使用 r = R * sqrt(rand()) 保证面积均匀
- ✗ 拒绝采样时忘记加圆心偏移，只返回 {x, y} → ✓ 返回 {xc + x, yc + y}
- ✗ 使用 rand() % N 获取随机数，精度不足 → ✓ 使用 uniform_real_distribution 获取 [0,1) 的双精度浮点数

## 面试追问
- **Q1**: 为什么 r = R * sqrt(U) 能保证均匀？ → 面积微元 dA = r*dr*dtheta，CDF F(r) = r^2/R^2，逆变换法
- **Q2**: 拒绝采样和极坐标法哪个更快？ → 拒绝采样期望 4/pi ~ 1.27 次迭代，极坐标法确定性 O(1) 但需要 cos/sin
- **Q3**: 如何推广到椭圆或任意凸多边形？ → 椭圆可先在单位圆采样再仿射变换；凸多边形用三角剖分后按面积加权采样

## 相关题型
- LC 470 用 Rand7 实现 Rand10（拒绝采样）
- LC 497 非重叠矩形中的随机点（加权随机 + 矩形采样）
- LC 528 按权重随机选择（前缀和 + 二分查找）
- LC 519 随机翻转矩阵（哈希映射随机）
