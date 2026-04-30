# 183. 从不订购的客户 (Easy)

## 核心思路

找出"不存在于订单表中"的客户。本质是 **反连接 (Anti-Join)** —— 从 A 表中找出在 B 表中没有匹配的行。

## 思维链

1. 需要找"从不订购"的客户 → Customers 表中的人不在 Orders 表中
2. 如何表达"不在"？→ NOT IN / NOT EXISTS / LEFT JOIN + IS NULL
3. 两表关联键是什么？→ Customers.id = Orders.customerId
4. 输出什么？→ 客户名字，别名为 Customers

## 解法概览

| 解法 | 思路 | 时间复杂度 | 推荐度 |
|------|------|-----------|--------|
| LEFT JOIN + IS NULL | 左连接后取 NULL 行 | O(n+m) | ⭐⭐⭐ |
| NOT IN 子查询 | 排除在订单中出现的ID | O(n*m) | ⭐⭐ |
| NOT EXISTS 子查询 | 相关子查询检查存在性 | O(n*m) | ⭐⭐⭐ |

## 关键提示

1. LEFT JOIN 是实现反连接最常用、性能最优的方式
2. NOT IN 有空值陷阱：如果子查询返回 NULL，整个 NOT IN 结果为空
3. NOT EXISTS 语义清晰，且自动处理 NULL 值，面试推荐

## 解法详解

### 解法一：LEFT JOIN + IS NULL（推荐）
```sql
SELECT c.name AS Customers
FROM Customers c
LEFT JOIN Orders o ON c.id = o.customerId
WHERE o.id IS NULL;
```

### 解法二：NOT IN
```sql
SELECT name AS Customers
FROM Customers
WHERE id NOT IN (
    SELECT customerId FROM Orders
);
```

### 解法三：NOT EXISTS
```sql
SELECT name AS Customers
FROM Customers c
WHERE NOT EXISTS (
    SELECT 1 FROM Orders o WHERE o.customerId = c.id
);
```

## 易错点

- ✗ LEFT JOIN 后用 `WHERE o.customerId IS NULL` 但 customerId 本身允许 NULL
  → ✓ 最好用主键 `o.id IS NULL` 判断，因为主键不可能为 NULL

- ✗ NOT IN 子查询中 customerId 包含 NULL 值，导致结果为空
  → ✓ 加过滤 `WHERE customerId IS NOT NULL`，或改用 NOT EXISTS

- ✗ 忘记给输出列起别名 `Customers`
  → ✓ `SELECT name AS Customers`

## 面试追问

**Q1：LEFT JOIN + IS NULL 和 NOT EXISTS 哪个性能更好？**
→ 在大多数数据库引擎中两者性能接近，MySQL 优化器通常将 NOT EXISTS 改写为反连接。LEFT JOIN 在有索引时通常略快。

**Q2：如果 Orders 表的 customerId 可能为 NULL，三种方案有何区别？**
→ NOT IN 会失效（返回空集）；LEFT JOIN 和 NOT EXISTS 不受影响。这是 NOT IN 的经典陷阱。

**Q3：如何扩展为"找出只下过一次订单的客户"？**
→ 改用 INNER JOIN + GROUP BY + HAVING COUNT(*) = 1

## 相关题型

- [175. 组合两个表](https://leetcode.com/problems/combine-two-tables/) - 基础 LEFT JOIN
- [182. 查找重复的电子邮箱](https://leetcode.com/problems/duplicate-emails/) - GROUP BY
- [1581. 进店却未进行过交易的顾客](https://leetcode.com/problems/customer-who-visited-but-did-not-make-any-transactions/) - 反连接变体
