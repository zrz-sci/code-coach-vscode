# 223. 矩形面积 (Medium)

**Tags:** Geometry, Math

## 核心思路

计算两个矩形覆盖的总面积 = 矩形A面积 + 矩形B面积 - 重叠部分面积。关键在于正确计算重叠矩形的宽和高（使用区间交集公式），以及处理不重叠的情况。

## 思维链

1. **基本公式**：总面积 = area_A + area_B - overlap_area。这是容斥原理的直接应用。
2. **矩形面积**：area_A = (ax2 - ax1) * (ay2 - ay1)，area_B 同理。
3. **重叠区域**：两个区间 [a1, a2] 和 [b1, b2] 的交集为 [max(a1,b1), min(a2,b2)]。如果 max > min 则无交集。
4. **重叠宽度**：overlap_w = max(0, min(ax2, bx2) - max(ax1, bx1))
5. **重叠高度**：overlap_h = max(0, min(ay2, by2) - max(ay1, by1))
6. **最终方案**：overlap_area = overlap_w * overlap_h，总面积 = area_A + area_B - overlap_area。O(1) 时间 O(1) 空间。

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| 容斥原理 + 区间交集 ⭐ | O(1) | O(1) | 唯一正解 |

## 关键提示

### 区间交集图解

```
X轴方向:
A:  ax1=====ax2
B:      bx1=======bx2
交集:   max(ax1,bx1)===min(ax2,bx2)
        overlap_w = min(ax2,bx2) - max(ax1,bx1)

不重叠的情况:
A: ax1===ax2
B:              bx1===bx2
   min(ax2,bx2) < max(ax1,bx1) → overlap_w = 0
```

### 二维重叠图解

```
      ay2 +--------+
          |   A    |
      by2 |   +----+-------+
          |   |////|       |
      ay1 +---+----+       |
              |      B     |
      by1     +------------+
         ax1  bx1  ax2    bx2

重叠区域（斜线部分）：
  宽 = min(ax2,bx2) - max(ax1,bx1)
  高 = min(ay2,by2) - max(ay1,by1)
```

### 完全包含的情况

```
      +------------------+
      |   A              |
      |    +--------+    |
      |    |   B    |    |
      |    +--------+    |
      |                  |
      +------------------+

overlap = B 的面积
总面积 = A 的面积（B 被完全包含）
公式仍然正确：area_A + area_B - area_B = area_A
```

## 解法详解

### 解法1: 容斥原理 + 区间交集 (唯一正解)

**思路**：分别算两个矩形面积，计算重叠区域面积，用容斥原理得到总面积。

```cpp
class Solution {
public:
    int computeArea(int ax1, int ay1, int ax2, int ay2,
                    int bx1, int by1, int bx2, int by2) {
        // 两个矩形的面积
        int area_A = (ax2 - ax1) * (ay2 - ay1);
        int area_B = (bx2 - bx1) * (by2 - by1);
        
        // 计算重叠区域的宽和高
        int overlap_w = max(0, min(ax2, bx2) - max(ax1, bx1));
        int overlap_h = max(0, min(ay2, by2) - max(ay1, by1));
        
        // 容斥原理
        return area_A + area_B - overlap_w * overlap_h;
    }
};
```

### 解法2: 分情况讨论（不推荐但面试可能被追问）

**思路**：先判断两矩形是否有重叠，无重叠直接返回面积之和，有重叠再计算。

```cpp
class Solution {
public:
    int computeArea(int ax1, int ay1, int ax2, int ay2,
                    int bx1, int by1, int bx2, int by2) {
        int area_A = (ax2 - ax1) * (ay2 - ay1);
        int area_B = (bx2 - bx1) * (by2 - by1);
        
        // 判断不重叠的四种情况
        if (ax2 <= bx1 || bx2 <= ax1 || ay2 <= by1 || by2 <= ay1) {
            return area_A + area_B;
        }
        
        // 计算重叠
        int overlap_w = min(ax2, bx2) - max(ax1, bx1);
        int overlap_h = min(ay2, by2) - max(ay1, by1);
        
        return area_A + area_B - overlap_w * overlap_h;
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ `overlap = (ax2-bx1) * (ay2-by1)` | ✓ `overlap = max(0, min(ax2,bx2)-max(ax1,bx1)) * max(0, min(ay2,by2)-max(ay1,by1))` | 必须用区间交集公式 |
| ✗ 忘记 `max(0, ...)` | ✓ `max(0, overlap_w)` | 不重叠时交集为负数，需要截断为 0 |
| ✗ 用 long long 计算面积 | ✓ int 足够 | 坐标范围 [-10^4, 10^4]，面积最大 4*10^8，不溢出 int |
| ✗ 混淆左下角和右上角坐标 | ✓ (ax1,ay1) 是左下，(ax2,ay2) 是右上 | 题目保证 ax1 <= ax2, ay1 <= ay2 |

## 面试追问

**Q1: 如果有 N 个矩形，求覆盖的总面积？**
→ 用坐标压缩 + 扫描线算法。将 x 坐标离散化，按 x 坐标扫描，对每段 x 区间用区间合并计算 y 方向的覆盖长度。时间 O(N^2) 或用线段树优化到 O(N log N)。参见 LeetCode 850。

**Q2: 如果矩形可以旋转（非轴对齐），如何计算重叠面积？**
→ 用多边形交集算法（Sutherland-Hodgman 裁剪算法），计算两个凸多边形的交集，再用叉积求面积。

**Q3: 如何判断两个矩形是否有重叠？**
→ 四种不重叠情况取反：`!(ax2 <= bx1 || bx2 <= ax1 || ay2 <= by1 || by2 <= ay1)`

## 相关题型

- [836. Rectangle Overlap](https://leetcode.com/problems/rectangle-overlap/) - 判断是否重叠
- [850. Rectangle Area II](https://leetcode.com/problems/rectangle-area-ii/) - N 个矩形的并面积（扫描线）
- [391. Perfect Rectangle](https://leetcode.com/problems/perfect-rectangle/) - 完美矩形判定
- [1401. Circle and Rectangle Overlapping](https://leetcode.com/problems/circle-and-rectangle-overlapping/) - 圆与矩形重叠
