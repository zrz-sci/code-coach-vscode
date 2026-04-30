# 180. 连续出现的数字 (Consecutive Numbers)

**难度**: Medium | **标签**: Database

## 核心思路

从Logs表中找出**至少连续出现三次**的数字。关键在于"连续"的定义：id是自增的连续整数，所以连续出现意味着相邻的id行拥有相同的num值。可以通过自连接三张表、窗口函数LAG/LEAD、或分组技巧来解决。

## 思维链

1. **理解"连续"** - id是自增列，连续 = id, id+1, id+2 的num相同
2. **选择检测方式** - 自连接(传统) vs LAG/LEAD(窗口函数) vs 分组差值法
3. **去重** - 一个数字可能连续出现4次、5次...结果只需出现一次
4. **注意边界** - 可能有多个不同数字都满足条件

## 解法概览

| 解法 | 思路 | 时间复杂度 | 推荐 |
|------|------|-----------|------|
| 三表自连接 | JOIN三次，条件id连续且num相同 | O(n) | ⭐ 推荐 |
| LAG/LEAD窗口函数 | 比较前后行的值 | O(n) | ⭐ 推荐 |
| 连续分组法(差值法) | ROW_NUMBER - 值分组编号 = 常数 → 同一连续段 | O(n log n) | |
| 变量模拟法 | 用用户变量逐行扫描计数 | O(n) | |

## 关键提示

- `id`是自增连续的，所以可以直接用`l1.id = l2.id - 1`判断相邻
- 自连接法最直观：`l1.id + 1 = l2.id AND l2.id + 1 = l3.id`
- 结果必须`DISTINCT`，因为连续4次会产生2组三元组
- LAG/LEAD方案更优雅：`LAG(num, 1)`获取前一行，`LAG(num, 2)`获取前两行

## 解法详解

### 解法1: 三表自连接 (推荐)

```sql
SELECT DISTINCT l1.num AS ConsecutiveNums
FROM Logs l1
JOIN Logs l2 ON l1.id = l2.id - 1
JOIN Logs l3 ON l2.id = l3.id - 1
WHERE l1.num = l2.num
  AND l2.num = l3.num;
```

**原理**: 将表自连接三次，确保三行id连续(l1.id, l1.id+1, l1.id+2)且num值全部相同。DISTINCT去除重复结果。

### 解法2: LAG窗口函数 (推荐)

```sql
SELECT DISTINCT num AS ConsecutiveNums
FROM (
    SELECT num,
           LAG(num, 1) OVER (ORDER BY id) AS prev1,
           LAG(num, 2) OVER (ORDER BY id) AS prev2
    FROM Logs
) t
WHERE num = prev1 AND num = prev2;
```

**原理**: `LAG(num, 1)`获取前一行的num，`LAG(num, 2)`获取前两行的num。当三个值相同时，说明连续出现了至少3次。

### 解法3: LEAD窗口函数

```sql
SELECT DISTINCT num AS ConsecutiveNums
FROM (
    SELECT num,
           LEAD(num, 1) OVER (ORDER BY id) AS next1,
           LEAD(num, 2) OVER (ORDER BY id) AS next2
    FROM Logs
) t
WHERE num = next1 AND num = next2;
```

**原理**: 与LAG方向相反，看后面两行是否和当前行相同。

### 解法4: 连续分组法(差值法)

```sql
SELECT DISTINCT num AS ConsecutiveNums
FROM (
    SELECT num,
           COUNT(*) AS cnt
    FROM (
        SELECT num,
               id - ROW_NUMBER() OVER (PARTITION BY num ORDER BY id) AS grp
        FROM Logs
    ) t
    GROUP BY num, grp
    HAVING COUNT(*) >= 3
) result;
```

**原理**: 对于同一个num值的连续段，`id - ROW_NUMBER()`的差值是常数。利用这个特性将连续段分组，然后统计每组长度 >= 3的。

## 易错点

| 错误写法 | 正确写法 | 原因 |
|---------|---------|------|
| ✗ 忘记DISTINCT | ✓ `SELECT DISTINCT l1.num` | 连续4次会产生两组三元组(1,2,3)和(2,3,4) |
| ✗ `l1.id = l2.id + 1` | ✓ `l1.id = l2.id - 1` 或 `l1.id + 1 = l2.id` | 方向搞反会匹配不上 |
| ✗ 假设id一定连续 | ✓ 题目保证id自增连续 | 实际生产中id可能有间隔（但本题保证连续） |
| ✗ `LAG(num)` 只取一行 | ✓ `LAG(num, 1)` 和 `LAG(num, 2)` | 需要比较前两行才能确认连续3次 |
| ✗ 没有处理NULL | ✓ `WHERE num = prev1 AND num = prev2` | LAG在第一行返回NULL，AND条件自动排除 |

## 面试追问

**Q1**: 如果要找连续出现至少K次的数字呢？
> 自连接法需要K张表，不够灵活。推荐用差值分组法：`id - ROW_NUMBER() OVER (PARTITION BY num ORDER BY id)`得到分组标识，然后`HAVING COUNT(*) >= K`。

**Q2**: 如果id不连续（有间隔），怎么处理？
> 不能用`l1.id + 1 = l2.id`。需要先用`ROW_NUMBER()`生成连续序号，再用差值法或LAG/LEAD方式判断。

**Q3**: 自连接方案的性能如何？
> 三表JOIN在id列有索引时是O(n)的（嵌套循环JOIN变成索引查找）。但如果要推广到K次连续，需要K-1次JOIN，不如窗口函数方案灵活。

## 相关题型

- [601. 体育馆的人流量](../0601_human_traffic_of_stadium/) - 连续3天满足条件
- [603. 连续空余座位](../0603_consecutive_available_seats/) - 连续2个
- [1164. 指定日期的产品价格](../1164_product_price_at_a_given_date/) - 时间序列查询
