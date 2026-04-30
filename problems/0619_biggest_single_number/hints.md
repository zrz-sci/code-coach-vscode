# 619. Biggest Single Number (只出现一次的最大数字)

**难度**: Easy | **标签**: Database

## 核心思路

分两步完成:
1. **找出只出现一次的数字**: `GROUP BY num HAVING COUNT(*) = 1`
2. **取其中最大的**: `MAX(num)` 或 `ORDER BY num DESC LIMIT 1`

关键难点: 如果不存在任何只出现一次的数字, 需要返回 `NULL`。`MAX()` 作用于空结果集时自动返回 NULL, 这是最优雅的处理方式。

## 思维链

1. **理解 "single number"**: 在表中只出现 1 次的数字
2. **统计频次**: GROUP BY num, 用 HAVING COUNT(*) = 1 筛选
3. **取最大值**: 对筛选结果取 MAX
4. **处理空集**: 如果没有 single number, 必须返回 NULL (不是空结果集)
5. **MAX 的特性**: `SELECT MAX(col) FROM empty_table` 返回 NULL 行, 完美解决空集问题
6. **子查询包装**: 将步骤 2 作为子查询, 外层 SELECT MAX

## 解法概览

| 解法 | 思路 | 时间复杂度 | 推荐度 |
|------|------|-----------|--------|
| 子查询 + MAX | 子查询筛 single, 外层 MAX | O(n log n) | ⭐⭐⭐⭐⭐ |
| CTE + MAX | CTE 语法更清晰 | O(n log n) | ⭐⭐⭐⭐ |
| 标量子查询 + LIMIT | 排序取第一个 | O(n log n) | ⭐⭐⭐⭐ |
| 窗口函数 | COUNT OVER + MAX | O(n log n) | ⭐⭐⭐ |

## 关键提示

1. 表没有主键, 可能有重复行, 所以必须用 `GROUP BY` + `HAVING COUNT(*) = 1`
2. `MAX()` 聚合空集返回 NULL -- 这是处理 "无 single number" 的最佳方式
3. 不能用 `LIMIT 1` 直接返回, 因为空结果集时 LIMIT 返回 0 行而非 NULL 行
4. 如果用 LIMIT 方案, 需要包一层标量子查询或用 IFNULL
5. 输出列名是 `num`

## 解法详解

### 解法一: 子查询 + MAX (推荐)

```sql
SELECT MAX(num) AS num
FROM (
    SELECT num
    FROM MyNumbers
    GROUP BY num
    HAVING COUNT(*) = 1
) t;
```

**思路**: 内层子查询用 GROUP BY + HAVING 筛出所有只出现一次的数字, 外层 MAX 取最大值。如果子查询结果为空, MAX 自动返回 NULL。

### 解法二: CTE + MAX

```sql
WITH singles AS (
    SELECT num
    FROM MyNumbers
    GROUP BY num
    HAVING COUNT(*) = 1
)
SELECT MAX(num) AS num
FROM singles;
```

**思路**: 与解法一等价, 用 CTE 提高可读性。

### 解法三: ORDER BY + LIMIT (需额外处理 NULL)

```sql
SELECT (
    SELECT num
    FROM MyNumbers
    GROUP BY num
    HAVING COUNT(*) = 1
    ORDER BY num DESC
    LIMIT 1
) AS num;
```

**思路**: 将整个查询作为标量子查询放在 SELECT 中。标量子查询返回空集时, 自动变为 NULL。

### 解法四: 窗口函数

```sql
SELECT MAX(num) AS num
FROM (
    SELECT num, COUNT(*) OVER (PARTITION BY num) AS cnt
    FROM MyNumbers
) t
WHERE cnt = 1;
```

**思路**: 窗口函数为每行计算该 num 的出现次数, 过滤 cnt=1 的行, 再取 MAX。

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ `SELECT num FROM ... ORDER BY DESC LIMIT 1` 不包子查询 | ✓ 包在 `SELECT (...) AS num` 中 | 直接 LIMIT 空结果集返回 0 行, 不返回 NULL |
| ✗ `HAVING COUNT(num) = 1` | ✓ `HAVING COUNT(*) = 1` | 两者在无 NULL 时等价, 但 COUNT(*) 更通用语义更明确 |
| ✗ `WHERE COUNT(*) = 1` | ✓ `HAVING COUNT(*) = 1` | 聚合条件必须用 HAVING, 不能用 WHERE |
| ✗ 用 `DISTINCT num` 替代 `GROUP BY` | ✓ DISTINCT 不能判断出现次数 | DISTINCT 只去重, 无法知道原始频次 |
| ✗ 忘记处理空集返回 NULL | ✓ 用 MAX 包装 | 题目要求无 single number 时返回 null |

## 面试追问

**Q1: 为什么 MAX() 作用于空集返回 NULL 而不是报错?**

A: 这是 SQL 标准定义的行为。所有聚合函数 (MAX, MIN, SUM, AVG) 作用于空集时都返回 NULL (COUNT 除外, 返回 0)。这是因为 NULL 语义为 "未知/不存在", 对空集取最大值确实是 "不存在" 的, 返回 NULL 在语义上是正确的。

**Q2: 如果要找出现次数恰好为 k 次的最大数字, 怎么改?**

A: 只需修改 HAVING 条件:
```sql
SELECT MAX(num) AS num
FROM (
    SELECT num FROM MyNumbers GROUP BY num HAVING COUNT(*) = k
) t;
```
将 `= 1` 改为 `= k` 即可。这是一个通用模板。

**Q3: 如果需要同时返回最大和最小的 single number, 怎么做?**

A: 在外层同时使用 MAX 和 MIN:
```sql
SELECT MAX(num) AS max_single, MIN(num) AS min_single
FROM (
    SELECT num FROM MyNumbers GROUP BY num HAVING COUNT(*) = 1
) t;
```
如果没有 single number, 两列都返回 NULL。

## 相关题型

| 题号 | 题目 | 关联 |
|------|------|------|
| 176 | Second Highest Salary | MAX + 空集返回 NULL |
| 586 | Customer Placing the Largest Number of Orders | GROUP BY + HAVING + 排序 |
| 1050 | Actors and Directors Who Cooperated At Least Three Times | GROUP BY + HAVING COUNT |
| 1069 | Product Sales Analysis II | GROUP BY 聚合 |
| 1084 | Sales Analysis III | HAVING 条件过滤 |
