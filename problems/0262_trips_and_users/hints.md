# 262. Trips and Users (行程和用户)

## 核心思路

这是一道经典的 SQL 多表关联 + 条件聚合题。核心在于：
1. **过滤被封禁用户**: 乘客和司机都不能被封禁
2. **按日期分组**: 对每一天分别计算取消率
3. **条件聚合**: 用 SUM(CASE WHEN ...) / COUNT(*) 计算取消率

## 思维链

1. **理解取消率定义**: 取消订单数 / 总订单数（仅算未被封禁用户的订单）
2. **过滤封禁用户**: JOIN Users 表两次（一次 client，一次 driver），或用 NOT IN/NOT EXISTS 子查询
3. **限定日期范围**: WHERE request_at BETWEEN '2013-10-01' AND '2013-10-03'
4. **条件聚合计算取消率**: SUM(status != 'completed') / COUNT(*) 或 AVG(status != 'completed')
5. **四舍五入**: ROUND(..., 2) 保留两位小数
6. **列名别名**: Day 和 `Cancellation Rate` 需匹配输出格式

## 解法概览

| 解法 | 方式 | 特点 |
|------|------|------|
| JOIN 过滤 | 双 JOIN Users 表 | ⭐ 最直观，面试首选 |
| NOT IN 子查询 | WHERE client_id NOT IN (...) | 简洁但有 NULL 陷阱 |
| NOT EXISTS | 相关子查询 | 语义清晰，适合大数据 |

## 关键提示

- 必须同时排除被封禁的 **乘客** 和 **司机**（JOIN 两次 Users 表）
- `status != 'completed'` 包含了 `cancelled_by_driver` 和 `cancelled_by_client` 两种取消情况
- ROUND 函数要注意数据类型，MySQL 中整数除法需要乘 1.0 或用 CAST
- NOT IN 子查询里如果有 NULL 会导致全部被过滤，要注意安全

## 解法详解

### 解法1: 双 JOIN 过滤 ⭐

```sql
SELECT
    t.request_at AS Day,
    ROUND(
        SUM(CASE WHEN t.status != 'completed' THEN 1 ELSE 0 END) / COUNT(*),
        2
    ) AS 'Cancellation Rate'
FROM Trips t
JOIN Users u1 ON t.client_id = u1.users_id AND u1.banned = 'No'
JOIN Users u2 ON t.driver_id = u2.users_id AND u2.banned = 'No'
WHERE t.request_at BETWEEN '2013-10-01' AND '2013-10-03'
GROUP BY t.request_at;
```

### 解法2: NOT IN 子查询

```sql
SELECT
    request_at AS Day,
    ROUND(
        SUM(IF(status != 'completed', 1, 0)) / COUNT(*),
        2
    ) AS 'Cancellation Rate'
FROM Trips
WHERE client_id NOT IN (SELECT users_id FROM Users WHERE banned = 'Yes')
  AND driver_id NOT IN (SELECT users_id FROM Users WHERE banned = 'Yes')
  AND request_at BETWEEN '2013-10-01' AND '2013-10-03'
GROUP BY request_at;
```

## 易错点

| ✗ 错误写法 | ✓ 正确写法 | 说明 |
|-----------|-----------|------|
| 只过滤 client 封禁 | 同时过滤 client 和 driver | 题目要求双方都未封禁 |
| SUM(...) / COUNT(*) 整数除法 | MySQL 中 SUM 返回的已是浮点 | 但其他 DB 需注意 CAST |
| NOT IN (...含 NULL) | 确保子查询不返回 NULL | NULL 会让整个 NOT IN 失效 |
| 忘记 GROUP BY | GROUP BY request_at | 需要按天分组 |

## 面试追问

- **Q1**: 如果 Users 表中 banned 列可能有 NULL，怎么处理？
  → 用 `COALESCE(banned, 'No') = 'No'` 或 `banned IS NULL OR banned = 'No'`
- **Q2**: 如果日期范围要动态传入参数怎么办？
  → 用参数化查询 `WHERE request_at BETWEEN ? AND ?`
- **Q3**: 如果某天没有任何行程但也要显示 0.00，怎么做？
  → 需要先生成日期序列（如用 CTE 或日历表），再 LEFT JOIN

## 相关题型

- [1084. Sales Analysis III](https://leetcode.com/problems/sales-analysis-iii/) - 条件过滤 + 分组
- [1193. Monthly Transactions I](https://leetcode.com/problems/monthly-transactions-i/) - 条件聚合
- [1174. Immediate Food Delivery II](https://leetcode.com/problems/immediate-food-delivery-ii/) - 比率计算
- [1934. Confirmation Rate](https://leetcode.com/problems/confirmation-rate/) - 类似取消率
