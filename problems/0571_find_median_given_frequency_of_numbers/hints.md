# 571. 给定数字的频率查询中位数 (Find Median Given Frequency of Numbers)

## 核心思路
利用累计频率（前缀和）定位中位数。一个数是中位数候选，当且仅当它的累计频率范围覆盖了中间位置。

## 思维链
1. 每个 num 出现 frequency 次 → 需要"展开"思维但不能真展开
2. 总数 total = SUM(frequency)
3. 中位数位置: 第 total/2 个和第 total/2 + 1 个的平均值
4. 用累计频率（前缀和）判断哪些 num 覆盖了这两个位置
5. 条件：cum_freq >= total/2.0 AND cum_freq - frequency < total/2.0 + 1

## 解法概览表

| 解法 | 思路 | 时间复杂度 | 推荐 |
|------|------|-----------|------|
| 累计频率法 ⭐ | 前缀和定位中位数 | O(n log n) | 首选 |
| 交叉累计法 | 正向/反向累计交叉 | O(n log n) | 备选 |

## 关键提示
1. 不能把频率表展开成行（可能有百万级别），必须用数学方法
2. 中位数判定条件：该数字的累计区间包含中间位置
3. 结果需要 ROUND 到一位小数

## 解法详解

### 解法一：累计频率法（推荐）⭐
```sql
SELECT ROUND(AVG(num), 1) AS median
FROM (
    SELECT num, frequency,
           SUM(frequency) OVER (ORDER BY num) AS cum_freq,
           (SUM(frequency) OVER ()) / 2.0 AS mid
    FROM Numbers
) t
WHERE cum_freq >= mid AND cum_freq - frequency < mid + 1;
```

### 解法二：交叉累计法
```sql
SELECT ROUND(AVG(n.num), 1) AS median
FROM Numbers n
WHERE n.frequency >= ABS(
    (SELECT SUM(frequency) FROM Numbers WHERE num <= n.num) -
    (SELECT SUM(frequency) FROM Numbers WHERE num >= n.num)
);
```

## 易错点
- ✗ 试图将频率表展开为单行 → ✓ 数据量可能极大，用累计频率方法
- ✗ 忘记 AVG（偶数个元素有两个中位数候选） → ✓ 用 AVG(num) 处理偶数情况
- ✗ 忘记 ROUND 到一位小数 → ✓ 题目要求 ROUND(..., 1)

## 面试追问
**Q1: 为什么用 AVG 而不是直接输出 num？**
> 当总数为偶数时，两个中位数候选可能不同（虽然频率表中不常见），AVG 保证正确计算。实际上在频率表场景中，如果两个中位数位置落在同一个 num 上，AVG 不改变值。

**Q2: 与 569 题的中位数有何不同？**
> 569 是找中位数所在的行（返回整行），本题是求中位数值（返回一个数字）。569 数据是展开的，本题是压缩的频率表。

**Q3: 大数据量下如何优化？**
> 对 num 建索引，使累计求和可以利用有序扫描。或者用变量累加避免窗口函数开销。

## 相关题型
- [569. Median Employee Salary](../0569_median_employee_salary/) - 分组求中位数行
- [480. Sliding Window Median](../0480_sliding_window_median/) - 滑动窗口中位数
