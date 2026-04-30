# 603. 连续空余座位

## 核心思路

找**相邻的两个空座位**：如果一个座位是空的，且它的前一个或后一个座位也是空的，那么它就属于"连续空余座位"。核心技巧是 **自连接（Self JOIN）** 判断相邻关系，或使用 **窗口函数 LAG/LEAD** 检查前后座位状态。

## 思维链

1. **理解"连续"** — seat_id 是自增的，相邻座位的 id 差值为 1
2. **筛选空座位** — free = 1 表示空闲
3. **判断相邻关系** — 座位 A 与座位 B 相邻 iff |A.seat_id - B.seat_id| = 1
4. **两个方向判断** — 一个空座位只要"前面"或"后面"有一个空座位，它就属于连续组
5. **去重排序** — 同一个 seat_id 可能匹配到前后两个邻居，需要 DISTINCT

## 解法概览

| 解法 | 思路 | 时间复杂度 | 推荐度 |
|------|------|-----------|--------|
| Self JOIN | 自连接判断相邻空座位 | O(n) | ⭐⭐⭐⭐⭐ |
| LAG/LEAD 窗口函数 | 检查前后行的 free 状态 | O(n) | ⭐⭐⭐⭐⭐ |
| 差值分组法 | seat_id - ROW_NUMBER 标记连续段 | O(n log n) | ⭐⭐⭐ |

## 关键提示

- **DISTINCT 去重** — 自连接中一个座位可能同时匹配前后两个空座位，结果需要去重
- **ABS 或双向条件** — 相邻关系可以用 `ABS(a.seat_id - b.seat_id) = 1`，也可以分两个条件 `a.seat_id = b.seat_id + 1 OR a.seat_id = b.seat_id - 1`
- **两个座位都要 free=1** — JOIN 条件中两侧都必须是空座位
- **结果要排序** — 题目明确要求按 seat_id 升序返回

## 解法详解

### 解法一：自连接 Self JOIN（推荐）

```sql
SELECT DISTINCT a.seat_id
FROM Cinema a
JOIN Cinema b
  ON ABS(a.seat_id - b.seat_id) = 1
WHERE a.free = 1 AND b.free = 1
ORDER BY a.seat_id;
```

**执行逻辑：**
- 表自连接，连接条件是两个座位 id 差的绝对值为 1（相邻）
- WHERE 子句确保两个座位都是空的
- DISTINCT 去重，因为座位 4 可能同时匹配座位 3 和座位 5
- 示例：seat 3 匹配 seat 4, seat 4 匹配 seat 3 和 5, seat 5 匹配 seat 4

### 解法二：LAG/LEAD 窗口函数

```sql
SELECT seat_id
FROM (
    SELECT seat_id, free,
           LAG(free) OVER (ORDER BY seat_id) AS prev_free,
           LEAD(free) OVER (ORDER BY seat_id) AS next_free
    FROM Cinema
) t
WHERE free = 1
  AND (prev_free = 1 OR next_free = 1)
ORDER BY seat_id;
```

**执行逻辑：**
- LAG 获取前一行的 free 值，LEAD 获取后一行的 free 值
- 当前座位空闲且前一个或后一个也空闲，即为连续空余座位
- 无需去重，每个座位只出现一次

### 解法三：差值分组法

```sql
SELECT seat_id
FROM (
    SELECT seat_id,
           COUNT(*) OVER (
               PARTITION BY seat_id - ROW_NUMBER() OVER (ORDER BY seat_id)
           ) AS grp_size
    FROM Cinema
    WHERE free = 1
) t
WHERE grp_size >= 2
ORDER BY seat_id;
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ 不加 DISTINCT | ✓ SELECT DISTINCT a.seat_id | 自连接中一个座位可匹配多个邻居 |
| ✗ 只检查 a.free = 1 | ✓ a.free = 1 AND b.free = 1 | 必须两个相邻座位都是空的 |
| ✗ ON a.seat_id = b.seat_id + 1 | ✓ ON ABS(a.seat_id - b.seat_id) = 1 | 单向只能匹配一个方向的邻居 |
| ✗ 忘记 ORDER BY | ✓ ORDER BY a.seat_id | 题目明确要求按 seat_id 升序 |

## 面试追问

**Q1: 如果要找至少连续 N 个空座位，如何修改？**
> 使用差值分组法：`seat_id - ROW_NUMBER()` 相同的属于同一连续段，用 COUNT(*) OVER (PARTITION BY grp) 计算段长度，筛选 >= N 的段。或者用多次 JOIN 检查连续 N 个座位。

**Q2: 自连接和窗口函数方案哪个性能更好？**
> 窗口函数通常更优：只需一次全表扫描 + 排序，而自连接需要两次扫描 + JOIN 操作。对于大表，窗口函数避免了笛卡尔积的开销。但如果 seat_id 有索引，自连接的 JOIN 也能利用索引查找。

**Q3: 如果 seat_id 不连续（比如 1, 3, 4, 5），逻辑需要改变吗？**
> 如果"相邻"定义为物理上挨着的座位，seat_id 不连续时自连接的 ABS 差值为 1 不再正确。此时应改用 LAG/LEAD 窗口函数，它按行顺序取前后行，不依赖 seat_id 的连续性。

## 相关题型

- [180. 连续出现的数字](../0180_consecutive_numbers/) — 经典连续元素问题，自连接 / 窗口函数
- [601. 体育馆的人流量](../0601_human_traffic_of_stadium/) — 连续至少三行，差值分组法
- [1225. 报告系统状态的连续日期](../1225_report_contiguous_dates/) — 连续日期段合并
- [197. 上升的温度](../0197_rising_temperature/) — 相邻行比较，DATEDIFF / LAG
