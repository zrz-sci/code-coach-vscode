/*
 * LeetCode 915: 分隔数组为连续子数组 (Partition Array into Disjoint Intervals)
 *
 * 【题目本质】
 * 在数组中找到最小的分割点 i，使得 left=nums[0..i-1] 的最大值
 * <= right=nums[i..n-1] 的最小值。
 * 核心技巧：前缀最大值 + 后缀最小值预处理，或一次遍历贪心。
 *
 * 【解法总览】
 * 解法1: 前缀最大 + 后缀最小   — O(n) / O(n) — 直观易懂
 * 解法2: 一次遍历贪心 ⭐       — O(n) / O(1) — 面试首选
 */

// ============================================================
// 解法1: 前缀最大 + 后缀最小 — 预处理两个数组
// 时间: O(n)  空间: O(n)
//
// 【思路】
// leftMax[i] = max(nums[0..i])，从左到右递推
// rightMin[i] = min(nums[i..n-1])，从右到左递推
// 找最小的 i 使得 leftMax[i] <= rightMin[i+1]，返回 i+1
//
// 预处理过程 (nums = [5, 0, 3, 8, 6]):
//
// nums:     5  0  3  8  6
// leftMax:  5  5  5  8  8    (前缀最大值)
// rightMin: 0  0  3  6  6    (后缀最小值)
//
// 检查分割点:
//   i=0: leftMax[0]=5 > rightMin[1]=0  ✗
//   i=1: leftMax[1]=5 > rightMin[2]=3  ✗
//   i=2: leftMax[2]=5 <= rightMin[3]=6 ✓ → 返回 3
// ============================================================
class Solution1 {
public:
    int partitionDisjoint(vector<int>& nums) {
        int n = nums.size();

        // 构建前缀最大值数组
        // leftMax[i] 表示 nums[0], nums[1], ..., nums[i] 的最大值
        vector<int> leftMax(n);
        leftMax[0] = nums[0];
        for (int i = 1; i < n; i++) {
            leftMax[i] = max(leftMax[i - 1], nums[i]);
        }

        // 构建后缀最小值数组
        // rightMin[i] 表示 nums[i], nums[i+1], ..., nums[n-1] 的最小值
        vector<int> rightMin(n);
        rightMin[n - 1] = nums[n - 1];
        for (int i = n - 2; i >= 0; i--) {
            rightMin[i] = min(rightMin[i + 1], nums[i]);
        }

        // 找最小的分割点
        // left = nums[0..i], right = nums[i+1..n-1]
        // 条件: leftMax[i] <= rightMin[i+1]
        for (int i = 0; i < n - 1; i++) {
            if (leftMax[i] <= rightMin[i + 1]) {
                return i + 1;  // 返回 left 的长度
            }
        }

        return -1;  // 题目保证有解，不会到这里
    }
};

// ============================================================
// 解法2: 一次遍历贪心 — 动态维护分割点 ⭐ 面试首选
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 从解法1的 O(n) 空间优化到 O(1)。
//
// 维护三个变量:
//   partitionIdx: 当前分割点（左半部分的最后一个索引）
//   leftMax:      左半部分 nums[0..partitionIdx] 的最大值
//   globalMax:    到目前遍历位置为止所有元素的最大值
//
// 核心逻辑:
//   对于 nums[i]，如果 nums[i] < leftMax，说明 nums[i]
//   不能放在右半部分（因为右半部分每个元素都要 >= leftMax），
//   所以必须把左半部分扩展到包含 nums[i]。
//   扩展后，leftMax 更新为 globalMax（因为左半部分现在包含了
//   nums[0..i] 中的所有元素）。
//
// 贪心过程 (nums = [1, 1, 1, 0, 6, 12]):
//
// 初始: partitionIdx=0, leftMax=1, globalMax=1
//
// i=1: nums[1]=1, globalMax=1
//      1 >= leftMax=1 → 不扩展
//
// i=2: nums[2]=1, globalMax=1
//      1 >= leftMax=1 → 不扩展
//
// i=3: nums[3]=0, globalMax=1
//      0 < leftMax=1 → 必须扩展！
//      partitionIdx=3, leftMax=globalMax=1
//
// i=4: nums[4]=6, globalMax=6
//      6 >= leftMax=1 → 不扩展
//
// i=5: nums[5]=12, globalMax=12
//      12 >= leftMax=1 → 不扩展
//
// 结果: partitionIdx=3, 返回 4
// left=[1,1,1,0], right=[6,12] ✓
// ============================================================
class Solution2 {
public:
    int partitionDisjoint(vector<int>& nums) {
        int n = nums.size();
        int partitionIdx = 0;       // 左半部分的最后一个索引
        int leftMax = nums[0];      // 左半部分的最大值
        int globalMax = nums[0];    // 全局最大值（到目前位置）

        for (int i = 1; i < n; i++) {
            // 每一步都更新全局最大值
            globalMax = max(globalMax, nums[i]);

            if (nums[i] < leftMax) {
                // nums[i] 比左半部分最大值还小
                // → 它不可能放在右半部分（违反条件）
                // → 扩展左半部分到 i
                partitionIdx = i;
                // 扩展后左半部分包含 nums[0..i]
                // 其最大值就是到 i 为止的全局最大值
                leftMax = globalMax;
            }
        }

        // 返回左半部分的长度 = partitionIdx + 1
        return partitionIdx + 1;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法                  | 时间  | 空间  | 核心思想                      | 适用场景       |
// |-----------------------|-------|-------|------------------------------|---------------|
// | 前缀最大+后缀最小     | O(n) | O(n) | 预处理两个数组，扫描比较       | 思路清晰的起步解 |
// | 一次遍历贪心 ⭐        | O(n) | O(1) | 动态维护 leftMax 和 globalMax | 面试首选       |
//
// 面试中口述解法1的思路作为铺垫，然后写解法2。
//
// ============================================================
// 【易错点】
//
// 1. 返回的是长度而非索引
//    ✗ return partitionIdx;     // 这是最后一个左半索引
//    ✓ return partitionIdx + 1; // 这是左半部分的长度
//
// 2. 混淆 leftMax 和 globalMax
//    leftMax 只在扩展分割点时才更新（跳到 globalMax）。
//    globalMax 每步都更新。如果把两者混为一谈，
//    会导致不必要的扩展或者遗漏必要的扩展。
//
// 3. 前缀/后缀数组边界
//    leftMax[0] = nums[0]（不是 INT_MIN）
//    rightMin[n-1] = nums[n-1]（不是 INT_MAX）
//    分割点 i 的范围是 [0, n-2]（left 和 right 都不能为空）
//
// 4. 初始化 partitionIdx=0 而非 -1
//    左半部分至少包含第一个元素，所以初始分割点是索引 0。
//
// ============================================================
// 【面试追问】
//
// Q1: 暴力做法是什么？时间复杂度？
// → 枚举每个分割点，分别计算 max(left) 和 min(right)。
//   时间 O(n^2)。瓶颈：重复计算聚合值。
//
// Q2: 贪心为什么正确？
// → nums[i] < leftMax 时 nums[i] 必须在左半部分（否则违反条件）。
//   扩展后 leftMax = globalMax 是因为左半部分包含了所有扫过的元素。
//   只在必要时扩展，保证了分割点最小。
//
// Q3: 这道题和 Trapping Rain Water 有什么联系？
// → 都用"前缀最大值/后缀最小值"预处理。42 题是取 min(leftMax, rightMax)
//   再减去当前高度得到水量。915 是比较 leftMax 和 rightMin 找分割点。
//
// ============================================================
// 【相关题型】
//
// - 238. Product of Array Except Self
//   前缀乘积 + 后缀乘积的经典应用。同样的"两次遍历预处理"模式。
//
// - 42. Trapping Rain Water
//   前缀最大 + 后缀最大的经典应用。预处理思路完全对称。
//
// - 135. Candy
//   类似的"正向遍历+反向遍历"确定每个位置的值。
// ============================================================
