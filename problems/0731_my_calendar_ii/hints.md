# 731. 我的日程安排表 II (My Calendar II)

## 难度：Medium

## 标签：Design, Segment Tree, Array, Binary Search, Ordered Set, Prefix Sum

## 提示

### 提示 1
维护两个列表：一个记录所有已预订的区间（单次预订），另一个记录所有重叠区间（双重预订）。

### 提示 2
新区间如果与任何双重预订的区间有交集，则会产生三重预订，应该拒绝。

### 提示 3
另一种思路：使用差分数组 / 事件扫描线。在 start 处 +1，在 end 处 -1，扫描所有时间点检查是否有任何位置计数 >= 3。

## 解题思路

### 方法一：双列表法 O(n^2)
1. `bookings`：所有已预订的区间
2. `overlaps`：所有双重预订的区间
3. 新区间到来时，先检查是否与 `overlaps` 中任意区间重叠（若是则返回 false）
4. 再与 `bookings` 中每个区间计算交集，将交集加入 `overlaps`
5. 最后将新区间加入 `bookings`

### 方法二：差分 + 有序 map O(n^2 log n)
1. 用 `map<int,int>` 做差分：`map[start]++`，`map[end]--`
2. 每次 book 时先尝试加入，然后扫描前缀和
3. 若前缀和 >= 3 则撤销并返回 false

## 复杂度分析
- 方法一：每次 book O(n)，总 O(n^2)
- 空间：O(n)
