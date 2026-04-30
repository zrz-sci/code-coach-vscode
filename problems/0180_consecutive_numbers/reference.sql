-- =====================================================
-- LeetCode 180: Consecutive Numbers (连续出现的数字)
-- 难度: Medium | 标签: Database
-- =====================================================
--
-- 【题目本质】
-- 找出Logs表中至少连续出现三次的数字。
-- id是自增连续列，"连续"意味着相邻id行的num相同。
--
-- 【解法总览】
-- 解法1: 三表自连接         ← 推荐，最直观
-- 解法2: LAG窗口函数        ← 推荐，现代SQL写法
-- 解法3: 连续分组法(差值法)  ← 可推广到连续K次
-- =====================================================

-- 解法1: 三表自连接（推荐）
-- 思路：将Logs表JOIN自身三次，匹配连续三行id且num相同
-- DISTINCT去重：连续4次会匹配出两组三元组
SELECT DISTINCT l1.num AS ConsecutiveNums
FROM Logs l1
JOIN Logs l2 ON l1.id = l2.id - 1    -- l2是l1的下一行
JOIN Logs l3 ON l2.id = l3.id - 1    -- l3是l2的下一行
WHERE l1.num = l2.num                 -- 三行的num必须相同
  AND l2.num = l3.num;


-- 解法2: LAG窗口函数（推荐）
-- 思路：用LAG取前1行和前2行的num值
-- 当前行num = 前1行num = 前2行num → 连续3次
SELECT DISTINCT num AS ConsecutiveNums
FROM (
    SELECT num,
           LAG(num, 1) OVER (ORDER BY id) AS prev1,   -- 前一行的num
           LAG(num, 2) OVER (ORDER BY id) AS prev2    -- 前两行的num
    FROM Logs
) t
WHERE num = prev1 AND num = prev2;    -- 三行值相同（NULL不等于任何值，自动排除边界）


-- 解法3: 连续分组法（差值法）
-- 思路：对同一个num，如果它在连续的id上出现，
--   id - ROW_NUMBER() 的差值是常数 → 属于同一个连续段
-- 统计每段长度 >= 3 的num
SELECT DISTINCT num AS ConsecutiveNums
FROM (
    SELECT num, COUNT(*) AS cnt
    FROM (
        SELECT num,
               id - ROW_NUMBER() OVER (PARTITION BY num ORDER BY id) AS grp
               -- 连续段内 id-rn 为常数，不同段值不同
        FROM Logs
    ) t
    GROUP BY num, grp             -- 按(num, 连续段)分组
    HAVING COUNT(*) >= 3          -- 只保留长度>=3的连续段
) result;


-- =====================================================
-- 【解法对比】
-- | 解法        | 时间     | 可推广到K次 | 依赖id连续 |
-- |-------------|---------|-----------|-----------|
-- | 三表自连接  | O(n)    | 需K-1个JOIN| 是        |
-- | LAG窗口     | O(n)    | 需K-1个LAG | 否(按id排序)|
-- | 差值分组    | O(nlogn)| 改HAVING即可| 否        |
--
-- 【易错点】
-- 1. 忘记DISTINCT → 连续4次输出2个重复结果
-- 2. JOIN条件方向写反 → l1.id = l2.id + 1 应为 l2.id - 1
-- 3. LAG返回NULL时直接比较 → NULL = NULL 为FALSE，自动排除（正确行为）
-- 4. 差值法忘记PARTITION BY num → 不同num混在一起分组错误
--
-- 【面试追问】
-- Q1: 推广到连续K次？→ 差值法最灵活，改HAVING COUNT(*)>=K即可
-- Q2: id不连续怎么办？→ 不能用自连接的id+1，改用ROW_NUMBER()生成连续序号
-- Q3: 性能比较？→ 自连接+索引O(n)，LAG O(n)，差值法O(nlogn)因排序
-- =====================================================
