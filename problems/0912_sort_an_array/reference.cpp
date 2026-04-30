/*
 * LeetCode 912: 排序数组 (Sort an Array)
 *
 * 【题目本质】
 * 手写 O(n log n) 排序算法。考察对归并排序、快速排序、堆排序的理解和实现能力。
 *
 * 【解法总览】
 * 解法1: 归并排序      — O(n log n) / O(n)      — 最稳定，面试首选
 * 解法2: 快速排序(三路) — O(n log n)期望 / O(log n) — 平均最快，但需随机化
 * 解法3: 堆排序        — O(n log n) / O(1)      — 原地排序
 * 解法4: 计数排序      — O(n + k) / O(k)        — 利用值域有限的特殊解法
 */

// ============================================================
// 解法1: 归并排序 — 分治：分成两半，各自排序，再合并
// 时间: O(n log n)  空间: O(n)
//
// 【思路】
// 分治法：把数组从中间切开，左右两半分别递归排序，
// 然后用双指针把两个有序数组合并成一个有序数组。
//
// 为什么是 O(n log n)？
// - 每次对半分 → 递归树 log n 层
// - 每层合并操作总共处理 n 个元素
// - 总工作量 = n × log n
//
//  分治过程图示:
//  [5, 2, 3, 1]          ← 分
//   /         \
// [5, 2]    [3, 1]       ← 分
//  / \       / \
// [5] [2]  [3] [1]       ← 单元素，天然有序
//  \ /       \ /
// [2, 5]    [1, 3]       ← 合并
//   \         /
//  [1, 2, 3, 5]          ← 合并
//
// 合并 [2,5] 和 [1,3] 的过程:
//  i=0 j=0: 比较 2 vs 1 → 取1, j++
//  i=0 j=1: 比较 2 vs 3 → 取2, i++
//  i=1 j=1: 比较 5 vs 3 → 取3, j++
//  i=1 j=2: j越界 → 取5, i++
//  结果: [1, 2, 3, 5]
// ============================================================
class Solution1 {
public:
    vector<int> sortArray(vector<int>& nums) {
        vector<int> temp(nums.size()); // 预分配临时数组，避免递归中反复分配
        mergeSort(nums, temp, 0, nums.size() - 1);
        return nums;
    }

private:
    void mergeSort(vector<int>& nums, vector<int>& temp, int left, int right) {
        if (left >= right) return; // 只有0或1个元素，天然有序

        int mid = left + (right - left) / 2; // 防止 left+right 溢出
        mergeSort(nums, temp, left, mid);     // 递归排序左半段
        mergeSort(nums, temp, mid + 1, right); // 递归排序右半段

        // 优化：如果左半段的最大值 <= 右半段的最小值，说明已经有序，跳过合并
        if (nums[mid] <= nums[mid + 1]) return;

        merge(nums, temp, left, mid, right);  // 合并两个有序段
    }

    void merge(vector<int>& nums, vector<int>& temp, int left, int mid, int right) {
        // 把待合并区间拷贝到临时数组
        for (int k = left; k <= right; k++) {
            temp[k] = nums[k];
        }

        int i = left;      // 左半段指针
        int j = mid + 1;   // 右半段指针

        for (int k = left; k <= right; k++) {
            if (i > mid) {
                // 左半段已用完，直接取右半段
                nums[k] = temp[j++];
            } else if (j > right) {
                // 右半段已用完，直接取左半段
                nums[k] = temp[i++];
            } else if (temp[i] <= temp[j]) {
                // 两边都有剩余，取较小者（<= 保证稳定性）
                nums[k] = temp[i++];
            } else {
                nums[k] = temp[j++];
            }
        }
    }
};

// ============================================================
// 解法2: 快速排序（三路分区 + 随机 pivot）
// 时间: O(n log n) 期望  空间: O(log n) 递归栈
//
// 【思路】
// 和归并排序的方向相反：
//   归并 = 先递归分开，再合并（后序工作）
//   快排 = 先分区（前序工作），再递归处理两边
//
// 关键操作是 partition：选一个 pivot，
// 把数组分成 [< pivot | == pivot | > pivot] 三段，
// 然后只需要递归处理 < 和 > 的部分（== 的已经在正确位置了）。
//
// 为什么必须随机选 pivot？
// 固定选首元素，对已排序输入：
//   pivot=1 → 左边0个，右边n-1个 → T(n)=T(n-1)+O(n) → O(n²)
// 随机化后，期望每次能分到大致均匀的两半。
//
// 为什么用三路而不是二路？
// 输入 [2,2,2,2,2]，二路快排每次分出的两边仍不均匀。
// 三路快排：所有 ==pivot 的一次性归位，只递归不等于的部分。
//
// 三路分区过程 (pivot=3):
//  原始: [5, 2, 3, 1, 3, 4]
//  lt=0, gt=5, i=1
//  i=1: nums[1]=5 > 3 → swap(nums[1],nums[5]) → [4,2,3,1,3,5] gt=4
//  i=1: nums[1]=4 > 3 → swap(nums[1],nums[4]) → [3,2,3,1,4,5] gt=3
//  i=1: nums[1]=3 == 3 → i=2
//  i=2: nums[2]=2 < 3 → swap(nums[2],nums[0]) → [2,3,3,1,4,5] lt=1, i=3
//  i=3: nums[3]=1 < 3 → swap(nums[3],nums[1]) → [2,1,3,3,4,5] lt=2, i=4
//  i=4 > gt=3 → 停止
//  结果: [2,1 | 3,3 | 4,5]  (lt=2, gt=3)
//         <3    ==3    >3
// ============================================================
class Solution2 {
public:
    vector<int> sortArray(vector<int>& nums) {
        srand(42);
        quickSort(nums, 0, nums.size() - 1);
        return nums;
    }

private:
    void quickSort(vector<int>& nums, int left, int right) {
        if (left >= right) return;

        // 随机选 pivot，交换到 left 位置，避免最坏情况
        int randomIdx = left + rand() % (right - left + 1);
        swap(nums[left], nums[randomIdx]);

        int pivot = nums[left];
        int lt = left;      // [left, lt-1] 是 < pivot 的区域
        int gt = right;     // [gt+1, right] 是 > pivot 的区域
        int i = left + 1;   // 当前考察的元素

        // 循环不变量:
        // nums[left..lt-1] < pivot
        // nums[lt..i-1] == pivot
        // nums[gt+1..right] > pivot
        // nums[i..gt] 待处理
        while (i <= gt) {
            if (nums[i] < pivot) {
                swap(nums[i++], nums[lt++]); // 放到左边，两个指针都前进
            } else if (nums[i] > pivot) {
                swap(nums[i], nums[gt--]);   // 放到右边，只 gt 后退
                // 注意 i 不动！因为从 gt 换来的值还没检查过
            } else {
                i++; // 等于 pivot，已经在正确区域
            }
        }

        // 递归处理 < 和 > 的部分，== pivot 的已经就位
        quickSort(nums, left, lt - 1);
        quickSort(nums, gt + 1, right);
    }
};

// ============================================================
// 解法3: 堆排序 — 建大顶堆 + 逐个取堆顶
// 时间: O(n log n)  空间: O(1) 原地排序
//
// 【思路】
// 分两步：
// 1) 建大顶堆（堆顶是最大值）
// 2) 循环：把堆顶和末尾交换（最大值就位），堆大小减1，重新调整堆
//
// 建堆过程（自底向上 siftDown，O(n)）:
//   从最后一个非叶节点 (n/2-1) 开始，逐个向前做 siftDown
//   为什么是 O(n)？底层节点多但下沉距离短，顶层节点少但下沉距离长，总和是 O(n)
//
// 排序过程示例 [5, 2, 3, 1]:
//   建堆:  已经是 [5, 2, 3, 1]（恰好满足大顶堆）
//          5
//         / \
//        2   3
//       /
//      1
//
//   Round1: swap(5,1) → [1,2,3,|5] → siftDown → [3,2,1,|5]
//          3           堆部分        已排好
//         / \
//        2   1          |5|
//
//   Round2: swap(3,1) → [1,2,|3,5] → siftDown → [2,1,|3,5]
//          2
//         /
//        1              |3,5|
//
//   Round3: swap(2,1) → [1,|2,3,5] → 完成!
// ============================================================
class Solution3 {
public:
    vector<int> sortArray(vector<int>& nums) {
        int n = nums.size();

        // Step1: 建大顶堆
        // 从最后一个非叶节点开始向前，逐个 siftDown
        // 叶节点（索引 n/2 到 n-1）天然满足堆性质，不用管
        for (int i = n / 2 - 1; i >= 0; i--) {
            siftDown(nums, i, n);
        }

        // Step2: 排序
        // 每次把堆顶（最大值）交换到数组末尾，然后缩小堆，重新调整
        for (int i = n - 1; i > 0; i--) {
            swap(nums[0], nums[i]); // 最大值放到最终位置
            siftDown(nums, 0, i);   // 堆大小变为 i，调整新堆顶
        }

        return nums;
    }

private:
    // 把 nums[i] 下沉到正确位置
    // heapSize: 堆的有效大小（后面的是已排好的，不能动）
    void siftDown(vector<int>& nums, int i, int heapSize) {
        while (true) {
            int largest = i;
            int left = 2 * i + 1;   // 左孩子
            int right = 2 * i + 2;  // 右孩子

            // 在 i、左孩子、右孩子中找最大的
            if (left < heapSize && nums[left] > nums[largest])
                largest = left;
            if (right < heapSize && nums[right] > nums[largest])
                largest = right;

            if (largest == i) break; // 当前节点已经比两个孩子大，停止

            swap(nums[i], nums[largest]);
            i = largest; // 继续下沉到交换的位置
        }
    }
};

// ============================================================
// 解法4: 计数排序 — 利用值域有限 [-50000, 50000]
// 时间: O(n + k)  空间: O(k)，k = 100001
//
// 【思路】
// 题目约束 nums[i] ∈ [-5×10⁴, 5×10⁴]，值域只有 10⁵+1 个整数。
// 不需要比较排序！直接数每个值出现几次，然后按顺序还原。
//
// 这是非比较排序，突破了比较排序的 Ω(n log n) 下界。
//
// 过程: [5, 1, 1, 2, 0, 0]
// 计数: count[0]=2, count[1]=2, count[2]=1, count[5]=1
// 还原: [0, 0, 1, 1, 2, 5]
// ============================================================
class Solution4 {
public:
    vector<int> sortArray(vector<int>& nums) {
        const int OFFSET = 50000;   // 把负数映射到非负
        const int RANGE = 100001;   // 值域大小: -50000 到 50000
        vector<int> count(RANGE, 0);

        // 统计每个值的出现次数
        for (int num : nums) {
            count[num + OFFSET]++;
        }

        // 按顺序还原到原数组
        int idx = 0;
        for (int i = 0; i < RANGE; i++) {
            while (count[i] > 0) {
                nums[idx++] = i - OFFSET; // 映射回原始值
                count[i]--;
            }
        }

        return nums;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法     | 时间           | 空间     | 稳定 | 特点                    |
// |----------|---------------|----------|------|------------------------|
// | 归并排序 | O(n log n)    | O(n)     | ✅   | 最稳定，链表排序首选      |
// | 快速排序 | O(n log n)期望 | O(log n) | ❌   | 平均最快，需随机化        |
// | 堆排序   | O(n log n)    | O(1)     | ❌   | 原地，但 cache 不友好    |
// | 计数排序 | O(n + k)      | O(k)     | ✅   | 值域有限时最优           |
//
// 面试推荐：先写归并（稳、不怕最坏情况），被追问再说快排/堆排。
//
// 【易错点】
// 1. 快排不随机化 → 对已排序输入 O(n²)，LeetCode 会 TLE
//    ✗ int pivot = nums[left]; 直接用
//    ✓ 先随机选 idx，swap 到 left 再用
//
// 2. 三路快排中 nums[i] > pivot 时也 i++
//    ✗ if (nums[i] > pivot) { swap(nums[i++], nums[gt--]); }
//    ✓ if (nums[i] > pivot) { swap(nums[i], nums[gt--]); } // i 不动
//    因为从 gt 换过来的值还没检查过，必须在下一轮继续检查
//
// 3. 归并排序 temp 数组在 merge 函数内部每次新建
//    ✗ void merge(...) { vector<int> tmp(right-left+1); }
//    ✓ 外部预分配一次，传引用进来，避免频繁内存分配
//
// 4. 堆排序建堆方向搞反
//    ✗ for(int i=0; i<n; i++) siftUp(i);  // O(n log n) 建堆
//    ✓ for(int i=n/2-1; i>=0; i--) siftDown(i, n);  // O(n) 建堆
//
// 5. 归并排序 mid 计算溢出
//    ✗ int mid = (left + right) / 2;  // left+right 可能超 INT_MAX
//    ✓ int mid = left + (right - left) / 2;
//
// 【面试追问】
// Q1: 归并排序为什么是 O(n log n)？能用主定理推吗？
//     → T(n) = 2T(n/2) + O(n)，主定理 case 2: a=b^k 即 2=2^1,
//       T(n) = O(n^1 × log n) = O(n log n)。
//
// Q2: 快排最坏 O(n²) 能完全避免吗？
//     → 随机化只是"期望" O(n log n)，理论上仍可能 O(n²)（概率极低）。
//       如果要保证最坏 O(n log n)，用"中位数的中位数"（Median of Medians）
//       选 pivot，但常数太大，实际不用。
//
// Q3: 如果排序的是链表，选哪种算法？
//     → 归并排序。链表不支持随机访问，快排 partition 效率差。
//       链表归并可以用快慢指针找中点，merge 直接改指针，
//       空间 O(log n)（递归栈），甚至自底向上迭代做到 O(1) 空间。
//       参见 LeetCode 148。
//
// Q4: 堆排序是原地的，为什么实践中不如快排常用？
//     → 堆排序的 siftDown 访问模式是父子节点跳跃访问（索引 i 和 2i+1），
//       cache locality 差。快排是连续扫描，对 CPU 缓存更友好，
//       实测常数比堆排序小 2-3 倍。
// ============================================================
