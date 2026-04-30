/*
 * 面试题 17.14: 最小K个数 (Smallest K LCCI)
 *
 * 【题目本质】
 * Top K 问题：从无序数组中找到最小的 k 个元素（任意顺序返回）。
 * 与 LC 215 互为镜像——215 找第 k 大，本题找前 k 小。
 *
 * 【解法总览】
 * 解法1: 排序           — O(n log n) / O(log n)  — 最直觉，面试起点
 * 解法2: 大顶堆         — O(n log k) / O(k)      — 面试首选，不改原数组
 * 解法3: QuickSelect    — O(n) avg / O(1)        — 最优平均，改原数组
 */

// ============================================================
// 解法1: 排序 — 最直觉的做法
// 时间: O(n log n)  空间: O(log n)
//
// 【思路】
// 排序后前 k 个元素就是最小的 k 个。
// 虽然不是最优，面试中可作为起点，然后主动优化。
// ============================================================
class Solution1 {
public:
    vector<int> smallestK(vector<int>& arr, int k) {
        sort(arr.begin(), arr.end());
        return vector<int>(arr.begin(), arr.begin() + k);
    }
};

// ============================================================
// 解法2: 大顶堆 — 维护最小 k 个元素
// 时间: O(n log k)  空间: O(k)
//
// 【思路】
// 排序的瓶颈：给出了所有元素的顺序，但我只需要最小的 k 个。
// 维护一个大小为 k 的大顶堆：
// - 堆顶是这 k 个数中最大的——充当"入场门槛"
// - 新来的数如果比堆顶小，就替换堆顶
// - 最终堆中就是最小的 k 个数
//
// 为什么用大顶堆而不是小顶堆？
// → 小顶堆弹出最小值，留下的是大的数。
//   大顶堆弹出最大值，留下的才是最小的 k 个。
//
// 遍历过程 (arr = [1,3,5,7,2,4,6,8], k=4):
//   处理 1: 堆 = [1]              (size < k, 直接加)
//   处理 3: 堆 = [3,1]            (size < k, 直接加)
//   处理 5: 堆 = [5,3,1]          (size < k, 直接加)
//   处理 7: 堆 = [7,5,3,1]        (size==k, 堆顶=7)
//   处理 2: 2 < 堆顶7 → 弹7压2    堆 = [5,3,2,1]
//   处理 4: 4 < 堆顶5 → 弹5压4    堆 = [4,3,2,1]
//   处理 6: 6 > 堆顶4 → 跳过      堆 = [4,3,2,1]
//   处理 8: 8 > 堆顶4 → 跳过      堆 = [4,3,2,1]
//   堆中 = {1,2,3,4} → 答案
// ============================================================
class Solution2 {
public:
    vector<int> smallestK(vector<int>& arr, int k) {
        if (k == 0) return {};

        // 大顶堆：top() 返回最大值
        priority_queue<int> maxHeap;

        for (int num : arr) {
            maxHeap.push(num);
            // 堆大小超过 k 就弹出最大的
            // 保证堆里始终是目前见过的最小 k 个数
            if ((int)maxHeap.size() > k) {
                maxHeap.pop();
            }
        }

        // 把堆中元素倒出来作为结果
        vector<int> result;
        result.reserve(k);
        while (!maxHeap.empty()) {
            result.push_back(maxHeap.top());
            maxHeap.pop();
        }
        return result;
    }
};

// ============================================================
// 解法3: QuickSelect（随机化）— 平均 O(n)
// 时间: O(n) 平均, O(n^2) 最坏  空间: O(1)
//
// 【思路】
// 堆的瓶颈：O(n log k)，当 k 接近 n/2 时接近 O(n log n)。
//
// 核心洞察：partition 操作能确定 pivot 的最终位置。
// - 如果 pivot 落在索引 k-1，arr[0..k-1] 就是最小的 k 个数
// - 如果 pivot 索引 < k-1，只需在右半继续找
// - 如果 pivot 索引 > k-1，只需在左半继续找
//
// 与快速排序的区别：
// - 快排两侧都递归 → O(n log n)
// - QuickSelect 只递归一侧 → 平均 O(n)
//
// 搜索过程 (arr=[1,3,5,7,2,4,6,8], k=4, target=3):
//   Round1: pivot=4 → [1,3,2] 4 [5,7,6,8]
//           pivotIdx=3, target=3 → 命中!
//           arr[0..3] = {1,3,2,4} 就是最小的4个
// ============================================================
class Solution3 {
public:
    vector<int> smallestK(vector<int>& arr, int k) {
        if (k == 0 || arr.empty()) return {};
        if (k >= (int)arr.size()) return arr;

        int targetIdx = k - 1;  // 前 k 小 = 让索引 k-1 就位
        int left = 0, right = (int)arr.size() - 1;

        while (left < right) {
            int pivotIdx = partition(arr, left, right);

            if (pivotIdx == targetIdx) {
                break;  // pivot 恰好在目标位置，arr[0..k-1] 就是答案
            } else if (pivotIdx < targetIdx) {
                left = pivotIdx + 1;   // 目标在右半
            } else {
                right = pivotIdx - 1;  // 目标在左半
            }
        }

        return vector<int>(arr.begin(), arr.begin() + k);
    }

private:
    int partition(vector<int>& arr, int left, int right) {
        // 【关键】随机选 pivot 避免有序数组退化到 O(n^2)
        int randomIdx = left + rand() % (right - left + 1);
        swap(arr[randomIdx], arr[right]);

        int pivot = arr[right];
        int storeIdx = left;  // storeIdx: 下一个放"<= pivot 的数"的位置

        for (int i = left; i < right; i++) {
            if (arr[i] <= pivot) {
                swap(arr[storeIdx], arr[i]);
                storeIdx++;
            }
        }
        // 把 pivot 放到最终位置
        swap(arr[storeIdx], arr[right]);
        return storeIdx;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法       | 时间        | 空间    | 修改原数组 | 适用场景          |
// |------------|-------------|---------|------------|-------------------|
// | 排序       | O(n log n)  | O(logn) | 是         | 快速实现/起步     |
// | 大顶堆     | O(n log k)  | O(k)   | 否         | 不能改数组/流数据 |
// | QuickSelect | O(n) avg   | O(1)   | 是         | 追求平均O(n)      |
//
// 面试建议：
// - 先写堆解法（简洁、不易出错、不改原数组）
// - 再提 QuickSelect 展示算法功底
// - 主动说明 QuickSelect 最坏 O(n^2) 和随机化解决方案
//
// 【易错点】
// 1. 堆方向选反：
//    ✗ priority_queue<int, vector<int>, greater<int>>（小顶堆）
//      弹出最小值，留下的是大的
//    ✓ priority_queue<int>（大顶堆）
//      弹出最大值，留下的是最小的 k 个
//
// 2. QuickSelect 中 partition 用 < 还是 <=：
//    ✗ if (arr[i] < pivot) → 大量重复元素时分割不均
//    ✓ if (arr[i] <= pivot) → 等于 pivot 的元素分散两侧
//
// 3. 忘记边界 k=0 或 k>=n：
//    ✗ 不检查直接调用 → k=0 时 targetIdx=-1 越界
//    ✓ if (k==0) return {}; if (k>=n) return arr;
//
// 4. QuickSelect 不随机化：
//    ✗ int pivot = arr[left]; → 有序数组 O(n^2)
//    ✓ swap(arr[left + rand()%(right-left+1)], arr[right]);
//
// 5. storeIdx 初始化错误：
//    ✗ int storeIdx = 0;  → 在子区间 [left,right] 上操作会出错
//    ✓ int storeIdx = left;
//
// 【面试追问】
// Q1: 暴力排序有什么不足？
//     → 排序确定了所有元素的顺序，但只需要前 k 小，做了大量无用功。
//
// Q2: 如果数据是流式到来，怎么动态维护最小 k 个？
//     → 用大顶堆。新元素比堆顶小就替换堆顶，每次 O(log k)。
//
// Q3: 不能修改原数组怎么办？
//     → 用堆解法 O(n log k)，或者拷贝一份再 QuickSelect。
//
// Q4: QuickSelect 最坏 O(n^2)，有确定性 O(n) 算法吗？
//     → BFPRT（Median of Medians）：5个一组取中位数做 pivot，
//       保证每次排除 3/10 元素。面试提到即可。
//
// Q5: 与 LC 215 的关系？
//     → 215 找第 k 大（单值），本题找前 k 小（k 个值）。
//       堆方向相反，QuickSelect 的 target 位置不同。
// ============================================================
