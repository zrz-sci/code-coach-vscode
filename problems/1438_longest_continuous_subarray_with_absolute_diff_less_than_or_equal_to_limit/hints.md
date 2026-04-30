# 1438. 绝对差不超过限制的最长连续子数组

## 核心思路

本质上在问：**找最长的连续窗口 [left, right]，使得窗口内 max - min <= limit**。任意两元素绝对差不超过 limit，等价于窗口最大值与最小值的差不超过 limit。

## 思维链

1. **读完题第一反应**：枚举所有连续子数组，对每个子数组求 max 和 min，检查差值。→ O(n²) 甚至 O(n³) 暴力。

2. **暴力的瓶颈在哪？** 两层循环枚举子数组已经 O(n²)，如果每次从头算 max/min 再加 O(n)，总共 O(n³)。即使维护 max/min 做到 O(n²)，对 10⁵ 数据量也超时。

3. **滑动窗口的直觉**：这是"最长连续子数组满足某条件"的问题，天然适合滑动窗口。右指针扩展，条件不满足时左指针收缩。**关键瓶颈变成：窗口扩展/收缩时，怎么 O(1) 或 O(log n) 维护窗口内的 max 和 min？**

4. **突破瓶颈的三种武器**：
   - **单调队列（Monotonic Deque）**：一个递减队列维护 max，一个递增队列维护 min，每个元素进出队列各一次 → 均摊 O(1) → 总 O(n) ⭐
   - **有序多重集（multiset）**：自动排序，begin() 是 min，rbegin() 是 max，插入删除 O(log n) → 总 O(n log n)
   - **堆（Priority Queue）**：大顶堆取 max，小顶堆取 min，但堆无法高效删除窗口外元素，需要懒删除 → 总 O(n log n)

5. **为什么单调队列最优？** 每个元素最多入队、出队各一次，均摊 O(1)，整体 O(n)。而且空间也是 O(n)。这是滑动窗口维护极值的经典范式。

6. **窗口收缩的正确性**：当 max - min > limit 时，左指针右移。如果 max 或 min 恰好是左指针指向的元素，还需要从单调队列中弹出。只要左指针不超过右指针，窗口就是合法的。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力枚举 | 枚举所有子数组，维护 max/min | O(n²) | O(1) | 能说出即可 |
| multiset 滑动窗口 | 有序集合维护窗口内极值 | O(n log n) | O(n) | 加分项 |
| 堆 + 懒删除 | 大顶堆 max + 小顶堆 min | O(n log n) | O(n) | 能说出即可 |
| 单调队列滑动窗口 | 两个单调 deque 维护极值 | O(n) | O(n) | ⭐ 必须写出 |

## 关键提示

1. **等价转化**：任意两元素绝对差 ≤ limit ⟺ max(窗口) - min(窗口) ≤ limit。只需要跟踪窗口的最大值和最小值。

2. **滑动窗口的单调性**：如果窗口 [left, right] 不满足条件（max - min > limit），那么 [left, right+1], [left, right+2], ... 也都不满足 → 必须收缩左边界。

3. **单调队列的核心**：递减队列（维护 max）保证队首总是当前窗口最大值；递增队列（维护 min）保证队首总是当前窗口最小值。新元素入队时，把队尾所有"不如它"的元素弹出。

4. **收缩时别忘了检查队首**：左指针移动后，如果队首元素的索引 < left，要从队首弹出（已经不在窗口内了）。

5. **窗口演示**（nums = [10,1,2,4,7,2], limit = 5）：
```
right=0: [10]         max=10, min=10, diff=0  ✓  len=1
right=1: [10,1]       max=10, min=1,  diff=9  ✗ → left=1: [1]
right=2: [1,2]        max=2,  min=1,  diff=1  ✓  len=2
right=3: [1,2,4]      max=4,  min=1,  diff=3  ✓  len=3
right=4: [1,2,4,7]    max=7,  min=1,  diff=6  ✗ → left=2: [2,4,7] diff=5 ✓  len=3→4? 
                       等等，[2,4,7] diff=5 ✓  len=3
right=5: [2,4,7,2]    max=7,  min=2,  diff=5  ✓  len=4 ← 答案
```

## 解法详解

### 解法1: 暴力枚举 — O(n²) / O(1)

**思考过程**：最直觉的做法，枚举每个起点 i，向右扩展 j，维护当前子数组的 max 和 min，一旦 max - min > limit 就停止。

```cpp
class Solution {
public:
    int longestSubarray(vector<int>& nums, int limit) {
        int n = nums.size(), ans = 0;
        for (int i = 0; i < n; i++) {
            int mx = nums[i], mn = nums[i];
            for (int j = i; j < n; j++) {
                mx = max(mx, nums[j]);
                mn = min(mn, nums[j]);
                if (mx - mn > limit) break;
                ans = max(ans, j - i + 1);
            }
        }
        return ans;
    }
};
```

**关键点**：n = 10⁵ 时 O(n²) 会 TLE。但这个解法帮我们确认了核心逻辑：只需跟踪窗口 max 和 min。

---

### 解法2: multiset 滑动窗口 — O(n log n) / O(n)

**从解法1优化**：解法1每次从头扩展，无法复用信息。滑动窗口可以复用，但瓶颈是窗口内的 max/min 查询。`multiset` 自动排序，`*begin()` 是 min，`*rbegin()` 是 max，插入删除都是 O(log n)。

```cpp
class Solution {
public:
    int longestSubarray(vector<int>& nums, int limit) {
        multiset<int> window; // 自动排序，允许重复
        int left = 0, ans = 0;
        for (int right = 0; right < (int)nums.size(); right++) {
            window.insert(nums[right]);
            // 窗口不合法时，收缩左边界
            while (*window.rbegin() - *window.begin() > limit) {
                // 只删除一个 nums[left]（不是所有相同值）
                window.erase(window.find(nums[left]));
                left++;
            }
            ans = max(ans, right - left + 1);
        }
        return ans;
    }
};
```

**关键点**：
- `window.erase(window.find(nums[left]))` 只删除一个元素。如果写成 `window.erase(nums[left])` 会删除所有等于 `nums[left]` 的元素！
- multiset 的常数较大，但代码简洁，面试中快速写出有优势。

---

### 解法3: 堆 + 懒删除 — O(n log n) / O(n)

**另一个角度**：用大顶堆取 max，小顶堆取 min。但堆不支持删除中间元素，所以用"懒删除"：堆顶如果索引 < left，说明已不在窗口内，弹掉。

```cpp
class Solution {
public:
    int longestSubarray(vector<int>& nums, int limit) {
        // pair<值, 索引>
        priority_queue<pair<int,int>> maxHeap; // 大顶堆：取 max
        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> minHeap; // 小顶堆：取 min
        int left = 0, ans = 0;
        for (int right = 0; right < (int)nums.size(); right++) {
            maxHeap.push({nums[right], right});
            minHeap.push({nums[right], right});
            // 堆顶可能是已过期的元素，先懒删除再判断
            while (maxHeap.top().first - minHeap.top().first > limit) {
                // 收缩左边界
                left++;
                // 懒删除：把索引 < left 的堆顶弹掉
                while (maxHeap.top().second < left) maxHeap.pop();
                while (minHeap.top().second < left) minHeap.pop();
            }
            ans = max(ans, right - left + 1);
        }
        return ans;
    }
};
```

**关键点**：懒删除的正确性 — 只有在用到堆顶时才检查是否过期，不用时无所谓。最坏情况每个元素入堆出堆各一次，O(n log n)。

---

### 解法4: 单调队列（Monotonic Deque） — O(n) / O(n) ⭐ 面试首选

**核心洞察**：我们需要动态维护窗口的 max 和 min，单调队列可以在均摊 O(1) 时间内做到。

- **递减队列 maxQ**：队首是窗口最大值。新元素入队时，把队尾所有 ≤ 它的弹出（因为它们不可能再成为 max）。
- **递增队列 minQ**：队首是窗口最小值。新元素入队时，把队尾所有 ≥ 它的弹出。

```
// 单调队列示意（以 maxQ 为例，存的是索引）:
// nums = [10, 1, 2, 4, 7, 2]
//
// right=0: maxQ=[0(10)]              队首值=10
// right=1: maxQ=[0(10), 1(1)]        队首值=10
// right=2: maxQ=[0(10), 2(2)]        1<2,弹出1  队首值=10
// → diff=10-1=9>5, left=1, maxQ队首idx=0<1,弹出 → maxQ=[2(2)]
// right=3: maxQ=[3(4)]               2<4,弹出2  队首值=4
// right=4: maxQ=[4(7)]               4<7,弹出4  队首值=7
// right=5: maxQ=[4(7), 5(2)]         2<7,不弹   队首值=7
```

```cpp
class Solution {
public:
    int longestSubarray(vector<int>& nums, int limit) {
        deque<int> maxQ, minQ; // 存索引
        int left = 0, ans = 0;
        for (int right = 0; right < (int)nums.size(); right++) {
            // 维护递减队列（队首最大）
            while (!maxQ.empty() && nums[maxQ.back()] <= nums[right])
                maxQ.pop_back();
            maxQ.push_back(right);
            
            // 维护递增队列（队首最小）
            while (!minQ.empty() && nums[minQ.back()] >= nums[right])
                minQ.pop_back();
            minQ.push_back(right);
            
            // 窗口不合法时收缩
            while (nums[maxQ.front()] - nums[minQ.front()] > limit) {
                left++;
                // 队首过期则弹出
                if (maxQ.front() < left) maxQ.pop_front();
                if (minQ.front() < left) minQ.pop_front();
            }
            
            ans = max(ans, right - left + 1);
        }
        return ans;
    }
};
```

**为什么是 O(n)？** 每个元素最多入队一次、出队一次（从 back 或 front），总操作 ≤ 2n 次。

---

## 解法对比

| | 暴力 | multiset | 堆+懒删 | 单调队列 |
|---|---|---|---|---|
| 时间 | O(n²) | O(n log n) | O(n log n) | **O(n)** |
| 空间 | O(1) | O(n) | O(n) | O(n) |
| 代码复杂度 | 最简单 | 简洁 | 中等 | 中等 |
| 面试推荐 | 说思路即可 | 快速实现 | 展示堆的应用 | **首选** |

**选择建议**：
- 面试首选单调队列，时间最优且展示你对单调队列的理解
- 如果一时想不起单调队列，multiset 解法代码最短，也完全可以
- 堆的解法适合展示"懒删除"技巧

## 易错点

1. **multiset 删除错误**：
   - ✗ `window.erase(nums[left])` — 删除所有等于 `nums[left]` 的元素
   - ✓ `window.erase(window.find(nums[left]))` — 只删除一个

2. **单调队列弹出方向搞反**：
   - ✗ `maxQ` 入队时弹出 `>= nums[right]` 的（这变成了递增队列）
   - ✓ `maxQ` 入队时弹出 `<= nums[right]` 的（保持递减，队首最大）

3. **收缩时忘记检查队首过期**：
   - ✗ left++ 后直接用 `maxQ.front()` 取最大值
   - ✓ left++ 后先检查 `if (maxQ.front() < left) maxQ.pop_front()`

4. **堆解法中收缩逻辑错误**：
   - ✗ 在 while 循环外只做一次懒删除
   - ✓ 每次 left++ 后都要检查两个堆顶是否过期（用 while 循环）

5. **单调队列存值 vs 存索引**：
   - 存值：收缩时不知道队首是否已过期（需要额外比较）
   - ✓ 存索引：可以直接判断 `front() < left`，更干净

## 面试追问

**Q1: 暴力解的瓶颈在哪？**
→ 每次窗口变化后重新求 max/min 是 O(n)，导致总体 O(n²)。我们需要一种数据结构能在 O(1) 或 O(log n) 内动态维护窗口极值。

**Q2: 为什么单调队列能做到均摊 O(1)？**
→ 每个元素最多入队一次、出队一次。虽然单次操作可能弹出多个元素，但全局总弹出次数 ≤ n。所以 n 次操作总时间 O(n)，均摊 O(1)。

**Q3: 如果改成"绝对差恰好等于 limit"怎么办？**
→ 这不再满足滑动窗口的单调性（窗口缩小不一定能让 diff 恰好等于 limit）。需要换思路：枚举 max 或者用哈希表统计差值。问题变成更复杂的组合问题。

**Q4: 如果是二维矩阵，找最大子矩阵使得 max - min ≤ limit？**
→ 固定上下边界，压缩成一维后用单调队列。类似"最大子矩阵和"的枚举行对思路，每对行 O(n) 处理列方向，总 O(m²·n)。

## 相关题型

- **239. 滑动窗口最大值** — 本题的单调队列维护 max 的技巧直接复用，区别：239 是固定窗口大小，本题是可变窗口
- **76. 最小覆盖子串** — 同样是可变窗口滑动，区别：76 用哈希表维护字符计数，本题用单调队列维护极值
- **862. 和至少为 K 的最短子数组** — 单调队列 + 前缀和，复用单调队列维护最小值的技巧
- **1696. 跳跃游戏 VI** — 单调队列优化 DP，复用"滑动窗口内取最大值"的模板