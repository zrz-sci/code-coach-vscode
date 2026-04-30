# 391. Perfect Rectangle - 完美矩形

## 核心思路
完美覆盖需同时满足两个条件：(1) 所有小矩形面积之和等于大矩形面积；(2) 除四个角点外，每个顶点出现偶数次（奇数次出现的点恰好是大矩形的四角）。

## 思维链
1. 面积相等是必要条件，但不充分（可能有重叠+空隙恰好抵消？不会，因为面积不能为负）
2. 面积相等也无法检测"有重叠但也有等面积空隙"的情况 -> 需要顶点约束
3. 完美覆盖 -> 内部每个顶点被偶数个矩形共享（2或4个），角点被奇数个（1个）
4. 用set做异或（出现偶数次删除，奇数次保留），最终set恰好剩4个角点

## 解法概览表

| 解法 | 时间 | 空间 | 特点 |
|------|------|------|------|
| ⭐ 面积+顶点计数 | O(n) | O(n) | 最优解，HashSet |
| 扫描线 | O(n log n) | O(n) | 通用几何方法，较复杂 |

## 关键提示
1. **面积验证**：`sum(每个小矩形面积) == 大矩形面积`
2. **大矩形**：由所有矩形的 min(x1), min(y1), max(x2), max(y2) 确定
3. **顶点奇偶**：每个小矩形贡献4个顶点，用set的insert/erase切换
4. 最终set中恰好只剩大矩形的4个角点

## 解法详解

### 解法一：面积 + 顶点计数（推荐）
```cpp
class Solution {
public:
    bool isRectangleCover(vector<vector<int>>& rectangles) {
        set<pair<int,int>> points;
        int x1 = INT_MAX, y1 = INT_MAX, x2 = INT_MIN, y2 = INT_MIN;
        long area = 0;
        for (auto& r : rectangles) {
            x1 = min(x1, r[0]); y1 = min(y1, r[1]);
            x2 = max(x2, r[2]); y2 = max(y2, r[3]);
            area += (long)(r[2] - r[0]) * (r[3] - r[1]);
            // 四个顶点：出现偶数次则删除，奇数次则保留
            for (auto& p : vector<pair<int,int>>{{r[0],r[1]},{r[0],r[3]},{r[2],r[1]},{r[2],r[3]}}) {
                if (points.count(p)) points.erase(p);
                else points.insert(p);
            }
        }
        if (area != (long)(x2 - x1) * (y2 - y1)) return false;
        if (points.size() != 4) return false;
        return points.count({x1,y1}) && points.count({x1,y2})
            && points.count({x2,y1}) && points.count({x2,y2});
    }
};
```

### 解法二：扫描线
```cpp
class Solution {
public:
    bool isRectangleCover(vector<vector<int>>& rectangles) {
        // 按x坐标扫描，用multiset维护活跃区间
        // 每个x事件：添加/移除y区间，检查无重叠无空隙
        // 实现较复杂，面试中不推荐
        // 省略详细实现
        return true;
    }
};
```

## 易错点
- ✗ 只检查面积相等 -> 无法检测重叠+空隙同时存在的情况
- ✓ 面积 + 顶点双重验证

- ✗ 面积用int -> 可能溢出（坐标范围[-10^5, 10^5]，面积最大4*10^10）
- ✓ 用long存储面积

- ✗ 忘记检查最终剩余4个点是否恰好是大矩形四角
- ✓ 不仅检查size==4，还要检查是正确的4个角

## 面试追问

**Q1: 为什么面积+顶点两个条件就能充分判断？**
> 面积相等排除了空隙（总面积不够）和纯重叠（总面积超了）。顶点条件排除了"有重叠同时有等面积空隙"的情况，因为重叠处的顶点计数会不正确。两者结合是充要条件。

**Q2: 能否用unordered_set代替set提高性能？**
> 可以，需要自定义pair的hash函数。或者用string编码"x,y"作为key。实际上用unordered_set从O(n log n)降到O(n)。

**Q3: 扫描线方法的优势是什么？**
> 扫描线可以处理更复杂的几何问题（如求矩形并集面积），通用性更强。但对于此题，顶点计数法更简洁高效。

## 相关题型
- [223. Rectangle Area](https://leetcode.com/problems/rectangle-area/) - 两矩形面积
- [850. Rectangle Area II](https://leetcode.com/problems/rectangle-area-ii/) - 矩形并集面积（扫描线）
- [836. Rectangle Overlap](https://leetcode.com/problems/rectangle-overlap/) - 矩形重叠判断
