# 912. 排序数组

## 核心思路
这道题本质上是**手写排序算法**——要求 O(n log n) 时间复杂度，不能用内置排序函数。核心考点是你能否正确实现归并排序、快速排序或堆排序，并理解它们为什么是 O(n log n)。

## 思维链

1. **读完题第一反应**：排序嘛，冒泡/选择/插入都能做，但它们是 O(n²)，题目要求 O(n log n)，pass。
2. **哪些排序算法是 O(n log n)?**：归并排序（稳定 O(n log n)）、快速排序（期望 O(n log n)）、堆排序（O(n log n) 原地）。
3. **归并排序**：分治思想，把数组一分为二，分别排好序再合并。合并操作是 O(n)，分 log n 层，所以总 O(n log n)。空间 O(n)。
4. **快速排序**：选一个 pivot，把小于它的放左边，大于的放右边，递归处理两边。最坏 O(n²)（已排序数组 + 固定选首元素），但通过**随机选 pivot** 可以规避。空间 O(log n)。
5. **堆排序**：建堆 O(n)，然后 n 次取堆顶 O(log n)，总 O(n log n)。原地排序，空间 O(1)。
6. **数据范围特殊**：nums[i] ∈ [-5×10⁴, 5×10⁴]，范围只有 10⁵，可以用**计数排序** O(n+k) 直接秒杀！

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 归并排序 | 分治：分成两半，分别排序，合并 | O(n log n) | O(n) | ⭐ 必须写出 |
| 快速排序 | 分治：随机 pivot 分区，递归两边 | O(n log n) 期望 | O(log n) | ⭐ 必须写出 |
| 堆排序 | 建大顶堆，逐个取出堆顶 | O(n log n) | O(1) | 能说出即可 |
| 计数排序 | 利用值域有限，计数后还原 | O(n + k) | O(k) | 加分项 |

## 关键提示

1. **归并排序的关键**：合并两个有序数组是核心操作。用两个指针分别指向两段的开头，比较后放入临时数组，最后拷贝回去。
2. **快速排序的陷阱**：如果 pivot 选不好（如固定选第一个），对已排序/近似排序数组会退化为 O(n²)。LeetCode 有专门的 test case 卡这个！必须**随机选 pivot**。
3. **堆排序需要理解 siftDown**：建堆从最后一个非叶节点开始向前 siftDown，而不是从前往后 siftUp（后者是 O(n log n) 建堆，前者是 O(n)）。
4. **计数排序适用条件**：值域有限时可用。本题 nums[i] ∈ [-5×10⁴, 5×10⁴]，范围只有 10⁵，完全可行。
5. **归并排序的分治图示**：
```
//  [5, 2, 3, 1]  — 分
//   /         \
// [5, 2]    [3, 1]  — 分
//  / \       / \
// [5] [2]  [3] [1]  — 基底
//  \ /       \ /
// [2, 5]    [1, 3]  — 合并
//   \         /
//  [1, 2, 3, 5]    — 合并
```

## 解法详解

### 解法1: 归并排序 — O(n log n) / O(n) ⭐ 面试首选

**思考过程**：分治法最经典的应用。把数组分成两半，假设两半各自已排好序（递归信仰），那我只需要把两个有序数组合并成一个。合并是 O(n) 的，递归树有 log n 层，所以总复杂度 O(n log n)。稳定、可靠，不怕最坏情况。

```cpp
class Solution {
public:
    vector<int> sortArray(vector<int>& nums) {
        vector<int> temp(nums.size()); // 预分配临时数组，避免递归中反复 new
        mergeSort(nums, temp, 0, nums.size() - 1);
        return nums;
    }
    
    void mergeSort(vector<int>& nums, vector<int>& temp, int left, int right) {
        if (left >= right) return; // 只剩一个元素，天然有序
        
        int mid = left + (right - left) / 2; // 防溢出的写法
        mergeSort(nums, temp, left, mid);
        mergeSort(nums, temp, mid + 1, right);
        merge(nums, temp, left, mid, right);
    }
    
    void merge(vector<int>& nums, vector<int>& temp, int left, int mid, int right) {
        // 先拷贝到临时数组
        for (int i = left; i <= right; i++) {
            temp[i] = nums[i];
        }
        
        int i = left, j = mid + 1; // 两个指针分别指向左右两段的开头
        for (int k = left; k <= right; k++) {
            if (i > mid) {              // 左半段已用完，直接取右半段
                nums[k] = temp[j++];
            } else if (j > right) {     // 右半段已用完，直接取左半段
                nums[k] = temp[i++];
            } else if (temp[i] <= temp[j]) { // 取较小者（<= 保证稳定性）
                nums[k] = temp[i++];
            } else {
                nums[k] = temp[j++];
            }
        }
    }
};
```

**关键点**：
- `temp` 数组在外面预分配一次，不要在每次 merge 里 new，否则常数巨大。
- `temp[i] <= temp[j]` 用 `<=` 而非 `<`，保证排序的**稳定性**。

---

### 解法2: 快速排序（随机 pivot）— O(n log n) 期望 / O(log n) ⭐ 面试首选

**从归并的另一个视角出发**：归并是"先分后合"，快排是"先整后分"——先把数组按 pivot 分成"小的一堆"和"大的一堆"（partition），再递归处理两边。关键在于 pivot 的选取决定划分是否均匀。

**为什么必须随机 pivot？**
```
// 如果固定选 nums[left] 作为 pivot:
// 输入 [1, 2, 3, 4, 5] (已排序)
// 每次 pivot = 最小值，左边0个元素，右边n-1个
// 递归深度 n 层，每层 O(n) → 总 O(n²)
// LeetCode 有这种 test case，不随机化必 TLE！
```

```cpp
class Solution {
public:
    vector<int> sortArray(vector<int>& nums) {
        srand(42); // 固定种子保证可复现
        quickSort(nums, 0, nums.size() - 1);
        return nums;
    }
    
    void quickSort(vector<int>& nums, int left, int right) {
        if (left >= right) return;
        
        // 随机选 pivot，交换到 left 位置
        int randomIdx = left + rand() % (right - left + 1);
        swap(nums[left], nums[randomIdx]);
        
        // 三路快排：处理大量重复元素的情况
        // 分区后：[left..lt-1] < pivot, [lt..gt] == pivot, [gt+1..right] > pivot
        int pivot = nums[left];
        int lt = left, gt = right, i = left + 1;
        
        while (i <= gt) {
            if (nums[i] < pivot) {
                swap(nums[i++], nums[lt++]); // 小于 pivot，放左边
            } else if (nums[i] > pivot) {
                swap(nums[i], nums[gt--]);   // 大于 pivot，放右边（i不动，换来的还没看过）
            } else {
                i++; // 等于 pivot，跳过
            }
        }
        
        // 等于 pivot 的部分已经在正确位置，只递归小于和大于的部分
        quickSort(nums, left, lt - 1);
        quickSort(nums, gt + 1, right);
    }
};
```

**为什么用三路快排而不是普通二路？**
```
// 输入 [2, 2, 2, 2, 2] (全部相同)
// 二路快排：每次 pivot 分出的左右不均匀，仍可能 O(n²)
// 三路快排：所有等于 pivot 的元素一次性归位，直接 O(n) 搞定
```

**关键点**：
- `nums[i] > pivot` 时 `i` 不动！因为从 `gt` 换过来的值还没检查过。
- 随机化是必须的，不是可选的。

---

### 解法3: 堆排序 — O(n log n) / O(1)

**思考过程**：如果面试官追问"能否原地排序？"，归并需要 O(n)，快排期望 O(log n)（栈空间），堆排序可以做到 O(1) 额外空间。

```
// 堆排序的过程：
// 原始: [5, 2, 3, 1]
// 
// Step1: 建大顶堆 → [5, 2, 3, 1]  (恰好已经是)
// Step2: swap(nums[0], nums[3]) → [1, 2, 3, | 5]  siftDown → [3, 2, 1, | 5]
// Step3: swap(nums[0], nums[2]) → [1, 2, | 3, 5]  siftDown → [2, 1, | 3, 5]
// Step4: swap(nums[0], nums[1]) → [1, | 2, 3, 5]  → 排序完成!
// （| 右边是已排好序的部分）
```

```cpp
class Solution {
public:
    vector<int> sortArray(vector<int>& nums) {
        int n = nums.size();
        
        // Step1: 建大顶堆（从最后一个非叶节点开始向前 siftDown）
        // 为什么从 n/2-1 开始？因为叶节点（n/2 到 n-1）天然满足堆性质
        for (int i = n / 2 - 1; i >= 0; i--) {
            siftDown(nums, i, n);
        }
        
        // Step2: 逐个取出堆顶（最大值），放到数组末尾
        for (int i = n - 1; i > 0; i--) {
            swap(nums[0], nums[i]); // 堆顶（最大值）放到最终位置
            siftDown(nums, 0, i);   // 堆大小减1，重新调整
        }
        
        return nums;
    }
    
    // 将 nums[i] 下沉到正确位置，堆的大小为 heapSize
    void siftDown(vector<int>& nums, int i, int heapSize) {
        while (true) {
            int largest = i;
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            
            if (left < heapSize && nums[left] > nums[largest])
                largest = left;
            if (right < heapSize && nums[right] > nums[largest])
                largest = right;
            
            if (largest == i) break; // 已经满足堆性质
            
            swap(nums[i], nums[largest]);
            i = largest; // 继续下沉
        }
    }
};
```

**关键点**：
- 建堆方向：从 `n/2-1` 向 `0`（自底向上 siftDown），时间 O(n)。如果从 `0` 向 `n-1` siftUp，建堆是 O(n log n)。
- 排序时 `heapSize` 递减，保证已排好的元素不会被打乱。

---

### 解法4: 计数排序 — O(n + k) / O(k) 加分项

**思考过程**：题目约束 nums[i] ∈ [-5×10⁴, 5×10⁴]，值域 k = 10⁵，比 n 还小。我们可以直接统计每个值出现的次数，然后按顺序还原。这不是比较排序，突破了 O(n log n) 的下界！

```cpp
class Solution {
public:
    vector<int> sortArray(vector<int>& nums) {
        const int OFFSET = 50000; // 把 [-50000, 50000] 映射到 [0, 100000]
        const int RANGE = 100001;
        vector<int> count(RANGE, 0);
        
        // 统计每个值的出现次数
        for (int num : nums) {
            count[num + OFFSET]++;
        }
        
        // 按顺序还原
        int idx = 0;
        for (int i = 0; i < RANGE; i++) {
            while (count[i] > 0) {
                nums[idx++] = i - OFFSET;
                count[i]--;
            }
        }
        
        return nums;
    }
};
```

**关键点**：
- 需要用 OFFSET 处理负数，把值域映射到非负整数。
- 计数排序不是比较排序，只有在值域有限时才能用。

---

## 解法对比

|  | 归并排序 | 快速排序 | 堆排序 | 计数排序 |
|--|---------|---------|--------|---------|
| 时间 | O(n log n) 稳定 | O(n log n) 期望 | O(n log n) 稳定 | O(n + k) |
| 空间 | O(n) | O(log n) | O(1) | O(k) |
| 稳定性 | ✅ 稳定 | ❌ 不稳定 | ❌ 不稳定 | ✅ 稳定 |
| 最坏情况 | O(n log n) | O(n²) | O(n log n) | O(n + k) |
| 适用 | 通用，链表排序首选 | 通用，平均最快 | 空间敏感场景 | 值域有限 |

**面试建议**：先写归并排序（最保险），被追问空间时说堆排序，被追问常数时说快排+三路+随机化。

## 易错点

1. **快排 pivot 固定选首元素 → TLE**
   - ✗ `int pivot = nums[left];` 直接用
   - ✓ `int idx = left + rand() % (right - left + 1); swap(nums[left], nums[idx]);` 先随机化

2. **归并排序每次 merge 新建临时数组**
   - ✗ `void merge(...) { vector<int> temp(right-left+1); ... }` — 频繁分配，慢
   - ✓ 在 sortArray 中预分配一次，传引用进去

3. **快排三路分区 i 的移动时机**
   - ✗ `nums[i] > pivot` 时也 `i++` — 错！换过来的值没检查
   - ✓ `nums[i] > pivot` 时只移动 `gt--`，`i` 不动

4. **堆排序建堆方向搞反**
   - ✗ 从 `i=0` 到 `i=n-1` 做 siftUp — O(n log n) 建堆
   - ✓ 从 `i=n/2-1` 到 `i=0` 做 siftDown — O(n) 建堆

5. **归并排序 mid 的计算**
   - ✗ `int mid = (left + right) / 2;` — left+right 可能溢出
   - ✓ `int mid = left + (right - left) / 2;`

## 面试追问

**Q1: 你选了归并排序，能解释为什么它是 O(n log n) 吗？**
→ 递归树有 log n 层（每次对半分），每层的合并操作总共处理 n 个元素，所以总共 n × log n。这个可以用主定理验证：T(n) = 2T(n/2) + O(n) → O(n log n)。

**Q2: 快排最坏 O(n²)，怎么避免？**
→ 随机选 pivot。还可以用"三数取中"（median of three）。如果有大量重复元素，用三路快排把等于 pivot 的元素一次性归位。

**Q3: 如果要排序的是链表而不是数组，你选哪种排序？**
→ 归并排序。因为链表不支持随机访问（快排的 partition 效率差），但归并排序的 merge 只需要按序遍历，链表正合适。而且链表归并可以做到 O(1) 空间（不需要临时数组，直接改指针），参见 LeetCode 148。

**Q4: 堆排序和快排都是原地的，为什么实际中快排更常用？**
→ 快排的缓存局部性（cache locality）更好——它访问的是连续内存。堆排序频繁跳跃访问（父子节点距离远），cache miss 多，常数大。

## 相关题型

- **LeetCode 148 排序链表** — 归并排序的链表版本，复用同样的分治+merge 框架，区别是用快慢指针找中点、merge 改为拼接指针
- **LeetCode 215 数组中的第K个最大元素** — 复用快排的 partition 思想（快速选择算法），只递归一半，O(n) 期望
- **LeetCode 75 颜色分类** — 复用三路快排的 partition 逻辑（荷兰国旗问题），pivot 固定为 1
- **LeetCode 347 前 K 个高频元素** — 复用堆排序思想，维护大小为 K 的小顶堆