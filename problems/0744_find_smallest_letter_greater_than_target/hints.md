# 744. 寻找比目标字母大的最小字母

## 核心思路

在一个**非递减排序**的字符数组中，找到比 target **严格大于** 的最小字符。如果不存在则返回数组第一个字符（环绕特性）。本质上是一个二分查找 upper_bound 的经典应用。

关键洞察：由于数组已排序，且我们要找的是 **严格大于** target 的第一个元素，这完全等价于 C++ STL 中的 `upper_bound` 操作。如果 upper_bound 越界，则环绕返回 `letters[0]`。

## 思维链

1. **输入分析**：数组已经非递减排序，长度 2~10^4，只含小写字母，target 是一个小写字母
2. **目标明确**：找到第一个 **严格大于** target 的字母（不是大于等于）
3. **环绕处理**：如果所有字母都 <= target，返回数组首元素
4. **算法选择**：排序数组 + 查找 => 二分查找，时间 O(log n)
5. **边界确认**：二分结束后检查是否越界，越界则取模回到开头
6. **验证**：target='z', letters=['x','y'] => 返回 'x'（环绕）

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| 二分查找 (upper_bound) | O(log n) | O(1) | 最优解，标准二分模板 |
| 线性扫描 | O(n) | O(1) | 简单直接，但效率不如二分 |
| 哈希/桶计数 | O(n + 26) | O(1) | 利用字符范围有限的特性 |

## 关键提示

1. **严格大于**：这里要的是 `> target`，不是 `>= target`。这决定了二分的条件应当是 upper_bound 而非 lower_bound
2. **环绕逻辑**：用取模 `% n` 来优雅处理越界情况
3. **重复字符**：数组中可能有重复字符，例如 `['x','x','y','y']`，二分需正确跳过
4. **二分模板**：使用左闭右开区间 `[lo, hi)`，循环条件 `lo < hi`，最终 `lo` 就是答案位置

## 解法详解

### 解法一：二分查找（推荐）

**思路**：在 `[0, n)` 的范围内二分搜索第一个满足 `letters[mid] > target` 的位置。如果没找到（即所有字符都 <= target），返回 `letters[0]`。

**步骤**：
1. 初始化 `lo = 0, hi = n`
2. 当 `lo < hi` 时：
   - 计算 `mid = lo + (hi - lo) / 2`
   - 如果 `letters[mid] <= target`，搜索右半区 `lo = mid + 1`
   - 否则，搜索左半区 `hi = mid`
3. 返回 `letters[lo % n]`（取模处理环绕）

**正确性**：当循环结束时，`lo` 指向第一个 `> target` 的位置。如果 `lo == n`，说明所有元素都不大于 target，取模后返回 `letters[0]`。

### 解法二：线性扫描

**思路**：从头到尾遍历，找到第一个大于 target 的字符即返回。若遍历结束未找到，返回 `letters[0]`。

**步骤**：
1. 遍历每个字符 `c` in `letters`
2. 如果 `c > target`，立即返回 `c`
3. 遍历结束返回 `letters[0]`

### 解法三：桶计数

**思路**：利用字符只有 26 种的特性，建立布尔数组标记出现过的字符，然后从 `target + 1` 开始向后扫描（环绕到 'a'）。

**步骤**：
1. 建立 `seen[26]` 数组
2. 标记 letters 中出现的字符
3. 从 `(target - 'a' + 1) % 26` 开始，遍历 26 个字符位置
4. 第一个被标记的就是答案

## 易错点

1. **混淆 lower_bound 和 upper_bound**：题目要求严格大于，用 lower_bound 会在 target 存在于数组时返回 target 本身，这是错的。应该用 upper_bound（`letters[mid] <= target` 时移动 lo）
2. **忘记环绕**：当所有字符都 <= target 时，二分结果 lo == n，必须处理这种越界情况
3. **二分边界写错**：右边界应为 `n`（不是 `n-1`），否则无法正确检测全部都不满足的情况
4. **重复元素干扰**：`letters = ['c','c','c']`，target = 'b' 应返回 'c'，二分必须正确处理连续重复
5. **等号归属**：二分条件中 `<=` 的等号不能丢，否则当 target 等于某个元素时会返回该元素而非下一个

## 面试追问

1. **Q: 如果数组未排序怎么办？**
   A: 先排序再二分 O(n log n)，或者线性扫描 O(n)。如果查询频繁，预排序后多次二分更优。

2. **Q: 如何处理环绕？还有其他方式吗？**
   A: 除了 `% n` 取模，还可以在二分前先判断 `target >= letters.back()`，如果是直接返回 `letters[0]`。

3. **Q: 这题和标准 upper_bound 有什么关系？**
   A: 完全等价。`upper_bound(letters.begin(), letters.end(), target)` 返回的就是第一个严格大于 target 的迭代器。如果等于 end，就返回 `letters[0]`。

4. **Q: 能否用 STL 一行解决？**
   A: 可以。`auto it = upper_bound(letters.begin(), letters.end(), target); return it == letters.end() ? letters[0] : *it;`

5. **Q: 如果要找大于等于 target 的最小字母呢？**
   A: 把二分条件改为 `letters[mid] < target`（去掉等号），即 lower_bound。

6. **Q: 时间复杂度能否比 O(log n) 更优？**
   A: 不能。在最坏情况下，二分查找已是有序数组查找的最优时间复杂度。但如果字符集固定（26 个字母），桶方法是 O(n) 预处理 + O(1) 查询。

## 相关题型

- [35. Search Insert Position](../0035_search_insert_position/) - 二分查找插入位置，lower_bound 变体
- [34. Find First and Last Position of Element in Sorted Array](../0034_find_first_and_last_position_of_element_in_sorted_array/) - 二分查找左右边界
- [278. First Bad Version](../0278_first_bad_version/) - 二分查找第一个满足条件的位置
- [374. Guess Number Higher or Lower](../0374_guess_number_higher_or_lower/) - 二分搜索变体
- [852. Peak Index in a Mountain Array](../0852_peak_index_in_a_mountain_array/) - 三分/二分查找峰值
