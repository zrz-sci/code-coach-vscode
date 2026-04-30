# LCR 076. 数组中的第 K 个最大元素（同主站 215）

## 核心思路

给定一个未排序的整数数组 `nums` 和一个整数 `k`，返回数组中**第 k 个最大的元素**。注意是排序后第 k 大，不是第 k 个不同的元素。

本质上就是一个**部分排序 / 选择问题**：不需要完全排序，只需要找到排名第 k 的元素。

## 思维链

1. **读完题第一反应** → 最直接：排序后取第 k 个。`sort` 一下，返回 `nums[n-k]`（升序）或 `nums[k-1]`（降序）。
2. **暴力能过吗？** → 排序是 O(n log n)，对大多数约束都够用，但面试官一定会问"能不能更快？"
3. **部分排序** → 我们不需要全局有序，只需要知道第 k 大的是谁。这自然联想到两个经典技巧：
   - **堆（优先队列）**：维护一个大小为 k 的最小堆，堆顶就是第 k 大。
   - **快速选择（Quick Select）**：类似快排的 partition，每次确定 pivot 的位置，只递归一侧。
4. **堆的思路** → 用大小为 k 的最小堆扫描数组，堆顶始终是"当前见过的第 k 大元素"。遍历完后堆顶就是答案。
5. **快速选择的思路** → partition 后 pivot 落在位置 p：
   - 如果 `p == n-k`，pivot 就是答案。
   - 如果 `p < n-k`，答案在右半边，递归右侧。
   - 如果 `p > n-k`，答案在左半边，递归左侧。
6. **快速选择最坏情况** → 如果每次 pivot 选到最小/最大值，退化为 O(n^2)。解决方案：**随机化 pivot** 或**三数取中**。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 排序 | 全局排序后取第k个 | O(n log n) | O(log n) | 能说出即可 |
| 最小堆 | 维护大小k的堆 | O(n log k) | O(k) | ⭐ 稳定可靠 |
| 快速选择 | 类快排 partition | O(n) 平均 | O(1) | ⭐⭐ 最优解 |

## 关键提示

1. **"第 k 大" vs "第 k 小"**：第 k 大等价于升序排列后下标 `n-k` 的元素。想清楚这个转换，代码会更自然。
2. **最小堆还是最大堆？** 找第 k 大用**最小堆**（大小 k）。堆顶是堆内最小值 = 堆内 k 个元素中的最小值 = 全局第 k 大。如果用最大堆需要大小 n-k+1，不如最小堆直观。
3. **C++ 的 `priority_queue`** 默认是最大堆。最小堆要写 `priority_queue<int, vector<int>, greater<int>>`。
4. **快速选择的 partition 边界** 是最容易写错的地方。推荐用 Lomuto 方案（单指针从左到右扫描），不易出错。
5. **随机化至关重要**：不随机化的快速选择在已排序数组上退化为 O(n^2)。`swap(nums[rand() % (right - left + 1) + left], nums[right])` 一行搞定。

## 解法详解

### 解法1: 排序 — O(n log n) / O(log n)

**思考过程**: 最直接的做法，排序后第 k 大就是 `nums[n-k]`。

```cpp
class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        sort(nums.begin(), nums.end());
        return nums[nums.size() - k];
    }
};
```

**关键点**:
- 时间 O(n log n)，空间 O(log n)（排序栈空间）。
- 面试中不够优秀，但可以作为起点："我先排序 O(n log n)，但我们可以用堆做到 O(n log k)，甚至快速选择做到平均 O(n)。"

---

### 解法2: 最小堆（大小 k） — O(n log k) / O(k)

**从排序优化**: 不需要全局排序，只需要维护"当前最大的 k 个数"。大小为 k 的最小堆完美胜任。

```
示例: nums = [3,2,1,5,6,4], k = 2

维护大小为2的最小堆:
加入3: 堆 = [3]
加入2: 堆 = [2, 3]       (堆满，大小=k=2)
加入1: 1 < 堆顶2 → 跳过  (比堆顶小，不可能是前k大)
加入5: 5 > 堆顶2 → 弹出2, 加入5 → 堆 = [3, 5]
加入6: 6 > 堆顶3 → 弹出3, 加入6 → 堆 = [5, 6]
加入4: 4 < 堆顶5 → 跳过

最终堆顶 = 5，即第2大元素 ✓
```

```cpp
class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        // 最小堆，大小保持为 k
        priority_queue<int, vector<int>, greater<int>> minHeap;
        for (int num : nums) {
            minHeap.push(num);
            if ((int)minHeap.size() > k) {
                minHeap.pop(); // 弹出最小的，保留最大的 k 个
            }
        }
        return minHeap.top(); // 堆顶 = 第 k 大
    }
};
```

**关键点**:
- 堆的大小始终 <= k，所以 push/pop 是 O(log k)，总共 n 次操作 → O(n log k)。
- 当 k 远小于 n 时（比如 k=10, n=10^6），O(n log k) 远优于 O(n log n)。
- 最小堆保证堆顶是堆内最小值 = 全局第 k 大。

---

### 解法3: 快速选择（Quick Select） — O(n) 平均 / O(1) ⭐

**核心思想**: 快排的 partition 每次把 pivot 放到正确位置，pivot 左边都 <= 它，右边都 >= 它。如果 pivot 恰好落在目标位置 `n-k`，我们就找到了答案。否则只需递归 pivot 的一侧，不需要两侧都排。

```
示例: nums = [3,2,1,5,6,4], k = 2, 目标位置 = 6-2 = 4

第一轮 partition (pivot=4):
  [3,2,1,4,6,5]  pivot 在位置3
         ↑
  3 < 4 (目标), 答案在右侧 → 只看 [6,5]

第二轮 partition (pivot=5):
  [..., 5, 6]  pivot 在位置4
       ↑
  4 == 4 (目标) → 返回 5 ✓
```

```cpp
class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        int target = nums.size() - k; // 转换为"第 target 小"
        return quickSelect(nums, 0, nums.size() - 1, target);
    }
    
private:
    int quickSelect(vector<int>& nums, int left, int right, int target) {
        if (left == right) return nums[left];
        
        // 随机选择 pivot，避免最坏情况
        int pivotIdx = left + rand() % (right - left + 1);
        swap(nums[pivotIdx], nums[right]);
        
        int pivotVal = nums[right];
        int storeIdx = left;
        for (int i = left; i < right; i++) {
            if (nums[i] <= pivotVal) {
                swap(nums[i], nums[storeIdx]);
                storeIdx++;
            }
        }
        swap(nums[storeIdx], nums[right]);
        
        if (storeIdx == target) return nums[storeIdx];
        else if (storeIdx < target) return quickSelect(nums, storeIdx + 1, right, target);
        else return quickSelect(nums, left, storeIdx - 1, target);
    }
};
```

**关键点**:
- 平均时间 O(n)：每轮排除一半 → n + n/2 + n/4 + ... = 2n = O(n)。
- 最坏 O(n^2)：每轮只排除1个元素。随机化 pivot 使最坏情况概率极低。
- **原地操作**，空间 O(1)（递归深度平均 O(log n)，可改为迭代消除栈开销）。

---

### 解法3b: 快速选择（迭代版） — 消除递归栈

```cpp
class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        int target = nums.size() - k;
        int left = 0, right = nums.size() - 1;
        while (left < right) {
            int pivotIdx = left + rand() % (right - left + 1);
            swap(nums[pivotIdx], nums[right]);
            int pivotVal = nums[right], storeIdx = left;
            for (int i = left; i < right; i++) {
                if (nums[i] <= pivotVal) swap(nums[i], nums[storeIdx++]);
            }
            swap(nums[storeIdx], nums[right]);
            if (storeIdx == target) return nums[storeIdx];
            else if (storeIdx < target) left = storeIdx + 1;
            else right = storeIdx - 1;
        }
        return nums[left];
    }
};
```

---

### 解法4: STL nth_element — 一行解法

```cpp
class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        nth_element(nums.begin(), nums.begin() + k - 1, nums.end(), greater<int>());
        return nums[k - 1];
    }
};
```

**关键点**:
- `nth_element` 内部就是快速选择（Introselect），平均 O(n)。
- 面试中最好能写出手动实现，`nth_element` 作为补充提及。

## 解法对比

|  | 排序 | 最小堆 | 快速选择 | nth_element |
|--|------|--------|---------|-------------|
| 时间 | O(n log n) | O(n log k) | **O(n) 平均** | O(n) 平均 |
| 空间 | O(log n) | O(k) | **O(1)** | O(1) |
| 最坏时间 | O(n log n) | O(n log k) | O(n^2) | O(n) |
| 稳定性 | 稳定 | 稳定 | 依赖随机化 | 内部 introselect |
| 修改原数组 | 是 | 否 | 是 | 是 |
| 面试推荐 | 起点 | k 远小于 n 时 | 最优解 | 提及即可 |

**面试策略**: 先提排序 O(n log n)，然后写堆 O(n log k)，最后讲快速选择 O(n)。如果时间允许，写出快速选择的代码。

## 易错点

1. **"第 k 大"转换错误**：
   - ✗ `nums[k]` 或 `nums[k-1]`（升序排列后）
   - ✓ `nums[n-k]`（升序排列后）

2. **最小堆 vs 最大堆搞混**：
   - ✗ 用最大堆找第 k 大 → 需要弹出 k-1 次堆顶才得到答案，O(n + k log n)
   - ✓ 用最小堆，大小 k，堆顶就是答案

3. **快速选择忘记随机化**：
   - ✗ 总是选最后一个元素做 pivot → 对已排序数组 O(n^2)
   - ✓ `swap(nums[rand() % (right-left+1) + left], nums[right])`

4. **Partition 边界条件**：
   - ✗ `for (int i = left; i <= right; ...)` → 把 pivot 也交换了
   - ✓ `for (int i = left; i < right; ...)` → pivot 放在 right 位置，不参与循环

5. **C++ priority_queue 默认是最大堆**：
   - ✗ `priority_queue<int> pq;` → 这是最大堆
   - ✓ `priority_queue<int, vector<int>, greater<int>> pq;` → 最小堆

## 面试追问

**Q1: 排序解法和堆解法的时间复杂度对比？什么时候堆更好？**
> 排序 O(n log n)，堆 O(n log k)。当 k << n 时（比如 k=10, n=10^6），log k ~ 3 vs log n ~ 20，堆快 6-7 倍。当 k ~ n 时两者差不多。

**Q2: 快速选择为什么是 O(n) 而不是 O(n log n)？**
> 快排需要两侧都递归，总工作量 O(n log n)。快速选择每轮只递归一侧（类似二分），工作量 n + n/2 + n/4 + ... = 2n = O(n)。

**Q3: 快速选择能不能保证 O(n)？**
> 可以，用 Median of Medians 算法选 pivot，保证每轮至少排除 30% 的元素，最坏 O(n)。但常数大，实际不如随机化快。面试中提到即可。

**Q4: 如果数据流不断到来（在线场景），怎么维持第 k 大？**
> 用大小为 k 的最小堆。每来一个数，和堆顶比较：如果 > 堆顶，弹出堆顶再插入新数，堆顶就是新的第 k 大。每次操作 O(log k)。

**Q5: 如果 k 值变化怎么办？**
> 可以维护一个有序数据结构如平衡 BST（`std::multiset`），支持 O(log n) 插入和 O(log n) 查找第 k 大（通过迭代器偏移）。

## 相关题型

- **347. 前 K 个高频元素** — 堆的经典应用，维护大小为 k 的最小堆存频率最高的 k 个元素。
- **703. 数据流中的第 K 大元素** — 在线版本，用大小为 k 的最小堆，每次插入 O(log k)。
- **973. 最接近原点的 K 个点** — 同样用大小为 k 的最大堆（按距离），或快速选择。
- **23. 合并 K 个升序链表** — 堆的另一个经典场景，维护大小为 k 的最小堆存每个链表的当前头节点。
