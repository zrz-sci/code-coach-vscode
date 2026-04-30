# 586. Customer Placing the Largest Number of Orders - 订单最多的客户

## 核心思路

本题是一道基础 SQL 聚合查询题。核心任务是在 Orders 表中找出下单次数最多的客户编号。关键操作是按 `customer_number` 分组、计数、然后排序取第一名。

这道题考察的是 GROUP BY + COUNT + ORDER BY + LIMIT 的经典组合，是 SQL 面试中最常见的聚合模式之一。

## 思维链

1. **理解需求**：找出下单次数最多的客户 -> 需要统计每个客户的订单数量
2. **分组聚合**：按 `customer_number` 分组，用 COUNT 统计每组的订单数
3. **排序取最大**：按订单数降序排列，取第一行
4. **考虑 Follow-up**：如果有并列最多的情况，需要用子查询或 HAVING 处理

## 解法概览

| 解法 | 思路 | 适用场景 |
|------|------|---------|
| ORDER BY + LIMIT | 排序后取第一行 | 题目保证唯一最大值 |
| 子查询 + HAVING | 先算最大count，再筛选 | Follow-up: 处理并列情况 |
| 窗口函数 RANK | 按count排名取rank=1 | Follow-up: 处理并列情况 |
| ALL 子查询 | >= ALL 比较 | 无需 ORDER BY 的替代方案 |

## 关键提示

1. `GROUP BY customer_number` 是必须的第一步，将同一客户的所有订单聚合
2. `COUNT(*)` 或 `COUNT(order_number)` 都可以统计订单数
3. `ORDER BY COUNT(*) DESC LIMIT 1` 是取最大值的经典手法
4. Follow-up 中如果有多个并列最大的客户，`LIMIT 1` 就不够了，需要子查询
5. 注意区分 `COUNT(*)` 和 `COUNT(column)`：前者统计行数，后者不计 NULL

## 解法详解

### 解法一：ORDER BY + LIMIT（基础解法）

```sql
SELECT customer_number
FROM Orders
GROUP BY customer_number
ORDER BY COUNT(*) DESC
LIMIT 1;
```

**执行流程**：
- `GROUP BY customer_number`：将订单按客户分组
- `COUNT(*)`：计算每组中的订单行数
- `ORDER BY ... DESC`：按计数从大到小排序
- `LIMIT 1`：只取排在第一位的客户

**时间复杂度**：O(n log n)，其中 n 是订单总数（排序开销）

### 解法二：子查询 + HAVING（Follow-up 解法）

```sql
SELECT customer_number
FROM Orders
GROUP BY customer_number
HAVING COUNT(*) = (
    SELECT COUNT(*) AS cnt
    FROM Orders
    GROUP BY customer_number
    ORDER BY cnt DESC
    LIMIT 1
);
```

**思路**：先用子查询找出最大订单数，然后用 HAVING 筛选所有等于这个最大值的客户。

### 解法三：窗口函数 RANK（Follow-up 解法）

```sql
SELECT customer_number
FROM (
    SELECT customer_number,
           RANK() OVER (ORDER BY COUNT(*) DESC) AS rnk
    FROM Orders
    GROUP BY customer_number
) ranked
WHERE rnk = 1;
```

**思路**：利用 RANK() 窗口函数按订单数排名，RANK 会给并列的记录相同排名，因此能正确处理并列情况。注意 RANK 和 DENSE_RANK 在这里效果相同，但不要用 ROW_NUMBER（不处理并列）。

### 解法四：ALL 子查询（无需 ORDER BY）

```sql
SELECT customer_number
FROM Orders
GROUP BY customer_number
HAVING COUNT(*) >= ALL (
    SELECT COUNT(*)
    FROM Orders
    GROUP BY customer_number
);
```

**思路**：使用 `>= ALL` 比较当前组的计数和所有组的计数，只有最大的组才能满足条件。

## 易错点

1. **忘记 GROUP BY**：直接写 `SELECT customer_number FROM Orders ORDER BY COUNT(*) DESC` 会报语法错误
2. **Follow-up 用 LIMIT 1**：如果有多个客户并列最多订单，LIMIT 1 只返回一个，漏掉其他
3. **COUNT 列选择**：`COUNT(order_number)` 和 `COUNT(*)` 在无 NULL 时等价，但语义不同
4. **ORDER BY 方向**：忘记写 DESC 会取到订单最少的客户
5. **SELECT 列与 GROUP BY 不一致**：在严格模式下，SELECT 中的非聚合列必须在 GROUP BY 中出现

## 面试追问

1. **如果有多个客户订单数并列最多怎么办？**
   - 使用子查询 + HAVING 或 RANK() 窗口函数，而非 LIMIT 1
2. **如何同时返回客户编号和订单数？**
   - `SELECT customer_number, COUNT(*) AS order_count FROM Orders GROUP BY customer_number ORDER BY order_count DESC LIMIT 1`
3. **如果 Orders 表有上亿行数据，如何优化？**
   - 在 `customer_number` 列建立索引，帮助 GROUP BY 加速
   - 如果只需要 Top-K，考虑使用物化视图或预计算表
4. **RANK vs DENSE_RANK vs ROW_NUMBER 的区别？**
   - RANK: 有并列时跳号 (1,1,3)
   - DENSE_RANK: 有并列时不跳号 (1,1,2)
   - ROW_NUMBER: 即使并列也给不同号 (1,2,3)
5. **能否不用 ORDER BY 解决？**
   - 可以用 `HAVING COUNT(*) >= ALL (SELECT COUNT(*) FROM Orders GROUP BY customer_number)`

## 相关题型

- [176. Second Highest Salary](../0176_second_highest_salary/) - 排序+LIMIT/子查询
- [181. Employees Earning More Than Their Managers](../0181_employees_earning_more_than_their_managers/) - 表自连接
- [183. Customers Who Never Order](../0183_customers_who_never_order/) - NOT IN / LEFT JOIN
- [1741. Find Total Time Spent by Each Employee](../1741_find_total_time_spent_by_each_employee/) - GROUP BY 聚合
- [1050. Actors and Directors Who Cooperated At Least Three Times](../1050_actors_and_directors_who_cooperated_at_least_three_times/) - GROUP BY + HAVING
