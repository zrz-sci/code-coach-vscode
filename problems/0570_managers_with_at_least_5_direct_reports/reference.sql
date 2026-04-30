-- =============================================
-- LeetCode 570: Managers with at Least 5 Direct Reports
-- =============================================

-- 【题目本质】
-- 自连接或分组聚合: 按 managerId 分组统计下属数量 >= 5，
-- 然后回查经理姓名。经典的 GROUP BY + HAVING 场景。

-- 【解法总览】
-- 1. 子查询 + JOIN (推荐)
-- 2. 子查询 IN
-- 3. 直接 JOIN + GROUP BY

-- =============================================
-- 解法一: 子查询 + JOIN（推荐）
-- 思路: 先子查询找出下属>=5的 managerId，再 JOIN 取 name
-- =============================================
SELECT e.name
FROM Employee e
JOIN (
    SELECT managerId
    FROM Employee
    WHERE managerId IS NOT NULL
    GROUP BY managerId
    HAVING COUNT(*) >= 5
) m ON e.id = m.managerId;

-- =============================================
-- 解法二: 子查询 IN
-- 思路: 用 IN 子句替代 JOIN，更简洁
-- =============================================
SELECT name
FROM Employee
WHERE id IN (
    SELECT managerId
    FROM Employee
    GROUP BY managerId
    HAVING COUNT(*) >= 5
);

-- =============================================
-- 解法三: 直接 JOIN + GROUP BY
-- 思路: 自连接后直接分组
-- =============================================
SELECT e1.name
FROM Employee e1
JOIN Employee e2 ON e1.id = e2.managerId
GROUP BY e1.id, e1.name
HAVING COUNT(*) >= 5;

-- 【解法对比】
-- | 解法            | 优点              | 缺点                |
-- |----------------|------------------|---------------------|
-- | 子查询 + JOIN   | 逻辑清晰，性能好   | 稍冗长               |
-- | 子查询 IN       | 最简洁            | 大数据量 IN 可能较慢   |
-- | 直接 JOIN       | 一步完成          | GROUP BY 需包含 id    |

-- 【易错点】
-- 1. GROUP BY name 可能合并同名经理 → 应 GROUP BY id (或 id, name)
-- 2. managerId 为 NULL 的行不影响结果，但过滤掉是好习惯
-- 3. HAVING COUNT(*) >= 5，不是 > 5

-- 【面试追问】
-- Q1: 找间接下属>=5的经理？→ 用递归 CTE 展开层级后聚合
-- Q2: 同时返回下属数量？→ SELECT name, COUNT(*) ... GROUP BY
-- Q3: 性能优化？→ 对 managerId 建索引
