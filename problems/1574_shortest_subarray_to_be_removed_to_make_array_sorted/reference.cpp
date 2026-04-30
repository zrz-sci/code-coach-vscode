/*
 * LeetCode 1574: 删除最短的子数组使剩余数组有序
 * (Shortest Subarray to be Removed to Make Array Sorted)
 *
 * 【题目本质】
 * 删除一个连续子数组后，剩余部分 = 左前缀 + 右后缀。
 * 要让拼接结果非递减，需要：
 *   (1) 左前缀自身非递减
 *   (2) 右后缀自身非递减
 *   (3) 拼接处 arr[i] <= arr[j]
 * 目标：最小化删除区间的长度。
 *
 * 【解法总览】
 * 解法1: 暴力枚举   — O(n²) / O(1)  — 枚举每对拼接点
 * 解法2: 双指针     — O(n)  / O(1)  — 利用两段单调性，指针不回退 ⭐
 * 解法3: 二分查找   — O(n log n) / O(1) — 对有序后缀二分
 */

// ============================================================
// 解法1: 暴力枚举 — 对每个左端点，在右后缀中线性查找
// 时间: O(n²) 最坏  空间: O(1)
//
// 【思路】
// 最直接的做法：
// 1. 找最长非递减前缀 arr[0..left]
// 2. 找最长非递减后缀 arr[right..n-1]
// 3. 枚举每个左前缀端点 i，在后缀中找第一个 >= arr[i] 的 j
//
// 为什么这样做：删除 arr[i+1..j-1] 后，剩余 arr[0..i] + arr[j..n-1]
// 要非递减，需要两段各自有序且拼接处 arr[i] <= arr[j]
//
// 示意图:
//   arr = [1, 2, 3, 10, 4, 2, 3, 5]
//          ←前缀→              ←后缀→
//          0  1  2   3  4  5   6  7
//          left=2               right=6
//
//   只保留左前缀: 删除 arr[3..7] → [1,2,3]       删除5个
//   只保留右后缀: 删除 arr[0..5] → [3,5]          删除6个
//   拼接 i=2,j=6: 删除 arr[3..5] → [1,2,3,3,5]   删除3个 ✓
// ============================================================
class Solution1 {
public:
    int findLengthOfShortestSubarray(vector<int>& arr) {
        int n = arr.size();

        // 找最长非递减前缀
        int left = 0;
        while (left + 1 < n && arr[left] <= arr[left + 1]) left++;
        if (left == n - 1) return 0; // 整个数组已非递减

        // 找最长非递减后缀
        int right = n - 1;
        while (right > 0 && arr[right - 1] <= arr[right]) right--;

        // 初始答案：只保留左前缀 或 只保留右后缀
        int ans = min(n - left - 1, right);

        // 枚举左前缀端点 i
        for (int i = 0; i <= left; i++) {
            // 在后缀中线性找第一个 >= arr[i] 的位置
            for (int j = right; j < n; j++) {
                if (arr[i] <= arr[j]) {
                    // 删除 arr[i+1..j-1]，长度 = j - i - 1
                    ans = min(ans, j - i - 1);
                    break; // 后缀有序，第一个满足的就是最优
                }
            }
        }
        return ans;
    }
};

// ============================================================
// 解法2: 双指针 — 利用前缀和后缀都单调的性质 ⭐ 面试首选
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 解法1的瓶颈：对每个 i 都从 right 开始找 j，最坏 O(n²)
//
// 优化观察：前缀 arr[0..left] 非递减，所以 i 增大时 arr[i] 增大。
// arr[i] 变大后，满足 arr[j] >= arr[i] 的最小 j 只会增大或不变。
// 因此 j 不需要回退 → 经典双指针，总共 O(n)。
//
// 双指针过程 (arr = [1,2,3,10,4,2,3,5]):
//
//   i=0, arr[0]=1: j=6, arr[6]=3 >= 1 ✓ → 删除[1,5]长度5, ans=min(5,5)=5
//                  i++
//   i=1, arr[1]=2: j=6, arr[6]=3 >= 2 ✓ → 删除[2,5]长度3, ans=min(5,3)=3
//                  i++
//   i=2, arr[2]=3: j=6, arr[6]=3 >= 3 ✓ → 删除[3,5]长度3, ans=min(3,3)=3
//                  i++
//   i=3 > left=2, 退出
//
//   最终 ans = 3
// ============================================================
class Solution2 {
public:
    int findLengthOfShortestSubarray(vector<int>& arr) {
        int n = arr.size();

        // Step1: 找最长非递减前缀
        int left = 0;
        while (left + 1 < n && arr[left] <= arr[left + 1]) left++;
        if (left == n - 1) return 0; // 已经全部有序

        // Step2: 找最长非递减后缀
        int right = n - 1;
        while (right > 0 && arr[right - 1] <= arr[right]) right--;

        // Step3: 只保留一侧的基础答案
        // n - left - 1 = 删掉 arr[left+1..n-1]，只留左前缀
        // right        = 删掉 arr[0..right-1]，只留右后缀
        int ans = min(n - left - 1, right);

        // Step4: 双指针找最优拼接
        // i 遍历左前缀 [0, left]，j 从 right 开始向右
        int i = 0, j = right;
        while (i <= left && j < n) {
            if (arr[i] <= arr[j]) {
                // 拼接成功：保留 arr[0..i] + arr[j..n-1]
                // 删除 arr[i+1..j-1]，长度 = j - i - 1
                ans = min(ans, j - i - 1);
                // 尝试保留更多左前缀元素
                i++;
            } else {
                // arr[i] > arr[j]，拼接处不满足非递减
                // 需要让 arr[j] 更大 → j 右移
                j++;
            }
        }

        return ans;
    }
};

// ============================================================
// 解法3: 二分查找 — 对有序后缀做二分
// 时间: O(n log n)  空间: O(1)
//
// 【思路】
// 和双指针的前置分析一样，但不利用"i增大j不减"的性质。
// 对每个 i，在有序后缀 arr[right..n-1] 中二分查找第一个 >= arr[i] 的位置。
//
// 为什么能二分：后缀 arr[right..n-1] 是非递减的，满足二分条件。
//
// 二分搜索空间收缩示意 (arr[i]=3, 后缀=[3,5]):
//   Round1: [3, 5]  mid=3 >= 3  → right=mid (向左收缩)
//                ↑
//   找到 j=6 (index in original array)
// ============================================================
class Solution3 {
public:
    int findLengthOfShortestSubarray(vector<int>& arr) {
        int n = arr.size();

        int left = 0;
        while (left + 1 < n && arr[left] <= arr[left + 1]) left++;
        if (left == n - 1) return 0;

        int right = n - 1;
        while (right > 0 && arr[right - 1] <= arr[right]) right--;

        int ans = min(n - left - 1, right);

        for (int i = 0; i <= left; i++) {
            // 在 arr[right..n-1] 中找第一个 >= arr[i] 的位置
            // lower_bound 返回迭代器，转为索引
            int j = (int)(lower_bound(arr.begin() + right, arr.end(), arr[i]) - arr.begin());
            // 删除 arr[i+1..j-1]，长度 = j - i - 1
            ans = min(ans, j - i - 1);
        }

        return ans;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法      | 时间       | 空间 | 核心                                |
// |-----------|-----------|------|-------------------------------------|
// | 暴力枚举  | O(n²)     | O(1) | 对每个i线性扫描后缀                 |
// | 双指针 ⭐ | O(n)      | O(1) | 利用两段同时单调，j不回退            |
// | 二分查找  | O(n log n)| O(1) | 只利用后缀单调，对后缀二分           |
//
// 面试中优先写双指针解法，思路清晰且最优。
// 二分解法作为"想不到双指针时"的保底方案，也完全可以通过。
//
// 【易错点】
//
// 1. left 计算越界:
//    ✗ while (left < n && arr[left] <= arr[left+1])  → left+1 可能越界
//    ✓ while (left + 1 < n && arr[left] <= arr[left+1])
//
// 2. 忘记只保留一侧的初始答案:
//    ✗ int ans = n; 然后只在双指针循环中更新
//      → 如果前缀的所有元素都大于后缀所有元素，双指针不会更新 ans
//    ✓ int ans = min(n - left - 1, right); 先考虑只保留一侧
//
// 3. 删除长度算错:
//    ✗ ans = min(ans, j - i)     → 多删了一个
//    ✓ ans = min(ans, j - i - 1) → 删除的是 arr[i+1..j-1]
//    推导: 长度 = (j-1) - (i+1) + 1 = j - i - 1
//
// 4. 非递减 vs 严格递增:
//    ✗ arr[i] < arr[j]   → 这是严格递增
//    ✓ arr[i] <= arr[j]  → 题目要求非递减，相等也合法
//
// 5. 整个数组已有序的特判:
//    ✗ 不判断 left == n-1，直接计算 right
//      → 此时 right 的 while 循环不会执行，right = n-1
//      → ans = min(n-left-1, right) = min(0, n-1) = 0，结果碰巧对
//      → 但后续双指针中 j 从 n-1 开始可能产生负数长度
//    ✓ 提前判断 left == n-1 返回 0，逻辑更清晰安全
//
// 【面试追问】
//
// Q1: "为什么只考虑前缀+后缀拼接就够了？"
// → 删除连续子数组后，剩下的一定是原数组的某个前缀 + 某个后缀。
//   这个前缀不能超过最长非递减前缀(否则自身就不有序)，
//   后缀同理。所以搜索空间被限制在 [0,left] 和 [right,n-1] 中。
//
// Q2: "双指针的正确性：为什么 j 不需要回退？"
// → 前缀非递减 → i 增大时 arr[i] 不减。
//   假设对 i₀ 的最优 j 是 j₀ (即 arr[i₀] <= arr[j₀])。
//   对 i₀+1，arr[i₀+1] >= arr[i₀]，需要 arr[j] >= arr[i₀+1] >= arr[i₀]。
//   所以满足条件的最小 j 只会 >= j₀。j 不需要回退。
//
// Q3: "如果改成删除不连续的元素呢？"
// → 变成求 LIS (最长非递减子序列)，答案 = n - LIS长度。
//   LIS 可以 O(n log n) 求解 (patience sort / 二分)。
//   和本题的框架完全不同。
//
// Q4: "如果要输出具体删除了哪个子数组？"
// → 在双指针循环中，记录取得最小 ans 时的 (i, j)。
//   最终删除的子数组是 arr[best_i+1 .. best_j-1]。
// ============================================================
