# 1838. 最高频元素的频数

## 核心思路

排序后，把若干个**相邻且较小的元素**增加到和某个目标值相等，问在总操作次数 ≤ k 的约束下，最多能让多少个元素变成同一个值。本质是**排序 + 滑动窗口/二分查找**求满足代价约束的最长子数组。

## 思维链

1. **读完题第一反应**：只能把元素增加 1，不能减少。所以要让尽可能多的元素变成同一个值，应该把较小的元素"拉高"到某个目标值。目标值应该选谁？自然选窗口里**最大的那个元素**（因为它不需要操作，其他元素往它靠拢代价最小）。

2. **排序是关键观察**：排序后，要拉齐到 `nums[right]` 的元素一定是它左边的一段连续区间 `[left, right]`。为什么？因为排序后越靠近 `nums[right]` 的元素需要的操作越少，选不连续的元素一定不如选连续的划算。

3. **暴力做法**：枚举每个 right 作为目标值，再枚举 left 看最多能把多少个元素拉齐。代价 = `nums[right] * (right - left + 1) - sum(nums[left..right])`。暴力计算 sum 是 O(n²)。

4. **前缀和优化求和**：用前缀和快速算区间和，把求和从 O(n) 降到 O(1)。但如何快速找最优的 left？
   - **方案A：二分查找** — 对于固定的 right，代价随 left 减小（窗口变大）而单调递增，所以可以二分找最小的 left 使得代价 ≤ k。
   - **方案B：滑动窗口** — 当代价 > k 时收缩左边界，右边界不断扩展。

5. **滑动窗口为什么正确**：排序后，right 右移时目标值变大，代价变大；left 右移时窗口变小，代价变小。窗口两端都是单调移动的，满足滑动窗口的条件。

6. **复杂度**：排序 O(n log n)，滑动窗口/二分都是 O(n)（二分是 O(n log n)），总体 O(n log n)。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力枚举 | 枚举目标值和左边界 | O(n²) | O(1) | 能说出即可 |
| 排序 + 前缀和 + 二分 | 二分查找最远左边界 | O(n log n) | O(n) | 加分项 |
| 排序 + 滑动窗口 | 维护代价 ≤ k 的最长窗口 | O(n log n) | O(1) | ⭐ 必须写出 |

## 关键提示

1. **排序不影响答案**：我们只关心频数（个数），不关心元素的原始位置。排序后，最优的目标值一定是数组中已有的某个元素（把它作为窗口右端点）。

2. **代价公式**：把窗口 `[left, right]` 内所有元素增加到 `nums[right]`，总代价 = `nums[right] * windowSize - windowSum`。想想为什么？

3. **滑动窗口的收缩条件**：当 `nums[right] * windowSize - windowSum > k` 时，说明预算不够，需要左边界右移，丢弃最小的元素（它代价最高）。

4. **注意数据类型**：`nums[right] * windowSize` 可能溢出 int（最大 10⁵ × 10⁵ = 10¹⁰），需要用 `long long`。

5. **二分 vs 滑动窗口**：二分是"对每个 right 独立找 left"，滑动窗口是"left 和 right 一起单调前进"。滑动窗口代码更简洁，面试首选。

```
排序后: [1, 2, 4], k = 5

right=0: 窗口[1], 代价=0, 频数=1
right=1: 窗口[1,2], 代价=2*2-3=1 ≤ 5, 频数=2
right=2: 窗口[1,2,4], 代价=4*3-7=5 ≤ 5, 频数=3 ✓ 答案!
```

## 解法详解

### 解法1: 暴力枚举 — O(n²) / O(1)

**思考过程**：最直觉的方式——排序后，枚举每个元素作为目标值（right），然后从 right 往左扩展，累加代价，直到超过 k 为止。

```cpp
class Solution {
public:
    int maxFrequency(vector<int>& nums, int k) {
        sort(nums.begin(), nums.end());
        int n = nums.size(), ans = 1;
        for (int right = 1; right < n; right++) {
            long long cost = 0;
            int count = 1; // nums[right] 自己
            for (int left = right - 1; left >= 0; left--) {
                // 把 nums[left] 增加到 nums[right] 的代价
                cost += (long long)(nums[right] - nums[left]);
                if (cost > k) break;
                count++;
            }
            ans = max(ans, count);
        }
        return ans;
    }
};
```

**关键点**：时间复杂度 O(n²)，对于 n = 10⁵ 会超时。但它帮助我们理解代价的计算方式，为优化铺路。

---

### 解法2: 排序 + 前缀和 + 二分查找 — O(n log n) / O(n)

**从解法1优化**：解法1对每个 right 线性扫描 left，瓶颈在于"找最远的 left"。观察到：固定 right 时，窗口越大代价越高（单调递增），所以可以**二分查找**最小的 left。

代价公式：`cost = nums[right] * (right - left + 1) - (prefix[right+1] - prefix[left])`

```cpp
class Solution {
public:
    int maxFrequency(vector<int>& nums, int k) {
        sort(nums.begin(), nums.end());
        int n = nums.size();
        
        // 构建前缀和，用于 O(1) 求区间和
        vector<long long> prefix(n + 1, 0);
        for (int i = 0; i < n; i++) {
            prefix[i + 1] = prefix[i] + nums[i];
        }
        
        int ans = 1;
        for (int right = 0; right < n; right++) {
            // 二分找最小的 left，使得代价 ≤ k
            // 搜索空间: [0, right]
            int lo = 0, hi = right;
            while (lo < hi) {
                int mid = lo + (hi - lo) / 2;
                long long windowSize = right - mid + 1;
                long long windowSum = prefix[right + 1] - prefix[mid];
                long long cost = (long long)nums[right] * windowSize - windowSum;
                if (cost <= k) {
                    hi = mid; // mid 可行，尝试更小的 left（更大的窗口）
                } else {
                    lo = mid + 1; // 代价太大，left 右移缩小窗口
                }
            }
            // lo == hi == 最优的 left
            ans = max(ans, right - lo + 1);
        }
        return ans;
    }
};
```

**二分搜索空间收缩过程**（以 `[1,4,8,13], k=5, right=3(target=13)` 为例）：
```
// 搜索 left ∈ [0, 3]:
// Round1: lo=0, hi=3, mid=1
//   window=[4,8,13], cost=13*3-25=14 > 5 → lo=2
// Round2: lo=2, hi=3, mid=2
//   window=[8,13], cost=13*2-21=5 ≤ 5 → hi=2
// lo==hi==2, 频数=3-2+1=2 ✓
```

**关键点**：
- 二分的判断：`cost <= k` 时向左探索（hi = mid），`cost > k` 时向右收缩（lo = mid + 1）
- 前缀和数组大小是 n+1，prefix[0] = 0

---

### 解法3: 排序 + 滑动窗口 — O(n log n) / O(1) ⭐ 面试首选

**从解法2再优化**：解法2对每个 right 独立二分找 left，但注意到 right 右移时，left 也只会右移不会左移（因为目标值变大了，代价变高，需要缩小窗口）。这正是**滑动窗口**的特征！

```cpp
class Solution {
public:
    int maxFrequency(vector<int>& nums, int k) {
        sort(nums.begin(), nums.end());
        int n = nums.size(), ans = 1;
        long long windowSum = 0;
        int left = 0;
        
        for (int right = 0; right < n; right++) {
            windowSum += nums[right];
            
            // 代价 = 目标值 * 窗口大小 - 窗口和
            // 当代价 > k 时，收缩左边界
            while ((long long)nums[right] * (right - left + 1) - windowSum > k) {
                windowSum -= nums[left];
                left++;
            }
            
            // 当前窗口 [left, right] 满足代价 ≤ k
            ans = max(ans, right - left + 1);
        }
        return ans;
    }
};
```

**滑动窗口过程演示**（`[1,2,4], k=5`）：

```
排序后: [1, 2, 4]

right=0: windowSum=1, window=[1]
  cost = 1*1 - 1 = 0 ≤ 5 ✓  → ans=1

right=1: windowSum=3, window=[1,2]
  cost = 2*2 - 3 = 1 ≤ 5 ✓  → ans=2

right=2: windowSum=7, window=[1,2,4]
  cost = 4*3 - 7 = 5 ≤ 5 ✓  → ans=3

最终答案: 3
```

再看 `[1,4,8,13], k=5`：

```
right=0: windowSum=1, window=[1]
  cost=0 ≤ 5 → ans=1

right=1: windowSum=5, window=[1,4]
  cost=4*2-5=3 ≤ 5 → ans=2

right=2: windowSum=13, window=[1,4,8]
  cost=8*3-13=11 > 5 → 收缩: windowSum-=1, left=1
  window=[4,8], cost=8*2-12=4 ≤ 5 → ans=2

right=3: windowSum=25, window=[4,8,13]
  cost=13*3-25=14 > 5 → 收缩: windowSum-=4, left=2
  window=[8,13], cost=13*2-21=5 ≤ 5 → ans=2

最终答案: 2
```

**关键点**：`windowSum` 用 `long long`，否则 `nums[right] * (right - left + 1)` 可能溢出。

---

## 解法对比

| | 暴力 | 二分 + 前缀和 | 滑动窗口 |
|---|---|---|---|
| 时间 | O(n²) | O(n log n) | O(n log n)* |
| 空间 | O(1) | O(n) 前缀和 | O(1)** |
| 代码量 | 简短 | 中等 | 最简洁 |
| 面试推荐 | 说思路即可 | 展示二分能力 | ⭐ 首选 |

\*排序 O(n log n) 是瓶颈，滑动窗口本身 O(n)。  
\**不算排序所用空间。

**什么时候选哪个？**
- 滑动窗口是面试首选：代码短、空间小、容易解释。
- 二分查找适合"对每个 right 独立查询"的场景，如果题目变体不满足滑动窗口的单调性，就需要二分。

## 易错点

1. **整数溢出**
   - ✗ `nums[right] * (right - left + 1)` — 两个 int 相乘，结果可能超过 2^31
   - ✓ `(long long)nums[right] * (right - left + 1)` — 先转 long long

2. **忘记排序**
   - ✗ 直接在未排序数组上滑窗 — 窗口内不连续，代价公式不成立
   - ✓ 排序是前提！排序后才能保证"拉齐到 nums[right] 代价最小的一定是它左边的连续元素"

3. **二分边界搞混**
   - ✗ `hi = mid - 1` — 可能跳过最优的 left
   - ✓ `hi = mid`（因为 mid 满足条件时，mid 本身可能是答案，不能排除）

4. **windowSum 的维护**
   - ✗ 在 while 收缩时忘记 `windowSum -= nums[left]`
   - ✓ 每次 left++ 前必须从 windowSum 中减去 nums[left]

## 面试追问

**Q1: 为什么排序后贪心选连续元素是最优的？**
> 排序后，要把若干元素拉高到 target，选择最接近 target 的元素代价最小。如果跳过某个中间元素而选更远的，总代价只会更大。所以最优解一定是排序后的一个连续子数组。

**Q2: 能否不排序解决？**
> 不排序的话，无法利用连续子数组的性质。你需要枚举所有子集，复杂度指数级。排序是这道题的关键 insight——O(n log n) 的排序代价换来了 O(n) 的扫描。

**Q3: 如果操作改为"可以增加或减少 1"，怎么做？**
> 目标值不再一定是窗口最大值。最优目标值是窗口的**中位数**（这是"最小化绝对偏差和"的经典结论）。可以用排序 + 滑动窗口 + 中位数维护来解。难度显著增加，参考 LeetCode 462。

**Q4: 这道题和"最长子数组使得代价 ≤ k"有什么关系？**
> 本质上就是同一个问题！排序后，"最高频数"等价于"找最长的子数组 [left, right] 使得 nums[right]*(right-left+1) - sum ≤ k"。这是滑动窗口的经典形式。

## 相关题型

- **LeetCode 209 长度最小的子数组** — 滑动窗口模板题，复用"右扩左缩"框架，区别：209 是求满足条件的最短窗口，本题求最长窗口。
- **LeetCode 1423 可获得的最大点数** — 也涉及滑动窗口 + 贪心，但窗口在数组两端。
- **LeetCode 2024 考试的最大困扰度** — 几乎相同的"最长窗口使得代价 ≤ k"模型，复用完全相同的滑动窗口框架。
- **LeetCode 462 最少移动次数使数组元素相等 II** — 上面 Q3 追问的变体：可增可减时取中位数。