# 585. 2016年的投资 (Investments in 2016)

**难度**: Medium | **标签**: Database | **链接**: [LeetCode 585](https://leetcode.com/problems/investments-in-2016/)

## 核心思路

找到满足**两个条件**的投保人，计算其 2016 年投保金额之和：
1. **tiv_2015 不唯一**: 该投保人的 tiv_2015 值至少与另一个投保人相同
2. **地理位置唯一**: 该投保人的 (lat, lon) 组合在表中唯一

核心技巧是使用**窗口函数 COUNT() OVER** 或**子查询/HAVING** 分别检测这两个条件。

## 思维链

1. **条件拆解**: 两个独立条件需要同时满足
   - 条件A: `tiv_2015` 有重复（同一个 tiv_2015 值出现 >= 2 次）
   - 条件B: `(lat, lon)` 唯一（同一个位置只出现 1 次）
2. **条件A的实现**: COUNT(*) OVER (PARTITION BY tiv_2015) >= 2，或子查询 GROUP BY tiv_2015 HAVING COUNT(*) >= 2
3. **条件B的实现**: COUNT(*) OVER (PARTITION BY lat, lon) = 1，或子查询 GROUP BY lat, lon HAVING COUNT(*) = 1
4. **合并两个条件**: WHERE 条件A AND 条件B
5. **最终聚合**: SUM(tiv_2016) 并 ROUND 到两位小数

## 解法概览

| 解法 | 思路 | 复杂度 | 推荐 |
|------|------|--------|------|
| 窗口函数 | COUNT OVER 按两个维度分别计数 | O(n) | ⭐⭐⭐⭐⭐ |
| IN + 子查询 | 两个子查询分别找满足条件的 tiv_2015 和 (lat,lon) | O(n) | ⭐⭐⭐⭐ |
| EXISTS | 两个 EXISTS/NOT EXISTS 子句 | O(n^2) | ⭐⭐ |
| JOIN | 自连接实现条件检测 | O(n^2) | ⭐⭐ |

## 关键提示

1. **窗口函数方案**: 两个 COUNT OVER 分别用不同的 PARTITION BY，这是最高效的方案
2. **IN 子查询方案**: 条件A用 `tiv_2015 IN (SELECT tiv_2015 ... HAVING COUNT(*) >= 2)`，条件B用 `(lat, lon) NOT IN (SELECT ... HAVING COUNT(*) >= 2)` 或 `(lat, lon) IN (SELECT ... HAVING COUNT(*) = 1)`
3. **ROUND(SUM(...), 2)**: 别忘了保留两位小数
4. **lat 和 lon 必须组合判断**: 不能分开判断 lat 唯一且 lon 唯一

## 解法详解

### 解法一：窗口函数（推荐，MySQL 8+）

```sql
WITH enriched AS (
    SELECT pid, tiv_2016,
           COUNT(*) OVER (PARTITION BY tiv_2015) AS cnt_tiv,
           COUNT(*) OVER (PARTITION BY lat, lon) AS cnt_loc
    FROM Insurance
)
SELECT ROUND(SUM(tiv_2016), 2) AS tiv_2016
FROM enriched
WHERE cnt_tiv >= 2
  AND cnt_loc = 1;
```

**执行流程**（以示例数据为例）：
| pid | tiv_2015 | tiv_2016 | lat | lon | cnt_tiv | cnt_loc |
|-----|----------|----------|-----|-----|---------|---------|
| 1 | 10 | 5 | 10 | 10 | 3 | 1 |
| 2 | 20 | 20 | 20 | 20 | 1 | 2 |
| 3 | 10 | 30 | 20 | 20 | 3 | 2 |
| 4 | 10 | 40 | 40 | 40 | 3 | 1 |

- pid=1: cnt_tiv=3>=2 AND cnt_loc=1=1 -> 保留 (5)
- pid=2: cnt_tiv=1>=2 -> 不满足 -> 过滤
- pid=3: cnt_loc=2=1 -> 不满足 -> 过滤
- pid=4: cnt_tiv=3>=2 AND cnt_loc=1=1 -> 保留 (40)
- 结果: ROUND(5+40, 2) = 45.00

### 解法二：IN 子查询（兼容 MySQL 5.7）

```sql
SELECT ROUND(SUM(tiv_2016), 2) AS tiv_2016
FROM Insurance
WHERE tiv_2015 IN (
    SELECT tiv_2015
    FROM Insurance
    GROUP BY tiv_2015
    HAVING COUNT(*) >= 2
)
AND (lat, lon) IN (
    SELECT lat, lon
    FROM Insurance
    GROUP BY lat, lon
    HAVING COUNT(*) = 1
);
```

**思路**：两个子查询分别筛选出满足条件的 tiv_2015 值集合和位置集合，主查询取交集。

### 解法三：EXISTS（不推荐但面试可能问到）

```sql
SELECT ROUND(SUM(i1.tiv_2016), 2) AS tiv_2016
FROM Insurance i1
WHERE EXISTS (
    SELECT 1 FROM Insurance i2
    WHERE i2.tiv_2015 = i1.tiv_2015 AND i2.pid != i1.pid
)
AND NOT EXISTS (
    SELECT 1 FROM Insurance i2
    WHERE i2.lat = i1.lat AND i2.lon = i1.lon AND i2.pid != i1.pid
);
```

## 易错点

| ✗ 错误写法 | ✓ 正确写法 | 说明 |
|-----------|-----------|------|
| `lat IN (SELECT lat ... HAVING COUNT(*)=1)` | `(lat, lon) IN (SELECT lat, lon ... HAVING COUNT(*)=1)` | 位置唯一性必须用(lat,lon)组合判断 |
| `SUM(tiv_2016)` | `ROUND(SUM(tiv_2016), 2)` | 忘记 ROUND 到两位小数 |
| `COUNT(*) > 1` 用于位置条件 | `COUNT(*) = 1` 用于位置条件 | 位置要求唯一（=1），不是重复（>1） |
| `HAVING COUNT(*) > 1` for tiv_2015 | `HAVING COUNT(*) >= 2` | 语义等价但 >= 2 更清晰表达"至少另一个" |
| 窗口函数放在 WHERE 中 | 窗口函数放在 CTE/子查询中 | 窗口函数不能直接在 WHERE 中使用，需要包一层 |

## 面试追问

**Q1: 窗口函数 COUNT OVER 和 GROUP BY COUNT 有什么区别？**

A1: GROUP BY COUNT 会把多行聚合为一行，丢失原始行的其他列信息。窗口函数 COUNT OVER 保留每一行，只是新增一个聚合列。在本题中，我们需要保留每行的 tiv_2016 来做最终的 SUM，所以窗口函数更合适。如果用 GROUP BY，需要额外 JOIN 回原表。

**Q2: 如果 tiv_2015 的条件改为"恰好与另一个投保人相同"（不是一个或多个），查询怎么改？**

A2: 将 `cnt_tiv >= 2` 改为 `cnt_tiv = 2`。这意味着 tiv_2015 恰好出现两次（即恰好与另一个人相同，没有第三个人也是这个值）。

**Q3: 如何优化这个查询在大数据集上的性能？**

A3: (1) 在 tiv_2015 上建索引，加速条件A的子查询；(2) 在 (lat, lon) 上建组合索引，加速条件B；(3) 使用窗口函数方案只扫描一次表；(4) 如果 Insurance 表很大，可以先用 CTE 分别计算满足两个条件的 pid 集合，再取交集做 SUM。

## 相关题型

- [184. Department Highest Salary](https://leetcode.com/problems/department-highest-salary/) - 分组聚合
- [180. Consecutive Numbers](https://leetcode.com/problems/consecutive-numbers/) - 重复值检测
- [196. Delete Duplicate Emails](https://leetcode.com/problems/delete-duplicate-emails/) - 重复值处理
- [1204. Last Person to Fit in the Bus](https://leetcode.com/problems/last-person-to-fit-in-the-bus/) - 窗口函数累计
- [1321. Restaurant Growth](https://leetcode.com/problems/restaurant-growth/) - 滑动窗口聚合
