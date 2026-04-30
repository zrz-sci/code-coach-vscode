-- LeetCode 596: Classes More Than 5 Students
-- ===========================================
-- 题意：从 Courses 表中找出至少有 5 名学生选修的课程
-- 表结构：Courses(student, class)，主键 (student, class)

-- ===========================================
-- 解法一：GROUP BY + HAVING (推荐)
-- 按 class 分组，用 HAVING 过滤学生数 >= 5 的组
-- 主键保证无重复，COUNT(*) 即为不同学生数

SELECT class
FROM Courses
GROUP BY class
HAVING COUNT(student) >= 5;


-- ===========================================
-- 解法二：子查询
-- 内层统计每门课学生数，外层过滤 >= 5
-- 语义更分层，适合需要复用统计结果的场景

-- SELECT class
-- FROM (
--     SELECT class, COUNT(*) AS num_students
--     FROM Courses
--     GROUP BY class
-- ) sub
-- WHERE num_students >= 5;


-- ===========================================
-- 解法三：窗口函数 (保留学生明细时更灵活)
-- COUNT(*) OVER 为每行标注课程的总学生数
-- 外层过滤后 DISTINCT 去重

-- SELECT DISTINCT class
-- FROM (
--     SELECT class,
--            COUNT(*) OVER (PARTITION BY class) AS cnt
--     FROM Courses
-- ) t
-- WHERE cnt >= 5;


-- ===========================================
-- 知识点总结：
--
-- 1. WHERE vs HAVING：
--    - WHERE  在 GROUP BY 之前过滤原始行
--    - HAVING 在 GROUP BY 之后过滤分组
--    - 聚合函数 (COUNT/SUM/AVG) 只能在 HAVING 中使用
--
-- 2. COUNT 的三种形式：
--    - COUNT(*)              统计所有行（含 NULL）
--    - COUNT(column)         统计非 NULL 行
--    - COUNT(DISTINCT col)   统计非 NULL 的不同值数
--    本题主键已去重，三者等价
--
-- 3. SQL 逻辑执行顺序：
--    FROM -> WHERE -> GROUP BY -> HAVING -> SELECT -> ORDER BY
