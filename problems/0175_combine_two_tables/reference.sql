-- 【题目本质】
-- 将两个表按 personId 关联，即使某人没有地址也要显示 → LEFT JOIN 的经典场景。
--
-- 【解法总览】
-- 解法1: LEFT JOIN — 直接、高效、面试首选
-- 解法2: 子查询 — 功能等价但性能差

-- ===================== 解法1: LEFT JOIN =====================
-- 思路：以 Person 为主表做左连接，保证所有 Person 都出现
-- 为什么不用 INNER JOIN？因为没有地址的人会被过滤掉，不满足"report null"的要求
SELECT p.firstName, p.lastName, a.city, a.state
FROM Person p
LEFT JOIN Address a ON p.personId = a.personId;

-- ===================== 解法2: 子查询 =====================
-- 思路：在 SELECT 中用相关子查询获取每个人的地址信息
-- 缺点：每行执行两次子查询，性能差于 JOIN
-- SELECT
--     p.firstName,
--     p.lastName,
--     (SELECT a.city FROM Address a WHERE a.personId = p.personId LIMIT 1) AS city,
--     (SELECT a.state FROM Address a WHERE a.personId = p.personId LIMIT 1) AS state
-- FROM Person p;

-- 【解法对比】
-- | 解法 | 思路 | 性能 | 面试 |
-- |------|------|------|------|
-- | LEFT JOIN | 直接连接两表 | 高效 | 必知 |
-- | 子查询 | SELECT 中嵌套查询 | 较差 | 了解 |
--
-- 【易错点】
-- 1. 用 INNER JOIN 而非 LEFT JOIN → 丢失无地址的人
-- 2. ON 条件写成 p.personId = a.addressId → 关联字段错误
-- 3. SELECT * → 输出多余列（应只选 firstName, lastName, city, state）
--
-- 【面试追问】
-- Q1: LEFT JOIN 中右表无匹配时发生什么？→ 右表所有字段填 NULL
-- Q2: 如果要找没有地址的人？→ LEFT JOIN + WHERE a.personId IS NULL
-- Q3: 如果一个人有多个地址只取最新？→ 子查询按时间排序取 LIMIT 1
