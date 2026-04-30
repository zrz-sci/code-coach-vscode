# 149. 直线上最多的点数

## 核心思路
本质是枚举每个点作为基准点，统计与其共线的最多点数，关键在于如何正确表示斜率避免浮点误差。

## 思维链
1. **暴力**: 枚举所有点对确定直线，再数每条线上有多少点 → O(n^3)
2. **优化**: 固定一个点，只需统计经过它的所有直线中哪条点最多 → O(n^2)
3. **斜率分组**: 经过同一点且斜率相同的点必定共线 → 用哈希表按斜率分组
4. **浮点陷阱**: 用 double 做斜率 key 会有精度问题 → 用约分后的 (dx, dy) 整数对
5. **约分方法**: dx 和 dy 同除以 gcd，并规范化符号(保证 dx >= 0)
6. **特殊处理**: 垂直线(dx=0)和水平线(dy=0)无需特殊处理，约分后自然区分

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 哈希表 + GCD约分斜率 | 枚举基准点 + 斜率分组 | O(n^2) | O(n) | ⭐ 必知 |
| 暴力三重循环 | 枚举两点定线 + 检查第三点 | O(n^3) | O(1) | 了解即可 |

## 关键提示
1. **斜率表示**: 不能用 `double slope = dy/dx`，要用约分后的 `(dx/g, dy/g)` 作为key
2. **符号规范化**: 约分后强制 dx >= 0；若 dx == 0 则令 dy = 1。保证同一斜率映射到同一key
3. **重复点**: 题目保证所有点互不相同(unique)，无需处理重复
4. **答案 = 最大共线数 + 1**: 哈希表存的是"与基准点共线的其他点数"，加上基准点本身
5. **n <= 300**: O(n^2) 完全够用，不需要更复杂的算法

```
示例: points = [[1,1],[3,2],[5,3],[4,1],[2,3],[1,4]]

以 (1,1) 为基准:
  → (3,2): dx=2, dy=1, gcd=1 → key=(2,1), count=1
  → (5,3): dx=4, dy=2, gcd=2 → key=(2,1), count=2  ← 同一斜率!
  → (4,1): dx=3, dy=0, gcd=3 → key=(1,0), count=1
  → (2,3): dx=1, dy=2, gcd=1 → key=(1,2), count=1
  → (1,4): dx=0, dy=3, gcd=3 → key=(0,1), count=1

最大共线 = 2，加上基准点 = 3
继续枚举其他基准点...最终答案 = 4
```

## 解法详解

### 解法1: 哈希表 + GCD约分斜率 (最优解)

**思路**: 对每个点i，计算它与其他所有点j的斜率，用约分后的整数对表示斜率，哈希表计数。

```cpp
class Solution {
public:
    int maxPoints(vector<vector<int>>& points) {
        int n = points.size();
        if (n <= 2) return n;
        
        int ans = 2; // 至少2个点共线
        for (int i = 0; i < n; i++) {
            map<pair<int,int>, int> slopeCount;
            for (int j = i + 1; j < n; j++) {
                int dx = points[j][0] - points[i][0];
                int dy = points[j][1] - points[i][1];
                
                // 用GCD约分
                int g = __gcd(abs(dx), abs(dy));
                dx /= g;
                dy /= g;
                
                // 规范化符号: 保证 dx >= 0
                // 若 dx == 0，保证 dy > 0
                if (dx < 0) { dx = -dx; dy = -dy; }
                if (dx == 0) dy = abs(dy); // 垂直线统一为 (0, 1)
                
                slopeCount[{dx, dy}]++;
            }
            for (auto& [slope, cnt] : slopeCount) {
                ans = max(ans, cnt + 1); // +1 是基准点自身
            }
        }
        return ans;
    }
};
```

**关键点**:
- gcd 约分保证相同斜率映射到同一 key
- 符号规范化: dx < 0 时翻转，dx == 0 时 dy 取正
- j 从 i+1 开始避免重复计算

### 解法2: 暴力验证 (面试了解)

**思路**: 枚举每对点(i,j)确定一条直线，再遍历所有其他点检查是否共线。用叉积判断共线。

```cpp
class Solution {
public:
    int maxPoints(vector<vector<int>>& points) {
        int n = points.size();
        if (n <= 2) return n;
        
        int ans = 2;
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                int count = 2;
                for (int k = j + 1; k < n; k++) {
                    // 叉积判断共线: (j-i) x (k-i) == 0
                    int dx1 = points[j][0] - points[i][0];
                    int dy1 = points[j][1] - points[i][1];
                    int dx2 = points[k][0] - points[i][0];
                    int dy2 = points[k][1] - points[i][1];
                    if ((long long)dx1 * dy2 == (long long)dy1 * dx2) {
                        count++;
                    }
                }
                ans = max(ans, count);
            }
        }
        return ans;
    }
};
```

**关键点**:
- 叉积 dx1*dy2 == dy1*dx2 避免除法和浮点数
- 需要 long long 防溢出 (坐标范围 10^4, 乘积可达 10^8 安全)

## 易错点

1. **用 double 做 key**
```
✗ double slope = (double)(dy) / dx;  // 浮点精度问题
  map[slope]++;
✓ int g = __gcd(abs(dx), abs(dy));    // GCD约分整数对
  map[{dx/g, dy/g}]++;
```

2. **符号未规范化导致同一斜率不同key**
```
✗ slopeCount[{dx/g, dy/g}]++;         // (-1,2) 和 (1,-2) 是同一斜率但不同key
✓ if (dx < 0) { dx = -dx; dy = -dy; } // 统一 dx >= 0
```

3. **忘记 +1 计入基准点**
```
✗ ans = max(ans, slopeCount[slope]);       // 少算了基准点
✓ ans = max(ans, slopeCount[slope] + 1);   // 共线点数 + 基准点
```

## 面试追问

**Q1**: 为什么不能用浮点数做斜率的 key？
→ 浮点数有精度误差，比如 1/3 和 2/6 可能产生不同的 double 值。用 GCD 约分后的整数对可以精确表示斜率。

**Q2**: 如果点可以重复(即允许多个相同坐标的点)，怎么处理？
→ 先统计与基准点重合的点数 `same`，这些点与任何直线共线。最终答案 = max(slopeCount) + same + 1。

**Q3**: 如果要求 O(n^2) 时间但不用哈希表（比如哈希冲突严重），有没有替代方案？
→ 可以用 `map<pair<int,int>, int>` (红黑树) 替代哈希表，时间变为 O(n^2 log n) 但无冲突。或者对每个基准点，将所有斜率排序后线性扫描计数。

## 相关题型
- [356. 直线镜像](../0356_line_reflection/) — 几何+哈希表，判断点集关于某直线对称
- [lines-through-points] — 几何计算扩展
- [面试常见] GCD 约分技巧在分数表示中复用
