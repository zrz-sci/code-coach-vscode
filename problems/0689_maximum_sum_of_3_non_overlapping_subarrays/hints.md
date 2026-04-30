# 689. Maximum Sum of 3 Non-Overlapping Subarrays - 三个无重叠子数组的最大和

## 难度：Hard

## 标签：Array, Dynamic Programming, Sliding Window, Prefix Sum

---

## 核心思路

将问题分解为三步：(1) 预处理每个位置开始的长度为 k 的窗口和；(2) 预处理 `left[i]` = 在 [0, i] 范围内窗口和最大的起始索引，`right[i]` = 在 [i, end] 范围内窗口和最大的起始索引；(3) 枚举中间子数组的起始位置 j，用 `left[j-k]` 和 `right[j+k]` 快速找到左右两侧的最优子数组。三者之和最大的组合即为答案。通过 left/right 数组的构建方式保证字典序最小。

---

## 思维链

1. 暴力做法是枚举三个子数组的起始位置，O(n^3) 不可接受。
2. 固定中间子数组后，左右两侧的最优子数组是独立的 → 可以预处理。
3. `windowSum[i]` = `nums[i] + nums[i+1] + ... + nums[i+k-1]`，可以用滑动窗口在 O(n) 内算出。
4. `left[i]` = `argmax(windowSum[0..i])`，从左往右扫描维护。
5. `right[i]` = `argmax(windowSum[i..m-1])`，从右往左扫描维护。
6. 枚举中间子数组起始位置 j ∈ [k, m-1-k]：
   - 左侧最优：`left[j-k]`（左侧子数组最晚在 j-k 开始才不重叠）
   - 右侧最优：`right[j+k]`（右侧子数组最早在 j+k 开始才不重叠）
   - 三段总和 = `windowSum[left[j-k]] + windowSum[j] + windowSum[right[j+k]]`
7. 取总和最大的组合，其字典序最小性由 left/right 的构建方式保证。

---

## 解法概览

| 解法 | 时间 | 空间 | 特点 |
|------|------|------|------|
| 预处理 left/right 最优数组 | O(n) | O(n) | 经典做法，最高效 |
| DP: dp[i][t] = 前 i 个窗口选 t 个的最大和 | O(n * 3) = O(n) | O(n) | DP 思路更通用，可推广到 m 个子数组 |
| 暴力枚举（优化剪枝） | O(n^2) ~ O(n^3) | O(n) | 仅做对比，实际不可取 |

---

## 关键提示

### 提示 1 - 窗口和的计算
用滑动窗口在 O(n) 内计算所有 `windowSum[i]`：维护一个大小为 k 的窗口，每次右移一位，加入新元素减去旧元素。或者用前缀和：`windowSum[i] = prefixSum[i+k] - prefixSum[i]`。

### 提示 2 - left 数组保证字典序最小
构建 `left[i]` 时，当 `windowSum[i] > windowSum[best]` 才更新（严格大于）。这确保在窗口和相等时保留更小的索引，从而保证字典序最小。

### 提示 3 - right 数组保证字典序最小
构建 `right[i]` 时，当 `windowSum[i] >= windowSum[best]` 就更新（大于等于）。从右往左扫描，遇到相等时更新为当前更小的索引，也是为了字典序。

### 提示 4 - 不重叠约束
三个子数组分别从 l, j, r 开始，长度都是 k。不重叠要求：
- `l + k <= j`（左和中不重叠）→ `l <= j - k`
- `j + k <= r`（中和右不重叠）→ `r >= j + k`

所以 `left[j-k]` 和 `right[j+k]` 恰好覆盖了不重叠的范围。

### 提示 5 - 推广到 m 个子数组
如果题目改为选 m 个不重叠子数组，可以用 DP：`dp[i][t]` = 在前 i 个窗口中选 t 个不重叠窗口的最大和。转移：选第 i 个窗口则 `dp[i][t] = dp[i-k][t-1] + windowSum[i]`，不选则 `dp[i][t] = dp[i-1][t]`。

---

## 解法详解

### 解法一：left/right 预处理 + 枚举中间

**Step 1: 计算窗口和数组**
```
windowSum 大小为 m = n - k + 1
windowSum[0] = nums[0] + nums[1] + ... + nums[k-1]
windowSum[i] = windowSum[i-1] + nums[i+k-1] - nums[i-1]
```

**Step 2: 构建 left 数组**
```
left[0] = 0
for i = 1 to m-1:
    if windowSum[i] > windowSum[left[i-1]]:
        left[i] = i        // 严格大于才更新 → 字典序最小
    else:
        left[i] = left[i-1]
```

**Step 3: 构建 right 数组**
```
right[m-1] = m-1
for i = m-2 downto 0:
    if windowSum[i] >= windowSum[right[i+1]]:
        right[i] = i       // 大于等于就更新 → 字典序最小
    else:
        right[i] = right[i+1]
```

**Step 4: 枚举中间子数组**
```
ans = {-1, -1, -1}, maxTotal = 0
for j = k to m-1-k:
    l = left[j - k]
    r = right[j + k]
    total = windowSum[l] + windowSum[j] + windowSum[r]
    if total > maxTotal:
        maxTotal = total
        ans = {l, j, r}
```

**正确性论证：**
- left[j-k] 给出 [0, j-k] 范围内最优窗口，且与 j 不重叠
- right[j+k] 给出 [j+k, m-1] 范围内最优窗口，且与 j 不重叠
- 枚举所有合法的 j，必然覆盖最优解

### 解法二：DP 通用做法

定义 `dp[t][i]` = 在窗口 0..i 中选 t 个不重叠窗口的最大和

```
转移:
dp[t][i] = max(dp[t][i-1],              // 不选窗口 i
               dp[t-1][i-k] + windowSum[i]) // 选窗口 i

base case: dp[0][i] = 0 for all i
```

答案 = `dp[3][m-1]`。需要额外回溯找到选了哪三个窗口。

---

## 易错点

1. **left/right 数组的更新条件搞反**：left 用严格大于（`>`），right 用大于等于（`>=`）。搞反会导致字典序不对。
2. **枚举范围错误**：中间子数组 j 的范围是 `[k, m-1-k]`（包含两端）。左边至少留 k 个位置给左侧子数组，右边同理。
3. **windowSum 的索引**：`windowSum` 大小为 `n - k + 1`，不是 n。索引 i 表示从 `nums[i]` 到 `nums[i+k-1]` 的和。
4. **滑动窗口初始化**：先累加前 k 个元素作为 `windowSum[0]`，后续滑动时加一减一。
5. **maxTotal 初始值**：初始化为 0 或负无穷都可以（nums[i] >= 1），但用 0 更安全。
6. **返回类型是索引数组**：返回的是三个子数组的起始索引，不是子数组的和。
7. **字典序"最小"意味着什么**：{0, 2, 4} < {0, 2, 5} < {0, 3, 4}，按位比较。

---

## 面试追问

1. **如果改为选 m 个不重叠子数组怎么做？**
   → 用 DP：`dp[t][i]` 表示前 i 个窗口选 t 个的最大和。时间 O(n * m)。需要回溯路径来恢复索引。

2. **如果子数组长度可以不同呢？**
   → 如果三个子数组长度分别为 k1, k2, k3，需要分别计算三种窗口和，left/right 需要根据不同的 k 值调整间距。

3. **如果要求第二大（而不是最大）的组合呢？**
   → 需要在 left/right 中维护 top-2，或者用 DP 记录前两名。复杂度不变。

4. **空间能优化到 O(1) 吗？**
   → windowSum 可以边算边用，但 left/right 数组必须预处理，至少需要 O(n) 空间。除非用不同的算法框架。

5. **如何处理 nums 中有负数的情况？**
   → 算法完全不变。windowSum 可能为负，但比较逻辑和更新逻辑不受影响。maxTotal 初始值应设为负无穷。

---

## 相关题型

| 题号 | 题目 | 关联 |
|------|------|------|
| 123 | Best Time to Buy and Sell Stock III | 类似的"左右预处理 + 枚举分割点"模式 |
| 188 | Best Time to Buy and Sell Stock IV | DP 选 k 段不重叠区间 |
| 53 | Maximum Subarray | 单个最大子数组，基础 Kadane 算法 |
| 152 | Maximum Product Subarray | 子数组优化的变体 |
| 918 | Maximum Sum Circular Subarray | 环形数组上的子数组和 |
| 2106 | Maximum Fruits Harvested After at Most K Steps | 滑动窗口 + 预处理 |
