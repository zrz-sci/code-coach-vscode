-- ============================================================
-- 585. Investments in 2016
-- ============================================================
-- 难度: Medium | 标签: Database
--
-- 题意: 找到满足以下两个条件的投保人，求其 tiv_2016 之和：
--   1. tiv_2015 值与至少一个其他投保人相同
--   2. (lat, lon) 位置在表中唯一
-- 结果保留两位小数
-- ============================================================

-- ============================================================
-- 解法一：窗口函数（推荐，MySQL 8.0+）
-- ============================================================
-- 思路：
--   1. COUNT OVER (PARTITION BY tiv_2015) -> 检测 tiv_2015 是否重复
--   2. COUNT OVER (PARTITION BY lat, lon) -> 检测位置是否唯一
--   3. 两个条件都满足的行做 SUM + ROUND
-- 优点: 一次扫描，O(n)，最高效
WITH enriched AS (
    SELECT pid,
           tiv_2016,
           COUNT(*) OVER (PARTITION BY tiv_2015) AS cnt_tiv,
           COUNT(*) OVER (PARTITION BY lat, lon) AS cnt_loc
    FROM Insurance
)
SELECT ROUND(SUM(tiv_2016), 2) AS tiv_2016
FROM enriched
WHERE cnt_tiv >= 2
  AND cnt_loc = 1;

-- ============================================================
-- 解法二：IN + 子查询（兼容 MySQL 5.7）
-- ============================================================
-- 思路：
--   条件A: tiv_2015 IN (出现>=2次的 tiv_2015 值集合)
--   条件B: (lat,lon) IN (只出现1次的位置集合)
--   主查询取两个条件的交集
-- 注意: 位置必须用 (lat, lon) 组合判断，不能分开
SELECT ROUND(SUM(tiv_2016), 2) AS tiv_2016
FROM Insurance
WHERE tiv_2015 IN (
    SELECT tiv_2015
    FROM Insurance
    GROUP BY tiv_2015
    HAVING COUNT(*) >= 2
)
AND (lat, lon) IN (
    SELECT lat, lon
    FROM Insurance
    GROUP BY lat, lon
    HAVING COUNT(*) = 1
);

-- ============================================================
-- 解法三：EXISTS / NOT EXISTS
-- ============================================================
-- 思路：
--   EXISTS: 存在另一个投保人有相同的 tiv_2015
--   NOT EXISTS: 不存在另一个投保人在相同位置
-- 缺点: 对每一行做相关子查询，大数据集性能差 O(n^2)
SELECT ROUND(SUM(i1.tiv_2016), 2) AS tiv_2016
FROM Insurance i1
WHERE EXISTS (
    SELECT 1
    FROM Insurance i2
    WHERE i2.tiv_2015 = i1.tiv_2015
      AND i2.pid != i1.pid
)
AND NOT EXISTS (
    SELECT 1
    FROM Insurance i2
    WHERE i2.lat = i1.lat
      AND i2.lon = i1.lon
      AND i2.pid != i1.pid
);

-- ============================================================
-- 解法四：自连接（面试展示用）
-- ============================================================
-- 思路：
--   两次 LEFT JOIN 自己来检测两个条件
--   有 tiv_2015 匹配 -> 条件A满足
--   无 (lat,lon) 匹配 -> 条件B满足
-- 缺点: 需要 DISTINCT 或小心去重
SELECT ROUND(SUM(i1.tiv_2016), 2) AS tiv_2016
FROM Insurance i1
WHERE i1.pid IN (
    -- 条件A: tiv_2015 有重复
    SELECT DISTINCT a.pid
    FROM Insurance a
    JOIN Insurance b ON a.tiv_2015 = b.tiv_2015 AND a.pid != b.pid
)
AND i1.pid NOT IN (
    -- 条件B: (lat, lon) 有重复的 pid 排除
    SELECT DISTINCT a.pid
    FROM Insurance a
    JOIN Insurance b ON a.lat = b.lat AND a.lon = b.lon AND a.pid != b.pid
);
