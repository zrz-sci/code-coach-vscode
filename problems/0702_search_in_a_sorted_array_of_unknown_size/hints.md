# 702. Search in a Sorted Array of Unknown Size / 搜索长度未知的有序数组

## 核心思路

本题的本质是**在无法直接获取数组长度的情况下进行二分查找**。标准二分需要已知边界 `[0, n-1]`，但这里数组长度未知，越界访问返回 `INT_MAX (2^31 - 1)`。因此需要分两步：第一步用**指数扩展（Exponential Search）**找到一个包含 target 的上界；第二步在确定的范围内做标准二分。

核心洞察：既然数组有序且元素唯一，我们可以从 index=1 开始，不断将右边界翻倍（1, 2, 4, 8, 16, ...），直到 `reader.get(right) >= target`，此时 target 一定在 `[right/2, right]` 区间内。然后在该区间内做标准二分即可。

## 思维链

1. **问题分析**：有序、唯一、未知长度，需要 O(log n) 复杂度 => 必须用二分，但缺少右边界
2. **确定搜索范围**：从 right=1 开始，当 `reader.get(right) < target` 时，令 `right *= 2`；退出时 target 一定在 `[right/2, right]` 内
3. **边界情况处理**：如果 `reader.get(right) == INT_MAX`，说明 right 已越界，但 target 仍可能在 `[right/2, right-1]` 某处（也可能不存在）
4. **标准二分**：在 `[left, right]` 内二分查找 target，若 `reader.get(mid) == target` 返回 mid，否则收缩边界
5. **终止条件**：`left > right` 时返回 -1

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| 指数扩展 + 二分 | O(log n) | O(1) | 最优解，两阶段均为对数级 |
| 线性搜索 | O(n) | O(1) | 不满足题目要求 |

## 关键提示

1. **指数扩展的精髓**：右边界从 1 开始翻倍，只需 O(log n) 步就能找到包含 target 的区间（因为每次翻倍相当于对数级增长）
2. **越界值的利用**：`reader.get(i)` 越界返回 `2^31 - 1`，这个值必定大于任何合法 target（`target <= 10^4`），所以越界不会干扰二分逻辑
3. **左边界的设定**：指数扩展退出后，`left = right / 2`，不是 `right / 2 + 1`，因为 right/2 处的值可能恰好等于 target
4. **注意溢出**：虽然本题数值范围小不会溢出，但 `mid = left + (right - left) / 2` 是好习惯

## 解法详解

### 解法一：指数扩展 + 二分查找（最优解）

**阶段一：确定搜索边界**
- 初始化 `right = 1`
- 循环：当 `reader.get(right) < target` 时，`right *= 2`
- 退出循环时，`reader.get(right) >= target`，设 `left = right / 2`

**阶段二：标准二分查找**
- 在 `[left, right]` 中二分
- `mid = left + (right - left) / 2`
- 若 `reader.get(mid) == target`，返回 mid
- 若 `reader.get(mid) < target`，`left = mid + 1`
- 若 `reader.get(mid) > target`，`right = mid - 1`
- 循环结束返回 -1

**为什么两阶段都是 O(log n)？**
- 阶段一：最多翻倍 log(n) 次就能使 right >= n
- 阶段二：搜索范围最大为 right - right/2 = right/2，而 right <= 2n，所以搜索范围 <= n，二分 O(log n)

### 解法一变体：更紧致的左边界

在指数扩展时同步推进 left：
- 每次翻倍前，令 `left = right`（当前 right 已检查过，值 < target）
- 这样搜索范围更紧致，实际常数更小

## 易错点

1. **right 初始值设为 0**：如果初始 right=0，且 target 恰好在 index 0，翻倍后 right 仍为 0，死循环。应初始化为 1
2. **left 边界取错**：扩展后 left 应为 `right / 2`（而非 `right / 2 + 1`），因为 `right/2` 处可能尚未检查
3. **忘记处理 reader.get(0) == target 的情况**：指数扩展从 right=1 开始，如果 target 在 index 0，第二阶段的二分会覆盖到，但需确保 left 从 0 开始（第一次循环若 reader.get(1) >= target，left = 1/2 = 0，正确）
4. **将越界值当作有效数据**：越界返回的 `2^31 - 1` 只是哨兵，不要将其与 target 做等值比较后返回
5. **二分写法的 off-by-one**：`left <= right` 而非 `left < right`，否则漏掉 left == right 的情况

## 面试追问

1. **Q: 如果数组可能包含重复元素，找到 target 的第一个/最后一个出现位置，如何修改？**
   A: 阶段一不变。阶段二改为左边界/右边界二分：找到 target 后不立即返回，而是继续收缩边界。

2. **Q: 如果不知道越界返回什么值（可能返回任何值），怎么办？**
   A: 无法用指数扩展+二分。只能用线性方式逐步探测边界。

3. **Q: 为什么用指数扩展而不是直接设一个很大的右边界（如 10^9）？**
   A: 指数扩展能自适应数组实际大小，扩展阶段 O(log n)。直接设大右边界虽然可行，但二分范围不紧致，常数更大；且如果数组很小，会浪费大量无效查询。

4. **Q: 能否用三分搜索代替二分？**
   A: 不必要。三分搜索用于单峰函数的极值问题，这里是有序数组的精确查找，二分更高效。

5. **Q: reader.get() 每次调用是否有成本？如何最小化调用次数？**
   A: 在阶段一中，每次翻倍只调用一次 get；阶段二每次迭代调用一次。总调用次数 O(log n)，已是最优。

## 相关题型

- [704. Binary Search](../0704_binary_search/) - 标准二分查找
- [278. First Bad Version](../0278_first_bad_version/) - 二分查找变体，API 调用
- [374. Guess Number Higher or Lower](../0374_guess_number_higher_or_lower/) - 交互式二分
- [33. Search in Rotated Sorted Array](../0033_search_in_rotated_sorted_array/) - 修改条件的二分
- [658. Find K Closest Elements](../0658_find_k_closest_elements/) - 二分查找 + 滑动窗口
