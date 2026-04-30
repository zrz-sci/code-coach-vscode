-- LeetCode 618: Students Report By Geography
-- 难度: Hard
-- 思路: 用 ROW_NUMBER 为每个大洲编号，再用条件聚合做行转列 (Pivot)

SELECT
    MAX(CASE WHEN continent = 'America' THEN name END) AS America,
    MAX(CASE WHEN continent = 'Asia' THEN name END) AS Asia,
    MAX(CASE WHEN continent = 'Europe' THEN name END) AS Europe
FROM (
    SELECT
        name,
        continent,
        ROW_NUMBER() OVER (PARTITION BY continent ORDER BY name) AS rn
    FROM Student
) t
GROUP BY rn;

-- 方法二：用变量模拟行号 (MySQL 5.x 兼容)
-- SELECT
--     MAX(CASE WHEN continent = 'America' THEN name END) AS America,
--     MAX(CASE WHEN continent = 'Asia' THEN name END) AS Asia,
--     MAX(CASE WHEN continent = 'Europe' THEN name END) AS Europe
-- FROM (
--     SELECT
--         name,
--         continent,
--         CASE continent
--             WHEN 'America' THEN @a := @a + 1
--             WHEN 'Asia' THEN @b := @b + 1
--             WHEN 'Europe' THEN @c := @c + 1
--         END AS rn
--     FROM Student,
--         (SELECT @a := 0, @b := 0, @c := 0) vars
--     ORDER BY name
-- ) t
-- GROUP BY rn;
