-- ============================================================
-- 608. Tree Node (树节点)
-- 难度: Medium | 标签: Database
-- ============================================================

-- ============================================================
-- 解法一: CASE + IN 子查询 (推荐, 最简洁)
-- 思路: 用 CASE WHEN 按优先级判断节点类型
--   1) p_id IS NULL -> Root
--   2) id 出现在 p_id 列中 -> Inner (有子节点)
--   3) 其余 -> Leaf
-- 时间复杂度: O(n^2)  空间: O(n)
-- ============================================================
SELECT
    id,
    CASE
        WHEN p_id IS NULL THEN 'Root'
        WHEN id IN (SELECT p_id FROM Tree) THEN 'Inner'
        ELSE 'Leaf'
    END AS type
FROM Tree;

-- ============================================================
-- 解法二: CASE + LEFT JOIN (避免子查询, 性能更优)
-- 思路: LEFT JOIN 去重后的父节点集合
--   - 能匹配上 -> 当前节点是别人的父节点 -> Inner
--   - 匹配不上 -> 没有子节点 -> Leaf
-- 时间复杂度: O(n log n)  空间: O(n)
-- ============================================================
SELECT
    t1.id,
    CASE
        WHEN t1.p_id IS NULL THEN 'Root'
        WHEN t2.p_id IS NOT NULL THEN 'Inner'
        ELSE 'Leaf'
    END AS type
FROM Tree t1
LEFT JOIN (
    SELECT DISTINCT p_id
    FROM Tree
    WHERE p_id IS NOT NULL
) t2 ON t1.id = t2.p_id;

-- ============================================================
-- 解法三: LEFT JOIN + GROUP BY (单次扫描思路)
-- 思路: 自连接 Tree, 用 COUNT 判断是否存在子节点
--   - GROUP BY 消除因多子节点产生的重复行
-- 时间复杂度: O(n log n)  空间: O(n)
-- ============================================================
SELECT
    t1.id,
    CASE
        WHEN t1.p_id IS NULL THEN 'Root'
        WHEN COUNT(t2.id) > 0 THEN 'Inner'
        ELSE 'Leaf'
    END AS type
FROM Tree t1
LEFT JOIN Tree t2 ON t1.id = t2.p_id
GROUP BY t1.id, t1.p_id;

-- ============================================================
-- 解法四: UNION ALL 分治
-- 思路: 分别查出三种类型, 再合并
-- 注意: NOT IN 子查询中必须排除 NULL, 否则结果集为空
-- ============================================================
SELECT id, 'Root' AS type
FROM Tree
WHERE p_id IS NULL

UNION ALL

SELECT id, 'Inner' AS type
FROM Tree
WHERE p_id IS NOT NULL
  AND id IN (SELECT p_id FROM Tree)

UNION ALL

SELECT id, 'Leaf' AS type
FROM Tree
WHERE p_id IS NOT NULL
  AND id NOT IN (
      SELECT p_id FROM Tree WHERE p_id IS NOT NULL
  );
