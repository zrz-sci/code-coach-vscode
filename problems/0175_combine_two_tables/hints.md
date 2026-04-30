# 175. 组合两个表

## 核心思路
将 Person 表和 Address 表按 personId 关联查询，要求即使某人没有地址也要显示——本质是 LEFT JOIN 的经典应用场景。

## 思维链
1. **理解需求**: 输出每个 Person 的 firstName, lastName, city, state，没有地址的显示 NULL
2. **JOIN 类型选择**: "即使没有地址也要显示" → 不能用 INNER JOIN（会丢失无地址的人），必须用 LEFT JOIN
3. **关联条件**: Person.personId = Address.personId
4. **字段选择**: SELECT firstName, lastName, city, state

## 解法概览
| 解法 | 思路 | 面试 |
|------|------|------|
| LEFT JOIN ⭐ | Person 左连接 Address | 必知 |
| 子查询 | SELECT 中嵌套子查询获取地址 | 了解 |

## 关键提示
1. **LEFT JOIN vs INNER JOIN**: LEFT JOIN 保留左表所有行，右表无匹配时填 NULL；INNER JOIN 只保留两表都有匹配的行
2. **为什么不能用 INNER JOIN**: personId=1 没有地址，INNER JOIN 会丢掉这一行，不满足题意
3. **NULL 的含义**: LEFT JOIN 中右表无匹配行时，右表所有字段自动为 NULL
4. **表别名**: 使用 p 和 a 作别名可以让 SQL 更简洁

## 解法详解

### 解法1: LEFT JOIN (推荐)

**思路**: 以 Person 为主表，LEFT JOIN Address 表，这样所有 Person 都会出现在结果中，即使没有匹配的 Address（此时 city 和 state 为 NULL）。

```sql
SELECT p.firstName, p.lastName, a.city, a.state
FROM Person p
LEFT JOIN Address a ON p.personId = a.personId;
```

**思考过程**:
- 为什么 Person 是左表？因为需求是"每个 Person"都要出现
- 如果一个 Person 有多个地址怎么办？题意保证 addressId 是主键，但 personId 不是 Address 的唯一约束，所以理论上可能有多行

### 解法2: 子查询

**思路**: 在 SELECT 子句中用相关子查询获取每个 Person 的 city 和 state。

```sql
SELECT 
    p.firstName,
    p.lastName,
    (SELECT a.city FROM Address a WHERE a.personId = p.personId LIMIT 1) AS city,
    (SELECT a.state FROM Address a WHERE a.personId = p.personId LIMIT 1) AS state
FROM Person p;
```

**思考过程**:
- 功能等价但性能差——每行都要执行两次子查询
- 面试中一般直接用 LEFT JOIN

## 易错点
- ✗ 使用 `INNER JOIN` → 丢失没有地址的 Person
- ✓ 使用 `LEFT JOIN` → 保留所有 Person
- ✗ `LEFT JOIN Address a ON p.personId = a.addressId` → 关联字段写错
- ✓ `LEFT JOIN Address a ON p.personId = a.personId` → 用 personId 关联
- ✗ 忘记 SELECT 指定具体字段，用 `SELECT *` → 会输出多余的列
- ✓ 只 SELECT 需要的四个字段

## 面试追问
- **Q1**: LEFT JOIN 和 RIGHT JOIN 的区别？
  - A: LEFT JOIN 保留左表所有行，RIGHT JOIN 保留右表所有行；本题用 RIGHT JOIN 的话需要交换表的位置
- **Q2**: 如果一个人有多个地址，结果会怎样？
  - A: 会产生多行，每个地址一行；如果只要最新地址需要加子查询或 DISTINCT/GROUP BY
- **Q3**: FULL OUTER JOIN 什么时候用？
  - A: 当需要同时保留两表中无匹配的行时，本题不需要因为只关心 Person 表的完整性

## 相关题型
- [176. Second Highest Salary](../0176_second_highest_salary/) - SQL 基础题，子查询
- [181. Employees Earning More Than Their Managers](../0181_employees_earning_more_than_their_managers/) - 自连接
- [183. Customers Who Never Order](../0183_customers_who_never_order/) - LEFT JOIN + IS NULL 判断
