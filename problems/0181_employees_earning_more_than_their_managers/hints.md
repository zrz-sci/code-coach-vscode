# 181. 超过经理收入的员工 (Employees Earning More Than Their Managers)

**难度**: Easy | **标签**: Database

## 核心思路

从Employee表中找出工资高于其直属经理的员工。这是一个经典的**自连接**问题：同一张表中，通过`managerId`字段建立员工和经理的关联关系，然后比较两者的salary。

## 思维链

1. **理解关系** - 每行有`managerId`指向经理的`id`（可能为NULL表示无经理）
2. **自连接** - 需要将Employee表和自身JOIN，一个作为员工，一个作为经理
3. **连接条件** - 员工的`managerId` = 经理的`id`
4. **过滤条件** - 员工的`salary` > 经理的`salary`
5. **输出** - 只需要员工的`name`

## 解法概览

| 解法 | 思路 | 时间复杂度 | 推荐 |
|------|------|-----------|------|
| 自连接 (JOIN) | 员工表JOIN自身，比较薪资 | O(n) | ⭐ 推荐 |
| WHERE子查询 | WHERE中关联子查询比较薪资 | O(n^2) | |
| EXISTS子查询 | EXISTS检查是否有薪资更低的经理 | O(n) | |

## 关键提示

- 这是**自连接**的经典入门题，面试中考察对JOIN的理解
- `managerId`为NULL的员工没有经理，自然不会出现在结果中（JOIN不匹配）
- 连接方式用`JOIN`(内连接)即可，不需要`LEFT JOIN`
- 输出列名是`Employee`（注意大写E，和表名相同但这里是列别名）

## 解法详解

### 解法1: 自连接 JOIN (推荐)

```sql
SELECT e.name AS Employee
FROM Employee e
JOIN Employee m ON e.managerId = m.id
WHERE e.salary > m.salary;
```

**原理**: 将Employee表分别起别名`e`(员工)和`m`(经理)，通过`e.managerId = m.id`建立关联。然后筛选`e.salary > m.salary`的记录。内连接自动排除没有经理的员工。

### 解法2: WHERE关联子查询

```sql
SELECT e.name AS Employee
FROM Employee e
WHERE e.managerId IS NOT NULL
  AND e.salary > (
      SELECT m.salary
      FROM Employee m
      WHERE m.id = e.managerId
  );
```

**原理**: 对每个有经理的员工，用子查询找到其经理的薪资，然后比较。

### 解法3: EXISTS子查询

```sql
SELECT e.name AS Employee
FROM Employee e
WHERE EXISTS (
    SELECT 1
    FROM Employee m
    WHERE m.id = e.managerId
      AND e.salary > m.salary
);
```

**原理**: 用EXISTS检查是否存在一个经理，其id等于当前员工的managerId且薪资更低。

## 易错点

| 错误写法 | 正确写法 | 原因 |
|---------|---------|------|
| ✗ `e.id = m.managerId` | ✓ `e.managerId = m.id` | 方向搞反：应该是员工的managerId指向经理的id |
| ✗ `LEFT JOIN` | ✓ `JOIN` (内连接) | LEFT JOIN会保留没有经理的员工（managerId=NULL），需要额外过滤 |
| ✗ `SELECT name` | ✓ `SELECT e.name` | 自连接时两张表都有name列，必须指定表别名 |
| ✗ `AS employee` | ✓ `AS Employee` | 输出列名需要大写E，匹配题目要求 |
| ✗ `e.salary >= m.salary` | ✓ `e.salary > m.salary` | 题目要求"超过"(strictly greater than)，不含等于 |

## 面试追问

**Q1**: 为什么用JOIN而不是LEFT JOIN？
> 题目只关心"有经理且工资超过经理"的员工。`JOIN`自动排除`managerId`为NULL的行（无经理的员工），逻辑更简洁。用`LEFT JOIN`也可以，但需要额外加`WHERE m.id IS NOT NULL`。

**Q2**: 如果经理关系是多层的（经理的经理），如何找出工资超过所有上级的员工？
> 需要用递归CTE(`WITH RECURSIVE`)遍历整条管理链，收集所有上级的最高工资，然后与员工工资比较。

**Q3**: 自连接和子查询方案的性能差异？
> 在`managerId`和`id`列有索引时，两者性能接近，都是O(n)。但自连接的执行计划更简单，优化器更容易优化。WHERE关联子查询在没有索引时可能退化为O(n^2)。

## 相关题型

- [176. 第二高的薪水](../0176_second_highest_salary/) - 薪资查询基础
- [183. 从不订购的客户](../0183_customers_who_never_order/) - 反向关联查询
- [184. 部门工资最高的员工](../0184_department_highest_salary/) - 分组最值+JOIN
- [197. 上升的温度](../0197_rising_temperature/) - 自连接比较相邻行
