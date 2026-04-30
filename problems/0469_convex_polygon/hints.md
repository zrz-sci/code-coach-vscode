# 469. 凸多边形

## 核心思路
利用叉积判断多边形所有相邻边的转向方向是否一致：凸多边形的所有连续三点的叉积必须同号（全正或全负）。

## 思维链

1. **凸多边形定义**: 所有内角都小于180度，等价于沿边走时所有转向方向一致
2. **如何判断转向**: 相邻三点A->B->C，计算向量AB和BC的叉积，叉积符号决定转向方向
3. **叉积公式**: cross = (Bx-Ax)(Cy-By) - (By-Ay)(Cx-Bx)，正=逆时针(左转)，负=顺时针(右转)
4. **环形遍历**: 多边形首尾相连，用取模 `i, (i+1)%n, (i+2)%n` 处理
5. **共线处理**: 叉积为0表示共线，不影响凸性判断，只需检查非零叉积同号
6. **溢出处理**: 坐标范围[-10^4, 10^4]，乘积可达10^8，用long long避免溢出

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 叉积判断 | 遍历所有连续三点，检查叉积同号 | O(n) | O(1) | ⭐ |

## 关键提示

1. **叉积的几何意义**:
```
        C
       /
      /  叉积 > 0: 左转(逆时针)
A----B
      \  叉积 < 0: 右转(顺时针)
       \
        C'

cross(AB, BC) = (Bx-Ax)(Cy-By) - (By-Ay)(Cx-Bx)
```

2. **凸 vs 凹多边形**:
```
凸多边形(正方形):          凹多边形(带凹角):
  B---C                     B---C
  |   |   所有叉积同号       |   |
  A---D                     A   D
                               \ /
                                E   <-- 叉积在E处变号
```

3. **环形遍历示意**:
```
points = [P0, P1, P2, ..., Pn-1]

检查三元组:
  (P0, P1, P2), (P1, P2, P3), ..., (Pn-2, Pn-1, P0), (Pn-1, P0, P1)

用取模: i, (i+1)%n, (i+2)%n
```

4. **叉积为0(共线)的处理**: 三点共线不算转向，跳过即可，只关心有明确转向的叉积

5. **整数溢出**: 坐标差最大2*10^4，两个差值相乘最大4*10^8，在int范围内但为安全用long long

## 解法详解

### 解法1: 叉积判断法

```cpp
class Solution {
public:
    bool isConvex(vector<vector<int>>& points) {
        int n = points.size();
        long long prevCross = 0;

        for (int i = 0; i < n; i++) {
            int a = i, b = (i + 1) % n, c = (i + 2) % n;
            // 向量 AB = (Bx-Ax, By-Ay)
            long long dx1 = points[b][0] - points[a][0];
            long long dy1 = points[b][1] - points[a][1];
            // 向量 BC = (Cx-Bx, Cy-By)
            long long dx2 = points[c][0] - points[b][0];
            long long dy2 = points[c][1] - points[b][1];

            long long cross = dx1 * dy2 - dy1 * dx2;

            if (cross != 0) {
                if (prevCross != 0 && ((prevCross > 0) != (cross > 0))) {
                    return false;  // 叉积变号，不是凸多边形
                }
                prevCross = cross;
            }
        }
        return true;
    }
};
```

### 解法2: 用sign标记简化

```cpp
class Solution {
public:
    bool isConvex(vector<vector<int>>& points) {
        int n = points.size();
        int sign = 0;  // 0=未确定, 1=正, -1=负

        for (int i = 0; i < n; i++) {
            int a = i, b = (i + 1) % n, c = (i + 2) % n;
            long long cross = (long long)(points[b][0] - points[a][0])
                            * (points[c][1] - points[b][1])
                            - (long long)(points[b][1] - points[a][1])
                            * (points[c][0] - points[b][0]);

            if (cross > 0) {
                if (sign == -1) return false;
                sign = 1;
            } else if (cross < 0) {
                if (sign == 1) return false;
                sign = -1;
            }
            // cross == 0: 共线，跳过
        }
        return true;
    }
};
```

## 易错点

1. **忘记环形处理**:
```cpp
// ✗ 错误: 只检查到 n-3，漏掉首尾相连的三元组
for (int i = 0; i < n - 2; i++) {
    int a = i, b = i + 1, c = i + 2;
    ...
}
// ✓ 正确: 用取模处理环形
for (int i = 0; i < n; i++) {
    int a = i, b = (i + 1) % n, c = (i + 2) % n;
    ...
}
```

2. **没有忽略叉积为0的情况**:
```cpp
// ✗ 错误: 共线(cross=0)时也做符号比较，导致误判
if ((prevCross > 0) != (cross > 0)) return false;
// ✓ 正确: 只在cross非零时比较
if (cross != 0) {
    if (prevCross != 0 && ((prevCross > 0) != (cross > 0)))
        return false;
    prevCross = cross;
}
```

3. **整数溢出**:
```cpp
// ✗ 错误: int乘法可能溢出
int cross = x1 * y2 - y1 * x2;
// ✓ 正确: 用long long
long long cross = (long long)x1 * y2 - (long long)y1 * x2;
```

## 面试追问

**Q1: 叉积的正负分别代表什么方向？**
> 叉积 > 0表示从AB到BC是逆时针(左转)，< 0表示顺时针(右转)，= 0表示共线。凸多边形要求所有转向一致。

**Q2: 如果所有点共线怎么办？**
> 所有叉积都为0，prevCross始终为0，函数返回true。严格来说共线不构成多边形，但题目保证输入是简单多边形，不会出现此情况。

**Q3: 如何判断多边形的顶点是按顺时针还是逆时针排列的？**
> 计算所有叉积的和(或计算有符号面积)。正 = 逆时针排列，负 = 顺时针排列。

## 相关题型

| 题号 | 题目 | 关系 |
|------|------|------|
| 149 | Max Points on a Line | 叉积/斜率判断共线 |
| 587 | Erect the Fence | 凸包算法 (Graham Scan) |
| 1232 | Check If It Is a Straight Line | 叉积判断共线 |
