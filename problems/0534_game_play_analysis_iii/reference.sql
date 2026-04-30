/*
 * 【题目本质】
 * 对每个玩家按日期计算累计游戏数（前缀和 / running sum）
 * 典型的分组窗口函数应用场景
 *
 * 【解法总览】
 * 解法一：窗口函数 SUM() OVER() ⭐推荐
 * 解法二：自连接累加
 * 解法三：相关子查询
 */

-- ===================== 解法一：窗口函数（推荐） =====================
-- 思路：PARTITION BY 按玩家分组，ORDER BY 按日期排序，SUM 自动累加
SELECT
    player_id,
    event_date,
    SUM(games_played) OVER (
        PARTITION BY player_id
        ORDER BY event_date
        -- 默认: ROWS BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW
    ) AS games_played_so_far
FROM Activity;

-- ===================== 解法二：自连接累加 =====================
-- 思路：a1为当前行，a2为同一玩家且日期<=当前日期的所有行
--       GROUP BY 后 SUM(a2.games_played) 就是累计值
SELECT
    a1.player_id,
    a1.event_date,
    SUM(a2.games_played) AS games_played_so_far
FROM Activity a1
JOIN Activity a2
    ON a1.player_id = a2.player_id
    AND a2.event_date <= a1.event_date
GROUP BY a1.player_id, a1.event_date;

-- ===================== 解法三：相关子查询 =====================
-- 思路：对每一行，用子查询计算该玩家截至当前日期的总和
SELECT
    a1.player_id,
    a1.event_date,
    (SELECT SUM(a2.games_played)
     FROM Activity a2
     WHERE a2.player_id = a1.player_id
       AND a2.event_date <= a1.event_date
    ) AS games_played_so_far
FROM Activity a1;

/*
 * 【解法对比】
 * | 解法 | 性能 | 兼容性 | 优势 |
 * |------|------|--------|------|
 * | 窗口函数 | O(n log n) | MySQL 8.0+ | 最简洁高效 |
 * | 自连接 | O(n^2) | 所有版本 | 兼容性好 |
 * | 相关子查询 | O(n^2) | 所有版本 | 语义直观 |
 *
 * 【易错点】
 * 1. 窗口函数必须 PARTITION BY player_id，否则所有玩家混在一起
 * 2. 自连接条件是 <=（包含当天），不是 <
 * 3. 自连接必须 GROUP BY a1.player_id 和 a1.event_date
 * 4. 窗口函数的 ORDER BY 必须是 event_date，保证累加顺序
 *
 * 【面试追问】
 * Q1: 窗口函数 vs 自连接性能差异？
 *     → 窗口函数 O(n log n)，自连接 O(n^2)
 * Q2: 如何改成移动平均（最近3天）？
 *     → ROWS BETWEEN 2 PRECEDING AND CURRENT ROW
 * Q3: 如果同一天有多条记录？
 *     → 先子查询按(player_id, event_date)聚合，再外层累计
 */
