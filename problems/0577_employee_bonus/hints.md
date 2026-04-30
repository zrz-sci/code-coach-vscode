# 577. Employee Bonus - 员工奖金

## 核心思路

找出奖金少于 1000 或没有奖金的员工。核心是 **LEFT JOIN** 保留所有员工（包括没有奖金记录的），然后用 WHERE 过滤 `bonus < 1000 OR bonus IS NULL`。这是 LEFT JOIN + NULL 处理的经典入门题。

## 思维链

1. **理解需求**：两类员工需要返回：奖金 < 1000 的，和完全没有奖金的
2. **识别 JOIN 类型**：不是所有员工都在 Bonus 表中有记录，所以必须用 LEFT JOIN
3. **LEFT JOIN 结果**：没有奖金的员工的 bonus 列值为 NULL
4. **过滤条件**：`bonus < 1000` 捕获有奖金但少于 1000 的；`IS NULL` 捕获没有奖金的
5. **注意 NULL 比较**：`NULL < 1000` 的结果是 NULL（非 TRUE），所以必须显式写 `IS NULL`

## 解法概览

| 解法 | 思路 | 适用场景 | 推荐 |
|------|------|----------|------|
| LEFT JOIN + OR | 分别处理两种情况 | 最直观，面试标准答案 | ⭐ |
| LEFT JOIN + IFNULL/COALESCE | 将 NULL 转为 0 后统一比较 | 条件合并，更简洁 | |
| NOT IN 子查询 | 排除奖金 >= 1000 的员工 | 思路反转，但性能可能差 | |

## 关键提示

- **LEFT JOIN vs INNER JOIN**：INNER JOIN 会丢掉没有奖金的员工，这恰恰是我们需要保留的
- **NULL 的比较陷阱**：`NULL < 1000` 结果是 UNKNOWN，不会被 WHERE 选中，必须用 `IS NULL` 单独处理
- **IFNULL/COALESCE 技巧**：`IFNULL(bonus, 0) < 1000` 可以将两个条件合并为一个，因为 NULL 被替换为 0，0 < 1000 为 TRUE
- 输出中没有奖金的员工 bonus 显示为 null，不是 0

## 解法详解

### 解法一：LEFT JOIN + OR（推荐）

```sql
-- 思路：LEFT JOIN 保留所有员工，WHERE 过滤两种情况
SELECT e.name, b.bonus
FROM Employee e
LEFT JOIN Bonus b ON e.empId = b.empId
WHERE b.bonus < 1000 OR b.bonus IS NULL;
```

**执行流程**：
1. LEFT JOIN：每个员工都保留，匹配不到 Bonus 的 bonus 列为 NULL
2. WHERE 过滤：bonus < 1000（有奖金且少于 1000）OR bonus IS NULL（无奖金）
3. 输出 name 和 bonus（NULL 保持为 null）

### 解法二：IFNULL 合并条件

```sql
-- 思路：IFNULL 将 NULL 替换为 0，统一用 < 1000 过滤
SELECT e.name, b.bonus
FROM Employee e
LEFT JOIN Bonus b ON e.empId = b.empId
WHERE IFNULL(b.bonus, 0) < 1000;
```

**执行流程**：
1. LEFT JOIN 同上
2. IFNULL(b.bonus, 0)：如果 bonus 是 NULL 就替换为 0
3. 0 < 1000 为 TRUE，所以无奖金的员工也会被选中

### 解法三：COALESCE（标准 SQL，跨数据库兼容）

```sql
-- 思路：COALESCE 是 IFNULL 的标准 SQL 替代
SELECT e.name, b.bonus
FROM Employee e
LEFT JOIN Bonus b ON e.empId = b.empId
WHERE COALESCE(b.bonus, 0) < 1000;
```

### 解法四：NOT IN 反向排除

```sql
-- 思路：找出奖金 >= 1000 的员工 id，排除它们
SELECT e.name, b.bonus
FROM Employee e
LEFT JOIN Bonus b ON e.empId = b.empId
WHERE e.empId NOT IN (
    SELECT empId FROM Bonus WHERE bonus >= 1000
);
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|----------|----------|------|
| ✗ `INNER JOIN` 代替 `LEFT JOIN` | ✓ `LEFT JOIN` | INNER JOIN 丢弃无奖金的员工 |
| ✗ `WHERE bonus < 1000`（不加 IS NULL） | ✓ `WHERE bonus < 1000 OR bonus IS NULL` | NULL < 1000 结果是 UNKNOWN，不会被选中 |
| ✗ `WHERE bonus < 1000 OR bonus = NULL` | ✓ `WHERE bonus < 1000 OR bonus IS NULL` | 不能用 = 比较 NULL，必须用 IS |
| ✗ `WHERE bonus IS NULL OR bonus < 1000` | ✓ 两种顺序都正确 | OR 无短路优化，但逻辑等价 |

## 面试追问

**Q1：IFNULL 和 COALESCE 有什么区别？什么时候用哪个？**

- `IFNULL(a, b)` 是 MySQL 特有函数，只接受两个参数
- `COALESCE(a, b, c, ...)` 是 SQL 标准，接受任意多个参数，返回第一个非 NULL 值
- 跨数据库兼容性要求时用 COALESCE；MySQL 环境下 IFNULL 更简洁

**Q2：如果 Bonus 表中一个员工有多条记录（多次奖金），如何返回奖金总和 < 1000 的？**

```sql
SELECT e.name, SUM(b.bonus) AS bonus
FROM Employee e
LEFT JOIN Bonus b ON e.empId = b.empId
GROUP BY e.empId, e.name
HAVING SUM(b.bonus) < 1000 OR SUM(b.bonus) IS NULL;
```

**Q3：LEFT JOIN 和 RIGHT JOIN 能互换吗？什么时候用哪个？**

可以互换，只需调换表的位置。例如 `A LEFT JOIN B` 等价于 `B RIGHT JOIN A`。实际开发中 LEFT JOIN 更常用，因为符合"以主表为基准"的阅读习惯。RIGHT JOIN 通常不推荐使用。

## 相关题型

| 题号 | 题目 | 关联点 |
|------|------|--------|
| 175 | Combine Two Tables | LEFT JOIN 入门，保留所有 Person |
| 181 | Employees Earning More Than Their Managers | 自连接 + 条件过滤 |
| 183 | Customers Who Never Ordered | LEFT JOIN + IS NULL 找无匹配记录 |
| 196 | Delete Duplicate Emails | JOIN + 数据修改 |
| 1075 | Project Employees I | LEFT JOIN + 聚合的组合 |
