-- =====================================================
-- LeetCode 178: Rank Scores (分数排名)
-- 难度: Medium | 标签: Database
-- =====================================================
--
-- 【题目本质】
-- 对分数进行DENSE_RANK排名：相同分数同名次，名次连续无间隔。
-- 输出score和rank两列，按score降序排列。
--
-- 【解法总览】
-- 解法1: DENSE_RANK 窗口函数  ← 推荐，一行搞定
-- 解法2: 子查询计数法          ← 不依赖窗口函数的传统方法
-- 解法3: 自连接法              ← JOIN思路
-- =====================================================

-- 解法1: DENSE_RANK 窗口函数（推荐）
-- 思路：DENSE_RANK的定义完美匹配题目要求
-- 注意：`rank` 是MySQL保留字，必须加反引号
SELECT score,
       DENSE_RANK() OVER (ORDER BY score DESC) AS `rank`
FROM Scores
ORDER BY score DESC;


-- 解法2: 子查询计数法
-- 思路：排名 = 有多少个不同分数 >= 当前分数
-- 例如最高分4.00，只有1个不同分数>=它，所以排名=1
SELECT s1.score,
       (SELECT COUNT(DISTINCT s2.score)
        FROM Scores s2
        WHERE s2.score >= s1.score      -- >= 包含自身，所以最高分排名为1
       ) AS `rank`
FROM Scores s1
ORDER BY s1.score DESC;


-- 解法3: 自连接法
-- 思路：JOIN自身，条件s2.score >= s1.score
-- 对每个s1行，统计匹配的不同s2.score数量 = 排名
SELECT s1.score,
       COUNT(DISTINCT s2.score) AS `rank`
FROM Scores s1
JOIN Scores s2 ON s2.score >= s1.score  -- 找所有>=当前分数的记录
GROUP BY s1.id, s1.score                -- 按id分组（避免同分数合并）
ORDER BY s1.score DESC;


-- =====================================================
-- 【解法对比】
-- | 解法          | 时间     | 是否需要窗口函数 | 面试推荐 |
-- |---------------|---------|----------------|---------|
-- | DENSE_RANK    | O(nlogn)| 是             | 首选    |
-- | 子查询计数    | O(n^2)  | 否             | 备选    |
-- | 自连接        | O(n^2)  | 否             | 备选    |
--
-- 【易错点】
-- 1. 用RANK()代替DENSE_RANK() → 排名有间隔(1,1,3)不符合题意
-- 2. `rank`不加反引号 → MySQL8.0语法错误（保留字）
-- 3. 子查询中用>而非>= → 最高分排名变成0
-- 4. 子查询中忘记DISTINCT → 相同分数被重复计数导致排名偏大
--
-- 【面试追问】
-- Q1: 三种排名函数的区别？→ ROW_NUMBER唯一/RANK有间隔/DENSE_RANK无间隔
-- Q2: 不用窗口函数能否实现RANK(有间隔)？→ COUNT(s2.id)而非COUNT(DISTINCT)
-- Q3: 性能优化？→ score列加索引，但窗口函数方案本身已是最优O(nlogn)
-- =====================================================
