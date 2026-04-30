# 633. 平方数之和

## 难度: Medium
## 标签: Math, Two Pointers, Binary Search

## 题目理解
给定非负整数 c，判断是否存在两个整数 a 和 b，使得 a^2 + b^2 = c。

## 关键观察
1. a 和 b 的范围是 [0, sqrt(c)]
2. 可以用双指针：一个从 0 开始，一个从 sqrt(c) 开始
3. 类似于在有序数组中找两数之和等于目标值

## 解题思路

### 方法：双指针
1. 设 `left = 0`, `right = sqrt(c)`
2. 计算 `sum = left*left + right*right`
   - 如果 `sum == c`，返回 true
   - 如果 `sum < c`，`left++`
   - 如果 `sum > c`，`right--`
3. 当 `left > right` 时结束，返回 false

### 注意事项
- 注意溢出：`left*left + right*right` 可能超过 int 范围，使用 `long long`
- `right` 初始值用 `(long long)sqrt(c)` 即可

## 提示序列
1. a 的范围最大到 sqrt(c)
2. 固定 a 后，检查 c - a^2 是否是完全平方数；或者用双指针
3. 双指针从两端往中间逼近，注意用 long long 防止溢出

## 复杂度
- 时间：O(sqrt(c))
- 空间：O(1)
