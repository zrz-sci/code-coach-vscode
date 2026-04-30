# 152. 乘积最大子数组

## 核心思路

这道题本质上是"最大连续子数组"问题的变体——和 **LeetCode 53 最大子数组和** 类似，但乘法的特殊性在于：**一个负数乘以一个很小的负数，可以变成一个很大的正数**。所以我们不仅要追踪"最大乘积"，还要同时追踪"最小乘积"（因为最小的负乘积随时可能翻身变最大）。

## 思维链

1. **读完题第一反应** → 枚举所有子数组，算出每个子数组的乘积，取最大值。暴力可行，但 O(n²)。

2. **能否像"最大子数组和"一样用 DP？** → 53 题的状态转移是 `dp[i] = max(dp[i-1] + nums[i], nums[i])`，意思是"要么接上前面的子数组，要么重新开始"。乘积能不能套？

3. **发现问题** → 加法中，`dp[i-1]` 为负一定要丢弃（接上只会更小）。但乘法中，`dp[i-1]` 为负且 `nums[i]` 也为负，两负相乘反而是最大的！所以只维护"最大"是不够的。

4. **关键洞察** → **同时维护以 `i` 结尾的最大乘积和最小乘积**。当 `nums[i]` 是负数时，最大和最小会交换——上一步的最小乘积乘上负数变成了最大乘积。

5. **写出转移方程** →
   - `curMax = max(nums[i], max(prevMax * nums[i], prevMin * nums[i]))`
   - `curMin = min(nums[i], min(prevMax * nums[i], prevMin * nums[i]))`
   - `answer = max(answer, curMax)`

6. **空间优化** → `dp[i]` 只依赖 `dp[i-1]`，所以只需要两个变量滚动即可，空间 O(1)。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力枚举 | 枚举所有子数组，计算乘积 | O(n²) | O(1) | 能说出即可 |
| DP（数组版） | 维护 dpMax[i] 和 dpMin[i] | O(n) | O(n) | 过渡理解 |
| DP（空间优化） | 滚动变量，只保留前一步 | O(n) | O(1) | ⭐ 必须写出 |
| 前后缀乘积 | 从左往右和从右往左各扫一遍 | O(n) | O(1) | 加分项 |

## 关键提示

1. **和 53 题对比**：最大子数组和只需要维护一个 `dp`，为什么乘积需要同时维护最大和最小？想想负数的作用。

2. **负数的"翻转"效应**：`-10 × -10 = 100`。如果前面积累了一个很大的负乘积，遇到负数时它会变成最大的正乘积。

3. **零的"截断"效应**：乘以 0 后一切归零，相当于"重新开始"。

4. **空间优化的前提**：`dp[i]` 只依赖 `dp[i-1]`，经典的滚动变量场景。

5. **前后缀解法的直觉**：如果没有零，整个数组的乘积要么从左边开始最大，要么从右边开始最大（取决于负数个数的奇偶）。零把数组分成了多段，每段独立处理。

```
示例: nums = [2, 3, -2, 4]

暴力枚举所有子数组:
[2]=2, [2,3]=6, [2,3,-2]=-12, [2,3,-2,4]=-48
      [3]=3, [3,-2]=-6, [3,-2,4]=-24
            [-2]=-2, [-2,4]=-8
                    [4]=4
最大 = 6

DP 过程:
index:  0    1    2     3
nums:   2    3   -2     4
dpMax:  2    6   -2     4    ← max(nums[i], prevMax*nums[i], prevMin*nums[i])
dpMin:  2    3  -12   -48    ← min(nums[i], prevMax*nums[i], prevMin*nums[i])
ans:    2    6    6     6
```

## 解法详解

### 解法1: 暴力枚举 — O(n²) / O(1)

**思考过程**: 最直觉的做法——枚举每个起点 `i`，从 `i` 开始向右累乘，记录过程中的最大值。

```cpp
class Solution {
public:
    int maxProduct(vector<int>& nums) {
        int n = nums.size();
        int ans = nums[0];
        for (int i = 0; i < n; i++) {
            int product = 1;
            for (int j = i; j < n; j++) {
                product *= nums[j];
                ans = max(ans, product);
            }
        }
        return ans;
    }
};
```

**关键点**: 这个解法正确但慢。注意内层循环用累乘而不是每次从头算，否则是 O(n³)。

---

### 解法2: DP（数组版）— O(n) / O(n)

**从暴力到 DP**: 暴力的瓶颈是对每个起点都要重新累乘。能否像"最大子数组和"那样，用 `dp[i]` 表示"以 `nums[i]` 结尾的最大乘积子数组"，然后 O(1) 从 `dp[i-1]` 推出？

**关键发现**: 乘法有负负得正的性质，所以仅跟踪最大值不够。我们需要两个 DP 数组：
- `dpMax[i]`：以 `nums[i]` 结尾的最大乘积
- `dpMin[i]`：以 `nums[i]` 结尾的最小乘积（可能是很大的负数）

```
// DP 填表过程:
// nums:    2     3    -2     4
// dpMax:   2     6    -2     4
// dpMin:   2     3   -12   -48
// ans:     2     6     6     6
//
// 以 index=2 (nums[i]=-2) 为例:
//   候选: nums[i]=-2, dpMax[1]*(-2)=-12, dpMin[1]*(-2)=-6
//   dpMax[2] = max(-2, -12, -6) = -2
//   dpMin[2] = min(-2, -12, -6) = -12
//
// 以 index=3 (nums[i]=4) 为例:
//   候选: nums[i]=4, dpMax[2]*4=-8, dpMin[2]*4=-48
//   dpMax[3] = max(4, -8, -48) = 4
//   dpMin[3] = min(4, -8, -48) = -48
```

```cpp
class Solution {
public:
    int maxProduct(vector<int>& nums) {
        int n = nums.size();
        vector<int> dpMax(n), dpMin(n);
        dpMax[0] = dpMin[0] = nums[0];
        int ans = nums[0];
        
        for (int i = 1; i < n; i++) {
            // 三个候选：自己单独成段、接上前面的最大、接上前面的最小
            dpMax[i] = max({nums[i], dpMax[i-1] * nums[i], dpMin[i-1] * nums[i]});
            dpMin[i] = min({nums[i], dpMax[i-1] * nums[i], dpMin[i-1] * nums[i]});
            ans = max(ans, dpMax[i]);
        }
        return ans;
    }
};
```

**关键点**: `dpMax[i]` 的三个候选缺一不可——`nums[i]` 自己对应"重新开始"，另外两个对应"接上前面"。

---

### 解法3: DP（空间优化）— O(n) / O(1) ⭐ 面试首选

**从解法2优化**: `dpMax[i]` 和 `dpMin[i]` 只依赖 `dpMax[i-1]` 和 `dpMin[i-1]`，经典的滚动变量优化。

```cpp
class Solution {
public:
    int maxProduct(vector<int>& nums) {
        int curMax = nums[0], curMin = nums[0];
        int ans = nums[0];
        
        for (int i = 1; i < nums.size(); i++) {
            // 注意：必须先保存 curMax，因为算 curMin 时还需要用到旧的 curMax
            int prevMax = curMax, prevMin = curMin;
            curMax = max({nums[i], prevMax * nums[i], prevMin * nums[i]});
            curMin = min({nums[i], prevMax * nums[i], prevMin * nums[i]});
            ans = max(ans, curMax);
        }
        return ans;
    }
};
```

**等价的"负数交换"写法**（更直观）:

```cpp
class Solution {
public:
    int maxProduct(vector<int>& nums) {
        int curMax = nums[0], curMin = nums[0];
        int ans = nums[0];
        
        for (int i = 1; i < nums.size(); i++) {
            // 核心洞察：nums[i] 为负时，最大和最小会互换
            if (nums[i] < 0) swap(curMax, curMin);
            
            curMax = max(nums[i], curMax * nums[i]);
            curMin = min(nums[i], curMin * nums[i]);
            ans = max(ans, curMax);
        }
        return ans;
    }
};
```

**为什么 swap 是对的**: 当 `nums[i] < 0` 时，`prevMax * nums[i]` 变成最小，`prevMin * nums[i]` 变成最大。先交换再按正常逻辑处理，等价于三候选的写法。

---

### 解法4: 前后缀乘积 — O(n) / O(1)

**完全不同的角度**: 不用 DP，而是从数学性质出发。

**核心观察**: 如果数组里没有 0，那么最大乘积子数组一定包含数组的某个前缀或某个后缀。

- 如果负数个数是偶数，整个数组的乘积就是最大的。
- 如果负数个数是奇数，最大乘积是"去掉最左边的负数及其左边部分"或"去掉最右边的负数及其右边部分"，这等价于从右往左的前缀积或从左往右的前缀积。
- 如果有 0，0 把数组分成多段，每段独立处理。

**做法**: 从左到右累乘一遍，从右到左累乘一遍，遇到 0 就重置为 1，全程取最大值。

```cpp
class Solution {
public:
    int maxProduct(vector<int>& nums) {
        int n = nums.size();
        int ans = nums[0];
        int prefix = 0, suffix = 0;
        
        for (int i = 0; i < n; i++) {
            // 遇到 0 后重新开始累乘（用 0 变 1 的技巧）
            prefix = (prefix == 0 ? 1 : prefix) * nums[i];
            suffix = (suffix == 0 ? 1 : suffix) * nums[n - 1 - i];
            ans = max(ans, max(prefix, suffix));
        }
        return ans;
    }
};
```

**为什么这个方法能覆盖所有情况**:
```
// 例: [2, -1, 3, -4, 5]  (2个负数，偶数个)
// prefix 从左往右: 2, -2, -6, 24, 120 → max=120（整个数组）
//
// 例: [2, 3, -2, 4]  (1个负数，奇数个)
// prefix 从左往右: 2, 6, -12, -48  → max=6  (去掉负数右边)
// suffix 从右往左: 4, -8, -24, -48 → max=4
// 总 max = 6
//
// 例: [-2, 0, -1]  (有0截断)
// prefix 从左往右: -2, 重置, -1 → max=-1... 不对
// suffix 从右往左: -1, 重置, -2 → max=-1
// 但 0 本身也参与比较: prefix 在 i=1 时 = (1)*0 = 0
// ans = max(-2, 0, -1, -1, 0, -2) = 0 ✓
```

## 解法对比

| | 暴力 | DP 数组版 | DP 空间优化 | 前后缀乘积 |
|---|---|---|---|---|
| 时间 | O(n²) | O(n) | O(n) | O(n) |
| 空间 | O(1) | O(n) | O(1) | O(1) |
| 思维难度 | 低 | 中 | 中 | 中高 |
| 面试推荐 | 说思路 | 过渡 | ⭐ 首选 | 加分 |

- **DP 空间优化**是面试最佳选择：思路清晰、代码简短、复杂度最优。
- **前后缀乘积**思路独特，适合展示数学直觉，但不如 DP 通用（面试官可能追问"为什么正确"）。
- **和 53 题的关键区别**：53 题只需维护一个 `dp`，本题需要同时维护 `max` 和 `min`，因为乘法有负负得正的性质。

## 易错点

1. **空间优化时没保存旧值**
   - ✗ `curMax = max({nums[i], curMax * nums[i], curMin * nums[i]}); curMin = min({nums[i], curMax * nums[i], curMin * nums[i]});` — 计算 `curMin` 时用了**已更新**的 `curMax`！
   - ✓ 先 `int prevMax = curMax;` 保存旧值，或者用 swap 技巧

2. **初始值设为 0 或 INT_MIN**
   - ✗ `ans = 0` → 如果所有元素都是负数，答案应该是最大的负数，不是 0
   - ✗ `ans = INT_MIN` → 可以，但更自然的是 `ans = nums[0]`
   - ✓ `ans = nums[0], curMax = nums[0], curMin = nums[0]`，从 `i=1` 开始遍历

3. **前后缀解法中 0 的处理**
   - ✗ 遇到 0 后 prefix 变成 0，后续一直是 0
   - ✓ 遇到 0 时重置为 1（但 0 本身已经通过 `prefix = ... * nums[i]` 被 ans 考虑过了）

4. **忘记 `nums[i]` 自身作为候选**
   - ✗ 只比较 `prevMax * nums[i]` 和 `prevMin * nums[i]`
   - ✓ 三个候选：`nums[i]`, `prevMax * nums[i]`, `prevMin * nums[i]`——`nums[i]` 代表"不接前面，重新开始"

## 面试追问

**Q1: 这道题和 53 题（最大子数组和）有什么区别？为什么加法只需要维护 max，乘法需要同时维护 max 和 min？**

> 加法中，如果前面的和是负数，接上它一定更差，直接丢弃。但乘法中，前面的乘积是负数，如果当前元素也是负数，接上反而更好（负负得正）。所以必须把"最小的负乘积"也记住，等待翻盘机会。

**Q2: 如果题目改成"找乘积最大的子序列（不要求连续）"，怎么做？**

> 不要求连续的话，问题变成：选哪些数使乘积最大。贪心策略：所有正数都要，负数两两配对（取绝对值最大的偶数个负数），零不选（除非所有数都是非正数）。这是一个排序+贪心问题。

**Q3: 如果要返回具体的子数组（起止下标），怎么改？**

> 在更新 `ans` 时记录下标。维护 `start` 表示当前子数组起点——当 `nums[i]` 自身比 `prevMax * nums[i]` 和 `prevMin * nums[i]` 都大时（即"重新开始"），更新 `start = i`。当 `curMax > ans` 时，记录 `bestStart = start, bestEnd = i`。

## 相关题型

- **53. 最大子数组和** — 本题的"加法版"。只需一个 `dp` 就够，因为加法没有"负负得正"的特性。学会 53 后做 152，核心改动是增加 `dpMin` 数组。
- **198. 打家劫舍** — 同样是线性 DP + 空间优化的经典题。dp[i] 只依赖 dp[i-1] 和 dp[i-2]，用滚动变量优化。
- **238. 除自身以外数组的乘积** — 也用到前缀积/后缀积的思想，和解法4的思路相通。
- **628. 三个数的最大乘积** — 也需要考虑"两个负数 × 一个正数"可能比"三个最大正数"更大，同样的"负负得正"思维。