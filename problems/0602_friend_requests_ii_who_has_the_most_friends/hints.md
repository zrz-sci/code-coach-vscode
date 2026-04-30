# 602. 好友申请 II：谁有最多的好友

## 核心思路

友谊关系是**双向**的：每条记录中 requester_id 和 accepter_id 都获得一个好友。因此需要将两列"拆开"后合并计数，再取最大值。关键技巧是 **UNION ALL 展开双向边 + GROUP BY 聚合**。

## 思维链

1. **识别双向关系** — 表中每行 `(requester_id, accepter_id)` 表示一对好友关系，requester 和 accepter 各自获得一位好友
2. **展开为单向记录** — 用 UNION ALL 将 requester_id 和 accepter_id 分别作为 "id" 输出，得到每人每次出现的记录
3. **按 id 聚合计数** — GROUP BY id，COUNT(*) 得到每人的好友总数
4. **取最大值** — ORDER BY num DESC LIMIT 1，或用子查询 / 窗口函数取最大

## 解法概览

| 解法 | 思路 | 时间复杂度 | 推荐度 |
|------|------|-----------|--------|
| UNION ALL + GROUP BY + LIMIT | 展开双向边后聚合取最大 | O(n) | ⭐⭐⭐⭐⭐ |
| UNION ALL + RANK() 窗口函数 | 展开后用窗口函数排名 | O(n log n) | ⭐⭐⭐⭐ |
| 子查询 + HAVING | 两次子查询分别计数后合并 | O(n) | ⭐⭐⭐ |

## 关键提示

- **UNION ALL 而非 UNION** — 同一人可能同时作为 requester 和 accepter 出现，不能去重
- **友谊是双向的** — 一条记录贡献两个计数（requester +1，accepter +1）
- **LIMIT 1 vs 窗口函数** — 题目保证唯一最大值，用 LIMIT 1 即可；Follow-up 需用 RANK()

## 解法详解

### 解法一：UNION ALL + GROUP BY（推荐）

```sql
-- 步骤 1: 展开双向好友关系
-- 步骤 2: 聚合计数
-- 步骤 3: 取最大值
SELECT id, COUNT(*) AS num
FROM (
    SELECT requester_id AS id FROM RequestAccepted
    UNION ALL
    SELECT accepter_id AS id FROM RequestAccepted
) t
GROUP BY id
ORDER BY num DESC
LIMIT 1;
```

**执行逻辑：**
- UNION ALL 把 requester_id 和 accepter_id 各自作为独立行输出
- 对于示例数据 `(1,2), (1,3), (2,3), (3,4)`，展开后 id=3 出现 3 次（作为 accepter 2 次 + requester 1 次）
- GROUP BY + COUNT 得到每人好友数，ORDER BY DESC + LIMIT 1 取最大

### 解法二：RANK() 窗口函数（Follow-up 适用）

```sql
-- 支持多个人并列最多好友的情况
SELECT id, num
FROM (
    SELECT id, COUNT(*) AS num,
           RANK() OVER (ORDER BY COUNT(*) DESC) AS rk
    FROM (
        SELECT requester_id AS id FROM RequestAccepted
        UNION ALL
        SELECT accepter_id AS id FROM RequestAccepted
    ) t
    GROUP BY id
) ranked
WHERE rk = 1;
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ 只统计 requester_id 的 COUNT | ✓ UNION ALL 两列再 COUNT | 友谊是双向的，两边都要计数 |
| ✗ 使用 UNION（去重） | ✓ 使用 UNION ALL（保留重复） | UNION 会丢失有效计数记录 |
| ✗ 分别 COUNT 再相加 | ✓ 先 UNION ALL 再一次 GROUP BY | 分别计数后合并需要 COALESCE 处理 NULL，更繁琐 |
| ✗ WHERE num = MAX(num) | ✓ ORDER BY num DESC LIMIT 1 | WHERE 子句不能直接用聚合函数 |

## 面试追问

**Q1: 如果多人并列最多好友，如何返回所有人？**
> 使用 RANK() 窗口函数代替 LIMIT 1。RANK() OVER (ORDER BY COUNT(*) DESC) 为每人打排名，取 rk = 1 的所有行即可。DENSE_RANK 在此场景下效果相同。

**Q2: 如何优化大数据量下的查询性能？**
> 可在 requester_id 和 accepter_id 上建立索引。UNION ALL 的两部分可以并行扫描。如果频繁查询，可建立物化视图预先聚合好友计数。另外可以用 SUM + CASE 代替 UNION ALL 来减少一次全表扫描。

**Q3: 如果表中可能有重复的好友请求记录，如何处理？**
> 先对 `(requester_id, accepter_id)` 做 DISTINCT 去重，再进行 UNION ALL 展开。或在建表时通过主键/唯一约束防止重复数据。

## 相关题型

- [1264. 页面推荐](../1264_page_recommendations/) — 好友关系 + 推荐，同样需要处理双向关系
- [1501. 可以放心投资的国家](../1501_countries_you_can_safely_invest_in/) — 多表 JOIN + 聚合
- [570. 至少有5名直接下属的经理](../0570_managers_with_at_least_5_direct_reports/) — GROUP BY + HAVING 聚合模式
- [1148. 文章浏览 I](../1148_article_views_i/) — 基础 SQL 聚合查询
