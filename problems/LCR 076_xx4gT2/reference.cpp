/*
 * LCR 076. 数组中的第 K 个最大元素（同主站 215）
 *
 * 【题目本质】
 * 在未排序数组中找到第 k 个最大的元素。
 * 核心问题：不需要完全排序，只需要找到"排名第 k"的元素。
 *
 * 【解法总览】
 * 解法1: 排序              — O(n log n) / O(log n)    — 最直觉
 * 解法2: 最小堆(大小 k)     — O(n log k) / O(k)       — 稳定可靠
 * 解法3: 快速选择(递归)     — O(n) 平均 / O(log n)     — 最优解 ⭐
 * 解法3b: 快速选择(迭代)    — O(n) 平均 / O(1)         — 消除递归栈
 * 解法4: nth_element        — O(n) 平均 / O(1)         — STL 一行
 */


// ============================================================
// 解法1: 排序
// 时间: O(n log n)  空间: O(log n)（排序栈空间）
//
// 【思路】
// 最朴素的想法：排好序后第 k 大的元素就在 nums[n-k] 位置。
// 这是面试中的起点，先说出来再优化。
//
// 示例: nums = [3,2,1,5,6,4], k = 2
//   排序后: [1,2,3,4,5,6]
//   n-k = 6-2 = 4 → nums[4] = 5 ✓
// ============================================================
class Solution1 {
public:
    int findKthLargest(vector<int>& nums, int k) {
        sort(nums.begin(), nums.end());
        return nums[nums.size() - k];
    }
};

// ============================================================
// 解法2: 最小堆（大小 k）
// 时间: O(n log k)  空间: O(k)
//
// 【思路】
// 维护一个大小为 k 的最小堆，存当前遍历过的最大的 k 个元素。
// 堆顶是这 k 个元素中最小的 = 全局第 k 大。
//
// 为什么用最小堆而不是最大堆？
//   最小堆大小 k：堆顶 = 第 k 大（直接读取）
//   最大堆大小 n：需要弹出 k-1 次才得到第 k 大，O(k log n) 更慢
//
// 工作流程:
//   - 堆未满(size < k)：直接加入
//   - 堆已满(size == k)：
//     - 新元素 > 堆顶 → 弹出堆顶，加入新元素
//     - 新元素 <= 堆顶 → 跳过
//
// 示例: nums = [3,2,1,5,6,4], k = 2
//   加入3:  堆 = [3]         (size=1 < k, 直接加)
//   加入2:  堆 = [2, 3]      (size=2 == k, 堆满)
//   加入1:  1 < 堆顶2 → 跳过
//   加入5:  5 > 堆顶2 → 弹2加5 → 堆 = [3, 5]
//   加入6:  6 > 堆顶3 → 弹3加6 → 堆 = [5, 6]
//   加入4:  4 < 堆顶5 → 跳过
//   堆顶 = 5 ✓
// ============================================================
class Solution2 {
public:
    int findKthLargest(vector<int>& nums, int k) {
        // C++ priority_queue 默认最大堆
        // 最小堆需要指定 greater<int>
        priority_queue<int, vector<int>, greater<int>> minHeap;

        for (int num : nums) {
            minHeap.push(num);
            // 堆大小超过 k 时，弹出最小的
            if ((int)minHeap.size() > k) {
                minHeap.pop();
            }
        }

        // 堆中保留了最大的 k 个元素
        // 堆顶是其中最小的 = 全局第 k 大
        return minHeap.top();
    }
};

// ============================================================
// 解法3: 快速选择（Quick Select）— 最优解 ⭐
// 时间: O(n) 平均, O(n^2) 最坏  空间: O(log n) 递归栈
//
// 【思路】
// 借鉴快排的 partition 思想，但每次只递归一侧。
//
// 快排 vs 快速选择：
//   快排：partition 后，左右两侧都要递归排序 → O(n log n)
//   快选：partition 后，只递归目标所在的一侧 → O(n)
//
// Partition (Lomuto 方案):
//   选定 pivot，把数组分成两半：
//   [<= pivot | pivot | >= pivot]
//   pivot 落在位置 p:
//     - p == target → 找到了
//     - p < target  → 答案在右侧
//     - p > target  → 答案在左侧
//
// 工作量分析（平均情况）：
//   每轮 partition 处理 n 个元素，pivot 期望落在中间位置
//   下一轮处理 n/2，再下一轮 n/4...
//   总工作量: n + n/2 + n/4 + ... = 2n = O(n)
//
// 随机化的重要性：
//   如果总选最后一个元素做 pivot：
//     已排序数组 [1,2,3,...,n]，每轮 pivot 是最大值
//     只排除1个元素 → n + (n-1) + (n-2) + ... = O(n^2)
//   随机选 pivot：期望每轮排除约一半 → O(n)
//
// 示例: nums = [3,2,1,5,6,4], k = 2, target = n-k = 4
//
//   第1轮: pivot=4 (随机选中)
//     partition → [3,2,1, | 4 | ,6,5]
//     pivot 在位置3, target=4 → 答案在右侧
//
//   第2轮: 只看 [6,5], pivot=5
//     partition → [5, | 6]  (相对整个数组: 位置4是5, 位置5是6)
//     pivot 在位置4 == target → 返回 5 ✓
// ============================================================
class Solution3 {
public:
    int findKthLargest(vector<int>& nums, int k) {
        srand(42); // 固定种子保证可复现
        int target = nums.size() - k; // "第 k 大" = "第 n-k 小"(0-indexed)
        return quickSelect(nums, 0, nums.size() - 1, target);
    }

private:
    int quickSelect(vector<int>& nums, int left, int right, int target) {
        // 只剩一个元素，必是答案
        if (left == right) return nums[left];

        // ---- 随机选 pivot，避免最坏情况 ----
        int pivotIdx = left + rand() % (right - left + 1);
        swap(nums[pivotIdx], nums[right]); // 把 pivot 换到最右边

        // ---- Lomuto Partition ----
        // storeIdx: "小于等于区域"的右边界
        // 扫描 [left, right-1]，把 <= pivot 的元素交换到左侧
        int pivotVal = nums[right];
        int storeIdx = left;

        for (int i = left; i < right; i++) {
            if (nums[i] <= pivotVal) {
                swap(nums[i], nums[storeIdx]);
                storeIdx++;
            }
        }
        // 把 pivot 放到它的最终位置
        swap(nums[storeIdx], nums[right]);

        // ---- 根据 pivot 位置决定递归方向 ----
        if (storeIdx == target) {
            return nums[storeIdx]; // 恰好是目标位置
        } else if (storeIdx < target) {
            return quickSelect(nums, storeIdx + 1, right, target); // 去右边找
        } else {
            return quickSelect(nums, left, storeIdx - 1, target); // 去左边找
        }
    }
};

// ============================================================
// 解法3b: 快速选择（迭代版）— 消除递归栈
// 时间: O(n) 平均  空间: O(1)
//
// 【思路】
// 将递归改为迭代，通过更新 left/right 边界实现。
// 本质上和递归版完全相同，只是消除了递归栈开销。
// ============================================================
class Solution3b {
public:
    int findKthLargest(vector<int>& nums, int k) {
        srand(42);
        int target = nums.size() - k;
        int left = 0, right = nums.size() - 1;

        while (left < right) {
            // 随机选 pivot
            int pivotIdx = left + rand() % (right - left + 1);
            swap(nums[pivotIdx], nums[right]);

            // Lomuto Partition
            int pivotVal = nums[right];
            int storeIdx = left;
            for (int i = left; i < right; i++) {
                if (nums[i] <= pivotVal) {
                    swap(nums[i], nums[storeIdx]);
                    storeIdx++;
                }
            }
            swap(nums[storeIdx], nums[right]);

            // 缩小搜索范围（不递归，直接更新边界）
            if (storeIdx == target) {
                return nums[storeIdx];
            } else if (storeIdx < target) {
                left = storeIdx + 1;
            } else {
                right = storeIdx - 1;
            }
        }

        return nums[left]; // left == right == target
    }
};

// ============================================================
// 解法4: STL nth_element — 一行搞定
// 时间: O(n) 平均  空间: O(1)
//
// 【思路】
// nth_element 内部实现就是 Introselect（快速选择 + 兜底堆排序）。
// 面试中先手写快速选择，再提 STL 作为加分项。
// ============================================================
class Solution4 {
public:
    int findKthLargest(vector<int>& nums, int k) {
        nth_element(nums.begin(), nums.begin() + k - 1, nums.end(), greater<int>());
        return nums[k - 1];
    }
};

// ============================================================
// 【解法对比】
//
// | 解法        | 时间(平均)  | 时间(最坏)  | 空间    | 修改原数组 |
// |-------------|-----------|-----------|---------|----------|
// | 排序        | O(nlogn)  | O(nlogn)  | O(logn) | 是       |
// | 最小堆      | O(nlogk)  | O(nlogk)  | O(k)    | 否       |
// | 快选(递归)  | O(n)      | O(n^2)    | O(logn) | 是       |
// | 快选(迭代)  | O(n)      | O(n^2)    | O(1)    | 是       |
// | nth_element | O(n)      | O(n)      | O(1)    | 是       |
//
// 面试策略：
// 1. 先说排序 O(nlogn) 作为 baseline
// 2. 写出最小堆解法 O(nlogk)，代码短且不易出错
// 3. 讲解快速选择的思路和平均 O(n) 的分析
// 4. 如果面试官要求写快速选择，用迭代版更安全
//
// ============================================================
// 【易错点】
//
// 1. "第 k 大"的索引转换:
//    ✗ nums[k-1] (降序) 或 nums[k] (忘记-1)
//    ✓ 升序排列后 nums[n-k]
//
// 2. C++ priority_queue 默认是最大堆:
//    ✗ priority_queue<int> pq; → 最大堆
//    ✓ priority_queue<int, vector<int>, greater<int>> pq; → 最小堆
//
// 3. 快速选择不随机化 pivot:
//    ✗ int pivotVal = nums[right]; → 已排序数组 O(n^2)
//    ✓ 先 swap(nums[rand()%(right-left+1)+left], nums[right])
//
// 4. Partition 循环范围包含 pivot:
//    ✗ for (int i = left; i <= right; i++) → pivot 位置被多次交换
//    ✓ for (int i = left; i < right; i++) → pivot 在 right 位置不参与循环
//
// 5. 堆弹出时机:
//    ✗ 先判断再 push → 需要处理堆为空的边界
//    ✓ 先 push 再判断 size > k 时 pop → 更简洁统一
//
// ============================================================
// 【面试追问】
//
// Q1: O(nlogk) vs O(nlogn)，k 多大时差异显著？
//     当 k << n 时差异最大。如 n=10^6, k=10: logk=3, logn=20。
//     当 k ~ n 时两者差不多。
//
// Q2: 快速选择平均 O(n) 怎么推导？
//     每轮 pivot 期望落在中间 → 处理 n, n/2, n/4, ...
//     等比数列求和: n(1 + 1/2 + 1/4 + ...) = 2n = O(n)
//
// Q3: 能否做到最坏 O(n)？
//     Median of Medians: 将数组分成5个一组，取每组中位数的中位数做 pivot。
//     保证每轮至少排除 30% 元素 → 最坏 O(n)。但常数大，实际不如随机化。
//
// Q4: 在线场景（数据流）怎么处理？
//     维护大小为 k 的最小堆。每来一个数 O(logk) 更新。
//     这就是 LeetCode 703 题的场景。
//
// ============================================================
// 【相关题型】
//
// - 347. 前 K 个高频元素: 堆 + 哈希表统计频率
// - 703. 数据流中的第 K 大元素: 在线版本，最小堆
// - 973. 最接近原点的 K 个点: 堆或快速选择
// - 23. 合并K个升序链表: 堆维护多路归并
// ============================================================
