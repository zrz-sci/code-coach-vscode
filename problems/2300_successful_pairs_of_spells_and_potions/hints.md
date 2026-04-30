# 2300. 咒语和药水的成功对数 (Successful Pairs of Spells and Potions)

[LeetCode 链接](https://leetcode.com/problems/successful-pairs-of-spells-and-potions/)

## 题目理解

给定两个正整数数组 `spells`（长度 n）和 `potions`（长度 m），以及整数 `success`。

一对 `(spell, potion)` 称为"成功"当且仅当 `spell * potion >= success`。

对每个 spell，求有多少个 potion 与它构成成功对。

**约束规模**：
- `n, m <= 10^5`
- `success <= 10^10`（注意 long long！）
- 暴力 O(n * m) = 10^10，必然 TLE

## 第一步提示：排序 + 单调性

<details>
<summary>展开提示</summary>

**核心观察**：如果 potions 是**排序**的，那么对于一个固定的 spell：
- `spell * potion` 随 potion 递增
- 存在一个**临界点**：小于临界点的 potion 不满足，大于等于临界点的都满足
- 这是**二分查找**的完美条件！

**具体来说**：
- 对于 `spell * potion >= success`，需要 `potion >= success / spell`
- 向上取整：`minPotion = ceil(success / spell)`
- 在排序后的 potions 中找第一个 `>= minPotion` 的位置 → 从该位置到末尾都是成功对

</details>

## 第二步提示：二分查找收缩过程

<details>
<summary>展开提示</summary>

**二分查找的目标**：在排序后的 potions 中，找到第一个 `>= minPotion` 的位置。

这正是 `lower_bound` 的定义！

**收缩过程示例**：
```
spells = [5,1,3], potions = [1,2,3,4,5], success = 7

排序后 potions = [1, 2, 3, 4, 5]

--- spell = 5 ---
minPotion = ceil(7 / 5) = 2
找第一个 >= 2 的位置:
  lo=0, hi=5
  mid=2, potions[2]=3 >= 2 → hi=2
  mid=1, potions[1]=2 >= 2 → hi=1
  mid=0, potions[0]=1 <  2 → lo=1
  lo==hi=1 → 位置1, 成功对数 = 5-1 = 4 ✓

  [1, 2, 3, 4, 5]   minPotion=2
   ^              ^
   lo=0           hi=5
         ^
         mid=2: 3>=2 → hi=2
      ^
      mid=1: 2>=2 → hi=1
   ^
   mid=0: 1<2 → lo=1
   结果: idx=1, 答案=5-1=4

--- spell = 1 ---
minPotion = ceil(7 / 1) = 7
找第一个 >= 7 的位置:
  lo=0, hi=5
  mid=2, potions[2]=3 < 7 → lo=3
  mid=4, potions[4]=5 < 7 → lo=5
  lo==hi=5 → 位置5 (超出末尾), 成功对数 = 5-5 = 0 ✓

  [1, 2, 3, 4, 5]   minPotion=7
   ^              ^
   lo=0           hi=5
         ^
         mid=2: 3<7 → lo=3
               ^
               mid=4: 5<7 → lo=5
   结果: idx=5, 答案=5-5=0

--- spell = 3 ---
minPotion = ceil(7 / 3) = 3
找第一个 >= 3 的位置:
  lo=0, hi=5
  mid=2, potions[2]=3 >= 3 → hi=2
  mid=1, potions[1]=2 <  3 → lo=2
  lo==hi=2 → 位置2, 成功对数 = 5-2 = 3 ✓

  [1, 2, 3, 4, 5]   minPotion=3
   ^              ^
   lo=0           hi=5
         ^
         mid=2: 3>=3 → hi=2
      ^
      mid=1: 2<3 → lo=2
   结果: idx=2, 答案=5-2=3
```

**二分搜索区间收缩图**：
```
spell=5, minPotion=2, potions=[1,2,3,4,5]

Step 0: [lo=0 ..................... hi=5)
         1   2   3   4   5
Step 1:  1  [2   3]  4   5        mid=2, 3>=2, hi→2
Step 2:  1  [2]  3   4   5        mid=1, 2>=2, hi→1
Step 3: [1]  2   3   4   5        mid=0, 1<2,  lo→1
Result:      ^idx=1                答案 = 5-1 = 4
```

</details>

## 第三步提示：整数除法向上取整

<details>
<summary>展开提示</summary>

计算 `minPotion = ceil(success / spell)` 是关键的细节。

**整数向上取整公式**：
```
ceil(a / b) = (a + b - 1) / b   (当 a, b > 0)
```

在本题中：
```cpp
long long minPotion = (success + spell - 1) / spell;
```

**为什么需要向上取整？**

考虑 `success = 7, spell = 3`：
- 精确值：7 / 3 = 2.333...
- 向下取整：2 → `3 * 2 = 6 < 7`，不满足！
- 向上取整：3 → `3 * 3 = 9 >= 7`，满足！

所以必须向上取整。

**另一种写法**（用 `lower_bound` 的变体来避免手动取整）：

可以直接用 `lower_bound` 找第一个 `potion` 使得 `(long long)spell * potion >= success`：
```cpp
auto it = lower_bound(potions.begin(), potions.end(), 0,
    [&](int p, int) { return (long long)spell * p < success; });
```
但这样可读性较差，推荐直接计算 minPotion。

**注意溢出**：`spell * potion` 最大 `10^5 * 10^5 = 10^10`，需要 `long long`。

</details>

## 第四步提示：为什么不需要对 spells 排序？

<details>
<summary>展开提示</summary>

只需要对 **potions** 排序。对每个 spell 独立做一次二分即可。

如果也对 spells 排序，可以用**双指针**代替二分（spell 增大时，minPotion 减小，左指针只会向左移动）。但这样需要额外处理"原始顺序"的映射，代码更复杂。

**排序 + 二分** 已经是 O(m log m + n log m)，完全高效，无需进一步优化。

**complexity breakdown**：
- 排序 potions：O(m log m)
- 对每个 spell 二分：O(n * log m)
- 总计：O((m + n) * log m)
- m = n = 10^5 时：10^5 * 17 ≈ 2 * 10^6，非常快

</details>

## 第五步提示（备选方案）：排序两个数组 + 双指针

<details>
<summary>展开提示</summary>

**思路**：
1. 将 spells 和其原始下标一起排序（降序）
2. potions 排序（升序）
3. 维护指针 j 从左到右扫描 potions

spell 降序遍历时，minPotion 递增 → j 只会向右移动：
```
spell 越大 → 需要的 minPotion 越小 → 更多 potion 满足
spell 越小 → 需要的 minPotion 越大 → 更少 potion 满足
```

等等！spell 降序时 minPotion 是递增的（minPotion = ceil(success/spell)，spell 减小 → minPotion 增大）。所以 j 从 0 开始只会向右移动。

**实现**：
```
对 spells 按降序排序 (带原始下标)
j = 0
for each spell (降序):
    minPotion = ceil(success / spell)
    while j < m && potions[j] < minPotion: j++
    result[original_idx] = m - j
```

时间 O(m log m + n log n)，空间 O(n)。
和二分法复杂度相同，但双指针的常数更小。

</details>

## 思维链总结

```
暴力: O(n*m) = 10^10 TLE
  |
  v
排序 potions → 单调性 → 二分查找
  |
  v
对每个 spell, 计算 minPotion = ceil(success/spell)
  |
  v
lower_bound 找第一个 >= minPotion 的位置
  |
  v
成功对数 = m - 该位置
  |
  v
O((m+n) * log m) ≈ 2*10^6
```

## 解法对比

| 解法 | 思路 | 时间 | 空间 | 代码复杂度 |
|------|------|------|------|-----------|
| 排序+二分 | potions 排序, 对每个 spell 二分 | O((m+n)logm) | O(1) | 简单 |
| 排序+双指针 | 两数组排序, 单调指针 | O(nlogn+mlogm) | O(n) | 中等 |
| 暴力 | 双重循环 | O(n*m) | O(1) | 极简但 TLE |

## 复杂度分析

**排序 + 二分**：
- 排序 potions：O(m log m)
- n 次二分查找：O(n log m)
- 总计：O(m log m + n log m) = O((m + n) log m)
- 空间 O(log m)（排序用的栈空间），如果不算排序则 O(1) 额外空间

## 易错点

1. **溢出！** `spell * potion` 和 `success` 可达 10^10，必须用 `long long`
2. **向上取整**：`(success + spell - 1) / spell`，别用 `success / spell`
3. **potions 排序后求的是位置到末尾的个数**：答案 = `m - idx`，不是 `idx`
4. **spell = 0 不存在**：题目保证正整数，不需要特判除以 0
5. **`lower_bound` 返回的是迭代器**：要减去 `begin()` 才得到下标

## 模式识别

经典的 **"排序 + 二分查找"** 模式：
- 对一个数组排序建立单调性
- 对另一个数组的每个元素，在排好序的数组上二分查找
- 时间从 O(n*m) 优化到 O((n+m) log m)

**适用场景**：两个数组配对，判断每个元素有多少"满足条件"的配对。条件通常是乘积/和/差与阈值的比较。

## 相关题目

| 题号 | 题目 | 关联 |
|------|------|------|
| 826 | Most Profit Assigning Work | 排序+二分/双指针 |
| 744 | Find Smallest Letter Greater Than Target | lower_bound 基础 |
| 1870 | Minimum Speed to Arrive on Time | 二分答案 |
| 875 | Koko Eating Bananas | 二分答案（类似的 ceil 除法） |
| 34 | Find First and Last Position | lower_bound / upper_bound |

---

## 面试追问

**Q1: 如果 potions 数组非常大无法排序（流式数据），怎么办？**
> 可以用大小为 k 的 min-heap 维护最大的 k 个 potions，但精确查询仍需完整数据。另一种思路：对值域二分，用 count 统计 >= threshold 的 potion 个数。

**Q2: 如果要求 `spell * potion` 恰好等于 success（而非 >=），怎么做？**
> 对每个 spell，检查 success 是否能被 spell 整除；若能，在 potions 中二分查找 `success / spell` 是否存在。用 `equal_range` 或 `lower_bound` + 值比较。O((n+m) log m)。

**Q3: 如果 spells 和 potions 都允许动态增删，如何高效查询？**
> 用平衡 BST（如 `std::multiset`）存储 potions，查询时 `lower_bound` 找 minPotion 的位置，用 `distance` 或 order-statistics tree 计算后缀长度。插入/删除 O(log m)，查询 O(log m)。
