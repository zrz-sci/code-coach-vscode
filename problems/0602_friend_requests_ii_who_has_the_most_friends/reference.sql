-- =============================================================================
-- 602. Friend Requests II: Who Has the Most Friends
-- =============================================================================
-- 核心思路: 友谊是双向关系, requester 和 accepter 各获得一位好友
--          UNION ALL 展开后 GROUP BY 聚合计数, 取最大值
-- =============================================================================

-- =============================================================================
-- 解法一: UNION ALL + GROUP BY + LIMIT (推荐, 最简洁)
-- =============================================================================
-- 将 requester_id 和 accepter_id 各自展开为独立行
-- 聚合计数后按好友数降序取第一行
SELECT id, COUNT(*) AS num
FROM (
    SELECT requester_id AS id FROM RequestAccepted
    UNION ALL
    SELECT accepter_id AS id FROM RequestAccepted
) friends
GROUP BY id
ORDER BY num DESC
LIMIT 1;

-- =============================================================================
-- 解法二: RANK() 窗口函数 (支持 Follow-up: 多人并列)
-- =============================================================================
-- 使用 RANK 窗口函数排名, 取排名第一的所有人
-- 适用于多人好友数相同且最多的情况
SELECT id, num
FROM (
    SELECT
        id,
        COUNT(*) AS num,
        RANK() OVER (ORDER BY COUNT(*) DESC) AS rk
    FROM (
        SELECT requester_id AS id FROM RequestAccepted
        UNION ALL
        SELECT accepter_id AS id FROM RequestAccepted
    ) friends
    GROUP BY id
) ranked
WHERE rk = 1;

-- =============================================================================
-- 解法三: 分别统计再合并 (思路清晰但较冗长)
-- =============================================================================
-- 分别统计作为 requester 和 accepter 的好友数, 再用 COALESCE 合并
SELECT id, SUM(cnt) AS num
FROM (
    SELECT requester_id AS id, COUNT(*) AS cnt
    FROM RequestAccepted
    GROUP BY requester_id
    UNION ALL
    SELECT accepter_id AS id, COUNT(*) AS cnt
    FROM RequestAccepted
    GROUP BY accepter_id
) combined
GROUP BY id
ORDER BY num DESC
LIMIT 1;
