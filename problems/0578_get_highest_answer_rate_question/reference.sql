-- ============================================================
-- 578. Get Highest Answer Rate Question
-- 难度: Medium | 标签: Database
-- ============================================================

-- ============================================================
-- 解法一: 条件聚合 SUM(CASE) + ORDER BY (推荐)
-- 思路: 按 question_id 分组，分别统计 answer 和 show 次数
--       计算 answer_rate = answer_count / show_count
--       按 rate DESC, question_id ASC 排序取第一
-- 关键: 乘 1.0 避免整数除法截断
-- ============================================================
SELECT question_id AS survey_log
FROM SurveyLog
GROUP BY question_id
ORDER BY SUM(CASE WHEN action = 'answer' THEN 1 ELSE 0 END) * 1.0
       / SUM(CASE WHEN action = 'show' THEN 1 ELSE 0 END) DESC,
         question_id ASC
LIMIT 1;

-- ============================================================
-- 解法二: MySQL IF 简写
-- 思路: IF(condition, true_val, false_val) 是 CASE WHEN 的简洁替代
-- 注意: IF 是 MySQL 特有函数，不可移植到其他数据库
-- ============================================================
-- SELECT question_id AS survey_log
-- FROM SurveyLog
-- GROUP BY question_id
-- ORDER BY SUM(IF(action = 'answer', 1, 0))
--        / SUM(IF(action = 'show', 1, 0)) DESC,
--          question_id ASC
-- LIMIT 1;

-- ============================================================
-- 解法三: COUNT 替代 SUM
-- 思路: COUNT 只计非 NULL 值，CASE 无 ELSE 时默认返回 NULL
--       因此 COUNT(CASE WHEN action='answer' THEN 1 END) 等效于 SUM
-- ============================================================
-- SELECT question_id AS survey_log
-- FROM SurveyLog
-- GROUP BY question_id
-- ORDER BY COUNT(CASE WHEN action = 'answer' THEN 1 END) * 1.0
--        / COUNT(CASE WHEN action = 'show' THEN 1 END) DESC,
--          question_id ASC
-- LIMIT 1;

-- ============================================================
-- 解法四: 窗口函数 RANK (适合返回所有并列最高)
-- 思路: 用 RANK() 排名，外层取 rnk = 1
-- 优点: 天然处理并列，可扩展为 Top-N
-- ============================================================
-- SELECT question_id AS survey_log
-- FROM (
--     SELECT question_id,
--            RANK() OVER (
--                ORDER BY SUM(CASE WHEN action = 'answer' THEN 1 ELSE 0 END) * 1.0
--                       / SUM(CASE WHEN action = 'show' THEN 1 ELSE 0 END) DESC,
--                         question_id ASC
--            ) AS rnk
--     FROM SurveyLog
--     GROUP BY question_id
-- ) ranked
-- WHERE rnk = 1;

-- ============================================================
-- 知识点总结:
-- 1. 条件聚合: SUM(CASE WHEN ... THEN 1 ELSE 0 END) 按条件计数
-- 2. 整数除法: 乘 1.0 或 CAST 确保浮点除法
-- 3. 多字段排序: ORDER BY rate DESC, question_id ASC
-- 4. 防除零: NULLIF(denominator, 0) 让除零返回 NULL
-- 5. 列别名: AS survey_log 满足输出格式要求
-- ============================================================
