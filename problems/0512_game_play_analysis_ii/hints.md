## 核心思路

找每位玩家首次登录时使用的设备。核心难点：GROUP BY + MIN 只能拿到日期，需要用子查询或窗口函数关联回原表获取 device_id。

## 思维链

1. **需求分析**：找每位玩家第一次登录时使用的设备 ID
2. **难点**：MIN(event_date) 和 device_id 不在同一聚合层级
3. **方案一**：先找最小日期，再关联原表取 device_id
4. **方案二**：窗口函数按日期排名，取第一行

## 解法概览

| 解法 | 思路 | 推荐 |
|------|------|------|
| 子查询 + JOIN | 先找最小日期，再关联原表 | ⭐ |
| 窗口函数 | RANK/ROW_NUMBER 取排名第一 | ⭐ |
| 子查询 + IN | 元组匹配 | |

## 关键提示

- 不能直接 `SELECT player_id, device_id, MIN(event_date) GROUP BY player_id`，因为 device_id 不在 GROUP BY 中
- 主键是 (player_id, event_date)，所以同一天同一玩家只有一条记录
- 窗口函数的 ROW_NUMBER 和 RANK 在此题效果相同（主键保证无并列）

## 解法详解

### 解法一：子查询 + IN（推荐）

```sql
SELECT player_id, device_id
FROM Activity
WHERE (player_id, event_date) IN (
    SELECT player_id, MIN(event_date)
    FROM Activity
    GROUP BY player_id
);
```

### 解法二：窗口函数

```sql
SELECT player_id, device_id
FROM (
    SELECT player_id, device_id,
           ROW_NUMBER() OVER (PARTITION BY player_id ORDER BY event_date) AS rn
    FROM Activity
) t
WHERE rn = 1;
```

### 解法三：子查询 + JOIN

```sql
SELECT a.player_id, a.device_id
FROM Activity a
JOIN (
    SELECT player_id, MIN(event_date) AS min_date
    FROM Activity
    GROUP BY player_id
) b ON a.player_id = b.player_id AND a.event_date = b.min_date;
```

## 易错点

| ✗ 错误写法 | ✓ 正确写法 | 说明 |
|-----------|-----------|------|
| `SELECT player_id, device_id, MIN(event_date) GROUP BY player_id` | 子查询/窗口函数 | device_id 不在 GROUP BY 中，SQL 标准不允许 |
| `RANK()` 可能并列 | `ROW_NUMBER()` 保证唯一 | 此题主键保证无并列，两者等价 |
| 忘记外层 WHERE rn = 1 | 加上过滤条件 | 窗口函数只是加列，需额外过滤 |

## 面试追问

**Q1: 如果同一天同一玩家可能有多条记录（无主键），如何处理？**
> 用 ROW_NUMBER 而非 RANK，确保每人只选一条。

**Q2: 子查询 + IN vs JOIN 的性能差异？**
> 通常 JOIN 更优，因为 IN 子查询在某些数据库（如 MySQL 旧版本）可能被转为关联子查询。

**Q3: 如何扩展为"找每位玩家最后一次登录的设备"？**
> 将 MIN 改为 MAX，或将 ORDER BY event_date 改为 DESC。

## 相关题型

- [511. Game Play Analysis I](https://leetcode.com/problems/game-play-analysis-i/) - 首次登录日期
- [512. Game Play Analysis II](https://leetcode.com/problems/game-play-analysis-ii/) - 本题
- [534. Game Play Analysis III](https://leetcode.com/problems/game-play-analysis-iii/) - 累计游戏数
