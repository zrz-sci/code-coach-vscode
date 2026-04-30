# 597. 好友申请 I：总体通过率 (Friend Requests I: Overall Acceptance Rate)

## 核心思路
两表独立去重计数后相除，注意处理分母为零的边界情况。

## 思维链
1. FriendRequest 表有重复行 → 需要按 (sender_id, send_to_id) 去重
2. RequestAccepted 表有重复行 → 需要按 (requester_id, accepter_id) 去重
3. 通过率 = 去重后的接受数 / 去重后的请求数
4. 请求数为 0 时返回 0.00 → 用 IFNULL 或 COALESCE 处理
5. 结果保留两位小数 → ROUND(..., 2)

## 解法概览表

| 解法 | 思路 | 时间复杂度 | 难度 |
|------|------|-----------|------|
| 子查询除法 ⭐ | 两个独立 COUNT(DISTINCT) 子查询相除 | O(n+m) | Easy |
| CTE 写法 | WITH 分别计算再 SELECT | O(n+m) | Easy |

## 关键提示
1. **两表独立**: 被接受的请求不一定在 FriendRequest 表中，两表分开计算
2. **去重维度**: 按 (sender_id, send_to_id) 和 (requester_id, accepter_id) 对去重，不是按单列去重
3. **空表处理**: 当 FriendRequest 为空时 COUNT(DISTINCT ...) 返回 0，除法会得到 NULL

## 解法详解

### 解法一：子查询除法 ⭐
```sql
SELECT ROUND(
    IFNULL(
        (SELECT COUNT(DISTINCT requester_id, accepter_id) FROM RequestAccepted)
        /
        (SELECT COUNT(DISTINCT sender_id, send_to_id) FROM FriendRequest),
        0
    ), 2
) AS accept_rate;
```
**关键**: MySQL 的 COUNT(DISTINCT a, b) 可以同时对多列去重

### 解法二：CTE 写法
```sql
WITH req AS (
    SELECT COUNT(DISTINCT sender_id, send_to_id) AS cnt FROM FriendRequest
),
acc AS (
    SELECT COUNT(DISTINCT requester_id, accepter_id) AS cnt FROM RequestAccepted
)
SELECT ROUND(IFNULL(acc.cnt / req.cnt, 0), 2) AS accept_rate
FROM req, acc;
```

## 易错点

| ✗ 错误写法 | ✓ 正确写法 | 原因 |
|-----------|-----------|------|
| `COUNT(DISTINCT sender_id)` | `COUNT(DISTINCT sender_id, send_to_id)` | 去重维度必须是一对 (sender, receiver) |
| 直接除法不处理 NULL | `IFNULL(..., 0)` | 分母为 0 时除法返回 NULL |
| 不用 ROUND | `ROUND(..., 2)` | 题目要求两位小数 |
| JOIN 两表 | 两个独立子查询 | 两表无关联关系，独立计算 |

## 面试追问

**Q1: 如何计算每个月的通过率？**
按月份 GROUP BY，在 FriendRequest 中按 request_date 的月份分组统计请求数，在 RequestAccepted 中按 accept_date 的月份分组统计接受数，再 JOIN 月份计算。

**Q2: 如何计算每一天的累积通过率？**
使用窗口函数 SUM() OVER (ORDER BY date) 对去重后的日计数做累积求和，再相除。

**Q3: 如果两表数据量很大（亿级），如何优化？**
可以先在应用层或 ETL 阶段对两表分别做预聚合（按天/月汇总去重计数），减少实时计算量。也可以创建索引 (sender_id, send_to_id) 和 (requester_id, accepter_id)。

## 相关题型
- 570. 至少有5名直接下属的经理
- 574. 当选者
- 602. 好友申请 II：谁有最多的好友
