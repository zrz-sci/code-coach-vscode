# 176. 第二高的薪水 (Second Highest Salary)

**难度**: Medium | **标签**: Database

## 核心思路

从Employee表中找出第二高的**不同**薪水值。核心难点在于：当不存在第二高薪水时，需要返回`null`而非空结果集。这要求我们理解SQL中子查询返回空集时自动转为`NULL`的特性。

## 思维链

1. **去重** - 薪水可能有重复值，必须用`DISTINCT`
2. **排序取第二** - 按薪水降序排列，跳过第一个，取下一个
3. **处理空值** - 如果只有一个不同薪水值，结果需返回`null`
4. **包装查询** - 用子查询或`IFNULL`包装，确保空结果集变为`NULL`

## 解法概览

| 解法 | 思路 | 时间复杂度 | 推荐 |
|------|------|-----------|------|
| 子查询 + LIMIT OFFSET | 排序后跳1取1，外层SELECT包装 | O(n log n) | ⭐ 推荐 |
| MAX + 子查询排除最大值 | 在排除最大值后的集合中取MAX | O(n) | ⭐ 推荐 |
| IFNULL + LIMIT OFFSET | 用IFNULL处理空值 | O(n log n) | |
| DENSE_RANK窗口函数 | 给薪水排名后筛选rank=2 | O(n log n) | |

## 关键提示

- `SELECT`子查询在结果为空时会自动返回`NULL`，这是解题的关键特性
- `LIMIT 1 OFFSET 1` 表示跳过1条取1条，即取第2条
- `MAX()`函数在空集上返回`NULL`，天然满足题目要求
- 务必`DISTINCT`去重，否则相同薪水会被当作不同排名

## 解法详解

### 解法1: 子查询 + LIMIT OFFSET (推荐)

```sql
SELECT (
    SELECT DISTINCT salary
    FROM Employee
    ORDER BY salary DESC
    LIMIT 1 OFFSET 1
) AS SecondHighestSalary;
```

**原理**: 将查询作为SELECT子句中的标量子查询。当内层查询返回空集时，外层SELECT自动将其转为NULL。

### 解法2: MAX + 排除最大值 (推荐)

```sql
SELECT MAX(salary) AS SecondHighestSalary
FROM Employee
WHERE salary < (SELECT MAX(salary) FROM Employee);
```

**原理**: 先找到最高薪水，然后在排除最高薪水后的集合中再取MAX。`MAX()`在空集上自然返回NULL。

### 解法3: IFNULL包装

```sql
SELECT IFNULL(
    (SELECT DISTINCT salary FROM Employee ORDER BY salary DESC LIMIT 1 OFFSET 1),
    NULL
) AS SecondHighestSalary;
```

### 解法4: DENSE_RANK窗口函数

```sql
SELECT MAX(salary) AS SecondHighestSalary
FROM (
    SELECT salary, DENSE_RANK() OVER (ORDER BY salary DESC) AS rnk
    FROM Employee
) ranked
WHERE rnk = 2;
```

## 易错点

| 错误写法 | 正确写法 | 原因 |
|---------|---------|------|
| ✗ `SELECT DISTINCT salary FROM Employee ORDER BY salary DESC LIMIT 1 OFFSET 1` | ✓ `SELECT (SELECT DISTINCT salary ... LIMIT 1 OFFSET 1) AS SecondHighestSalary` | 直接查询在无结果时返回空集而非NULL |
| ✗ `LIMIT 2` 然后取最后一个 | ✓ `LIMIT 1 OFFSET 1` | LIMIT 2返回最多2行，逻辑更复杂 |
| ✗ 忘记DISTINCT | ✓ `SELECT DISTINCT salary` | 相同薪水200,200,300中第二高是200不是200 |
| ✗ `ORDER BY salary LIMIT 1 OFFSET 1` | ✓ `ORDER BY salary DESC LIMIT 1 OFFSET 1` | 默认升序，需要降序 |

## 面试追问

**Q1**: 为什么直接`SELECT DISTINCT salary ... LIMIT 1 OFFSET 1`不行？
> 因为当只有一个不同薪水时，该查询返回空结果集(0行)，而题目要求返回一行包含`null`的结果。

**Q2**: 如果改成求第N高的薪水怎么办？
> 可以用`LIMIT 1 OFFSET N-1`，但注意N需要先减1。这就是LeetCode 177的题目。需要写成函数形式，在函数体内`SET N = N - 1`。

**Q3**: `MAX()`方法和`LIMIT OFFSET`方法各有什么优缺点？
> `MAX()`不需要排序，理论上更快(O(n))，但不容易推广到第N高。`LIMIT OFFSET`需要排序(O(n log n))，但更灵活，可推广到任意第N高。

## 相关题型

- [177. 第N高的薪水](../0177_nth_highest_salary/) - 本题的泛化版本
- [184. 部门工资最高的员工](../0184_department_highest_salary/) - 分组求最值
- [185. 部门工资前三高的所有员工](../0185_department_top_three_salaries/) - TopN问题
