-- =============================================================================
-- 603. Consecutive Available Seats
-- =============================================================================
-- 核心思路: 空座位如果前后有相邻的空座位, 则属于连续空余座位
--          自连接判断相邻关系, 或用 LAG/LEAD 窗口函数检查前后状态
-- =============================================================================

-- =============================================================================
-- 解法一: Self JOIN 自连接 (经典, 推荐)
-- =============================================================================
-- 表自连接, 连接条件为座位 id 相邻(差值绝对值为1)
-- 两个座位都必须是空的(free=1)
-- DISTINCT 去重: 一个座位可能同时匹配前后两个邻居
SELECT DISTINCT a.seat_id
FROM Cinema a
JOIN Cinema b
  ON ABS(a.seat_id - b.seat_id) = 1
WHERE a.free = 1
  AND b.free = 1
ORDER BY a.seat_id;

-- =============================================================================
-- 解法二: LAG/LEAD 窗口函数 (现代 SQL, 更高效)
-- =============================================================================
-- LAG 取前一行的 free 值, LEAD 取后一行的 free 值
-- 当前座位空闲 且 前一个或后一个也空闲, 即为连续空余
SELECT seat_id
FROM (
    SELECT
        seat_id,
        free,
        LAG(free)  OVER (ORDER BY seat_id) AS prev_free,
        LEAD(free) OVER (ORDER BY seat_id) AS next_free
    FROM Cinema
) t
WHERE free = 1
  AND (prev_free = 1 OR next_free = 1)
ORDER BY seat_id;

-- =============================================================================
-- 解法三: 差值分组法 (适用于找连续 N 个的扩展场景)
-- =============================================================================
-- seat_id - ROW_NUMBER() 对连续空座位产生相同的差值
-- 按差值分组, 统计组内大小 >= 2 即为连续
SELECT seat_id
FROM (
    SELECT
        seat_id,
        COUNT(*) OVER (
            PARTITION BY seat_id - ROW_NUMBER() OVER (ORDER BY seat_id)
        ) AS grp_size
    FROM Cinema
    WHERE free = 1
) t
WHERE grp_size >= 2
ORDER BY seat_id;
