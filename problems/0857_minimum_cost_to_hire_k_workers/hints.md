# 857. Minimum Cost to Hire K Workers

## 核心思路

本题的核心在于理解"按比例支付"这一约束。如果选定一组 k 个工人，每个工人的实际工资与其 quality 成正比，而"比率"由该组中 wage[i]/quality[i] 最大的那个工人决定。因此，总花费 = (该组所有工人的 quality 之和) * (该组中最大的 wage/quality 比率)。

关键洞察：按 wage/quality 比率从小到大排序后，依次考虑每个工人作为"比率决定者"（即该工人是当前组中比率最大的）。在其之前的所有工人都满足最低工资要求，我们只需从中选 quality 最小的 k-1 个工人即可最小化总花费。这可以通过一个最大堆（max-heap）维护当前最小的 k 个 quality 值来实现。

## 思维链

1. **定义比率**：对每个工人 i，定义 ratio[i] = wage[i] / quality[i]，这是雇佣该工人时每单位 quality 的最低单价。
2. **排序**：按 ratio 从小到大排序所有工人。
3. **贪心选择**：从左到右遍历排序后的工人。当前工人的 ratio 是组内最大的，因此总花费 = ratio[i] * sum_of_quality。
4. **维护堆**：用最大堆维护当前组中 quality 值最大的工人。当堆大小超过 k 时，弹出 quality 最大的，同时更新 quality 总和。
5. **更新答案**：当堆中恰好有 k 个工人时，计算当前总花费并更新全局最小值。

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| 排序 + 最大堆 | O(n log n) | O(n) | 最优解，贪心 + 优先队列 |
| 暴力枚举 | O(C(n,k) * k) | O(k) | 枚举所有组合，不可行 |

## 关键提示

1. **比率排序是关键**：按 wage/quality 排序后，遍历到第 i 个工人时，前 i-1 个工人的比率都不超过 ratio[i]，所以用 ratio[i] 作为统一比率，前面的工人一定满足最低工资。
2. **最大堆维护最小 quality 子集**：我们想让 quality 之和最小，所以用最大堆弹出 quality 最大的工人。
3. **浮点精度**：ratio 用 double 计算即可，题目允许 1e-5 误差。
4. **边界处理**：必须在堆中有恰好 k 个元素时才更新答案。

## 解法详解

### 解法一：排序 + 最大堆（最优解）

**思路**：

将所有工人按 wage[i]/quality[i] 升序排序。维护一个大小为 k 的最大堆存储 quality 值，以及一个 qualitySum 变量。遍历排序后的每个工人：将其 quality 加入堆和 qualitySum。如果堆大小超过 k，弹出最大的 quality 并从 qualitySum 中减去。当堆大小等于 k 时，用 qualitySum * 当前工人的 ratio 更新答案。

**步骤**：
1. 构建 (ratio, quality) 对并按 ratio 排序。
2. 初始化最大堆 pq，qualitySum = 0，ans = DBL_MAX。
3. 遍历每个工人 (r, q)：
   - qualitySum += q, pq.push(q)
   - 若 pq.size() > k：qualitySum -= pq.top(), pq.pop()
   - 若 pq.size() == k：ans = min(ans, qualitySum * r)
4. 返回 ans。

**复杂度分析**：
- 时间：O(n log n) 排序 + O(n log k) 堆操作 = O(n log n)
- 空间：O(n) 存储排序数组 + O(k) 堆

## 易错点

1. **忘记用 double 类型**：ratio 必须用浮点数，整数除法会丢失精度。
2. **堆方向搞反**：我们要保留最小的 k 个 quality，所以用最大堆弹出最大值。
3. **在堆未满 k 个时就更新答案**：必须等堆中有恰好 k 个元素。
4. **排序时 ratio 相等的工人处理**：ratio 相等时顺序不影响结果，无需特殊处理。
5. **整数溢出**：quality 和 wage 都是 int，ratio 计算时注意类型转换。

## 面试追问

1. **为什么按 ratio 排序后贪心是正确的？**
   - 因为当前工人的 ratio 是组内最大值，所有之前的工人用该 ratio 支付一定满足其最低工资。总花费只取决于 ratio 和 quality 之和，ratio 固定时最小化 quality 之和即可。

2. **如果允许最多雇佣 k 个工人（不一定恰好 k 个），怎么处理？**
   - 在遍历过程中，堆大小从 1 到 k 的每一步都更新答案取最小值。

3. **如果工人有附加条件（如同一城市不能超过 m 人），如何修改？**
   - 需要更复杂的数据结构或回溯策略，堆中需要记录额外信息。

4. **时间复杂度能否优化到 O(n log k)？**
   - 排序的 O(n log n) 是瓶颈。若数据已排序可以做到 O(n log k)。

5. **如何处理 quality 或 wage 为 0 的情况？**
   - 题目约束 quality >= 1, wage >= 1，实际面试中需确认。

## 相关题型

- [253. Meeting Rooms II](https://leetcode.com/problems/meeting-rooms-ii/) - 贪心 + 堆
- [502. IPO](https://leetcode.com/problems/ipo/) - 排序 + 贪心 + 堆
- [871. Minimum Number of Refueling Stops](https://leetcode.com/problems/minimum-number-of-refueling-stops/) - 贪心 + 最大堆
- [1383. Maximum Performance of a Team](https://leetcode.com/problems/maximum-performance-of-a-team/) - 排序 + 最小堆，与本题思路非常相似
- [2542. Maximum Subsequence Score](https://leetcode.com/problems/maximum-subsequence-score/) - 排序 + 堆选 k 个元素
