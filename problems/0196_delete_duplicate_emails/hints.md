# 196. 删除重复的电子邮箱 (Delete Duplicate Emails)

**难度**: Easy | **标签**: Database

## 核心思路

自连接（Self Join）找到同 email 中 id 较大的行并删除。关键在于写 DELETE 而非 SELECT，且保留最小 id。

## 思维链

1. 需要找到重复的 email → 同一 email 出现多次
2. 对于每个重复 email，保留 id 最小的那一行
3. 删除所有 id 不是最小的重复行
4. 可以通过自连接或子查询实现

## 解法概览

| 解法 | 思路 | 推荐 |
|------|------|------|
| 自连接 DELETE | JOIN 自身，删除 id 较大者 | ⭐ |
| 子查询 DELETE | NOT IN 保留最小 id 集合 | ⭐ |
| 窗口函数 | ROW_NUMBER() 标记后删除 | |

## 关键提示

1. 自连接条件：`p1.email = p2.email AND p1.id > p2.id`
2. MySQL 不允许在 DELETE 的子查询中直接引用被删除的表 → 需要嵌套一层
3. DELETE 语句中需要指定删除哪个表的别名

## 解法详解

### 解法一：自连接 DELETE（推荐）

```sql
DELETE p1
FROM Person p1, Person p2
WHERE p1.email = p2.email
  AND p1.id > p2.id;
```

**原理**：对于相同 email 的两行，删除 id 较大的那行。

### 解法二：子查询 DELETE

```sql
DELETE FROM Person
WHERE id NOT IN (
    SELECT id FROM (
        SELECT MIN(id) AS id
        FROM Person
        GROUP BY email
    ) AS tmp
);
```

**原理**：先找出每个 email 的最小 id，不在这个集合中的行全部删除。

### 解法三：窗口函数（MySQL 8.0+）

```sql
DELETE FROM Person
WHERE id IN (
    SELECT id FROM (
        SELECT id,
               ROW_NUMBER() OVER (PARTITION BY email ORDER BY id) AS rn
        FROM Person
    ) AS ranked
    WHERE rn > 1
);
```

## 易错点

1. ✗ 写成 SELECT 语句而非 DELETE
   ✓ 题目要求必须是 DELETE 操作

2. ✗ `DELETE FROM Person WHERE id NOT IN (SELECT MIN(id) FROM Person GROUP BY email)`
   MySQL 报错：不能在子查询中直接引用被修改的表
   ✓ 需要额外包一层：`... NOT IN (SELECT id FROM (SELECT MIN(id) AS id ...) AS tmp)`

3. ✗ `DELETE p1, p2 FROM Person p1 ...` 误删两边
   ✓ `DELETE p1 FROM ...` 只删除 p1（id 较大者）

## 面试追问

**Q1**: 如果要保留最大 id 而非最小 id，如何修改？
→ 自连接中条件改为 `p1.id < p2.id`

**Q2**: 为什么 MySQL 子查询中不能直接引用被 DELETE 的表？
→ MySQL 限制：同一语句中不能对同一个表同时进行修改和子查询读取

**Q3**: 如果数据量很大（百万级），哪种方案更高效？
→ 自连接方案通常更高效，子查询需要额外的临时表

## 相关题型

- 182: Duplicate Emails（SELECT 查找重复，本题前置）
- 175: Combine Two Tables（JOIN 基础）
- 176: Second Highest Salary（子查询）
