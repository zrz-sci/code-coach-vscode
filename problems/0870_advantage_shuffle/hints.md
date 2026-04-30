# 870. 优势洗牌 (Advantage Shuffle)

## 核心思路

本质就是经典的**田忌赛马**策略：给定 `nums1` 和 `nums2` 两组"马"，如何排列 `nums1`，使得 `nums1[i] > nums2[i]` 的位数最多？

**贪心策略**：对于 `nums2` 中的每一个元素，用 `nums1` 中**刚好能赢它的最小元素**去对抗（"以小胜大"）；如果没有能赢的，就把 `nums1` 中**最小的**（最弱的）扔给它当炮灰。

## 思维链

1. **读完题第一反应** → 需要找到 `nums1` 的一个排列，使得 `nums1[i] > nums2[i]` 的位数尽可能多。暴力做法是枚举所有排列 O(n!)，这不可能。
2. **联想到什么？** → 田忌赛马！这是贪心分配的经典场景。
3. **贪心策略怎么定？** → 对 `nums2` 的每个元素，最优策略是用"刚好比它大的最小值"去打它。能赢就赢，不能赢就送人头（把最差的给它）。
4. **为什么"刚好赢"而不是"用最大的去赢"？** → 用最大的去赢是浪费资源。如果用 100 去打 10，那 100 本来可以打 90。我们要用最节约的方式取胜，把大数留给更需要的位置。
5. **为什么"不能赢就扔最小"？** → 既然这个位置赢不了，不如把最差的牌打出去，保留好牌给其他能赢的位置。
6. **实现方式？** → 排序 `nums1`，用 multiset 或双指针实现高效的 "找到刚好大于 x 的元素" 操作。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 贪心 + Multiset | 用 multiset 的 upper_bound 找刚好赢的牌 | O(n log n) | O(n) | 推荐，代码简洁 |
| 贪心 + 排序 + 双指针 | 排序后用双端对打策略 | O(n log n) | O(n) | 面试首选，思路清晰 |
| 贪心 + TreeMap | 用有序映射维护可用牌 | O(n log n) | O(n) | Java 友好 |

## 关键提示

1. **田忌赛马的关键**：对手的出场顺序固定（`nums2` 的索引位置），我们只能调整自己的出场顺序（重排 `nums1`）。
2. **排序但要记住原始位置**：`nums2` 不能直接排序（因为结果要按原始 `nums2` 的位置存放），所以要对 `nums2` 的索引排序。
3. **upper_bound 而非 lower_bound**：因为题目要求严格大于 `>`，所以要找 `upper_bound`，不是 `lower_bound`。
4. **为什么贪心正确？** → 可以用交换论证：假设存在一个最优解不遵循"刚好赢"策略，可以通过交换得到一个不差于它的"刚好赢"解。

## 解法详解

### 解法1: 贪心 + Multiset — O(n log n) / O(n)

**思考过程**: 我们需要一个数据结构来维护 `nums1` 的可用元素，支持：
- 查找大于某值的最小元素（upper_bound）
- 查找最小元素
- 删除一个元素

`multiset` 完美满足以上三个需求。

```
田忌赛马策略图解:

nums2 = [13, 25, 32, 11]
nums1 = [12, 24, 8, 32]  →  multiset: {8, 12, 24, 32}

处理 nums2[0] = 13:
  upper_bound(13) → 24   可以赢！ result[0] = 24
  multiset: {8, 12, 32}

处理 nums2[1] = 25:
  upper_bound(25) → 32   可以赢！ result[1] = 32
  multiset: {8, 12}

处理 nums2[2] = 32:
  upper_bound(32) → end  赢不了！送最小 result[2] = 8
  multiset: {12}

处理 nums2[3] = 11:
  upper_bound(11) → 12   可以赢！ result[3] = 12
  multiset: {}

结果: [24, 32, 8, 12] ✓
```

```cpp
class Solution {
public:
    vector<int> advantageCount(vector<int>& nums1, vector<int>& nums2) {
        multiset<int> avail(nums1.begin(), nums1.end());
        int n = nums2.size();
        vector<int> result(n);
        
        for (int i = 0; i < n; i++) {
            // 找到 nums1 中刚好大于 nums2[i] 的最小值
            auto it = avail.upper_bound(nums2[i]);
            if (it != avail.end()) {
                // 有能赢的牌，用它
                result[i] = *it;
            } else {
                // 没有能赢的牌，扔最小的当炮灰
                it = avail.begin();
                result[i] = *it;
            }
            avail.erase(it); // 用掉这张牌
        }
        
        return result;
    }
};
```

**关键点**:
- `multiset` 而非 `set`：`nums1` 可能有重复元素。
- `erase(it)` 只删一个：`erase(value)` 会删所有重复的，而 `erase(iterator)` 只删一个。
- 时间：每次 upper_bound O(log n)，共 n 次 → O(n log n)。

---

### 解法2: 贪心 + 排序 + 双指针 — O(n log n) / O(n) ⭐ 面试首选

**从解法1优化思路**: 解法1 用 multiset 的 upper_bound 来找"刚好赢"的牌。另一种方式是直接排序，然后用双端策略——对 `nums2` 从大到小处理，用 `nums1` 的最大值去尝试赢。

**策略转换**：
- 将 `nums2` 的**索引**按值从大到小排序
- 将 `nums1` 排序
- 用两个指针 `lo` 和 `hi` 指向 `nums1` 排序后的两端
- 对于 `nums2` 从大到小的每个值：
  - 如果 `nums1[hi]` 能赢它 → 分配 `nums1[hi]`，`hi--`
  - 否则 → 送 `nums1[lo]`（最小的炮灰），`lo++`

```
双指针策略图解:

nums1 排序后: [8, 12, 24, 32]
                lo            hi

nums2 按值排序的索引: [2(32), 1(25), 0(13), 3(11)]

Step 1: nums2[2]=32, nums1[hi]=32, 32>32? No → 送 nums1[lo]=8, lo++
  result[2] = 8,  lo=1, hi=3

Step 2: nums2[1]=25, nums1[hi]=32, 32>25? Yes → result[1]=32, hi--
  result[1] = 32, lo=1, hi=2

Step 3: nums2[0]=13, nums1[hi]=24, 24>13? Yes → result[0]=24, hi--
  result[0] = 24, lo=1, hi=1

Step 4: nums2[3]=11, nums1[hi]=12, 12>11? Yes → result[3]=12, hi--
  result[3] = 12, lo=1, hi=0

结果: [24, 32, 8, 12] ✓
```

```cpp
class Solution {
public:
    vector<int> advantageCount(vector<int>& nums1, vector<int>& nums2) {
        int n = nums1.size();
        
        // 对 nums2 的索引按值从大到小排序
        vector<int> idx(n);
        iota(idx.begin(), idx.end(), 0);
        sort(idx.begin(), idx.end(), [&](int a, int b) {
            return nums2[a] > nums2[b];
        });
        
        sort(nums1.begin(), nums1.end());
        
        vector<int> result(n);
        int lo = 0, hi = n - 1;
        
        for (int i : idx) {
            if (nums1[hi] > nums2[i]) {
                result[i] = nums1[hi--]; // 能赢就用最大的赢
            } else {
                result[i] = nums1[lo++]; // 赢不了就送最小的
            }
        }
        
        return result;
    }
};
```

**关键点**:
- **为什么从大到小处理 nums2？** → 先处理最难赢的对手。如果最大的牌能赢，就用它；否则对最强的对手也只能送人头。
- **为什么双指针不会出错？** → 每次只消费一端的牌。赢了消费右端（大牌），输了消费左端（小牌），两端互不干扰。
- 空间上只额外用了 `idx` 和 `result`，没有复杂数据结构。

## 面试应对

### 面试官可能追问

**Q: 为什么贪心是正确的？能给出证明吗？**

A: 用**交换论证**。假设最优解 OPT 中，对 `nums2[i]`，OPT 使用了 `nums1[x]`（能赢），而 `nums1[x]` 不是"刚好赢"的最小值。设"刚好赢"的最小值为 `nums1[y]`，则 `nums1[y] < nums1[x]` 且 `nums1[y] > nums2[i]`。把 `nums1[x]` 和 `nums1[y]` 在 OPT 中的位置交换后：
- 位置 i: `nums1[y]` 仍然赢 `nums2[i]`（因为 `nums1[y] > nums2[i]`）
- 位置 j（原来 `nums1[y]` 的位置）: `nums1[x] > nums1[y]`，所以 `nums1[x]` 赢的能力不低于 `nums1[y]`
- 交换后总优势不减少，因此"刚好赢"策略至少与 OPT 一样好。

**Q: 如果 `nums2` 可以重排呢？（双方都可以排列）**

A: 那就是完全版的田忌赛马博弈问题。这里简化了：对手出场顺序固定，只需要安排自己。

**Q: 时间复杂度分析？**

A: 排序 O(n log n)，遍历 O(n)，总体 O(n log n)。空间 O(n) 存结果和索引。

## 相关题目

| 题目 | 关联 |
|------|------|
| [881. 救生艇](https://leetcode.com/problems/boats-to-save-people/) | 排序 + 双指针贪心分配 |
| [455. 分发饼干](https://leetcode.com/problems/assign-cookies/) | 贪心匹配策略 |
| [826. 安排工作以达到最大收益](https://leetcode.com/problems/most-profit-assigning-work/) | 排序 + 贪心匹配 |

## Follow-up 思考

1. **如果要求返回的优势数量最大且字典序最小？** → 贪心策略类似，但在多个"刚好赢"的候选中选最小的（upper_bound 自然做到了）。
2. **如果 nums1 的长度大于 nums2？** → 多出来的牌不用，先选出最有利的 n 张牌来打。
3. **如果允许不严格大于（>=）也算赢？** → 把 `upper_bound` 改成 `lower_bound` 即可。
