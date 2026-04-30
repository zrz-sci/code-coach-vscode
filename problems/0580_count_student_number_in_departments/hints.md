# 580. 统计各专业学生人数 (Count Student Number in Departments)

**难度**: Medium | **标签**: Database | **链接**: [LeetCode 580](https://leetcode.com/problems/count-student-number-in-departments/)

## 核心思路

本题要求统计每个系的学生人数，关键在于**没有学生的系也要显示（人数为0）**。这是一个经典的 LEFT JOIN + COUNT 问题。必须以 Department 表为主表做左连接，确保所有系都出现在结果中，然后用 COUNT 统计匹配的学生数。

## 思维链

1. **识别"所有部门都要显示"**: 题目明确说"even ones with no current students"，这决定了必须用 LEFT JOIN
2. **确定主表**: Department 是主表（左表），Student 是从表（右表）
3. **连接条件**: `Department.dept_id = Student.dept_id`
4. **计数方式**: 使用 `COUNT(student_id)` 而非 `COUNT(*)`，因为 LEFT JOIN 后无学生的系对应 NULL，COUNT(列名)会忽略 NULL，而 COUNT(*)会计为1
5. **排序**: 先按学生人数降序，再按系名字母升序

## 解法概览

| 解法 | 思路 | 复杂度 | 推荐 |
|------|------|--------|------|
| LEFT JOIN + COUNT | Department 左连接 Student，COUNT(student_id) | O(n + m) | ⭐⭐⭐⭐⭐ |
| LEFT JOIN + SUM(CASE) | 用 CASE WHEN 代替 COUNT | O(n + m) | ⭐⭐ |
| 子查询 | 相关子查询统计每个系的学生数 | O(n * m) | ⭐⭐ |
| UNION + IFNULL | 分别查询有学生和无学生的系再合并 | O(n + m) | ⭐ |

## 关键提示

1. **COUNT(student_id) vs COUNT(*)**: LEFT JOIN 后无匹配行为 NULL，COUNT(*)会把 NULL 行计为1，得到错误结果。必须用 COUNT(student_id) 或 COUNT(s.student_id)
2. **LEFT JOIN 方向**: 以 Department 为左表，不能反过来。如果以 Student 为左表，没有学生的系不会出现
3. **GROUP BY**: 必须按 dept_id 或 dept_name 分组。建议用 dept_id（主键，唯一），SELECT 中可取 dept_name
4. **别名冲突**: 聚合列别名 `student_number` 在 ORDER BY 中可直接引用

## 解法详解

### 解法一：LEFT JOIN + COUNT（最佳实践）

```sql
SELECT d.dept_name,
       COUNT(s.student_id) AS student_number
FROM Department d
LEFT JOIN Student s ON d.dept_id = s.dept_id
GROUP BY d.dept_id, d.dept_name
ORDER BY student_number DESC, d.dept_name ASC;
```

**执行流程**：
- LEFT JOIN 保证 Department 所有行都保留
- 对于 Law 系（dept_id=3），Student 中没有匹配行，连接后 student_id 为 NULL
- COUNT(s.student_id) 忽略 NULL，返回 0
- Engineering 有2条匹配，Science 有1条匹配

### 解法二：相关子查询

```sql
SELECT d.dept_name,
       (SELECT COUNT(*)
        FROM Student s
        WHERE s.dept_id = d.dept_id
       ) AS student_number
FROM Department d
ORDER BY student_number DESC, d.dept_name ASC;
```

**思路**：对 Department 的每一行，执行一次子查询统计对应的学生数。无学生的系子查询返回 0。性能略差但逻辑清晰。

### 解法三：IFNULL + LEFT JOIN

```sql
SELECT d.dept_name,
       IFNULL(cnt.student_number, 0) AS student_number
FROM Department d
LEFT JOIN (
    SELECT dept_id, COUNT(*) AS student_number
    FROM Student
    GROUP BY dept_id
) cnt ON d.dept_id = cnt.dept_id
ORDER BY student_number DESC, d.dept_name ASC;
```

**思路**：先在子查询中统计每个系的学生数，再 LEFT JOIN 回 Department。没有学生的系 cnt 为 NULL，用 IFNULL 转为 0。

## 易错点

| ✗ 错误写法 | ✓ 正确写法 | 说明 |
|-----------|-----------|------|
| `COUNT(*)` | `COUNT(s.student_id)` | LEFT JOIN 后无匹配行 COUNT(*) 返回1而非0 |
| `Student s LEFT JOIN Department d` | `Department d LEFT JOIN Student s` | 主表方向错误会丢失无学生的系 |
| `INNER JOIN` | `LEFT JOIN` | INNER JOIN 会过滤掉没有学生的系 |
| `ORDER BY student_number` | `ORDER BY student_number DESC, dept_name ASC` | 遗漏排序方向或二级排序 |
| `GROUP BY dept_name` | `GROUP BY d.dept_id, d.dept_name` | 虽然 dept_name 通常唯一，但用主键 dept_id 更严谨 |

## 面试追问

**Q1: COUNT(*) 和 COUNT(column) 的区别是什么？**

A1: COUNT(*) 统计所有行数（包括 NULL 行），COUNT(column) 只统计该列非 NULL 的行数。在 LEFT JOIN 场景中，不匹配的行所有右表列都是 NULL，COUNT(*) 仍然计为1，而 COUNT(右表列) 正确返回0。这是 SQL 面试的经典考点。

**Q2: 如果要求同时显示男女生人数，如何修改查询？**

A2: 使用条件聚合（CASE WHEN + SUM/COUNT）：
```sql
SELECT d.dept_name,
       COUNT(s.student_id) AS student_number,
       SUM(CASE WHEN s.gender = 'M' THEN 1 ELSE 0 END) AS male_count,
       SUM(CASE WHEN s.gender = 'F' THEN 1 ELSE 0 END) AS female_count
FROM Department d
LEFT JOIN Student s ON d.dept_id = s.dept_id
GROUP BY d.dept_id, d.dept_name
ORDER BY student_number DESC, d.dept_name ASC;
```

**Q3: LEFT JOIN 和 RIGHT JOIN 的关系？能否互换？**

A3: `A LEFT JOIN B` 等价于 `B RIGHT JOIN A`，只是主从表位置互换。实际工作中建议统一用 LEFT JOIN，可读性更好。本题也可以写成 `Student RIGHT JOIN Department`，但不推荐。

## 相关题型

- [175. Combine Two Tables](https://leetcode.com/problems/combine-two-tables/) - 基础 LEFT JOIN
- [181. Employees Earning More Than Their Managers](https://leetcode.com/problems/employees-earning-more-than-their-managers/) - 自连接
- [183. Customers Who Never Order](https://leetcode.com/problems/customers-who-never-order/) - LEFT JOIN + IS NULL
- [1164. Product Price at a Given Date](https://leetcode.com/problems/product-price-at-a-given-date/) - LEFT JOIN 处理缺失
- [1084. Sales Analysis III](https://leetcode.com/problems/sales-analysis-iii/) - 多表聚合
