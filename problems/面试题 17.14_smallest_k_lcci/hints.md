# 面试题 17.14. 最小K个数 (Smallest K LCCI)

## 核心思路

这道题是经典的 **Top K 问题**：从无序数组中找到最小的 k 个元素。它与 LC 215（第 K 大元素）互为镜像，但方向相反——215 找第 k 大，本题找前 k 小。核心挑战是如何避免完全排序，以低于 O(n log n) 的代价完成任务。

## 思维链

1. **读完题第一反应** → 排序后取前 k 个，`sort` + `slice`，O(n log n)。简单粗暴，面试中可以作为起点。
2. **能不能不完全排序？** → 我不需要整个有序数组，只需要"最小的 k 个"是谁（且不要求有序），排序做了太多无用功。
3. **维护"最小 k 个"** → 遍历数组时，用一个大小为 k 的**大顶堆**来维护"当前见过的最小 k 个数"。堆顶是这 k 个数中最大的"门槛"——新来的数如果比堆顶小，就把堆顶替换掉。这样 O(n log k)。
4. **能否做到 O(n)？** → 快速排序的 partition 每次能确定一个元素的最终位置。如果刚好把数组划分为"前 k 小"和"后 n-k 大"，就搞定了！只需在一侧继续划分，平均 O(n)。
5. **QuickSelect 最坏 O(n^2) 怎么办？** → 随机选 pivot，平均 O(n)。面试中说明随机化即可。
6. **题目说"以任意顺序返回"** → 这是暗示不需要排序输出，QuickSelect 特别合适——partition 之后左侧全是答案，直接截取。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 排序 | sort 后取前 k 个 | O(n log n) | O(log n) | 能说出即可 |
| 大顶堆 | 维护大小为 k 的堆 | O(n log k) | O(k) | ⭐ 必须写出 |
| QuickSelect | partition 定位第 k 小 | O(n) 平均 | O(1) | ⭐ 必须写出 |

## 关键提示

1. **堆的方向选择**：要最小 k 个，用**大顶堆**（大小为 k），堆顶是"门槛"——当前第 k 小元素。新数比堆顶小才能入堆。为什么不用小顶堆？小顶堆弹出最小值，留下的不是最小的 k 个。
2. **QuickSelect 的目标**：partition 之后，如果 pivot 恰好在索引 k-1，那么 `nums[0..k-1]` 就是答案。不需要它们有序，直接返回。
3. **随机化是关键**：不随机选 pivot，有序数组退化到 O(n^2)。`swap(arr[left], arr[rand() % (right-left+1) + left])` 一行搞定。
4. **边界条件**：k=0 时返回空数组；k=n 时返回整个数组。面试中主动提出这些边界检查。
5. **与 LC 215 的关系**：215 找第 k 大 = 第 (n-k+1) 小。本题找前 k 小。两者的 QuickSelect 和堆思路完全一样，只是堆的方向和比较方向相反。

## 解法详解

### 解法1: 排序 — O(n log n) / O(log n)

**思考过程**: 最直觉的做法。排序后前 k 个元素就是答案。面试中作为起点快速说出，然后主动优化。

```cpp
class Solution {
public:
    vector<int> smallestK(vector<int>& arr, int k) {
        sort(arr.begin(), arr.end());
        // 取排序后的前 k 个
        return vector<int>(arr.begin(), arr.begin() + k);
    }
};
```

**关键点**: 时间 O(n log n) 不是最优。但代码极简，面试中可以先写这个"保底"，然后说"这样可以工作，但我来优化到 O(n log k) 或 O(n)"。

---

### 解法2: 大顶堆 — O(n log k) / O(k) ⭐ 面试首选之一

**从解法1优化**: 排序给出了所有元素的顺序，但我只关心"最小的 k 个"。用一个大小为 k 的大顶堆，堆顶是"当前最小 k 个数中最大的"——充当入场门槛。

```
// 示例: arr = [1,3,5,7,2,4,6,8], k = 4
//
// 遍历过程（大顶堆，容量 k=4）:
// 处理 1: 堆 = [1]                 (size < k, 直接加)
// 处理 3: 堆 = [3,1]              (size < k, 直接加)
// 处理 5: 堆 = [5,3,1]            (size < k, 直接加)
// 处理 7: 堆 = [7,5,3,1]          (size == k, 堆顶=7)
// 处理 2: 2 < 堆顶7 → 弹7压2      堆 = [5,3,2,1]
// 处理 4: 4 < 堆顶5 → 弹5压4      堆 = [4,3,2,1]
// 处理 6: 6 > 堆顶4 → 跳过        堆 = [4,3,2,1]
// 处理 8: 8 > 堆顶4 → 跳过        堆 = [4,3,2,1]
// 堆中所有元素 = [1,2,3,4] → 答案!
```

```cpp
class Solution {
public:
    vector<int> smallestK(vector<int>& arr, int k) {
        if (k == 0) return {};
        
        // 大顶堆：堆顶是最大值，充当"入场门槛"
        priority_queue<int> maxHeap;
        
        for (int num : arr) {
            maxHeap.push(num);
            // 堆大小超过 k 时弹出最大值
            // 这样堆里始终保留当前见过的最小的 k 个数
            if ((int)maxHeap.size() > k) {
                maxHeap.pop();
            }
        }
        
        // 把堆中元素倒出来
        vector<int> result;
        while (!maxHeap.empty()) {
            result.push_back(maxHeap.top());
            maxHeap.pop();
        }
        return result;
    }
};
```

**关键点**: 
- 为什么用**大顶堆**而不是小顶堆？小顶堆弹出最小值，留下的是大的。大顶堆弹出最大值，保证留下的都是小的。
- 堆大小始终 <= k，所以每次 push/pop 是 O(log k)，总计 O(n log k)。
- 当 k 很小时（如 k=10），O(n log 10) 接近 O(n)，非常高效。

---

### 解法3: QuickSelect — O(n) 平均 / O(1) ⭐ 面试首选

**核心洞察**: 快速排序的 `partition` 操作能确定 pivot 的最终位置。如果 pivot 在索引 k-1，那么 `arr[0..k-1]` 就是最小的 k 个数（虽然内部可能无序）。如果不在，只需在一侧继续搜索。

```
// QuickSelect 过程示意 (找最小4个):
// arr = [1,3,5,7,2,4,6,8], k = 4
//
// Round1: pivot=4(随机), partition后: [1,3,2] 4 [5,7,6,8]
//         pivot在索引3, 目标索引3 (k-1=3) → 命中!
//         arr[0..3] = [1,3,2,4] 就是最小的4个数!
//
// 如果 pivot 没命中:
// pivot在索引2, 目标k-1=3 > 2 → 在右半中继续找
// pivot在索引5, 目标k-1=3 < 5 → 在左半中继续找
```

```cpp
class Solution {
public:
    vector<int> smallestK(vector<int>& arr, int k) {
        if (k == 0 || arr.empty()) return {};
        if (k >= (int)arr.size()) return arr;
        
        // 用 QuickSelect 让 arr[0..k-1] 恰好是最小的 k 个
        quickSelect(arr, 0, arr.size() - 1, k - 1);
        
        return vector<int>(arr.begin(), arr.begin() + k);
    }

private:
    void quickSelect(vector<int>& arr, int left, int right, int targetIdx) {
        while (left < right) {
            int pivotIdx = partition(arr, left, right);
            
            if (pivotIdx == targetIdx) {
                return;  // pivot 恰好在目标位置
            } else if (pivotIdx < targetIdx) {
                left = pivotIdx + 1;   // 目标在右半
            } else {
                right = pivotIdx - 1;  // 目标在左半
            }
        }
    }
    
    int partition(vector<int>& arr, int left, int right) {
        // 随机选 pivot 避免最坏情况
        int randomIdx = left + rand() % (right - left + 1);
        swap(arr[randomIdx], arr[right]);
        
        int pivot = arr[right];
        int storeIdx = left;
        
        for (int i = left; i < right; i++) {
            if (arr[i] <= pivot) {
                swap(arr[storeIdx], arr[i]);
                storeIdx++;
            }
        }
        swap(arr[storeIdx], arr[right]);
        return storeIdx;
    }
};
```

**关键点**:
- 与快速排序的区别：快排两侧都递归 → O(n log n)，QuickSelect 只递归一侧 → 平均 O(n)（n + n/2 + n/4 + ... = 2n）。
- QuickSelect 结束后，`arr[0..k-1]` 不一定有序，但题目说"以任意顺序返回"，完全可以。
- 随机化 pivot 是必须的，否则有序数组退化为 O(n^2)。

## 解法对比

| | 排序 | 大顶堆 | QuickSelect |
|---|---|---|---|
| 时间 | O(n log n) | O(n log k) | O(n) 平均 |
| 空间 | O(log n) | O(k) | O(1) |
| 最坏时间 | O(n log n) | O(n log k) | O(n^2) |
| 修改原数组 | 是 | 否 | 是 |
| 输出有序 | 是 | 否（堆序） | 否 |
| 适用场景 | 快速实现 | 不能修改数组/流数据 | 允许修改且追求 O(n) |

**面试建议**: 先说堆解法（代码短、好写、不出错、不改原数组），再提 QuickSelect（展示算法功底），主动说明 QuickSelect 最坏情况和随机化的重要性。

## 易错点

1. **堆的方向选反**
   - ✗ 用小顶堆（`priority_queue<int, vector<int>, greater<int>>`）→ 弹出最小值，留下的是大的
   - ✓ 用大顶堆（`priority_queue<int>`）→ 弹出最大值，留下的是最小的 k 个

2. **QuickSelect 的 partition 用 `<` 还是 `<=`**
   - ✗ `if (arr[i] < pivot)` → 当有大量重复元素时，等于 pivot 的元素全在右侧，分割不均
   - ✓ `if (arr[i] <= pivot)` → 等于 pivot 的元素可以分散在两侧

3. **忘记处理 k=0 或 k >= n 的边界**
   - ✗ 不检查直接 `quickSelect(arr, 0, n-1, k-1)` → k=0 时 targetIdx=-1 导致越界
   - ✓ 开头加 `if (k == 0) return {}; if (k >= n) return arr;`

4. **QuickSelect 忘记随机化 pivot**
   - ✗ 总是选 `arr[left]` 作 pivot → 输入 `[1,2,3,...,n]` 退化到 O(n^2)
   - ✓ `int randomIdx = left + rand() % (right - left + 1);`

5. **partition 的 storeIdx 初始化错误**
   - ✗ `storeIdx = 0` → 在子区间 [left, right] 上操作时越界
   - ✓ `storeIdx = left`

## 面试追问

**Q1: 暴力排序 O(n log n)，有什么不足？**
→ 排序确定了所有元素的相对顺序，但我们只需要"前 k 小"是谁，做了大量无用功。当 n 很大但 k 很小时尤其浪费。

**Q2: 如果数据是流式到来（一次来一个数），怎么随时维护最小 k 个？**
→ 用大顶堆。每来一个新元素，如果比堆顶小就替换堆顶然后调整。这保证堆里始终是当前最小的 k 个。时间每次 O(log k)。

**Q3: 如果不能修改原数组呢？**
→ QuickSelect 会修改数组，改用堆解法（不修改原数组）。或者先拷贝一份再做 QuickSelect，额外 O(n) 空间。

**Q4: QuickSelect 最坏 O(n^2)，如何保证 O(n)？**
→ 确定性做法是 BFPRT（Median of Medians）：将数组每 5 个分一组，取各组中位数，再递归取中位数的中位数作为 pivot，保证每次至少排除 3/10 的元素。面试中提到即可，一般不要求实现。

**Q5: 本题与 LC 215 有什么联系和区别？**
→ 215 找第 k 大（单个值），本题找前 k 小（k 个值）。QuickSelect 在 215 中是找到目标位置后返回该元素，在本题中是找到目标位置后返回左侧所有元素。堆的方向也相反：215 用小顶堆，本题用大顶堆。

## 相关题型

- [215. 数组中的第K个最大元素](../0215_kth_largest_element_in_an_array/) — 镜像题，找第 k 大而非前 k 小，QuickSelect 和堆都可复用，方向相反。
- [347. 前 K 个高频元素](../0347_top_k_frequent_elements/) — 复用 Top K 框架，区别是先用 HashMap 统计频率，再对频率做 Top K。
- [703. 数据流中的第 K 大元素](../0703_kth_largest_element_in_a_stream/) — 在线版本，用堆动态维护第 k 大。
- [973. 最接近原点的 K 个点](../0973_k_closest_points_to_origin/) — 复用 Top K 框架，比较的是距离而非元素值。
- [692. 前K个高频单词](../0692_top_k_frequent_words/) — Top K + 字符串频率排序，堆的比较器更复杂。
