/*
 * LeetCode 183. 从不订购的客户 (Easy)
 *
 * 【题目本质】
 *   反连接 (Anti-Join) —— 从 Customers 表中找出不存在于 Orders 表的客户。
 *   核心考点：LEFT JOIN + IS NULL / NOT IN / NOT EXISTS 三种反连接写法。
 *
 * 【解法总览】
 *   解法1: LEFT JOIN + IS NULL  — 性能最优，推荐 ⭐⭐⭐
 *   解法2: NOT IN 子查询        — 简洁但有 NULL 陷阱 ⭐⭐
 *   解法3: NOT EXISTS 子查询    — 语义清晰，NULL 安全 ⭐⭐⭐
 */

-- ============================================================
-- 解法1: LEFT JOIN + IS NULL（推荐）
-- 思路：左连接后，右表为 NULL 的行即为"不存在"的客户
-- ============================================================
SELECT c.name AS Customers
FROM Customers c
LEFT JOIN Orders o
  ON c.id = o.customerId
WHERE o.id IS NULL;

-- ============================================================
-- 解法2: NOT IN 子查询
-- 思路：找出所有下过单的 customerId，排除它们
-- 注意：如果 customerId 可能为 NULL，结果会为空集！
-- ============================================================
SELECT name AS Customers
FROM Customers
WHERE id NOT IN (
    SELECT customerId
    FROM Orders
);

-- ============================================================
-- 解法3: NOT EXISTS 子查询
-- 思路：对每个客户，检查是否存在对应订单
-- 优点：NULL 安全，语义清晰
-- ============================================================
SELECT name AS Customers
FROM Customers c
WHERE NOT EXISTS (
    SELECT 1
    FROM Orders o
    WHERE o.customerId = c.id
);

/*
 * 【解法对比】
 * +---------------------+----------+------------+------+
 * | 解法                | 可读性   | NULL 安全  | 推荐 |
 * +---------------------+----------+------------+------+
 * | LEFT JOIN + IS NULL | 高       | 是         | ⭐⭐⭐ |
 * | NOT IN              | 高       | 否(有陷阱) | ⭐⭐   |
 * | NOT EXISTS          | 中       | 是         | ⭐⭐⭐ |
 * +---------------------+----------+------------+------+
 *
 * 【易错点】
 *   1. NOT IN 的 NULL 陷阱：子查询若返回 NULL，整个 NOT IN 结果为空
 *   2. LEFT JOIN 后判 NULL 应选不可能为 NULL 的列（如主键 o.id）
 *   3. 输出列别名必须是 "Customers"，不是 "name"
 *
 * 【面试追问】
 *   Q1: LEFT JOIN vs NOT EXISTS 性能差异？
 *       → 现代优化器下两者执行计划基本一致(Semi Anti Join)
 *   Q2: NOT IN 为何有 NULL 陷阱？
 *       → SQL 三值逻辑: x NOT IN (1, NULL) = x!=1 AND x!=NULL = UNKNOWN
 *   Q3: 大数据量下如何优化？
 *       → 对 Orders.customerId 建索引；分区裁剪
 */
