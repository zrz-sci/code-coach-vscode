-- =============================================
-- LeetCode 571: Find Median Given Frequency of Numbers
-- =============================================

-- 【题目本质】
-- 频率压缩表求中位数：不展开数据，用累计频率（前缀和）定位中位数。
-- 结果四舍五入到一位小数。

-- 【解法总览】
-- 1. 窗口函数累计频率法 (推荐)
-- 2. 交叉累计法（正向反向累计比较）
-- 3. 子查询前缀和法

-- =============================================
-- 解法一: 窗口函数累计频率法（推荐）
-- 思路: 算前缀和 cum_freq，找覆盖中间位置的 num
-- =============================================
SELECT ROUND(AVG(num), 1) AS median
FROM (
    SELECT num, frequency,
           SUM(frequency) OVER (ORDER BY num) AS cum_freq,
           (SUM(frequency) OVER ()) / 2.0 AS mid
    FROM Numbers
) t
WHERE cum_freq >= mid
  AND cum_freq - frequency < mid + 1;

-- =============================================
-- 解法二: 交叉累计法
-- 思路: 一个数是中位数 ⇔ 它的频率 >= |左侧累计 - 右侧累计|
-- =============================================
SELECT ROUND(AVG(n.num), 1) AS median
FROM Numbers n
WHERE n.frequency >= ABS(
    (SELECT SUM(frequency) FROM Numbers WHERE num <= n.num) -
    (SELECT SUM(frequency) FROM Numbers WHERE num >= n.num)
);

-- =============================================
-- 解法三: 子查询前缀和法
-- 思路: 用子查询手动计算前缀和，不依赖窗口函数
-- =============================================
SELECT ROUND(AVG(n1.num), 1) AS median
FROM Numbers n1
WHERE (SELECT SUM(frequency) FROM Numbers WHERE num <= n1.num) >=
      (SELECT SUM(frequency) FROM Numbers) / 2.0
  AND (SELECT SUM(frequency) FROM Numbers WHERE num >= n1.num) >=
      (SELECT SUM(frequency) FROM Numbers) / 2.0;

-- 【解法对比】
-- | 解法            | 优点              | 缺点                 |
-- |----------------|------------------|---------------------|
-- | 窗口函数累计     | 高效，逻辑清晰     | 需要窗口函数支持       |
-- | 交叉累计        | 数学直觉好         | 相关子查询 O(n²)      |
-- | 子查询前缀和     | 不依赖窗口函数     | 多次全表扫描           |

-- 【易错点】
-- 1. 不能展开频率表为行 → 大数据量会超时/超内存
-- 2. 偶数个元素时两个中位数候选需要 AVG
-- 3. 结果必须 ROUND(..., 1) 到一位小数
-- 4. 除法要用 2.0 而非 2，避免整数除法截断

-- 【面试追问】
-- Q1: 总数为偶数时怎么处理？→ AVG 自动处理（可能选中两个不同的 num）
-- Q2: 与 569 的区别？→ 569 找中位数行，本题求中位数值；569 展开数据，本题压缩数据
-- Q3: 不用窗口函数？→ 解法三子查询前缀和
