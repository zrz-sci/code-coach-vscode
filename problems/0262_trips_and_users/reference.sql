/*
 * 【题目本质】
 * 计算每天的行程取消率。需要：
 * 1. 过滤掉乘客或司机被封禁的行程
 * 2. 按日期分组，计算 取消数/总数
 * 3. 四舍五入保留两位小数
 *
 * 【解法总览】
 * 解法1: 双 JOIN + CASE WHEN 聚合 ⭐ 面试首选
 * 解法2: NOT IN 子查询 + IF 聚合
 * 解法3: NOT EXISTS 相关子查询
 * 解法4: AVG 简写技巧
 */

-- ============================================================
-- 解法1: 双 JOIN + CASE WHEN ⭐ 推荐
-- 思路：JOIN Users 两次分别验证 client 和 driver 未被封禁
-- 优势：语义清晰，面试官一眼看懂
-- ============================================================
SELECT
    t.request_at AS Day,
    ROUND(
        SUM(CASE WHEN t.status != 'completed' THEN 1 ELSE 0 END)
        / COUNT(*),
        2
    ) AS 'Cancellation Rate'
FROM Trips t
-- JOIN 验证乘客未封禁
JOIN Users u1 ON t.client_id = u1.users_id AND u1.banned = 'No'
-- JOIN 验证司机未封禁
JOIN Users u2 ON t.driver_id = u2.users_id AND u2.banned = 'No'
WHERE t.request_at BETWEEN '2013-10-01' AND '2013-10-03'
GROUP BY t.request_at;

-- ============================================================
-- 解法2: NOT IN 子查询
-- 思路：先找出所有被封禁用户 ID，排除对应行程
-- 注意：如果 users_id 有 NULL，NOT IN 会失效
-- ============================================================
SELECT
    request_at AS Day,
    ROUND(
        SUM(IF(status != 'completed', 1, 0)) / COUNT(*),
        2
    ) AS 'Cancellation Rate'
FROM Trips
WHERE client_id NOT IN (
    SELECT users_id FROM Users WHERE banned = 'Yes'
)
AND driver_id NOT IN (
    SELECT users_id FROM Users WHERE banned = 'Yes'
)
AND request_at BETWEEN '2013-10-01' AND '2013-10-03'
GROUP BY request_at;

-- ============================================================
-- 解法3: NOT EXISTS 相关子查询
-- 思路：用 EXISTS 检查是否存在封禁记录
-- 优势：语义最严谨，不受 NULL 影响
-- ============================================================
SELECT
    t.request_at AS Day,
    ROUND(
        SUM(CASE WHEN t.status != 'completed' THEN 1 ELSE 0 END)
        / COUNT(*),
        2
    ) AS 'Cancellation Rate'
FROM Trips t
WHERE NOT EXISTS (
    SELECT 1 FROM Users u
    WHERE u.users_id = t.client_id AND u.banned = 'Yes'
)
AND NOT EXISTS (
    SELECT 1 FROM Users u
    WHERE u.users_id = t.driver_id AND u.banned = 'Yes'
)
AND t.request_at BETWEEN '2013-10-01' AND '2013-10-03'
GROUP BY t.request_at;

-- ============================================================
-- 解法4: AVG 简写技巧
-- 思路：AVG(condition) 等价于 SUM(condition)/COUNT(*)
-- MySQL 中 bool 表达式返回 0/1，可直接 AVG
-- ============================================================
SELECT
    t.request_at AS Day,
    ROUND(AVG(t.status != 'completed'), 2) AS 'Cancellation Rate'
FROM Trips t
JOIN Users u1 ON t.client_id = u1.users_id AND u1.banned = 'No'
JOIN Users u2 ON t.driver_id = u2.users_id AND u2.banned = 'No'
WHERE t.request_at BETWEEN '2013-10-01' AND '2013-10-03'
GROUP BY t.request_at;

/*
 * 【解法对比】
 * | 解法         | 优势           | 劣势                |
 * |-------------|---------------|-------------------|
 * | 双 JOIN      | 语义清晰       | JOIN 两次 Users     |
 * | NOT IN      | 代码简洁       | NULL 陷阱           |
 * | NOT EXISTS  | 严谨安全       | 写法较冗长           |
 * | AVG 简写     | 最简洁        | MySQL 特有语法       |
 *
 * 【易错点】
 * 1. 只过滤乘客未封禁，忘记司机也要过滤 -> 结果偏大
 * 2. NOT IN 子查询如果返回 NULL，整个 WHERE 失效
 *    解决：确保 users_id NOT NULL 或用 NOT EXISTS
 * 3. 忘记 GROUP BY request_at -> 整体只有一行
 * 4. ROUND 的第二个参数必须是 2（两位小数）
 * 5. 列别名必须用 'Cancellation Rate'（带空格需引号）
 *
 * 【面试追问】
 * Q: JOIN 和 NOT IN 哪个性能更好？
 * A: 通常 JOIN 更好，因为优化器可以用索引做 hash join；
 *    NOT IN 在子查询结果集大时可能变慢。NOT EXISTS 通常与 JOIN 相当。
 *
 * Q: 如果某天完全没有行程，也要显示 0.00 怎么办？
 * A: 需要先构造日期序列表，再 LEFT JOIN 行程数据。
 */
