# 579. 查询员工的累计薪水 (Find Cumulative Salary of an Employee)

**难度**: Hard | **标签**: Database | **链接**: [LeetCode 579](https://leetcode.com/problems/find-cumulative-salary-of-an-employee/)

## 核心思路

本题要求计算每位员工的**3个月滑动窗口累计薪水**，但要**排除每位员工最近一个月**的记录。核心挑战在于：
1. 月份可能不连续（如员工1有月份1,2,3,4,7,8，缺少5和6）
2. 不存在的月份薪水视为0
3. 需要排除每位员工的最大月份

关键技术点是使用**窗口函数**或**自连接**实现滑动累计，配合子查询排除最近月。

## 思维链

1. **理解"3个月累计"**: 对每个(id, month)，累计 = 当月salary + 前1个月salary + 前2个月salary
2. **理解"不连续月份"**: 月份5和6没有记录，所以月份7的累计只有自己(90+0+0=90)
3. **排除最近月**: 先找到每位员工的MAX(month)，然后过滤掉
4. **选择技术方案**: 窗口函数 SUM() OVER (ROWS BETWEEN 2 PRECEDING AND CURRENT ROW) 或自连接
5. **注意窗口范围**: ROWS vs RANGE — 这里必须用自连接或RANGE来处理不连续月份的gap

## 解法概览

| 解法 | 思路 | 复杂度 | 推荐 |
|------|------|--------|------|
| 自连接 + 子查询 | 同一员工自连接，月份差在[0,2]范围内求和 | O(n^2) | ⭐⭐⭐ |
| 窗口函数 (RANGE) | SUM OVER + RANGE BETWEEN 排除最近月 | O(n log n) | ⭐⭐⭐⭐ |
| CTE + 自连接 | 先CTE排除最近月，再自连接求累计 | O(n^2) | ⭐⭐ |
| 窗口函数 + DENSE_RANK | 用DENSE_RANK排除最近月配合窗口累计 | O(n log n) | ⭐⭐⭐⭐⭐ |

## 关键提示

1. `ROWS BETWEEN 2 PRECEDING AND CURRENT ROW` 在月份不连续时**会出错**，因为ROWS按物理行计算而非按月份值
2. 需要先排除最近月，否则滑动窗口计算会把最近月的值纳入其他月份的累计
3. 自连接条件: `e1.id = e2.id AND e2.month BETWEEN e1.month - 2 AND e1.month`
4. 输出列名是 `Salary`（大写S），不是 `salary`

## 解法详解

### 解法一：自连接 + 子查询（最通用）

```sql
SELECT e1.id, e1.month,
       SUM(e2.salary) AS Salary
FROM Employee e1
JOIN Employee e2
  ON e1.id = e2.id
  AND e2.month BETWEEN e1.month - 2 AND e1.month
WHERE (e1.id, e1.month) NOT IN (
    SELECT id, MAX(month)
    FROM Employee
    GROUP BY id
)
GROUP BY e1.id, e1.month
ORDER BY e1.id ASC, e1.month DESC;
```

**思路**：e1是基准行，e2提供当月及前两个月的salary。自连接天然处理了月份不连续的情况——如果前面的月份不存在，JOIN不到，SUM自动忽略。

### 解法二：窗口函数（推荐，MySQL 8+）

```sql
WITH ranked AS (
    SELECT id, month, salary,
           RANK() OVER (PARTITION BY id ORDER BY month DESC) AS rk
    FROM Employee
)
SELECT id, month,
       SUM(salary) OVER (
           PARTITION BY id
           ORDER BY month
           RANGE BETWEEN 2 PRECEDING AND CURRENT ROW
       ) AS Salary
FROM ranked
WHERE rk > 1
ORDER BY id ASC, month DESC;
```

**思路**：先用RANK()标记每位员工的最近月(rk=1)并排除，再用窗口函数RANGE BETWEEN按月份值做滑动累计。RANGE按值计算，完美处理月份不连续的gap。

### 解法三：CTE分步（清晰易读）

```sql
WITH max_month AS (
    SELECT id, MAX(month) AS max_m
    FROM Employee
    GROUP BY id
),
filtered AS (
    SELECT e.id, e.month, e.salary
    FROM Employee e
    JOIN max_month m ON e.id = m.id
    WHERE e.month < m.max_m
)
SELECT f1.id, f1.month,
       SUM(f2.salary) AS Salary
FROM filtered f1
JOIN filtered f2
  ON f1.id = f2.id
  AND f2.month BETWEEN f1.month - 2 AND f1.month
GROUP BY f1.id, f1.month
ORDER BY f1.id ASC, f1.month DESC;
```

## 易错点

| ✗ 错误写法 | ✓ 正确写法 | 说明 |
|-----------|-----------|------|
| `ROWS BETWEEN 2 PRECEDING AND CURRENT ROW` | `RANGE BETWEEN 2 PRECEDING AND CURRENT ROW` | ROWS按物理行，RANGE按值。月份不连续时ROWS会错误地累加非相邻月份 |
| `WHERE month != MAX(month)` | 子查询 `NOT IN (SELECT id, MAX(month) ...)` | 不能直接在WHERE中用聚合函数 |
| `SUM(salary) AS salary` | `SUM(salary) AS Salary` | 输出列名大小写需匹配题目要求 |
| 不排除最近月就做窗口计算 | 先CTE排除最近月，再做窗口计算 | 最近月的数据会影响前面月份的窗口范围 |
| `e2.month BETWEEN e1.month AND e1.month + 2` | `e2.month BETWEEN e1.month - 2 AND e1.month` | 方向错误：应该是向前看2个月，不是向后 |

## 面试追问

**Q1: 如果月份可能有重复记录（同一员工同月多条记录），查询需要怎么改？**

A1: 需要先按(id, month)做GROUP BY SUM合并，再进行滑动窗口计算。可以用CTE先聚合：
```sql
WITH merged AS (
    SELECT id, month, SUM(salary) AS salary
    FROM Employee GROUP BY id, month
)
```

**Q2: ROWS 和 RANGE 在窗口函数中的区别是什么？举例说明。**

A2: ROWS按物理行偏移（第前1行、第前2行），RANGE按ORDER BY列的值偏移。例如月份序列[1,2,4]中，对月份4：ROWS 2 PRECEDING会取[2,4]两行；RANGE 2 PRECEDING会取月份值在[2,4]范围内的行，也是[2,4]。但对月份序列[1,3,4]中的月份4：ROWS取[3,4]，RANGE取[2,4]范围即[3,4]。两者在有gap或重复值时行为不同。

**Q3: 如果数据量很大（千万级），如何优化这个查询？**

A3: (1) 在(id, month)上建组合索引；(2) 使用窗口函数方案替代自连接，避免O(n^2)；(3) 如果只关心部分员工，先用WHERE id IN (...)过滤减少数据量；(4) 考虑按id分区做并行计算。

## 相关题型

- [178. Rank Scores](https://leetcode.com/problems/rank-scores/) - 窗口函数排名
- [180. Consecutive Numbers](https://leetcode.com/problems/consecutive-numbers/) - 连续值检测
- [185. Department Top Three Salaries](https://leetcode.com/problems/department-top-three-salaries/) - 分组Top-N
- [615. Average Salary: Departments vs Company](https://leetcode.com/problems/average-salary-departments-vs-company/) - 分组聚合比较
- [1454. Active Users](https://leetcode.com/problems/active-users/) - 连续登录天数
