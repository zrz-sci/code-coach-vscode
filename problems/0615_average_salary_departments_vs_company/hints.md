# 615. 平均工资：部门与公司比较 - 提示

## 题目理解
- Salary 表记录每个员工每月的薪资
- Employee 表记录员工所属部门
- 需要按月份比较每个部门的平均薪资与公司整体平均薪资，输出 higher/lower/same

## 提示 1 - 分别计算两个平均值
需要计算两个维度的平均值：
1. **公司月平均**：按月份 GROUP BY，对所有员工的 amount 取 AVG
2. **部门月平均**：按月份 + 部门 GROUP BY，对部门内员工的 amount 取 AVG

## 提示 2 - 日期处理
`pay_date` 是完整日期，需要提取年月：
- MySQL: `DATE_FORMAT(pay_date, '%Y-%m')`
- 也可用 `LEFT(pay_date, 7)`

## 提示 3 - 比较并输出结果
用 `CASE WHEN` 比较部门平均与公司平均：
- 部门平均 > 公司平均 → 'higher'
- 部门平均 < 公司平均 → 'lower'
- 相等 → 'same'

## 提示 4 - 整体结构
使用 CTE 或子查询分别算出公司月平均和部门月平均，再 JOIN 起来做比较。

## 关键知识点
- 窗口函数 `AVG() OVER()` 或 CTE 子查询
- `DATE_FORMAT` 日期处理
- `CASE WHEN` 条件表达式
- 多表 JOIN

## 复杂度
- 核心操作：两次分组聚合 + 一次连接
