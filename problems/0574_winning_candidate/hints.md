# 574. Winning Candidate - 当选者

## 核心思路

本题需要从 Vote 表统计每个候选人的得票数，再与 Candidate 表关联获取得票最多者的姓名。核心是 **GROUP BY + ORDER BY + LIMIT** 模式，或用子查询/窗口函数找到最大票数对应的候选人。

## 思维链

1. **理解数据关系**：Vote.candidateId -> Candidate.id，每行 Vote 代表一票
2. **统计票数**：按 candidateId 分组，COUNT(*) 得到每人票数
3. **找最大值**：ORDER BY count DESC LIMIT 1，或用子查询/窗口函数
4. **关联姓名**：JOIN Candidate 表获取 name
5. **边界确认**：题目保证恰好一个赢家，无需处理平局

## 解法概览

| 解法 | 思路 | 适用场景 | 推荐 |
|------|------|----------|------|
| GROUP BY + ORDER BY + LIMIT | 直接统计排序取第一 | 最通用，面试首选 | ⭐ |
| 子查询嵌套 | 先找最大票数的 candidateId，再查 name | 逻辑清晰 | |
| 窗口函数 RANK | 用 RANK() 排名取第一 | 需处理并列时更灵活 | |
| HAVING + ALL | 票数 >= ALL 其他候选人 | 不常用但展示 SQL 功底 | |

## 关键提示

- Vote 表中 candidateId 可能有 NULL（某些投票无效），但本题测试数据保证有效
- 题目明确只有一个赢家，所以 LIMIT 1 是安全的
- LEFT JOIN vs INNER JOIN：如果 Candidate 中有人零票也需考虑，本题用 INNER JOIN 即可
- GROUP BY 是统计聚合的基础，面试中 SQL 聚合题几乎都离不开

## 解法详解

### 解法一：GROUP BY + ORDER BY + LIMIT（推荐）

```sql
-- 思路：统计每个候选人的票数，降序取第一
SELECT c.name
FROM Candidate c
JOIN Vote v ON c.id = v.candidateId
GROUP BY c.id, c.name
ORDER BY COUNT(*) DESC
LIMIT 1;
```

**执行流程**：
1. JOIN 将每张选票关联到候选人
2. GROUP BY 按候选人分组
3. COUNT(*) 统计每组（每个候选人）的票数
4. ORDER BY DESC 按票数降序
5. LIMIT 1 取票数最多的一个

### 解法二：子查询嵌套

```sql
-- 思路：先在子查询中找到得票最多的 candidateId，再查 name
SELECT name
FROM Candidate
WHERE id = (
    SELECT candidateId
    FROM Vote
    GROUP BY candidateId
    ORDER BY COUNT(*) DESC
    LIMIT 1
);
```

**执行流程**：
1. 内层子查询：Vote 按 candidateId 分组，统计票数，取最大者的 id
2. 外层查询：用该 id 在 Candidate 表中查询 name

### 解法三：窗口函数

```sql
-- 思路：用 DENSE_RANK 排名，适合需处理并列的场景
SELECT name
FROM (
    SELECT c.name,
           DENSE_RANK() OVER (ORDER BY COUNT(*) DESC) AS rnk
    FROM Candidate c
    JOIN Vote v ON c.id = v.candidateId
    GROUP BY c.id, c.name
) ranked
WHERE rnk = 1;
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|----------|----------|------|
| ✗ `SELECT name, COUNT(*) FROM Vote GROUP BY candidateId` | ✓ 需要 JOIN Candidate 获取 name | 单独查 Vote 无法得到 name |
| ✗ `WHERE COUNT(*) = MAX(COUNT(*))` | ✓ 用 ORDER BY + LIMIT 或子查询 | 聚合函数不能直接嵌套 |
| ✗ `GROUP BY name` 而非 `GROUP BY c.id` | ✓ `GROUP BY c.id, c.name` | 不同 id 可能同名，应按主键分组 |
| ✗ 忘记 JOIN 直接 WHERE id IN (SELECT ...) | ✓ 确保子查询返回正确的 candidateId | 逻辑层次要清晰 |

## 面试追问

**Q1：如果可能有多个候选人票数并列最高，如何返回所有赢家？**

用窗口函数 `DENSE_RANK()` 代替 `LIMIT 1`：
```sql
SELECT name FROM (
    SELECT c.name,
           DENSE_RANK() OVER (ORDER BY COUNT(*) DESC) AS rnk
    FROM Candidate c JOIN Vote v ON c.id = v.candidateId
    GROUP BY c.id, c.name
) t WHERE rnk = 1;
```

**Q2：如果 Vote 表非常大（数亿行），如何优化查询性能？**

- 在 `Vote(candidateId)` 上建索引，加速 GROUP BY 聚合
- 使用近似计算（如 HyperLogLog）做初步筛选
- 分区表按时间段分区，缩小扫描范围
- 物化视图预聚合票数

**Q3：如果需要同时返回候选人的名字和具体得票数，SQL 怎么改？**

```sql
SELECT c.name, COUNT(*) AS vote_count
FROM Candidate c
JOIN Vote v ON c.id = v.candidateId
GROUP BY c.id, c.name
ORDER BY vote_count DESC
LIMIT 1;
```

## 相关题型

| 题号 | 题目 | 关联点 |
|------|------|--------|
| 176 | Second Highest Salary | ORDER BY + LIMIT + OFFSET 取第 N 名 |
| 177 | Nth Highest Salary | 通用化的排名问题 |
| 184 | Department Highest Salary | 分组后取最大值的经典模式 |
| 185 | Department Top Three Salaries | 窗口函数 DENSE_RANK 的应用 |
| 586 | Customer Placing the Largest Number of Orders | 同类聚合 + 排序 + LIMIT 模式 |
