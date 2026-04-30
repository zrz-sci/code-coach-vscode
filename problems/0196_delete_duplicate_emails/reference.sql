-- ============================================================
-- LeetCode 196: Delete Duplicate Emails (删除重复的电子邮箱)
-- 难度: Easy | 标签: Database
--
-- 【题目本质】
-- 删除 Person 表中 email 重复的行，每个 email 只保留 id 最小的一行
-- 关键：必须写 DELETE，不是 SELECT
--
-- 【解法总览】
-- 解法1: 自连接 DELETE       — 最直观，面试首选
-- 解法2: 子查询 NOT IN       — 思路清晰，注意 MySQL 限制
-- 解法3: 窗口函数 ROW_NUMBER — MySQL 8.0+，通用性强
-- ============================================================

-- ==================== 解法1: 自连接 DELETE（推荐） ====================
-- 思路：Person 表自连接自身
--   连接条件：email 相同
--   删除条件：id 较大的那行
-- 时间复杂度：O(n^2)，需要两两比较
DELETE p1
FROM Person p1, Person p2
WHERE p1.email = p2.email
  AND p1.id > p2.id;

-- 等价的 JOIN 写法：
-- DELETE p1
-- FROM Person p1
-- JOIN Person p2
--   ON p1.email = p2.email AND p1.id > p2.id;

-- ==================== 解法2: 子查询 NOT IN ====================
-- 思路：先找出每个 email 的最小 id，不在该集合中的行全部删除
-- 注意：MySQL 不允许在 DELETE 子查询中直接引用被修改的表
--       必须嵌套一层临时表绕过限制
-- DELETE FROM Person
-- WHERE id NOT IN (
--     SELECT id FROM (
--         SELECT MIN(id) AS id
--         FROM Person
--         GROUP BY email
--     ) AS tmp
-- );

-- ==================== 解法3: 窗口函数（MySQL 8.0+） ====================
-- 思路：用 ROW_NUMBER() 对每个 email 按 id 排序编号
--       删除编号 > 1 的行（即非最小 id 的重复行）
-- DELETE FROM Person
-- WHERE id IN (
--     SELECT id FROM (
--         SELECT id,
--                ROW_NUMBER() OVER (PARTITION BY email ORDER BY id) AS rn
--         FROM Person
--     ) AS ranked
--     WHERE rn > 1
-- );

-- ============================================================
-- 【解法对比】
-- | 解法     | 简洁度 | MySQL版本要求 | 性能   | 面试推荐 |
-- |---------|--------|-------------|--------|---------|
-- | 自连接   | ★★★    | 所有版本     | 中等    | ★★★     |
-- | 子查询   | ★★     | 所有版本     | 中等    | ★★      |
-- | 窗口函数 | ★★     | 8.0+        | 较好    | ★★      |
--
-- 【易错点】
-- 1. 写成 SELECT 而非 DELETE（题目要求修改表）
-- 2. DELETE p1, p2 误删两边 → 应只 DELETE p1
-- 3. MySQL 子查询直接引用被修改表报错 → 需嵌套临时表
-- 4. 忘记 GROUP BY email 导致只保留全局最小 id
--
-- 【面试追问】
-- Q1: 保留最大 id？ → 条件改为 p1.id < p2.id
-- Q2: MySQL 子查询限制的原因？ → 同一语句不能同时修改和读取同一表
-- Q3: 百万级数据优化？ → 先 SELECT 出要保留的 id 到临时表，再 DELETE
-- ============================================================
