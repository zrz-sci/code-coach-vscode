# 729. 我的日程安排表 I (My Calendar I)

## 难度：Medium

## 标签：Design, Segment Tree, Array, Binary Search, Ordered Set

## 提示

### 提示 1
两个区间 `[s1, e1)` 和 `[s2, e2)` 有重叠当且仅当 `s1 < e2 && s2 < e1`。

### 提示 2
可以用有序容器（如 `map` 或 `set`）存储已预订的区间，利用二分查找快速定位可能冲突的区间。

### 提示 3
使用 `lower_bound` 找到第一个结束时间大于新区间起始时间的已有区间，然后检查它的起始时间是否小于新区间的结束时间。

## 解题思路

### 方法一：暴力遍历 O(n^2)
每次 book 时遍历所有已有区间，检查是否有重叠。

### 方法二：有序集合 + 二分查找 O(n log n)
1. 用 `map<int, int>` 存储 `{start -> end}` 的已预订区间
2. `book` 时用 `lower_bound(start)` 找到第一个 `start >= 新start` 的区间
3. 检查该区间以及前一个区间是否与新区间重叠
4. 无重叠则插入

## 复杂度分析
- 方法二：每次 book O(log n)，总共 O(n log n)
- 空间：O(n)
