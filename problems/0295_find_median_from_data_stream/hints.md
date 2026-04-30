# 295. 数据流的中位数

## 核心思路

本质上是一个**动态维护有序性**的问题：数据不断流入，每次都需要快速取出"中间位置"的元素。关键在于：我们不需要维护全局有序，只需要知道"中间那一两个数是什么"。

## 思维链

1. **读完题第一反应**：每次加入数字后排序，取中间值。这就是暴力解——维护一个有序数组。
2. **暴力解的瓶颈在哪？** 每次插入需要找到正确位置并插入，数组插入是 O(n)（移动元素）；或者先 push_back 再排序是 O(n log n)。在大量调用下这太慢了。
3. **重新审视需求**：我们真正需要的不是"全部有序"，而是能快速拿到"中间的数"。如果把数据分成"较小的一半"和"较大的一半"，中位数就在这两半的交界处。
4. **什么数据结构能快速拿最值？** 堆！较小的一半用**大顶堆**（堆顶是较小一半中的最大值），较大的一半用**小顶堆**（堆顶是较大一半中的最小值）。中位数就从两个堆顶取。
5. **如何维护平衡？** 保证两个堆的大小差不超过 1，每次插入后如果不平衡就把多的那边堆顶挪给另一边。
6. **Follow-up**：如果数据范围是 [0, 100]，可以用计数排序的思想——维护一个频次数组，找中位数时遍历频次数组即可。

## 解法概览

| 解法 | 思路 | addNum 时间 | findMedian 时间 | 空间 | 面试 |
|------|------|-------------|-----------------|------|------|
| 有序数组/插入排序 | 维护有序数组，二分找位置插入 | O(n) | O(1) | O(n) | 能说出即可 |
| 双堆（大顶堆+小顶堆） | 两个堆分别维护较小和较大的一半 | O(log n) | O(1) | O(n) | ⭐ 必须写出 |
| multiset + 双迭代器 | 用平衡BST，维护中位数指针 | O(log n) | O(1) | O(n) | 加分项 |
| 计数排序 (Follow-up) | 频次数组 + 遍历找中位数 | O(1) | O(100) | O(101) | Follow-up |

## 关键提示

1. **不要想着维护全局有序**——你只需要知道"中间的数"，这比排序容易得多。
2. **两个堆的分工**：大顶堆存较小的一半，小顶堆存较大的一半。想象一下把有序数组从中间切开，左边是大顶堆，右边是小顶堆。
3. **平衡策略**：让大顶堆的 size 始终等于小顶堆的 size 或者恰好多 1。这样奇数个元素时中位数就是大顶堆顶，偶数个时取两堆顶平均。
4. **插入时不要先判断该放哪个堆**——更简单的做法是：先无脑放进大顶堆，把大顶堆顶挤到小顶堆，再根据 size 调整。这保证了正确性。
5. **Follow-up 的 [0, 100] 范围**：说明数据离散且范围小，用桶计数 + 线性扫描比堆更高效。

```
// 双堆示意图 (元素 [1, 2, 3, 4, 5]):
//
// maxHeap (较小一半)    minHeap (较大一半)
//     ┌───┐                ┌───┐
//     │ 3 │ ← 堆顶        │ 4 │ ← 堆顶
//     ├───┤                ├───┤
//     │ 2 │                │ 5 │
//     ├───┤                └───┘
//     │ 1 │
//     └───┘
//  size = 3               size = 2
//
//  奇数个元素 → 中位数 = maxHeap.top() = 3
//
// 如果再加入 6:
//  maxHeap: [1,2,3]  minHeap: [4,5,6]  各3个
//  偶数个元素 → 中位数 = (3 + 4) / 2 = 3.5
```

## 解法详解

### 解法1: 有序数组（插入排序） — O(n) addNum / O(1) findMedian

**思考过程**：最直觉的做法——如果数组始终有序，中位数就是 `arr[n/2]`（奇数）或 `(arr[n/2-1] + arr[n/2]) / 2`（偶数）。每次加入新数时，用二分查找找到插入位置，然后插入。

```cpp
class MedianFinder {
    vector<int> sorted_arr;
public:
    MedianFinder() {}
    
    void addNum(int num) {
        // 二分找到插入位置 — O(log n)
        auto pos = lower_bound(sorted_arr.begin(), sorted_arr.end(), num);
        // 插入 — O(n)，因为要移动后续元素
        sorted_arr.insert(pos, num);
    }
    
    double findMedian() {
        int n = sorted_arr.size();
        if (n % 2 == 1) return sorted_arr[n / 2];
        return (sorted_arr[n / 2 - 1] + sorted_arr[n / 2]) / 2.0;
    }
};
```

**瓶颈分析**：二分查找是 O(log n)，但 `vector::insert` 在中间位置是 O(n)（需要移动元素）。5×10⁴ 次操作，最坏 O(n²) ≈ 2.5×10⁹，会超时。

---

### 解法2: 双堆 — O(log n) addNum / O(1) findMedian ⭐ 面试首选

**从解法1优化**：解法1的瓶颈是"维护全局有序的插入代价"。但我们并不需要全局有序，只需要知道中间位置的值。

**核心洞察**：把数据分成两半——较小的一半和较大的一半。中位数就在两半的交界处。用什么数据结构能快速拿到"较小一半中的最大值"和"较大一半中的最小值"？→ **大顶堆和小顶堆！**

**插入策略（重要）**：
1. 先把 num 放入大顶堆 maxHeap
2. 把 maxHeap 的堆顶（最大值）弹出，放入小顶堆 minHeap
3. 如果 minHeap.size() > maxHeap.size()，把 minHeap 堆顶弹出放回 maxHeap

这个三步走保证了：(a) maxHeap 里的所有元素 ≤ minHeap 里的所有元素；(b) maxHeap.size() == minHeap.size() 或 maxHeap.size() == minHeap.size() + 1。

```cpp
class MedianFinder {
    // maxHeap: 存较小的一半，堆顶是这一半中的最大值
    priority_queue<int> maxHeap;
    // minHeap: 存较大的一半，堆顶是这一半中的最小值
    priority_queue<int, vector<int>, greater<int>> minHeap;
    
public:
    MedianFinder() {}
    
    void addNum(int num) {
        // Step1: 先放入大顶堆
        maxHeap.push(num);
        // Step2: 把大顶堆的最大值挤到小顶堆
        //        这保证了 maxHeap 所有元素 <= minHeap 所有元素
        minHeap.push(maxHeap.top());
        maxHeap.pop();
        // Step3: 如果小顶堆多了，搬一个回来
        //        保证 maxHeap.size() >= minHeap.size()
        if (minHeap.size() > maxHeap.size()) {
            maxHeap.push(minHeap.top());
            minHeap.pop();
        }
    }
    
    double findMedian() {
        // 奇数个: maxHeap 多一个，堆顶就是中位数
        if (maxHeap.size() > minHeap.size())
            return maxHeap.top();
        // 偶数个: 两个堆顶的平均值
        return (maxHeap.top() + minHeap.top()) / 2.0;
    }
};
```

**为什么三步走是正确的？**
- Step1+Step2 相当于：num 先经过大顶堆"过滤"，保证了送到小顶堆的一定是大的那部分。
- Step3 维护 size 平衡。最终效果：maxHeap 始终存储较小的 ⌈n/2⌉ 个数，minHeap 存储较大的 ⌊n/2⌋ 个数。

```
// 模拟插入过程: addNum(1), addNum(2), addNum(3)
//
// === addNum(1) ===
// Step1: maxHeap=[1]           minHeap=[]
// Step2: maxHeap=[]            minHeap=[1]
// Step3: maxHeap.size < minHeap.size → 搬回
//        maxHeap=[1]           minHeap=[]
// findMedian → 奇数 → maxHeap.top() = 1
//
// === addNum(2) ===
// Step1: maxHeap=[2,1]         minHeap=[]
// Step2: maxHeap=[1]           minHeap=[2]
// Step3: size 相等，不搬
// findMedian → 偶数 → (1+2)/2 = 1.5
//
// === addNum(3) ===
// Step1: maxHeap=[3,1]         minHeap=[2]
// Step2: maxHeap=[1]           minHeap=[2,3]
// Step3: minHeap.size > maxHeap.size → 搬回
//        maxHeap=[2,1]         minHeap=[3]
// findMedian → 奇数 → maxHeap.top() = 2  ✓
```

---

### 解法3: multiset + 双迭代器 — O(log n) / O(1)

**不同的思路**：用平衡 BST（C++ 的 `multiset`）维护所有元素的有序性。关键技巧：维护一个指向中位数位置的迭代器，每次插入后只需要微调迭代器。

```cpp
class MedianFinder {
    multiset<int> data;
    multiset<int>::iterator lo, hi;
    
public:
    MedianFinder() : lo(data.end()), hi(data.end()) {}
    
    void addNum(int num) {
        int n = data.size();
        data.insert(num);
        
        if (n == 0) {
            // 第一个元素，lo 和 hi 都指向它
            lo = hi = data.begin();
        } else if (n % 2 == 1) {
            // 插入前是奇数个 (lo == hi)，插入后偶数个
            // 需要 lo 和 hi 分开，分别指向中间两个
            if (num < *lo) {
                lo--;       // 新数在左半边，lo 左移
            } else {
                hi++;       // 新数在右半边，hi 右移
            }
        } else {
            // 插入前是偶数个 (lo + 1 == hi)，插入后奇数个
            // 需要 lo == hi
            if (num > *lo && num < *hi) {
                lo++;       // 新数恰好在 lo 和 hi 之间
                hi--;
            } else if (num >= *hi) {
                lo++;       // 新数在右半边，lo 右移跟上
            } else {
                // num <= *lo
                hi--;       // 新数在左半边，hi 左移
                lo = hi;    // 偶数→奇数，lo==hi
            }
        }
    }
    
    double findMedian() {
        return (*lo + *hi) / 2.0;
    }
};
```

**注意**：这个解法虽然复杂度相同，但实现复杂，面试时不推荐。了解即可。

---

### 解法4: 计数排序 (Follow-up) — O(1) addNum / O(100) findMedian

**Follow-up 问题**：如果数据范围是 [0, 100]，怎么优化？

**思路**：数据范围极小，用一个大小为 101 的频次数组。找中位数时线性扫描频次数组，累加计数直到达到中间位置。

```cpp
class MedianFinder {
    int count[101] = {};    // count[i] = 值 i 出现的次数
    int total = 0;          // 总元素数
    
public:
    MedianFinder() {}
    
    void addNum(int num) {
        count[num]++;
        total++;
    }
    
    double findMedian() {
        // 找第 mid1 个和第 mid2 个数 (1-indexed)
        int mid1 = (total + 1) / 2;
        int mid2 = (total + 2) / 2;
        int val1 = -1, val2 = -1;
        int cumSum = 0;
        
        for (int i = 0; i <= 100; i++) {
            cumSum += count[i];
            if (val1 == -1 && cumSum >= mid1) val1 = i;
            if (val2 == -1 && cumSum >= mid2) val2 = i;
            if (val1 != -1 && val2 != -1) break;
        }
        return (val1 + val2) / 2.0;
    }
};
```

**为什么这比堆更好（在 Follow-up 场景下）？**
- addNum: O(1) vs O(log n)
- findMedian: O(100) = O(1) 常数级 vs O(1)
- 空间: O(101) 固定 vs O(n)

---

## 解法对比

| | 有序数组 | 双堆 ⭐ | multiset | 计数排序 |
|---|---|---|---|---|
| addNum | O(n) | O(log n) | O(log n) | O(1) |
| findMedian | O(1) | O(1) | O(1) | O(range) |
| 实现难度 | 简单 | 中等 | 较难 | 简单 |
| 适用场景 | n 很小 | 通用 | 通用 | 数据范围小 |
| 面试推荐 | 暴力讲解用 | ✅ 首选 | 了解即可 | Follow-up |

**核心区别**：双堆利用了"只需要中间值，不需要全局有序"的特性，用两个堆各持有一半数据，让中位数始终在堆顶暴露出来。

## 易错点

1. **整数溢出**：
   - ✗ `return (maxHeap.top() + minHeap.top()) / 2;` → 整数除法，丢失小数部分
   - ✓ `return (maxHeap.top() + minHeap.top()) / 2.0;` → 浮点除法

2. **堆的 size 不平衡**：
   - ✗ 直接判断 `num < maxHeap.top()` 然后放到 maxHeap → 如果一直放一边，两堆严重不平衡
   - ✓ 使用"先放→挤出→调整"的三步策略，保证每次插入后两堆 size 差 ≤ 1

3. **空堆时取 top()**：
   - ✗ `maxHeap.top()` 在 maxHeap 为空时未定义行为
   - ✓ 题目保证 findMedian 前至少一个元素，但 addNum 内部的三步策略天然避免了空堆问题（Step1 先 push 再 pop）

4. **C++ priority_queue 方向搞反**：
   - `priority_queue<int>` 是大顶堆（默认）
   - `priority_queue<int, vector<int>, greater<int>>` 是小顶堆
   - 如果搞反，maxHeap 顶部不是最大值而是最小值，逻辑全错

5. **Follow-up 计数排序：找第几个元素搞错**：
   - ✗ 奇偶都找 `total/2` → 偶数时少找了一个
   - ✓ 统一找第 `(total+1)/2` 和第 `(total+2)/2` 个，取平均。奇数时两者相同，偶数时分别是中间两个。

## 面试追问

**Q1: 暴力解能过吗？瓶颈在哪？**
→ 有序数组插入 O(n)，5×10⁴ 次调用最坏 O(n²) ≈ 2.5×10⁹，不能过。瓶颈是每次插入都要移动大量元素。

**Q2: 双堆方案中，为什么不直接判断 num 该放哪个堆，而是用"先放大堆→挤出→调整"的三步？**
→ 直接判断需要处理空堆、等号、边界等多种情况，容易出错。三步策略利用了堆的自动排序能力，先让 num 和大顶堆的最大值"竞争"，胜出者（较大的）去小顶堆，天然保证了左半 ≤ 右半的不变量。

**Q3: 如果数据范围是 [0, 100]，怎么进一步优化？**
→ 用大小 101 的频次数组，addNum O(1)，findMedian 遍历频次数组找到中间位置 O(100)=O(1)。空间固定 O(101)，比堆更省且更快。

**Q4: 如果 99% 的整数在 [0, 100] 范围内，但有极少数超范围的怎么办？**
→ 仍用频次数组处理 [0, 100] 的部分，超范围的存入两个额外容器（比如两个 vector 或两个小堆），分别记录 <0 和 >100 的。找中位数时综合三部分的总数来定位。

## 相关题型

- **480. 滑动窗口中位数** — 复用双堆思路，但需要支持"删除不在堆顶的元素"，引入惰性删除(lazy deletion)技巧。
- **703. 数据流中的第 K 大元素** — 只需要一个小顶堆维护前 K 大，比本题简单，可以看作本题的简化版（K 固定为 n/2）。
- **346. 数据流中的移动平均值** — 同为数据流设计题，但只需队列+窗口求和，不需要堆。