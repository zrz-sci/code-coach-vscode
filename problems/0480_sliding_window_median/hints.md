# 480. 滑动窗口中位数

## 核心思路

本质上在问：如何在一个动态集合中**高效地维护中位数**，这个集合每次滑动时会**删除一个旧元素、插入一个新元素**。

## 思维链

1. **读完题第一反应**：每次窗口滑动后，对窗口内 k 个元素排序取中位数 → 暴力解 O(nk·log k)。
2. **暴力解的瓶颈在哪？** 每次排序 O(k log k) 太贵。能否利用上一个窗口的排序结果？窗口只变了两个元素（删一个加一个），没必要重新排序。
3. **维护有序结构**：用一个始终有序的数据结构（如 `multiset`），插入 O(log k)，删除 O(log k)，取中位数用迭代器 → 总体 O(n log k)。
4. **另一个思路：对顶堆**：经典的"动态中位数"手段 — 用一个大顶堆存较小的一半，小顶堆存较大的一半。但堆不支持按值删除任意元素！
5. **惰性删除（Lazy Deletion）**：堆不能直接删中间元素，但可以先**记录要删的值**，等它浮到堆顶时再真正删除。用哈希表记录"欠删"的元素个数，维护一个逻辑上的有效大小。
6. **两种路径的对比**：multiset 写法简洁适合面试手写；对顶堆+惰性删除更贴近 295 题（Find Median from Data Stream）的扩展，面试追问概率高。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力排序 | 每次窗口排序取中位数 | O(nk log k) | O(k) | 能说出即可 |
| 有序集合 (multiset) | 维护有序结构 + 迭代器 | O(n log k) | O(k) | ⭐ 必须写出 |
| 对顶堆 + 惰性删除 | 大顶堆 + 小顶堆 + 延迟删除 | O(n log k) | O(n) | 加分项 |

## 关键提示

1. **中位数的本质**：中位数把集合分成"较小的一半"和"较大的一半"。如果能高效维护这个划分，中位数就是分界处的元素。
2. **multiset 的迭代器陷阱**：`erase(value)` 会删除所有等于该值的元素！必须用 `erase(iterator)` 只删一个。
3. **对顶堆的平衡条件**：始终保证大顶堆的 size 等于 `(k+1)/2`，小顶堆的 size 等于 `k/2`。每次插入/删除后要重新平衡。
4. **溢出风险**：两个 int 相加求平均数可能溢出 int 范围（`nums[i]` 可达 2^31-1）。用 `double` 或 `(long long)` 来计算。
5. **惰性删除的关键**：删除只标记不实际删除。只有当堆顶是待删元素时，才真正 pop。逻辑大小用 balance 变量追踪。

## 解法详解

### 解法1: 暴力排序 — O(nk log k) / O(k)

**思考过程**: 最直觉的做法 — 每个窗口提取 k 个元素，排序，取中间值。

```cpp
class Solution {
public:
    vector<double> medianSlidingWindow(vector<int>& nums, int k) {
        vector<double> result;
        for (int i = 0; i + k <= (int)nums.size(); i++) {
            vector<int> window(nums.begin() + i, nums.begin() + i + k);
            sort(window.begin(), window.end());
            // k 为奇数取中间，偶数取中间两个平均
            if (k % 2 == 1) {
                result.push_back(window[k / 2]);
            } else {
                // 用 double 避免 int 溢出
                result.push_back(((double)window[k / 2 - 1] + window[k / 2]) / 2.0);
            }
        }
        return result;
    }
};
```

**关键点**: 当 n=10^5, k=10^5 时，排序开销巨大。但这个解法清晰展示了题目要求。

---

### 解法2: 有序集合 (multiset + 迭代器) — O(n log k) / O(k) ⭐ 面试首选

**从解法1优化**: 每次窗口只变动 2 个元素，没必要重新排序。用 `multiset` 维护有序窗口，插入/删除 O(log k)，用迭代器指向中位数位置。

**核心技巧 — 迭代器的移动规则**：

```
// 窗口大小 k=5 时, multiset 内部有序:
// [-3, -1, 1, 3, 5]
//           ↑ mid (指向第 (k+1)/2 = 3 个元素, 即中位数)
//
// 插入新元素 num:
//   如果 num < *mid → mid 左边多了一个元素 → mid 应该左移 (--mid)
// 删除旧元素 out:
//   如果 out < *mid → mid 左边少了一个元素 → mid 应该右移 (++mid)
//   如果 out == *mid → 删的就是 mid 指向的元素 → 先把 mid 右移再删
```

```cpp
class Solution {
public:
    vector<double> medianSlidingWindow(vector<int>& nums, int k) {
        multiset<int> window(nums.begin(), nums.begin() + k);
        // mid 指向中位数（第 ⌈k/2⌉ 个元素，0-indexed 即第 (k-1)/2 个）
        auto mid = next(window.begin(), (k - 1) / 2);
        
        vector<double> result;
        for (int i = k; ; i++) {
            // 计算当前窗口中位数
            // k 奇数: *mid; k 偶数: (*mid + *next(mid)) / 2.0
            result.push_back(((double)(*mid) + *next(mid, k % 2 == 0)) / 2.0);
            
            if (i >= (int)nums.size()) break;
            
            // 插入新元素
            window.insert(nums[i]);
            // 新元素 < *mid，说明插在左边，mid 需要左移
            if (nums[i] < *mid) --mid;
            
            // 删除旧元素（窗口最左边的）
            // 旧元素 <= *mid，说明删的在左边或就是 mid，mid 需要右移
            if (nums[i - k] <= *mid) ++mid;
            window.erase(window.lower_bound(nums[i - k]));
        }
        return result;
    }
};
```

**关键点**:
- **先插入再删除**：保证 multiset 始终非空，mid 迭代器有效。
- `erase(lower_bound(val))` 只删除一个等于 val 的元素，不是全删。
- 中位数计算用 `double` 防溢出。
- mid 的移动必须在插入之后、删除对应位置之后各调整一次。

---

### 解法3: 对顶堆 + 惰性删除 — O(n log k) / O(n)

**从 295 题扩展**: [295. Find Median from Data Stream](https://leetcode.com/problems/find-median-from-data-stream/) 用对顶堆维护动态中位数，但那题只有 add 没有 remove。滑动窗口需要 remove，堆不支持按值删除，所以用**惰性删除**。

**对顶堆结构**:
```
//  大顶堆 maxHeap          小顶堆 minHeap
//  (存较小的一半)           (存较大的一半)
//  
//       3                      5
//      / \                    / \
//     1   2                  7   6
//
//  maxHeap.top() = 3       minHeap.top() = 5
//  中位数 = 3 (k奇数) 或 (3+5)/2 (k偶数)
//
//  约定: maxHeap.size() == (k+1)/2, minHeap.size() == k/2
//  (逻辑大小，不含被惰性删除的元素)
```

**惰性删除流程**:
```
// 要删除元素 val:
// 1. 在 delayed 哈希表中 delayed[val]++
// 2. 判断 val 属于哪个堆，更新 balance
// 3. 调用 prune(heap): 如果堆顶是待删元素，pop 并减少 delayed 计数
//    （只清理堆顶，中间的等它浮上来再清）
```

```cpp
class Solution {
public:
    vector<double> medianSlidingWindow(vector<int>& nums, int k) {
        // maxHeap: 较小的一半 (大顶堆)
        priority_queue<int> maxHeap;
        // minHeap: 较大的一半 (小顶堆)
        priority_queue<int, vector<int>, greater<int>> minHeap;
        unordered_map<int, int> delayed; // 惰性删除记录
        
        // 初始化：先排序前 k 个，较小一半进 maxHeap，较大一半进 minHeap
        vector<int> init(nums.begin(), nums.begin() + k);
        sort(init.begin(), init.end());
        for (int i = 0; i < (k + 1) / 2; i++) maxHeap.push(init[i]);
        for (int i = (k + 1) / 2; i < k; i++) minHeap.push(init[i]);
        
        // smallSize, bigSize 追踪逻辑上的有效大小
        int smallSize = (k + 1) / 2, bigSize = k / 2;
        
        auto getMedian = [&]() -> double {
            if (k & 1) return maxHeap.top();
            return ((double)maxHeap.top() + minHeap.top()) / 2.0;
        };
        
        // 清理堆顶的已删除元素
        auto prune = [&](auto& heap) {
            while (!heap.empty() && delayed.count(heap.top()) && delayed[heap.top()] > 0) {
                delayed[heap.top()]--;
                if (delayed[heap.top()] == 0) delayed.erase(heap.top());
                heap.pop();
            }
        };
        
        // 平衡两个堆，保证 smallSize == (k+1)/2
        auto rebalance = [&]() {
            if (smallSize > (k + 1) / 2) {
                // maxHeap 多了，移一个到 minHeap
                minHeap.push(maxHeap.top()); maxHeap.pop();
                smallSize--; bigSize++;
                prune(maxHeap);
            } else if (smallSize < (k + 1) / 2) {
                // minHeap 多了，移一个到 maxHeap
                maxHeap.push(minHeap.top()); minHeap.pop();
                bigSize--; smallSize++;
                prune(minHeap);
            }
        };
        
        vector<double> result;
        result.push_back(getMedian());
        
        for (int i = k; i < (int)nums.size(); i++) {
            int add = nums[i], del = nums[i - k];
            
            // 1. 删除 del（惰性标记）
            delayed[del]++;
            if (del <= maxHeap.top()) {
                smallSize--;
            } else {
                bigSize--;
            }
            // 如果 del 恰好在堆顶，立即清理
            prune(maxHeap);
            prune(minHeap);
            
            // 2. 插入 add
            if (!maxHeap.empty() && add <= maxHeap.top()) {
                maxHeap.push(add);
                smallSize++;
            } else {
                minHeap.push(add);
                bigSize++;
            }
            
            // 3. 平衡
            rebalance();
            prune(maxHeap);
            prune(minHeap);
            
            result.push_back(getMedian());
        }
        return result;
    }
};
```

**关键点**: 
- `delayed` 表只做标记，真正删除发生在 `prune` 中（堆顶检查时）。
- `smallSize`/`bigSize` 是逻辑大小，和堆的物理 `size()` 不同。
- rebalance 后必须再 prune，因为移动元素可能让新堆顶变成待删元素。

---

## 解法对比

| | 暴力排序 | multiset + 迭代器 | 对顶堆 + 惰性删除 |
|---|---|---|---|
| 时间 | O(nk log k) | O(n log k) | O(n log k) |
| 空间 | O(k) | O(k) | O(n)（delayed 可能积累） |
| 代码量 | 最少 | 中等 | 最多 |
| 面试推荐 | 暖场 | ⭐ 首选 | 被追问 295 扩展时用 |
| 核心难点 | 无 | 迭代器移动逻辑 | 惰性删除 + 平衡维护 |

**什么时候选哪个？**
- multiset 解法代码简洁、不易出错，面试首选。
- 对顶堆解法展示了更强的工程能力，且和 295 题形成系列，如果面试官追问"如果用堆怎么做？"就需要这个。

## 易错点

1. **multiset 的 erase 陷阱**:
   - ✗ `window.erase(nums[i-k])` → 删除所有等于该值的元素！
   - ✓ `window.erase(window.lower_bound(nums[i-k]))` → 只删一个

2. **int 溢出**:
   - ✗ `(*mid + *next(mid)) / 2.0` → 两个 INT_MAX 相加溢出
   - ✓ `((double)(*mid) + *next(mid)) / 2.0` → 先转 double

3. **迭代器调整顺序**（解法2）:
   - 必须**先插入、调整 mid，再删除、调整 mid**。如果先删除，可能导致 mid 失效（指向被删元素）。

4. **对顶堆的 prune 时机**:
   - ✗ 只在删除后 prune → rebalance 移动元素后堆顶可能是已删元素
   - ✓ 删除后 prune + rebalance 后再 prune

5. **k=1 边界**:
   - 此时没有 `*next(mid)` 可用（k 偶数逻辑不触发），确保公式 `*next(mid, k%2==0)` 在 k 奇数时等价于 `*mid`。

## 面试追问

**Q1: 暴力解的瓶颈在哪？能否利用上一个窗口的信息？**

> 每次重新排序 O(k log k)。窗口滑动只改变了 2 个元素（删一个加一个），可以用有序数据结构维护，插入删除 O(log k)。

**Q2: multiset 方案中，mid 迭代器为什么不会失效？**

> 因为 multiset 的插入/删除不会使其他迭代器失效（红黑树性质）。但我们必须在删除操作前把 mid 移开，避免 mid 恰好指向被删元素。代码中的做法是：如果要删的值 ≤ *mid，先 `++mid` 再删。

**Q3: 如果改成"滑动窗口第 K 大"怎么做？**

> 思路一样，multiset 用迭代器指向第 K 大的位置。对顶堆则调整两个堆的目标 size 比例。也可以用 [Order-Statistic Tree](https://en.wikipedia.org/wiki/Order-statistic_tree)（C++ 的 `__gnu_pbds::tree`）在 O(log k) 内按 rank 查询。

**Q4: 对顶堆方案的惰性删除最坏情况下空间是多少？如何优化？**

> 最坏 O(n)（所有元素都被惰性标记但没清理）。优化：在 prune 中更积极清理，但无法避免最坏情况。multiset 方案不存在这个问题。

## 相关题型

- **[295. Find Median from Data Stream](https://leetcode.com/problems/find-median-from-data-stream/)** — 对顶堆的基础版（只有 add 没有 remove）。本题 = 295 + 惰性删除 + 滑动窗口。
- **[239. Sliding Window Maximum](https://leetcode.com/problems/sliding-window-maximum/)** — 滑动窗口求最大值，用单调双端队列 O(n)。本题求中位数比求极值更难，因为中位数需要全局排序信息。
- **[4. Median of Two Sorted Arrays](https://leetcode.com/problems/median-of-two-sorted-arrays/)** — 另一个中位数题，但用二分而非滑动窗口。
- **[703. Kth Largest Element in a Stream](https://leetcode.com/problems/kth-largest-element-in-a-stream/)** — 复用对顶堆思路，只需一个小顶堆维护前 K 大。