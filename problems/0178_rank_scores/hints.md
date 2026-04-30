# 178. 分数排名 (Rank Scores)

**难度**: Medium | **标签**: Database

## 核心思路

对Scores表中的分数进行排名，要求：1) 按分数从高到低排列；2) 相同分数排名相同；3) 排名连续无间隔。这实际上就是`DENSE_RANK()`窗口函数的定义。如果不用窗口函数，可以用子查询计数有多少个不同的更高分数。

## 思维链

1. **理解排名规则** - 相同分数同名次 + 名次无间隔 = DENSE_RANK
2. **区分排名类型** - ROW_NUMBER(唯一) vs RANK(有间隔) vs DENSE_RANK(无间隔)
3. **选择实现方式** - 窗口函数(现代) vs 子查询计数(传统)
4. **注意列别名** - `rank`是MySQL保留字，需要用反引号或双引号

## 解法概览

| 解法 | 思路 | 时间复杂度 | 推荐 |
|------|------|-----------|------|
| DENSE_RANK窗口函数 | 直接用内置函数 | O(n log n) | ⭐ 推荐 |
| 子查询计数法 | 统计比当前分数高的不同值个数+1 | O(n^2) | |
| 自连接计数法 | JOIN去重分数表后计数 | O(n^2) | |

## 关键提示

- 题目要求的排名规则 = DENSE_RANK（无间隔连续排名）
- `rank`是SQL保留关键字，作为列别名时必须加反引号 `` `rank` ``
- 排名 = 有多少个**不同的**分数比当前分数高 + 1
- 必须按`score DESC`排序输出结果

## 解法详解

### 解法1: DENSE_RANK窗口函数 (推荐)

```sql
SELECT score,
       DENSE_RANK() OVER (ORDER BY score DESC) AS `rank`
FROM Scores
ORDER BY score DESC;
```

**原理**: `DENSE_RANK()`完美匹配题目要求——相同值同排名，排名连续无间隔。窗口函数中`ORDER BY score DESC`使最高分排名为1。

### 解法2: 子查询计数法

```sql
SELECT s1.score,
       (SELECT COUNT(DISTINCT s2.score)
        FROM Scores s2
        WHERE s2.score >= s1.score) AS `rank`
FROM Scores s1
ORDER BY s1.score DESC;
```

**原理**: 对于每个分数，统计有多少个不同的分数 >= 它（包括自己），这个数量就是排名。最高分只有1个 >= 它的不同值，所以排名为1。

### 解法3: 自连接法

```sql
SELECT s1.score,
       COUNT(DISTINCT s2.score) AS `rank`
FROM Scores s1
JOIN Scores s2 ON s2.score >= s1.score
GROUP BY s1.id, s1.score
ORDER BY s1.score DESC;
```

**原理**: 将原表和自身JOIN，条件是s2.score >= s1.score。对每个s1的记录，计算匹配的不同s2.score个数。

## 易错点

| 错误写法 | 正确写法 | 原因 |
|---------|---------|------|
| ✗ `RANK() OVER(...)` | ✓ `DENSE_RANK() OVER(...)` | RANK()有间隔(1,1,3)，不满足"连续"要求 |
| ✗ `AS rank` | ✓ `` AS `rank` `` | rank是MySQL8.0保留字，必须加反引号 |
| ✗ `COUNT(s2.score)` | ✓ `COUNT(DISTINCT s2.score)` | 不加DISTINCT会因重复分数导致排名错误 |
| ✗ `WHERE s2.score > s1.score` | ✓ `WHERE s2.score >= s1.score` | 用>的话最高分排名会变成0 |
| ✗ 忘记最终ORDER BY | ✓ `ORDER BY score DESC` | 题目要求结果按分数降序 |

## 面试追问

**Q1**: DENSE_RANK、RANK、ROW_NUMBER输出有什么区别？
> 假设分数为[4.00, 4.00, 3.85, 3.65]：
> - ROW_NUMBER: 1, 2, 3, 4（唯一编号）
> - RANK: 1, 1, 3, 4（同值同名次，但有间隔，跳过2）
> - DENSE_RANK: 1, 1, 2, 3（同值同名次，无间隔，连续）

**Q2**: 如果要实现RANK()的效果（有间隔），不用窗口函数怎么写？
> 把`COUNT(DISTINCT s2.score)`改成`COUNT(s2.id)`——统计有多少行（不去重）的分数 >= 当前分数。但要注意自连接时每个s1会匹配多行，需要正确分组。

**Q3**: 不用窗口函数的解法性能如何优化？
> 在score列上建索引，可以将子查询的全表扫描优化为索引扫描。但最优解仍然是窗口函数，因为只需一次排序遍历。

## 相关题型

- [177. 第N高的薪水](../0177_nth_highest_salary/) - 排名后取特定名次
- [184. 部门工资最高的员工](../0184_department_highest_salary/) - 分组排名
- [185. 部门工资前三高的所有员工](../0185_department_top_three_salaries/) - 分组DENSE_RANK
