# 177. 第N高的薪水 (Nth Highest Salary)

**难度**: Medium | **标签**: Database

## 核心思路

这是176题（第二高薪水）的泛化版本。需要创建一个MySQL函数`getNthHighestSalary(N)`，返回第N高的不同薪水。核心难点：1) 必须写成函数形式；2) LIMIT子句中不能直接用表达式`N-1`，需要先声明变量；3) 不足N个不同薪水时返回NULL。

## 思维链

1. **函数框架** - MySQL要求`CREATE FUNCTION ... RETURNS INT BEGIN ... RETURN ... END`
2. **变量声明** - `LIMIT`不接受表达式，需`SET N = N - 1`或声明新变量`SET M = N - 1`
3. **去重排序** - `SELECT DISTINCT salary ORDER BY salary DESC`
4. **取第N个** - `LIMIT 1 OFFSET M`（M = N - 1）
5. **空值处理** - 函数RETURN的子查询为空时自动返回NULL

## 解法概览

| 解法 | 思路 | 关键点 | 推荐 |
|------|------|--------|------|
| LIMIT OFFSET + 变量 | 先SET N=N-1，再LIMIT 1 OFFSET N | 需要预处理N | ⭐ 推荐 |
| 子查询计数法 | 恰好有N-1个不同薪水比它大 | 不依赖LIMIT | |
| DENSE_RANK窗口函数 | 排名后筛选rank=N | 通用性最强 | ⭐ 推荐 |

## 关键提示

- MySQL的`LIMIT`子句不支持表达式（如`LIMIT 1 OFFSET N-1`是语法错误）
- 必须先`SET N = N - 1`或`DECLARE M INT; SET M = N - 1`
- 函数内可以直接修改参数N的值，不需要额外变量
- `RETURN`语句中的子查询返回空集时，函数自动返回NULL

## 解法详解

### 解法1: LIMIT OFFSET + 变量预处理 (推荐)

```sql
CREATE FUNCTION getNthHighestSalary(N INT) RETURNS INT
BEGIN
  SET N = N - 1;              -- LIMIT不接受表达式，预先减1
  RETURN (
      SELECT DISTINCT salary
      FROM Employee
      ORDER BY salary DESC
      LIMIT 1 OFFSET N        -- 跳过前N-1个，取第N个
  );
END
```

**原理**: 第N高 = 降序排列后第N个 = OFFSET N-1。由于MySQL的LIMIT不接受运算表达式，必须预先计算。

### 解法2: 子查询计数法

```sql
CREATE FUNCTION getNthHighestSalary(N INT) RETURNS INT
BEGIN
  RETURN (
      SELECT DISTINCT e1.salary
      FROM Employee e1
      WHERE (
          SELECT COUNT(DISTINCT e2.salary)
          FROM Employee e2
          WHERE e2.salary > e1.salary
      ) = N - 1                -- 恰好有N-1个不同薪水比它大 → 它是第N高
  );
END
```

**原理**: 第N高的薪水意味着恰好有N-1个不同的薪水比它高。用相关子查询计数来判断。

### 解法3: DENSE_RANK窗口函数 (推荐)

```sql
CREATE FUNCTION getNthHighestSalary(N INT) RETURNS INT
BEGIN
  RETURN (
      SELECT DISTINCT salary
      FROM (
          SELECT salary,
                 DENSE_RANK() OVER (ORDER BY salary DESC) AS rnk
          FROM Employee
      ) ranked
      WHERE rnk = N
  );
END
```

**原理**: DENSE_RANK产生无间隔的排名(1,1,2,3,3,4...)，直接筛选rank=N即可。

## 易错点

| 错误写法 | 正确写法 | 原因 |
|---------|---------|------|
| ✗ `LIMIT 1 OFFSET N-1` | ✓ `SET N = N-1; ... LIMIT 1 OFFSET N` | MySQL的LIMIT/OFFSET不接受表达式 |
| ✗ `RANK()` | ✓ `DENSE_RANK()` | RANK()有间隔(1,1,3)，DENSE_RANK()无间隔(1,1,2) |
| ✗ 忘记DISTINCT | ✓ `SELECT DISTINCT salary` | 重复薪水不应占用排名 |
| ✗ `WHERE rnk = N - 1` | ✓ `WHERE rnk = N` | DENSE_RANK从1开始，第N高就是rank=N |
| ✗ 不处理N<=0的情况 | ✓ 函数RETURN空子查询自动返回NULL | N无效时子查询返回空集 |

## 面试追问

**Q1**: 为什么不能在LIMIT中直接写`N-1`？
> MySQL的LIMIT/OFFSET语法要求参数必须是常量或变量，不能是表达式。这是SQL解析器的限制，不是执行引擎的限制。PostgreSQL则允许表达式。

**Q2**: DENSE_RANK、RANK、ROW_NUMBER有什么区别？
> - `ROW_NUMBER()`: 每行唯一编号(1,2,3,4)，相同值也不同编号
> - `RANK()`: 相同值同排名，但有间隔(1,1,3,4)
> - `DENSE_RANK()`: 相同值同排名，无间隔(1,1,2,3)
> 本题要求"不同薪水"的第N高，用DENSE_RANK最合适。

**Q3**: 如果数据量很大（百万级），哪种解法最优？
> LIMIT OFFSET方案最优，因为排序后只需扫描前N条即可停止。子查询计数法是O(n^2)最慢。DENSE_RANK需要全表扫描和排序。如果salary列有索引，LIMIT OFFSET可以直接用索引扫描。

## 相关题型

- [176. 第二高的薪水](../0176_second_highest_salary/) - 本题N=2的特殊情况
- [185. 部门工资前三高的所有员工](../0185_department_top_three_salaries/) - 分组TopN
- [178. 分数排名](../0178_rank_scores/) - 排名函数的应用
