# 732. 我的日程安排表 III (My Calendar III)

## 难度：Hard

## 标签：Design, Segment Tree, Binary Search, Ordered Set, Prefix Sum

## 提示

### 提示 1
与 Calendar I/II 不同，这道题不需要拒绝任何预订，只需要返回当前最大的重叠次数 k。

### 提示 2
使用差分数组 + 有序 map：在 `startTime` 处 +1，在 `endTime` 处 -1，然后扫描前缀和找最大值。

### 提示 3
也可以用线段树实现 O(log C) 的单次操作，但差分法实现更简洁，面试中更常用。

## 解题思路

**差分 + 有序 map 扫描**

1. 用 `map<int, int>` 维护差分：`map[start]++`，`map[end]--`
2. 每次 `book` 后，遍历 map 计算前缀和
3. 前缀和的最大值即为当前最大 k 值

这道题与 731 (Calendar II) 的核心技巧完全一致，只是 731 需要检查是否 >= 3 来拒绝，而 732 只需要返回最大值。

## 复杂度分析
- 时间：每次 book O(n log n)，总 O(n^2 log n)，n 为 book 调用次数
- 空间：O(n)
