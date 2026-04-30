# 683. K Empty Slots - K 个空位

## 核心思路

n 个灯泡排成一排，每天打开一个，求最早在哪一天存在两个亮着的灯泡之间恰好有 k 个全部关着的灯泡。核心技巧是将问题转化：构造 `days[]` 数组（`days[x]` = 位置 x 的灯在第几天被打开），然后在 `days[]` 上用滑动窗口找一个长度为 `k+2` 的区间 `[left, right]`（right = left + k + 1），使得区间内部所有元素都大于两个端点的最大值。也可以用有序集合 `std::set`，每次插入后检查前驱/后继的距离。

## 思维链

1. 读题：bulbs[i] = x 表示第 i+1 天打开位置 x 的灯。求最早哪天存在两个亮灯中间恰好 k 个灭灯。
2. 直觉：每天打开一个灯后，在所有已开灯的位置中找"相邻且距离恰好 k+1 的pair"。
3. 思路一 (有序集合)：用 set 维护已开灯的位置。每次插入位置 pos，检查 set 中的前驱 prev 和后继 next，若 `pos - prev == k+1` 或 `next - pos == k+1`，返回当前天数。
4. 思路二 (滑动窗口)：转换成 `days[]` 数组。要找连续的位置 left 和 right = left + k + 1，使得 `[left+1, right-1]` 中所有 days 值都大于 `max(days[left], days[right])`，即这两个灯打开时中间的灯都还没开。
5. 滑动窗口细节：遍历窗口内部的每个位置 i，若 `days[i] < days[left]` 或 `days[i] < days[right]`，窗口失败，left 移到 i 重新开始。否则 i 走到 right 时窗口验证成功，记录答案 `max(days[left], days[right])`。
6. 为什么 left 移到 i：因为 i 是窗口内打开最早的位置，以它为端点才有可能找到合法窗口。
7. 取所有合法窗口中 `max(days[left], days[right])` 的最小值。

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 适用场景 |
|------|-----------|-----------|---------|
| 滑动窗口（days 数组） | O(n) | O(n) | 最优解，面试首选 |
| 有序集合 (std::set) | O(n log n) | O(n) | 实现更直观 |
| 线段树 / BIT | O(n log n) | O(n) | 过度设计 |

## 关键提示

1. **days 数组的构造**：`days[bulbs[i] - 1] = i + 1`，即位置转天数的映射。注意 bulbs 是 1-indexed 而 days 是 0-indexed。
2. **窗口的含义**：`[left, right]` 代表位置 left 和 right 的灯是亮的边界，中间有 `right - left - 1 = k` 个灯必须在这两个灯之后才打开。
3. **窗口验证条件**：窗口内部每个位置 i 的 `days[i]` 必须大于两个端点中的较大者。
4. **窗口失败后的跳跃**：发现 `days[i]` 比端点小时，直接将 left 移到 i（不是 left+1），因为 i 之前的位置作为 left 都不可能形成合法窗口。
5. **set 方法的边界检查**：insert 后 iterator 可能是 begin() 或 end() 附近，必须检查前驱/后继是否存在。

## 解法详解

### 解法一：滑动窗口 -- days 数组（推荐）

**预处理**：构造 `days[]` 数组，`days[x]` = 位置 x+1 的灯在第几天打开。

**滑动窗口**：
- 初始化 `left = 0`, `right = k + 1`, `i = left + 1`
- 遍历窗口内部 `i` 从 `left + 1` 到 `right - 1`：
  - 若 `days[i] < days[left]` 或 `days[i] < days[right]`：窗口失败
    - `left = i`, `right = i + k + 1`, `i = left + 1`
  - 否则 `i++`
- 当 `i == right`：窗口验证成功
  - `ans = min(ans, max(days[left], days[right]))`
  - `left = right`, `right = left + k + 1`, `i = left + 1`
- 最终返回 `ans`（若始终为 INT_MAX 则返回 -1）

**为什么是 O(n)**：每个位置最多被 left 和 i 各经过一次。

### 解法二：有序集合 (std::set)

**步骤**：
1. 遍历 bulbs 数组，第 `i+1` 天打开位置 `pos = bulbs[i]`
2. 将 pos 插入 `set<int>`
3. 取得插入后的 iterator `it`
4. 检查右邻居 `next(it)`：若存在且 `*next(it) - pos == k + 1`，返回 `i + 1`
5. 检查左邻居 `prev(it)`：若 `it != begin()` 且 `pos - *prev(it) == k + 1`，返回 `i + 1`
6. 遍历结束无发现，返回 -1

**优点**：代码简洁直观，逻辑清晰。
**缺点**：O(n log n) 比滑动窗口慢。

## 易错点

1. **days 数组索引错位**：bulbs 是 1-indexed（位置从 1 到 n），days 是 0-indexed（0 到 n-1），转换时 `days[bulbs[i] - 1] = i + 1`。
2. **窗口失败时 left 不前进**：应该将 left 设为 i（当前违规位置），而不是 left + 1，否则可能错过最优解。
3. **k = 0 的边界**：k=0 意味着两个灯泡相邻，right = left + 1，窗口内没有内部元素，直接验证成功。
4. **right 越界**：检查 `right < n` 后才能访问 `days[right]`。
5. **set 方法中 begin() 的判断**：插入第一个元素后 `it == begin()`，此时没有左邻居，不能调用 `prev(it)`。
6. **答案取 max 而非 min**：单个窗口的答案是 `max(days[left], days[right])`（两个灯都打开的天数），但全局答案取所有窗口中的最小值。

## 面试追问

1. **Q: 滑动窗口为什么是 O(n) 而不是 O(n*k)？**
   A: 关键在于 left 和 i 都是单调递增的。窗口失败时 left 跳到 i，i 从 left+1 重新开始。每个位置最多被 left 跳过一次、被 i 扫过一次，总计 O(n)。

2. **Q: 如果改为"最多 k 个灭灯"而非"恰好 k 个"呢？**
   A: set 方法中将 `== k+1` 改为 `<= k+1`。滑动窗口需要枚举所有合法的 k' (0 到 k) 的窗口大小，或者用 set 方法更简单。

3. **Q: 如果灯泡可以关闭（toggle），怎么处理？**
   A: 不能再用 days 数组（它假设灯只打开不关闭）。需要用 set + 实时维护，每次 toggle 后检查前驱/后继距离。

4. **Q: 为什么不用线段树或 BIT？**
   A: 可以用，但过度设计。线段树可以区间查询最小 days 值，但滑动窗口 O(n) 更简单更快。

5. **Q: 能否用二分搜索优化 set 方法？**
   A: std::set 的 insert + 找前驱/后继已经是 O(log n)，二分搜索不会更快。如果用数组 + 二分，insert 变为 O(n)，反而更慢。

## 相关题型

| 题号 | 题名 | 关联 |
|------|------|------|
| 239 | Sliding Window Maximum | 滑动窗口技巧 |
| 480 | Sliding Window Median | 有序集合 + 滑动窗口 |
| 220 | Contains Duplicate III | 有序集合 + 距离约束 |
| 352 | Data Stream as Disjoint Intervals | 有序集合维护区间 |
| 995 | Minimum Number of K Consecutive Bit Flips | 滑动窗口变体 |
