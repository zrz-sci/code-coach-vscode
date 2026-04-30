# 534. 游戏玩法分析 III (Game Play Analysis III)

**难度**: Medium | **标签**: Database

## 核心思路

对每个玩家，按日期计算累计游戏数（running sum / prefix sum）。本质是**分组窗口函数**或**自连接累加**。

## 思维链

1. 按 player_id 分组
2. 在每个分组内，按 event_date 排序
3. 计算 games_played 的累计和（前缀和）
4. 输出 player_id, event_date, games_played_so_far

## 解法概览

| 解法 | 适用场景 | 推荐度 |
|------|---------|--------|
| 窗口函数 SUM() OVER() | MySQL 8.0+ | ⭐⭐⭐⭐⭐ |
| 自连接累加 | 所有 MySQL 版本 | ⭐⭐⭐⭐ |
| 相关子查询 | 所有 MySQL 版本 | ⭐⭐⭐ |

## 关键提示

1. 窗口函数是最优雅的解法：`SUM(games_played) OVER (PARTITION BY player_id ORDER BY event_date)`
2. 自连接方式：将 Activity 自连接，条件为同一玩家且日期 <= 当前日期，然后 GROUP BY 求 SUM
3. 注意 ORDER BY 必须按 event_date，确保累加顺序正确
4. ROWS BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW 是窗口函数的默认行为

## 解法详解

### 解法一：窗口函数（推荐）

```sql
SELECT 
    player_id,
    event_date,
    SUM(games_played) OVER (
        PARTITION BY player_id 
        ORDER BY event_date
    ) AS games_played_so_far
FROM Activity;
```

### 解法二：自连接累加

```sql
SELECT 
    a1.player_id,
    a1.event_date,
    SUM(a2.games_played) AS games_played_so_far
FROM Activity a1
JOIN Activity a2 
    ON a1.player_id = a2.player_id 
    AND a2.event_date <= a1.event_date
GROUP BY a1.player_id, a1.event_date;
```

### 解法三：相关子查询

```sql
SELECT 
    a1.player_id,
    a1.event_date,
    (SELECT SUM(a2.games_played) 
     FROM Activity a2 
     WHERE a2.player_id = a1.player_id 
       AND a2.event_date <= a1.event_date
    ) AS games_played_so_far
FROM Activity a1;
```

## 易错点

| 错误写法 | 正确写法 |
|---------|---------|
| ✗ 窗口函数忘记 PARTITION BY player_id | ✓ 必须按玩家分组，否则所有玩家累加在一起 |
| ✗ 自连接条件写成 `a2.event_date < a1.event_date` | ✓ 应该是 `<=`，包含当天 |
| ✗ 自连接忘记 GROUP BY | ✓ 必须 GROUP BY a1.player_id, a1.event_date |
| ✗ ORDER BY 写错列导致累加顺序错误 | ✓ 必须 ORDER BY event_date |

## 面试追问

**Q1: 窗口函数和自连接方式的性能差异？**

> 窗口函数通常更快，时间复杂度 O(n log n)（排序 + 扫描）。自连接是 O(n^2)（每行需要和之前所有行做连接）。

**Q2: 如果要计算每个玩家的移动平均（最近3天），怎么改？**

> 窗口函数加 ROWS 子句：`AVG(games_played) OVER (PARTITION BY player_id ORDER BY event_date ROWS BETWEEN 2 PRECEDING AND CURRENT ROW)`

**Q3: 如果 (player_id, event_date) 不是主键，同一天有多条记录怎么办？**

> 需要先用子查询按 (player_id, event_date) 聚合 SUM(games_played)，再在外层做累计。

## 相关题型

- [550. 游戏玩法分析 IV](https://leetcode.com/problems/game-play-analysis-iv/) - 次日留存率
- [1321. 餐馆营业额变化增长](https://leetcode.com/problems/restaurant-growth/) - 移动窗口累加
- [176. 第二高的薪水](https://leetcode.com/problems/second-highest-salary/) - SQL 基础
