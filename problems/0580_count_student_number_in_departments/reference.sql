-- ============================================================
-- 580. Count Student Number in Departments
-- ============================================================
-- 难度: Medium | 标签: Database
--
-- 题意: 统计每个系的学生人数，包括没有学生的系（显示0）
--       按学生人数降序排列，人数相同则按系名字母升序
-- ============================================================

-- ============================================================
-- 解法一：LEFT JOIN + COUNT（推荐，最佳实践）
-- ============================================================
-- 思路：
--   1. Department 为主表（左表），确保所有系都出现
--   2. LEFT JOIN Student，无学生的系 student_id 为 NULL
--   3. COUNT(s.student_id) 自动忽略 NULL -> 返回 0
-- 关键: COUNT(student_id) 而非 COUNT(*)
--       COUNT(*) 对 NULL 行也计为 1，结果错误
SELECT d.dept_name,
       COUNT(s.student_id) AS student_number
FROM Department d
LEFT JOIN Student s ON d.dept_id = s.dept_id
GROUP BY d.dept_id, d.dept_name
ORDER BY student_number DESC, d.dept_name ASC;

-- ============================================================
-- 解法二：相关子查询
-- ============================================================
-- 思路：
--   对每个系执行一次 COUNT 子查询
--   无学生的系子查询天然返回 0
-- 优点: 逻辑清晰，无需理解 LEFT JOIN + COUNT 的 NULL 行为
-- 缺点: 相关子查询在大数据集上性能较差 O(n*m)
SELECT d.dept_name,
       (SELECT COUNT(*)
        FROM Student s
        WHERE s.dept_id = d.dept_id
       ) AS student_number
FROM Department d
ORDER BY student_number DESC, d.dept_name ASC;

-- ============================================================
-- 解法三：子查询先聚合 + LEFT JOIN + IFNULL
-- ============================================================
-- 思路：
--   1. 子查询在 Student 表中按 dept_id 分组统计人数
--   2. LEFT JOIN 回 Department 表
--   3. IFNULL 处理无学生的系（NULL -> 0）
-- 优点: 聚合在子查询中完成，外层不需要 GROUP BY
SELECT d.dept_name,
       IFNULL(cnt.student_number, 0) AS student_number
FROM Department d
LEFT JOIN (
    SELECT dept_id,
           COUNT(*) AS student_number
    FROM Student
    GROUP BY dept_id
) cnt ON d.dept_id = cnt.dept_id
ORDER BY student_number DESC, d.dept_name ASC;
