# 888. 公平的糖果交换

## 核心思路

本质上是一个**数学推导 + 查找问题**：Alice 和 Bob 各交换一个糖果盒后，双方总量相等。设 Alice 总量 SA、Bob 总量 SB，Alice 交出 a 收入 b，则 `SA - a + b = SB - b + a`，推导出 `b = a + (SB - SA) / 2`。有了这个公式，只需遍历 Alice 的每个盒子，看 Bob 是否有对应的 b 值。

## 思维链

1. **读完题第一反应**：暴力枚举——对 Alice 的每个盒子 a 和 Bob 的每个盒子 b，检查交换后总量是否相等。时间 O(m*n)，肯定能做但不够优。

2. **推导数学关系**：交换后 Alice 总量 = SA - a + b，Bob 总量 = SB - b + a。令它们相等：
   ```
   SA - a + b = SB - b + a
   2b = SB - SA + 2a
   b = a + (SB - SA) / 2
   ```
   设 `delta = (SB - SA) / 2`，则对每个 a，我们需要找 Bob 中是否存在 `b = a + delta`。

3. **加速查找**：把 Bob 的盒子放入 HashSet，查找 O(1)。总时间 O(m + n)。

4. **也可以排序 + 二分查找**：把 Bob 排序，对每个 a 二分查找 `a + delta`。时间 O(m*log(n) + n*log(n))。

5. **还可以排序 + 双指针**：两个数组都排序，用双指针同步扫描找到满足条件的对。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力枚举 | 双重循环检查每对 (a, b) | O(m*n) | O(1) | 能说出即可 |
| HashSet 查找 | 公式推导 + Set 查找 | O(m+n) | O(n) | ⭐ 面试首选 |
| 排序 + 二分查找 | 公式推导 + 二分 | O((m+n)log(n)) | O(1) | 可选方案 |
| 排序 + 双指针 | 两数组排序后同步扫描 | O((m+n)log(m+n)) | O(1) | 可选方案 |

> m = len(aliceSizes), n = len(bobSizes)

## 关键提示

1. **公式推导是关键**：`b = a + (SB - SA) / 2`。设 `delta = (SB - SA) / 2`，对每个 Alice 的 a，查找 Bob 中是否有 `a + delta`。

2. **`(SB - SA)` 一定是偶数**：因为 SA + SB 是总量，交换不改变总量，要让双方各得 (SA+SB)/2。如果 SA+SB 是奇数则无解，但题目保证有解。

3. **delta 的正负**：如果 SA > SB，delta < 0，意味着 Alice 要找一个比 a 小的 b；如果 SA < SB，delta > 0，Alice 要找一个比 a 大的 b。公式自动处理了这两种情况。

4. **HashSet 存 Bob 的数组**：因为 Alice 数组要遍历，Bob 数组用于查找，把被查找的一方放入 Set。

5. **题目保证至少有一个答案**：不需要处理无解情况。

## 解法详解

### 解法1: 暴力枚举 — O(m*n) / O(1)

**思考过程**：最直觉——双重循环枚举 Alice 的每个盒子 a 和 Bob 的每个盒子 b，检查交换后双方总量是否相等。

```cpp
class Solution {
public:
    vector<int> fairCandySwap(vector<int>& aliceSizes, vector<int>& bobSizes) {
        int sumA = 0, sumB = 0;
        for (int a : aliceSizes) sumA += a;
        for (int b : bobSizes) sumB += b;
        
        for (int a : aliceSizes) {
            for (int b : bobSizes) {
                // 交换后: Alice = sumA - a + b, Bob = sumB - b + a
                if (sumA - a + b == sumB - b + a) {
                    return {a, b};
                }
            }
        }
        return {}; // 题目保证有解，不会走到这里
    }
};
```

**关键点**：时间 O(m*n)，虽然简单但效率低。可以通过化简条件 `sumA - a + b == sumB - b + a` 得到 `b = a + (sumB - sumA) / 2`，从而引出优化思路。

### 解法2: HashSet 查找 — O(m+n) / O(n) ⭐ 面试首选

**从暴力优化**：推导出 `b = a + delta`（delta = (SB-SA)/2）后，对每个 a 只需要判断 Bob 中是否存在 `a + delta`。用 HashSet 将查找从 O(n) 降到 O(1)。

```cpp
class Solution {
public:
    vector<int> fairCandySwap(vector<int>& aliceSizes, vector<int>& bobSizes) {
        int sumA = 0, sumB = 0;
        for (int a : aliceSizes) sumA += a;
        for (int b : bobSizes) sumB += b;
        
        int delta = (sumB - sumA) / 2;
        
        // 把 Bob 的盒子放入 Set
        unordered_set<int> bobSet(bobSizes.begin(), bobSizes.end());
        
        for (int a : aliceSizes) {
            int target = a + delta;
            if (bobSet.count(target)) {
                return {a, target};
            }
        }
        return {};
    }
};
```

**关键点**：数学推导将问题从"双重枚举"降维为"单次遍历 + 查找"，HashSet 保证 O(1) 查找。

### 解法3: 排序 + 二分查找 — O((m+n)log(n)) / O(1)

**不用额外空间的替代方案**：把 Bob 排序，对每个 a 用二分查找 `a + delta`。空间 O(1)（如果排序是原地的）。

```cpp
class Solution {
public:
    vector<int> fairCandySwap(vector<int>& aliceSizes, vector<int>& bobSizes) {
        int sumA = 0, sumB = 0;
        for (int a : aliceSizes) sumA += a;
        for (int b : bobSizes) sumB += b;
        
        int delta = (sumB - sumA) / 2;
        
        sort(bobSizes.begin(), bobSizes.end());
        
        for (int a : aliceSizes) {
            int target = a + delta;
            if (binary_search(bobSizes.begin(), bobSizes.end(), target)) {
                return {a, target};
            }
        }
        return {};
    }
};
```

**关键点**：如果面试官限制不能使用 HashSet（或者要求 O(1) 额外空间），排序 + 二分查找是好的替代方案。

### 解法4: 排序 + 双指针 — O((m+n)log(m+n)) / O(1)

**完全不同的思路**：两个数组都排序，用双指针同步扫描。指针 i 遍历 Alice，指针 j 遍历 Bob。根据 `a - b` 和 `delta` 的关系移动指针。

```cpp
class Solution {
public:
    vector<int> fairCandySwap(vector<int>& aliceSizes, vector<int>& bobSizes) {
        int sumA = 0, sumB = 0;
        for (int a : aliceSizes) sumA += a;
        for (int b : bobSizes) sumB += b;
        
        int delta = (sumB - sumA) / 2;
        
        sort(aliceSizes.begin(), aliceSizes.end());
        sort(bobSizes.begin(), bobSizes.end());
        
        int i = 0, j = 0;
        while (i < (int)aliceSizes.size() && j < (int)bobSizes.size()) {
            int diff = aliceSizes[i] - bobSizes[j];
            if (diff == -delta) {
                // a + delta == b，即 a - b == -delta
                return {aliceSizes[i], bobSizes[j]};
            } else if (diff < -delta) {
                // a 太小，需要更大的 a
                i++;
            } else {
                // b 太小，需要更大的 b
                j++;
            }
        }
        return {};
    }
};
```

**关键点**：双指针的移动方向由 `a - b` 和 `-delta` 的关系决定。类似于两数之和的排序双指针解法。

## 解法对比

| | 暴力枚举 | HashSet | 排序+二分 | 排序+双指针 |
|--|---------|---------|----------|------------|
| 时间 | O(mn) | O(m+n) | O((m+n)log(n)) | O((m+n)log(m+n)) |
| 空间 | O(1) | O(n) | O(1) | O(1) |
| 思路难度 | 低 | 低 | 低 | 中 |
| 代码难度 | 低 | 低 | 低 | 中 |
| 面试建议 | 说思路 | ⭐首选 | 追问优化 | 加分项 |

**选择建议**：
- 面试首选 HashSet 解法：时间最优，代码简洁，数学推导清晰
- 如果面试官限制空间，给排序 + 二分查找
- 双指针作为展示多种思路的加分项

## 易错点

1. **delta 的计算方向搞反**
   - 错：`delta = (sumA - sumB) / 2` → 需要 `target = a - delta`
   - 对：统一用 `delta = (sumB - sumA) / 2`，则 `target = a + delta`

2. **忘记考虑 delta 可能是负数**
   - 错：假设 delta 一定是正数 → 当 Alice 总量更大时找不到答案
   - 对：公式自动处理正负，不需要特判

3. **HashSet 存错数组**
   - 错：把 Alice 放入 Set，遍历 Bob → 公式要反过来
   - 对：被查找的数组放入 Set（Bob），遍历的数组逐个查找（Alice）

4. **双指针移动方向搞错**
   - 错：当 diff < -delta 时移动 j → 越走越远
   - 对：diff < -delta 说明 a 太小，移动 i 增大 a

5. **整数溢出**
   - 在 sumA, sumB 计算中，如果 aliceSizes[i] 范围大可能溢出
   - 本题约束 1 <= aliceSizes[i] <= 10^5, 长度 <= 10^4，最大和 = 10^9，int 范围内安全

## 面试追问

**Q1: 数学公式怎么推导出来的？**
→ 交换后 Alice = SA-a+b，Bob = SB-b+a。令两者相等：SA-a+b = SB-b+a → 2b = 2a + SB-SA → b = a + (SB-SA)/2。

**Q2: 如果没有合法答案怎么办？**
→ 题目保证有解。如果不保证，(SB-SA) 为奇数时一定无解（因为 a, b 都是整数）；为偶数时遍历完 Alice 数组找不到匹配也是无解。

**Q3: 如果允许交换多个盒子呢？**
→ 变成子集和问题（Subset Sum），是 NP 的。但如果总和差和盒子数值范围有限，可以用 DP 解决。

**Q4: 能否同时交换 k 个盒子？**
→ 等价于在 Alice 中选 k 个、Bob 中选 k 个，使得两组总和之差恰好弥补原始差距。k 固定时可以用类似 k-Sum 的方法。

## 相关题型

- **LeetCode 1 两数之和** — 同样的"推导目标值 + HashSet 查找"模式。本题的公式推导更显式，但查找逻辑完全一致。
- **LeetCode 167 两数之和 II** — 排序 + 双指针的经典应用，本题的解法4 用了类似技巧。
- **LeetCode 416 分割等和子集** — 如果交换不限一个盒子，问题退化为子集和，可以用 DP 解决。
- **LeetCode 532 数组中的 k-diff 数对** — 也是"遍历 + HashSet 查找目标差值"的模式。
