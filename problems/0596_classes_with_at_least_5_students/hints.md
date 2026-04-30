# 596. 超过 5 名学生的课 (Classes More Than 5 Students)

## 核心思路
从 `Courses` 表中找出**至少有 5 名不同学生**选修的课程。经典的 `GROUP BY + HAVING` 题型：按课程分组，统计每组学生数，用 `HAVING` 筛选满足条件的组。

## 思维链
1. 表 Courses 有两列：student 和 class，主键是 (student, class)
2. 主键保证同一学生不会重复选同一门课
3. 按 class 分组，统计每组的行数
4. 用 HAVING 过滤 COUNT >= 5 的组
5. 由于主键已去重，`COUNT(*)` 和 `COUNT(student)` 等价
6. 如果没有主键约束，需要用 `COUNT(DISTINCT student)` 去重

## 解法概览
| 解法 | 特点 | 适用场景 |
|------|------|----------|
| GROUP BY + HAVING | 最标准，一条语句 | 面试和生产都推荐 |
| 子查询 + WHERE | 先统计再过滤 | 语义更清晰但多一层嵌套 |
| 窗口函数 | 先标注再过滤 | 需要同时保留学生信息时 |

## 关键提示

### 提示 1 - GROUP BY + HAVING 的语义
```sql
SELECT class
FROM Courses
GROUP BY class
HAVING COUNT(student) >= 5;
```
- `GROUP BY class`：按课程名分组
- `HAVING`：对分组后的结果做条件过滤（相当于聚合后的 WHERE）
- `WHERE` 在分组前过滤行，`HAVING` 在分组后过滤组

### 提示 2 - WHERE vs HAVING
- `WHERE` 作用于原始行，在 `GROUP BY` 之前执行
- `HAVING` 作用于分组结果，在 `GROUP BY` 之后执行
- `HAVING` 中可以使用聚合函数（COUNT, SUM 等），`WHERE` 中不可以

### 提示 3 - COUNT(*) vs COUNT(student) vs COUNT(DISTINCT student)
- 在本题中三者等价（因为主键保证无重复无 NULL）
- 如果没有主键约束：
  - `COUNT(*)` 会包含重复行
  - `COUNT(student)` 排除 NULL 但不去重
  - `COUNT(DISTINCT student)` 才是真正的不同学生数

### 提示 4 - 子查询替代方案
```sql
SELECT class
FROM (
    SELECT class, COUNT(student) AS cnt
    FROM Courses
    GROUP BY class
) t
WHERE cnt >= 5;
```
先统计再过滤，语义等价但多了一层嵌套，面试中不推荐除非面试官特别要求。

## 解法详解

### 解法一：GROUP BY + HAVING（推荐）
```sql
SELECT class
FROM Courses
GROUP BY class
HAVING COUNT(student) >= 5;
```
**说明**：
- 最简洁标准的写法
- `HAVING COUNT(student) >= 5` 直接过滤出学生数达标的课程
- 由于 (student, class) 是主键，不需要 DISTINCT

### 解法二：子查询
```sql
SELECT class
FROM (
    SELECT class, COUNT(*) AS num_students
    FROM Courses
    GROUP BY class
) sub
WHERE num_students >= 5;
```
**说明**：
- 内层查询统计每门课的学生数
- 外层查询过滤 >= 5 的课程
- 语义更分层，适合复杂场景

### 解法三：窗口函数（需要保留学生信息时）
```sql
SELECT DISTINCT class
FROM (
    SELECT class, COUNT(*) OVER (PARTITION BY class) AS cnt
    FROM Courses
) t
WHERE cnt >= 5;
```
**说明**：
- `COUNT(*) OVER (PARTITION BY class)` 为每行标注该课程的学生总数
- 外层过滤后用 DISTINCT 去重
- 如果同时需要返回这些课程的学生名单，窗口函数更方便

## 易错点
1. **用 WHERE 代替 HAVING**：`WHERE COUNT(student) >= 5` 语法错误，聚合函数不能在 WHERE 中使用
2. **忘记 GROUP BY**：没有分组就用 HAVING 或聚合函数，结果会对整张表做一次聚合
3. **不必要的 DISTINCT**：本题主键已保证去重，加 `COUNT(DISTINCT student)` 不影响正确性但冗余
4. **混淆 >= 和 >**：题目要求"至少 5 名"，用 `>= 5`，不是 `> 5`
5. **返回多余列**：题目只要求返回 class，不要多返回 COUNT 列（除非题目要求）

## 面试追问
1. **WHERE 和 HAVING 的执行顺序？** - SQL 逻辑执行顺序：FROM -> WHERE -> GROUP BY -> HAVING -> SELECT -> ORDER BY
2. **如果去掉主键约束，同一学生可以重复选课？** - 需要改用 `COUNT(DISTINCT student)`
3. **如何找出恰好有 5 名学生的课？** - `HAVING COUNT(student) = 5`
4. **如何找出学生最多的那门课？** - `ORDER BY COUNT(student) DESC LIMIT 1`
5. **索引如何优化？** - 在 class 列建索引可以加速 GROUP BY 操作

## 相关题型
- [595. Big Countries](https://leetcode.com/problems/big-countries/) - 基础 WHERE 筛选
- [182. Duplicate Emails](https://leetcode.com/problems/duplicate-emails/) - GROUP BY + HAVING COUNT > 1
- [586. Customer Placing the Largest Number of Orders](https://leetcode.com/problems/customer-placing-the-largest-number-of-orders/) - GROUP BY + ORDER BY + LIMIT
- [1084. Sales Analysis III](https://leetcode.com/problems/sales-analysis-iii/) - HAVING 与日期过滤
- [184. Department Highest Salary](https://leetcode.com/problems/department-highest-salary/) - GROUP BY 与 JOIN 结合
