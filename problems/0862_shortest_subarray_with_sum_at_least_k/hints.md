# 862. Shortest Subarray with Sum at Least K

## 核心思路

本题要求找到和至少为 k 的最短连续子数组。由于数组中可能包含负数，经典的滑动窗口（双指针）方法不适用，因为负数会导致前缀和不单调递增。解决方案是使用**前缀和 + 单调双端队列（monotonic deque）**。

关键洞察：构建前缀和数组 prefix[i]，子数组 [l, r) 的和为 prefix[r] - prefix[l]。我们需要找到使 prefix[r] - prefix[l] >= k 且 r - l 最小的 (l, r) 对。维护一个存储前缀和索引的单调递增双端队列：
1. 对于当前位置 r，从队首弹出所有满足 prefix[r] - prefix[deque.front()] >= k 的元素（贪心取最短）。
2. 从队尾弹出所有 prefix[deque.back()] >= prefix[r] 的元素（它们永远不会比 r 更优）。

## 思维链

1. **前缀和转化**：将子数组和问题转化为两个前缀和之差：sum(l..r-1) = prefix[r] - prefix[l]。
2. **目标**：找最小的 r - l 使得 prefix[r] - prefix[l] >= k，其中 l < r。
3. **暴力思路**：对每个 r，遍历所有 l < r 检查。O(n^2)。
4. **优化观察一**：如果 prefix[l1] >= prefix[l2] 且 l1 < l2，那么 l1 永远不如 l2（l2 的前缀和更小或相等且距离更近）。所以队列中的前缀和应该是严格递增的。
5. **优化观察二**：如果 prefix[r] - prefix[l] >= k，那么对于后续的 r' > r，l 不会产生更短的子数组（因为 r' - l > r - l）。所以满足条件后可以从队首弹出 l。
6. **单调队列**：维护一个索引队列，使得对应的 prefix 值严格递增。这两个优化使得每个元素最多入队出队一次，总时间 O(n)。

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| 前缀和 + 单调队列 | O(n) | O(n) | 最优解 |
| 前缀和 + 堆 | O(n log n) | O(n) | 较容易理解但更慢 |
| 前缀和 + 二分搜索 | O(n log n) | O(n) | 利用单调性二分 |

## 关键提示

1. **为什么滑动窗口不行**：滑动窗口要求缩小窗口时和减少。但有负数时，缩小窗口可能反而增加和，破坏了单调性。
2. **前缀和数组长度 n+1**：prefix[0] = 0 表示空前缀，prefix[n] = 全部元素之和。
3. **单调队列存索引不存值**：队列中存前缀和的索引，通过索引访问前缀和值。
4. **严格递增**：队列中维护的前缀和是严格递增的（不含等于）。
5. **long long 防溢出**：前缀和可能很大，需要用 long long。

## 解法详解

### 解法一：前缀和 + 单调双端队列（最优解）

**思路**：

构建前缀和数组。用双端队列维护候选左端点的索引，确保对应前缀和严格递增。遍历每个右端点 r：

1. **队首检查**：while 队列非空且 prefix[r] - prefix[front] >= k：
   - 更新 ans = min(ans, r - front)
   - 弹出队首（front 不会再被后续 r 使用，因为距离只会更大）
2. **队尾维护**：while 队列非空且 prefix[back] >= prefix[r]：
   - 弹出队尾（back 被 r 支配：r 的前缀和更小且位置更靠右）
3. **入队**：将 r 加入队尾。

**步骤**：
1. 计算 prefix[0..n]，prefix[0] = 0，prefix[i] = prefix[i-1] + nums[i-1]。
2. 初始化 deque dq，ans = INT_MAX。
3. 遍历 r = 0 到 n：
   - 队首弹出并更新 ans
   - 队尾弹出维护单调性
   - r 入队
4. 返回 ans == INT_MAX ? -1 : ans。

**复杂度分析**：
- 时间：O(n)，每个索引最多入队和出队各一次
- 空间：O(n)，前缀和数组和队列

### 解法二：前缀和 + 最小堆

**思路**：

用最小堆存储 (prefix[i], i)。遍历每个 r，从堆顶弹出所有满足 prefix[r] - heap.top().first >= k 的元素并更新答案。然后将 (prefix[r], r) 入堆。

**复杂度**：O(n log n) 时间，O(n) 空间。

### 解法三：前缀和 + TreeMap/二分搜索

**思路**：

维护一个有序结构存储之前见过的 (prefix[l], l)。对于每个 r，二分查找 prefix[r] - k，找到所有 prefix[l] <= prefix[r] - k 的左端点中最大的索引。

**复杂度**：O(n log n) 时间，O(n) 空间。

## 易错点

1. **前缀和用 int 导致溢出**：nums[i] 最大 1e5，n 最大 1e5，前缀和最大 1e10，必须用 long long。
2. **忘记 prefix[0] = 0**：前缀和数组需要 n+1 个元素，prefix[0] = 0 代表空子数组。
3. **单调队列弹出顺序错误**：队首是找满足条件的最短子数组（贪心弹出），队尾是维护单调性（弹出被支配的）。
4. **返回 -1 的情况**：如果所有元素之和 < k，或无法找到满足条件的子数组。
5. **将 nums 全正数的滑动窗口思路硬套到有负数的情况**：这是本题的核心陷阱。
6. **队列中存值而非索引**：需要存索引来计算子数组长度。

## 面试追问

1. **为什么单调队列中被弹出的元素不会是更优解的一部分？**
   - 队首弹出：prefix[front] 已经找到了满足 >= k 的最近右端点 r，后续 r' > r 只会让子数组更长。
   - 队尾弹出：prefix[back] >= prefix[r] 且 back < r，对于任何未来的右端点 r'，用 r 替代 back 既差值更大（更容易满足 >= k）又距离更近。

2. **如果数组全是正数，能用什么更简单的方法？**
   - 经典滑动窗口（双指针），O(n) 时间，无需前缀和数组。

3. **如果要找和恰好等于 k 的最短子数组呢？**
   - 哈希表存前缀和首次出现的索引，遍历时查找 prefix[r] - k。

4. **如何输出具体的子数组（而非只是长度）？**
   - 在更新 ans 时记录对应的 l 和 r 值。

5. **这题和 209. Minimum Size Subarray Sum 的区别？**
   - 209 保证全正数，可用滑动窗口。本题有负数，需要单调队列。

## 相关题型

- [209. Minimum Size Subarray Sum](https://leetcode.com/problems/minimum-size-subarray-sum/) - 全正数版本，滑动窗口
- [239. Sliding Window Maximum](https://leetcode.com/problems/sliding-window-maximum/) - 单调队列经典应用
- [560. Subarray Sum Equals K](https://leetcode.com/problems/subarray-sum-equals-k/) - 前缀和 + 哈希表
- [1438. Longest Continuous Subarray With Absolute Diff <= Limit](https://leetcode.com/problems/longest-continuous-subarray-with-absolute-diff-less-than-or-equal-to-limit/) - 单调队列维护极值
- [1499. Max Value of Equation](https://leetcode.com/problems/max-value-of-equation/) - 单调队列优化
- [303. Range Sum Query - Immutable](https://leetcode.com/problems/range-sum-query-immutable/) - 前缀和基础
