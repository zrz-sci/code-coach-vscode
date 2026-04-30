/*
 * LeetCode 185. 部门工资前三高的所有员工 (Hard)
 *
 * 【题目本质】
 *   分组 Top-N —— 在每个部门中，找出工资排在前三的不同薪资等级的所有员工。
 *   核心考点：DENSE_RANK 窗口函数 / 相关子查询 COUNT(DISTINCT)。
 *   难点：理解"前三高的不同工资"意味着并列不占额外名次(DENSE_RANK)。
 *
 * 【解法总览】
 *   解法1: 窗口函数 DENSE_RANK()    — 简洁通用，推荐 ⭐⭐⭐
 *   解法2: 相关子查询 COUNT DISTINCT — 无窗口函数时的替代方案 ⭐⭐
 */

-- ============================================================
-- 解法1: 窗口函数 DENSE_RANK()（推荐）
-- 思路：按部门分区、工资降序用 DENSE_RANK 排名，取 <= 3
-- 关键：DENSE_RANK 不跳号 → 完美匹配"前三高的不同工资"语义
--   工资 [90K, 85K, 85K, 70K, 69K]
--   DENSE_RANK: 1, 2, 2, 3, 4 → rk<=3 包含 90K, 85K, 70K 三档
-- ============================================================
SELECT Department, Employee, Salary
FROM (
    SELECT d.name AS Department,
           e.name AS Employee,
           e.salary AS Salary,
           DENSE_RANK() OVER (
               PARTITION BY e.departmentId
               ORDER BY e.salary DESC
           ) AS rk
    FROM Employee e
    JOIN Department d
      ON e.departmentId = d.id
) ranked
WHERE rk <= 3;

-- ============================================================
-- 解法2: 相关子查询 COUNT DISTINCT
-- 思路：对每个员工，统计同部门中严格高于其工资的不同工资数
--       如果 < 3，说明该员工的工资排在前三
-- 适用：不支持窗口函数的旧版 MySQL (5.x)
-- ============================================================
SELECT d.name AS Department,
       e.name AS Employee,
       e.salary AS Salary
FROM Employee e
JOIN Department d
  ON e.departmentId = d.id
WHERE (
    SELECT COUNT(DISTINCT e2.salary)
    FROM Employee e2
    WHERE e2.departmentId = e.departmentId
      AND e2.salary > e.salary
) < 3;

/*
 * 【解法对比】
 * +---------------------------+----------+-----------+------+
 * | 解法                      | 可读性   | 性能      | 推荐 |
 * +---------------------------+----------+-----------+------+
 * | DENSE_RANK 窗口函数       | 高       | O(n logn) | ⭐⭐⭐ |
 * | 相关子查询 COUNT DISTINCT | 中       | O(n*m)    | ⭐⭐   |
 * +---------------------------+----------+-----------+------+
 *
 * 【易错点】
 *   1. 用 RANK() 替代 DENSE_RANK() → RANK 跳号，Top3 可能漏人
 *      RANK:       1, 2, 2, 4, 5  → rk<=3 漏掉第三档工资
 *      DENSE_RANK: 1, 2, 2, 3, 4  → rk<=3 正确包含三档
 *   2. 相关子查询中用 >= 而非 > → 会把自己也算进去，逻辑错误
 *   3. 忘记 JOIN Department 表 → 缺少部门名称列
 *
 * 【面试追问】
 *   Q1: 如何改为 Top N (参数化)?
 *       → DENSE_RANK: WHERE rk <= N; 相关子查询: WHERE (...) < N
 *   Q2: DENSE_RANK vs RANK vs ROW_NUMBER 区别?
 *       → 数据 [100, 90, 90, 80]:
 *         DENSE_RANK: 1, 2, 2, 3
 *         RANK:       1, 2, 2, 4
 *         ROW_NUMBER: 1, 2, 3, 4
 *   Q3: 大表优化?
 *       → (departmentId, salary) 联合索引；CTE 减少重复扫描
 */
