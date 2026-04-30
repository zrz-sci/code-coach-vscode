## 核心思路

按 player_id 分组，取每组中 event_date 的最小值，即为每位玩家的首次登录日期。

## 思维链

1. **需求分析**：找每位玩家的第一次登录日期
2. **分组依据**：按 player_id 分组
3. **聚合函数**：对 event_date 取 MIN
4. **输出列名**：player_id, first_login

## 解法概览

| 解法 | 思路 | 推荐 |
|------|------|------|
| GROUP BY + MIN | 分组取最小日期 | ⭐ |
| 窗口函数 + RANK | 排名取第一行 | |
| 子查询 + IN | 子查询找最小日期再关联 | |

## 关键提示

- 这是最基础的 GROUP BY + 聚合函数题型
- 注意输出列的别名要与期望一致：`first_login`
- 不需要 ORDER BY，题目说"任意顺序"

## 解法详解

### 解法一：GROUP BY + MIN（推荐）

```sql
SELECT player_id, MIN(event_date) AS first_login
FROM Activity
GROUP BY player_id;
```

### 解法二：窗口函数

```sql
SELECT player_id, event_date AS first_login
FROM (
    SELECT player_id, event_date,
           RANK() OVER (PARTITION BY player_id ORDER BY event_date) AS rk
    FROM Activity
) t
WHERE rk = 1;
```

### 解法三：子查询

```sql
SELECT player_id, event_date AS first_login
FROM Activity
WHERE (player_id, event_date) IN (
    SELECT player_id, MIN(event_date)
    FROM Activity
    GROUP BY player_id
);
```

## 易错点

| ✗ 错误写法 | ✓ 正确写法 | 说明 |
|-----------|-----------|------|
| `SELECT player_id, event_date` 不加 GROUP BY | 加 `GROUP BY player_id` | 非聚合列必须在 GROUP BY 中 |
| `FIRST(event_date)` | `MIN(event_date)` | SQL 标准没有 FIRST 函数 |
| 忘记别名 `AS first_login` | 加上别名 | 输出列名需匹配 |

## 面试追问

**Q1: 如果还要返回首次登录使用的设备呢？（LC 512）**
> 需要用窗口函数或子查询关联，因为 GROUP BY + MIN 只能拿到日期，无法确定对应的 device_id。

**Q2: 如果数据量很大，如何优化？**
> 在 (player_id, event_date) 上建联合索引，GROUP BY 可利用索引排序。

**Q3: GROUP BY + MIN 和窗口函数哪个更高效？**
> 通常 GROUP BY + MIN 更高效，因为不需要为每行计算排名。

## 相关题型

- [511. Game Play Analysis I](https://leetcode.com/problems/game-play-analysis-i/) - 本题
- [512. Game Play Analysis II](https://leetcode.com/problems/game-play-analysis-ii/) - 首次登录设备
- [550. Game Play Analysis IV](https://leetcode.com/problems/game-play-analysis-iv/) - 连续登录
