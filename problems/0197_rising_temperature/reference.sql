-- ============================================================
-- LeetCode 197: Rising Temperature (上升的温度)
-- 难度: Easy | 标签: Database
--
-- 【题目本质】
-- 找出比前一天温度更高的日期的 id
-- 关键：用日期函数关联「昨天」，不能用 id 差
--
-- 【解法总览】
-- 解法1: DATEDIFF 自连接    — 最直观，面试首选
-- 解法2: DATE_SUB 自连接    — 等价写法，更显式
-- 解法3: 窗口函数 LAG       — MySQL 8.0+，需额外验证日期连续
-- ============================================================

-- ==================== 解法1: DATEDIFF 自连接（推荐） ====================
-- 思路：w1 代表「今天」，w2 代表「昨天」
--   连接条件：两者日期恰好差1天
--   筛选条件：今天温度 > 昨天温度
SELECT w1.id
FROM Weather w1
JOIN Weather w2
  ON DATEDIFF(w1.recordDate, w2.recordDate) = 1
WHERE w1.temperature > w2.temperature;

-- ==================== 解法2: DATE_SUB 自连接 ====================
-- 思路：将「昨天」表达为 DATE_SUB(今天, 1天)
-- 与解法1 等价，只是日期关联方式不同
-- SELECT w1.id
-- FROM Weather w1
-- JOIN Weather w2
--   ON w2.recordDate = DATE_SUB(w1.recordDate, INTERVAL 1 DAY)
-- WHERE w1.temperature > w2.temperature;

-- 也可以用 DATE_ADD：
-- SELECT w1.id
-- FROM Weather w1
-- JOIN Weather w2
--   ON w1.recordDate = DATE_ADD(w2.recordDate, INTERVAL 1 DAY)
-- WHERE w1.temperature > w2.temperature;

-- ==================== 解法3: 窗口函数 LAG（MySQL 8.0+） ====================
-- 思路：按日期排序，用 LAG 取前一行的温度和日期
-- 注意：LAG 取的是排序后的前一行，日期不一定连续
--       必须额外检查 DATEDIFF = 1
-- SELECT id
-- FROM (
--     SELECT id,
--            temperature,
--            recordDate,
--            LAG(temperature) OVER (ORDER BY recordDate) AS prev_temp,
--            LAG(recordDate) OVER (ORDER BY recordDate) AS prev_date
--     FROM Weather
-- ) AS sub
-- WHERE temperature > prev_temp
--   AND DATEDIFF(recordDate, prev_date) = 1;

-- ============================================================
-- 【解法对比】
-- | 解法       | 简洁度 | MySQL版本要求 | 性能   | 面试推荐 |
-- |-----------|--------|-------------|--------|---------|
-- | DATEDIFF  | ★★★    | 所有版本     | 好      | ★★★     |
-- | DATE_SUB  | ★★★    | 所有版本     | 好      | ★★★     |
-- | 窗口函数   | ★★     | 8.0+        | 较好    | ★★      |
--
-- 【易错点】
-- 1. 用 id-1 代替日期差 → id 和日期不一定对应
-- 2. DATEDIFF 方向写反 → DATEDIFF(w1, w2)=1 表示 w1 比 w2 晚一天
-- 3. LAG 后不验证日期连续性 → 排序后前一行可能跳天
-- 4. 温度比较方向写反 → 要求今天 > 昨天
--
-- 【面试追问】
-- Q1: 连续3天温度上升？ → 三表自连接或双 LAG
-- Q2: DATEDIFF vs TIMESTAMPDIFF？ → 前者只算天，后者可指定秒/分/时
-- Q3: recordDate 不唯一怎么办？ → GROUP BY date + AVG 先聚合
-- ============================================================
