-- ============================================================
-- 619. Biggest Single Number (只出现一次的最大数字)
-- 难度: Easy | 标签: Database
-- ============================================================

-- ============================================================
-- 解法一: 子查询 + MAX (推荐, 最简洁)
-- 思路:
--   1) 内层: GROUP BY num + HAVING COUNT(*) = 1 -> 筛出 single numbers
--   2) 外层: MAX(num) -> 取最大值
--   3) 空集处理: MAX 对空集自动返回 NULL
-- 时间复杂度: O(n log n)  空间: O(n)
-- ============================================================
SELECT MAX(num) AS num
FROM (
    SELECT num
    FROM MyNumbers
    GROUP BY num
    HAVING COUNT(*) = 1
) t;

-- ============================================================
-- 解法二: CTE 写法 (可读性更好)
-- 思路: 与解法一完全等价, 用 WITH 子句提高可读性
-- ============================================================
WITH singles AS (
    SELECT num
    FROM MyNumbers
    GROUP BY num
    HAVING COUNT(*) = 1
)
SELECT MAX(num) AS num
FROM singles;

-- ============================================================
-- 解法三: 标量子查询 + ORDER BY (处理空集返回 NULL)
-- 思路: 将查询放入 SELECT 的标量子查询位置
--   标量子查询返回空集时自动变为 NULL
--   ORDER BY DESC + LIMIT 1 取最大值
-- 时间复杂度: O(n log n)  空间: O(n)
-- ============================================================
SELECT (
    SELECT num
    FROM MyNumbers
    GROUP BY num
    HAVING COUNT(*) = 1
    ORDER BY num DESC
    LIMIT 1
) AS num;

-- ============================================================
-- 解法四: 窗口函数 (MySQL 8.0+)
-- 思路: COUNT OVER (PARTITION BY num) 计算每个值的出现次数
--   过滤 cnt = 1 的行, 取 MAX
-- 时间复杂度: O(n log n)  空间: O(n)
-- ============================================================
SELECT MAX(num) AS num
FROM (
    SELECT
        num,
        COUNT(*) OVER (PARTITION BY num) AS cnt
    FROM MyNumbers
) t
WHERE cnt = 1;

-- ============================================================
-- 解法五: HAVING + IN 子查询 (另一种组合方式)
-- 思路: 先用子查询找出 single numbers, 再从原表中 MAX
--   利用 IN 过滤, 外层直接 MAX
-- ============================================================
SELECT MAX(num) AS num
FROM MyNumbers
WHERE num IN (
    SELECT num
    FROM MyNumbers
    GROUP BY num
    HAVING COUNT(*) = 1
);
