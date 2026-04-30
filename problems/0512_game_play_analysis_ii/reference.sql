/*
 * 【题目本质】
 * 找每位玩家首次登录时使用的设备。关键：需要关联非聚合列 device_id。
 *
 * 【解法总览】
 * 解法一：子查询 + IN（推荐）
 * 解法二：窗口函数 ROW_NUMBER
 * 解法三：子查询 + JOIN
 */

-- ===================== 解法一：子查询 + IN（推荐） =====================
-- 思路：先子查询找每人最小日期，再用元组 IN 匹配回原表
SELECT player_id, device_id
FROM Activity
WHERE (player_id, event_date) IN (
    SELECT player_id, MIN(event_date)
    FROM Activity
    GROUP BY player_id
);

-- ===================== 解法二：窗口函数 =====================
-- 思路：按日期排名，取第一行
-- SELECT player_id, device_id
-- FROM (
--     SELECT player_id, device_id,
--            ROW_NUMBER() OVER (PARTITION BY player_id ORDER BY event_date) AS rn
--     FROM Activity
-- ) t
-- WHERE rn = 1;

-- ===================== 解法三：子查询 + JOIN =====================
-- 思路：子查询找最小日期，JOIN 回原表取 device_id
-- SELECT a.player_id, a.device_id
-- FROM Activity a
-- JOIN (
--     SELECT player_id, MIN(event_date) AS min_date
--     FROM Activity
--     GROUP BY player_id
-- ) b ON a.player_id = b.player_id AND a.event_date = b.min_date;

/*
 * 【解法对比】
 * | 解法 | 优点 | 缺点 |
 * |------|------|------|
 * | 子查询+IN | 简洁直观 | 元组IN部分数据库支持差 |
 * | 窗口函数 | 通用，可扩展 | 需要子查询包装 |
 * | 子查询+JOIN | 性能好 | 代码稍长 |
 *
 * 【易错点】
 * 1. 不能直接 GROUP BY player_id 然后 SELECT device_id（非聚合列）
 * 2. 窗口函数结果需要外层 WHERE 过滤
 * 3. JOIN 条件需同时匹配 player_id 和 event_date
 *
 * 【面试追问】
 * Q1: 同一天多条记录怎么办？→ ROW_NUMBER 保证唯一
 * Q2: 如何找最后一次登录的设备？→ MIN→MAX 或 ORDER BY DESC
 * Q3: 大数据量优化？→ (player_id, event_date) 联合索引
 */
