# 2080. 区间内查询数字的频率 (Range Frequency Queries)

[LeetCode 链接](https://leetcode.com/problems/range-frequency-queries/)

## 题目理解

设计一个数据结构，支持以下操作：
- **构造**：给定整数数组 `arr`（下标从 0 开始）
- **查询**：给定 `left, right, value`，返回 `arr[left...right]` 中 `value` 出现的次数

**约束规模**：
- `arr.length <= 10^5`
- `arr[i], value <= 10^4`
- 最多 `10^5` 次查询

**核心挑战**：暴力法每次查询 O(n)，`10^5 * 10^5 = 10^10` 必然 TLE。需要 **O(log n)** 的查询方案。

## 第一步提示：如何快速知道某个值出现在哪些位置？

<details>
<summary>展开提示</summary>

**关键观察**：查询只关心 **特定值** 在 **特定区间** 内的出现次数。

如果我们能提前知道每个值出现在数组中的哪些下标位置，就能把"区间频率查询"转化为"在一个有序数组中查区间"的问题。

**预处理思路**：
- 用 **哈希表** `map<int, vector<int>>`
- key = 值，value = 该值出现的所有下标（按自然顺序，天然递增）

例如 `arr = [12, 33, 4, 56, 22, 2, 34, 33, 22, 12, 34, 56]`：
```
12 → [0, 9]
33 → [1, 7]
4  → [2]
56 → [3, 11]
22 → [4, 8]
2  → [5]
34 → [6, 10]
```

现在 `query(0, 11, 33)` 就变成：在 `[1, 7]` 中有多少个值落在 `[0, 11]` 范围内？

</details>

## 第二步提示：如何在有序数组中高效查区间？

<details>
<summary>展开提示</summary>

每个值的下标列表天然递增，这就是一个 **有序数组**。要查 `[left, right]` 范围内有多少个元素？

经典方法：**二分查找**！

- 用 `lower_bound` 找到第一个 `>= left` 的位置 → `lo`
- 用 `upper_bound` 找到第一个 `> right` 的位置 → `hi`
- 频率 = `hi - lo`

```
indices of 33: [1, 7]
query(0, 11, 33):
  lower_bound([1,7], 0) → 指向 1 (第0个位置)
  upper_bound([1,7], 11) → 指向 end (第2个位置)
  频率 = 2 - 0 = 2 ✓

query(1, 2, 4):
  indices of 4: [2]
  lower_bound([2], 1) → 指向 2 (第0个位置)
  upper_bound([2], 2) → 指向 end (第1个位置)
  频率 = 1 - 0 = 1 ✓
```

**为什么用 `lower_bound` 和 `upper_bound`？**
- `lower_bound(left)` → 第一个 `>= left` 的迭代器（包含 left 本身）
- `upper_bound(right)` → 第一个 `> right` 的迭代器（排除 right 之后）
- 区间 `[left, right]` 包含两端，刚好需要这对组合

</details>

## 第三步提示：边界情况和实现细节

<details>
<summary>展开提示</summary>

**value 不存在于数组中**：
- 哈希表中找不到 value → 直接返回 0
- 注意要先检查 key 是否存在，否则 `unordered_map` 会自动插入空 vector

**时间复杂度分析**：
- 构造：O(n)（遍历一次数组）
- 查询：O(log n)（二分查找，每个值的下标列表长度总和为 n）
- n 和 query 次数都是 10^5，总操作 O(n + q * log n) ≈ 10^5 * 17 ≈ 2 * 10^6，完全足够

**空间复杂度**：O(n)，存储所有下标。

**C++ 实现要点**：
```cpp
unordered_map<int, vector<int>> idx;
// 构造时:
for (int i = 0; i < arr.size(); i++)
    idx[arr[i]].push_back(i);
// 查询时:
auto& v = idx[value]; // 用引用避免拷贝
auto lo = lower_bound(v.begin(), v.end(), left);
auto hi = upper_bound(v.begin(), v.end(), right);
return hi - lo;
```

</details>

## 第四步提示（备选思路）：前缀和数组

<details>
<summary>展开提示</summary>

**思路**：对每个不同的值维护一个前缀和数组。
- `prefix[value][i]` = `arr[0..i-1]` 中 value 出现的次数
- 查询 `[left, right]` = `prefix[value][right+1] - prefix[value][left]`

**问题**：值域 `[1, 10^4]`，数组长度 `10^5`，总空间 `10^4 * 10^5 = 10^9`，太大！

**优化**：只对出现过的值维护前缀和。最多 `10^5` 个不同值，每个前缀和长度 `10^5`，但实际上所有不同值的出现次数总和为 n，所以用哈希表 + 二分更优。

**结论**：前缀和在这道题的约束下空间太大，**哈希表 + 二分** 是最优解。

</details>

## 第五步提示（进阶）：线段树 / 分块

<details>
<summary>展开提示</summary>

如果要支持**动态修改**（比如修改数组元素后再查询频率），哈希表 + 二分就不够了，需要：

**分块法**：
- 将数组分成 `sqrt(n)` 块，每块维护一个频率计数器
- 查询时边界块暴力，中间块直接查计数器
- 修改 O(1)，查询 O(sqrt(n))

**线段树 + 离散化**：
- 每个节点存储一个频率哈希表（merge 需要合并）
- 查询 O(log^2 n)，修改 O(log^2 n)
- 空间 O(n * log n)，实际中因为值域限制可行

但本题是**静态查询**（构造后不修改），哈希表 + 二分是最优选择。

</details>

## 思维链总结

```
需求: 多次查询区间内某值的出现次数
  |
  v
关键: 对每个值记录其出现的所有下标(天然有序)
  |
  v
转化: "区间频率" → "有序数组中 [left,right] 范围的元素个数"
  |
  v
工具: 二分查找 (lower_bound + upper_bound)
  |
  v
构造 O(n), 查询 O(log n)
```

## 解法对比

| 解法 | 构造时间 | 查询时间 | 空间 | 适用场景 |
|------|---------|---------|------|---------|
| 哈希表+二分 | O(n) | O(log n) | O(n) | 静态查询，最优 |
| 前缀和 | O(n * V) | O(1) | O(n * V) | 值域小时可用 |
| 暴力遍历 | O(1) | O(n) | O(n) | 仅用于验证 |
| 线段树(merge) | O(n log n) | O(log^2 n) | O(n log n) | 支持动态修改 |
| 分块 | O(n) | O(sqrt(n)) | O(n * sqrt(n)) | 支持动态修改 |

## 复杂度分析

**哈希表 + 二分**：
- 构造时间：O(n)（遍历数组，每个元素 O(1) 插入哈希表）
- 查询时间：O(log n)（二分查找，某个值的下标列表长度最大为 n）
- 空间：O(n)（所有下标列表长度之和 = n）

**注意**：`unordered_map` 的查找是平均 O(1)，最坏 O(n)。如果面试官关心这一点，可以用 `vector<vector<int>>` 代替（因为值域 <= 10^4，开大小为 10001 的数组即可）。

## 易错点

1. **先检查 value 是否存在**：`unordered_map` 的 `[]` 操作会自动插入空值，改用 `find` 或 `count` 检查
2. **`lower_bound` vs `upper_bound` 的语义**：lower_bound 找 `>=`，upper_bound 找 `>`
3. **用引用获取 vector**：`auto& v = idx[value]` 而非 `auto v = ...`，避免拷贝
4. **值域限制**：`arr[i] <= 10^4`，可以用定长数组代替哈希表提升常数
5. **空 vector 的二分**：如果 value 不存在，直接返回 0，不要对空 vector 做二分

## 模式识别

本题是经典的 **"倒排索引 + 二分查找"** 模式：
- 建立从 **值** 到 **位置列表** 的映射（倒排索引）
- 利用位置列表有序的性质做二分区间查询

这个模式广泛应用于：搜索引擎（倒排索引查询）、数据库（B+ 树范围查询）、日志分析（按时间戳二分）等场景。

## 相关题目

| 题号 | 题目 | 关联 |
|------|------|------|
| 1146 | Snapshot Array | 类似的"存下标 + 二分查历史"模式 |
| 981 | Time Based Key-Value Store | 哈希表 + 有序列表 + 二分 |
| 350 | Intersection of Two Arrays II | 哈希表统计频率 |
| 362 | Design Hit Counter | 设计题 + 二分查时间窗口 |
| 34 | Find First and Last Position of Element in Sorted Array | lower_bound / upper_bound 经典练习 |

---

## 面试追问

**Q1: 如果需要支持单点修改 `arr[i] = new_val`，怎么做？**
> 分块法：更新 `arr[i]` 所在块的频率表，O(1) 修改 + O(sqrt(n)) 查询。或者用 BIT/线段树按值域维护，但实现复杂度更高。

**Q2: 如果查询改为"区间内出现次数最多的值"，怎么做？**
> 这是经典的区间众数问题（Range Mode Query），没有已知的 O(n) 预处理 + O(1) 查询方案。常见做法：分块 + 预处理块间众数 O(n * sqrt(n)) 预处理 / O(sqrt(n)) 查询；或 Mo's Algorithm O((n+q) * sqrt(n))。

**Q3: 如果值域很大（`arr[i] <= 10^9`），倒排索引方案需要改吗？**
> 不需要改。`unordered_map` 本身就支持大值域，空间仍为 O(n)，因为只存储出现过的值。但定长数组方案就不适用了。如果担心哈希冲突，可以先离散化再用定长数组。
