# 182. 查找重复的电子邮箱 (Easy)

## 核心思路

按 email 分组，筛选出现次数 > 1 的记录。本质是 **GROUP BY + HAVING** 的基础应用。

## 思维链

1. 需要找"重复"的 email → 同一个值出现多次
2. 如何统计出现次数？→ GROUP BY email + COUNT(*)
3. 如何筛选"重复"？→ HAVING COUNT(*) > 1
4. 还有别的方法吗？→ 自连接：同一张表 JOIN 自己，找 email 相同但 id 不同的行

## 解法概览

| 解法 | 思路 | 时间复杂度 | 推荐度 |
|------|------|-----------|--------|
| GROUP BY + HAVING | 分组后筛选计数>1 | O(n) | ⭐⭐⭐ |
| 自连接 (Self Join) | 表与自身连接找重复 | O(n²) | ⭐⭐ |
| 子查询 + IN | 子查询找重复email | O(n) | ⭐⭐ |

## 关键提示

1. `HAVING` 是对分组后的结果进行过滤，而 `WHERE` 是在分组前过滤
2. 自连接时注意要加 `DISTINCT`，否则同一 email 可能出现多次
3. 题目保证 email 不为 NULL，无需处理空值

## 解法详解

### 解法一：GROUP BY + HAVING（推荐）
```sql
SELECT email AS Email
FROM Person
GROUP BY email
HAVING COUNT(*) > 1;
```

### 解法二：自连接
```sql
SELECT DISTINCT a.email AS Email
FROM Person a
JOIN Person b ON a.email = b.email AND a.id != b.id;
```

### 解法三：子查询
```sql
SELECT DISTINCT email AS Email
FROM Person
WHERE email IN (
    SELECT email
    FROM Person
    GROUP BY email
    HAVING COUNT(*) > 1
);
```

## 易错点

- ✗ 忘记 `DISTINCT`（自连接解法中，重复 email 会出现多次）
  → ✓ 自连接时务必加 `SELECT DISTINCT`

- ✗ 用 `WHERE COUNT(*) > 1`（语法错误，WHERE 不能跟聚合函数）
  → ✓ 应使用 `HAVING COUNT(*) > 1`

- ✗ 输出列名写成 `email` 而非 `Email`
  → ✓ 注意题目要求的输出列名，使用 `AS Email`

## 面试追问

**Q1：GROUP BY 和 HAVING 的执行顺序是什么？**
→ SQL 执行顺序：FROM → WHERE → GROUP BY → HAVING → SELECT → ORDER BY。HAVING 在 GROUP BY 之后执行。

**Q2：如果要找出现恰好 2 次的 email 怎么办？**
→ 改为 `HAVING COUNT(*) = 2`

**Q3：如果数据量非常大（亿级），如何优化查询？**
→ 对 email 列建索引；考虑分区表；如果是实时场景可用 Redis 等缓存做计数。

## 相关题型

- [196. 删除重复的电子邮箱](https://leetcode.com/problems/delete-duplicate-emails/) - 删除重复记录
- [197. 上升的温度](https://leetcode.com/problems/rising-temperature/) - 自连接应用
- [176. 第二高的薪水](https://leetcode.com/problems/second-highest-salary/) - 聚合函数
