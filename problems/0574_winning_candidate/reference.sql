-- ============================================================
-- 574. Winning Candidate
-- 难度: Medium | 标签: Database
-- ============================================================

-- ============================================================
-- 解法一: GROUP BY + ORDER BY + LIMIT (推荐)
-- 思路: 统计每位候选人的得票数，降序排列取第一名
-- 复杂度: 扫描 Vote 表一遍做聚合 + JOIN Candidate
-- ============================================================
SELECT c.name
FROM Candidate c
JOIN Vote v ON c.id = v.candidateId
GROUP BY c.id, c.name
ORDER BY COUNT(*) DESC
LIMIT 1;

-- ============================================================
-- 解法二: 子查询嵌套
-- 思路: 内层找到得票最多的 candidateId，外层查 name
-- 优点: 逻辑分层清晰，易于理解和调试
-- ============================================================
-- SELECT name
-- FROM Candidate
-- WHERE id = (
--     SELECT candidateId
--     FROM Vote
--     GROUP BY candidateId
--     ORDER BY COUNT(*) DESC
--     LIMIT 1
-- );

-- ============================================================
-- 解法三: 窗口函数 DENSE_RANK
-- 思路: 按票数排名，取 rank=1 的候选人
-- 优点: 天然支持并列第一（本题保证唯一赢家，但更通用）
-- ============================================================
-- SELECT name
-- FROM (
--     SELECT c.name,
--            DENSE_RANK() OVER (ORDER BY COUNT(*) DESC) AS rnk
--     FROM Candidate c
--     JOIN Vote v ON c.id = v.candidateId
--     GROUP BY c.id, c.name
-- ) ranked
-- WHERE rnk = 1;

-- ============================================================
-- 知识点总结:
-- 1. GROUP BY + 聚合函数: 统计分组数据的基础模式
-- 2. ORDER BY ... DESC LIMIT 1: 取最大值的简洁写法
-- 3. 子查询 vs JOIN: 子查询更清晰，JOIN 有时更高效
-- 4. DENSE_RANK: 处理并列排名的标准窗口函数
-- 5. 外键关联: Vote.candidateId -> Candidate.id
-- ============================================================
