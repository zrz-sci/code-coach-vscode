/*
 * LeetCode 182. 查找重复的电子邮箱 (Easy)
 *
 * 【题目本质】
 *   单表分组统计 —— 找出 email 列中出现次数 > 1 的值。
 *   核心考点：GROUP BY + HAVING 的基本用法。
 *
 * 【解法总览】
 *   解法1: GROUP BY + HAVING   — 最直观，推荐 ⭐⭐⭐
 *   解法2: 自连接 (Self Join)  — 展示 JOIN 思维 ⭐⭐
 *   解法3: 子查询 + IN         — 嵌套思路 ⭐⭐
 */

-- ============================================================
-- 解法1: GROUP BY + HAVING（推荐）
-- 思路：按 email 分组，保留出现次数 > 1 的组
-- ============================================================
SELECT email AS Email
FROM Person
GROUP BY email
HAVING COUNT(*) > 1;

-- ============================================================
-- 解法2: 自连接
-- 思路：表与自身连接，找 email 相同但 id 不同的行
-- 注意：必须加 DISTINCT 去重
-- ============================================================
SELECT DISTINCT a.email AS Email
FROM Person a
JOIN Person b
  ON a.email = b.email
 AND a.id != b.id;

-- ============================================================
-- 解法3: 子查询
-- 思路：先在子查询中找到重复 email，外层查询过滤
-- ============================================================
SELECT DISTINCT email AS Email
FROM Person
WHERE email IN (
    SELECT email
    FROM Person
    GROUP BY email
    HAVING COUNT(*) > 1
);

/*
 * 【解法对比】
 * +------------------+----------+-----------+------+
 * | 解法             | 可读性   | 性能      | 推荐 |
 * +------------------+----------+-----------+------+
 * | GROUP BY+HAVING  | 高       | O(n)      | ⭐⭐⭐ |
 * | 自连接           | 中       | O(n²)     | ⭐⭐   |
 * | 子查询+IN        | 中       | O(n)      | ⭐⭐   |
 * +------------------+----------+-----------+------+
 *
 * 【易错点】
 *   1. 自连接忘记 DISTINCT → 重复 email 出现多行
 *   2. 使用 WHERE COUNT(*) > 1 → 语法错误，应用 HAVING
 *   3. 列别名大小写 → 注意题目要求 "Email"
 *
 * 【面试追问】
 *   Q1: HAVING 和 WHERE 的区别？
 *       → WHERE 过滤原始行(GROUP BY 之前)；HAVING 过滤分组结果(GROUP BY 之后)
 *   Q2: 如何同时输出重复次数？
 *       → SELECT email, COUNT(*) AS cnt FROM Person GROUP BY email HAVING cnt > 1
 *   Q3: 亿级数据如何优化？
 *       → email 列建索引；分区表；流式计算
 */
