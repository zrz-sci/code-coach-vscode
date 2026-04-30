# 197. 上升的温度 (Rising Temperature)

**难度**: Easy | **标签**: Database

## 核心思路

找出比「前一天」温度更高的日期的 id。关键在于如何正确关联「昨天」的记录 —— 用日期函数 `DATEDIFF` 或 `DATE_SUB` 进行自连接。

## 思维链

1. 需要比较每一天与前一天的温度
2. 「前一天」不能用 id-1，因为 id 和日期不一定连续对应
3. 必须用日期运算找到真正的「昨天」
4. 自连接 + 日期差 = 1 天的条件

## 解法概览

| 解法 | 思路 | 推荐 |
|------|------|------|
| DATEDIFF 自连接 | `DATEDIFF(w1.recordDate, w2.recordDate) = 1` | ⭐ |
| DATE_SUB 自连接 | `w2.recordDate = DATE_SUB(w1.recordDate, INTERVAL 1 DAY)` | ⭐ |
| 窗口函数 LAG | LAG 取前一行日期和温度 | |

## 关键提示

1. `DATEDIFF(a, b)` 返回 a - b 的天数差
2. 不能用 `id - 1` 代替日期关系，id 不一定连续或与日期对应
3. `DATE_SUB(date, INTERVAL 1 DAY)` 返回前一天日期
4. 题目保证 recordDate 唯一，不需要处理同日期多条记录

## 解法详解

### 解法一：DATEDIFF 自连接（推荐）

```sql
SELECT w1.id
FROM Weather w1
JOIN Weather w2
  ON DATEDIFF(w1.recordDate, w2.recordDate) = 1
WHERE w1.temperature > w2.temperature;
```

**原理**：w1 是「今天」，w2 是「昨天」。DATEDIFF = 1 确保恰好差一天，再比较温度。

### 解法二：DATE_SUB 自连接

```sql
SELECT w1.id
FROM Weather w1
JOIN Weather w2
  ON w2.recordDate = DATE_SUB(w1.recordDate, INTERVAL 1 DAY)
WHERE w1.temperature > w2.temperature;
```

### 解法三：窗口函数 LAG（MySQL 8.0+）

```sql
SELECT id
FROM (
    SELECT id,
           temperature,
           recordDate,
           LAG(temperature) OVER (ORDER BY recordDate) AS prev_temp,
           LAG(recordDate) OVER (ORDER BY recordDate) AS prev_date
    FROM Weather
) AS sub
WHERE temperature > prev_temp
  AND DATEDIFF(recordDate, prev_date) = 1;
```

**注意**：LAG 取的是排序后的前一行，但日期不一定连续，所以仍需检查 DATEDIFF = 1。

## 易错点

1. ✗ `ON w1.id = w2.id - 1` 用 id 差代替日期差
   ✓ 必须用 `DATEDIFF` 或 `DATE_SUB` 进行日期运算

2. ✗ `DATEDIFF(w1.recordDate, w2.recordDate) >= 1` 匹配所有更早的日期
   ✓ 必须严格等于 1，只比较「昨天」

3. ✗ 窗口函数 LAG 后不检查日期连续性
   ✓ 排序后前一行日期可能不是「昨天」（如跳过周末），需额外验证

## 面试追问

**Q1**: 如果要找连续3天温度上升的日期，如何实现？
→ 三表自连接，或用 LAG 取前两天的温度一起比较

**Q2**: DATEDIFF 和 TIMESTAMPDIFF 的区别？
→ DATEDIFF 只算天数差，TIMESTAMPDIFF 可指定单位（秒/分/时/天）

**Q3**: 如果 recordDate 不唯一（同一天多条记录），如何处理？
→ 需要先用 GROUP BY + AVG/MAX 聚合每天的温度

## 相关题型

- 180: Consecutive Numbers（连续出现的数字）
- 196: Delete Duplicate Emails（自连接删除）
- 550: Game Play Analysis IV（日期差关联）
