-- =============================================================================
-- 607. Sales Person
-- =============================================================================
-- 核心思路: 否定查询 -- 找出没有向 "RED" 公司销售过的销售员
--          先确定 RED 公司关联的 sales_id, 再从全体销售员中排除
-- =============================================================================

-- =============================================================================
-- 解法一: NOT IN 子查询 (推荐, 最简洁直观)
-- =============================================================================
-- 内层: Orders JOIN Company 找出所有向 RED 销售过的 sales_id
-- 外层: 排除这些 sales_id
SELECT name
FROM SalesPerson
WHERE sales_id NOT IN (
    SELECT o.sales_id
    FROM Orders o
    JOIN Company c ON o.com_id = c.com_id
    WHERE c.name = 'RED'
);

-- =============================================================================
-- 解法二: NOT EXISTS 相关子查询 (对 NULL 更安全)
-- =============================================================================
-- 对每个销售员检查是否存在 RED 公司的订单
-- NOT EXISTS 遇到匹配行即短路, 大数据集下通常更高效
SELECT sp.name
FROM SalesPerson sp
WHERE NOT EXISTS (
    SELECT 1
    FROM Orders o
    JOIN Company c ON o.com_id = c.com_id
    WHERE c.name = 'RED'
      AND o.sales_id = sp.sales_id
);

-- =============================================================================
-- 解法三: LEFT JOIN + IS NULL (等价否定查询)
-- =============================================================================
-- 先构建 RED 公司关联的 sales_id 子表
-- LEFT JOIN 后筛选无匹配记录 (IS NULL)
SELECT sp.name
FROM SalesPerson sp
LEFT JOIN (
    SELECT DISTINCT o.sales_id
    FROM Orders o
    JOIN Company c ON o.com_id = c.com_id
    WHERE c.name = 'RED'
) red_sales ON sp.sales_id = red_sales.sales_id
WHERE red_sales.sales_id IS NULL;

-- =============================================================================
-- 解法四: 标量子查询 (先查 RED 的 com_id, 再排除)
-- =============================================================================
-- 适用于确定 RED 只有一个 com_id 的情况
-- 减少一次 JOIN, 用标量子查询直接获取 com_id
SELECT name
FROM SalesPerson
WHERE sales_id NOT IN (
    SELECT sales_id
    FROM Orders
    WHERE com_id = (SELECT com_id FROM Company WHERE name = 'RED')
);
