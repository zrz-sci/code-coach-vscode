/*
 * 【题目本质】
 * 计算"首次登录后次日再次登录的玩家比例"（次日留存率）
 * 步骤：1) 找每个玩家首次登录日期  2) 检查次日是否有记录  3) 算比例
 *
 * 【解法总览】
 * 解法一：子查询 + LEFT JOIN ⭐推荐
 * 解法二：子查询 + IN/EXISTS
 * 解法三：窗口函数（MySQL 8.0+）
 */

-- ===================== 解法一：子查询 + LEFT JOIN（推荐） =====================
-- 思路：
-- 1. 子查询求每个玩家的首次登录日期
-- 2. LEFT JOIN 原表，匹配 player_id 且 event_date = first_login + 1
-- 3. COUNT(非NULL的匹配行) / 总玩家数 = 留存率
SELECT
    ROUND(
        COUNT(a.player_id)
        / (SELECT COUNT(DISTINCT player_id) FROM Activity),
        2
    ) AS fraction
FROM (
    SELECT player_id, MIN(event_date) AS first_login
    FROM Activity
    GROUP BY player_id
) f
LEFT JOIN Activity a
    ON f.player_id = a.player_id
    AND a.event_date = DATE_ADD(f.first_login, INTERVAL 1 DAY);

-- ===================== 解法二：子查询 + EXISTS =====================
-- 思路：对每个玩家的首次登录记录，用 EXISTS 检查次日是否有记录
SELECT
    ROUND(
        SUM(
            CASE WHEN EXISTS (
                SELECT 1 FROM Activity a2
                WHERE a2.player_id = t.player_id
                  AND a2.event_date = DATE_ADD(t.first_login, INTERVAL 1 DAY)
            ) THEN 1 ELSE 0 END
        ) / COUNT(*),
        2
    ) AS fraction
FROM (
    SELECT player_id, MIN(event_date) AS first_login
    FROM Activity
    GROUP BY player_id
) t;

-- ===================== 解法三：窗口函数（MySQL 8.0+） =====================
-- 思路：用 MIN() OVER() 为每行标注首次登录日期，
--       然后在外层筛选 event_date = first_date + 1 的记录
WITH first_login AS (
    SELECT
        player_id,
        event_date,
        MIN(event_date) OVER (PARTITION BY player_id) AS first_date
    FROM Activity
)
SELECT
    ROUND(
        COUNT(DISTINCT CASE
            WHEN event_date = DATE_ADD(first_date, INTERVAL 1 DAY)
            THEN player_id END)
        / COUNT(DISTINCT player_id),
        2
    ) AS fraction
FROM first_login;

/*
 * 【解法对比】
 * | 解法 | 性能 | 兼容性 | 优势 |
 * |------|------|--------|------|
 * | LEFT JOIN | O(n log n) | 所有版本 | ⭐ 最直观高效 |
 * | EXISTS | O(n * m) | 所有版本 | 语义清晰 |
 * | 窗口函数 | O(n log n) | MySQL 8.0+ | 无需子查询 |
 *
 * 【易错点】
 * 1. 分母用 COUNT(*) 而非 COUNT(DISTINCT player_id)
 *    → LEFT JOIN 后行数 = 玩家数（每个玩家一行），可以用 COUNT(*)
 *    → 但直接从 Activity 表算分母要用 DISTINCT
 * 2. LEFT JOIN 后用 COUNT(*) 统计匹配数
 *    → 应该用 COUNT(a.player_id)，因为未匹配行 a.player_id 为 NULL
 * 3. 日期加 1 写成 first_login + 1
 *    → MySQL 中应用 DATE_ADD(first_login, INTERVAL 1 DAY)
 * 4. 忘记 ROUND(..., 2)
 *
 * 【面试追问】
 * Q1: LEFT JOIN vs INNER JOIN？
 *     → LEFT JOIN 保留所有玩家行（不论是否次日登录），
 *       用 COUNT(a.player_id) 只计匹配行。
 *       INNER JOIN 只保留有次日登录的，分母需另算。
 * Q2: 如何改成第 N 天留存率？
 *     → INTERVAL 1 DAY 改成 INTERVAL N DAY
 * Q3: 大表优化？
 *     → (player_id, event_date) 上建索引，GROUP BY + MIN 可利用索引
 */
