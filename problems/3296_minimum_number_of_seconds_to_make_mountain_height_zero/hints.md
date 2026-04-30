# 3296. 移山所需的最少秒数 (Minimum Number of Seconds to Make Mountain Height Zero)

## 难度: Medium | 标签: Greedy, Array, Math, Binary Search, Heap (Priority Queue)

---

## 核心思路

多个工人同时工作降低山的高度。工人 i 降低高度 x 需要 `workerTimes[i] * (1 + 2 + ... + x) = workerTimes[i] * x*(x+1)/2` 秒（递增的边际成本）。求所有工人完成总降高度 >= mountainHeight 的最少时间。

关键洞察：**二分答案**。二分总时间 T，对于给定的 T，每个工人能独立完成的最大降高度可以用等差数列求和公式反推。如果所有工人的总降高度 >= mountainHeight，则 T 可行。

## 思维链

1. **读完题第一反应** → 工人并行工作，总时间取决于最慢的工人。这是一个"最小化最大值"的问题，经典二分答案模式。
2. **工人 i 降高度 x 的时间** → `w[i] * (1+2+...+x) = w[i] * x*(x+1)/2`。这是等差数列求和。
3. **给定时间 T，工人 i 能降多少？** → 需要 `w[i] * x*(x+1)/2 <= T`，即 `x*(x+1) <= 2*T/w[i]`。解这个二次不等式：`x = floor((-1 + sqrt(1 + 8*T/w[i])) / 2)`。
4. **二分范围** → 下界 0，上界取最快工人（最小 w）单独完成全部高度的时间 `w_min * H*(H+1)/2`。
5. **check 函数** → 对每个工人算出能降的高度 x_i，如果 sum(x_i) >= mountainHeight 则 T 可行。
6. **浮点精度** → sqrt 可能不精确，计算 x 后需要微调（检查 `(x+1)*(x+2)` 是否也满足条件，以及 `x*(x+1)` 是否真的满足）。

## 二分答案可视化

```
mountainHeight = 4, workerTimes = [2, 1, 1]

二分 T 的范围: lo=0, hi = min(w) * H*(H+1)/2 = 1 * 4*5/2 = 10

T=5: w[0]=2 → x*(x+1) <= 2*5/2=5 → x=1 (1*2=2<=5, 2*3=6>5)
     w[1]=1 → x*(x+1) <= 2*5/1=10 → x=2 (2*3=6<=10, 3*4=12>10)
     w[2]=1 → x*(x+1) <= 10 → x=2
     total = 1+2+2 = 5 >= 4 ✓ → hi=5

T=2: w[0]=2 → x*(x+1) <= 2*2/2=2 → x=1 (1*2=2<=2 ✓)
     w[1]=1 → x*(x+1) <= 4 → x=1 (1*2=2<=4, 2*3=6>4)
     w[2]=1 → x*(x+1) <= 4 → x=1
     total = 1+1+1 = 3 < 4 ✗ → lo=3

T=3: w[0]=2 → x*(x+1) <= 2*3/2=3 → x=1 (1*2=2<=3, 2*3=6>3)
     w[1]=1 → x*(x+1) <= 6 → x=2 (2*3=6<=6 ✓)
     w[2]=1 → x*(x+1) <= 6 → x=2
     total = 1+2+2 = 5 >= 4 ✓ → hi=3

T=3 是最小可行时间 ✓
验证: 工人0降1(花2s), 工人1降2(花3s), 工人2降1(花1s)
     max(2, 3, 1) = 3 ✓
```

```
mountainHeight = 10, workerTimes = [3, 2, 2, 4]

T=12: w[0]=3 → x*(x+1) <= 2*12/3=8 → x=2 (2*3=6<=8)
      w[1]=2 → x*(x+1) <= 2*12/2=12 → x=3 (3*4=12<=12)
      w[2]=2 → x*(x+1) <= 12 → x=3
      w[3]=4 → x*(x+1) <= 2*12/4=6 → x=2 (2*3=6<=6)
      total = 2+3+3+2 = 10 >= 10 ✓

T=11: w[0]=3 → x*(x+1) <= 7.33 → x=2
      w[1]=2 → x*(x+1) <= 11 → x=2 (2*3=6<=11, 3*4=12>11)
      w[2]=2 → x*(x+1) <= 11 → x=2
      w[3]=4 → x*(x+1) <= 5.5 → x=1 (1*2=2<=5.5, 2*3=6>5.5)
      total = 2+2+2+1 = 7 < 10 ✗

所以答案是 12 ✓
```

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 二分答案 | 二分时间 T，check 能否完成 | O(n * log(maxT)) | O(1) | ⭐ 面试首选 |
| 小根堆贪心 | 每次分配给当前"性价比"最高的工人 | O(H * log(n)) | O(n) | 替代方案 |

## 关键提示

1. **二分的是时间 T**，不是分配方案。给定时间后，每个工人独立工作，能做多少就做多少。
2. **等差数列公式**：工人 i 降 x 高度需要 `w[i] * x*(x+1)/2` 秒。反推 x：`x*(x+1) <= 2*T/w[i]`，解得 `x = floor((-1 + sqrt(1 + 8*T/w[i])) / 2)`。
3. **浮点修正**：sqrt 可能有精度误差。计算 x 后，用 while 循环检查 `(x+1)*(x+2) <= limit` 是否成立来上调，或 `x*(x+1) > limit` 来下调。
4. **注意溢出**：`T` 可能很大（`w_min * H*(H+1)/2`，H 可达 10^5，w 可达 10^6），需要用 `long long`。
5. **提前退出优化**：在 check 函数中，一旦 total >= mountainHeight 就可以立即返回 true，不需要遍历所有工人。
6. **二分上界**：用最小的 workerTime 和完整的 mountainHeight 计算，即最快工人单独完成全部的时间。

## 解法详解

### 解法1: 二分答案 — O(n * log(maxT)) / O(1) ⭐

**核心公式推导**:
```
工人 i 降高度 x 的时间:
  t(x) = w[i] * 1 + w[i] * 2 + ... + w[i] * x
       = w[i] * (1 + 2 + ... + x)
       = w[i] * x * (x + 1) / 2

给定时间 T，工人 i 最多能降的高度 x:
  w[i] * x * (x + 1) / 2 <= T
  x * (x + 1) <= 2 * T / w[i]
  
  设 limit = 2 * T / w[i]
  解二次不等式 x^2 + x - limit <= 0
  x <= (-1 + sqrt(1 + 4 * limit)) / 2
  x = floor((-1 + sqrt(1 + 4 * limit)) / 2)
```

```cpp
class Solution {
public:
    long long minNumberOfSeconds(int mountainHeight, vector<int>& workerTimes) {
        long long lo = 0;
        long long hi = 1LL * (*min_element(workerTimes.begin(), workerTimes.end()))
                       * mountainHeight * ((long long)mountainHeight + 1) / 2;

        auto check = [&](long long T) -> bool {
            long long total = 0;
            for (int w : workerTimes) {
                long long limit = 2 * T / w;
                long long x = (long long)((-1.0 + sqrt(1.0 + 4.0 * limit)) / 2.0);
                while ((x + 1) * (x + 2) <= limit) x++;
                while (x > 0 && x * (x + 1) > limit) x--;
                total += x;
                if (total >= mountainHeight) return true;
            }
            return false;
        };

        while (lo < hi) {
            long long mid = lo + (hi - lo) / 2;
            if (check(mid)) hi = mid;
            else lo = mid + 1;
        }
        return lo;
    }
};
```

---

### 解法2: 小根堆（贪心分配） — O(H * log n) / O(n)

**思考过程**：另一种视角——每次分配 1 单位高度给"当前完成时间最早的工人"。这等价于最优分配策略。

```
mountainHeight = 4, workerTimes = [2, 1, 1]

堆中存: (下一步完成时间, workerTime, 已完成的高度)
初始: [(2, 2, 0), (1, 1, 0), (1, 1, 0)]  // 完成时间 = w*(h+1)

Round 1: 弹出 (1, 1, 0) → 降1高度, 完成时间=1
  下一步: 该工人再降1需要 1*(1+2)=3s → push (3, 1, 1)
  堆: [(1, 1, 0), (2, 2, 0), (3, 1, 1)]

Round 2: 弹出 (1, 1, 0) → 降1高度, 完成时间=1
  下一步: push (3, 1, 1)
  堆: [(2, 2, 0), (3, 1, 1), (3, 1, 1)]

Round 3: 弹出 (2, 2, 0) → 降1高度, 完成时间=2
  下一步: push (6, 2, 1)
  堆: [(3, 1, 1), (3, 1, 1), (6, 2, 1)]

Round 4: 弹出 (3, 1, 1) → 降1高度, 完成时间=3
  总降高度=4 >= 4 ✓

答案 = 3 ✓
```

---

## 易错点

1. **整数溢出**：上界 `w_min * H * (H+1) / 2` 可达 10^6 * 10^5 * 10^5 / 2 = 5 * 10^15，必须用 `long long`。
2. **sqrt 精度**：浮点 sqrt 可能偏大或偏小，必须用 while 循环做整数修正。直接 floor(sqrt(...)) 可能差 1。
3. **二分边界**：lo=0, hi=最大可能时间。二分模板是 `while(lo < hi)`，返回 lo。
4. **check 提前退出**：total 累加过程中 >= mountainHeight 就可以返回 true，避免不必要的计算。
5. **堆解法的完成时间计算**：工人已经降了 h 高度后，再降 1 的额外时间是 `w * (h+1)`，总时间是 `w * (h+1)*(h+2)/2`。

## 面试追问递进链

1. **"为什么二分可行？"** → 时间 T 越大，每个工人能降的越多，总能力单调递增。所以可行性关于 T 是单调的，适合二分。
2. **"二分的上界怎么定？"** → 最坏情况：只有一个最快工人工作。`hi = w_min * H*(H+1)/2`。
3. **"堆解法的正确性？"** → 贪心把每单位高度分给当前"最快能完成下一步"的工人，等价于最优分配（可以用交换论证法证明）。
4. **"如何处理所有 workerTimes 相同的情况？"** → 均匀分配。每人分 H/n，余数给前几个人。

## 复杂度分析

| 解法 | 时间 | 空间 | 说明 |
|------|------|------|------|
| 二分答案 | O(n * log(maxT)) | O(1) | maxT ~ 10^16, log ~ 53 |
| 小根堆 | O(H * log n) | O(n) | H 可达 10^5 |

二分答案在 H 较大时更优（O(n * 53) vs O(H * log n)）。

## 相关题型

- [875. Koko Eating Bananas](../0875_koko_eating_bananas/) — 二分答案经典入门
- [1011. Capacity to Ship Packages](../1011_capacity_to_ship_packages_within_d_days/) — 二分答案 + check
- [410. Split Array Largest Sum](../0410_split_array_largest_sum/) — 最小化最大值
- [1482. Minimum Number of Days to Make m Bouquets](../1482_minimum_number_of_days_to_make_m_bouquets/) — 二分天数
