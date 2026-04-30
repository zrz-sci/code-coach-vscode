# 584. 寻找用户推荐人 (Find Customer Referee)

**难度**: Easy | **标签**: Database | **链接**: [LeetCode 584](https://leetcode.com/problems/find-customer-referee/)

## 核心思路

找出**不是由 id=2 推荐**的客户。看似简单，但核心陷阱在于 **NULL 值的处理**：`referee_id` 为 NULL 的客户没有推荐人，他们也应该被包含在结果中。SQL 中 `NULL != 2` 的结果是 **UNKNOWN**（不是 TRUE），WHERE 子句只保留结果为 TRUE 的行，所以单纯写 `WHERE referee_id != 2` 会丢失 NULL 行。

## 思维链

1. **理解需求**: 找出 referee_id 不等于 2 **或** referee_id 为 NULL 的客户
2. **识别 NULL 陷阱**: `WHERE referee_id != 2` 无法匹配 referee_id IS NULL 的行
3. **SQL 三值逻辑**: 任何与 NULL 的比较返回 UNKNOWN，WHERE 过滤掉 UNKNOWN 和 FALSE
4. **正确方案**: `WHERE referee_id != 2 OR referee_id IS NULL`，或使用 IFNULL/COALESCE

## 解法概览

| 解法 | 思路 | 复杂度 | 推荐 |
|------|------|--------|------|
| OR IS NULL | `referee_id != 2 OR referee_id IS NULL` | O(n) | ⭐⭐⭐⭐⭐ |
| IFNULL | `IFNULL(referee_id, 0) != 2` | O(n) | ⭐⭐⭐⭐ |
| COALESCE | `COALESCE(referee_id, 0) != 2` | O(n) | ⭐⭐⭐⭐ |
| NOT IN + 子查询 | 不推荐，仅展示 | O(n) | ⭐ |
| <=> 安全等于 | MySQL特有 `NOT (referee_id <=> 2)` | O(n) | ⭐⭐⭐ |

## 关键提示

1. **SQL 三值逻辑**: TRUE / FALSE / UNKNOWN。NULL 参与的比较运算结果都是 UNKNOWN
2. **WHERE 的过滤行为**: 只保留结果为 TRUE 的行，FALSE 和 UNKNOWN 都被过滤
3. **`<=>` 是 MySQL 的 NULL-safe 等于运算符**: `NULL <=> NULL` 返回 TRUE，`NULL <=> 2` 返回 FALSE（标准SQL中没有这个运算符）
4. **IFNULL 的选择值**: `IFNULL(referee_id, 0)` 中的0可以是任何不等于2的值

## 解法详解

### 解法一：OR IS NULL（最标准）

```sql
SELECT name
FROM Customer
WHERE referee_id != 2 OR referee_id IS NULL;
```

**执行逻辑**：
| id | name | referee_id | `!=2` | `IS NULL` | OR结果 | 保留? |
|----|------|-----------|-------|-----------|--------|-------|
| 1 | Will | NULL | UNKNOWN | TRUE | TRUE | YES |
| 2 | Jane | NULL | UNKNOWN | TRUE | TRUE | YES |
| 3 | Alex | 2 | FALSE | FALSE | FALSE | NO |
| 4 | Bill | NULL | UNKNOWN | TRUE | TRUE | YES |
| 5 | Zack | 1 | TRUE | FALSE | TRUE | YES |
| 6 | Mark | 2 | FALSE | FALSE | FALSE | NO |

### 解法二：IFNULL / COALESCE

```sql
-- MySQL
SELECT name
FROM Customer
WHERE IFNULL(referee_id, 0) != 2;

-- 标准 SQL (兼容所有数据库)
SELECT name
FROM Customer
WHERE COALESCE(referee_id, 0) != 2;
```

**思路**：将 NULL 替换为一个确定不等于2的值（如0），然后正常比较。

### 解法三：MySQL NULL-safe 等于

```sql
SELECT name
FROM Customer
WHERE NOT (referee_id <=> 2);
```

**思路**：`<=>` 是 MySQL 特有的 NULL-safe 比较，`NULL <=> 2` 返回 0(FALSE) 而非 NULL。NOT FALSE = TRUE，所以 NULL 行被保留。

## 易错点

| ✗ 错误写法 | ✓ 正确写法 | 说明 |
|-----------|-----------|------|
| `WHERE referee_id != 2` | `WHERE referee_id != 2 OR referee_id IS NULL` | 遗漏 NULL 行！这是本题最核心的考点 |
| `WHERE referee_id <> 2` | 同上，加 `OR referee_id IS NULL` | `<>` 和 `!=` 等价，同样不处理 NULL |
| `WHERE referee_id IS NOT 2` | `WHERE referee_id != 2 OR referee_id IS NULL` | `IS NOT` 只能跟 NULL 搭配，不能跟具体值 |
| `WHERE NOT referee_id = 2` | 同上 | NOT UNKNOWN = UNKNOWN，NULL 行依然被过滤 |
| `IFNULL(referee_id, 2) != 2` | `IFNULL(referee_id, 0) != 2` | 替代值不能等于要排除的值，否则 NULL 行会被错误过滤 |

## 面试追问

**Q1: SQL 的三值逻辑是什么？请详细解释。**

A1: SQL 采用三值逻辑：TRUE / FALSE / UNKNOWN。任何值与 NULL 的比较（=, !=, <, >, <=, >=）结果都是 UNKNOWN。逻辑运算规则：
- `TRUE AND UNKNOWN = UNKNOWN`
- `FALSE AND UNKNOWN = FALSE`
- `TRUE OR UNKNOWN = TRUE`
- `FALSE OR UNKNOWN = UNKNOWN`
- `NOT UNKNOWN = UNKNOWN`

WHERE 子句只保留 TRUE 的行。HAVING 也是同理。这就是为什么 `WHERE col != 2` 会丢失 NULL 行。

**Q2: 除了 IS NULL，还有哪些方法可以检测 NULL？**

A2: (1) `IS NULL` / `IS NOT NULL` 是标准方法；(2) `IFNULL(col, default)` / `COALESCE(col, default)` 替换 NULL；(3) `<=>` (MySQL) 是 NULL-safe 等于；(4) `ISNULL(col)` (MySQL函数) 返回1/0；(5) `NULLIF(col, val)` 如果 col=val 返回 NULL，否则返回 col。

**Q3: NOT IN 子查询中如果包含 NULL 会怎样？**

A3: 这是经典陷阱！`WHERE col NOT IN (1, 2, NULL)` 等价于 `col != 1 AND col != 2 AND col != NULL`。最后一项结果是 UNKNOWN，AND UNKNOWN 使整个表达式永远不为 TRUE，查询返回空集。所以使用 NOT IN 时必须确保子查询结果集不含 NULL。

## 相关题型

- [183. Customers Who Never Order](https://leetcode.com/problems/customers-who-never-order/) - NULL/缺失值判断
- [595. Big Countries](https://leetcode.com/problems/big-countries/) - 基础 WHERE 过滤
- [1068. Product Sales Analysis I](https://leetcode.com/problems/product-sales-analysis-i/) - 基础 JOIN
- [1148. Article Views I](https://leetcode.com/problems/article-views-i/) - 条件过滤
- [1378. Replace Employee ID With The Unique Identifier](https://leetcode.com/problems/replace-employee-id-with-the-unique-identifier/) - LEFT JOIN 处理 NULL
