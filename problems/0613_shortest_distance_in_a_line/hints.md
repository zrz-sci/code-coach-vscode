# 613. Shortest Distance in a Line (直线上的最近距离)

**难度**: Easy | **标签**: Database

## 核心思路

求一维数轴上所有点对之间的最短距离。核心步骤:
1. **自连接**: Point 表与自身 JOIN, 枚举所有不同点对
2. **计算距离**: `ABS(p1.x - p2.x)`, 一维距离就是坐标差的绝对值
3. **取最小值**: `MIN(ABS(p1.x - p2.x))`

优化思路: 如果表已排序, 最短距离一定出现在相邻点之间, 可用窗口函数 `LEAD/LAG` 只比较相邻对。

## 思维链

1. **问题简化**: 一维点集求最近点对, 比 612 (二维) 简单很多
2. **暴力法**: 枚举所有 C(n,2) 对, 算 |x1 - x2|, 取 MIN
3. **SQL 实现**: 自连接 + ABS + MIN
4. **排除自身**: `WHERE p1.x != p2.x` (因为 x 是主键, 唯一即可)
5. **优化**: 排序后相邻差值 -> 用窗口函数 `LEAD(x) OVER (ORDER BY x) - x`
6. **Follow-up 回答**: 如果表已排序, 用窗口函数是 O(n) 的

## 解法概览

| 解法 | 思路 | 时间复杂度 | 推荐度 |
|------|------|-----------|--------|
| 自连接 + ABS + MIN | 枚举所有点对 | O(n^2) | ⭐⭐⭐⭐⭐ |
| 自连接 + 不等式避免重复 | p1.x < p2.x 减半 | O(n^2) | ⭐⭐⭐⭐ |
| 窗口函数 LEAD | 排序后只算相邻差 | O(n log n) | ⭐⭐⭐⭐ |
| 子查询求下一个点 | 相关子查询 | O(n^2) | ⭐⭐⭐ |

## 关键提示

1. x 是主键, 值唯一, 所以 `p1.x != p2.x` 就能排除自身配对
2. `ABS` 函数取绝对值, 或者用 `p1.x - p2.x` 配合 `WHERE p1.x < p2.x` 保证非负
3. Follow-up 问的是排序优化 -- 排序后最短距离一定在相邻点, 用 `LEAD` 窗口函数
4. 不需要 `ROUND`, 因为坐标是整数, 差值也是整数
5. 输出列名是 `shortest`

## 解法详解

### 解法一: 自连接 + ABS + MIN (推荐)

```sql
SELECT
    MIN(ABS(p1.x - p2.x)) AS shortest
FROM Point p1
JOIN Point p2
    ON p1.x != p2.x;
```

**思路**: 枚举所有不同点对, 计算一维距离的绝对值, 取最小值。

### 解法二: 不等式避免重复

```sql
SELECT
    MIN(p2.x - p1.x) AS shortest
FROM Point p1
JOIN Point p2
    ON p1.x < p2.x;
```

**思路**: 强制 `p1.x < p2.x`, 这样 `p2.x - p1.x` 一定为正, 不需要 ABS。同时每对只算一次, 计算量减半。

### 解法三: 窗口函数 LEAD (Follow-up 最优解)

```sql
SELECT
    MIN(next_x - x) AS shortest
FROM (
    SELECT
        x,
        LEAD(x) OVER (ORDER BY x) AS next_x
    FROM Point
) t
WHERE next_x IS NOT NULL;
```

**思路**: 排序后, 最短距离一定出现在相邻点之间。用 `LEAD` 获取下一个点的 x 值, 算差值, 取 MIN。时间复杂度 O(n log n) (排序) + O(n) (扫描), 远优于 O(n^2) 的自连接。

### 解法四: 相关子查询 (效率较低)

```sql
SELECT
    MIN(
        (SELECT MIN(ABS(p1.x - p2.x))
         FROM Point p2
         WHERE p2.x != p1.x)
    ) AS shortest
FROM Point p1;
```

**思路**: 对每个点, 用相关子查询找到最近的另一个点。嵌套两层 MIN。

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ 不排除自身 `ON 1=1` | ✓ `ON p1.x != p2.x` | 自身配对距离为 0, 结果错误 |
| ✗ `MIN(p1.x - p2.x)` 不取绝对值 | ✓ `MIN(ABS(p1.x - p2.x))` 或用 `p1.x < p2.x` | 差值可能为负, MIN 会取到最大负数 |
| ✗ LEAD 后不过滤 NULL | ✓ `WHERE next_x IS NOT NULL` | 最后一行的 LEAD 为 NULL, 会导致结果出错 |
| ✗ 输出列名写成 `distance` | ✓ 列名必须是 `shortest` | 必须匹配题目要求 |

## 面试追问

**Q1: 题目 Follow-up: 如果表按升序排列, 如何优化?**

A: 如果已排序, 最短距离一定在相邻点之间 (数学性质: 排序后 |a[i]-a[j]| >= |a[i]-a[i+1]| 对 j>i+1)。使用窗口函数:
```sql
SELECT MIN(next_x - x) AS shortest
FROM (
    SELECT x, LEAD(x) OVER (ORDER BY x) AS next_x FROM Point
) t WHERE next_x IS NOT NULL;
```
如果数据已物理排序, 甚至可以省去 ORDER BY, 直接顺序扫描。

**Q2: 自连接方案中, `p1.x < p2.x` 和 `p1.x != p2.x` 有什么区别?**

A: `p1.x != p2.x` 会产生 n*(n-1) 行 (每对算两次), `p1.x < p2.x` 只产生 C(n,2) = n*(n-1)/2 行 (每对算一次)。后者计算量减半, 且不需要 ABS 因为差值一定为正。

**Q3: 如果需要返回最短距离对应的两个点坐标, 怎么改?**

A: 使用子查询或 CTE 先算最短距离, 再回查:
```sql
WITH dists AS (
    SELECT p1.x AS x1, p2.x AS x2, ABS(p1.x - p2.x) AS dist
    FROM Point p1 JOIN Point p2 ON p1.x < p2.x
)
SELECT x1, x2, dist AS shortest
FROM dists WHERE dist = (SELECT MIN(dist) FROM dists);
```

## 相关题型

| 题号 | 题目 | 关联 |
|------|------|------|
| 612 | Shortest Distance in a Plane | 二维版本, 用 CROSS JOIN |
| 1501 | Countries You Can Safely Invest In | 自连接 + 聚合 |
| 1661 | Average Time of Process per Machine | 自连接计算差值 |
| 177 | Nth Highest Salary | 窗口函数应用 |
| 178 | Rank Scores | 窗口函数 RANK/DENSE_RANK |
