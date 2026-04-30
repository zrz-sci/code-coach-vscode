# 1648. 销售价值减少的颜色球

## 核心思路

本质上是一个**贪心**问题：每次都卖当前最贵的球（即库存最多的颜色），为了避免逐个卖的 O(orders) 时间开销，用**二分查找**找到一个"阈值"，所有高于阈值的球批量计算总价值。

## 思维链

1. **读完题第一反应**：每次卖当前价值最高的球（库存最多的颜色），这是贪心策略。可以用大顶堆，每次取堆顶，卖一个后放回去。
2. **暴力解的瓶颈**：orders 最多 10⁹，堆每次只卖1个球，O(orders × log n) 必然超时。
3. **关键观察**：我们不需要逐个卖。设想把所有库存从高到低排列，我们要找一个"水位线" threshold —— 所有高于 threshold 的球都卖掉，刚好凑够 orders 个。
4. **怎么找这个 threshold？** 二分查找！对 threshold 进行二分，检查"如果把所有 > threshold 的球都卖掉，总共能卖多少个？"如果能卖的 ≥ orders，说明 threshold 可以更高；否则要更低。
5. **找到 threshold 后怎么算总价值？** 对于每种颜色 inventory[i] > threshold，从 inventory[i] 卖到 threshold+1，用等差数列求和公式批量算。可能多卖了一些，需要在 threshold 这一层微调。
6. **注意取模**：答案可能很大，过程中用 long long 并对 10⁹+7 取模。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 大顶堆模拟 | 每次弹出最大值，卖一个放回 | O(orders × log n) | O(n) | 能说出即可 |
| 排序 + 贪心分层 | 排序后从最大值往下逐层"削平" | O(n log n) | O(1) | ⭐ 必须写出 |
| 二分查找阈值 | 二分找 threshold，批量算等差求和 | O(n log M) | O(1) | ⭐ 必须写出 |

> n = inventory.length, M = max(inventory[i])

## 关键提示

1. **贪心正确性**：每次卖最贵的球一定最优，因为每卖一个球，该颜色的"单价"降1，而其他颜色不变。如果不卖当前最贵的，换成更便宜的，总价值一定更低。

2. **等差数列求和**：从 a 卖到 b+1（即卖 a, a-1, ..., b+1），总价值 = (a + b + 1) × (a - b) / 2。这是批量计算的核心。

3. **二分的搜索空间**：threshold 的范围是 [0, max(inventory)]。check 函数计算 `∑ max(0, inventory[i] - threshold)` 是否 ≥ orders。

4. **精确处理"多卖"**：找到 threshold 后，把所有 > threshold 的球卖到 threshold+1 层可能不够或刚好超。超出的部分在 threshold 这一层，需要精确计算有多少个颜色需要额外卖到 threshold。

5. **溢出陷阱**：inventory[i] 最大 10⁹，等差求和可达 ~10¹⁸，必须用 long long。取模只在最后累加价值时做，二分的 check 函数中计数不需要取模。

## 解法详解

### 解法1: 大顶堆模拟 — O(orders × log n) / O(n)

**思考过程**: 最直觉的贪心——每次卖最贵的。用大顶堆快速取最大值。

```cpp
class Solution {
public:
    int maxProfit(vector<int>& inventory, int orders) {
        const int MOD = 1e9 + 7;
        priority_queue<int> pq(inventory.begin(), inventory.end());
        long long ans = 0;
        while (orders > 0) {
            int top = pq.top(); pq.pop();
            ans = (ans + top) % MOD;
            pq.push(top - 1);  // 卖掉一个，库存-1
            orders--;
        }
        return ans;
    }
};
```

**关键点**: orders 最大 10⁹，每次只卖1个 → **必然超时**。但这个解法帮助理解贪心策略的正确性。

---

### 解法2: 排序 + 贪心分层 — O(n log n) / O(1) ⭐ 面试首选

**从解法1优化**: 解法1每次只卖1个球，瓶颈在于重复操作。观察：如果最大的几种颜色库存都是 10，下一层是 7，那么可以一次性把这几种颜色从 10 "削"到 7，用等差数列批量算价值。

```
// 示例: inventory = [2, 8, 4, 10, 6], orders = 20
// 排序后: [10, 8, 6, 4, 2]
//
// 可视化（每列代表一种颜色的库存高度）:
//
// 10 |X|                     ← 第1层: 只有1种颜色在10, 削到8, 卖 1×(10+9)=19, 共2个球
//  9 |X|
//  8 |X|X|                   ← 第2层: 有2种颜色在8, 削到6, 卖 2×(8+7)=30, 共4个球
//  7 |X|X|
//  6 |X|X|X|                 ← 第3层: 有3种颜色在6, 削到4, 卖 3×(6+5)=33, 共6个球
//  5 |X|X|X|
//  4 |X|X|X|X|               ← 第4层: 有4种颜色在4, 削到2, 需要8个球但只剩8个orders
//  3 |X|X|X|X|                  实际只需再卖8个, 可以全部削完
//  2 |X|X|X|X|X|
//  1 | | | | | |
//     c1 c2 c3 c4 c5
//
// 已用: 2+4+6+8 = 20 = orders ✓
// 总价值: 19+30+33+28 = 110
```

```cpp
class Solution {
public:
    int maxProfit(vector<int>& inventory, int orders) {
        const long long MOD = 1e9 + 7;
        sort(inventory.begin(), inventory.end(), greater<int>());
        int n = inventory.size();
        long long ans = 0;
        int i = 0;
        
        while (orders > 0) {
            // 当前最高水位
            long long cur = inventory[i];
            // 下一个不同的水位（如果 i 是最后一种颜色，下一层是0）
            long long next = (i + 1 < n) ? inventory[i + 1] : 0;
            // 有 (i+1) 种颜色都在 cur 这个高度
            long long width = i + 1;
            // 从 cur 削到 next+1，每种颜色卖 (cur - next) 个
            long long height = cur - next;
            long long canSell = width * height;
            
            if (canSell <= orders) {
                // 整层都能卖完
                // 等差求和: 每种颜色从 cur 卖到 next+1
                // 求和 = (cur + next + 1) * (cur - next) / 2
                long long sum = (cur + next + 1) % MOD * ((cur - next) % MOD) % MOD;
                sum = sum * 500000004 % MOD; // 500000004 是 2 在 MOD 下的逆元
                ans = (ans + sum % MOD * (width % MOD)) % MOD;
                orders -= canSell;
                i++;
            } else {
                // 不能整层削完，需要在这层精确分配
                long long fullRows = orders / width;    // 能完整削几行
                long long remainder = orders % width;   // 剩余零散的
                
                if (fullRows > 0) {
                    long long bottom = cur - fullRows + 1;
                    // 等差求和: cur + (cur-1) + ... + bottom = (cur+bottom)*fullRows/2
                    long long sum = (cur + bottom) % MOD * (fullRows % MOD) % MOD;
                    sum = sum * 500000004 % MOD;
                    ans = (ans + sum % MOD * (width % MOD)) % MOD;
                }
                // 剩余 remainder 个球，每个价值 (cur - fullRows)
                ans = (ans + remainder % MOD * ((cur - fullRows) % MOD)) % MOD;
                orders = 0;
            }
        }
        return (int)(ans % MOD);
    }
};
```

**关键点**: 逆元的使用——因为我们取模后不能直接除以2，需要乘以2的模逆元 `500000004`（因为 2 × 500000004 ≡ 1 (mod 10⁹+7)）。

---

### 解法3: 二分查找阈值 — O(n log M) / O(1) ⭐ 面试首选

**思路转换**: 不逐层削，直接二分找到最终的"水位线" threshold。所有库存 > threshold 的颜色，从 inventory[i] 卖到 threshold+1。

```
// 二分搜索 threshold 的过程:
//
// inventory = [2, 8, 4, 10, 6], orders = 20
//
// 搜索空间: [0, 10]
//
// threshold=5: 能卖 (10-5)+(8-5)+(6-5) = 5+3+1 = 9 < 20 → 太高，往低
// threshold=2: 能卖 (10-2)+(8-2)+(6-2)+(4-2) = 8+6+4+2 = 20 >= 20 → 可行，往高试试
// threshold=3: 能卖 (10-3)+(8-3)+(6-3)+(4-3) = 7+5+3+1 = 16 < 20 → 太高
// → threshold=2 是答案
//
// 验证: 把 >2 的部分全卖 = 20 个，刚好!
// 价值 = (10+3)*8/2 + (8+3)*6/2 + (6+3)*4/2 + (4+3)*2/2
//       = 52 + 33 + 18 + 7 = 110 ✓
```

```cpp
class Solution {
public:
    int maxProfit(vector<int>& inventory, int orders) {
        const long long MOD = 1e9 + 7;
        int n = inventory.size();
        
        // 第一步: 二分找 threshold
        // 含义: 最终所有颜色的库存都不超过 threshold（卖掉 >threshold 的部分）
        long long lo = 0, hi = *max_element(inventory.begin(), inventory.end());
        
        while (lo < hi) {
            long long mid = lo + (hi - lo) / 2;
            // 计算: 如果 threshold = mid，能卖多少个球？
            long long canSell = 0;
            for (int x : inventory) {
                if (x > mid) canSell += x - mid;
            }
            if (canSell <= orders) {
                // 卖得不够多（或刚好），threshold 应该更低
                hi = mid;
            } else {
                // 卖得太多了，threshold 应该更高
                lo = mid + 1;
            }
        }
        
        long long threshold = lo;
        
        // 第二步: 计算总价值
        // 先把所有 > threshold 的部分（从 inventory[i] 卖到 threshold+1）的价值算出来
        long long ans = 0;
        long long totalSold = 0;
        
        for (int x : inventory) {
            if (x > threshold) {
                // 等差数列: x + (x-1) + ... + (threshold+1)
                // = (x + threshold + 1) * (x - threshold) / 2
                long long high = x, low = threshold + 1;
                long long count = high - low + 1;
                long long sum = (high + low) % MOD * (count % MOD) % MOD * 500000004 % MOD;
                ans = (ans + sum) % MOD;
                totalSold += count;
            }
        }
        
        // 可能还差一些球（都在 threshold 这一层的价值）
        long long remaining = orders - totalSold;
        // remaining 个球，每个价值 threshold
        ans = (ans + remaining % MOD * (threshold % MOD)) % MOD;
        
        return (int)((ans % MOD + MOD) % MOD);
    }
};
```

**关键点**: 二分条件的方向容易搞混。`canSell <= orders` 时 `hi = mid`（threshold 可以更低来卖更多球），`canSell > orders` 时 `lo = mid + 1`。

---

## 解法对比

| | 解法1: 堆模拟 | 解法2: 排序分层 | 解法3: 二分阈值 |
|---|---|---|---|
| **时间** | O(orders × log n) | O(n log n) | O(n log M) |
| **空间** | O(n) | O(1)（原地排序） | O(1) |
| **适用** | orders 小时 | 通用 | 通用，M 大也只是 log |
| **代码复杂度** | 简单 | 中等（分层逻辑） | 中等（二分+求和） |
| **面试推荐** | 说思路即可 | ⭐ 首选 | ⭐ 首选 |

> M = max(inventory[i])。两种 O(n log) 解法在面试中任选一种即可，但都要能说清楚。

## 易错点

1. **整数溢出**：
   - ✗ `int sum = (high + low) * count / 2;` → inventory[i] 最大 10⁹，乘积溢出 int
   - ✓ 全程用 `long long`，或在乘法前强转

2. **除以2的取模问题**：
   - ✗ `(a * b / 2) % MOD` → 先除后模可能丢精度
   - ✓ `(a % MOD) * (b % MOD) % MOD * inv2 % MOD`，其中 `inv2 = 500000004`
   - 或者先判断 a 和 b 哪个是偶数，先除再模

3. **二分边界写错**：
   - ✗ `canSell < orders` 时 `hi = mid - 1` → 可能跳过正确答案
   - ✓ 明确 threshold 的含义：找最大的 threshold 使得 canSell ≥ orders

4. **剩余球的处理遗漏**：
   - 二分找到 threshold 后，`∑(inventory[i] - threshold)` 可能 > orders
   - 必须计算 `remaining = orders - totalSold`，这些剩余球的价值是 threshold（不是 threshold+1）

5. **排序分层解法中 i 越界**：
   - 当所有颜色库存相同时，`i+1 < n` 的检查很关键，否则 `inventory[i+1]` 越界

## 面试追问

**Q1: 为什么贪心策略（每次卖最贵的）是最优的？**
> 简要证明：假设某一步我们不卖最贵的球 A（价值 a），而卖了更便宜的球 B（价值 b < a）。交换这两步，先卖 A 后卖 B，由于 A 的后续价值不变（A 之后还是 a-1, a-2...），总价值不减。所以贪心选最大的不会更差。

**Q2: 二分的 check 函数为什么不需要取模？**
> check 函数计算的是"能卖多少个球"，这是一个精确的计数值，需要和 orders 比较大小。取模后大小关系会被破坏。只有最后算价值时才取模。

**Q3: 如果 inventory 可以动态更新（不断有新球补货），怎么优化？**
> 可以用**平衡 BST / 有序集合**维护库存，每次补货 O(log n) 更新。卖球时仍然用类似"分层"的思想，从最大值开始削。或者用线段树维护值域上的信息，支持区间求和。

## 相关题型

- **LeetCode 1818. 绝对差值和** — 同样用二分查找在值域上找"最优替换点"
- **LeetCode 1482. 制作 m 束花所需的最少天数** — 二分答案 + check 函数的经典模板，和本题的二分框架完全相同
- **LeetCode 1167. 连接棍棒的最低费用** — 也是贪心 + 优先队列，但本题需要优化掉逐个处理的开销
- **LeetCode 870. 优势洗牌** — 贪心 + 排序，思路类似"让价值最大化"