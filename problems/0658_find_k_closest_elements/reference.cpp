/*
 * LeetCode 658: Find K Closest Elements (找到 K 个最接近的元素)
 *
 * 【题目本质】
 * 在有序数组中找一个长度为 k 的连续窗口，使窗口内元素离 x 最近。
 * 关键性质：数组有序 → 答案一定是连续的 k 个元素。
 *
 * 【解法总览】
 * 解法1: 排序法        — O(n log n) / O(n)        — 最直觉，没利用有序性
 * 解法2: 双指针收缩    — O(n) / O(1)              — 面试首选，简洁高效
 * 解法3: 二分查找窗口  — O(log(n-k) + k) / O(1)   — 最优，面试加分
 * 解法4: 堆            — O(n log k) / O(k)        — 通用 Top-K，备选
 */

// ============================================================
// 解法1: 排序法 — 按距离排序取前 k 个
// 时间: O(n log n)  空间: O(n)
//
// 【思路】
// 完全没利用有序性的暴力思路：
// 把所有元素按到 x 的距离排序，取前 k 个，再排回升序。
// 面试中用这个开场展示理解，然后主动提出优化。
// ============================================================
class Solution1 {
public:
    vector<int> findClosestElements(vector<int>& arr, int k, int x) {
        vector<int> sorted_arr(arr.begin(), arr.end());

        // 自定义排序：按距离升序，距离相等按值升序
        sort(sorted_arr.begin(), sorted_arr.end(), [x](int a, int b) {
            int da = abs(a - x), db = abs(b - x);
            return da != db ? da < db : a < b;
        });

        // 取前 k 个（距离最小的 k 个）
        vector<int> result(sorted_arr.begin(), sorted_arr.begin() + k);

        // 题目要求结果按升序
        sort(result.begin(), result.end());
        return result;
    }
};

// ============================================================
// 解法2: 双指针收缩 — 从两端去掉离 x 更远的元素
// 时间: O(n)  空间: O(1)（不算输出）
//
// 【思路】
// 核心观察：答案是连续的 k 个元素（数组有序保证）。
// 从整个数组 [0, n-1] 开始，每次比较两端到 x 的距离，
// 去掉更远的那端，重复 n-k 次。
//
// 为什么正确？每次去掉的一定不在最优窗口里：
// 两端中离 x 更远的那个，一定不如窗口中间的元素优。
//
// 示例: arr = [1,2,3,4,5], k=4, x=3
//
// 初始:  [1, 2, 3, 4, 5]   left=0, right=4
//         ↑              ↑
//        |1-3|=2  vs  |5-3|=2  → 相等,去右端 (取较小值)
//
// Step1: [1, 2, 3, 4]       left=0, right=3
//         ↑        ↑
//        right - left = 3 = k - 1 → 停止
//
// 结果: [1, 2, 3, 4] ✓
// ============================================================
class Solution2 {
public:
    vector<int> findClosestElements(vector<int>& arr, int k, int x) {
        int left = 0, right = (int)arr.size() - 1;

        // 每次去掉一个离 x 更远的端点
        // 当 right - left == k - 1 时，窗口恰好 k 个元素，停止
        while (right - left >= k) {
            // 为什么用 >= k 而不是 > k？
            // right - left == k 表示窗口大小是 k+1，还需收缩一次
            if (abs(arr[left] - x) > abs(arr[right] - x)) {
                left++;   // 左端离 x 更远，去掉
            } else {
                // 右端更远，或距离相等（相等时去右端，保留较小值）
                right--;
            }
        }

        return vector<int>(arr.begin() + left, arr.begin() + right + 1);
    }
};

// ============================================================
// 解法3: 二分查找窗口左端点 — 最优解
// 时间: O(log(n-k) + k)  空间: O(1)（不算输出）
//
// 【思路】
// 解法2是 O(n)，瓶颈在线性扫描。能不能更快？
//
// 答案是长度 k 的窗口，左端点范围是 [0, n-k]。
// 我们在这个范围上二分：对于候选左端点 mid，
// 比较"窗口左边界" arr[mid] 和"窗口右边界外第一个" arr[mid+k] 到 x 的距离。
//
// 如果 x - arr[mid] > arr[mid+k] - x:
//   → 左端太远，窗口应该右移 → left = mid + 1
// 否则:
//   → 窗口位置合适或应左移 → right = mid
//
// 【为什么不取绝对值？】
// 不取绝对值能正确处理 x 在窗口外的情况：
// - x 在左边: x - arr[mid] < 0, 一定 < arr[mid+k] - x, 窗口左移 ✓
// - x 在右边: arr[mid+k] - x < 0, 一定 < x - arr[mid], 窗口右移 ✓
// - x 在中间: 两个差值都≥0, 直接比大小 ✓
// - 距离相等: 条件不成立, right=mid, 偏左(取较小值) ✓
//
// 二分搜索过程示例: arr = [1,2,3,4,5], k=4, x=3
// 搜索空间: 窗口左端点 ∈ [0, 1]
//
// Round1: left=0, right=1, mid=0
//   x - arr[0] = 3-1 = 2
//   arr[0+4] - x = 5-3 = 2
//   2 > 2? No → right = 0
//
// left == right == 0 → 窗口 = arr[0..3] = [1,2,3,4] ✓
// ============================================================
class Solution3 {
public:
    vector<int> findClosestElements(vector<int>& arr, int k, int x) {
        int left = 0, right = (int)arr.size() - k;

        // 二分查找最优窗口的左端点
        while (left < right) {
            int mid = left + (right - left) / 2;

            // 比较窗口左端 arr[mid] 和窗口右端外 arr[mid+k] 到 x 的距离
            // 不取绝对值！原因见上方注释
            if (x - arr[mid] > arr[mid + k] - x) {
                // 左端太远了，窗口右移
                left = mid + 1;
            } else {
                // 右端更远或相等，窗口左移或保持
                right = mid;
            }
        }

        // left 就是最优窗口的左端点
        return vector<int>(arr.begin() + left, arr.begin() + left + k);
    }
};

// ============================================================
// 解法4: 堆 (Priority Queue) — 通用 Top-K 思路
// 时间: O(n log k)  空间: O(k)
//
// 【思路】
// 没利用有序性的通用方法：维护大小为 k 的大顶堆，
// 堆顶是离 x 最远的元素。遍历数组，如果当前元素
// 比堆顶更近，替换堆顶。
//
// 什么时候用这个？如果数组无序，或者面试时一时想不到
// 二分怎么写，这个方法也能 AC。
// ============================================================
class Solution4 {
public:
    vector<int> findClosestElements(vector<int>& arr, int k, int x) {
        // 大顶堆：堆顶是离 x 最远的（距离最大，距离相等则值最大）
        auto cmp = [x](int a, int b) {
            int da = abs(a - x), db = abs(b - x);
            return da != db ? da < db : a < b;
            // 注意 priority_queue 的比较函数含义：
            // 返回 true 表示 a 优先级低于 b（b 在堆顶）
            // 所以距离小的优先级低，距离大的在堆顶
        };
        priority_queue<int, vector<int>, decltype(cmp)> pq(cmp);

        for (int num : arr) {
            pq.push(num);
            if ((int)pq.size() > k) {
                pq.pop(); // 去掉离 x 最远的
            }
        }

        vector<int> result;
        while (!pq.empty()) {
            result.push_back(pq.top());
            pq.pop();
        }

        // 题目要求升序输出
        sort(result.begin(), result.end());
        return result;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法         | 时间           | 空间  | 利用有序性 | 代码难度 |
// |-------------|---------------|-------|----------|---------|
// | 排序法       | O(n log n)    | O(n)  | ✗        | 低      |
// | 双指针收缩   | O(n)          | O(1)  | ✓        | 低      |
// | 二分查找     | O(log(n-k)+k) | O(1)  | ✓✓       | 中      |
// | 堆          | O(n log k)    | O(k)  | ✗        | 中      |
//
// 面试策略:
// 1. 先说双指针收缩 (简洁+O(n)，不易出错)
// 2. 被追问"能否更快"时给出二分解法
// 3. 如果数组无序，说堆或排序
//
// 【易错点】
// 1. 二分中取绝对值:
//    ✗ if (abs(x - arr[mid]) > abs(arr[mid+k] - x))
//    ✓ if (x - arr[mid] > arr[mid+k] - x)
//    不取绝对值利用了符号信息，天然处理 x 在窗口外的情况，
//    且距离相等时保证取较小值。
//
// 2. 双指针循环条件:
//    ✗ while (right - left > k)   — 窗口大小 k+1 时不再收缩，多留一个
//    ✓ while (right - left >= k)  — 窗口 k+1 时还要收缩一次
//
// 3. 二分 right 初始值:
//    ✗ right = arr.size() - 1     — 会导致 arr[mid+k] 越界
//    ✓ right = arr.size() - k     — 窗口左端点最大到 n-k
//
// 4. 二分 arr[mid+k] 越界检查:
//    left < right 且 right = n-k 保证 mid ≤ n-k-1，
//    所以 mid+k ≤ n-1，不越界。但如果错写 right = n-k+1 就会越界！
//
// 【面试追问】
// Q1: 暴力解能过吗？→ O(n log n) 能过但没利用有序性，面试需利用。
//
// Q2: 为什么答案一定连续？→ 反证法：如果不连续，跳过的 arr[i+1]
//     一定比 arr[i] 或 arr[j] 更近（有序性），应该替换，矛盾。
//
// Q3: O(n) 能否更快？→ 二分搜索窗口左端点 O(log(n-k))。
//     搜索空间: [0, n-k]，比较窗口两端到 x 的距离决定方向。
//
// Q4: 数组无序怎么办？→ 堆 O(n log k) 或 Quick Select O(n) 平均。
//
// Q5: 大量查询优化？→ 排序一次，每次查询 O(log(n-k)+k)。
// ============================================================
