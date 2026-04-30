-- =====================================================
-- LeetCode 176: Second Highest Salary (第二高的薪水)
-- 难度: Medium | 标签: Database
-- =====================================================
--
-- 【题目本质】
-- 从Employee表中找第二高的不同薪水。
-- 难点：不存在第二高时必须返回NULL（而非空结果集）。
--
-- 【解法总览】
-- 解法1: 子查询 + LIMIT OFFSET  ← 推荐，简洁直观
-- 解法2: MAX + 排除最大值        ← 推荐，无需排序
-- 解法3: DENSE_RANK 窗口函数     ← 通用性强
-- =====================================================

-- 解法1: 子查询 + LIMIT OFFSET（推荐）
-- 思路：将排序取值的查询放在SELECT子句的标量子查询中
-- 当内层查询返回空集时，标量子查询自动变为NULL → 满足题意
SELECT (
    SELECT DISTINCT salary
    FROM Employee
    ORDER BY salary DESC
    LIMIT 1 OFFSET 1          -- 跳过最高薪水，取第二高
) AS SecondHighestSalary;


-- 解法2: MAX + 排除最大值（推荐）
-- 思路：先用子查询找到最高薪水，再在排除它之后的集合中取MAX
-- MAX()在空集上自然返回NULL，无需额外处理
SELECT MAX(salary) AS SecondHighestSalary
FROM Employee
WHERE salary < (              -- 排除最高薪水
    SELECT MAX(salary) FROM Employee
);


-- 解法3: DENSE_RANK 窗口函数
-- 思路：用DENSE_RANK给所有不同薪水排名，然后筛选rank=2
-- MAX包装确保无rank=2时返回NULL
SELECT MAX(salary) AS SecondHighestSalary
FROM (
    SELECT salary,
           DENSE_RANK() OVER (ORDER BY salary DESC) AS rnk
    FROM Employee
) ranked
WHERE rnk = 2;


-- =====================================================
-- 【解法对比】
-- | 解法          | 是否排序 | 空值处理   | 可推广到第N高 |
-- |---------------|---------|-----------|-------------|
-- | LIMIT OFFSET  | 是      | 子查询自动 | 改OFFSET即可 |
-- | MAX排除法     | 否      | MAX自动   | 需嵌套N层    |
-- | DENSE_RANK    | 是      | MAX包装   | 改WHERE即可  |
--
-- 【易错点】
-- 1. 忘记DISTINCT → 重复薪水干扰排名
-- 2. 直接SELECT而不包子查询 → 空集而非NULL
-- 3. OFFSET从0开始 → OFFSET 1才是跳过第一条
-- 4. 忘记DESC → 取成了最低薪水
--
-- 【面试追问】
-- Q1: 只有一个薪水值时怎么办？→ 子查询返回空集，外层自动NULL
-- Q2: 推广到第N高？→ 见LeetCode 177，用LIMIT 1 OFFSET N-1
-- Q3: MAX方法 vs LIMIT方法性能？→ MAX是O(n)无需排序，LIMIT需O(nlogn)排序
-- =====================================================
