# 1011. 在 D 天内送达包裹的能力

## 核心思路

这道题本质上是：**在一个连续的答案空间（船的运载能力）上，找到满足条件（D 天内送完）的最小值**——经典的「二分答案」问题。

## 思维链

1. **读完题第一反应**：题目要我们找"最低运载能力"。运载能力越大，需要的天数越少；运载能力越小，需要的天数越多。这种**单调关系**是关键观察。

2. **暴力解怎么做？** 运载能力的范围是 `[max(weights), sum(weights)]`。我们可以从最小值开始逐个尝试：对每个运载能力，贪心模拟装船过程，看需要多少天。第一个满足 `≤ days` 天的就是答案。

3. **暴力的瓶颈在哪？** 答案空间可能很大（最多 `5*10^4 * 500 = 2.5*10^7`），逐个尝试太慢。但关键是：**运载能力越大，天数越少**——答案空间具有单调性！

4. **怎么突破？** 单调性 → **二分查找**！在答案空间 `[max(weights), sum(weights)]` 上二分，每次用 `O(n)` 验证 mid 是否可行，总复杂度 `O(n * log(sum - max))`。

5. **验证函数怎么写？** 给定运载能力 `cap`，贪心地从左到右装：当前这一天能装就装，装不下就开新的一天。统计总天数，判断是否 `≤ days`。

6. **边界怎么确定？** 下界 = `max(weights)`（至少得装下最重的包裹），上界 = `sum(weights)`（一天全装完）。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 线性枚举 | 从小到大逐个试运载能力 | O(n × S) | O(1) | 能说出即可 |
| 二分答案 | 在答案空间上二分 + 贪心验证 | O(n × log S) | O(1) | ⭐ 必须写出 |

> S = sum(weights) - max(weights)，即答案空间大小

## 关键提示

1. **这不是在数组上二分，而是在"答案"上二分**。搜索空间是运载能力的取值范围，不是 weights 数组的下标。

2. **单调性证明**：运载能力 `cap` 增大 → 每天能装更多 → 需要天数减少（或不变）。所以存在一个临界点，左边不行，右边都行——这就是二分的前提。

3. **验证函数是贪心的**：因为包裹顺序固定，不需要考虑排列组合，直接从左到右扫一遍即可。

4. **下界不是 1，是 max(weights)**：如果运载能力比最重的包裹还小，那个包裹永远装不上船。

5. **二分找的是"左边界"**：满足条件的最小 cap。当 `canShip(mid)` 为 true 时，mid 可能是答案，但还要往左找更小的。

## 解法详解

### 解法1: 线性枚举 — O(n × S) / O(1)

**思考过程**：最直觉的方法——从最小可能的运载能力（最重包裹的重量）开始，逐个递增，每次检查是否能在 days 天内送完，第一个满足的就是答案。

```cpp
class Solution {
public:
    int shipWithinDays(vector<int>& weights, int days) {
        int maxW = *max_element(weights.begin(), weights.end());
        int sumW = accumulate(weights.begin(), weights.end(), 0);
        
        // 从最小可能的运载能力开始逐个尝试
        for (int cap = maxW; cap <= sumW; cap++) {
            if (canShip(weights, days, cap)) {
                return cap;
            }
        }
        return sumW; // 最坏情况：一天全送
    }
    
private:
    // 贪心验证：运载能力为 cap 时，需要多少天？
    bool canShip(vector<int>& weights, int days, int cap) {
        int needDays = 1;   // 至少需要1天
        int curLoad = 0;    // 当前这一天已装的重量
        
        for (int w : weights) {
            if (curLoad + w > cap) {
                // 当前这一天装不下了，开新的一天
                needDays++;
                curLoad = 0;
            }
            curLoad += w;
        }
        return needDays <= days;
    }
};
```

**关键点**：这个解法能通过小数据，但答案空间最大 `2.5 × 10^7`，每次验证 `O(n)`，总共可能 `O(n × S) ≈ 10^12`，会超时。

### 解法2: 二分答案 — O(n × log S) / O(1) ⭐ 面试首选

**从解法1优化**：解法1的瓶颈是逐个枚举答案。由于"运载能力↑ → 需要天数↓"的单调性，我们可以用二分查找在 `O(log S)` 次内定位答案，每次验证 `O(n)`。

```
// 搜索空间收缩过程 (示例1: weights=[1..10], days=5):
//
// 答案空间: [10, 55]    (max=10, sum=55)
//
// Round1: left=10, right=55, mid=32 → needDays=2 ≤ 5 ✓ → right=32
//         [10 ........................ 32]
//                                     ↑ mid可行,往左找更小的
//
// Round2: left=10, right=32, mid=21 → needDays=3 ≤ 5 ✓ → right=21
//         [10 ............ 21]
//                          ↑
//
// Round3: left=10, right=21, mid=15 → needDays=5 ≤ 5 ✓ → right=15
//         [10 .... 15]
//                  ↑
//
// Round4: left=10, right=15, mid=12 → needDays=7 > 5  ✗ → left=13
//         [10 . 12]  mid不行,往右
//              ↑
//
// Round5: left=13, right=15, mid=14 → needDays=6 > 5  ✗ → left=15
//         [13 14]
//            ↑
//
// Round6: left=15, right=15 → left==right, 返回 15 ✓
```

```cpp
class Solution {
public:
    int shipWithinDays(vector<int>& weights, int days) {
        // 二分的下界：必须能装下最重的包裹
        int left = *max_element(weights.begin(), weights.end());
        // 二分的上界：一天装完所有包裹
        int right = accumulate(weights.begin(), weights.end(), 0);
        
        // 二分找满足条件的最小 cap（左边界）
        while (left < right) {
            int mid = left + (right - left) / 2;
            
            if (canShip(weights, days, mid)) {
                // mid 可行，但可能不是最小的，继续往左找
                right = mid;
            } else {
                // mid 不可行（天数超了），运载能力要更大
                left = mid + 1;
            }
        }
        // 循环结束时 left == right，就是最小可行运载能力
        return left;
    }
    
private:
    bool canShip(vector<int>& weights, int days, int cap) {
        int needDays = 1;
        int curLoad = 0;
        
        for (int w : weights) {
            if (curLoad + w > cap) {
                needDays++;
                curLoad = 0;
            }
            curLoad += w;
        }
        return needDays <= days;
    }
};
```

**关键点**：
- 使用 `left < right` 模板（不是 `left <= right`），因为我们找的是左边界
- `right = mid`（不是 `mid - 1`），因为 mid 可能就是答案
- `left = mid + 1`，因为 mid 已经验证不可行，可以排除
- 循环结束时 `left == right`，无需额外判断

## 解法对比

| | 线性枚举 | 二分答案 |
|---|---|---|
| 时间 | O(n × S)，可达 10^12 | O(n × log S)，约 10^4 × 25 ≈ 10^6 |
| 空间 | O(1) | O(1) |
| 核心差异 | 逐个试答案 | 利用单调性跳着试 |
| 验证函数 | 完全相同 | 完全相同 |
| 面试推荐 | 作为思路起点 | **必须掌握** |

两种解法的验证函数 `canShip` 完全相同，区别仅在于如何遍历答案空间。

## 易错点

1. **下界写成 1 或 0**
   - ✗ `int left = 1;` → 运载能力小于最重包裹时，该包裹永远装不上，`canShip` 会死循环或返回错误结果
   - ✓ `int left = *max_element(weights.begin(), weights.end());`

2. **验证函数中 needDays 初始化为 0**
   - ✗ `int needDays = 0;` → 最后一批货的那一天没被计入
   - ✓ `int needDays = 1;` → 至少需要1天，每次"装不下开新天"时 `needDays++`

3. **二分时 right = mid - 1**
   - ✗ `right = mid - 1;` → 可能跳过正确答案（mid 本身可能是最小可行值）
   - ✓ `right = mid;` → 保留 mid 在搜索范围内

4. **mid 溢出**
   - ✗ `int mid = (left + right) / 2;` → 当 left + right 很大时可能溢出
   - ✓ `int mid = left + (right - left) / 2;`
   - （本题数据范围 `5*10^4 * 500 = 2.5*10^7`，int 不会溢出，但养成好习惯）

5. **验证函数中忘记处理 `curLoad + w > cap` 时要重置 curLoad**
   - ✗ 开新天后直接 `curLoad += w`，但忘了先 `curLoad = 0`
   - ✓ 先 `curLoad = 0`，再 `curLoad += w`（或直接 `curLoad = w` 也行，更简洁）

## 面试追问

**Q1: 为什么验证函数用贪心是对的？不需要 DP 来最优分配吗？**
> 因为包裹顺序固定（不能重排），每天的装载是连续子数组。贪心策略"能装就装"是最优的——如果当前天提前结束（少装了），后面只会需要更多天，不会更少。

**Q2: 如果包裹可以任意重排顺序，这道题怎么变？**
> 变成一个 NP-hard 的分箱问题（Bin Packing），不能简单贪心。但如果只是让天数最少（不限制天数具体为多少），排序后贪心可以得到近似解。

**Q3: 如果有多条船，每条船独立运输（并行），怎么改？**
> 这变成了「将数组分成 K 个连续子数组，使最大子数组和最小」——和本题本质相同！LeetCode 410（分割数组的最大值）就是这道题的变体。

**Q4: 能否用 DP 做这道题？**
> 可以！`dp[i][j]` = 前 i 个包裹分 j 天的最小运载能力。转移需要枚举分割点，时间 `O(n² × days)`，比二分答案慢。但这个 DP 思路在 LeetCode 410 的题解中很常见。

## 相关题型

- **LeetCode 875 爱吃香蕉的珂珂** — 几乎一模一样的「二分答案」模板：搜索空间是吃香蕉的速度，验证函数计算吃完所有堆需要的小时数。复用本题的二分框架，只改验证函数。

- **LeetCode 410 分割数组的最大值** — 本题的"对偶问题"：410 是给定分 K 段，求最大段和的最小值。和本题完全等价（运载能力 = 最大段和，days = K），代码几乎相同。

- **LeetCode 1482 制作 m 束花所需的最少天数** — 同样是「二分答案」：搜索空间是天数，验证函数检查该天数下能否做出 m 束花。复用本题的二分模板。

- **LeetCode 774 最小化去加油站的最大距离**（Premium）— 连续答案空间上的二分，区别是搜索空间是浮点数，需要用精度控制循环。