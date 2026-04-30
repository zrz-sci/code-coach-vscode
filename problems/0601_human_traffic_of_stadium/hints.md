# 601. 体育馆的人流量 (Human Traffic of Stadium)

## 核心思路
找出 id 连续且 people >= 100 的行，筛选出属于长度 >= 3 的连续段的所有行。经典的"连续问题"用 id - ROW_NUMBER 分组技巧。

## 思维链
1. 先筛选 people >= 100 的行
2. 在筛选后的结果中找连续 id 的分组 → 用 id - ROW_NUMBER() 技巧
3. 同一连续段的行 id - row_number 值相同
4. 按这个分组 key 分组，只保留 COUNT >= 3 的组
5. 最终按 visit_date 升序排列

## 解法概览表

| 解法 | 思路 | 时间复杂度 | 难度 |
|------|------|-----------|------|
| ROW_NUMBER 分组 ⭐ | id - row_number 做分组键 | O(n log n) | Hard |
| 三表自连接 | 三个 Stadium 表 JOIN 找连续三行 | O(n^3) | Hard |

## 关键提示
1. **连续 id 技巧**: 筛选后的行中，连续 id 组成的段有性质：id - ROW_NUMBER() 为常数
2. **注意是 id 连续不是日期连续**: 题目明确说了按 id 连续判断
3. **去重**: 三表自连接写法需要最后 DISTINCT

## 解法详解

### 解法一：ROW_NUMBER 分组 ⭐
```sql
WITH filtered AS (
    SELECT *, ROW_NUMBER() OVER (ORDER BY id) AS rn
    FROM Stadium
    WHERE people >= 100
),
grouped AS (
    SELECT *, id - rn AS grp
    FROM filtered
)
SELECT id, visit_date, people
FROM grouped
WHERE grp IN (
    SELECT grp FROM grouped GROUP BY grp HAVING COUNT(*) >= 3
)
ORDER BY visit_date;
```
**原理**: 对于 people >= 100 的连续 id 行，id 递增 1 而 row_number 也递增 1，差值不变。

### 解法二：三表自连接
```sql
SELECT DISTINCT s1.*
FROM Stadium s1, Stadium s2, Stadium s3
WHERE s1.people >= 100 AND s2.people >= 100 AND s3.people >= 100
  AND (
    (s1.id = s2.id - 1 AND s2.id = s3.id - 1) OR
    (s1.id = s2.id + 1 AND s1.id = s3.id - 1) OR
    (s1.id = s2.id + 1 AND s2.id = s3.id + 1)
  )
ORDER BY s1.visit_date;
```

## 易错点

| ✗ 错误写法 | ✓ 正确写法 | 原因 |
|-----------|-----------|------|
| 按 visit_date 判断连续 | 按 id 判断连续 | 题目说 id 连续，日期可能不连续 |
| 三表连接不去重 | 加 DISTINCT | 同一行可能匹配多组三元组 |
| ORDER BY id | ORDER BY visit_date | 题目要求按 visit_date 排序 |
| HAVING COUNT >= 3 漏掉等于 | HAVING COUNT(*) >= 3 | 等于 3 也要包含 |

## 面试追问

**Q1: 如果要找连续 k 行（k 是参数），怎么改？**
ROW_NUMBER 分组法天然支持任意 k，只需改 HAVING COUNT(*) >= k。三表自连接法则需要 k 个表自连接，不实用。

**Q2: 如果 id 不保证连续（有空洞），如何改为按日期连续？**
先用 ROW_NUMBER 排序，然后用 DATEDIFF(visit_date, 基准日期) - ROW_NUMBER 作为分组键。

**Q3: 如果数据量很大（亿级），如何优化？**
先在 people 列上建索引过滤 >= 100 的行（减少数据量），然后在过滤后的小结果集上做 ROW_NUMBER 分组。

## 相关题型
- 180. 连续出现的数字
- 603. 连续空余座位
- 1454. 活跃用户（连续登录天数）
