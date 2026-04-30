-- LeetCode 615: Average Salary: Departments VS Company
-- 难度: Hard
-- 思路: 分别计算公司月平均和部门月平均，再做比较

-- 方法一：CTE
WITH company_avg AS (
    SELECT
        DATE_FORMAT(pay_date, '%Y-%m') AS pay_month,
        AVG(amount) AS company_avg_salary
    FROM Salary
    GROUP BY DATE_FORMAT(pay_date, '%Y-%m')
),
dept_avg AS (
    SELECT
        DATE_FORMAT(s.pay_date, '%Y-%m') AS pay_month,
        e.department_id,
        AVG(s.amount) AS dept_avg_salary
    FROM Salary s
    JOIN Employee e ON s.employee_id = e.employee_id
    GROUP BY DATE_FORMAT(s.pay_date, '%Y-%m'), e.department_id
)
SELECT
    d.pay_month,
    d.department_id,
    CASE
        WHEN d.dept_avg_salary > c.company_avg_salary THEN 'higher'
        WHEN d.dept_avg_salary < c.company_avg_salary THEN 'lower'
        ELSE 'same'
    END AS comparison
FROM dept_avg d
JOIN company_avg c ON d.pay_month = c.pay_month;

-- 方法二：窗口函数
-- SELECT DISTINCT
--     DATE_FORMAT(s.pay_date, '%Y-%m') AS pay_month,
--     e.department_id,
--     CASE
--         WHEN AVG(s.amount) OVER (PARTITION BY e.department_id, DATE_FORMAT(s.pay_date, '%Y-%m'))
--              > AVG(s.amount) OVER (PARTITION BY DATE_FORMAT(s.pay_date, '%Y-%m'))
--         THEN 'higher'
--         WHEN AVG(s.amount) OVER (PARTITION BY e.department_id, DATE_FORMAT(s.pay_date, '%Y-%m'))
--              < AVG(s.amount) OVER (PARTITION BY DATE_FORMAT(s.pay_date, '%Y-%m'))
--         THEN 'lower'
--         ELSE 'same'
--     END AS comparison
-- FROM Salary s
-- JOIN Employee e ON s.employee_id = e.employee_id;
