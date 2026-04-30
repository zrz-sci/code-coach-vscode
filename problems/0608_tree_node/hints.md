# 608. Tree Node (树节点)

**难度**: Medium | **标签**: Database | **同题**: 3054. Binary Tree Nodes

## 核心思路

根据节点的父子关系判断每个节点的类型:
- **Root**: `p_id IS NULL` (没有父节点)
- **Leaf**: 该节点的 `id` 没有出现在任何其他节点的 `p_id` 中 (没有子节点)
- **Inner**: 既有父节点又有子节点 (既不是 Root 也不是 Leaf)

核心技巧是使用 `CASE WHEN` + 子查询/LEFT JOIN 判断某个节点是否被其他节点引用为父节点。

## 思维链

1. **理解树结构**: 表中 `id` 是节点自身, `p_id` 是其父节点
2. **Root 判定**: `p_id IS NULL` → 没有父节点的就是根
3. **Leaf 判定**: 该 `id` 不在 `p_id` 列中 → 没有被任何节点引用为父
4. **Inner 判定**: 既有 `p_id`(非 NULL), 且自身 `id` 存在于其他行的 `p_id` 中
5. **用 CASE WHEN 三分支**: 先判 Root, 再判 Leaf, 剩下就是 Inner
6. **子查询 vs JOIN**: 子查询方式用 `IN`, JOIN 方式用 `LEFT JOIN` + `IS NOT NULL`

## 解法概览

| 解法 | 思路 | 时间复杂度 | 推荐度 |
|------|------|-----------|--------|
| CASE + IN 子查询 | 子查询判断是否为父节点 | O(n^2) | ⭐⭐⭐⭐⭐ |
| CASE + LEFT JOIN | JOIN 自身表判断子节点 | O(n log n) | ⭐⭐⭐⭐ |
| UNION ALL 分治 | 分别查 Root/Inner/Leaf 再合并 | O(n^2) | ⭐⭐⭐ |
| IF 函数简写 | 用 MySQL IF 替代 CASE | O(n^2) | ⭐⭐⭐ |

## 关键提示

1. `p_id IS NULL` 是判定根节点的唯一条件
2. 子查询 `id IN (SELECT p_id FROM Tree)` 可以判断节点是否有子节点
3. 注意 `IN` 子查询中 `p_id` 可能包含 NULL, 但不影响 `IN` 判断
4. 只有一个节点的树, 该节点既是 Root 也可以理解为 Leaf, 但题目要求输出 Root
5. CASE WHEN 的分支顺序很重要 -- 先判 Root, 再用 IN 判是否有子节点

## 解法详解

### 解法一: CASE + IN 子查询 (推荐)

```sql
SELECT
    id,
    CASE
        WHEN p_id IS NULL THEN 'Root'
        WHEN id IN (SELECT p_id FROM Tree) THEN 'Inner'
        ELSE 'Leaf'
    END AS type
FROM Tree;
```

**思路**: 对每个节点, 先看有没有父节点 (Root), 再看自己是否是别人的父节点 (Inner), 都不是就是 Leaf。

### 解法二: CASE + LEFT JOIN

```sql
SELECT
    t1.id,
    CASE
        WHEN t1.p_id IS NULL THEN 'Root'
        WHEN t2.id IS NOT NULL THEN 'Inner'
        ELSE 'Leaf'
    END AS type
FROM Tree t1
LEFT JOIN (SELECT DISTINCT p_id FROM Tree WHERE p_id IS NOT NULL) t2
    ON t1.id = t2.p_id;
```

**思路**: LEFT JOIN 去重后的父节点表, 如果能匹配上说明当前节点有子节点。

### 解法三: UNION ALL 分治

```sql
SELECT id, 'Root' AS type FROM Tree WHERE p_id IS NULL
UNION ALL
SELECT id, 'Inner' AS type FROM Tree
WHERE p_id IS NOT NULL AND id IN (SELECT p_id FROM Tree)
UNION ALL
SELECT id, 'Leaf' AS type FROM Tree
WHERE p_id IS NOT NULL AND id NOT IN (SELECT p_id FROM Tree WHERE p_id IS NOT NULL);
```

**思路**: 分别筛选三种类型, 用 UNION ALL 合并。注意 NOT IN 子查询需排除 NULL。

### 解法四: IF 函数简写 (MySQL)

```sql
SELECT
    id,
    IF(p_id IS NULL, 'Root',
       IF(id IN (SELECT p_id FROM Tree), 'Inner', 'Leaf')
    ) AS type
FROM Tree;
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ `WHEN id NOT IN (SELECT p_id FROM Tree)` | ✓ `WHEN id NOT IN (SELECT p_id FROM Tree WHERE p_id IS NOT NULL)` | NOT IN 子查询含 NULL 会导致结果为空 |
| ✗ 先判 Inner 再判 Root | ✓ 先判 Root (p_id IS NULL) 再判 Inner | 单节点树的根也满足 "无子节点", 先判 Root 避免误分 |
| ✗ `WHEN p_id IS NOT NULL THEN 'Inner'` | ✓ 需同时检查有子节点 | 有父节点不代表是 Inner, 还需有子节点 |
| ✗ `LEFT JOIN Tree t2 ON t1.id = t2.p_id` 不去重 | ✓ 先 DISTINCT 或用 EXISTS | 不去重会产生重复行 |

## 面试追问

**Q1: 如果树可能有多个根节点 (森林), 你的查询还能正确工作吗?**

A: 可以。`p_id IS NULL` 的判断对多个根节点同样有效, 每个无父节点的都会被标记为 Root。IN 子查询判断子节点也不受影响。

**Q2: IN 子查询 vs EXISTS 子查询, 哪个性能更好? 为什么?**

A: 当子查询结果集较小时, `IN` 更简洁高效; 当主表较小而子查询结果集很大时, `EXISTS` 通常更好, 因为 EXISTS 找到第一个匹配即停止, 而 IN 需要完整遍历子查询。对本题数据量级来说差异不大, 但大表场景推荐 EXISTS。

**Q3: 如何只用一次表扫描完成分类, 不使用子查询?**

A: 可以用窗口函数或自连接 + GROUP BY:
```sql
SELECT t1.id,
       CASE
           WHEN t1.p_id IS NULL THEN 'Root'
           WHEN COUNT(t2.id) > 0 THEN 'Inner'
           ELSE 'Leaf'
       END AS type
FROM Tree t1
LEFT JOIN Tree t2 ON t1.id = t2.p_id
GROUP BY t1.id, t1.p_id;
```
通过 GROUP BY + COUNT 避免子查询, 但本质仍是 JOIN 操作。

## 相关题型

| 题号 | 题目 | 关联 |
|------|------|------|
| 176 | Second Highest Salary | 子查询 + NULL 处理 |
| 196 | Delete Duplicate Emails | 自连接操作 |
| 3054 | Binary Tree Nodes | 同题 |
| 1270 | All People Report to Given Manager | 树形层级查询 |
| 1613 | Find the Missing IDs | NOT IN + 集合判断 |
