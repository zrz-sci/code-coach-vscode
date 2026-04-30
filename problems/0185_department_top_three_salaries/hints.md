# 185. 部门工资前三高的所有员工 (Hard)

## 核心思路

找每个部门中工资排名前三（按**不同**工资值排）的所有员工。本质是 **分组 Top-N** 问题，需要处理并列排名。

## 思维链

1. "前三高的不同工资" → 需要按 DISTINCT salary 排名
2. 排名方式？→ DENSE_RANK（不跳号）最合适
3. 分区排名 → PARTITION BY departmentId ORDER BY salary DESC
4. 取 rank <= 3 的行 → 子查询/CTE 过滤
5. 还需要部门名称 → JOIN Department 表

## 解法概览

| 解法 | 思路 | 时间复杂度 | 推荐度 |
|------|------|-----------|--------|
| 窗口函数 DENSE_RANK | 分区排名后取 Top 3 | O(n log n) | ⭐⭐⭐ |
| 相关子查询 COUNT DISTINCT | 统计比当前大的不同工资数 | O(n*m) | ⭐⭐ |

## 关键提示

1. 必须用 `DENSE_RANK()` 而非 `RANK()` —— 题目要"前三高的不同工资"
   - 假设工资为 90000, 85000, 85000, 70000：
   - DENSE_RANK: 1, 2, 2, 3 → 三个不同工资等级都取到
   - RANK: 1, 2, 2, 4 → 第三名变成 4，WHERE rk <= 3 会漏掉 70000
2. 相关子查询解法中，`COUNT(DISTINCT ...)` 统计的是"严格大于当前工资"的不同工资数量
3. 同一部门同一工资的多个员工都要保留

## 解法详解

### 解法一：窗口函数 DENSE_RANK（推荐）
```sql
SELECT Department, Employee, Salary
FROM (
    SELECT d.name AS Department, e.name AS Employee, e.salary AS Salary,
           DENSE_RANK() OVER (PARTITION BY e.departmentId ORDER BY e.salary DESC) AS rk
    FROM Employee e
    JOIN Department d ON e.departmentId = d.id
) ranked
WHERE rk <= 3;
```

### 解法二：相关子查询
```sql
SELECT d.name AS Department, e.name AS Employee, e.salary AS Salary
FROM Employee e
JOIN Department d ON e.departmentId = d.id
WHERE (
    SELECT COUNT(DISTINCT e2.salary)
    FROM Employee e2
    WHERE e2.departmentId = e.departmentId AND e2.salary > e.salary
) < 3;
```

## 易错点

- ✗ 用 `RANK()` 而非 `DENSE_RANK()` → 有并列时排名跳号，可能漏取第三名
  → ✓ 题目说"前三高的不同工资"，需用 DENSE_RANK 不跳号

- ✗ 相关子查询中用 `<=` 而非 `<` → 逻辑错误
  → ✓ 比当前工资高的不同工资数 < 3，即当前工资排名在前三

- ✗ 忘记 JOIN Department 表，输出缺少部门名称
  → ✓ 题目要求输出三列：Department, Employee, Salary

## 面试追问

**Q1：DENSE_RANK 和 RANK 在此题中的具体区别？**
→ 假设工资 [90K, 85K, 85K, 70K, 69K]：
- DENSE_RANK: 1, 2, 2, 3, 4 → rk<=3 取到 90K, 85K, 85K, 70K
- RANK: 1, 2, 2, 4, 5 → rk<=3 只取到 90K, 85K, 85K，漏了 70K

**Q2：如果不支持窗口函数（如 MySQL 5.x），如何实现？**
→ 用相关子查询 COUNT(DISTINCT salary) 方案。

**Q3：如何改为 Top N（参数化）？**
→ 窗口函数解法只需改 `WHERE rk <= N`；相关子查询改 `< N`。

## 相关题型

- [184. 部门工资最高的员工](https://leetcode.com/problems/department-highest-salary/) - Top 1 简化版
- [178. 分数排名](https://leetcode.com/problems/rank-scores/) - DENSE_RANK 应用
- [176. 第二高的薪水](https://leetcode.com/problems/second-highest-salary/) - 单表 Top N
