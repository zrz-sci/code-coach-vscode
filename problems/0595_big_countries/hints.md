# 595. 大的国家 (Big Countries)

## 核心思路
从 `World` 表中筛选满足**面积 >= 3,000,000 或人口 >= 25,000,000** 的国家，返回国家名、人口和面积。这是一道基础 SQL 筛选题，核心考点是 `WHERE` 条件的组合方式以及 `OR` vs `UNION` 的性能差异。

## 思维链
1. 需求很直接：筛选满足两个条件之一的行
2. 最简单的方式是 `WHERE ... OR ...`
3. 但在生产环境中，如果 area 和 population 各自有索引，`OR` 可能导致全表扫描
4. `UNION` 拆成两个查询，每个查询可以独立走索引，合并后自动去重
5. `UNION ALL` 更快但会有重复（同时满足两个条件的国家出现两次），需要外层去重
6. 对于小表差异不大，但面试时能说出性能分析是加分项

## 解法概览
| 解法 | 特点 | 适用场景 |
|------|------|----------|
| WHERE + OR | 最简洁，一条语句 | 小表或无索引时 |
| UNION | 可利用各自的索引 | 大表且 area/population 有索引 |
| UNION ALL + DISTINCT | 避免 UNION 隐式排序去重 | 需要精细控制去重策略时 |

## 关键提示

### 提示 1 - WHERE + OR 基础写法
```sql
SELECT name, population, area
FROM World
WHERE area >= 3000000 OR population >= 25000000;
```
注意 `OR` 的短路求值：数据库可能不会短路，两个条件都会被评估。

### 提示 2 - UNION 优化索引利用
```sql
SELECT name, population, area FROM World WHERE area >= 3000000
UNION
SELECT name, population, area FROM World WHERE population >= 25000000;
```
`UNION` 自带去重（等价于 `UNION DISTINCT`），不需要额外处理重复行。

### 提示 3 - OR 的索引陷阱
当 `WHERE col1 = x OR col2 = y` 时，即使 col1 和 col2 分别有索引，MySQL 也可能退化为全表扫描。因为优化器难以同时利用两个不同列的索引。`UNION` 方案让每个子查询只涉及一列，可以各自走索引。

### 提示 4 - 实际生产中的考量
- 对于 World 表（约 200 个国家），任何方案都很快
- 对于亿级数据表，`UNION` + 各列索引的方案可能快几个数量级
- 还可以考虑覆盖索引（covering index）减少回表

## 解法详解

### 解法一：WHERE + OR（最简洁）
```sql
SELECT name, population, area
FROM World
WHERE area >= 3000000 OR population >= 25000000;
```
**说明**：直接用 `OR` 连接两个条件，简单明了。

### 解法二：UNION（索引友好）
```sql
SELECT name, population, area
FROM World
WHERE area >= 3000000

UNION

SELECT name, population, area
FROM World
WHERE population >= 25000000;
```
**说明**：
- 两个 SELECT 分别筛选，各自可走独立索引
- `UNION` 默认去重，同时满足两个条件的国家只出现一次
- 比 `UNION ALL` 慢一点（需要排序去重），但保证结果正确

### 解法三：UNION ALL + 外层去重
```sql
SELECT DISTINCT name, population, area
FROM (
    SELECT name, population, area FROM World WHERE area >= 3000000
    UNION ALL
    SELECT name, population, area FROM World WHERE population >= 25000000
) t;
```
**说明**：`UNION ALL` 不去重更快，通过外层 `DISTINCT` 手动去重。但通常直接用 `UNION` 更简洁。

## 易错点
1. **混淆 AND 和 OR**：题目要求面积大**或**人口多，用 `OR` 而非 `AND`
2. **忘记 UNION 自动去重**：`UNION` = `UNION DISTINCT`，不需要额外 `DISTINCT`
3. **UNION ALL 导致重复行**：如果用 `UNION ALL`，同时满足两个条件的国家会出现两次
4. **列名顺序不匹配**：`UNION` 的两个 SELECT 列数和类型必须一致
5. **数值常量写错**：3,000,000 在 SQL 中写作 `3000000`（无逗号）

## 面试追问
1. **OR 和 UNION 的性能差异？** - OR 可能导致全表扫描；UNION 的子查询可各自利用索引，但有去重开销
2. **如何为这张表建索引？** - `CREATE INDEX idx_area ON World(area)` 和 `CREATE INDEX idx_pop ON World(population)`
3. **如果条件是 AND 而非 OR？** - 可以建联合索引 `(area, population)` 更高效
4. **UNION vs UNION ALL 何时选择？** - 确定无重复时用 UNION ALL 更快；有重复时用 UNION 保证正确性
5. **如何用 EXISTS 改写？** - 不适合此场景，EXISTS 适合关联子查询

## 相关题型
- [596. Classes With at Least 5 Students](https://leetcode.com/problems/classes-more-than-5-students/) - GROUP BY + HAVING
- [182. Duplicate Emails](https://leetcode.com/problems/duplicate-emails/) - GROUP BY 筛选
- [175. Combine Two Tables](https://leetcode.com/problems/combine-two-tables/) - LEFT JOIN 基础
- [181. Employees Earning More Than Their Managers](https://leetcode.com/problems/employees-earning-more-than-their-managers/) - 自连接
- [197. Rising Temperature](https://leetcode.com/problems/rising-temperature/) - 日期比较
