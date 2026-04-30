# 618. 学生地理信息报告 - 提示

## 题目理解
- Student 表包含学生姓名和所在大洲（America / Asia / Europe）
- 需要做**行转列**（Pivot），将三个大洲的学生分别排列成三列
- 每列按字母序排列，不足的位置填 null

## 提示 1 - 行号分配
关键思路：为每个大洲内的学生分配行号，这样同一行号的学生就会排在同一行。
- 使用 `ROW_NUMBER()` 窗口函数
- `PARTITION BY continent ORDER BY name` 为每个大洲内按字母序编号

## 提示 2 - 行转列
有了行号后，使用条件聚合实现 Pivot：
```sql
MAX(CASE WHEN continent = 'America' THEN name END) AS America
```
按 `row_number` 分组，每组取各大洲的名字。

## 提示 3 - 变量模拟行号（备选）
如果不支持窗口函数，可以用用户变量模拟行号：
```sql
@row := @row + 1
```

## 关键知识点
- `ROW_NUMBER()` 窗口函数
- `CASE WHEN` 条件聚合（行转列 / Pivot）
- `MAX` 配合 `CASE` 提取非 NULL 值
- `GROUP BY` 行号

## 复杂度
- 核心操作：一次窗口函数 + 一次分组聚合
