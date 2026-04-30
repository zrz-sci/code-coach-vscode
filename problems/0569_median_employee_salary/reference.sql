-- =============================================
-- LeetCode 569: Median Employee Salary
-- =============================================

-- 【题目本质】
-- 在每个公司内部找中位数所在的行（偶数个取两行，奇数个取一行）。
-- 排序规则: salary ASC, id ASC。

-- 【解法总览】
-- 1. 窗口函数 ROW_NUMBER (推荐)
-- 2. 自连接计数法 (无窗口函数, Follow-up)

-- =============================================
-- 解法一: 窗口函数 ROW_NUMBER（推荐）
-- 思路: 用 ROW_NUMBER 编号，用 COUNT 统计总数，取中位数位置
-- =============================================
SELECT id, company, salary
FROM (
    SELECT id, company, salary,
           ROW_NUMBER() OVER (PARTITION BY company ORDER BY salary ASC, id ASC) AS rn,
           COUNT(*) OVER (PARTITION BY company) AS cnt
    FROM Employee
) t
WHERE rn IN (FLOOR((cnt + 1) / 2.0), CEIL((cnt + 1) / 2.0));

-- =============================================
-- 解法二: 自连接计数法（无窗口函数）
-- 思路: 对每个员工，统计同公司比它"小"的人数和"大"的人数，差 <= 1 即为中位数
-- =============================================
SELECT e1.id, e1.company, e1.salary
FROM Employee e1
WHERE ABS(
    (SELECT COUNT(*)
     FROM Employee e2
     WHERE e2.company = e1.company
       AND (e2.salary < e1.salary OR (e2.salary = e1.salary AND e2.id < e1.id)))
    -
    (SELECT COUNT(*)
     FROM Employee e3
     WHERE e3.company = e1.company
       AND (e3.salary > e1.salary OR (e3.salary = e1.salary AND e3.id > e1.id)))
) <= 1;

-- 【解法对比】
-- | 解法            | 优点           | 缺点               |
-- |----------------|---------------|-------------------|
-- | 窗口函数        | 简洁高效        | 需要支持窗口函数     |
-- | 自连接计数       | 无需窗口函数    | O(n²) 性能较差      |

-- 【易错点】
-- 1. 排序必须包含 id 作为 tiebreaker
-- 2. 偶数个元素时中位数是两行，不能只取一行
-- 3. FLOOR/CEIL 公式要用 (cnt+1)/2.0 保证浮点除法

-- 【面试追问】
-- Q1: 不用窗口函数怎么做？ → 解法二自连接
-- Q2: 如果要返回中位数数值而非整行？ → 外层 GROUP BY company, AVG(salary)
-- Q3: 大数据量下如何优化？ → 对 (company, salary, id) 建复合索引
