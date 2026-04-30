# 53. 最大子数组和

## 核心思路

本质上是在所有连续子数组中找和最大的那个。关键洞察：**遍历到每个位置时，你只需要做一个决策——是把当前元素接在前面的子数组后面，还是从当前元素重新开始一段新的子数组。**

## 思维链

1. **读完题第一反应**：枚举所有连续子数组，算出每个的和，取最大值。子数组由起点 `i` 和终点 `j` 确定，暴力枚举是 O(n²) 或 O(n³)。
2. **暴力解的瓶颈**：O(n²) 对每个 `(i, j)` 区间求和，很多计算是重复的。有没有办法在遍历的过程中"一次性"找到答案？
3. **核心观察**：考虑以第 `i` 个元素**结尾**的最大子数组和 `dp[i]`。如果 `dp[i-1] > 0`，说明前面那段子数组对我有正向贡献，接上它；否则，前面那段是累赘，不如从 `nums[i]` 重新开始。
4. **状态转移**：`dp[i] = max(dp[i-1] + nums[i], nums[i])` = `max(dp[i-1], 0) + nums[i]`。
5. **答案**：`max(dp[0], dp[1], ..., dp[n-1])`。
6. **空间优化**：`dp[i]` 只依赖 `dp[i-1]`，所以只需要一个变量，空间 O(1)。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力枚举 | 枚举所有 (i,j) 区间求和 | O(n²) | O(1) | 能说出即可 |
| Kadane 算法 (DP) | 以每个位置结尾的最大子数组和 | O(n) | O(1) | ⭐ 必须写出 |
| 前缀和 | maxSum = max(prefixSum[j] - minPrefix) | O(n) | O(1) | 加分项 |
| 分治法 | 线段树思想，左/右/跨越三种情况 | O(n log n) | O(log n) | 加分项 (Follow-up) |

## 关键提示

1. **子问题定义**：不要试图定义 `dp[i]` 为"前 i 个元素中的最大子数组和"——这样无法写出转移方程（因为不知道最大子数组是否包含 `nums[i]`）。正确定义是"以 `nums[i]` **结尾**的最大子数组和"。
2. **Kadane 的决策**：到每个位置只有两个选择——延续前面的子数组 or 重新开始。如果前面的累积和是负数，延续只会拖后腿。
3. **前缀和视角**：子数组 `[i..j]` 的和 = `prefix[j] - prefix[i-1]`。固定 `j`，要让这个差最大，就要让 `prefix[i-1]` 最小。所以维护一个"到目前为止的最小前缀和"。
4. **分治法**：把数组一分为二，最大子数组要么全在左半，要么全在右半，要么**跨越中点**。跨越中点的情况从中间向两边扩展即可。

```
DP 填表过程 (nums = [-2, 1, -3, 4, -1, 2, 1, -5, 4]):

index:   0    1    2    3    4    5    6    7    8
nums:   -2    1   -3    4   -1    2    1   -5    4
dp:     -2    1   -2    4    3    5    6    1    5
         ↑    ↑    ↑    ↑
         重新  重新  接上  重新   ← 每步的决策
         开始  开始  前面  开始

全局最大: max(dp) = 6, 对应子数组 [4, -1, 2, 1]
```

## 解法详解

### 解法1: 暴力枚举 — O(n²) / O(1)

**思考过程**: 最直觉的方式——枚举每个可能的起点 `i`，然后从 `i` 往右扩展终点 `j`，维护当前区间和，更新全局最大值。

```cpp
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        int n = nums.size();
        int result = INT_MIN;
        for (int i = 0; i < n; i++) {
            int sum = 0;
            for (int j = i; j < n; j++) {
                sum += nums[j];           // 从 i 到 j 的区间和（增量计算）
                result = max(result, sum);
            }
        }
        return result;
    }
};
```

**关键点**: 注意 `result` 初始化为 `INT_MIN` 而不是 `0`，因为数组可能全是负数，最大子数组和也是负的。

---

### 解法2: Kadane 算法 (动态规划) — O(n) / O(1) ⭐ 面试首选

**从解法1优化**: 暴力的瓶颈是对每个起点都要遍历一遍。换个角度：不枚举起点，而是对每个**终点** `i`，思考以 `i` 结尾的最大子数组和是多少。

**状态定义**: `dp[i]` = 以 `nums[i]` 结尾的最大子数组和

**转移方程**: `dp[i] = max(dp[i-1] + nums[i], nums[i])`

含义：要么接上前面的子数组（`dp[i-1] + nums[i]`），要么从自己重新开始（`nums[i]`）。

**空间优化**: `dp[i]` 只依赖 `dp[i-1]`，用一个变量 `curMax` 滚动即可。

```cpp
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        // curMax: 以当前元素结尾的最大子数组和
        // result: 全局最大子数组和
        int curMax = nums[0];
        int result = nums[0];
        
        for (int i = 1; i < nums.size(); i++) {
            // 核心决策：接上前面 or 重新开始
            // 等价于: if (curMax < 0) curMax = 0; curMax += nums[i];
            curMax = max(curMax + nums[i], nums[i]);
            result = max(result, curMax);
        }
        return result;
    }
};
```

**关键点**: 
- 初始化 `curMax = nums[0]` 和 `result = nums[0]`，循环从 `i=1` 开始
- 也可以初始化 `curMax = 0, result = INT_MIN`，循环从 `i=0` 开始

---

### 解法3: 前缀和 — O(n) / O(1)

**不同视角**: 子数组 `nums[i..j]` 的和等于 `prefix[j] - prefix[i-1]`。要最大化这个差值，对于每个右端点 `j`，我们需要找到 `j` 之前最小的前缀和。

```
前缀和视角:
nums:    [-2,  1,  -3,  4,  -1,  2,  1,  -5,  4]
prefix: [0, -2, -1, -4,  0,  -1,  1,  2,  -3,  1]
                                          ↑       ↑
              minPrefix = -4 (在index 3)   maxSum = prefix[7] - (-4) = 2-(-4) = 6
```

```cpp
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        int result = INT_MIN;
        int prefixSum = 0;
        int minPrefix = 0;  // prefix[0] = 0，表示"一个都不取"
        
        for (int i = 0; i < nums.size(); i++) {
            prefixSum += nums[i];
            // 当前前缀和 - 之前最小前缀和 = 以 i 结尾的某段子数组的最大和
            result = max(result, prefixSum - minPrefix);
            // 更新最小前缀和（注意：先用再更新，保证子数组非空）
            minPrefix = min(minPrefix, prefixSum);
        }
        return result;
    }
};
```

**关键点**: `minPrefix` 初始化为 `0`（对应"不取任何前缀"），且必须**先计算 result 再更新 minPrefix**，否则会出现空子数组。

---

### 解法4: 分治法 — O(n log n) / O(log n) 【Follow-up】

**思路**: 类似归并排序，把数组从中间一分为二：
- 最大子数组全在**左半部分**
- 最大子数组全在**右半部分**  
- 最大子数组**跨越中点**

前两种递归解决，第三种从中点向两边扩展求最大。

```
分治过程 (nums = [-2, 1, -3, 4, -1, 2, 1, -5, 4]):

                    [-2, 1, -3, 4, -1, 2, 1, -5, 4]
                   /                                 \
         [-2, 1, -3, 4]                    [-1, 2, 1, -5, 4]
         /            \                    /               \
     [-2, 1]      [-3, 4]            [-1, 2, 1]        [-5, 4]
     /    \       /    \             /         \        /    \
   [-2]  [1]   [-3]  [4]      [-1, 2]        [1]   [-5]   [4]

跨越中点的处理（以顶层为例）:
  左半部分从 mid 向左扩展: ... + 4 = 4
  右半部分从 mid+1 向右扩展: -1 + 2 + 1 = 2
  跨越中点和: 4 + 2 = 6 ← 这就是全局最大
```

```cpp
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        return solve(nums, 0, nums.size() - 1);
    }
    
private:
    int solve(vector<int>& nums, int left, int right) {
        if (left == right) return nums[left];  // base case: 只有一个元素
        
        int mid = left + (right - left) / 2;
        int leftMax = solve(nums, left, mid);
        int rightMax = solve(nums, mid + 1, right);
        int crossMax = crossingMax(nums, left, mid, right);
        
        return max({leftMax, rightMax, crossMax});
    }
    
    int crossingMax(vector<int>& nums, int left, int mid, int right) {
        // 从 mid 向左扩展，找最大和
        int leftSum = INT_MIN, sum = 0;
        for (int i = mid; i >= left; i--) {
            sum += nums[i];
            leftSum = max(leftSum, sum);
        }
        // 从 mid+1 向右扩展，找最大和
        int rightSum = INT_MIN;
        sum = 0;
        for (int i = mid + 1; i <= right; i++) {
            sum += nums[i];
            rightSum = max(rightSum, sum);
        }
        return leftSum + rightSum;
    }
};
```

**关键点**: `crossingMax` 中 `leftSum` 和 `rightSum` 初始化为 `INT_MIN`，因为跨越中点的子数组**必须**包含 mid 和 mid+1 两个位置。

---

## 解法对比

| | 暴力枚举 | Kadane (DP) | 前缀和 | 分治 |
|---|---|---|---|---|
| 时间 | O(n²) | O(n) | O(n) | O(n log n) |
| 空间 | O(1) | O(1) | O(1) | O(log n) 栈 |
| 思维难度 | 低 | 中 | 中 | 高 |
| 面试推荐 | 起步 | ⭐ 首选 | 展示多角度 | Follow-up |
| 本质 | 穷举 | 局部最优决策 | 差值最大化 | 分而治之 |

- **Kadane vs 前缀和**: 本质上等价。Kadane 的 `max(curMax + nums[i], nums[i])` 等价于前缀和中"如果前缀和比之前的最小值还小，就重置"。
- **分治法**: 时间不如 Kadane，但展示了分治思想，是面试 Follow-up 的经典考点。

## 易错点

1. **全负数组的处理**:
   - ✗ `result = 0` → 全负数组会错误返回 0
   - ✓ `result = nums[0]` 或 `result = INT_MIN` → 正确返回最大的那个负数

2. **前缀和的更新顺序**:
   - ✗ 先 `minPrefix = min(minPrefix, prefixSum)` 再算 `result` → 可能让子数组长度为 0
   - ✓ 先算 `result = max(result, prefixSum - minPrefix)` 再更新 `minPrefix`

3. **分治法中 crossingMax 的初始化**:
   - ✗ `leftSum = 0` → 如果 mid 左边全是负数，`leftSum` 应该取最大的负数而不是 0
   - ✓ `leftSum = INT_MIN` → 因为跨越中点的子数组必须至少包含一个左边元素

4. **Kadane 的等价写法容易出错**:
   - `if (curMax < 0) curMax = 0; curMax += nums[i];` 这种写法要注意初始化 `result = INT_MIN`
   - 如果初始化 `result = 0` 配合这个写法，全负数组会返回 0

## 面试追问

**Q1: 暴力解能通过吗？瓶颈在哪？**
> 暴力 O(n²) 对 n=10⁵ 约 10¹⁰ 次操作，超时。瓶颈在于对每个起点都要遍历到末尾。

**Q2: Kadane 算法的贪心决策为什么是正确的？**
> 如果 `dp[i-1] < 0`，加上 `nums[i]` 后一定比 `nums[i]` 本身小，所以丢弃前面的累积不会错过最优解。因为最大子数组的起点一定不会让前面的累积为负。

**Q3: 如果要求返回最大子数组的起止下标呢？**
> 在 Kadane 中额外维护 `start, end, tempStart`。当 `curMax < 0` 重新开始时更新 `tempStart = i`；当 `curMax > result` 时更新 `start = tempStart, end = i`。

**Q4: 如果是环形数组（首尾相连），最大子数组和怎么求？**
> 这就是 LeetCode 918。答案是 `max(普通最大子数组和, 总和 - 最小子数组和)`。注意全负数组的特殊情况（最小子数组 = 整个数组时答案不能用第二种）。

## 相关题型

- **152. 乘积最大子数组** — 复用 Kadane 的"以 i 结尾"的 DP 定义，区别：乘法中负负得正，需同时维护最大值和最小值
- **918. 环形子数组的最大和** — 复用 Kadane，增加一个"总和减最小子数组和"的情况（面试追问 Q4 的原题）
- **121. 买卖股票的最佳时机** — 前缀和视角的直接变体：`maxProfit = price[j] - minPrice`，和本题的 `prefixSum - minPrefix` 完全同构
- **560. 和为 K 的子数组** — 前缀和 + 哈希表，把"求最大"变成"求等于 K 的个数"