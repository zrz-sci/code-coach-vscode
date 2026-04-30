-- =====================================================
-- LeetCode 177: Nth Highest Salary (第N高的薪水)
-- 难度: Medium | 标签: Database
-- =====================================================
--
-- 【题目本质】
-- 编写MySQL函数，返回第N高的不同薪水。
-- 难点：LIMIT不接受表达式，必须预处理N；不足N个时返回NULL。
--
-- 【解法总览】
-- 解法1: LIMIT OFFSET + SET预处理  ← 推荐，简洁高效
-- 解法2: 子查询计数法              ← 不依赖LIMIT，但慢
-- 解法3: DENSE_RANK 窗口函数       ← 通用性最强
-- =====================================================

-- 解法1: LIMIT OFFSET + SET预处理（推荐）
-- 思路：LIMIT不接受N-1表达式，先SET N = N-1
-- 然后排序去重后用OFFSET跳过前N-1个
CREATE FUNCTION getNthHighestSalary(N INT) RETURNS INT
BEGIN
  SET N = N - 1;                -- 关键：LIMIT不接受表达式，必须预处理
  RETURN (
      SELECT DISTINCT salary
      FROM Employee
      ORDER BY salary DESC
      LIMIT 1 OFFSET N          -- 跳过前N-1个不同薪水，取第N个
  );
END


-- 解法2: 子查询计数法
-- 思路：第N高 = 恰好有N-1个不同薪水比它大
-- 优点：不依赖LIMIT语法；缺点：O(n^2)相关子查询
CREATE FUNCTION getNthHighestSalary(N INT) RETURNS INT
BEGIN
  RETURN (
      SELECT DISTINCT e1.salary
      FROM Employee e1
      WHERE (
          SELECT COUNT(DISTINCT e2.salary)   -- 数有多少个不同薪水比e1高
          FROM Employee e2
          WHERE e2.salary > e1.salary
      ) = N - 1                              -- 恰好N-1个 → e1就是第N高
  );
END


-- 解法3: DENSE_RANK 窗口函数（推荐）
-- 思路：DENSE_RANK产生无间隔排名，直接筛选rank=N
-- 优点：语义清晰，面试中最容易解释
CREATE FUNCTION getNthHighestSalary(N INT) RETURNS INT
BEGIN
  RETURN (
      SELECT DISTINCT salary
      FROM (
          SELECT salary,
                 DENSE_RANK() OVER (ORDER BY salary DESC) AS rnk
          FROM Employee
      ) ranked
      WHERE rnk = N             -- DENSE_RANK从1开始，第N高就是rank=N
  );
END


-- =====================================================
-- 【解法对比】
-- | 解法          | 时间     | 需要预处理N | 可推广到分组TopN |
-- |---------------|---------|-----------|----------------|
-- | LIMIT OFFSET  | O(nlogn)| 是(SET)   | 不方便          |
-- | 子查询计数    | O(n^2)  | 否        | 可以            |
-- | DENSE_RANK    | O(nlogn)| 否        | 非常方便        |
--
-- 【易错点】
-- 1. LIMIT 1 OFFSET N-1 → 语法错误！必须 SET N=N-1 再 OFFSET N
-- 2. 用RANK()代替DENSE_RANK() → RANK有间隔(1,1,3)，不符合"不同薪水"语义
-- 3. 忘记DISTINCT → 重复薪水被重复计数
-- 4. N=0或N为负数 → 子查询返回空集，函数自动返回NULL（无需特判）
--
-- 【面试追问】
-- Q1: 为什么LIMIT不能写表达式？→ MySQL解析器限制，非执行引擎限制
-- Q2: DENSE_RANK vs RANK vs ROW_NUMBER？→ 无间隔 vs 有间隔 vs 唯一编号
-- Q3: 大数据量下哪种最优？→ LIMIT+索引最优，子查询计数O(n^2)最差
-- =====================================================
