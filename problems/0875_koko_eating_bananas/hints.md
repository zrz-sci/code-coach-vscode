# 875. 爱吃香蕉的珂珂

## 核心思路

这道题本质上是：**在一个有序的答案空间 [1, max(piles)] 上，找到满足条件的最小值**——这就是经典的"二分搜索答案"模式。

## 思维链

1. **读完题第一反应**：珂珂以速度 `k` 吃香蕉，每堆要花 `ceil(piles[i]/k)` 小时，所有堆加起来不超过 `h` 小时就行。那最笨的方法就是从 `k=1` 开始一个一个试，直到找到第一个能在 `h` 小时内吃完的速度。

2. **暴力解的瓶颈**：`k` 的范围是 `[1, max(piles)]`，而 `max(piles)` 可达 `10^9`。线性枚举每个 `k`，每次要遍历所有堆计算时间，总复杂度 `O(max(piles) × n)`，约 `10^13`，远远超时。

3. **关键观察——单调性**：速度 `k` 越大，吃完所需小时数越少。即 `totalHours(k)` 是关于 `k` 的**单调递减函数**。这意味着存在一个分界点：小于它的 `k` 都不行，大于等于它的 `k` 都行。**有单调性 + 找分界点 = 二分搜索！**

4. **搜索空间怎么定？**
   - 最小速度 `k = 1`（每小时至少吃 1 根）
   - 最大速度 `k = max(piles)`（一小时吃掉最大堆，更快没意义，因为每小时只能吃一堆）
   - 在 `[1, max(piles)]` 上二分

5. **判断条件怎么写？** 给定速度 `k`，算出吃完所有堆需要的总小时数。如果 `totalHours <= h`，说明 `k` 够快（甚至可以更慢），往左找；否则 `k` 太慢，往右找。

6. **上取整技巧**：每堆需要 `ceil(piles[i] / k)` 小时，整数上取整可以用 `(piles[i] + k - 1) / k` 避免浮点运算。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力枚举 | 从 k=1 到 max 逐一检查 | O(max × n) | O(1) | 能说出即可 |
| 二分搜索答案 | 在 [1, max] 上二分 k | O(n × log(max)) | O(1) | ⭐ 必须写出 |

> 其中 `max = max(piles)`, `n = piles.length`

## 关键提示

1. **不是在数组上二分，是在答案范围上二分**。搜索空间是「可能的速度值」`[1, max(piles)]`，不是 piles 数组的索引。

2. **为什么能二分？** 问自己：如果速度 `k` 能在 `h` 小时内吃完，那 `k+1` 能不能？当然能，而且更快。这就是单调性。

3. **整数上取整别用浮点**：`ceil(a/b)` 用整数写法 `(a + b - 1) / b`，避免精度问题。

4. **totalHours 的累加可能溢出 int**：虽然题目保证 `h <= 10^9`，但中间计算时 totalHours 可能先超过 `h`，用 `long long` 更安全，或者一旦超过 `h` 就提前 break。

5. **二分的搜索空间收缩过程**（以示例1为例）：
```
piles = [3,6,7,11], h = 8

搜索空间 [1, 11]:
Round1: left=1, right=11, mid=6  → hours=1+1+2+2=6 ≤ 8 → 够快，right=6
Round2: left=1, right=6,  mid=3  → hours=1+2+3+4=10 > 8 → 太慢，left=4
Round3: left=4, right=6,  mid=5  → hours=1+2+2+3=8 ≤ 8 → 够快，right=5
Round4: left=4, right=5,  mid=4  → hours=1+2+2+3=8 ≤ 8 → 够快，right=4
Round5: left=4, right=4   → left==right，结束，答案=4
```

## 解法详解

### 解法1: 暴力枚举 — O(max × n) / O(1)

**思考过程**：最朴素的想法——从最慢的速度 `k=1` 开始，每次计算以该速度吃完所有香蕉需要的总时间，找到第一个 `totalHours <= h` 的 `k` 就是答案。

```cpp
class Solution {
public:
    int minEatingSpeed(vector<int>& piles, int h) {
        int maxPile = *max_element(piles.begin(), piles.end());
        
        for (int k = 1; k <= maxPile; k++) {
            long long totalHours = 0;
            for (int pile : piles) {
                // 上取整：吃这一堆需要多少小时
                totalHours += (pile + k - 1) / k;
            }
            if (totalHours <= h) {
                return k;  // 找到第一个满足条件的，就是最小速度
            }
        }
        return maxPile;  // 最坏情况：以最大堆的大小为速度
    }
};
```

**关键点**：
- 这个解法正确但 `max(piles)` 可达 `10^9`，外层循环太多次，必定超时。
- 但它帮我们明确了核心子问题：给定 `k`，如何计算 `totalHours`——这个函数在后续解法中会直接复用。

---

### 解法2: 二分搜索答案 — O(n × log(max)) / O(1) ⭐ 面试首选

**从解法1优化**：解法1的瓶颈是外层循环 `k` 从 1 到 max 逐一枚举。而我们发现 `totalHours(k)` 随 `k` 增大而单调递减，存在一个"刚好够"的分界点。用二分搜索把 O(max) 的枚举降到 O(log(max))。

```cpp
class Solution {
public:
    int minEatingSpeed(vector<int>& piles, int h) {
        int left = 1;
        int right = *max_element(piles.begin(), piles.end());
        
        // 在 [1, max(piles)] 上二分，找满足条件的最小 k
        while (left < right) {
            int mid = left + (right - left) / 2;
            
            // 计算以速度 mid 吃完所有香蕉需要多少小时
            long long hours = 0;
            for (int pile : piles) {
                hours += (pile + mid - 1) / mid;  // 上取整
            }
            
            if (hours <= h) {
                // mid 速度够快（甚至可能更慢），答案在左半边（含 mid）
                right = mid;
            } else {
                // mid 速度太慢，需要更快，答案在右半边（不含 mid）
                left = mid + 1;
            }
        }
        
        return left;  // left == right 时就是答案
    }
};
```

**为什么用 `left < right` 模板？**
- 我们找的是"满足 `hours <= h` 的最小 k"，即**左边界**问题。
- 当 `hours <= h` 时，`mid` 可能是答案，所以 `right = mid`（不排除 mid）。
- 当 `hours > h` 时，`mid` 一定不是答案，所以 `left = mid + 1`（排除 mid）。
- 循环结束时 `left == right`，就是分界点。

## 解法对比

| | 暴力枚举 | 二分搜索答案 |
|---|---|---|
| 时间 | O(max × n) ≈ 10^13 | O(n × log(max)) ≈ 10^4 × 30 ≈ 3×10^5 |
| 空间 | O(1) | O(1) |
| 核心区别 | 线性扫描答案空间 | 二分缩小答案空间 |
| 面试 | 用来展示理解题意 | **必须写出** |

二分搜索答案是唯一能通过的解法。关键不在于"怎么写二分"，而在于**识别出这道题的答案具有单调性，可以二分**。

## 易错点

1. **上取整写错**：
   - ✗ `totalHours += pile / k;` — 少算了不满一小时的部分，如 `7/4=1` 但实际要 2 小时
   - ✓ `totalHours += (pile + k - 1) / k;` — 整数上取整公式

2. **右边界设错**：
   - ✗ `right = piles.size();` — 右边界应该是最大堆的值，不是堆的数量
   - ✓ `right = *max_element(piles.begin(), piles.end());`

3. **totalHours 溢出**：
   - ✗ `int hours = 0;` — 当 k=1 时，totalHours = sum(piles)，可能超过 int 范围
   - ✓ `long long hours = 0;`

4. **二分判断方向搞反**：
   - ✗ `if (hours <= h) left = mid + 1;` — 这样找的是"不满足条件的最小k"
   - ✓ `if (hours <= h) right = mid;` — 满足条件时向左收缩，找最小的满足条件的 k

5. **`left < right` vs `left <= right`**：
   - 这里用 `left < right`，因为 `right = mid` 不减一。如果用 `left <= right` 配合 `right = mid`，会死循环。

## 面试追问

**Q1: 暴力能过吗？瓶颈在哪？**
不能。暴力从 k=1 枚举到 max(piles)，最坏 10^9 次，每次遍历 piles 花 O(n)，总计 10^13。瓶颈在于线性枚举答案空间太大。

**Q2: 为什么能用二分？怎么证明单调性？**
速度 k 越大，每堆需要的时间 `ceil(piles[i]/k)` 越少（不会增加），所以总时间是 k 的单调递减函数。存在一个分界点，左边不满足，右边满足。找分界点（最左满足点）就是经典二分。

**Q3: 如果珂珂每小时可以从多堆中吃（不限于一堆），问题怎么变？**
那就变成了：总香蕉数 / k 向上取整就是总时间。直接算 `ceil(sum(piles) / k) <= h`，解得 `k >= ceil(sum / h)`，不需要二分了。原题之所以需要二分，就是因为"每小时只能吃一堆"这个约束导致每堆要独立计算。

**Q4: 这道题的二分模板和"在有序数组中找 target"有什么区别？**
搜索空间不同。传统二分在数组索引上搜索，这里在**答案值域**上搜索。判断条件不是直接比较值，而是调用一个 O(n) 的函数来检验。这是"二分搜索答案"这类题的共同特征。

## 相关题型

- **LeetCode 1011. 在 D 天内送达包裹的能力** — 几乎一模一样的结构！搜索空间从「吃香蕉速度」变成「船的载重量」，判断函数从「总小时数 ≤ h」变成「总天数 ≤ D」。直接复用二分答案模板，只改判断函数。
- **LeetCode 410. 分割数组的最大值** — 也是二分答案，搜索空间是「子数组和的最大值」，判断函数是「能否分成 ≤ m 组使得每组和不超过 mid」。
- **LeetCode 69. x 的平方根** — 最简单的二分答案入门题。搜索空间是 [0, x]，判断 `mid*mid <= x`。可以先做这道找感觉。
- **LeetCode 1539. 第 k 个缺失的正整数** — 二分搜索答案的变体，训练在非显式数组上二分。