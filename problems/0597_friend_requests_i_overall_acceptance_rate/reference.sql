-- ============================================================
-- LeetCode 597: 好友申请 I：总体通过率
-- ============================================================

-- 【题目本质】
-- 两表独立去重计数后做除法，处理分母为零的边界情况。

-- 【解法总览】
-- 解法一：子查询除法（推荐）
-- 解法二：CTE 写法

-- ============================================================
-- 解法一：子查询除法（推荐）
-- 思路：两个独立 COUNT(DISTINCT) 子查询直接相除
-- ============================================================
SELECT ROUND(
    IFNULL(
        (SELECT COUNT(DISTINCT requester_id, accepter_id) FROM RequestAccepted)
        /
        (SELECT COUNT(DISTINCT sender_id, send_to_id) FROM FriendRequest),
        0
    ), 2
) AS accept_rate;

-- ============================================================
-- 解法二：CTE 写法
-- 思路：用 CTE 分别计算请求数和接受数，再做除法
-- ============================================================
-- WITH req AS (
--     SELECT COUNT(DISTINCT sender_id, send_to_id) AS cnt FROM FriendRequest
-- ),
-- acc AS (
--     SELECT COUNT(DISTINCT requester_id, accepter_id) AS cnt FROM RequestAccepted
-- )
-- SELECT ROUND(IFNULL(acc.cnt / NULLIF(req.cnt, 0), 0), 2) AS accept_rate
-- FROM req, acc;

-- 【解法对比】
-- | 解法         | 可读性 | 性能  | 适用场景           |
-- |-------------|--------|------|--------------------|
-- | 子查询除法   | ⭐⭐    | 相同 | 简洁场景           |
-- | CTE 写法     | ⭐⭐⭐  | 相同 | 需要复用中间结果时  |

-- 【易错点】
-- 1. 去重必须按 (sender_id, send_to_id) 对去重，不是单列
-- 2. 分母为 0 时除法返回 NULL，必须用 IFNULL 包裹
-- 3. 两表无 JOIN 关系，必须独立计算

-- 【面试追问】
-- Q1: 每月通过率 → 按 DATE_FORMAT 分组 + LEFT JOIN
-- Q2: 累积通过率 → 窗口函数 SUM() OVER(ORDER BY date)
-- Q3: 数据量大 → 预聚合 + 索引 (sender_id, send_to_id)
