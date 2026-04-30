-- =====================================================
-- LeetCode 181: Employees Earning More Than Their Managers
-- (超过经理收入的员工)
-- 难度: Easy | 标签: Database
-- =====================================================
--
-- 【题目本质】
-- 自连接经典题：同一张表中员工与经理通过managerId关联，
-- 找出工资严格高于直属经理的员工姓名。
--
-- 【解法总览】
-- 解法1: 自连接 JOIN          ← 推荐，最直观
-- 解法2: WHERE关联子查询      ← 传统子查询方式
-- 解法3: EXISTS子查询         ← 语义更清晰
-- =====================================================

-- 解法1: 自连接 JOIN（推荐）
-- 思路：表自连接，e是员工、m是经理
-- 连接条件：员工的managerId = 经理的id
-- 过滤条件：员工salary > 经理salary
SELECT e.name AS Employee
FROM Employee e
JOIN Employee m ON e.managerId = m.id   -- 员工的managerId指向经理的id
WHERE e.salary > m.salary;              -- 员工工资严格高于经理


-- 解法2: WHERE关联子查询
-- 思路：对每个员工，用子查询找到其经理的薪水并比较
-- 注意：先排除managerId为NULL的员工，避免子查询返回空
SELECT e.name AS Employee
FROM Employee e
WHERE e.managerId IS NOT NULL
  AND e.salary > (
      SELECT m.salary
      FROM Employee m
      WHERE m.id = e.managerId          -- 子查询找到经理的薪水
  );


-- 解法3: EXISTS子查询
-- 思路：检查是否存在一个"比当前员工薪水低的经理"
SELECT e.name AS Employee
FROM Employee e
WHERE EXISTS (
    SELECT 1
    FROM Employee m
    WHERE m.id = e.managerId            -- 找到经理
      AND e.salary > m.salary           -- 且员工薪水更高
);


-- =====================================================
-- 【解法对比】
-- | 解法          | 时间   | 可读性 | 面试推荐 |
-- |---------------|-------|--------|---------|
-- | 自连接 JOIN   | O(n)  | 高     | 首选    |
-- | WHERE子查询   | O(n)* | 中     | 备选    |
-- | EXISTS子查询  | O(n)  | 中     | 备选    |
-- * WHERE子查询在有索引时O(n)，无索引时可能O(n^2)
--
-- 【易错点】
-- 1. JOIN方向写反：e.id = m.managerId → 应为 e.managerId = m.id
-- 2. 用LEFT JOIN → 会保留managerId=NULL的行，需额外过滤
-- 3. salary用>=而非> → 题目要求"超过"(strictly greater)
-- 4. 不指定表别名 → 自连接时name列歧义
--
-- 【面试追问】
-- Q1: 为什么用JOIN不用LEFT JOIN？→ 内连接自动排除无经理(managerId=NULL)的行
-- Q2: 多层经理关系怎么处理？→ 用WITH RECURSIVE递归CTE遍历管理链
-- Q3: 性能优化？→ 在id和managerId列建索引，JOIN走索引查找O(1)
-- =====================================================
