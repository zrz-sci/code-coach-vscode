# 607. 销售员

## 核心思路

找出**没有**向 "RED" 公司销售过的所有销售员。这是一个典型的**否定查询**：先找出向 RED 公司销售过的销售员，再用 NOT IN / NOT EXISTS / LEFT JOIN 取反，返回不在其中的销售员。

## 思维链

1. **定位目标公司** — Company 表中找到 name = 'RED' 的 com_id
2. **找出关联的销售员** — Orders 表中关联 RED 公司的 com_id，得到对应的 sales_id 集合
3. **取反** — SalesPerson 表中排除上述 sales_id 集合的所有人
4. **输出名字** — 只需返回 name 列

## 解法概览

| 解法 | 思路 | 时间复杂度 | 推荐度 |
|------|------|-----------|--------|
| NOT IN 子查询 | 子查询找 RED 关联的 sales_id，主查询排除 | O(n*m) | ⭐⭐⭐⭐⭐ |
| NOT EXISTS 相关子查询 | EXISTS 检查每个销售员是否有 RED 订单 | O(n*m) | ⭐⭐⭐⭐ |
| LEFT JOIN + IS NULL | 左连接后筛选无匹配记录 | O(n+m) | ⭐⭐⭐⭐ |

## 关键提示

- **三表关联** — SalesPerson <-> Orders <-> Company 通过外键关联
- **NOT IN 的 NULL 陷阱** — 如果子查询返回 NULL，NOT IN 会返回空结果；此题数据保证无 NULL，但面试时应提及
- **直接 JOIN Company 到 Orders** — 不需要 SalesPerson 参与 RED 公司的查找
- **否定查询模式** — "找不满足条件的"通常用 NOT IN / NOT EXISTS / LEFT JOIN + IS NULL

## 解法详解

### 解法一：NOT IN 子查询（推荐，最直观）

```sql
SELECT name
FROM SalesPerson
WHERE sales_id NOT IN (
    SELECT o.sales_id
    FROM Orders o
    JOIN Company c ON o.com_id = c.com_id
    WHERE c.name = 'RED'
);
```

**执行逻辑：**
- 内层查询：Orders JOIN Company 找出所有向 RED 公司销售过的 sales_id
- 外层查询：SalesPerson 中排除这些 sales_id
- 对于示例：Orders 3, 4 的 com_id = 1（RED），sales_id = 1, 4，所以排除 John 和 Pam

### 解法二：NOT EXISTS 相关子查询

```sql
SELECT sp.name
FROM SalesPerson sp
WHERE NOT EXISTS (
    SELECT 1
    FROM Orders o
    JOIN Company c ON o.com_id = c.com_id
    WHERE c.name = 'RED'
      AND o.sales_id = sp.sales_id
);
```

**执行逻辑：**
- 对 SalesPerson 的每一行，检查是否存在该销售员向 RED 公司的订单
- NOT EXISTS 返回 true 表示该销售员没有 RED 公司的订单
- 与 NOT IN 功能等价，但对 NULL 更安全

### 解法三：LEFT JOIN + IS NULL

```sql
SELECT sp.name
FROM SalesPerson sp
LEFT JOIN (
    SELECT DISTINCT o.sales_id
    FROM Orders o
    JOIN Company c ON o.com_id = c.com_id
    WHERE c.name = 'RED'
) red_sales ON sp.sales_id = red_sales.sales_id
WHERE red_sales.sales_id IS NULL;
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ 直接 JOIN 三表然后 WHERE name != 'RED' | ✓ 用 NOT IN/NOT EXISTS 子查询 | JOIN 会漏掉没有任何订单的销售员 |
| ✗ WHERE company.name <> 'RED' | ✓ NOT IN (子查询 WHERE name = 'RED') | <> 只过滤单条记录，同一人可能还有 RED 订单 |
| ✗ NOT IN 不考虑 NULL | ✓ 确保子查询不返回 NULL 或改用 NOT EXISTS | NOT IN 子查询含 NULL 时返回空集 |
| ✗ 返回 sales_id | ✓ 返回 name | 题目要求输出销售员姓名 |

## 面试追问

**Q1: NOT IN、NOT EXISTS、LEFT JOIN + IS NULL 性能差异？**
> 在 MySQL 中：NOT EXISTS 通常最优，遇到匹配行即短路退出；LEFT JOIN + IS NULL 性能接近；NOT IN 需要完整构建子查询结果集。对于大数据集，NOT EXISTS 和 LEFT JOIN 通常优于 NOT IN。PostgreSQL 的优化器在三者之间差异较小。

**Q2: 如果 SalesPerson 表中有同名销售员怎么办？**
> 本题按 sales_id 判断，同名不影响逻辑。但如果要按名字去重，需要加 DISTINCT 或确认 name 列是否有唯一约束。

**Q3: 如果要找出既向 RED 销售过又向 GREEN 销售过的销售员？**
> 使用两个子查询取交集：`WHERE sales_id IN (SELECT ... WHERE name = 'RED') AND sales_id IN (SELECT ... WHERE name = 'GREEN')`。或者用 GROUP BY + HAVING COUNT(DISTINCT c.name) = 2 配合 WHERE c.name IN ('RED', 'GREEN')。

## 相关题型

- [1084. 销售分析 III](../1084_sales_analysis_iii/) — 类似的否定查询模式
- [1075. 项目员工 I](../1075_project_employees_i/) — 多表 JOIN + 聚合
- [183. 从不订购的客户](../0183_customers_who_never_order/) — 经典 NOT IN / LEFT JOIN 否定查询
- [1581. 进店却未进行过交易的顾客](../1581_customer_who_visited_but_did_not_make_any_transactions/) — LEFT JOIN + IS NULL 模式
