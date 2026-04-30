# 627. Swap Sex of Employees

## 核心思路

使用 SQL 的 `UPDATE` 语句配合条件表达式（`CASE WHEN` 或 `IF`），在**单条语句**中完成 `sex` 列的值互换：`'m'` 变 `'f'`，`'f'` 变 `'m'`。关键在于理解 SQL 的 UPDATE 是**同时对所有行生效**的语义——不存在"先改 m 再改 f 导致覆盖"的问题。

## 思维链

1. **审题**：必须用单条 UPDATE，不能用 SELECT 或临时表
2. **分析**：sex 列只有两个 ENUM 值 `'m'` 和 `'f'`，需要互换
3. **方案选择**：
   - 方案 A：`CASE WHEN` 条件分支（SQL 标准，通用）
   - 方案 B：`IF()` 函数（MySQL 专用）
   - 方案 C：ASCII 码运算技巧
   - 方案 D：异或 / 字符替换技巧
4. **执行**：UPDATE 语句对所有行同时求值再赋值，不需要担心顺序问题
5. **验证**：原来 m 的行变为 f，原来 f 的行变为 m

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐度 |
|------|-----------|-----------|--------|
| CASE WHEN | O(n) | O(1) | ⭐⭐⭐⭐⭐ |
| IF 函数 | O(n) | O(1) | ⭐⭐⭐⭐ |
| ASCII 运算 | O(n) | O(1) | ⭐⭐⭐ |
| XOR 技巧 | O(n) | O(1) | ⭐⭐ |

## 关键提示

1. SQL 的 `UPDATE SET` 在求值阶段使用**旧值**，赋值阶段写入**新值**，不会出现"改了 m 后 f 找不到"的情况
2. `CASE WHEN` 是最通用、最易读的条件更新方式，面试首选
3. `IF(condition, true_val, false_val)` 是 MySQL 特有的简写，PostgreSQL 不支持
4. ASCII 技巧：`CHAR(ASCII('m') + ASCII('f') - ASCII(sex))` 利用了 m=109, f=102 的数学关系
5. 题目要求不能写 SELECT 语句——是指不能用独立 SELECT，子查询中的 SELECT 在本题不需要

## 解法详解

### 解法一：CASE WHEN（推荐）

**思路**：对每一行，判断 sex 当前值，返回相反值。

```sql
UPDATE Salary
SET sex = CASE
    WHEN sex = 'm' THEN 'f'
    WHEN sex = 'f' THEN 'm'
END;
```

**要点**：
- `CASE WHEN ... THEN ... END` 是 SQL 标准语法，所有数据库通用
- 由于 ENUM 只有两个值，也可以简化为 `CASE WHEN sex = 'm' THEN 'f' ELSE 'm' END`
- UPDATE 语义保证所有行用旧值求值后统一更新

### 解法二：IF 函数

**思路**：MySQL 的 `IF()` 函数，三元条件表达式。

```sql
UPDATE Salary
SET sex = IF(sex = 'm', 'f', 'm');
```

**要点**：
- 比 CASE 更简洁，但 MySQL 专用
- PostgreSQL / SQL Server 不支持 IF()，面试时需确认数据库类型

### 解法三：ASCII 运算

**思路**：利用 ASCII 码之和减去当前字符得到目标字符。

```sql
UPDATE Salary
SET sex = CHAR(ASCII('m') + ASCII('f') - ASCII(sex));
```

**要点**：
- `ASCII('m') = 109`，`ASCII('f') = 102`，和为 211
- 当 sex='m' 时：211 - 109 = 102 = 'f'
- 当 sex='f' 时：211 - 102 = 109 = 'm'
- 这是经典的"两数交换不用临时变量"思路在 SQL 中的应用

### 解法四：XOR / REPLACE 技巧

**思路**：利用位运算实现交换。

```sql
UPDATE Salary
SET sex = CHAR(ASCII(sex) ^ ASCII('m') ^ ASCII('f'));
```

**要点**：XOR 的自反性质 `a ^ b ^ a = b`，`ASCII('m') ^ ASCII('f') = 11`

## 易错点

1. **写成两条 UPDATE**：先 `UPDATE ... SET sex='f' WHERE sex='m'`，再反过来更新——这会导致第一条把所有 m 都变成 f，第二条又全部变回来
2. **忘记 CASE WHEN 的 END 关键字**：CASE 表达式必须以 END 结尾
3. **使用 SELECT 语句**：题目明确要求只能写 UPDATE，不能写 SELECT
4. **ENUM 类型假设**：虽然题目说只有 'm' 和 'f'，但好的代码应该用完整 CASE 而非假设"非 m 即 f"
5. **数据库兼容性**：IF() 是 MySQL 专用，面试时使用 CASE WHEN 更安全

## 面试追问

**Q1: 如果不是两个值而是多个值（比如 m/f/other）的循环替换，怎么做？**

使用 CASE WHEN 链：
```sql
UPDATE Salary
SET sex = CASE
    WHEN sex = 'm' THEN 'f'
    WHEN sex = 'f' THEN 'other'
    WHEN sex = 'other' THEN 'm'
END;
```
核心思想不变：SQL UPDATE 对所有行使用旧值求值，不会出现级联覆盖。

**Q2: 如果要同时交换 sex 和 salary（高低薪互换），如何实现？**

只需在同一 UPDATE 中 SET 多个列即可：
```sql
UPDATE Salary
SET sex = CASE WHEN sex = 'm' THEN 'f' ELSE 'm' END,
    salary = (SELECT MAX(salary) + MIN(salary) FROM Salary) - salary;
```
SQL 保证同一行中所有 SET 赋值使用的都是旧值。

**Q3: 在实际生产环境中执行这类批量 UPDATE 需要注意什么？**

- **事务控制**：包裹在事务中，出错可回滚
- **锁粒度**：大表全表 UPDATE 会获取大量行锁甚至表锁，可能阻塞其他操作
- **分批执行**：超大表应分批（如 LIMIT + 循环）避免长事务
- **备份**：先 SELECT 确认影响行数，或先在测试环境验证
- **索引影响**：如果 sex 列上有索引，批量更新后可能需要 ANALYZE TABLE

## 相关题型

| 题号 | 题目 | 关联 |
|------|------|------|
| 196 | Delete Duplicate Emails | SQL UPDATE/DELETE 基础操作 |
| 197 | Rising Temperature | SQL 日期比较与自连接 |
| 620 | Not Boring Movies | SQL 条件过滤 |
| 626 | Exchange Seats | SQL 条件交换（相邻行交换座位） |
| 1873 | Calculate Special Bonus | SQL 条件计算 |
