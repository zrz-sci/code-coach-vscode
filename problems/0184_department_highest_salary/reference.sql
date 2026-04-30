/*
 * LeetCode 184. 部门工资最高的员工 (Medium)
 *
 * 【题目本质】
 *   分组求极值 + 多表连接 —— 每个部门中工资 = MAX(salary) 的员工。
 *   核心考点：GROUP BY + MAX / 窗口函数 RANK；注意并列最高的情况。
 *
 * 【解法总览】
 *   解法1: 子查询 + IN 元组匹配  — 经典写法，推荐 ⭐⭐⭐
 *   解法2: 窗口函数 RANK()       — 通用性强，推荐 ⭐⭐⭐
 *   解法3: 相关子查询            — 直观但性能一般 ⭐⭐
 */

-- ============================================================
-- 解法1: 子查询 + IN 元组匹配（推荐）
-- 思路：先求每个部门的 MAX(salary)，再用 (departmentId, salary) 匹配
-- 优点：处理并列最高工资的多个员工
-- ============================================================
SELECT d.name AS Department,
       e.name AS Employee,
       e.salary AS Salary
FROM Employee e
JOIN Department d
  ON e.departmentId = d.id
WHERE (e.departmentId, e.salary) IN (
    SELECT departmentId, MAX(salary)
    FROM Employee
    GROUP BY departmentId
);

-- ============================================================
-- 解法2: 窗口函数 RANK()（推荐）
-- 思路：按部门分区、工资降序排名，取 rank = 1
-- 注意：用 RANK() 而非 ROW_NUMBER()，以保留并列
-- ============================================================
SELECT Department, Employee, Salary
FROM (
    SELECT d.name AS Department,
           e.name AS Employee,
           e.salary AS Salary,
           RANK() OVER (
               PARTITION BY e.departmentId
               ORDER BY e.salary DESC
           ) AS rk
    FROM Employee e
    JOIN Department d
      ON e.departmentId = d.id
) ranked
WHERE rk = 1;

-- ============================================================
-- 解法3: 相关子查询
-- 思路：对每个员工，检查其工资是否等于所在部门的最高工资
-- ============================================================
SELECT d.name AS Department,
       e.name AS Employee,
       e.salary AS Salary
FROM Employee e
JOIN Department d
  ON e.departmentId = d.id
WHERE e.salary = (
    SELECT MAX(salary)
    FROM Employee
    WHERE departmentId = e.departmentId
);

/*
 * 【解法对比】
 * +---------------------+----------+-----------+------+
 * | 解法                | 可读性   | 扩展性    | 推荐 |
 * +---------------------+----------+-----------+------+
 * | 子查询 + IN         | 高       | 仅Top1    | ⭐⭐⭐ |
 * | 窗口函数 RANK()     | 高       | Top N通用 | ⭐⭐⭐ |
 * | 相关子查询          | 中       | 仅Top1    | ⭐⭐   |
 * +---------------------+----------+-----------+------+
 *
 * 【易错点】
 *   1. IN 只匹配 salary 不匹配 departmentId → 跨部门误匹配
 *   2. 用 ROW_NUMBER 替代 RANK → 并列最高只保留一个
 *   3. WHERE 中直接用 MAX() → 语法错误，需子查询
 *
 * 【面试追问】
 *   Q1: 改为 Top 3 怎么办？ → 用 RANK() 或 DENSE_RANK()，WHERE rk <= 3
 *   Q2: RANK vs DENSE_RANK vs ROW_NUMBER 区别？
 *       → RANK(1,2,2,4) / DENSE_RANK(1,2,2,3) / ROW_NUMBER(1,2,3,4)
 *   Q3: 无窗口函数的数据库怎么做 Top N？
 *       → 相关子查询: WHERE (SELECT COUNT(DISTINCT salary) ... ) < N
 */
