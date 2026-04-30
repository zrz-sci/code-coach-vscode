-- LeetCode 595: Big Countries
-- ===========================================
-- 题意：从 World 表中选出面积 >= 3000000 或人口 >= 25000000 的国家
-- 返回：name, population, area

-- ===========================================
-- 解法一：WHERE + OR (最简洁)
-- 直接用 OR 连接两个条件
-- 小表场景简洁高效

SELECT name, population, area
FROM World
WHERE area >= 3000000 OR population >= 25000000;


-- ===========================================
-- 解法二：UNION (索引友好)
-- 拆成两个独立查询，各自可走独立索引
-- UNION 自动去重，无需额外 DISTINCT
-- 大表 + 各列有索引时性能更优

-- SELECT name, population, area
-- FROM World
-- WHERE area >= 3000000
--
-- UNION
--
-- SELECT name, population, area
-- FROM World
-- WHERE population >= 25000000;


-- ===========================================
-- 解法三：UNION ALL + 外层去重
-- UNION ALL 不排序去重，速度更快
-- 通过外层 DISTINCT 手动去重
-- 适合需要精细控制去重策略的场景

-- SELECT DISTINCT name, population, area
-- FROM (
--     SELECT name, population, area FROM World WHERE area >= 3000000
--     UNION ALL
--     SELECT name, population, area FROM World WHERE population >= 25000000
-- ) t;


-- ===========================================
-- 性能对比：
-- | 方法          | 索引利用 | 去重开销 | 推荐场景       |
-- |---------------|----------|----------|----------------|
-- | WHERE + OR    | 可能差   | 无       | 小表/无索引    |
-- | UNION         | 好       | 隐式排序 | 大表有索引     |
-- | UNION ALL+DIS | 好       | 显式     | 精细控制       |
--
-- 对于 World 表 (~200行)，三种方法无明显差异
-- 生产环境大表场景建议用 UNION + 各列独立索引
