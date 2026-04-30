# 215. 数组中的第K个最大元素

## 核心思路

这道题本质上是一个**选择问题（Selection Problem）**：从无序数组中找到排序后某个特定位置的元素，而不需要完全排序整个数组。

## 思维链

1. **读完题第一反应** → 排序后直接取第 k 个不就行了？`sort` 之后返回 `nums[n-k]`，O(n log n)。
2. **能不能不完全排序？** → 我不需要整个有序数组，只需要知道"第 k 大"是谁。排序做了太多无用功。
3. **换个思路：维护"前 k 大"** → 用一个大小为 k 的小顶堆，堆顶就是"前 k 大中最小的" = 第 k 大。这样 O(n log k)。
4. **能否做到 O(n)？** → 快速排序的 partition 每次能确定一个元素的最终位置。如果刚好是第 k 大的位置，就找到了！如果不是，只需在一侧继续找。这就是 **QuickSelect**。
5. **QuickSelect 最坏 O(n²) 怎么办？** → 随机选 pivot，平均 O(n)。面试中说明随机化即可。
6. **还有确定性 O(n) 吗？** → 有（BFPRT / Median of Medians），但面试一般不考，知道存在即可。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 排序 | sort 后取 nums[n-k] | O(n log n) | O(1)~O(log n) | 能说出即可 |
| 小顶堆 | 维护大小为 k 的堆 | O(n log k) | O(k) | ⭐ 必须写出 |
| QuickSelect | partition 定位 | O(n) 平均 | O(1) | ⭐ 必须写出 |
| BFPRT | 中位数的中位数选 pivot | O(n) 确定性 | O(n) | 加分项 |

## 关键提示

1. **第 k 大 = 第 (n-k) 小**：转换视角后，找的是排序数组中索引为 `n-k` 的元素。
2. **堆的方向选择**：要第 k 大，用**小顶堆**（大小为 k），堆顶就是答案。为什么不用大顶堆？因为大顶堆弹出的是最大值，没法直接定位第 k 大。
3. **QuickSelect 的 partition**：和快速排序的 partition 完全一样，区别是快排两侧都递归，QuickSelect 只递归一侧。
4. **随机化是关键**：如果不随机选 pivot，有序数组会退化到 O(n²)。`swap(nums[left], nums[rand() % (right-left+1) + left])` 一行搞定。
5. **Follow-up "Can you solve it without sorting?"**：暗示面试官期望堆或 QuickSelect 解法。

## 解法详解

### 解法1: 排序 — O(n log n) / O(log n)

**思考过程**: 最直觉的做法。第 k 大 = 排序后从后往前数第 k 个 = 索引 `n-k`。

```cpp
class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        sort(nums.begin(), nums.end());
        return nums[nums.size() - k];
    }
};
```

**关键点**: 时间 O(n log n) 不满足题目要求的 O(n)，但面试时作为起点说出来展示理解，然后主动优化。

---

### 解法2: 小顶堆 — O(n log k) / O(k) ⭐ 面试首选之一

**从解法1优化**: 排序给出了所有元素的顺序，但我只关心"前 k 大"。用一个大小为 k 的小顶堆，堆顶始终是"前 k 大中最小的"，即第 k 大元素。

```
// 示例: nums = [3,2,1,5,6,4], k = 2
// 
// 遍历过程（小顶堆，容量 k=2）:
// 处理 3: 堆 = [3]             (size < k, 直接加)
// 处理 2: 堆 = [2, 3]          (size == k)
// 处理 1: 1 < 堆顶2 → 跳过     堆 = [2, 3]
// 处理 5: 5 > 堆顶2 → 弹2压5   堆 = [3, 5]
// 处理 6: 6 > 堆顶3 → 弹3压6   堆 = [5, 6]
// 处理 4: 4 < 堆顶5 → 跳过     堆 = [5, 6]
// 堆顶 = 5 → 答案!
```

```cpp
class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        // 小顶堆：堆顶是最小值
        priority_queue<int, vector<int>, greater<int>> minHeap;
        
        for (int num : nums) {
            minHeap.push(num);
            // 堆大小超过 k 时弹出最小值
            // 这样堆里始终保留当前见过的最大的 k 个数
            if (minHeap.size() > k) {
                minHeap.pop();
            }
        }
        
        // 堆顶 = 前 k 大中最小的 = 第 k 大
        return minHeap.top();
    }
};
```

**关键点**: 
- 为什么用**小顶堆**而不是大顶堆？大顶堆弹出最大值，无法保留"前 k 大"。小顶堆弹出最小值，保证留下的都是大的。
- 堆大小始终 ≤ k，所以每次 push/pop 是 O(log k)，总计 O(n log k)。

---

### 解法3: QuickSelect（随机化） — O(n) 平均 / O(1) ⭐ 面试首选

**核心洞察**: 快速排序的 `partition` 操作能确定 pivot 的最终位置。如果 pivot 刚好落在第 k 大的位置，就找到了。否则只需搜索一侧——每次砍掉一半搜索空间。

```
// QuickSelect 过程示意 (找第2大 = 找索引 n-k = 4):
// nums = [3, 2, 1, 5, 6, 4]
//
// Round1: pivot=4(随机), partition后: [3,2,1] 4 [5,6]
//         pivot在索引3, 目标索引4 > 3 → 在右半[5,6]中找
//
// Round2: 在[5,6]中, pivot=6, partition后: [5] 6
//         pivot在索引5, 目标索引4 < 5 → 在左半[5]中找
//
// Round3: 只剩[5], 就是它! 返回 5
```

```cpp
class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        int n = nums.size();
        // 第 k 大 = 升序排列后索引 n-k 的元素
        int targetIdx = n - k;
        return quickSelect(nums, 0, n - 1, targetIdx);
    }
    
private:
    int quickSelect(vector<int>& nums, int left, int right, int targetIdx) {
        while (left < right) {
            int pivotIdx = partition(nums, left, right);
            
            if (pivotIdx == targetIdx) {
                return nums[pivotIdx];
            } else if (pivotIdx < targetIdx) {
                // pivot 位置太小，答案在右侧
                left = pivotIdx + 1;
            } else {
                // pivot 位置太大，答案在左侧
                right = pivotIdx - 1;
            }
        }
        // left == right，区间只剩一个元素
        return nums[left];
    }
    
    int partition(vector<int>& nums, int left, int right) {
        // 随机选 pivot 避免最坏情况
        int randomIdx = left + rand() % (right - left + 1);
        swap(nums[randomIdx], nums[right]);
        
        int pivot = nums[right];
        int storeIdx = left;  // storeIdx 左边的都 < pivot
        
        for (int i = left; i < right; i++) {
            if (nums[i] < pivot) {
                swap(nums[storeIdx], nums[i]);
                storeIdx++;
            }
        }
        // 把 pivot 放到最终位置
        swap(nums[storeIdx], nums[right]);
        return storeIdx;
    }
};
```

**关键点**:
- 与快速排序的区别：快排两侧都递归 → O(n log n)，QuickSelect 只递归一侧 → 平均 O(n)（n + n/2 + n/4 + ... ≈ 2n）。
- 随机化 pivot 是必须的，否则有序数组退化为 O(n²)。

---

### 解法4（补充）: 三路 partition 优化 — 处理大量重复元素

**从解法3优化**: 当数组有大量重复元素时，标准 partition 效率低。三路 partition 将数组分为 `< pivot`、`== pivot`、`> pivot` 三段，如果目标索引落在 `== pivot` 段则直接返回。

```cpp
class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        int n = nums.size();
        int targetIdx = n - k;
        int left = 0, right = n - 1;
        
        while (left <= right) {
            // 三路 partition
            int randomIdx = left + rand() % (right - left + 1);
            swap(nums[randomIdx], nums[left]);
            int pivot = nums[left];
            
            int lt = left;      // nums[left..lt-1] < pivot
            int gt = right;     // nums[gt+1..right] > pivot
            int i = left + 1;   // nums[lt..i-1] == pivot
            
            while (i <= gt) {
                if (nums[i] < pivot) {
                    swap(nums[i++], nums[lt++]);
                } else if (nums[i] > pivot) {
                    swap(nums[i], nums[gt--]);
                    // 注意 i 不动，因为换过来的数还没检查
                } else {
                    i++;
                }
            }
            // 现在 nums[lt..gt] 都等于 pivot
            
            if (targetIdx < lt) {
                right = lt - 1;
            } else if (targetIdx > gt) {
                left = gt + 1;
            } else {
                // targetIdx 落在 [lt, gt] 之间，pivot 就是答案
                return pivot;
            }
        }
        
        return nums[left];
    }
};
```

## 解法对比

| | 排序 | 小顶堆 | QuickSelect | 三路 QS |
|---|---|---|---|---|
| 时间 | O(n log n) | O(n log k) | O(n) 平均 | O(n) 平均 |
| 空间 | O(log n) | O(k) | O(1) | O(1) |
| 最坏时间 | O(n log n) | O(n log k) | O(n²) | O(n²) |
| 修改原数组 | 是 | 否 | 是 | 是 |
| 适用场景 | 快速实现 | 不能修改数组/流数据 | 允许修改且追求O(n) | 大量重复元素 |

**面试建议**: 先说堆解法（代码短、好写、不出错），再提 QuickSelect（展示算法功底），主动说明 QuickSelect 最坏情况和随机化的重要性。

## 易错点

1. **第 k 大 vs 第 k 小搞混**
   - ✗ `return nums[k-1]` （这是排序后第 k 小）
   - ✓ `return nums[n-k]` （第 k 大 = 从末尾数第 k 个 = 索引 n-k）

2. **堆的方向选反**
   - ✗ 用大顶堆保留 k 个 → 弹出的是最大值，留下的不是前 k 大
   - ✓ 用小顶堆保留 k 个 → 弹出的是最小值，留下的是最大的 k 个

3. **QuickSelect 忘记随机化**
   - ✗ 总是选 `nums[left]` 作 pivot → 输入 `[1,2,3,...,n]` 退化 O(n²)
   - ✓ `int randomIdx = left + rand() % (right - left + 1)`

4. **三路 partition 中 `nums[i] > pivot` 时 i++**
   - ✗ `swap(nums[i], nums[gt--]); i++;` → 换过来的数没检查就跳过了
   - ✓ `swap(nums[i], nums[gt--]);` → i 不动，下一轮重新检查

5. **partition 的 storeIdx 初始化错误**
   - ✗ `storeIdx = 0` → 没有从 left 开始，多段区间时越界
   - ✓ `storeIdx = left`

## 面试追问

**Q1: 暴力排序 O(n log n)，能通过吗？瓶颈在哪？**
→ 能通过大部分测试，但题目要求 O(n)。瓶颈在于排序给出了所有元素的相对顺序，而我们只需要一个位置的元素，做了大量无用功。

**Q2: 如果不能修改原数组呢？**
→ QuickSelect 会修改数组，此时用堆解法 O(n log k)，不修改原数组。或者先拷贝一份再 QuickSelect，额外 O(n) 空间。

**Q3: 如果是数据流（元素一个一个到来），怎么随时知道第 k 大？**
→ 维护一个大小为 k 的小顶堆。每来一个新元素，如果比堆顶大就替换堆顶。堆顶始终是第 k 大。这就是 LeetCode 703 的做法。

**Q4: QuickSelect 最坏 O(n²)，有确定性 O(n) 的算法吗？**
→ 有，BFPRT 算法（Median of Medians）。每次选 pivot 时把数组分成 5 个一组，取每组中位数，再取中位数的中位数作 pivot，保证最坏情况也是 O(n)。面试中提到即可，一般不要求实现。

## 相关题型

- **703. 数据流中的第 K 大元素** — 直接复用本题的小顶堆解法，区别：703 是在线问题，需要支持动态添加元素。
- **347. 前 K 个高频元素** — 复用小顶堆思路，区别：堆中比较的不是元素值而是频率，需要先用 HashMap 统计频率。
- **973. 最接近原点的 K 个点** — 复用 Top K 框架（堆或 QuickSelect），区别：比较的是距离而非元素值。
- **912. 排序数组** — QuickSelect 的 partition 逻辑和快速排序完全一样，区别：快排两侧都递归，QuickSelect 只递归一侧。
- **4. 寻找两个正序数组的中位数** — 也是找第 k 小问题，但输入是两个有序数组，需要不同的二分策略。