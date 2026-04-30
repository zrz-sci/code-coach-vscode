/*
 * LeetCode 215: 数组中的第K个最大元素 (Kth Largest Element in an Array)
 *
 * 【题目本质】
 * 选择问题（Selection Problem）：在无序数组中找到排序后第 k 大的元素，
 * 不需要完全排序，关键是如何避免 O(n log n) 的排序开销。
 *
 * 【解法总览】
 * 解法1: 排序            — O(n log n) / O(log n) — 最直觉，面试起点
 * 解法2: 小顶堆          — O(n log k) / O(k)    — 面试首选，代码简洁
 * 解法3: QuickSelect     — O(n) avg / O(1)      — 满足题目 O(n) 要求
 * 解法4: 三路 QuickSelect — O(n) avg / O(1)      — 优化重复元素场景
 */

// ============================================================
// 解法1: 排序 — 最直觉的做法
// 时间: O(n log n)  空间: O(log n) （快排栈空间）
//
// 【思路】
// 第 k 大就是升序排列后索引 n-k 的元素。
// 虽然超过题目要求的 O(n)，但面试中可以作为起点，
// 展示你理解题意，然后主动说"我来优化"。
// ============================================================
class Solution1 {
public:
    int findKthLargest(vector<int>& nums, int k) {
        sort(nums.begin(), nums.end());
        // 第 k 大 = 升序后从右数第 k 个 = 索引 n-k
        return nums[nums.size() - k];
    }
};

// ============================================================
// 解法2: 小顶堆 — 维护前 k 大元素
// 时间: O(n log k)  空间: O(k)
//
// 【思路】
// 排序的瓶颈：我只需要第 k 大，但排序给出了所有元素的顺序。
// 换个思路：维护一个大小为 k 的小顶堆。
// - 遍历数组，堆满后只有比堆顶大的元素才能进堆。
// - 堆里始终是当前见过的最大 k 个数。
// - 堆顶（最小值）= 前 k 大中最小的 = 第 k 大。
//
// 为什么用小顶堆而不是大顶堆？
// → 大顶堆弹出最大值，无法保留"最大的 k 个"。
//   小顶堆弹出最小值，每次淘汰的是不够大的数，留下的都是大的。
//
// 遍历过程 (nums = [3,2,1,5,6,4], k=2):
//   处理 3: 堆 = [3]           (size < k, 直接加)
//   处理 2: 堆 = [2, 3]        (size == k)
//   处理 1: 1 < 堆顶2, 跳过    堆 = [2, 3]
//   处理 5: 5 > 堆顶2, 弹2压5  堆 = [3, 5]
//   处理 6: 6 > 堆顶3, 弹3压6  堆 = [5, 6]
//   处理 4: 4 < 堆顶5, 跳过    堆 = [5, 6]
//   堆顶 = 5 → 答案
// ============================================================
class Solution2 {
public:
    int findKthLargest(vector<int>& nums, int k) {
        // greater<int> 使得 top() 返回最小值（小顶堆）
        priority_queue<int, vector<int>, greater<int>> minHeap;

        for (int num : nums) {
            minHeap.push(num);
            // 堆大小超过 k 就弹出最小的
            // 这保证堆里始终是目前见过的最大 k 个数
            if ((int)minHeap.size() > k) {
                minHeap.pop();
            }
        }

        // 前 k 大中最小的 = 第 k 大
        return minHeap.top();
    }
};

// ============================================================
// 解法3: QuickSelect（随机化）— 平均 O(n)
// 时间: O(n) 平均, O(n²) 最坏  空间: O(1)
//
// 【思路】
// 堆的瓶颈：O(n log k)，当 k 接近 n/2 时接近 O(n log n)。
// 
// 核心洞察：快速排序的 partition 操作能确定一个元素的最终位置。
// - partition 后，pivot 左边都 <= pivot，右边都 >= pivot
// - 如果 pivot 正好在目标位置 n-k，就找到了
// - 如果不在，只需在一侧继续搜索（砍掉另一半）
//
// 与快速排序的关键区别：
// - 快排：两侧都递归 → T(n) = 2T(n/2) + O(n) = O(n log n)
// - QuickSelect：只递归一侧 → T(n) = T(n/2) + O(n) = O(n)
//
// 搜索空间收缩过程 (nums=[3,2,1,5,6,4], k=2, target=4):
//   Round1: [3,2,1,5,6,4] pivot=4 → [3,2,1] 4 [5,6]
//           pivotIdx=3, target=4 → 在右半 [5,6] 中找
//   Round2: [5,6] pivot=6 → [5] 6
//           pivotIdx=5, target=4 → 在左半 [5] 中找  
//   Round3: [5] 只剩一个 → 返回 5
// ============================================================
class Solution3 {
public:
    int findKthLargest(vector<int>& nums, int k) {
        int n = nums.size();
        int targetIdx = n - k;  // 第 k 大 = 升序索引 n-k
        int left = 0, right = n - 1;

        while (left < right) {
            int pivotIdx = partition(nums, left, right);

            if (pivotIdx == targetIdx) {
                return nums[pivotIdx];
            } else if (pivotIdx < targetIdx) {
                // pivot 在目标左边，答案在右半
                left = pivotIdx + 1;
            } else {
                // pivot 在目标右边，答案在左半
                right = pivotIdx - 1;
            }
        }
        // left == right，区间收缩到一个元素
        return nums[left];
    }

private:
    int partition(vector<int>& nums, int left, int right) {
        // 【关键】随机选 pivot 避免有序数组退化到 O(n²)
        int randomIdx = left + rand() % (right - left + 1);
        swap(nums[randomIdx], nums[right]);

        int pivot = nums[right];
        int storeIdx = left;  // storeIdx: 下一个放"小于 pivot 的数"的位置

        for (int i = left; i < right; i++) {
            if (nums[i] < pivot) {
                swap(nums[storeIdx], nums[i]);
                storeIdx++;
            }
        }
        // 把 pivot 放到最终位置：左边全 < pivot，右边全 >= pivot
        swap(nums[storeIdx], nums[right]);
        return storeIdx;
    }
};

// ============================================================
// 解法4: 三路 QuickSelect — 处理大量重复元素
// 时间: O(n) 平均  空间: O(1)
//
// 【思路】
// 解法3的瓶颈：当数组有大量重复元素时，标准 partition 把等于 pivot
// 的元素全放一边，导致分割不均匀。
//
// 三路 partition 将数组分成三段：
//   [left..lt-1] < pivot | [lt..gt] == pivot | [gt+1..right] > pivot
//
// 如果 targetIdx 落在 [lt, gt] 之间，可以直接返回 pivot，
// 不需要继续递归。当数组有大量重复时，"等于区间"很宽，可以一次排除大量元素。
//
// 三路 partition 示意 (pivot=4):
//   初始: [3, 2, 4, 5, 4, 4, 1, 6]
//   结果: [3, 2, 1] [4, 4, 4] [5, 6]
//          < pivot   == pivot   > pivot
//          lt=3       gt=5
// ============================================================
class Solution4 {
public:
    int findKthLargest(vector<int>& nums, int k) {
        int n = nums.size();
        int targetIdx = n - k;
        int left = 0, right = n - 1;

        while (left <= right) {
            // 随机选 pivot
            int randomIdx = left + rand() % (right - left + 1);
            swap(nums[randomIdx], nums[left]);
            int pivot = nums[left];

            // 三路 partition
            int lt = left;      // nums[left..lt-1] 全 < pivot
            int gt = right;     // nums[gt+1..right] 全 > pivot
            int i = left + 1;   // 扫描指针

            while (i <= gt) {
                if (nums[i] < pivot) {
                    swap(nums[i++], nums[lt++]);
                } else if (nums[i] > pivot) {
                    // 注意：i 不自增！因为从 gt 换过来的数还没检查
                    swap(nums[i], nums[gt--]);
                } else {
                    // nums[i] == pivot，已在等于区间中
                    i++;
                }
            }
            // 此时 nums[lt..gt] 全等于 pivot

            if (targetIdx < lt) {
                right = lt - 1;     // 目标在左段
            } else if (targetIdx > gt) {
                left = gt + 1;      // 目标在右段
            } else {
                return pivot;       // 目标在等于段中，直接返回
            }
        }

        return nums[left];
    }
};

// ============================================================
// 【解法对比】
//
// | 解法      | 时间        | 空间    | 修改原数组 | 适用场景          |
// |-----------|-------------|---------|------------|-------------------|
// | 排序      | O(n log n)  | O(logn) | 是         | 快速实现/起步     |
// | 小顶堆   | O(n log k)  | O(k)    | 否         | 不能改数组/流数据 |
// | QuickSel  | O(n) avg    | O(1)    | 是         | 追求平均O(n)      |
// | 三路 QS   | O(n) avg    | O(1)    | 是         | 大量重复元素      |
//
// 面试建议：
// - 先写堆解法（简洁、不易出错、不改原数组）
// - 再提 QuickSelect 展示算法功底
// - 主动说明 QuickSelect 最坏 O(n²) 和随机化解决方案
//
// 【易错点】
// 1. 第 k 大 vs 第 k 小：
//    ✗ return nums[k-1]  → 这是第 k 小
//    ✓ return nums[n-k]  → 第 k 大
//
// 2. 堆的方向选反：
//    ✗ priority_queue<int> maxHeap（大顶堆）→ 弹出最大的，留不住前 k 大
//    ✓ priority_queue<int, vector<int>, greater<int>> minHeap（小顶堆）
//
// 3. QuickSelect 不随机化：
//    ✗ int pivot = nums[left]; → 有序数组 O(n²)
//    ✓ swap(nums[left + rand()%(right-left+1)], nums[right]); 然后取 nums[right]
//
// 4. 三路 partition 中 nums[i] > pivot 时 i++：
//    ✗ swap(nums[i], nums[gt--]); i++;  → 换过来的数没检查就跳过
//    ✓ swap(nums[i], nums[gt--]);       → i 不动，下轮重新检查
//
// 5. partition 的 storeIdx 初始化：
//    ✗ int storeIdx = 0;  → 在子区间 [left,right] 上操作会出错
//    ✓ int storeIdx = left;
//
// 【面试追问】
// Q1: 暴力排序能过吗？瓶颈在哪？
//     → 能过大部分case，但 O(n log n) 不满足题目 O(n) 要求。
//       瓶颈在于排序确定了所有元素的相对顺序，而我们只需要一个位置。
//
// Q2: 如果不能修改原数组呢？
//     → 用堆解法 O(n log k)，不修改原数组。
//       或者拷贝一份再 QuickSelect，O(n) 额外空间。
//
// Q3: 如果是数据流，元素一个一个到来，随时需要第 k 大？
//     → 维护大小为 k 的小顶堆。新元素比堆顶大就替换。
//       这就是 LeetCode 703 的做法。
//
// Q4: QuickSelect 最坏 O(n²)，有确定性 O(n) 的算法吗？
//     → BFPRT（Median of Medians）：5个一组取中位数，再取中位数的中位数
//       做 pivot，保证每次至少排除 3/10 的元素。面试提到即可。
// ============================================================
