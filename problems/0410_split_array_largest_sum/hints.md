# 410. 分割数组的最大值

## 核心思路

这道题本质上是一个**最小化最大值**问题：把数组分成 k 段，让"最大的一段和"尽可能小。这类"最小化最大值"/"最大化最小值"问题有两条经典路线：**DP（枚举分割点）**和**二分答案（猜答案+贪心验证）**。

## 思维链

1. **读完题第一反应**：要把数组分成 k 段，暴力做法就是枚举所有可能的分割方式——在 n-1 个间隔中选 k-1 个切割点，总共 C(n-1, k-1) 种，指数级，不可行。

2. **能不能用 DP？** 分割问题天然适合 DP：定义 `dp[i][j]` = 把前 i 个元素分成 j 段时的"最大段和"的最小值。枚举最后一段的起点，用前缀和 O(1) 算区间和。时间 O(n² × k)，对于 n=1000, k=50 大约 5×10⁷，勉强能过。

3. **瓶颈在哪？** DP 的时间复杂度较高，而且状态定义和转移不太直觉。有没有更优雅的方法？

4. **关键洞察——二分答案！** 题目问"最大段和的最小值"，这个**答案本身**有一个范围：最小是 max(nums)（每个元素至少要放进一段），最大是 sum(nums)（全部放一段）。而且答案具有**单调性**：如果允许的最大段和越大，需要的段数就越少。所以可以二分这个答案！

5. **怎么验证？** 给定一个上限 mid，贪心地从左到右扫描：当前段能放就放，放不下就开新段。最后看需要多少段——如果 ≤ k 段就够了，说明 mid 可行，尝试更小；否则 mid 太小，要更大。

6. **复杂度**：二分范围是 [max(nums), sum(nums)]，区间长度最大 10⁹，二分 O(log(10⁹)) ≈ 30 次，每次验证 O(n)，总共 O(n × log(sum))，远优于 DP。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| DP + 前缀和 | 枚举分割点，状态转移 | O(n²k) | O(nk) | 能说出即可 |
| DP 空间优化 | 滚动数组降一维 | O(n²k) | O(n) | 加分项 |
| 二分答案 + 贪心 | 二分最大段和，贪心验证 | O(n log S) | O(1) | ⭐ 必须写出 |

> S = sum(nums) - max(nums)，即二分的搜索区间宽度

## 关键提示

1. **"最小化最大值"是二分答案的经典信号**：答案在一个有界范围内，且满足单调性（答案越大越容易满足），就可以二分。

2. **贪心验证的正确性**：给定上限后，从左到右尽量多塞元素到当前段（贪心），这样能用最少的段数。如果连贪心都需要超过 k 段，那其他方案只会更差。

3. **二分的左右边界**：`left = max(nums)`（至少要能装下最大的那个元素），`right = sum(nums)`（全放一段）。

4. **DP 的状态转移**：`dp[i][j] = min over all m in [j-1, i-1] of max(dp[m][j-1], sum(m+1..i))`，意思是"前 m 个分 j-1 段的最优值"和"第 j 段 [m+1..i] 的和"取 max，再对所有切割点取 min。

5. **搜索空间收缩示例**（以 nums=[7,2,5,10,8], k=2 为例）：
```
// 二分 [10, 32]:
// Round1: mid=21, 贪心分段: [7,2,5] sum=14, [10,8] sum=18 → 2段 ≤ k=2 ✓ → right=21
// Round2: mid=15, 贪心分段: [7,2,5] sum=14, [10] sum=10, [8] sum=8 → 3段 > k=2 ✗ → left=16
// Round3: mid=18, 贪心分段: [7,2,5] sum=14, [10,8] sum=18 → 2段 ≤ k=2 ✓ → right=18
// Round4: mid=17, 贪心分段: [7,2,5] sum=14, [10] sum=10, [8] sum=8 → 3段 > k=2 ✗ → left=18
// left==right==18, 返回 18 ✓
```

## 解法详解

### 解法1: DP + 前缀和 — O(n²k) / O(nk)

**思考过程**：分割问题 → 考虑"最后一段从哪里开始"→ 枚举切割点。定义 `dp[i][j]` 为把前 i 个元素分成 j 段时，各段和最大值的最小值。转移时枚举最后一段的起始位置 m+1（即前 m 个元素分成 j-1 段），用前缀和 O(1) 计算区间和。

```cpp
class Solution {
public:
    int splitArray(vector<int>& nums, int k) {
        int n = nums.size();
        // prefix[i] = nums[0] + ... + nums[i-1]
        vector<long long> prefix(n + 1, 0);
        for (int i = 0; i < n; i++) {
            prefix[i + 1] = prefix[i] + nums[i];
        }
        
        // dp[i][j] = 前i个元素分成j段的最小"最大段和"
        // 初始化为极大值
        vector<vector<long long>> dp(n + 1, vector<long long>(k + 1, LLONG_MAX));
        dp[0][0] = 0; // 0个元素分0段，最大段和为0
        
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= min(i, k); j++) {
                // 枚举最后一段 [m+1..i]，前m个元素分j-1段
                for (int m = j - 1; m < i; m++) {
                    // m >= j-1 保证前m个元素至少有j-1个（每段非空）
                    if (dp[m][j - 1] == LLONG_MAX) continue;
                    long long lastSegSum = prefix[i] - prefix[m];
                    long long candidate = max(dp[m][j - 1], lastSegSum);
                    dp[i][j] = min(dp[i][j], candidate);
                }
            }
        }
        return (int)dp[n][k];
    }
};
```

**关键点**：
- `dp[0][0] = 0` 是 base case，0个元素分0段是合法的。
- `m` 的下界是 `j-1`（前面至少有 j-1 个元素才能分 j-1 段），不是 0。
- 用 `long long` 避免求和溢出（n=1000, 每个元素最大 10⁶，总和最大 10⁹，在 int 范围内但 LLONG_MAX 作哨兵需要 long long）。

```
// DP 填表方向 (n=5, k=2):
//        j=0  j=1  j=2
// i=0     0    ∞    ∞
// i=1     ∞    7    ∞      dp[1][1] = max(dp[0][0], prefix[1]-prefix[0]) = 7
// i=2     ∞    9    ∞      dp[2][1] = max(0, 9) = 9
// i=3     ∞    14   ∞      dp[3][1] = 14
// i=4     ∞    24   14     dp[4][2] = min(max(7,17), max(9,15), max(14,10)) = min(17,15,14) = 14
// i=5     ∞    32   18     dp[5][2] = min(max(7,25), max(9,23), max(14,18), max(24,8)) 
//                          = min(25,23,18,24) = 18 ✓
```

### 解法1.5: DP 空间优化 — O(n²k) / O(n)

**从解法1优化**：`dp[i][j]` 只依赖 `dp[*][j-1]`，所以可以用滚动数组把第二维消掉。注意内层 i 要**从大到小**遍历（或者用两个一维数组交替），避免覆盖还需要的旧值。

```cpp
class Solution {
public:
    int splitArray(vector<int>& nums, int k) {
        int n = nums.size();
        vector<long long> prefix(n + 1, 0);
        for (int i = 0; i < n; i++)
            prefix[i + 1] = prefix[i] + nums[i];
        
        // prev[i] = dp[i][j-1], curr[i] = dp[i][j]
        vector<long long> prev(n + 1, LLONG_MAX), curr(n + 1, LLONG_MAX);
        prev[0] = 0;
        
        for (int j = 1; j <= k; j++) {
            fill(curr.begin(), curr.end(), LLONG_MAX);
            for (int i = j; i <= n; i++) { // i >= j 保证每段非空
                for (int m = j - 1; m < i; m++) {
                    if (prev[m] == LLONG_MAX) continue;
                    long long lastSeg = prefix[i] - prefix[m];
                    curr[i] = min(curr[i], max(prev[m], lastSeg));
                }
            }
            swap(prev, curr);
        }
        return (int)prev[n];
    }
};
```

### 解法2: 二分答案 + 贪心验证 — O(n log S) / O(1) ⭐ 面试首选

**思考过程**：
- 题目问"最大段和的最小值"→ 答案是一个数值，范围在 `[max(nums), sum(nums)]`。
- **单调性**：如果允许每段和 ≤ X，从左到右贪心分段需要 cnt 段。X 越大 → cnt 越小（越容易满足）。X 越小 → cnt 越大。
- 所以对 X 做二分：找到最小的 X 使得 cnt ≤ k。

```cpp
class Solution {
public:
    int splitArray(vector<int>& nums, int k) {
        // 二分的搜索空间：[max(nums), sum(nums)]
        long long left = *max_element(nums.begin(), nums.end());
        long long right = accumulate(nums.begin(), nums.end(), 0LL);
        
        while (left < right) {
            long long mid = left + (right - left) / 2;
            // 验证：最大段和不超过 mid 时，最少需要几段？
            if (canSplit(nums, k, mid)) {
                right = mid;    // mid 可行，尝试更小
            } else {
                left = mid + 1; // mid 不可行，需要更大
            }
        }
        return (int)left;
    }
    
private:
    // 贪心验证：每段和 ≤ maxSum 时，能否分成 ≤ k 段？
    bool canSplit(vector<int>& nums, int k, long long maxSum) {
        int segments = 1;       // 至少1段
        long long curSum = 0;
        for (int num : nums) {
            if (curSum + num > maxSum) {
                // 当前段放不下了，开新段
                segments++;
                curSum = num;   // num 成为新段的第一个元素
                if (segments > k) return false; // 提前终止
            } else {
                curSum += num;
            }
        }
        return true; // segments <= k
    }
};
```

**为什么贪心验证是正确的？**
给定上限 maxSum，我们从左到右尽可能多地把元素塞进当前段（只要不超过 maxSum 就继续塞），超了就开新段。这样做能**最小化段数**——因为如果提前切割，后面的段只会更多不会更少。所以如果连这种"最省段数"的方式都需要超过 k 段，那 maxSum 一定不够大。

**关键点**：
- `left = max(nums)` 不是 0，因为每段至少包含一个元素，最大元素必须被某段容纳。
- 二分用 `left < right` 模板，收缩到 left == right 时就是答案。
- 验证函数初始 `segments = 1`（不是 0），因为第一段是自动开始的。

## 解法对比

| | DP | 二分答案 + 贪心 |
|---|---|---|
| 时间 | O(n²k) ≈ 5×10⁷ | O(n log S) ≈ 3×10⁴ |
| 空间 | O(nk) 或 O(n) | O(1) |
| 思路 | 枚举切割点，经典但较重 | 转化问题，优雅且高效 |
| 面试推荐 | 作为思考出发点 | **首选**，代码简洁清晰 |
| 适用场景 | 需要知道具体分割方案时 | 只需要最优值时 |

**DP 的优势**：如果面试官追问"能不能输出最优分割方案"，DP 可以通过回溯 dp 表得到具体切割点，而二分解法只给出最优值。

## 易错点

1. **二分左边界设为 0 而非 max(nums)**
   - ✗ `left = 0` → 如果 maxSum < max(nums)，单个元素都放不下，验证函数逻辑出错
   - ✓ `left = *max_element(nums.begin(), nums.end())` → 保证每个元素都能被容纳

2. **验证函数 segments 初始化为 0**
   - ✗ `int segments = 0;` → 最后一段没被计数，结果偏小
   - ✓ `int segments = 1;` → 第一段自动开始，遇到放不下的才 segments++

3. **DP 中 m 的下界写错**
   - ✗ `for (int m = 0; m < i; m++)` → 可能导致前 m 个元素不够分 j-1 段
   - ✓ `for (int m = j - 1; m < i; m++)` → 保证每段至少有一个元素

4. **整数溢出**
   - ✗ 用 int 做 sum → n=1000, nums[i]=10⁶ 时 sum=10⁹，接近 INT_MAX
   - ✓ 用 `long long` 或 `0LL` 做累加

5. **二分时 mid 的计算**
   - ✗ `mid = (left + right) / 2` → left + right 可能溢出
   - ✓ `mid = left + (right - left) / 2`

## 面试追问

**Q1: 为什么这道题可以二分？二分的前提条件是什么？**
> 答：二分答案需要两个条件：(1) 答案有明确的有界范围 [max, sum]；(2) 满足单调性——允许的最大段和越大，需要的分段数越少（或不变）。这保证了存在一个临界点，使得 ≥ 临界点都可行、< 临界点都不可行。

**Q2: 如果要求输出具体的分割方案（每段包含哪些元素），怎么做？**
> 答：二分解法找到最优答案 ans 后，再贪心扫一遍，每当当前段和超过 ans 就切割，记录切割位置即可。或者用 DP 解法，在填表过程中记录每个 dp[i][j] 是从哪个 m 转移来的，最后回溯路径。

**Q3: 如果 k 非常大（接近 n），或者 n 非常大（10⁶+），哪种解法更好？**
> 答：二分解法始终是 O(n log S)，不受 k 的影响。而 DP 是 O(n²k)，k 大时更慢。当 n 很大时，二分解法的常数也更小（只需一次线性扫描验证）。所以二分在各种场景下都更优。

**Q4: 这道题和「1011. 在 D 天内送达包裹的能力」有什么关系？**
> 答：完全相同的模型！1011 把 nums 看作包裹重量，k 看作天数，求最小的船运力。框架一模一样：二分运力 → 贪心装包裹 → 验证天数是否 ≤ D。

## 相关题型

- **1011. 在 D 天内送达包裹的能力** — 和本题几乎完全相同的二分+贪心框架，区别只在语境（包裹重量 vs 数组元素），代码验证函数可以直接复用。
- **875. 爱吃香蕉的珂珂** — 同样是"二分答案 + 验证"模型，二分吃香蕉速度，验证能否在 h 小时内吃完。
- **1482. 制作 m 束花所需的最少天数** — 二分答案（天数）+ 贪心验证（能否摘到 m 束花）。
- **LeetCode 813. 最大平均值和的分组** — 类似的 DP 分割问题，dp[i][j] 定义几乎相同，可以复用"枚举最后一段起始位置"的转移框架。