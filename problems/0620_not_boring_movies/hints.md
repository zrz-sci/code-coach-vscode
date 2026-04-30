# 620. 有趣的电影 (Not Boring Movies)

## 核心思路

筛选 id 为奇数且 description 不为 "boring" 的电影，按 rating 降序排列。关键在于**奇数判断**和**字符串不等比较**两个 WHERE 条件的组合。

## 思维链

1. **理解筛选条件**：题目要求两个同时满足的条件 —— id 为奇数 AND description 不是 "boring"
2. **奇数判断方式**：`id % 2 = 1` 或 `MOD(id, 2) = 1` 或位运算 `id & 1 = 1`
3. **字符串不等比较**：`description != 'boring'` 或 `description <> 'boring'`
4. **排序要求**：`ORDER BY rating DESC`
5. **组合查询**：将以上条件通过 AND 连接，SELECT 所有列

## 解法概览

| 解法 | 思路 | 时间复杂度 | 空间复杂度 | 推荐 |
|------|------|-----------|-----------|------|
| MOD 函数 | MOD(id, 2) = 1 判断奇数 | O(n) | O(1) | ⭐ |
| 取模运算符 | id % 2 = 1 判断奇数 | O(n) | O(1) | ⭐ |
| 位运算 | id & 1 = 1 判断奇数 | O(n) | O(1) | |

## 关键提示

1. **奇数判断**有多种写法：`% 2 = 1`、`MOD(id,2) = 1`、`id & 1` —— 面试中展示多种方式加分
2. **字符串比较**在 MySQL 中默认不区分大小写（取决于 collation），题目测试用例中 "boring" 固定为小写
3. **不等号写法**：`!=` 和 `<>` 在 SQL 中完全等价，`<>` 是 SQL 标准写法
4. **SELECT \***：题目要求返回所有列，可以用 `SELECT *` 简化
5. **排序方向**：注意是 DESC 降序，不要遗漏

## 解法详解

### 解法一：MOD 函数（推荐）

```sql
SELECT *
FROM cinema
WHERE MOD(id, 2) = 1
  AND description != 'boring'
ORDER BY rating DESC;
```

**逐行解析**：
- `MOD(id, 2) = 1`：MOD 是标准 SQL 函数，兼容性好
- `description != 'boring'`：排除无聊电影
- `ORDER BY rating DESC`：按评分降序输出

### 解法二：取模运算符

```sql
SELECT *
FROM cinema
WHERE id % 2 = 1
  AND description <> 'boring'
ORDER BY rating DESC;
```

**说明**：`%` 运算符是 MySQL 特有写法，`<>` 是 SQL 标准不等号。

### 解法三：位运算

```sql
SELECT *
FROM cinema
WHERE id & 1
  AND description != 'boring'
ORDER BY rating DESC;
```

**说明**：`id & 1` 利用位与运算，奇数的最低位为 1，结果非零即为 TRUE。这是性能最好的写法，但可读性略差。

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ `WHERE id % 2 = 0` | ✓ `WHERE id % 2 = 1` | 0 是偶数，1 才是奇数 |
| ✗ `ORDER BY rating` | ✓ `ORDER BY rating DESC` | 题目要求降序，默认是升序 |
| ✗ `description = 'not boring'` | ✓ `description != 'boring'` | 不是找描述为 "not boring" 的行，而是排除 "boring" |
| ✗ `description NOT LIKE '%boring%'` | ✓ `description != 'boring'` | 题目要求精确匹配排除，不是模糊匹配 |
| ✗ `WHERE id % 2 = 1 OR description != 'boring'` | ✓ `WHERE id % 2 = 1 AND description != 'boring'` | 两个条件是同时满足（AND），不是或（OR） |

## 面试追问

**Q1: MOD 函数、% 运算符和位运算 & 在判断奇偶时有什么区别？性能如何？**

A: `MOD()` 是标准 SQL 函数，跨数据库兼容性最好。`%` 是 MySQL 特有运算符，写法简洁。`& 1` 位运算性能最优（CPU 级别操作），但可读性差。在大数据量下位运算有微小性能优势，但在有索引的情况下差异可忽略。面试中建议先写 `MOD` 再提及位运算作为优化方案。

**Q2: 如果 description 列可能包含 NULL 值，当前查询能正确处理吗？**

A: `description != 'boring'` 在 MySQL 中，NULL 与任何值比较都返回 NULL（即 FALSE），因此 NULL 行不会出现在结果中。如果需要包含 NULL 行，应改为 `(description != 'boring' OR description IS NULL)`。这是 SQL 三值逻辑的经典考点。

**Q3: 如果需要在百万级数据上优化这个查询，你会怎么做？**

A: (1) 在 `id` 列上利用主键索引，但 `MOD(id, 2) = 1` 无法利用索引（函数作用于索引列会导致全表扫描）。可以考虑创建生成列 `is_odd TINYINT GENERATED ALWAYS AS (id % 2) STORED` 并建索引。(2) 在 `rating` 列建索引可加速排序。(3) 如果 `description` 列区分度高，可以建索引配合 `!= 'boring'` 使用（但不等条件索引效率低）。(4) 使用覆盖索引 `(id, description, rating)` 避免回表。

## 相关题型

- [595. 大的国家](../0595_big_countries/) - 简单 WHERE 过滤
- [596. 超过5名学生的课](../0596_classes_more_than_5_students/) - GROUP BY + HAVING 筛选
- [626. 换座位](../0626_exchange_seats/) - 同样涉及奇偶判断的 SQL 题
- [1179. 重新格式化部门表](../1179_reformat_department_table/) - SQL 数据处理
