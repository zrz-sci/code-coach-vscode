/*
 * 【题目本质】
 * 找每位玩家的首次登录日期，本质是 GROUP BY + 聚合函数的基础应用。
 *
 * 【解法总览】
 * 解法一：GROUP BY + MIN（推荐）
 * 解法二：窗口函数 RANK
 * 解法三：子查询关联
 */

-- ===================== 解法一：GROUP BY + MIN（推荐） =====================
-- 思路：按玩家分组，取最小日期即为首次登录
SELECT player_id, MIN(event_date) AS first_login
FROM Activity
GROUP BY player_id;

-- ===================== 解法二：窗口函数 =====================
-- 思路：按日期排名，取排名第一的行
-- SELECT player_id, event_date AS first_login
-- FROM (
--     SELECT player_id, event_date,
--            RANK() OVER (PARTITION BY player_id ORDER BY event_date) AS rk
--     FROM Activity
-- ) t
-- WHERE rk = 1;

-- ===================== 解法三：子查询 =====================
-- 思路：先子查询找出每人最小日期，再关联原表
-- SELECT player_id, event_date AS first_login
-- FROM Activity
-- WHERE (player_id, event_date) IN (
--     SELECT player_id, MIN(event_date)
--     FROM Activity
--     GROUP BY player_id
-- );

/*
 * 【解法对比】
 * | 解法 | 复杂度 | 优点 | 缺点 |
 * |------|--------|------|------|
 * | GROUP BY+MIN | 最优 | 简洁高效 | 只能取聚合列 |
 * | 窗口函数 | 中等 | 可取非聚合列 | 需要子查询 |
 * | 子查询 | 中等 | 逻辑清晰 | 扫描两次表 |
 *
 * 【易错点】
 * 1. 别名必须是 first_login，不是 event_date
 * 2. GROUP BY + MIN 只能获取聚合列，无法同时获取 device_id
 * 3. 不需要 ORDER BY（题目不要求排序）
 *
 * 【面试追问】
 * Q1: 如何同时获取首次登录的设备？→ 窗口函数或子查询（见 LC 512）
 * Q2: 大数据量如何优化？→ 在 (player_id, event_date) 建联合索引
 * Q3: RANK vs ROW_NUMBER vs DENSE_RANK 的区别？→ 并列时行为不同
 */
