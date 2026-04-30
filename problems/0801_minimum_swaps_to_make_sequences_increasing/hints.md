# 801. 使序列递增的最小交换次数

## 核心思路

本质是一个**状态DP**问题：对于每个位置 `i`，只有两种选择——**交换**或**不交换** `nums1[i]` 和 `nums2[i]`。我们需要跟踪在每个位置做出不同选择后，使得两个序列到当前位置都严格递增所需的最小交换次数。

## 思维链

1. **读完题第一反应** → 每个位置有两种操作（交换/不交换），暴力枚举所有 2^n 种选择？指数级不可行。
2. **关键观察** → 位置 `i` 的合法性只取决于位置 `i-1` 的状态（交换了还是没交换），以及 `nums1[i-1], nums2[i-1], nums1[i], nums2[i]` 的大小关系。
3. **状态定义** → 用两个变量：`noSwap[i]` = 位置 i 不交换时的最小交换次数；`swap[i]` = 位置 i 交换时的最小交换次数。
4. **状态转移** → 分两种情况讨论：
   - 如果 `nums1[i] > nums1[i-1] && nums2[i] > nums2[i-1]`（原始就合法）：不换可以从不换来，换可以从换来。
   - 如果 `nums1[i] > nums2[i-1] && nums2[i] > nums1[i-1]`（交叉也合法）：不换可以从换来，换可以从不换来。
5. **空间优化** → 只依赖前一个位置，可以用两个变量滚动，空间 O(1)。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 状态DP（数组） | dp[i][0/1] 分别表示不交换/交换 | O(n) | O(n) | 能讲出即可 |
| 状态DP（滚动变量） | 只用前一步的两个状态 | O(n) | O(1) | ⭐ 必须写出 |

## 关键提示

1. **题目保证有解**：一定存在一种交换方案使两个数组都严格递增。这意味着对于每个位置 i，至少满足"原始合法"或"交叉合法"中的一个条件（也可能两个都满足）。
2. **两种合法条件可能同时成立**：比如 `nums1 = [1,3,5,7], nums2 = [2,4,6,8]`，每个位置原始合法且交叉也合法。这时两种条件都要考虑取 min。
3. **初始状态**：`swap[0] = 1`（位置 0 交换需要 1 次），`noSwap[0] = 0`（不交换需要 0 次）。
4. **转移时取 min 而非覆盖**：当两种条件同时成立时，需要用 `min` 更新而不是直接赋值。所以初始化 dp 值为 INF 再逐步取 min。
5. **最终答案**：`min(swap[n-1], noSwap[n-1])`。

## 解法详解

### 解法1: 状态DP（滚动变量） -- O(n) / O(1) ⭐ 面试首选

**思考过程**: 每个位置只有交换/不交换两种选择，且合法性只和前一个位置的选择有关，典型的状态机DP。

```
状态转移示意:
位置 i-1        位置 i
 不换(ns) ────→ 不换(ns): 需要原始合法
 不换(ns) ────→ 换(sw):   需要交叉合法
 换(sw)   ────→ 不换(ns): 需要交叉合法
 换(sw)   ────→ 换(sw):   需要原始合法

条件1 (原始合法): a1[i]>a1[i-1] && a2[i]>a2[i-1]
  ns[i] = min(ns[i], ns[i-1])      // 都不换
  sw[i] = min(sw[i], sw[i-1] + 1)  // 都换

条件2 (交叉合法): a1[i]>a2[i-1] && a2[i]>a1[i-1]
  ns[i] = min(ns[i], sw[i-1])      // 前面换了，当前不换
  sw[i] = min(sw[i], ns[i-1] + 1)  // 前面不换，当前换
```

```cpp
class Solution {
public:
    int minSwap(vector<int>& nums1, vector<int>& nums2) {
        int n = nums1.size();
        // prevNoSwap: 上一个位置不交换的最小次数
        // prevSwap:   上一个位置交换的最小次数
        int prevNoSwap = 0, prevSwap = 1;
        
        for (int i = 1; i < n; i++) {
            int curNoSwap = INT_MAX, curSwap = INT_MAX;
            
            // 条件1: 原始顺序就合法
            if (nums1[i] > nums1[i-1] && nums2[i] > nums2[i-1]) {
                curNoSwap = min(curNoSwap, prevNoSwap);      // 都不换
                curSwap   = min(curSwap,   prevSwap + 1);    // 都换
            }
            
            // 条件2: 交叉后合法
            if (nums1[i] > nums2[i-1] && nums2[i] > nums1[i-1]) {
                curNoSwap = min(curNoSwap, prevSwap);        // 前换当前不换
                curSwap   = min(curSwap,   prevNoSwap + 1);  // 前不换当前换
            }
            
            prevNoSwap = curNoSwap;
            prevSwap = curSwap;
        }
        
        return min(prevNoSwap, prevSwap);
    }
};
```

**关键点**:
- 初始化 `curNoSwap = curSwap = INT_MAX`，只通过满足条件来更新，保证不合法的转移不会被选中。
- 两个条件用 `if`（不是 `else if`），因为可能同时成立。
- 空间优化到 O(1)：只需要 `prev` 和 `cur` 两组变量。

---

### 解法2: 状态DP（数组版） -- O(n) / O(n)

```cpp
class Solution {
public:
    int minSwap(vector<int>& nums1, vector<int>& nums2) {
        int n = nums1.size();
        vector<int> noSwap(n, INT_MAX), sw(n, INT_MAX);
        noSwap[0] = 0;
        sw[0] = 1;
        
        for (int i = 1; i < n; i++) {
            if (nums1[i] > nums1[i-1] && nums2[i] > nums2[i-1]) {
                noSwap[i] = min(noSwap[i], noSwap[i-1]);
                sw[i] = min(sw[i], sw[i-1] + 1);
            }
            if (nums1[i] > nums2[i-1] && nums2[i] > nums1[i-1]) {
                noSwap[i] = min(noSwap[i], sw[i-1]);
                sw[i] = min(sw[i], noSwap[i-1] + 1);
            }
        }
        
        return min(noSwap[n-1], sw[n-1]);
    }
};
```

## 易错点

1. **两个条件用 else if 而非独立 if**：
   - ✗ `if (条件1) {...} else if (条件2) {...}` → 两个条件可能同时成立，漏掉了一半转移
   - ✓ `if (条件1) {...} if (条件2) {...}` → 两个条件独立判断

2. **交叉条件写错**：
   - ✗ `nums1[i] > nums1[i-1] && nums2[i] > nums1[i-1]` → 第二个比较对象错了
   - ✓ `nums1[i] > nums2[i-1] && nums2[i] > nums1[i-1]` → 注意是交叉比较

3. **忘记初始化为 INT_MAX**：
   - ✗ `curNoSwap = 0, curSwap = 0` → 不合法的状态被当成合法
   - ✓ `curNoSwap = INT_MAX, curSwap = INT_MAX` → 只有通过合法条件才能更新

4. **swap 初始值不是 1**：
   - ✗ `prevSwap = 0` → 位置 0 交换需要花费 1 次操作
   - ✓ `prevSwap = 1`

## 面试追问

**Q1: 为什么只需要考虑相邻位置的关系？**
> 因为严格递增只要求 `arr[i] > arr[i-1]`。只要每对相邻元素都满足严格递增，整个数组就严格递增。所以每个位置的决策只取决于前一个位置的状态。

**Q2: 如果允许交换任意位置的 nums1[i] 和 nums2[j]（不限 i==j），问题会变怎样？**
> 那就不再是简单的状态DP了。交换范围扩大后，一个位置的决策可能影响其他不相邻位置，需要更复杂的建模（比如图论或全局优化）。当前题目的约束"只能在同一位置交换"是关键简化。

**Q3: 能否用贪心解决？**
> 不能简单贪心。比如当前位置不交换是局部最优，但可能导致后续位置需要更多交换。DP 通过同时跟踪两种状态避免了这个问题。贪心没有全局视野。

## 相关题型

- **122. 买卖股票的最佳时机 II** — 同样是状态机DP，每天有"持有/不持有"两种状态，转移只看前一天。
- **309. 最佳买卖股票时机含冷冻期** — 状态机DP的进阶，三种状态（持有/冷冻/空闲），与本题的两状态转移思路一致。
- **714. 买卖股票的最佳时机含手续费** — 状态机DP，持有/不持有两状态滚动，和本题空间优化方式相同。
- **926. 将字符串翻转到单调递增** — 类似的两状态DP，每个位置选择0或1，使序列满足单调性。
