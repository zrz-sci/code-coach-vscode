-- ============================================================
-- 579. Find Cumulative Salary of an Employee
-- ============================================================
-- 难度: Hard | 标签: Database
--
-- 题意: 计算每位员工除最近月份外的3个月滑动窗口累计薪水
--       月份可能不连续，缺失月份薪水视为0
-- ============================================================

-- ============================================================
-- 解法一：自连接 + 子查询（最通用，兼容 MySQL 5.7）
-- ============================================================
-- 思路：
--   1. 子查询找到每位员工的最大月份并排除
--   2. 自连接: e2提供 [e1.month-2, e1.month] 范围内的salary
--   3. GROUP BY + SUM 得到3个月累计
-- 时间复杂度: O(n * k)，k为每个员工的月份数（最多3次匹配）
SELECT e1.id,
       e1.month,
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

-- ============================================================
-- 解法二：窗口函数 RANGE（推荐，MySQL 8.0+）
-- ============================================================
-- 思路：
--   1. RANK() 标记每位员工月份降序排名，rk=1 是最近月
--   2. RANGE BETWEEN 2 PRECEDING 按月份值做滑动窗口（非物理行）
--   3. 过滤掉 rk=1 的最近月
-- 关键: RANGE vs ROWS
--   - RANGE 按 ORDER BY 列的值偏移 → 处理不连续月份正确
--   - ROWS 按物理行偏移 → 月份有gap时会错误累加
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

-- ============================================================
-- 解法三：CTE 分步（清晰易读，面试友好）
-- ============================================================
-- 思路：
--   Step 1: CTE 找出每位员工的最大月份
--   Step 2: CTE 过滤掉最大月份的记录
--   Step 3: 在过滤后的数据上做自连接求3个月累计
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
SELECT f1.id,
       f1.month,
       SUM(f2.salary) AS Salary
FROM filtered f1
JOIN filtered f2
  ON f1.id = f2.id
  AND f2.month BETWEEN f1.month - 2 AND f1.month
GROUP BY f1.id, f1.month
ORDER BY f1.id ASC, f1.month DESC;
