# 3381. 长度可被 K 整除的子数组的最大元素和

## 核心思路

给定数组 `nums` 和正整数 `k`，找出长度能被 `k` 整除的子数组中，元素和的最大值。

本质上是**前缀和 + 同余分组**的问题：子数组 `nums[i+1..j]` 的长度为 `j - i`，要求 `(j - i) % k == 0`，等价于 `j % k == i % k`。所以问题变成：对于每个前缀和 `prefix[j]`，在所有满足 `i % k == j % k` 且 `i < j` 的前缀和 `prefix[i]` 中，找最小的那个，使得 `prefix[j] - prefix[i]` 最大。

## 思维链

1. **读完题第一反应** → 枚举所有长度为 k, 2k, 3k, ... 的子数组，对每个子数组求和取最大。暴力 O(n^2) 或 O(n^2/k)。
2. **怎么快速求子数组和？** → 前缀和。`sum(l..r) = prefix[r+1] - prefix[l]`。
3. **长度被 k 整除意味着什么？** → 子数组 `[l..r]` 长度为 `r - l + 1`。用前缀和视角：`prefix[r+1] - prefix[l]`，长度 = `(r+1) - l`。令 `j = r+1, i = l`，则长度 = `j - i`，要求 `(j - i) % k == 0`，即 `j % k == i % k`。
4. **等价问题** → 对所有 `(i, j)` 对，`i < j`，`j % k == i % k`，最大化 `prefix[j] - prefix[i]`。即：对每个 `j`，在同余类 `j % k` 中找 `i < j` 使得 `prefix[i]` 最小。
5. **怎么高效维护？** → 用大小为 k 的数组 `minPrefix[r]`，记录余数为 `r` 的位置中已经遇到的最小前缀和。遍历 `j` 时，先用 `minPrefix[j % k]` 更新答案，再更新 `minPrefix[j % k]`。
6. **初始化** → `prefix[0] = 0`，索引 `0` 对应余数 `0 % k = 0`，所以 `minPrefix[0] = 0`，其余初始化为 `LLONG_MAX`（表示还没有对应的前缀和）。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力枚举 | 枚举所有 k 倍长度子数组 | O(n^2/k) | O(1) | 能说出即可 |
| 前缀和+同余分组 | 按 index%k 分组追踪最小前缀和 | O(n) | O(k) | 必须掌握 |

## 关键提示

1. **前缀和转换**：子数组和 = `prefix[j] - prefix[i]`。要最大化这个差值，对于固定的 `j`，需要最小化 `prefix[i]`。
2. **同余条件**：`(j - i) % k == 0` 等价于 `j % k == i % k`。这把所有前缀和按余数分成了 `k` 组，每组内部互相匹配。
3. **初始化关键**：`prefix[0] = 0` 的索引是 `0`，余数是 `0 % k = 0`。所以 `minPrefix[0] = 0`。其余余数类还没有对应的前缀和，用 `LLONG_MAX` 表示"不可用"。
4. **先查后更新**：遍历到 `j` 时，先用 `minPrefix[j%k]` 计算候选答案，再更新 `minPrefix[j%k]`。这保证了 `i < j`（子数组非空）。
5. **答案初始化为 `LLONG_MIN`**：因为数组元素可以全为负数，子数组和也可能为负。

## 解法详解

### 解法1: 暴力枚举 -- O(n^2/k) / O(n)

**思考过程**: 直接枚举所有长度为 k 的倍数的子数组。

```
示例: nums = [1,2], k = 1
所有子数组长度都能被 1 整除，找最大子数组和 → max(1, 2, 3) = 3

示例: nums = [-1,-2,-3,-4,-5], k = 4
长度为 4 的子数组: [-1,-2,-3,-4] 和=−10, [-2,-3,-4,-5] 和=−14
→ 最大 = −10

示例: nums = [-1,3,2], k = 3
长度为 3 的子数组: [-1,3,2] 和=4
→ 答案 = 4
```

```cpp
class Solution {
public:
    long long maxSubarraySum(vector<int>& nums, int k) {
        int n = nums.size();
        // 计算前缀和
        vector<long long> prefix(n + 1, 0);
        for (int i = 0; i < n; i++) {
            prefix[i + 1] = prefix[i] + nums[i];
        }
        long long ans = LLONG_MIN;
        // 枚举所有 (i, j) 使得 (j - i) % k == 0
        for (int i = 0; i < n; i++) {
            for (int j = i + k; j <= n; j += k) {
                ans = max(ans, prefix[j] - prefix[i]);
            }
        }
        return ans;
    }
};
```

**关键点**:
- 时间复杂度 O(n^2/k)：外层 O(n)，内层步长 k 所以 O(n/k)。
- n 最大 2*10^5，k 如果很小（比如 k=1），退化为 O(n^2)，超时。
- 这个解法的价值：**明确了瓶颈在于"对每个 j，要在所有同余的 i 中找最小前缀和"**。

---

### 解法2: 前缀和 + 同余分组 -- O(n) / O(k) -- 最优解

**从解法1优化**: 对于固定的余数 `r = j % k`，我们遍历 `j` 时只关心同一余数类中 `prefix[i]` 的最小值。维护一个大小为 k 的数组即可。

```
推导过程:

子数组 nums[l..r] 的和 = prefix[r+1] - prefix[l]
长度 = r - l + 1 = (r+1) - l

令 j = r+1, i = l:
  sum = prefix[j] - prefix[i]
  len = j - i
  
要求 len % k == 0 → (j - i) % k == 0 → j % k == i % k

所以问题变成:
  对每个 j (从1到n), 找 i < j 且 i%k == j%k, 
  使得 prefix[j] - prefix[i] 最大
  即 prefix[i] 最小

用 minPrefix[r] 维护余数为 r 的位置中的最小前缀和:
```

```
示例: nums = [1,2], k = 1

prefix:  [0, 1, 3]
索引:     0  1  2
余数(mod 1): 0  0  0    (所有索引模1都是0)

初始: minPrefix[0] = 0 (来自 prefix[0])

j=1 (prefix=1): r=1%1=0, ans = max(-INF, 1-0) = 1, 
                minPrefix[0] = min(0, 1) = 0
j=2 (prefix=3): r=2%1=0, ans = max(1, 3-0) = 3, 
                minPrefix[0] = min(0, 3) = 0

答案: 3 ✓
```

```
示例: nums = [-1,-2,-3,-4,-5], k = 4

prefix:  [0, -1, -3, -6, -10, -15]
索引:     0   1   2   3    4    5
余数%4:   0   1   2   3    0    1

初始: minPrefix = [0, INF, INF, INF]

j=1 (prefix=-1): r=1, minPrefix[1]=INF → 跳过
                  更新 minPrefix[1] = -1
j=2 (prefix=-3): r=2, minPrefix[2]=INF → 跳过
                  更新 minPrefix[2] = -3
j=3 (prefix=-6): r=3, minPrefix[3]=INF → 跳过
                  更新 minPrefix[3] = -6
j=4 (prefix=-10):r=0, minPrefix[0]=0 → ans = -10-0 = -10
                  更新 minPrefix[0] = min(0, -10) = -10
j=5 (prefix=-15):r=1, minPrefix[1]=-1 → ans = max(-10, -15-(-1)) = max(-10,-14) = -10
                  更新 minPrefix[1] = min(-1, -15) = -15

答案: -10 ✓ (子数组 [-1,-2,-3,-4], 长度4, 和=-10)
```

```cpp
class Solution {
public:
    long long maxSubarraySum(vector<int>& nums, int k) {
        int n = nums.size();
        // minPrefix[r] = 余数为 r 的前缀和索引中，已见过的最小前缀和值
        vector<long long> minPrefix(k, LLONG_MAX);
        minPrefix[0] = 0;  // prefix[0] = 0, 索引 0 % k = 0

        long long prefix = 0;
        long long ans = LLONG_MIN;

        for (int j = 0; j < n; j++) {
            prefix += nums[j];
            // prefix[j+1] 对应索引 j+1, 余数为 (j+1) % k
            int r = (j + 1) % k;

            // 先查: 看同余类中是否有可用的前缀和
            if (minPrefix[r] != LLONG_MAX) {
                ans = max(ans, prefix - minPrefix[r]);
            }

            // 后更新: 将当前前缀和纳入同余类的最小值追踪
            minPrefix[r] = min(minPrefix[r], prefix);
        }

        return ans;
    }
};
```

**关键点**:
- **先查后更新的顺序**：保证 `i < j`。如果先更新后查，可能用 `prefix[j]` 自己匹配自己，得到子数组长度为 0。
- **余数计算用 `(j+1) % k`**：因为 `prefix[j+1]` 对应的前缀和索引是 `j+1`。这里的 `prefix` 变量实际上等于 `prefix[j+1]`（累加了 `nums[0]` 到 `nums[j]`）。
- **LLONG_MAX 哨兵**：表示该余数类还没有可用的前缀和。检查时跳过。

## 解法对比

|  | 暴力枚举 | 前缀和+同余分组 |
|--|---------|----------------|
| 时间 | O(n^2/k) | **O(n)** |
| 空间 | O(n) | **O(k)** |
| 适用规模 | n 小或 k 大 | **通用** |
| 核心技巧 | 无 | 前缀和 + 模运算分组 |

## 易错点

1. **余数计算错误**：
   - 错: `int r = j % k;` → 这是 `prefix[j]` 的索引余数，但代码中 `prefix` 实际对应 `prefix[j+1]`
   - 对: `int r = (j + 1) % k;` → 匹配当前累加到 `j+1` 位的前缀和

2. **初始化错误**：
   - 错: 所有 `minPrefix` 初始为 `LLONG_MAX` → 忘记 `prefix[0] = 0` 在余数 0 组
   - 对: `minPrefix[0] = 0`，其余 `LLONG_MAX`

3. **先更新后查导致空子数组**：
   - 错: `minPrefix[r] = min(minPrefix[r], prefix); ans = max(ans, prefix - minPrefix[r]);`
     → 当 `minPrefix[r]` 被更新为 `prefix` 后，`prefix - minPrefix[r] = 0`，这意味着子数组长度为 0
   - 对: 先查再更新

4. **答案初始化为 0**：
   - 错: `long long ans = 0;` → 如果所有合法子数组和都为负数，答案会错误地返回 0
   - 对: `long long ans = LLONG_MIN;`

5. **忽略 long long**：
   - 错: 用 `int` 存前缀和 → n 最大 2*10^5，元素最大 10^9，前缀和可达 2*10^14，溢出
   - 对: 全程用 `long long`

## 面试追问

**Q1: 为什么同余条件成立？**
> `(j - i) % k == 0` 意味着 `j ≡ i (mod k)`。用数学语言，`j` 和 `i` 在模 `k` 意义下同余。这是模运算的基本性质：`a - b` 能被 `k` 整除 当且仅当 `a % k == b % k`。

**Q2: 如果要返回子数组的起止索引呢？**
> 在 `minPrefix` 中不仅存最小值，还存对应的索引。当 `ans` 被更新时，记录 `(minIdx, j)` 作为子数组的范围。

**Q3: 如果改成"长度恰好为 k"呢？**
> 这更简单——滑动窗口。维护一个大小为 k 的窗口，每次滑动一格，用前缀和 O(1) 计算窗口和。时间 O(n)，空间 O(1)。

**Q4: 这个"前缀和+同余"的技巧还在哪些题里出现？**
> - **523. 连续的子数组和**：子数组和是 k 的倍数 → 前缀和模 k，找同余的两个前缀和。
> - **974. 和可被 K 整除的子数组**：计数版本，统计同余前缀和对的数量。
> - **560. 和为 K 的子数组**：前缀和差 = k，用哈希表计数（不需要模运算，但同一思路）。

## 相关题型

- **523. 连续的子数组和** -- 判断是否存在长度 >= 2 的子数组，其和为 k 的倍数。同样用前缀和模 k，用 HashMap 记录每个余数第一次出现的位置。
- **974. 和可被 K 整除的子数组** -- 统计和能被 k 整除的子数组个数。前缀和模 k，对每个余数计数，同余的前缀和两两配对。
- **560. 和为 K 的子数组** -- 前缀和 + HashMap 找 prefix[j] - k 是否存在。没有模运算但核心的"前缀和差值"思想完全相同。
- **862. 和至少为 K 的最短子数组** -- 前缀和 + 单调队列，要求 prefix[j] - prefix[i] >= k 且 j-i 最小。不同的约束但同样的前缀和框架。
