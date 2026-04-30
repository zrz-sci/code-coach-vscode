# 184. 部门工资最高的员工 (Medium)

## 核心思路

找每个部门中工资等于该部门最高工资的员工。本质是 **分组求极值 + 多表连接**。注意：同一部门可能有多个并列最高工资的员工。

## 思维链

1. 需要每个部门的最高工资 → GROUP BY departmentId + MAX(salary)
2. 需要找出工资 = 最高工资的员工 → 子查询或窗口函数
3. 需要显示部门名称 → JOIN Department 表
4. 可能有并列 → 不能用 LIMIT 1，要用等值匹配

## 解法概览

| 解法 | 思路 | 时间复杂度 | 推荐度 |
|------|------|-----------|--------|
| 子查询 + IN | 先求每组MAX，再匹配 | O(n) | ⭐⭐⭐ |
| 窗口函数 RANK | RANK() 排名后取第1 | O(n log n) | ⭐⭐⭐ |
| 相关子查询 | 对每行检查是否为组最大 | O(n*m) | ⭐⭐ |

## 关键提示

1. IN 子查询匹配 `(departmentId, salary)` 组合，而非单独匹配
2. 窗口函数用 `RANK()` 而非 `ROW_NUMBER()`，因为需要保留并列
3. JOIN 时注意外键关系：Employee.departmentId = Department.id

## 解法详解

### 解法一：子查询 + IN（推荐）
```sql
SELECT d.name AS Department, e.name AS Employee, e.salary AS Salary
FROM Employee e
JOIN Department d ON e.departmentId = d.id
WHERE (e.departmentId, e.salary) IN (
    SELECT departmentId, MAX(salary)
    FROM Employee
    GROUP BY departmentId
);
```

### 解法二：窗口函数 RANK
```sql
SELECT Department, Employee, Salary
FROM (
    SELECT d.name AS Department, e.name AS Employee, e.salary AS Salary,
           RANK() OVER (PARTITION BY e.departmentId ORDER BY e.salary DESC) AS rk
    FROM Employee e
    JOIN Department d ON e.departmentId = d.id
) ranked
WHERE rk = 1;
```

### 解法三：相关子查询
```sql
SELECT d.name AS Department, e.name AS Employee, e.salary AS Salary
FROM Employee e
JOIN Department d ON e.departmentId = d.id
WHERE e.salary = (
    SELECT MAX(salary) FROM Employee WHERE departmentId = e.departmentId
);
```

## 易错点

- ✗ 用 `WHERE salary = MAX(salary)` → 聚合函数不能直接用在 WHERE 中
  → ✓ 需要通过子查询或窗口函数获取 MAX 值

- ✗ 用 `ROW_NUMBER()` 导致并列最高工资只取一个人
  → ✓ 应使用 `RANK()` 保留并列

- ✗ IN 子查询只匹配 salary 不匹配 departmentId
  → ✓ 必须用元组匹配 `(departmentId, salary) IN (...)`

## 面试追问

**Q1：RANK() 和 DENSE_RANK() 在此题中有区别吗？**
→ 对于取第1名，两者完全等价。差异在取 Top N 时：RANK 跳号，DENSE_RANK 不跳号。

**Q2：如何修改为取每个部门工资前两名？**
→ 窗口函数解法中改为 `WHERE rk <= 2`；子查询解法需要更复杂的改造。

**Q3：如果 Employee 表非常大，如何优化？**
→ 在 (departmentId, salary) 上建联合索引；或用 CTE + 窗口函数减少扫描次数。

## 相关题型

- [185. 部门工资前三高的所有员工](https://leetcode.com/problems/department-top-three-salaries/) - Top N 扩展
- [176. 第二高的薪水](https://leetcode.com/problems/second-highest-salary/) - 单表求第N大
- [178. 分数排名](https://leetcode.com/problems/rank-scores/) - DENSE_RANK 应用
