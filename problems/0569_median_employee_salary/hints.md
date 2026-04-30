# 569. 员工薪水中位数 (Median Employee Salary)

## 核心思路
按公司分组，对每组员工按薪水排序（相同薪水按id排序），找到中位数位置的行。偶数个元素取中间两行，奇数个取中间一行。

## 思维链
1. 需要在每个公司内部对员工排序 → 窗口函数 ROW_NUMBER()
2. 需要知道每个公司的员工总数 → COUNT() OVER
3. 中位数位置判断：排名 rn 满足 rn IN (FLOOR((cnt+1)/2), CEIL((cnt+1)/2))
4. 偶数个 → 两行；奇数个 → 一行（FLOOR = CEIL 时合并为一行）

## 解法概览表

| 解法 | 思路 | 时间复杂度 | 推荐 |
|------|------|-----------|------|
| 窗口函数 ROW_NUMBER ⭐ | 排名后取中位数位置 | O(n log n) | 首选 |
| 自连接计数 | 不用窗口函数，比较大小关系 | O(n²) | Follow-up |

## 关键提示
1. 排序规则：先按 salary ASC，再按 id ASC 打破平局
2. 中位数位置公式：FLOOR((cnt+1)/2) 和 CEIL((cnt+1)/2)
3. 奇数个元素时两个位置相同，自动去重

## 解法详解

### 解法一：窗口函数（推荐）⭐
```sql
SELECT id, company, salary
FROM (
    SELECT id, company, salary,
           ROW_NUMBER() OVER (PARTITION BY company ORDER BY salary, id) AS rn,
           COUNT(*) OVER (PARTITION BY company) AS cnt
    FROM Employee
) t
WHERE rn IN (FLOOR((cnt + 1) / 2.0), CEIL((cnt + 1) / 2.0));
```

### 解法二：自连接（无窗口函数）
```sql
SELECT e1.id, e1.company, e1.salary
FROM Employee e1
WHERE ABS(
    (SELECT COUNT(*) FROM Employee e2 WHERE e2.company = e1.company AND e2.salary < e1.salary
     OR (e2.company = e1.company AND e2.salary = e1.salary AND e2.id < e1.id))
    -
    (SELECT COUNT(*) FROM Employee e3 WHERE e3.company = e1.company AND e3.salary > e1.salary
     OR (e3.company = e1.company AND e3.salary = e1.salary AND e3.id > e1.id))
) <= 1;
```

## 易错点
- ✗ 排序只按 salary → ✓ 必须按 (salary, id) 排序，题目要求 break ties by id
- ✗ 中位数只取一行 → ✓ 偶数个员工时需要取两行
- ✗ 使用 AVG 求中位数值 → ✓ 题目要求返回中位数所在的行，不是数值

## 面试追问
**Q1: 如果不能使用窗口函数，怎么做？**
> 用自连接 + 子查询计数：对每个员工统计"比它小的个数"和"比它大的个数"，两者之差的绝对值 <= 1 的就是中位数行。

**Q2: ROW_NUMBER vs RANK vs DENSE_RANK 在此题的区别？**
> ROW_NUMBER 保证唯一编号（即使值相同），RANK 会跳号，DENSE_RANK 不跳号。此题排序已含 id，理论上三者结果一样，但 ROW_NUMBER 最安全。

**Q3: 如果要求返回中位数的数值（不是行），怎么改？**
> 在当前查询基础上，对每个 company 取 AVG(salary) 即可。

## 相关题型
- [571. Find Median Given Frequency of Numbers](../0571_find_median_given_frequency_of_numbers/) - 频率表求中位数
- [480. Sliding Window Median](../0480_sliding_window_median/) - 滑动窗口中位数
