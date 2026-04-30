# 612. Shortest Distance in a Plane (平面上的最近距离)

**难度**: Medium | **标签**: Database

## 核心思路

求二维平面上所有点对之间的最短欧几里得距离。核心步骤:
1. **笛卡尔积**: 将 Point2D 表与自身做 CROSS JOIN, 产生所有点对
2. **排除自身配对**: 同一个点不能与自己配对
3. **计算距离**: `SQRT((x2-x1)^2 + (y2-y1)^2)`
4. **取最小值**: `MIN()` 聚合并 `ROUND(..., 2)` 保留两位小数

关键: 如何排除同一个点与自己配对 -- 使用 `WHERE (p1.x, p1.y) != (p2.x, p2.y)` 或 `WHERE p1.x != p2.x OR p1.y != p2.y`。

## 思维链

1. **问题转化**: 求 n 个二维点中最近点对的距离
2. **暴力枚举**: 需要检查所有 C(n,2) 对点
3. **SQL 实现**: CROSS JOIN 表自身, 得到 n^2 对, 过滤掉 (i,i) 对
4. **距离公式**: `SQRT(POW(x2-x1, 2) + POW(y2-y1, 2))`
5. **聚合求最小**: `SELECT MIN(distance)`, 不需要 GROUP BY
6. **精度处理**: `ROUND(..., 2)` 保留两位小数
7. **性能考虑**: CROSS JOIN 产生 O(n^2) 行, 数据量大时需注意

## 解法概览

| 解法 | 思路 | 时间复杂度 | 推荐度 |
|------|------|-----------|--------|
| CROSS JOIN + MIN | 枚举所有点对取最小距离 | O(n^2) | ⭐⭐⭐⭐⭐ |
| 不开方比较 | 比较距离平方, 最后再开方 | O(n^2) | ⭐⭐⭐⭐ |
| 带条件避免重复 | p1 < p2 只算一半 | O(n^2) | ⭐⭐⭐⭐ |

## 关键提示

1. CROSS JOIN 等价于 `FROM Point2D p1, Point2D p2`, 不需要 ON 条件
2. 必须排除同一个点自身配对: `WHERE (p1.x, p1.y) != (p2.x, p2.y)`
3. `SQRT` 和 `POW` 是 MySQL 内置函数, 也可以用 `(x2-x1)*(x2-x1)` 替代 `POW`
4. `ROUND(value, 2)` 保留两位小数 -- 题目明确要求
5. (x,y) 是联合主键, 不会有重复点, 所以排除自身配对条件足够

## 解法详解

### 解法一: CROSS JOIN + MIN (推荐)

```sql
SELECT
    ROUND(
        MIN(SQRT(POW(p1.x - p2.x, 2) + POW(p1.y - p2.y, 2))),
        2
    ) AS shortest
FROM Point2D p1
CROSS JOIN Point2D p2
WHERE (p1.x, p1.y) != (p2.x, p2.y);
```

**思路**: 笛卡尔积枚举所有点对, 排除自身, 计算欧几里得距离后取 MIN。

### 解法二: 比较距离平方 (性能优化)

```sql
SELECT
    ROUND(
        SQRT(MIN(POW(p1.x - p2.x, 2) + POW(p1.y - p2.y, 2))),
        2
    ) AS shortest
FROM Point2D p1
CROSS JOIN Point2D p2
WHERE (p1.x, p1.y) != (p2.x, p2.y);
```

**思路**: 先对距离的平方取 MIN, 最后再开方。因为 `SQRT` 是单调递增函数, MIN(SQRT(d)) = SQRT(MIN(d)), 减少了 n^2 次开方运算为 1 次。

### 解法三: 避免重复配对

```sql
SELECT
    ROUND(
        SQRT(MIN(POW(p1.x - p2.x, 2) + POW(p1.y - p2.y, 2))),
        2
    ) AS shortest
FROM Point2D p1
JOIN Point2D p2
    ON (p1.x, p1.y) < (p2.x, p2.y);
```

**思路**: 用 `<` 确保每对点只算一次 (利用元组比较), 将计算量减半。MySQL 支持行值表达式比较 `(a,b) < (c,d)`, 按字典序比较。

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ 不排除自身配对 | ✓ `WHERE (p1.x, p1.y) != (p2.x, p2.y)` | 自身配对距离为 0, 结果错误 |
| ✗ `WHERE p1.x != p2.x AND p1.y != p2.y` | ✓ `WHERE p1.x != p2.x OR p1.y != p2.y` | AND 会漏掉 x 相同或 y 相同的不同点 |
| ✗ 忘记 `ROUND(..., 2)` | ✓ `ROUND(SQRT(...), 2)` | 题目要求保留两位小数 |
| ✗ `ROUND(MIN(SQRT(...)), 2)` | ✓ 都可以, 但推荐 `ROUND(SQRT(MIN(...)), 2)` | 先 MIN 距离平方再开方, 减少计算 |

## 面试追问

**Q1: CROSS JOIN 产生 n^2 行, 如果点表有 100 万行怎么办?**

A: 100 万行会产生 10^12 行笛卡尔积, 纯 SQL 无法处理。实际中需要:
- 算法优化: 使用分治法 (Closest Pair Algorithm), O(n log n)
- 空间索引: 使用 R-Tree 索引或 MySQL 的 SPATIAL INDEX
- 近似方法: 先用网格划分 (Grid-based), 只计算相邻网格中的点对

**Q2: 如果表中有重复点 (x,y 不是主键), 应该如何修改查询?**

A: 需要给每行加行号来区分:
```sql
WITH numbered AS (
    SELECT x, y, ROW_NUMBER() OVER () AS rn FROM Point2D
)
SELECT ROUND(SQRT(MIN(POW(a.x-b.x,2)+POW(a.y-b.y,2))),2) AS shortest
FROM numbered a JOIN numbered b ON a.rn < b.rn;
```
或者如果有重复点, 最短距离就是 0。

**Q3: `ROUND(SQRT(MIN(...)),2)` 和 `ROUND(MIN(SQRT(...)),2)` 有区别吗?**

A: 数学结果相同 (因为 SQRT 单调递增, MIN 可交换)。但性能有区别: 前者只做一次 SQRT, 后者对 n^2 个值都做 SQRT。实际测试中前者更快。

## 相关题型

| 题号 | 题目 | 关联 |
|------|------|------|
| 613 | Shortest Distance in a Line | 一维版本, 更简单 |
| 1179 | Reformat Department Table | 行列转换/聚合 |
| 1484 | Group Sold Products By The Date | 聚合函数应用 |
| 1661 | Average Time of Process per Machine | 自连接 + 聚合 |
