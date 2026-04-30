# 3013. 将数组分成最小总代价的子数组 II

## 核心思路

将数组分成 k 个连续子数组，每个子数组的"代价"是其第一个元素。第一个子数组必须从 index 0 开始（代价固定为 `nums[0]`）。剩余 k-1 个分割点的起始索引 `i1, i2, ..., ik-1` 需满足 `ik-1 - i1 <= dist`。求所有子数组代价之和的最小值。

**问题转化**：在滑动窗口 `[i, i+dist]` 内选 k-1 个最小元素。用**两个有序多重集合（multiset）**动态维护窗口内前 k-1 小的元素及其和。

## 思维链

1. **读完题第一反应**：代价 = 每个子数组的第一个元素。第一个子数组代价固定为 `nums[0]`。需要在 `nums[1..n-1]` 中选 k-1 个下标作为其余子数组的起点，且这些下标落在长度为 `dist+1` 的窗口内。总代价 = `nums[0]` + 这 k-1 个下标对应值的和。

2. **问题简化**：在 `nums[1..n-1]` 的每个大小为 `dist+1` 的滑动窗口中，选 k-1 个最小的元素，求最小和。答案 = `nums[0]` + 最小的这个和。

3. **暴力做法**：对每个窗口位置，排序选前 k-1 小。O(n * (dist+1) * log(dist+1))，太慢。

4. **滑动窗口 + 两个有序集合**：维护 `small`（前 k-1 小）和 `large`（其余）两个 multiset，以及 `sumSmall`。窗口滑动时：
   - 新元素进入：比较与 `small` 的最大值，决定放入哪个集合
   - 旧元素离开：从所在集合删除
   - 每次操作后调整两个集合的大小使 `|small| = k-1`

5. **为什么用 multiset？** 需要支持：插入 O(log n)、删除特定元素 O(log n)、取最大/最小值 O(1)。multiset 完美满足。C++ 中 `multiset` 有 `begin()`（最小）和 `rbegin()`（最大）。

6. **时间复杂度**：每个元素最多被插入和删除各一次，每次 O(log n)。总时间 O(n log n)。

## 滑动窗口过程图示

```
nums = [1, 3, 2, 6, 4, 2], k=3, dist=3
固定代价 nums[0] = 1, 需要选 k-1 = 2 个最小分割点代价

窗口 [1, 1+dist] = [1, 4], 即 nums[1..4] = {3, 2, 6, 4}
  small (前2小): {2, 3}, sumSmall = 5
  large (其余): {4, 6}
  当前答案: 1 + 5 = 6

滑动: 加入 nums[5]=2, 移除 nums[1]=3
  加入 2: 2 <= small最大(3) → 放入small → {2, 2, 3}, sumSmall=7
    small 大小=3 > need=2 → 移出最大 3 到 large
    small = {2, 2}, sumSmall = 4
    large = {3, 4, 6}
  移除 3: 3 在 large 中 → 直接删除
    large = {4, 6}
    small 大小=2 == need → 无需调整
  当前答案: 1 + 4 = 5

最终答案: min(6, 5) = 5
```

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 滑动窗口 + 双multiset | 维护窗口内前k-1小 | O(n log n) | O(n) | ⭐ 首选 |
| 滑动窗口 + 堆(懒删除) | 两个堆+延迟删除 | O(n log n) | O(n) | 替代方案 |
| 滑动窗口 + 线段树/BIT | 值域维护第k小 | O(n log V) | O(V) | 竞赛解法 |

> n = 数组长度, V = 值域大小

## 关键提示

1. **问题转化是关键**：原题看似是 DP 分割问题，但约束 `ik-1 - i1 <= dist` 意味着所有 k-1 个分割点都在一个大小为 `dist+1` 的窗口内。所以本质是滑动窗口 + TopK 问题。

2. **双 multiset 的角色**：
   - `small`：维护窗口内最小的 k-1 个元素（这些是分割点的最优选择）
   - `large`：维护窗口内其余元素（候补）
   - `sumSmall`：`small` 的元素总和（直接参与答案计算）

3. **何时调整**：每次插入或删除后，检查 `small.size()` 是否等于 `need = k-1`：
   - `small.size() > need`：将 `small` 的最大值移到 `large`
   - `small.size() < need` 且 `large` 非空：将 `large` 的最小值移到 `small`

4. **multiset 删除陷阱**：`small.erase(val)` 会删除所有等于 val 的元素！必须用 `small.erase(small.find(val))` 只删一个。

5. **窗口初始化**：第一个窗口 `[1, 1+dist]` 需要一次性插入所有元素。之后每次滑动只插入一个、删除一个。

6. **long long 溢出**：nums[i] 最大 10^9，k-1 个元素的和最大约 10^14，需要 `long long`。

## 解法详解

### 解法1: 滑动窗口 + 双 multiset — O(n log n) / O(n) ⭐ 面试首选

**核心数据结构**：
- `multiset<int> small`：当前窗口内最小的 k-1 个元素
- `multiset<int> large`：窗口内其余元素
- `long long sumSmall`：`small` 中元素的总和

**操作定义**：
- `addToWindow(val)`：将新元素加入窗口
- `removeFromWindow(val)`：将离开窗口的元素移除
- 每次操作后自动调整 small 和 large 的平衡

```
// 添加元素 val 的流程:
//
// val <= small.rbegin()?  → 属于前k-1小, 放入 small
//   YES: small.insert(val), sumSmall += val
//        if small.size > need:
//          moved = small.rbegin()  (small中最大的不应该在这里)
//          移到 large, sumSmall -= moved
//   NO:  large.insert(val)
//        (无需调整, small大小不变)
//
// 删除元素 val 的流程:
//
// val 在 small 中?
//   YES: small.erase(small.find(val)), sumSmall -= val
//        if small.size < need && large非空:
//          moved = large.begin()  (large中最小的应该补上)
//          移到 small, sumSmall += moved
//   NO:  large.erase(large.find(val))
//        (无需调整)
```

```cpp
class Solution {
public:
    long long minimumCost(vector<int>& nums, int k, int dist) {
        int n = nums.size();
        int need = k - 1; // 需要选 need 个分割点

        multiset<int> small, large;
        long long sumSmall = 0;

        // 添加元素到窗口
        auto addToWindow = [&](int val) {
            // 如果 small 不满或 val 比 small 的最大值小，放入 small
            if ((int)small.size() < need || val <= *small.rbegin()) {
                small.insert(val);
                sumSmall += val;
                // small 过多，最大的移到 large
                if ((int)small.size() > need) {
                    int moved = *small.rbegin();
                    small.erase(small.find(moved));
                    sumSmall -= moved;
                    large.insert(moved);
                }
            } else {
                large.insert(val);
            }
        };

        // 从窗口移除元素
        auto removeFromWindow = [&](int val) {
            auto it = small.find(val);
            if (it != small.end()) {
                // val 在 small 中
                small.erase(it);
                sumSmall -= val;
                // small 不足，从 large 补充
                if (!large.empty() && (int)small.size() < need) {
                    int moved = *large.begin();
                    large.erase(large.begin());
                    small.insert(moved);
                    sumSmall += moved;
                }
            } else {
                // val 在 large 中
                large.erase(large.find(val));
            }
        };

        // 初始化第一个窗口 [1, min(1+dist, n-1)]
        for (int i = 1; i <= min(1 + dist, n - 1); i++) {
            addToWindow(nums[i]);
        }

        long long ans = (long long)nums[0] + sumSmall;

        // 滑动窗口: 新元素进入, 旧元素离开
        for (int i = 2 + dist; i < n; i++) {
            addToWindow(nums[i]);               // 右端新元素进入
            removeFromWindow(nums[i - dist - 1]); // 左端旧元素离开
            ans = min(ans, (long long)nums[0] + sumSmall);
        }

        return ans;
    }
};
```

**关键点**：
- `addToWindow` 和 `removeFromWindow` 封装了所有平衡逻辑
- `multiset::rbegin()` 返回最大元素的迭代器，`begin()` 返回最小元素
- 用 `find()` 定位后 `erase(iterator)` 只删一个，避免删除所有重复值

### 解法2: 滑动窗口 + 懒删除堆 — O(n log n) / O(n)

**替代思路**：用两个堆（小顶堆和大顶堆）模拟 multiset 的功能。堆不支持直接删除任意元素，用"懒删除"（标记删除但不立即执行）。

```cpp
class Solution2 {
public:
    long long minimumCost(vector<int>& nums, int k, int dist) {
        int n = nums.size();
        int need = k - 1;

        // small: 最大堆 (前 k-1 小的元素)
        // large: 最小堆 (其余元素)
        priority_queue<int> small;                           // 大顶堆
        priority_queue<int, vector<int>, greater<int>> large; // 小顶堆
        unordered_map<int, int> toDel; // 懒删除标记: val -> 待删次数
        long long sumSmall = 0;
        int smallSize = 0; // small 中有效元素个数

        // 清理堆顶的已标记删除元素
        auto cleanSmall = [&]() {
            while (!small.empty() && toDel.count(small.top()) && toDel[small.top()] > 0) {
                toDel[small.top()]--;
                if (toDel[small.top()] == 0) toDel.erase(small.top());
                small.pop();
            }
        };
        auto cleanLarge = [&]() {
            while (!large.empty() && toDel.count(large.top()) && toDel[large.top()] > 0) {
                toDel[large.top()]--;
                if (toDel[large.top()] == 0) toDel.erase(large.top());
                large.pop();
            }
        };

        // 平衡: 确保 small 有效大小 = need
        auto balance = [&]() {
            while (smallSize > need) {
                cleanSmall();
                int moved = small.top(); small.pop();
                sumSmall -= moved;
                smallSize--;
                large.push(moved);
            }
            while (smallSize < need && !large.empty()) {
                cleanLarge();
                if (large.empty()) break;
                int moved = large.top(); large.pop();
                sumSmall += moved;
                smallSize++;
                small.push(moved);
            }
        };

        // 初始化窗口
        for (int i = 1; i <= min(1 + dist, n - 1); i++) {
            small.push(nums[i]);
            sumSmall += nums[i];
            smallSize++;
        }
        balance();

        long long ans = (long long)nums[0] + sumSmall;

        for (int i = 2 + dist; i < n; i++) {
            // 添加新元素
            cleanSmall();
            if (smallSize < need || nums[i] <= small.top()) {
                small.push(nums[i]);
                sumSmall += nums[i];
                smallSize++;
            } else {
                large.push(nums[i]);
            }

            // 懒删除旧元素
            int oldVal = nums[i - dist - 1];
            toDel[oldVal]++;
            cleanSmall();
            if (!small.empty() && oldVal <= small.top()) {
                // oldVal 曾在 small 中
                sumSmall -= oldVal;
                smallSize--;
            }
            // 否则 oldVal 在 large 中，只标记不用更新 sumSmall

            balance();
            ans = min(ans, (long long)nums[0] + sumSmall);
        }

        return ans;
    }
};
```

**关键点**：懒删除堆实现复杂度高，容易出错。面试中强烈推荐 multiset 方案。

## 解法对比

| | 双 multiset | 懒删除堆 | 线段树 |
|--|-----------|---------|--------|
| 时间 | O(n log n) | O(n log n) | O(n log V) |
| 空间 | O(n) | O(n) | O(V) |
| 代码量 | 中等 | 较多 | 多 |
| 出错概率 | 低 | 高 | 中 |
| 面试建议 | ⭐首选 | 不推荐 | 竞赛用 |

**选择建议**：
- 面试首选双 multiset：代码简洁，接口直观，不容易出错
- 懒删除堆只在不支持有序集合的语言中使用（如 Python, Java 的 TreeMap 等价）
- 线段树/BIT 是竞赛解法，面试中不推荐

## 易错点

1. **multiset erase 误删所有重复值**
   - X `small.erase(val)` → 删除所有值为 val 的元素
   - O `small.erase(small.find(val))` → 只删一个

2. **忘记 long long**
   - X `int sumSmall` → k-1 个 10^9 的和溢出 int
   - O `long long sumSmall`

3. **窗口边界 off-by-one**
   - X 初始窗口为 `[1, dist]` → 少了一个元素
   - O 初始窗口为 `[1, 1+dist]`（长度 dist+1，从 index 1 开始）

4. **滑动时添加和删除顺序**
   - X 先删后加，可能导致 small 暂时不足时从空 large 补
   - O 先加后删（推荐），或加删后统一调整

5. **small 为空时访问 rbegin()**
   - X `addToWindow` 中不检查 small 是否为空直接 `*small.rbegin()`
   - O 先检查 `small.size() < need` 再比较最大值

6. **答案只取最后一个窗口**
   - X 只在循环结束后取答案 → 错过中间更优的窗口
   - O 每次窗口滑动后都 `ans = min(ans, ...)`

## 面试追问

**Q1: 为什么这道题可以转化成滑动窗口 TopK 问题？**
→ 分割点 i1 到 ik-1 必须满足 `ik-1 - i1 <= dist`。这意味着所有 k-1 个分割点都在某个长度为 `dist+1` 的窗口内。我们枚举这个窗口的起始位置（即 i1 的位置），在每个窗口内选 k-1 个最小值作为分割点代价。

**Q2: 能用优先队列（堆）代替 multiset 吗？**
→ 直接用堆不行，因为堆不支持删除任意元素。需要"懒删除"技巧：标记要删除的元素，在取堆顶时跳过。但代码复杂度高，容易出错。Java/Python 中没有 multiset 等价物时才考虑。

**Q3: 如果 k 很大（接近 n）怎么办？**
→ need = k-1 可能接近窗口大小 dist+1。极端情况 need = dist+1 时，整个窗口所有元素都要选，sumSmall = 窗口总和。此时 small = 窗口全部，large 为空。算法仍然正确，只是 small 满了不需要和 large 交换。

**Q4: 时间复杂度为什么是 O(n log n) 而不是 O(n * k)？**
→ 每个元素最多执行一次 addToWindow 和一次 removeFromWindow。每次操作涉及 multiset 的 insert/erase/find，都是 O(log n)。窗口内 small 和 large 之间的调整也是 O(log n)。总计 O(n log n)。

**Q5: 和 LeetCode 480（滑动窗口中位数）有什么关系？**
→ 非常类似！480 也是用双 multiset/堆维护滑动窗口的有序统计量。480 维护的是中位数（第 n/2 小），本题维护的是前 k-1 小的和。核心的 addToWindow/removeFromWindow + 平衡逻辑几乎一模一样。

## 相关题型

- **LeetCode 480 滑动窗口中位数** — 几乎相同的双 multiset 滑动窗口模板，维护中位数而不是前 k 小的和。掌握 480 后本题水到渠成。
- **LeetCode 295 数据流的中位数** — 两个堆维护中位数，是 480 的静态版（不需要删除元素）。
- **LeetCode 239 滑动窗口最大值** — 滑动窗口经典题，用 deque 维护单调队列。
- **LeetCode 2653 滑动子数组的美丽值** — 滑动窗口 + 有序统计，和本题思路类似。
- **LeetCode 3013 的简化版 3012** — 本题的 I 版本（dist 没有限制），更简单。
