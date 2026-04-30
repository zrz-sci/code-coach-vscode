# 715. Range Module / Range 模块

## 核心思路

本题要求设计一个数据结构来管理**半开区间 [left, right)** 的集合，支持三种操作：添加区间、查询区间是否完全覆盖、删除区间。核心挑战在于区间可能互相重叠、合并或被分割。

最佳思路是使用**有序映射（ordered map）**，用 `map<int, int>` 存储不相交的区间集合，其中 key 是区间左端点，value 是区间右端点。利用 map 的有序性（底层红黑树），可以高效地用 `lower_bound` 定位相关区间，然后合并/分割/删除。

另一种方案是**线段树**，但实现更复杂，面试中不推荐除非面试官明确要求。

## 思维链

1. **数据结构选择**：需要高效地查找、合并、分割区间 => 有序容器 => `std::map<int,int>`
2. **区间存储**：map 中每个 entry `{left: right}` 表示区间 `[left, right)`，且所有区间不相交
3. **addRange(left, right)**：
   - 找到所有与 [left, right) 重叠的已有区间
   - 将它们合并为一个大区间 `[min(left, 已有左端点), max(right, 已有右端点)]`
   - 删除原有重叠区间，插入合并后的新区间
4. **queryRange(left, right)**：
   - 找到左端点 <= left 的最后一个区间
   - 检查该区间的右端点是否 >= right（即完全覆盖）
5. **removeRange(left, right)**：
   - 找到所有与 [left, right) 重叠的已有区间
   - 删除重叠部分，保留两端可能残留的部分

## 解法概览

| 解法 | addRange | queryRange | removeRange | 空间 | 特点 |
|------|----------|-----------|-------------|------|------|
| 有序 Map | O(n) 摊销 | O(log n) | O(n) 摊销 | O(n) | 实现简洁，面试首选 |
| 线段树 | O(log C) | O(log C) | O(log C) | O(C 或动态) | C=10^9, 需要动态开点 |

## 关键提示

1. **为什么用 map 而非 unordered_map？** 需要按区间左端点排序来快速定位重叠区间，`map` 底层红黑树提供有序遍历和 `lower_bound`。
2. **lower_bound vs upper_bound**：`lower_bound(left)` 找到第一个左端点 >= left 的区间。但还需要检查前一个区间（它的右端点可能覆盖到 left）。
3. **半开区间的好处**：`[left, right)` 使得相邻区间 `[a, b)` 和 `[b, c)` 自然合并为 `[a, c)`，无需 +1/-1 调整。
4. **removeRange 最易出错**：删除 [left, right) 后，原区间可能分裂成最多两段——左残 `[origLeft, left)` 和右残 `[right, origRight)`。
5. **区间合并的不变量**：操作后 map 中的区间必须保持不相交且按序排列。

## 解法详解

### 解法一：有序 Map（推荐）

**数据结构**：`map<int, int> intervals_`，key=left, value=right，表示 `[key, value)`

**addRange(left, right)**：
1. 用 `upper_bound(left)` 找到第一个左端点 > left 的迭代器 it
2. 如果 it 不是 begin()，检查前一个区间 prev，如果 prev->second >= left，则从 prev 开始合并
3. 向后遍历，只要当前区间的左端点 <= right，就合并（扩展 left/right 的范围），并删除该区间
4. 插入合并后的 `[newLeft, newRight)`

**queryRange(left, right)**：
1. 用 `upper_bound(left)` 找到第一个左端点 > left 的迭代器
2. 如果是 begin()，没有区间能覆盖 left，返回 false
3. 否则看前一个区间（--it），检查 `it->second >= right`

**removeRange(left, right)**：
1. 用 `upper_bound(left)` 定位
2. 检查前一个区间是否与 [left, right) 重叠
3. 遍历所有重叠区间，对每个区间：
   - 如果左端点 < left，保留左残 `[origLeft, left)`
   - 如果右端点 > right，保留右残 `[right, origRight)`
   - 删除原区间
4. 插入残留区间

### 解法二：线段树（动态开点）

- 值域 `[1, 10^9]` 太大，无法开固定数组，需要动态开点
- 每个节点维护区间是否完全覆盖的标志
- 使用懒标记（lazy propagation）处理区间更新
- 实现复杂，面试时间紧时不推荐

## 易错点

1. **addRange 时忘记检查前一个区间**：`upper_bound(left)` 返回的迭代器的前一个区间可能右端点覆盖了 left，必须回退检查。
2. **删除区间时修改迭代器**：在遍历 map 时删除元素，需要先保存 next 迭代器再删除，否则迭代器失效。C++11 起 `erase()` 返回下一个迭代器。
3. **removeRange 的残留区间处理**：删除 [left, right) 后，原区间两端可能有残留，必须重新插入。
4. **queryRange 的边界**：使用 `upper_bound` 而非 `lower_bound` 可以简化逻辑——`upper_bound(left)` 的前一个区间（如果存在）是唯一可能覆盖 left 的候选。
5. **空 map 的处理**：查询或删除时 map 可能为空，此时 `begin() == end()`，不要解引用。

## 面试追问

1. **Q: 如果操作次数极多（10^6），有序 Map 的性能是否足够？**
   A: addRange 和 removeRange 最坏 O(n) 但摊销分析下总操作数与区间合并/分裂次数线性相关。每次 add 最多创建 1 个区间，每次 remove 最多创建 2 个。总区间数不超 O(ops)，摊销性能良好。

2. **Q: 能否用并查集（Union-Find）来做？**
   A: 区间合并可以用 UF，但区间分裂（removeRange）很难用 UF 高效处理，所以不适合。

3. **Q: 如果查询频率远高于修改频率，如何优化？**
   A: queryRange 已经是 O(log n)，非常高效。如果需要更快，可以考虑缓存或分块。

4. **Q: 如果要支持 countRange（统计区间内被覆盖的长度），怎么改？**
   A: 在 addRange/removeRange 时同步维护一个全局覆盖长度计数器，根据合并/分裂时的变化量更新。

5. **Q: 与 LeetCode 352 (Data Stream as Disjoint Intervals) 有什么异同？**
   A: 352 是只有 addNum（单点插入），本题是区间操作（add/remove/query）。数据结构类似（有序 map），但本题操作更复杂。

## 相关题型

- [56. Merge Intervals](../0056_merge_intervals/) - 区间合并基础
- [57. Insert Interval](../0057_insert_interval/) - 插入区间并合并
- [352. Data Stream as Disjoint Intervals](../0352_data_stream_as_disjoint_intervals/) - 数据流合并区间
- [436. Find Right Interval](../0436_find_right_interval/) - 二分查找区间
- [729. My Calendar I](../0729_my_calendar_i/) - 区间不重叠判断
