# 315. 计算右侧小于当前元素的个数

## 核心思路

本质上是一个**逆序对计数**问题：对数组中每个元素，统计其右侧比它小的元素个数。暴力 O(n^2) 必超时，需要利用**归并排序在 merge 阶段天然统计逆序对**的性质，或者**从右往左扫描 + 树状数组/线段树**来高效计数。

核心洞察：归并排序的 merge 步骤在合并两个有序子数组时，左半部分的元素移到结果时，右半部分已经放入结果的元素个数就是"比它小且在它右侧"的个数。

## 思维链

1. **读完题第一反应** → 对每个 `nums[i]`，遍历 `i+1` 到 `n-1`，统计比它小的个数。O(n^2) 两层循环，n <= 10^5 会超时。
2. **暴力的瓶颈在哪？** → 对每个元素独立扫描右侧所有元素，没有利用"已经扫描过的信息"。能否在某种排序过程中"顺便"统计？
3. **归并排序的启示** → 归并排序的 merge 阶段：合并 `left[]` 和 `right[]` 两个有序数组时，如果 `left[i] > right[j]`，那么 `right[j]` 比 `left[i]` 小，且 `right[j]` 在原数组中位于 `left[i]` 的右侧。此时 `right` 中已放入结果的元素个数就是对 `left[i]` 的贡献。
4. **怎么追踪原始索引？** → 排序会打乱位置，需要用 `pair<int,int>` 或者单独的 `index[]` 数组来追踪每个元素的原始位置，这样在 merge 过程中统计出的计数可以正确写入 `counts[原始索引]`。
5. **另一个思路：树状数组** → 从右往左遍历数组，将每个元素"插入"到树状数组中。插入 `nums[i]` 之前，查询树状数组中已有的、值 < `nums[i]` 的元素个数。这利用了树状数组 O(log n) 的前缀和查询能力。
6. **离散化处理负数** → 树状数组的下标从 1 开始且为正整数。由于 `nums[i]` 范围是 `[-10^4, 10^4]`，需要将值域整体平移（+offset）使所有值变为正数。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 归并排序（带索引） | merge 阶段统计逆序对 | O(n log n) | O(n) | ⭐ 面试首选 |
| 树状数组 (BIT) | 从右往左扫描，BIT 统计前缀 | O(n log M) | O(M) | ⭐ 适合追问 |
| 线段树 | 类似 BIT 但更通用 | O(n log M) | O(M) | 加分项 |

> M = 值域范围 = 2 * 10^4

## 关键提示

1. **归并排序关键**：merge 时，当左半部分元素 `left[i]` 放入结果时，右半部分已经被放入结果的元素个数（即 `j - mid - 1`）就是"右侧比 `left[i]` 小的"贡献量。
2. **必须追踪原始索引**：排序过程中元素位置会改变，需要同步移动索引数组 `index[]`，确保统计结果写入正确的 `counts[原始位置]`。
3. **树状数组的离散化**：BIT 下标必须为正整数。`nums[i]` 的范围是 `[-10^4, 10^4]`，加一个偏移量 `offset = 10001` 使得所有值映射到 `[1, 20001]`。
4. **BIT 的 update 和 query**：`update(x)` 表示"值 x 出现了一次"，`query(x-1)` 表示"值严格小于 x 的元素有多少个"。从右往左遍历，先 query 再 update。
5. **归并排序 vs 树状数组**：归并排序不需要离散化、不受值域大小影响；树状数组代码更短但需要处理值域映射。面试中两种都能写是加分项。

## 解法详解

### 解法1: 归并排序（带索引追踪） — O(n log n) / O(n) ⭐ 面试首选

**思考过程**: 逆序对问题是归并排序的经典应用。在 merge 两个有序子数组时，对于左半部分的每个元素，统计右半部分中已经被放入结果的（即比它小的）元素个数。

```
示例: nums = [5, 2, 6, 1]
index =      [0, 1, 2, 3]

归并排序过程（简化展示）：
第一层: [5,2] 和 [6,1]
  merge [5] 和 [2]:
    2<5, 右边的2先放入 → 右半已放1个
    5放入时, counts[0] += 1 (2比5小且在右侧)
    结果: [2,5], index=[1,0]
  
  merge [6] 和 [1]:
    1<6, 右边的1先放入 → 右半已放1个
    6放入时, counts[2] += 1 (1比6小且在右侧)
    结果: [1,6], index=[3,2]

第二层: merge [2,5] 和 [1,6]:
    比较2和1: 1<2, 右边的1先放入 → 右半已放1个
    比较2和6: 2<6, 左边的2放入, counts[1] += 1 (右半已放入1个比2小的)
    比较5和6: 5<6, 左边的5放入, counts[0] += 1 (右半已放入1个比5小的)
    6放入

最终: counts = [2, 1, 1, 0] ✓
    (counts[0] = 1 + 1 = 2, 分两次 merge 累加)
```

```cpp
class Solution {
public:
    vector<int> countSmaller(vector<int>& nums) {
        int n = nums.size();
        vector<int> counts(n, 0);
        vector<int> index(n);
        iota(index.begin(), index.end(), 0); // index = [0,1,2,...,n-1]
        
        vector<int> tempNums(n), tempIndex(n); // merge 用的临时数组
        mergeSort(nums, index, counts, tempNums, tempIndex, 0, n - 1);
        return counts;
    }
    
private:
    void mergeSort(vector<int>& nums, vector<int>& index,
                   vector<int>& counts,
                   vector<int>& tempNums, vector<int>& tempIndex,
                   int left, int right) {
        if (left >= right) return;
        int mid = left + (right - left) / 2;
        mergeSort(nums, index, counts, tempNums, tempIndex, left, mid);
        mergeSort(nums, index, counts, tempNums, tempIndex, mid + 1, right);
        merge(nums, index, counts, tempNums, tempIndex, left, mid, right);
    }
    
    void merge(vector<int>& nums, vector<int>& index,
               vector<int>& counts,
               vector<int>& tempNums, vector<int>& tempIndex,
               int left, int mid, int right) {
        int i = left, j = mid + 1, k = left;
        while (i <= mid && j <= right) {
            if (nums[i] <= nums[j]) {
                // 左边元素 <= 右边元素, 放入左边的
                // j - (mid+1) = 右半已放入结果的个数, 它们都比 nums[i] 小
                counts[index[i]] += (j - mid - 1);
                tempNums[k] = nums[i];
                tempIndex[k] = index[i];
                i++;
            } else {
                tempNums[k] = nums[j];
                tempIndex[k] = index[j];
                j++;
            }
            k++;
        }
        // 左半剩余
        while (i <= mid) {
            counts[index[i]] += (j - mid - 1); // 右半已全部放入
            tempNums[k] = nums[i]; tempIndex[k] = index[i];
            i++; k++;
        }
        // 右半剩余
        while (j <= right) {
            tempNums[k] = nums[j]; tempIndex[k] = index[j];
            j++; k++;
        }
        for (int p = left; p <= right; p++) {
            nums[p] = tempNums[p];
            index[p] = tempIndex[p];
        }
    }
};
```

**关键点**:
- `index[]` 数组与 `nums[]` 同步排序，保证始终知道每个元素的**原始位置**。
- 当 `nums[i] <= nums[j]` 时，把 `i` 放入结果，此时 `j - (mid+1)` 就是右半部分中已经被放入结果的个数——它们都比 `nums[i]` 小且原本在 `nums[i]` 的右侧。
- `<=` 而非 `<` 的判断保证**相等元素**不计入（题目要求"严格小于"）。
- 临时数组 `tempNums`、`tempIndex` 提前分配避免每次 merge 都 new。

---

### 解法2: 树状数组 (Binary Indexed Tree) — O(n log M) / O(M) ⭐

**从不同角度思考**: 从右往左遍历数组，维护一个"已处理元素"的频率统计。对于当前元素 `nums[i]`，查询"已有多少个元素的值 < `nums[i]`"。树状数组天生支持 O(log n) 的前缀和查询。

```
示例: nums = [5, 2, 6, 1], offset = 10001, 映射后 = [10006, 10003, 10007, 10002]

从右往左:
i=3: val=10002, query(10001)=0 → counts[3]=0, update(10002)
i=2: val=10007, query(10006)=1 → counts[2]=1, update(10007)  (10002<10007)
i=1: val=10003, query(10002)=1 → counts[1]=1, update(10003)  (10002<10003)
i=0: val=10006, query(10005)=2 → counts[0]=2, update(10006)  (10002,10003<10006)

结果: [2, 1, 1, 0] ✓
```

```cpp
class Solution {
public:
    vector<int> countSmaller(vector<int>& nums) {
        int n = nums.size();
        int offset = 10001; // -10^4 → 1
        int size = 20002;
        vector<int> tree(size, 0);
        
        vector<int> counts(n);
        for (int i = n - 1; i >= 0; i--) {
            int val = nums[i] + offset;
            counts[i] = query(tree, val - 1); // 严格小于 val
            update(tree, val, size);
        }
        return counts;
    }
private:
    void update(vector<int>& tree, int i, int size) {
        for (; i < size; i += i & (-i)) tree[i]++;
    }
    int query(vector<int>& tree, int i) {
        int sum = 0;
        for (; i > 0; i -= i & (-i)) sum += tree[i];
        return sum;
    }
};
```

**关键点**:
- 树状数组下标从 **1** 开始，所以偏移量用 `10001`（不是 `10000`），确保最小值 `-10^4` 映射到 `1`。
- `query(val - 1)` 查询的是 **严格小于** `val` 的前缀和（不含等于）。
- `i & (-i)` 是 lowbit 操作，是树状数组的核心。

---

### 解法3: 线段树 — O(n log M) / O(M)

**与 BIT 类似的思路，更通用的数据结构**: 线段树可以实现区间查询和单点更新，这里用于统计某个值域范围内的元素个数。

```cpp
class Solution {
public:
    vector<int> countSmaller(vector<int>& nums) {
        int n = nums.size();
        int offset = 10001;
        int size = 20002;
        vector<int> seg(4 * size, 0);
        
        vector<int> counts(n);
        for (int i = n - 1; i >= 0; i--) {
            int val = nums[i] + offset;
            counts[i] = (val > 1) ? query(seg, 1, 1, size - 1, 1, val - 1) : 0;
            update(seg, 1, 1, size - 1, val);
        }
        return counts;
    }
private:
    void update(vector<int>& seg, int node, int lo, int hi, int pos) {
        if (lo == hi) { seg[node]++; return; }
        int mid = (lo + hi) / 2;
        if (pos <= mid) update(seg, 2*node, lo, mid, pos);
        else update(seg, 2*node+1, mid+1, hi, pos);
        seg[node] = seg[2*node] + seg[2*node+1];
    }
    int query(vector<int>& seg, int node, int lo, int hi, int l, int r) {
        if (l > r || lo > r || hi < l) return 0;
        if (l <= lo && hi <= r) return seg[node];
        int mid = (lo + hi) / 2;
        return query(seg, 2*node, lo, mid, l, r) + query(seg, 2*node+1, mid+1, hi, l, r);
    }
};
```

**关键点**:
- 线段树需要 4 倍空间，比树状数组空间开销大。
- 代码量也比树状数组多，但支持更多操作（如区间修改）。
- 面试中除非面试官要求，否则优先写归并排序或树状数组。

## 解法对比

|  | 归并排序 | 树状数组 | 线段树 |
|--|---------|---------|--------|
| 时间 | O(n log n) | O(n log M) | O(n log M) |
| 空间 | O(n) | O(M) | O(4M) |
| 值域依赖 | **无** | 有（需离散化或偏移） | 有 |
| 代码量 | 中等 | **最短** | 最长 |
| 扩展性 | 只适合逆序对类 | 多种前缀查询 | **最通用** |
| 面试推荐 | ⭐ 首选 | ⭐ 追问时讲 | 了解即可 |

**面试策略**: 先写归并排序解法，展示对分治和逆序对的理解。面试官追问"还有别的方法吗？"时讲树状数组。

## 易错点

1. **归并排序中 `<=` vs `<` 判断**：
   - ✗ `if (nums[i] < nums[j])` → 相等元素也会被计入 counts，导致结果偏大
   - ✓ `if (nums[i] <= nums[j])` → 相等时优先放左边，不计入（题目要求"严格小于"）

2. **忘记同步移动 index 数组**：
   - ✗ 只排序 `nums` 不移动 `index` → 统计结果写入错误位置
   - ✓ `tempNums` 和 `tempIndex` 必须同步操作

3. **树状数组偏移量算错**：
   - ✗ `offset = 10000` → `-10^4 + 10000 = 0`，BIT 下标不能为 0
   - ✓ `offset = 10001` → 最小值映射到 1

4. **树状数组查询 `query(val)` 而非 `query(val-1)`**：
   - ✗ `query(val)` → 包含了值等于 `nums[i]` 的元素，不是"严格小于"
   - ✓ `query(val - 1)` → 只统计严格小于的

5. **归并排序左半剩余元素忘记加 count**：
   - ✗ 只在主循环 `while (i <= mid && j <= right)` 中加了 counts
   - ✓ 左半剩余的处理中也要 `counts[index[i]] += (j - mid - 1)`

## 面试追问

**Q1: 这道题和"逆序对"有什么关系？**
> 逆序对 `(i, j)` 定义为 `i < j` 且 `nums[i] > nums[j]`。本题要求的 `counts[i]` 就是以 `i` 为左端点的逆序对个数。经典归并排序求逆序对总数是这道题的简化版（只要总数不要每个位置的）。

**Q2: 如果值域非常大（如 10^9），树状数组还能用吗？**
> 直接开 10^9 大小的数组不可行。需要**离散化**：先对所有值排序去重，然后把每个值映射到 `[1, n]` 的紧凑区间。离散化后树状数组大小只与 n 相关，不受原始值域影响。归并排序不需要离散化，这是它的优势。

**Q3: 能否用 BST（二叉搜索树）来解？**
> 可以。从右往左遍历，把每个元素插入 BST。插入时顺便统计比它小的节点个数（利用左子树大小）。但普通 BST 最坏 O(n) 退化为链表，需要用平衡 BST（如 AVL、红黑树）保证 O(log n)。C++ 中可以用 `__gnu_pbds` 的 `order_of_key()` 实现，但面试中不推荐。

## 相关题型

- **[剑指 Offer 51. 数组中的逆序对](../面试题%2051_shu_zu_zhong_de_ni_xu_dui_lcof/)** — 归并排序求逆序对总数，本题的简化版。直接复用解法1，把 `counts[index[i]] += ...` 改为全局计数器 `++` 即可。
- **[327. 区间和的个数](../0327_count_of_range_sum/)** — 用归并排序统计满足条件的前缀和对数，是本题思路的推广（从"比较单个元素大小"推广到"比较前缀和区间"）。
- **[493. 翻转对](../0493_reverse_pairs/)** — 定义 `i < j` 且 `nums[i] > 2 * nums[j]` 为翻转对。归并排序变体，merge 前先额外扫描一遍统计翻转对数，然后正常 merge。
- **[307. 区域和检索 - 数组可修改](../0307_range_sum_query_mutable/)** — 树状数组的经典应用，本题的树状数组解法可以直接迁移其 update/query 模板。
