# 3584. 子序列首尾元素的最大乘积 (Maximum Product of First and Last Elements of a Subsequence)

## 难度: Medium | 标签: Array, Two Pointers, Sorting

---

## 题目理解

给定整数数组 `nums` 和整数 `m`，从 `nums` 中选一个长度为 `m` 的**子序列**（保持相对顺序），求子序列**首元素 x 尾元素**的最大值。

关键点：
- 子序列（非子数组）：可以跳过元素，但要保持相对顺序
- 首尾乘积：只关心子序列的第一个和最后一个元素
- m = 1 时首尾是同一个元素，乘积 = 元素^2

---

## 思维链

```
1. 读完题 --> 选长度为 m 的子序列，最大化 first * last
2. 子序列的"首"和"尾"是什么？
   - 首元素: 子序列中第一个元素，即原数组中最靠左的被选元素
   - 尾元素: 子序列中最后一个元素，即原数组中最靠右的被选元素
   - 中间 m-2 个元素可以是任意的（只要位置在首和尾之间）

3. 关键约束:
   - 设首元素在原数组位置 i，尾元素在位置 j
   - 需要 j >= i + m - 1（中间至少要能放下 m-2 个元素）
   - 即 j - i >= m - 1

4. 首元素能选哪些位置？ --> i in [0, n-m]
   因为后面至少还要有 m-1 个位置给剩下的元素

5. 尾元素能选哪些位置？ --> j in [m-1, n-1]
   因为前面至少要有 m-1 个位置给首元素和中间元素

6. 对于固定的首元素 nums[i]:
   尾元素 j 的范围是 [i+m-1, n-1]
   为了最大化乘积，需要找这个范围内使得 nums[i]*nums[j] 最大的 j
   - 若 nums[i] > 0: 找 max(nums[j])
   - 若 nums[i] < 0: 找 min(nums[j])
   - 若 nums[i] = 0: 乘积 = 0

7. 用后缀最大值和后缀最小值数组预处理！

8. m == 1 特殊情况:
   首尾同一元素，乘积 = nums[i]^2
   答案 = max(nums[i]^2) = 绝对值最大的元素的平方
```

---

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 特点 |
|------|------|------|------|------|
| **后缀极值 + 枚举首元素** | 预处理后缀 max/min，枚举首元素 | O(n) | O(n) | 推荐，最清晰 |
| 排序 + 分类讨论 | 排序后只看四种极值组合 | O(n log n) | O(1) | 更难理解但思路精炼 |

---

## 解法一: 后缀极值 + 枚举首元素 -- O(n) / O(n) 【推荐】

### 核心思路

```
1. 预处理两个后缀数组:
   suffMax[j] = max(nums[j], nums[j+1], ..., nums[n-1])
   suffMin[j] = min(nums[j], nums[j+1], ..., nums[n-1])

2. 对于 m == 1:
   答案 = max(nums[i]^2) for all i

3. 对于 m >= 2:
   枚举首元素位置 i in [0, n-m]:
     尾元素范围起点 j = i + m - 1
     候选乘积1 = nums[i] * suffMax[j]
     候选乘积2 = nums[i] * suffMin[j]
     ans = max(ans, 候选乘积1, 候选乘积2)

   为什么要看 suffMax 和 suffMin 两个？
   - nums[i] > 0 时，乘最大的尾元素更好
   - nums[i] < 0 时，乘最小的（可能为负）尾元素更好
   - 两者取 max 就覆盖了所有情况
```

### 图解

```
nums = [2, -1, 2, -6, 5, 2, -5, 7]   m = 2   n = 8

suffMax: 从右往左取最大值
  index:  0  1  2  3  4  5  6  7
  nums:   2 -1  2 -6  5  2 -5  7
  suffMax: 7  7  7  7  7  7  7  7

suffMin: 从右往左取最小值
  suffMin:-6 -6 -6 -6 -5 -5 -5  7

枚举首元素 i in [0, 6]:  (n-m = 6)

i=0: nums[0]=2,  j起=1
     2 * suffMax[1]=7 = 14
     2 * suffMin[1]=-6 = -12
     候选 14

i=1: nums[1]=-1, j起=2
     -1 * suffMax[2]=7 = -7
     -1 * suffMin[2]=-6 = 6
     候选 6

i=2: nums[2]=2,  j起=3
     2 * 7 = 14, 2 * -6 = -12
     候选 14

i=3: nums[3]=-6, j起=4
     -6 * 7 = -42, -6 * -5 = 30
     候选 30

i=4: nums[4]=5,  j起=5
     5 * 7 = 35, 5 * -5 = -25
     候选 35  <-- 最大!

i=5: nums[5]=2,  j起=6
     2 * 7 = 14, 2 * -5 = -10
     候选 14

i=6: nums[6]=-5, j起=7
     -5 * 7 = -35, -5 * 7 = -35
     候选 -35

答案 = 35 (子序列 [5, 7]) ✓
```

### 代码

```cpp
class Solution {
public:
    long long maximumProduct(vector<int>& nums, int m) {
        int n = nums.size();

        // m == 1: 首尾同一元素，答案 = max(nums[i]^2)
        if (m == 1) {
            long long ans = LLONG_MIN;
            for (int x : nums)
                ans = max(ans, (long long)x * x);
            return ans;
        }

        // 预处理后缀最大值和最小值
        vector<int> suffMax(n), suffMin(n);
        suffMax[n - 1] = suffMin[n - 1] = nums[n - 1];
        for (int j = n - 2; j >= 0; j--) {
            suffMax[j] = max(nums[j], suffMax[j + 1]);
            suffMin[j] = min(nums[j], suffMin[j + 1]);
        }

        // 枚举首元素
        long long ans = LLONG_MIN;
        for (int i = 0; i <= n - m; i++) {
            int j = i + m - 1; // 尾元素最早位置
            long long prod1 = (long long)nums[i] * suffMax[j];
            long long prod2 = (long long)nums[i] * suffMin[j];
            ans = max({ans, prod1, prod2});
        }
        return ans;
    }
};
```

---

## 解法二: 排序 + 分类讨论 -- O(n log n) / O(n)

### 核心思路

排序后，子序列的首元素只能从 `sorted[0..n-m]`（排序后前 n-m+1 个）中选，尾元素只能从 `sorted[m-1..n-1]` 中选。但排序后索引约束自动满足。

最大乘积只可能来自四种极值组合：
- 最小首 * 最小尾（两负得正）
- 最小首 * 最大尾
- 最大首 * 最小尾
- 最大首 * 最大尾（两正得正）

### 代码

```cpp
class Solution {
public:
    long long maximumProduct(vector<int>& nums, int m) {
        int n = nums.size();
        sort(nums.begin(), nums.end());

        if (m == 1) {
            long long ans = LLONG_MIN;
            for (int x : nums)
                ans = max(ans, (long long)x * x);
            return ans;
        }

        long long ans = LLONG_MIN;
        // 四种极值组合
        ans = max(ans, (long long)nums[0] * nums[m - 1]);
        ans = max(ans, (long long)nums[0] * nums[n - 1]);
        ans = max(ans, (long long)nums[n - m] * nums[m - 1]);
        ans = max(ans, (long long)nums[n - m] * nums[n - 1]);
        return ans;
    }
};
```

---

## 关键提示序列（渐进式）

| # | 提示 | 思路方向 |
|---|------|----------|
| 1 | 子序列中间 m-2 个元素对答案有影响吗？ | 没有，只看首和尾 |
| 2 | 首元素位置 i 的范围是什么？ | [0, n-m] |
| 3 | 尾元素对首元素的约束是什么？ | j >= i + m - 1 |
| 4 | 对于固定的首元素，如何快速找最优尾元素？ | 后缀 max/min |
| 5 | nums[i] 为负时，乘哪个尾元素最大？ | 最小的（负负得正） |
| 6 | m == 1 的特殊性在哪里？ | 首尾同一元素，乘积 = x^2 |

---

## 易错点

1. **m == 1 时首尾是同一个元素**
   - 乘积 = nums[i]^2，不是 nums[i] * nums[j] (i != j)
   - 绝对值最大的元素的平方就是答案

2. **long long 溢出**
   - nums[i] 可达 10^5，乘积可达 10^10
   - 必须用 long long

3. **正负分类讨论**
   - 首元素为正 -> 尾元素越大越好
   - 首元素为负 -> 尾元素越小越好（负负得正）
   - 首元素为零 -> 乘积为零
   - 需要同时考虑后缀 max 和后缀 min

4. **排序解法中的索引**
   - 排序后首元素候选: `nums[0]` 和 `nums[n-m]`
   - 排序后尾元素候选: `nums[m-1]` 和 `nums[n-1]`
   - 四种组合不多不少

5. **中间元素的存在性**
   - 首元素在 i，尾元素在 j，要求 j - i >= m - 1
   - 且 [i+1, j-1] 中至少有 m-2 个元素可选
   - 后缀极值方法自动满足这个约束

---

## 复杂度分析

### 解法一（后缀极值）
- **时间: O(n)** -- 两遍扫描（后缀预处理 + 枚举首元素）
- **空间: O(n)** -- 后缀 max/min 数组

### 解法二（排序）
- **时间: O(n log n)** -- 排序主导
- **空间: O(1)** -- 原地排序（忽略排序内部栈空间）

---

## 相关题目

| 题号 | 题目 | 关联 |
|------|------|------|
| 152 | Maximum Product Subarray | 正负分类讨论的乘积优化 |
| 628 | Maximum Product of Three Numbers | 排序 + 极值组合 |
| 1014 | Best Sightseeing Pair | 枚举右端点 + 维护左端最优 |
| 2909 | Minimum Sum of Mountain Triplets II | 前后缀极值 |

---

## 面试追问

1. **如果要求子序列首尾乘积最小（而非最大），怎么改？**
   - 正数首元素想要小尾(suffMin)，负数首元素想要大尾(suffMax)
   - 仍然两个都检查，取 min 而非 max
   - 框架完全一样，只改 `max` -> `min`

2. **如果不限制子序列长度（m 可以是 1 到 n 的任意值），最大乘积？**
   - m=1: `max(x^2)` for all x
   - m>=2: 从数组中选两个元素使乘积最大
   - 排序后只需比较 `nums[0]*nums[1]` 和 `nums[n-1]*nums[n-2]`

3. **如果改成三个元素的乘积（首、中、尾），怎么优化？**
   - 固定首尾后，中间元素可以是区间 `[i+1, j-1]` 内任意值
   - 需要区间极值查询 -> 用 Sparse Table 预处理 O(n log n)，查询 O(1)
   - 正负讨论变成 8 种组合，逐一检查取 max

---

## 面试技巧

1. **先确认理解**：子序列 vs 子数组，首尾乘积 vs 所有元素乘积
2. **m == 1 先搞定**：特殊情况容易漏，先处理展示细心
3. **正负讨论**：面试官希望看到你主动考虑正负号对乘积的影响
4. **后缀极值是通用模式**：很多"枚举一端 + 快速查另一端最优"的题都用这个技巧
