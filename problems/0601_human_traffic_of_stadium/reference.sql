-- ============================================================
-- LeetCode 601: 体育馆的人流量 (Human Traffic of Stadium)
-- ============================================================

-- 【题目本质】
-- 找 id 连续且 people >= 100 的行，只输出属于长度 >= 3 的连续段的行。
-- 经典 SQL 连续问题，核心技巧：id - ROW_NUMBER() 分组。

-- 【解法总览】
-- 解法一：ROW_NUMBER 分组法（推荐）
-- 解法二：三表自连接

-- ============================================================
-- 解法一：ROW_NUMBER 分组法（推荐）
-- 思路：people >= 100 的连续 id 行，id - row_number 值相同
-- ============================================================
WITH filtered AS (
    SELECT *, ROW_NUMBER() OVER (ORDER BY id) AS rn
    FROM Stadium
    WHERE people >= 100
),
grouped AS (
    SELECT *, id - rn AS grp
    FROM filtered
)
SELECT id, visit_date, people
FROM grouped
WHERE grp IN (
    SELECT grp FROM grouped GROUP BY grp HAVING COUNT(*) >= 3
)
ORDER BY visit_date;

-- ============================================================
-- 解法二：三表自连接
-- 思路：三个 Stadium 表自连接，枚举连续三行的三种位置关系
-- ============================================================
-- SELECT DISTINCT s1.*
-- FROM Stadium s1, Stadium s2, Stadium s3
-- WHERE s1.people >= 100 AND s2.people >= 100 AND s3.people >= 100
--   AND (
--     (s1.id = s2.id - 1 AND s2.id = s3.id - 1) OR   -- s1 是开头
--     (s1.id = s2.id + 1 AND s1.id = s3.id - 1) OR   -- s1 是中间
--     (s1.id = s2.id + 1 AND s2.id = s3.id + 1)       -- s1 是结尾
--   )
-- ORDER BY s1.visit_date;

-- 【解法对比】
-- | 解法             | 可读性 | 扩展性 | 性能      |
-- |-----------------|--------|--------|----------|
-- | ROW_NUMBER ⭐    | ⭐⭐⭐  | 支持任意 k | O(n log n) |
-- | 三表自连接       | ⭐⭐    | 仅支持 k=3 | O(n^3)     |

-- 【易错点】
-- 1. 按 id 连续，不是 visit_date 连续
-- 2. 三表自连接必须加 DISTINCT，同一行可能匹配多组
-- 3. ORDER BY visit_date，不是 ORDER BY id
-- 4. ROW_NUMBER 必须在 WHERE people >= 100 之后计算

-- 【面试追问】
-- Q1: 连续 k 行 → ROW_NUMBER 法改 HAVING COUNT >= k
-- Q2: 按日期连续 → DATEDIFF - ROW_NUMBER 做分组键
-- Q3: 数据量大 → 索引 people 列 + 先过滤再分组
