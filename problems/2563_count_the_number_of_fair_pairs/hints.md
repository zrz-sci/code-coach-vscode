# 2563. 统计公平数对的数目

## 核心思路

在数组中找满足 `lower <= nums[i] + nums[j] <= upper` 且 `i < j` 的数对 (i,j) 的个数。核心技巧：**排序不影响计数**（因为排序后仍能正确统计满足和约束的对数），然后用**双指针**或**二分查找**高效计算。进一步，利用**区间分解**将范围约束拆分为两个单边问题：`count(sum <= upper) - count(sum <= lower-1)`。

## 思维链

1. **读完题第一反应**：暴力 O(n^2) 枚举所有 (i,j) 对，检查和是否在 [lower, upper] 范围内。对于 n=10^5 来说太慢。

2. **关键洞察 — 排序不改变答案**：
   - 题目要求的是满足条件的 **(i,j) 对的数量**，不关心具体是哪些下标
   - 排序后，原来的一对 (i,j) 变成了不同位置的一对，但"两个元素的和"不变
   - 排序后我们能利用有序性加速查找

3. **区间分解技巧 — 化范围为单边**：
   - `lower <= nums[i] + nums[j] <= upper`
   - 等价于 `count(sum <= upper) - count(sum <= lower - 1)`
   - 这样只需实现一个 `countPairs(bound)` 函数：统计 `nums[i] + nums[j] <= bound` 的对数

4. **双指针的工作原理**：
   - 排序后，left=0, right=n-1
   - 如果 `nums[left] + nums[right] <= bound`：left 能和 `[left+1, right]` 中所有元素配对（因为右边的都更大，但 left 是当前最小值，和最大值配都满足，和中间值当然也满足），贡献 `right - left` 对，然后 left++
   - 如果 `nums[left] + nums[right] > bound`：当前 right 太大了，right--

5. **为什么这个双指针是正确的**：排序后，固定 left，所有满足条件的 right 是一个连续区间 `[left+1, R]`。双指针利用了"left 增大时 R 不增"的单调性。

```
区间分解 + 双指针 可视化:

nums = [0, 1, 7, 4, 4, 5],  lower = 3, upper = 6

排序后: [0, 1, 4, 4, 5, 7]

目标: count(sum <= 6) - count(sum <= 2)

--- countPairs(bound=6) ---
  [0, 1, 4, 4, 5, 7]
   L              R    0+7=7 > 6  → R--
   L           R       0+5=5 <= 6 → count += 4 (与1,4,4,5都能配), L++
      L        R       1+5=6 <= 6 → count += 3 (与4,4,5配), L++
         L     R       4+5=9 > 6  → R--
         L  R          4+4=8 > 6  → R--
         LR            L >= R 结束
  count(6) = 4 + 3 = 7

--- countPairs(bound=2) ---
  [0, 1, 4, 4, 5, 7]
   L              R    0+7=7 > 2 → R--
   L           R       0+5=5 > 2 → R--
   L        R          0+4=4 > 2 → R--
   L     R             0+4=4 > 2 → R--
   L  R                0+1=1 <= 2 → count += 1, L++
      LR               L >= R 结束
  count(2) = 1

答案: 7 - 1 = 6  ✓
```

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试推荐 |
|------|------|------|------|----------|
| 排序 + 双指针 | 区间分解 + 双指针计数 | O(n log n) | O(1) | ⭐ 首选 |
| 排序 + 二分查找 | 对每个i二分找j的范围 | O(n log n) | O(1) | 备选 |

## 解法详解

### 解法1: 排序 + 双指针 -- O(n log n) / O(1)

**思考过程**：范围约束 → 区间分解为两个单边问题 → 对每个单边问题用双指针高效计数。

**核心公式**：`答案 = countPairs(upper) - countPairs(lower - 1)`

其中 `countPairs(bound)` 统计排序后数组中 `nums[i] + nums[j] <= bound` 且 `i < j` 的对数。

```cpp
class Solution {
public:
    long long countFairPairs(vector<int>& nums, int lower, int upper) {
        sort(nums.begin(), nums.end());
        return countPairs(nums, upper) - countPairs(nums, lower - 1);
    }

    long long countPairs(vector<int>& nums, int bound) {
        long long count = 0;
        int left = 0, right = nums.size() - 1;
        while (left < right) {
            if (nums[left] + nums[right] <= bound) {
                // left能与[left+1, right]中所有元素配对
                count += right - left;
                left++;
            } else {
                right--;
            }
        }
        return count;
    }
};
```

**双指针正确性的详细分析**：

```
为什么 nums[left]+nums[right] <= bound 时，count += right-left ？

排序后: a0 <= a1 <= ... <= a_{n-1}

当 a[left] + a[right] <= bound:
  → a[left] + a[right-1] <= bound  (因为 a[right-1] <= a[right])
  → a[left] + a[right-2] <= bound
  → ...
  → a[left] + a[left+1] <= bound
  
所以 left 能和 right, right-1, ..., left+1 一共 (right-left) 个元素配对
全部满足 sum <= bound

count += right - left 一次性统计了 left 的所有贡献
然后 left++ 处理下一个元素
```

### 解法2: 排序 + 二分查找 -- O(n log n) / O(1)

**思考过程**：排序后，对每个 `nums[i]`，用二分查找确定满足 `lower <= nums[i] + nums[j] <= upper` 的 j 的范围。

**核心**：对于固定的 `i`，需要 `lower - nums[i] <= nums[j] <= upper - nums[i]`。在排序后的数组中，用 `lower_bound` 和 `upper_bound` 找到 j 的范围。

```cpp
class Solution {
public:
    long long countFairPairs(vector<int>& nums, int lower, int upper) {
        sort(nums.begin(), nums.end());
        long long count = 0;
        int n = nums.size();
        
        for (int i = 0; i < n; i++) {
            // j 必须在 i 之后 (排序后用 i+1 作为搜索起点)
            // nums[j] >= lower - nums[i]
            int lo = lower - nums[i];
            // nums[j] <= upper - nums[i]
            int hi = upper - nums[i];
            
            // 在 nums[i+1..n-1] 中找 [lo, hi] 范围内的元素个数
            auto left_it = lower_bound(nums.begin() + i + 1, nums.end(), lo);
            auto right_it = upper_bound(nums.begin() + i + 1, nums.end(), hi);
            
            count += (right_it - left_it);
        }
        
        return count;
    }
};
```

## 关键提示

- **提示1**：排序不影响计数——我们统计的是满足和约束的无序对的数量，排序前后这个数量不变。
- **提示2**：区间分解是处理"两端约束"的经典技巧。`lower <= x <= upper` 可以拆为 `count(x <= upper) - count(x <= lower-1)`，化双端为单端。
- **提示3**：双指针中 `count += right - left` 是一次性统计的关键——利用了排序后的单调性，left 能和 left+1 到 right 的所有元素配对。
- **提示4**：注意使用 `long long`！n=10^5 时最多有约 5*10^9 对，超过 int 范围。
- **提示5**：二分查找方法中，搜索范围是 `nums[i+1..n-1]`（不包含 i 自身），避免 (i,i) 这种非法对。

## 易错点

1. **忘记排序**：双指针和二分查找都依赖数组有序，忘记排序会得到错误结果。
2. **整数溢出**：返回值是 `long long`，中间计算（如 `right - left`）在累加时也要用 `long long`。
3. **区间分解的减一**：是 `countPairs(lower - 1)` 不是 `countPairs(lower)`，因为要包含 `sum == lower` 的情况。
4. **二分查找的搜索范围**：必须从 `i+1` 开始搜索，否则会把 (i,i) 计入。
5. **双指针终止条件**：`left < right`，不是 `left <= right`，因为一个元素不能和自己配对。

## 复杂度分析

| 维度 | 双指针 | 二分查找 |
|------|--------|----------|
| 时间 | O(n log n) 排序 + O(n) 双指针 x 2次 | O(n log n) 排序 + O(n log n) 二分 |
| 空间 | O(1) 原地排序 | O(1) 原地排序 |
| 总计 | O(n log n) | O(n log n) |

虽然两者都是 O(n log n)，但双指针常数因子更小（每次 countPairs 是 O(n)），实际运行更快。

## 相关题型

- **167. 两数之和 II (有序数组)** — 双指针基础：在有序数组中找和为 target 的一对
- **1. 两数之和** — HashMap 方法找和为 target 的一对
- **1877. 数组中最大数对和的最小值** — 排序+双指针的 minimax 问题
- **327. 区间和的个数** — 更复杂的范围计数问题，需要归并排序

## Follow-up 思考

1. **如果要返回具体的数对（而非数量）？** 不能排序（排序会丢失原始下标），需要用 HashMap 或 indexed sort。
2. **如果是三元组满足 lower <= sum <= upper？** 固定一个元素后变成二元问题，O(n^2 log n) 或 O(n^2)。
3. **如果数组在线到来（流式）？** 排序不可行，需要平衡BST（如 `std::multiset`）维护已见元素，每来一个新元素用 lower_bound/upper_bound 查找。
