# 570. 至少有5名直接下属的经理 (Managers with at Least 5 Direct Reports)

## 核心思路
按 managerId 分组统计下属数量，筛选 >= 5 的，再关联回 Employee 表获取经理姓名。

## 思维链
1. 每个员工的 managerId 指向其经理 → 按 managerId 分组计数
2. 筛选 COUNT >= 5 的 managerId
3. 用 managerId 回查 Employee 表获取经理的 name

## 解法概览表

| 解法 | 思路 | 时间复杂度 | 推荐 |
|------|------|-----------|------|
| GROUP BY + HAVING + JOIN ⭐ | 分组统计后连接 | O(n) | 首选 |
| 子查询 IN | 子查询找出符合条件的 managerId | O(n) | 简洁 |
| 相关子查询 | 对每个员工检查下属数 | O(n²) | 不推荐 |

## 关键提示
1. managerId 可能为 NULL（该员工没有经理），分组时自然排除
2. 只需返回 name 列
3. 自连接的 JOIN 条件: e1.id = e2.managerId

## 解法详解

### 解法一：GROUP BY + HAVING + JOIN（推荐）⭐
```sql
SELECT e.name
FROM Employee e
JOIN (
    SELECT managerId
    FROM Employee
    WHERE managerId IS NOT NULL
    GROUP BY managerId
    HAVING COUNT(*) >= 5
) m ON e.id = m.managerId;
```

### 解法二：子查询 IN
```sql
SELECT name
FROM Employee
WHERE id IN (
    SELECT managerId
    FROM Employee
    GROUP BY managerId
    HAVING COUNT(*) >= 5
);
```

### 解法三：直接 JOIN + GROUP BY
```sql
SELECT e1.name
FROM Employee e1
JOIN Employee e2 ON e1.id = e2.managerId
GROUP BY e1.id, e1.name
HAVING COUNT(*) >= 5;
```

## 易错点
- ✗ 忘记 WHERE managerId IS NOT NULL → ✓ 虽然 GROUP BY 会把 NULL 分一组，但不影响结果（因为 id 不会为 NULL），但最佳实践是过滤
- ✗ GROUP BY 只写 name 不写 id → ✓ 如果有同名经理会被合并，应 GROUP BY id
- ✗ 输出 managerId 而非 name → ✓ 题目要求返回 name

## 面试追问
**Q1: 如果要返回经理姓名及其下属数量怎么办？**
> 在 SELECT 中加 COUNT(*) AS direct_reports，并在结果中保留。

**Q2: 如果层级不止一层，要找间接下属总数 >= 5 的经理？**
> 需要递归 CTE (WITH RECURSIVE) 先展开所有层级关系，再统计。

**Q3: 这道题有没有性能优化建议？**
> 对 managerId 列建索引，加速 GROUP BY 操作。

## 相关题型
- [1270. All People Report to the Given Manager](../1270_all_people_report_to_the_given_manager/) - 递归查询管理层级
- [1350. Students With Invalid Departments](../1350_students_with_invalid_departments/) - JOIN 验证外键
