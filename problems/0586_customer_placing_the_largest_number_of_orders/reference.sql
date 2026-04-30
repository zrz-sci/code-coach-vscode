-- ============================================================
-- 586. Customer Placing the Largest Number of Orders
-- ============================================================

-- 解法一：ORDER BY + LIMIT（题目保证唯一最大值）
-- 按客户分组统计订单数，降序取第一个
SELECT customer_number
FROM Orders
GROUP BY customer_number
ORDER BY COUNT(*) DESC
LIMIT 1;

-- ============================================================
-- 解法二：子查询 + HAVING（Follow-up: 处理并列最大值）
-- 先找出最大订单数，再筛选所有匹配的客户
-- ============================================================
-- SELECT customer_number
-- FROM Orders
-- GROUP BY customer_number
-- HAVING COUNT(*) = (
--     SELECT COUNT(*) AS cnt
--     FROM Orders
--     GROUP BY customer_number
--     ORDER BY cnt DESC
--     LIMIT 1
-- );

-- ============================================================
-- 解法三：窗口函数 RANK（Follow-up: 优雅处理并列）
-- RANK() 给并列记录相同排名，适合 Top-K 问题
-- ============================================================
-- SELECT customer_number
-- FROM (
--     SELECT customer_number,
--            RANK() OVER (ORDER BY COUNT(*) DESC) AS rnk
--     FROM Orders
--     GROUP BY customer_number
-- ) ranked
-- WHERE rnk = 1;

-- ============================================================
-- 解法四：ALL 子查询（无需 ORDER BY）
-- 使用 >= ALL 比较每组计数与所有其他组的计数
-- ============================================================
-- SELECT customer_number
-- FROM Orders
-- GROUP BY customer_number
-- HAVING COUNT(*) >= ALL (
--     SELECT COUNT(*)
--     FROM Orders
--     GROUP BY customer_number
-- );
