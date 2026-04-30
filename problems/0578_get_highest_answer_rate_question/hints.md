# 578. Get Highest Answer Rate Question - 查询回答率最高的问题

## 核心思路

回答率 = 某问题被 answer 的次数 / 被 show 的次数。按 question_id 分组，用条件聚合分别统计 answer 和 show 的次数，计算比率后排序取最大。平局时取 question_id 最小的。核心是 **条件聚合 + 多字段排序**。

## 思维链

1. **理解回答率**：answer_rate = COUNT(action='answer') / COUNT(action='show')
2. **数据分组**：按 question_id 分组
3. **条件聚合**：`SUM(CASE WHEN action = 'answer' THEN 1 ELSE 0 END)` 统计回答次数
4. **计算比率**：answer_count / show_count
5. **排序策略**：先按 answer_rate DESC，再按 question_id ASC
6. **取第一条**：LIMIT 1
7. **别名注意**：输出列名为 `survey_log`

## 解法概览

| 解法 | 思路 | 适用场景 | 推荐 |
|------|------|----------|------|
| 条件聚合 + ORDER BY | SUM(CASE) 分别统计，直接排序 | 最通用，面试首选 | ⭐ |
| COUNT + IF (MySQL) | COUNT(IF(...)) 简洁写法 | MySQL 特有 | |
| 窗口函数 | RANK() + 子查询 | 需返回多条并列时 | |
| 子查询 + MAX | 先算最大回答率再匹配 | 逻辑更分层 | |

## 关键提示

- **SUM vs COUNT 做条件统计**：`SUM(CASE WHEN ... THEN 1 ELSE 0 END)` 和 `COUNT(CASE WHEN ... THEN 1 END)` 效果相同（COUNT 忽略 NULL）
- **除零风险**：如果某问题没有 show 记录，show_count 为 0 会导致除零。但题意保证先 show 后 answer/skip
- **MySQL 的 IF 函数**：`SUM(IF(action='answer', 1, 0))` 是 MySQL 简写，非标准 SQL
- **输出列名**：题目要求列名为 `survey_log`，需要用 AS 别名
- **整数除法陷阱**：两个整数相除在某些数据库中会截断为 0，建议乘 1.0 或用 CAST

## 解法详解

### 解法一：条件聚合 + ORDER BY（推荐）

```sql
-- 思路：SUM(CASE) 分别统计 answer 和 show，计算比率排序
SELECT question_id AS survey_log
FROM SurveyLog
GROUP BY question_id
ORDER BY SUM(CASE WHEN action = 'answer' THEN 1 ELSE 0 END) * 1.0
       / SUM(CASE WHEN action = 'show' THEN 1 ELSE 0 END) DESC,
         question_id ASC
LIMIT 1;
```

**执行流程**：
1. GROUP BY question_id：按问题分组
2. SUM(CASE)：分别统计每组的 answer 次数和 show 次数
3. 计算比率：answer_count / show_count（乘 1.0 避免整数截断）
4. ORDER BY：按比率降序，平局按 question_id 升序
5. LIMIT 1：取第一条

### 解法二：MySQL IF 简写

```sql
-- 思路：IF 函数是 CASE WHEN 的简写形式
SELECT question_id AS survey_log
FROM SurveyLog
GROUP BY question_id
ORDER BY SUM(IF(action = 'answer', 1, 0))
       / SUM(IF(action = 'show', 1, 0)) DESC,
         question_id ASC
LIMIT 1;
```

### 解法三：COUNT 替代 SUM

```sql
-- 思路：COUNT 只计数非 NULL 值，CASE 不写 ELSE 则默认 NULL
SELECT question_id AS survey_log
FROM SurveyLog
GROUP BY question_id
ORDER BY COUNT(CASE WHEN action = 'answer' THEN 1 END) * 1.0
       / COUNT(CASE WHEN action = 'show' THEN 1 END) DESC,
         question_id ASC
LIMIT 1;
```

### 解法四：窗口函数（适合返回多条并列）

```sql
-- 思路：窗口函数排名，适合需要返回所有并列最高的场景
SELECT question_id AS survey_log
FROM (
    SELECT question_id,
           RANK() OVER (
               ORDER BY SUM(CASE WHEN action = 'answer' THEN 1 ELSE 0 END) * 1.0
                      / SUM(CASE WHEN action = 'show' THEN 1 ELSE 0 END) DESC,
                        question_id ASC
           ) AS rnk
    FROM SurveyLog
    GROUP BY question_id
) ranked
WHERE rnk = 1;
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|----------|----------|------|
| ✗ `SELECT question_id` 不加别名 | ✓ `SELECT question_id AS survey_log` | 输出列名必须是 survey_log |
| ✗ 只按 answer_rate DESC 排序 | ✓ 加 `question_id ASC` 作为第二排序键 | 平局时取最小 question_id |
| ✗ 整数除法 `3/4 = 0` | ✓ 乘 `1.0` 或 `CAST AS DECIMAL` | 确保浮点除法 |
| ✗ `COUNT(action = 'answer')` | ✓ `SUM(CASE WHEN action = 'answer' THEN 1 ELSE 0 END)` | COUNT 不接受布尔表达式（标准SQL） |
| ✗ 忘记处理 show_count = 0 | ✓ 题目保证 show 先于 answer，但生产环境应加 NULLIF | 防御性编程 |

## 面试追问

**Q1：如果需要返回回答率最高的前 3 个问题，怎么改？**

把 `LIMIT 1` 改为 `LIMIT 3`。如果要求处理并列（第 3 名有多个相同比率），用窗口函数 `DENSE_RANK()` 取 rnk <= 3。

**Q2：如果 SurveyLog 表非常大，如何优化这个查询？**

- 在 `(question_id, action)` 上建复合索引，加速 GROUP BY 和条件聚合
- 如果实时性要求不高，用物化视图预计算每个 question_id 的 answer/show 计数
- 分区表按 timestamp 分区，缩小扫描范围
- 如果只需要 Top-1，可以用堆排序思路，不必全排序

**Q3：如何防御 show_count = 0 导致的除零错误？**

```sql
ORDER BY CASE
    WHEN SUM(CASE WHEN action = 'show' THEN 1 ELSE 0 END) = 0 THEN 0
    ELSE SUM(CASE WHEN action = 'answer' THEN 1 ELSE 0 END) * 1.0
       / SUM(CASE WHEN action = 'show' THEN 1 ELSE 0 END)
END DESC, question_id ASC
```
或用 `NULLIF` 函数：`/ NULLIF(SUM(...), 0)`，除零结果为 NULL，排序时在最后。

## 相关题型

| 题号 | 题目 | 关联点 |
|------|------|--------|
| 574 | Winning Candidate | 同为 GROUP BY + ORDER BY + LIMIT 模式 |
| 580 | Count Student Number in Departments | 条件聚合 + 排序 |
| 586 | Customer Placing the Largest Number of Orders | 聚合排序取最大 |
| 602 | Friend Requests II | 类似的比率计算 |
| 1211 | Queries Quality and Percentage | 条件聚合计算比率的进阶 |
