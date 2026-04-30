# 550. 游戏玩法分析 IV

## 核心思路

计算"首次登录后次日再次登录的玩家比例"。先找每个玩家的首次登录日期，再检查是否存在首日+1 的登录记录，最后算比例。

## 思维链

1. **读完题第一反应**：需要两步 —— (1) 找每个玩家的首次登录日期；(2) 检查该玩家在首日+1 天是否有记录。

2. **子查询方案**：用 `MIN(event_date)` 按 player_id 分组求首次登录日期，然后与原表连接查找次日记录。

3. **窗口函数方案**：用 `FIRST_VALUE` 或 `MIN() OVER()` 为每行标注首次登录日期，然后筛选。

4. **计算比例**：满足条件的玩家数 / 总玩家数，ROUND 到两位小数。

## 解法概览

| 解法 | 适用场景 | 推荐度 |
|------|---------|--------|
| 子查询 + LEFT JOIN | 所有 MySQL 版本 | ⭐⭐⭐⭐⭐ |
| 子查询 + IN/EXISTS | 所有 MySQL 版本 | ⭐⭐⭐⭐ |
| 窗口函数 | MySQL 8.0+ | ⭐⭐⭐⭐ |

## 关键提示

1. 先求每个玩家的首次登录日期：`SELECT player_id, MIN(event_date) AS first_login FROM Activity GROUP BY player_id`

2. 次日留存判断：检查 Activity 表中是否有 `(player_id, first_login + 1)` 的记录

3. 比例计算：`COUNT(满足条件的玩家) / COUNT(DISTINCT player_id)`，注意用 `ROUND(..., 2)`

4. DATE_ADD 或 INTERVAL 做日期加减：`DATE_ADD(first_login, INTERVAL 1 DAY)` 或 `first_login + 1`

## 解法详解

### 解法一：子查询 + LEFT JOIN（推荐）

```sql
SELECT 
    ROUND(
        COUNT(a.player_id) / (SELECT COUNT(DISTINCT player_id) FROM Activity),
        2
    ) AS fraction
FROM (
    SELECT player_id, MIN(event_date) AS first_login
    FROM Activity
    GROUP BY player_id
) f
LEFT JOIN Activity a
    ON f.player_id = a.player_id
    AND a.event_date = DATE_ADD(f.first_login, INTERVAL 1 DAY);
```

### 解法二：子查询 + IN

```sql
SELECT 
    ROUND(
        SUM(CASE WHEN (player_id, DATE_ADD(first_login, INTERVAL 1 DAY)) 
                  IN (SELECT player_id, event_date FROM Activity) 
             THEN 1 ELSE 0 END) 
        / COUNT(*),
        2
    ) AS fraction
FROM (
    SELECT player_id, MIN(event_date) AS first_login
    FROM Activity
    GROUP BY player_id
) t;
```

### 解法三：窗口函数

```sql
WITH first_login AS (
    SELECT 
        player_id,
        event_date,
        MIN(event_date) OVER (PARTITION BY player_id) AS first_date
    FROM Activity
)
SELECT 
    ROUND(
        COUNT(DISTINCT CASE 
            WHEN event_date = DATE_ADD(first_date, INTERVAL 1 DAY) 
            THEN player_id END) 
        / COUNT(DISTINCT player_id),
        2
    ) AS fraction
FROM first_login;
```

## 易错点

| 错误写法 | 正确写法 |
|---------|---------|
| ✗ 分母用 COUNT(player_id) 而非 COUNT(DISTINCT player_id) | ✓ 分母是总玩家数，需要 DISTINCT 或用子查询去重 |
| ✗ 日期加 1 写成 `first_login + 1`（MySQL 可能不支持） | ✓ 用 `DATE_ADD(first_login, INTERVAL 1 DAY)` |
| ✗ LEFT JOIN 后用 COUNT(*) 统计匹配数 | ✓ 用 COUNT(a.player_id) 排除 NULL（未匹配行） |
| ✗ 忘记 ROUND 到 2 位小数 | ✓ ROUND(result, 2) |

## 面试追问

**Q1: LEFT JOIN 和 INNER JOIN 在这里的区别？**

> LEFT JOIN 保留所有玩家（即使次日没登录也保留，a.player_id 为 NULL）。用 COUNT(a.player_id) 只计非 NULL 的（即有次日登录的）。如果用 INNER JOIN，分子正确但分母需要另外算（因为只保留了有次日登录的玩家）。

**Q2: 如果要计算第 N 天留存率而不是次日留存率？**

> 把 `INTERVAL 1 DAY` 改成 `INTERVAL N DAY`。或者用参数化查询。

**Q3: 如何优化大表性能？**

> 确保 (player_id, event_date) 上有索引。子查询中 GROUP BY player_id 的 MIN() 操作可以利用 B-tree 索引加速。

## 相关题型

- [511. 游戏玩法分析 I](https://leetcode.com/problems/game-play-analysis-i/) - 求首次登录日期
- [512. 游戏玩法分析 II](https://leetcode.com/problems/game-play-analysis-ii/) - 求首次登录设备
- [534. 游戏玩法分析 III](https://leetcode.com/problems/game-play-analysis-iii/) - 累计游戏数
- [1097. 游戏玩法分析 V](https://leetcode.com/problems/game-play-analysis-v/) - 留存率按日期统计
