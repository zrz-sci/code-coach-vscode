-- ============================================================
-- 577. Employee Bonus
-- 难度: Easy | 标签: Database
-- ============================================================

-- ============================================================
-- 解法一: LEFT JOIN + OR (推荐)
-- 思路: LEFT JOIN 保留所有员工，用 WHERE 过滤两种情况
--       1) bonus < 1000 (有奖金但少于 1000)
--       2) bonus IS NULL (无奖金记录)
-- 关键: 必须用 LEFT JOIN 而非 INNER JOIN，否则丢掉无奖金员工
-- ============================================================
SELECT e.name, b.bonus
FROM Employee e
LEFT JOIN Bonus b ON e.empId = b.empId
WHERE b.bonus < 1000 OR b.bonus IS NULL;

-- ============================================================
-- 解法二: IFNULL 合并条件
-- 思路: 将 NULL 替换为 0，统一用 < 1000 过滤
-- 优点: 条件更简洁，只需一个比较
-- 注意: IFNULL 是 MySQL 特有，标准 SQL 用 COALESCE
-- ============================================================
-- SELECT e.name, b.bonus
-- FROM Employee e
-- LEFT JOIN Bonus b ON e.empId = b.empId
-- WHERE IFNULL(b.bonus, 0) < 1000;

-- ============================================================
-- 解法三: COALESCE (标准 SQL，跨数据库兼容)
-- 思路: COALESCE 返回第一个非 NULL 值
-- 优点: 可移植到 PostgreSQL, SQL Server, Oracle 等
-- ============================================================
-- SELECT e.name, b.bonus
-- FROM Employee e
-- LEFT JOIN Bonus b ON e.empId = b.empId
-- WHERE COALESCE(b.bonus, 0) < 1000;

-- ============================================================
-- 解法四: NOT IN 反向排除
-- 思路: 排除所有奖金 >= 1000 的员工
-- 注意: NOT IN 遇到 NULL 可能有意外行为，此处 Bonus.empId 不为 NULL 故安全
-- ============================================================
-- SELECT e.name, b.bonus
-- FROM Employee e
-- LEFT JOIN Bonus b ON e.empId = b.empId
-- WHERE e.empId NOT IN (
--     SELECT empId FROM Bonus WHERE bonus >= 1000
-- );

-- ============================================================
-- 知识点总结:
-- 1. LEFT JOIN: 保留左表所有行，右表无匹配时补 NULL
-- 2. NULL 比较: NULL < 1000 => UNKNOWN，必须用 IS NULL 显式判断
-- 3. IFNULL(x, default): MySQL 特有，将 NULL 替换为 default
-- 4. COALESCE(a, b, ...): SQL 标准，返回第一个非 NULL 参数
-- 5. NOT IN 陷阱: 子查询含 NULL 时 NOT IN 全部返回 UNKNOWN
-- ============================================================
