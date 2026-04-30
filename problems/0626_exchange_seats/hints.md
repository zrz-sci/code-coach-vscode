# 626. 换座位 (Exchange Seats)

## 核心思路

交换每两个相邻学生的座位号（id）。奇数 id 的学生与下一个偶数 id 的学生互换，如果总人数为奇数，最后一个学生不换。核心技巧是使用 **CASE WHEN** 根据 id 奇偶性重新分配 id，或使用**窗口函数 LEAD/LAG** 实现行间交换。

## 思维链

1. **理解交换规则**：id=1 与 id=2 互换，id=3 与 id=4 互换，依此类推
2. **奇数 id**：应变成 id+1（与下一个人换），但最后一个奇数 id 如果没有配对则保持不变
3. **偶数 id**：应变成 id-1（与上一个人换）
4. **最后一个奇数处理**：需要知道总人数，如果 id 是最后一个且为奇数，不变
5. **CASE WHEN 方案**：直接对 id 做条件变换
6. **位运算技巧**：奇偶判断 `id % 2 = 1` 或 `id & 1`
7. **排序**：按新 id 排序输出

## 解法概览

| 解法 | 思路 | 时间复杂度 | 空间复杂度 | 推荐 |
|------|------|-----------|-----------|------|
| CASE WHEN + 子查询 | 条件分支重新计算 id | O(n) | O(1) | ⭐ |
| CASE WHEN + COUNT窗口 | 避免子查询 | O(n) | O(1) | ⭐ |
| 位运算 XOR 技巧 | (id-1) XOR 1 + 1 | O(n) | O(1) | |
| 自连接 COALESCE | LEFT JOIN 偏移 | O(n) | O(n) | |

## 关键提示

1. **三种情况**：偶数 id -> id-1；奇数 id 且不是最后一个 -> id+1；奇数 id 且是最后一个 -> 保持 id
2. **获取总人数**：子查询 `(SELECT COUNT(*) FROM Seat)` 或窗口函数 `COUNT(*) OVER()`
3. **排序很重要**：必须按新 id 排序 (`ORDER BY id`)，否则输出顺序可能错误
4. **XOR 技巧**：`(id-1) ^ 1 + 1` 可以将奇偶互换（1->2, 2->1, 3->4, 4->3），但要小心最后一个奇数
5. **COALESCE 处理边界**：`COALESCE(next_student, current_student)` 可以优雅处理最后一个无配对的情况

## 解法详解

### 解法一：CASE WHEN + 子查询（推荐）

```sql
SELECT
    CASE
        WHEN id % 2 = 1 AND id = (SELECT COUNT(*) FROM Seat)
            THEN id                    -- 最后一个且为奇数: 不换
        WHEN id % 2 = 1
            THEN id + 1               -- 奇数: 换成下一个 id
        ELSE id - 1                   -- 偶数: 换成上一个 id
    END AS id,
    student
FROM Seat
ORDER BY id;
```

**逐行解析**：
- 第一个 WHEN：如果 id 是奇数且等于总行数（即最后一行），保持不变
- 第二个 WHEN：如果 id 是奇数（且不是最后一行），id+1
- ELSE：偶数行一律 id-1
- CASE 中条件有**先后优先级**，第一个匹配的条件生效

### 解法二：位运算 XOR 技巧

```sql
SELECT
    CASE
        WHEN id % 2 = 1 AND id = (SELECT COUNT(*) FROM Seat)
            THEN id
        ELSE (id - 1) ^ 1 + 1    -- XOR 位翻转实现奇偶互换
    END AS id,
    student
FROM Seat
ORDER BY id;
```

**说明**：`(id-1) ^ 1 + 1` 的原理：先减 1 使 id 变为 0-based，XOR 1 翻转最低位（0->1, 1->0），再加 1 回到 1-based。效果：1->2, 2->1, 3->4, 4->3。

### 解法三：COALESCE + 自连接

```sql
SELECT
    s1.id,
    COALESCE(s2.student, s1.student) AS student
FROM Seat s1
LEFT JOIN Seat s2
    ON (s1.id % 2 = 1 AND s2.id = s1.id + 1)
    OR (s1.id % 2 = 0 AND s2.id = s1.id - 1)
ORDER BY s1.id;
```

**说明**：奇数 id 行找下一行的 student，偶数 id 行找上一行的 student。如果找不到（最后一个奇数行），COALESCE 回退到自己的 student。

### 解法四：窗口函数 LEAD/LAG（MySQL 8.0+）

```sql
SELECT
    id,
    CASE
        WHEN id % 2 = 1
            THEN COALESCE(LEAD(student) OVER (ORDER BY id), student)
        ELSE LAG(student) OVER (ORDER BY id)
    END AS student
FROM Seat;
```

**说明**：奇数行取下一行的 student（LEAD），偶数行取上一行的 student（LAG）。COALESCE 处理最后一个奇数行没有下一行的情况。

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ 忘记处理最后一个奇数 id | ✓ 单独 WHEN 判断 `id = COUNT(*)` | 最后奇数行没有配对，不交换 |
| ✗ `ORDER BY student` | ✓ `ORDER BY id` | 必须按新 id 排序 |
| ✗ CASE 中先判断奇数再判断最后 | ✓ 先判断"奇数且是最后"再判断普通奇数 | CASE WHEN 按顺序匹配，特殊情况要先写 |
| ✗ `(id ^ 1)` 直接作为新 id | ✓ `(id-1) ^ 1 + 1` | XOR 要在 0-based 上操作再转回 1-based |
| ✗ 不加 ORDER BY | ✓ 必须 ORDER BY id ASC | 题目要求按 id 升序返回 |

## 面试追问

**Q1: CASE WHEN 中条件的顺序为什么重要？**

A: CASE WHEN 按顺序逐个检查条件，**第一个为 TRUE 的分支生效**，后续分支不再检查。因此"奇数且是最后一个"必须写在"奇数"之前，否则最后一个奇数行会被普通奇数分支匹配到，错误地执行 id+1。这类似编程中 if-else if 的顺序 —— 特殊情况要放前面。

**Q2: 如果 id 不是连续的（有间隔），这个解法还能用吗？**

A: 不能直接用。当前解法假设 id 从 1 开始连续递增（题目保证了这一点）。如果 id 有间隔（如 1, 3, 5, 8），id+1 和 id-1 不一定指向相邻行。此时应该改用窗口函数 `LEAD(student) OVER (ORDER BY id)` 和 `LAG(student) OVER (ORDER BY id)` 来获取物理上相邻的行，而不是依赖 id 的数值关系。

**Q3: 如果数据量非常大（百万级），哪种解法性能最优？**

A: (1) CASE WHEN + 子查询需要一次全表扫描 + 一次 COUNT 子查询（通常会被优化为常量）+ 一次排序，O(n log n)。(2) 自连接方案需要 JOIN 操作，可能产生大量中间结果。(3) 窗口函数方案需要排序但避免了子查询。实际中 CASE WHEN 方案最优，因为子查询 `COUNT(*)` 只执行一次，主查询是全表顺序扫描。如果 id 上有索引，排序也很高效。

## 相关题型

- [620. 有趣的电影](../0620_not_boring_movies/) - 同样涉及奇偶判断的 SQL 题
- [196. 删除重复的电子邮箱](../0196_delete_duplicate_emails/) - SQL 数据修改操作
- [197. 上升的温度](../0197_rising_temperature/) - SQL 行间比较
- [180. 连续出现的数字](../0180_consecutive_numbers/) - SQL 连续行分析
