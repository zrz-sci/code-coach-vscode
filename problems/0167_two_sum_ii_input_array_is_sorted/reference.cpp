/*
 * LeetCode 167: 两数之和 II - 输入有序数组
 *
 * 【题目本质】
 * 在已排序数组中找两个数使其和为 target，返回 1-indexed 下标。
 * 核心考点：如何利用有序性将 O(n²) 优化到 O(n)，且 O(1) 空间。
 *
 * 【解法总览】
 * 解法1: 暴力枚举     — O(n²)      / O(1) — 最直觉，不利用有序性
 * 解法2: 二分查找     — O(n log n) / O(1) — 利用有序性加速内层查找
 * 解法3: 双指针(对撞) — O(n)       / O(1) — 最优解，面试首选
 *
 * 【与 LeetCode 1 (Two Sum) 的关系】
 * - LeetCode 1: 数组无序，用哈希表 O(n)/O(n)
 * - 本题: 数组有序 + 要求 O(1) 空间 → 双指针 O(n)/O(1)
 * - 本题本质上就是 "如果数组有序且不能用额外空间" 的 follow-up
 */

// ============================================================
// 解法1: 暴力枚举 — 两层循环检查所有配对
// 时间: O(n²)  空间: O(1)
//
// 【思路】
// 最朴素的想法：枚举所有 (i, j) 对，检查和是否为 target。
// 完全没利用"有序"条件，但能帮助理解题意。
// 瓶颈：对每个 i，内层线性扫描找配对，时间浪费在查找上。
// ============================================================
class Solution1 {
public:
    vector<int> twoSum(vector<int>& numbers, int target) {
        int n = numbers.size();
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                if (numbers[i] + numbers[j] == target) {
                    return {i + 1, j + 1}; // 题目要求 1-indexed
                }
                // 小优化：有序数组中如果和已经超过 target，后面只会更大
                if (numbers[i] + numbers[j] > target) break;
            }
        }
        return {};
    }
};

// ============================================================
// 解法2: 二分查找 — 固定一个数，二分找另一个
// 时间: O(n log n)  空间: O(1)
//
// 【思路】
// 解法1的瓶颈是内层的线性查找 O(n)。
// 数组有序 → 可以用二分查找，把内层从 O(n) 降到 O(log n)。
//
// 对于每个 numbers[i]，需要找的配对值是 complement = target - numbers[i]。
// 在 [i+1, n-1] 范围内二分查找 complement。
//
// 二分搜索空间收缩过程 (numbers=[2,3,4], target=6, i=0, 找complement=4):
//   Round1: lo=1, hi=2, mid=1, numbers[1]=3 < 4 → lo=2
//   Round2: lo=2, hi=2, mid=2, numbers[2]=4 == 4 → 找到!
// ============================================================
class Solution2 {
public:
    vector<int> twoSum(vector<int>& numbers, int target) {
        int n = numbers.size();
        for (int i = 0; i < n; i++) {
            int complement = target - numbers[i];
            // 在 i 右侧二分查找（不能重复使用同一元素，所以从 i+1 开始）
            int lo = i + 1, hi = n - 1;
            while (lo <= hi) {
                int mid = lo + (hi - lo) / 2; // 防溢出的标准写法
                if (numbers[mid] == complement) {
                    return {i + 1, mid + 1};
                } else if (numbers[mid] < complement) {
                    lo = mid + 1; // complement 在右半区
                } else {
                    hi = mid - 1; // complement 在左半区
                }
            }
        }
        return {};
    }
};

// ============================================================
// 解法3: 双指针（对撞指针） — 从两端向中间夹逼 ⭐ 面试首选
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 核心观察：数组有序，最小在最左，最大在最右。
// 设 left = 0, right = n-1:
//   - sum == target → 找到答案
//   - sum < target  → 需要更大的和，left++ (让较小数变大)
//   - sum > target  → 需要更小的和，right-- (让较大数变小)
//
// 【为什么不会漏解？】
// 假设答案是 (i*, j*)。
// - left 从 0 右移到 i* 的过程中，每次右移是因为 sum < target，
//   说明当前 left 太小，和任何 right 配对都不够大，排除正确。
// - right 从 n-1 左移到 j* 同理。
// 所以双指针一定能在 (i*, j*) 处相遇。
//
// 【过程演示】 numbers = [2, 7, 11, 15], target = 9
//
//   Step1: left=0, right=3 → sum = 2+15 = 17 > 9 → right--
//   Step2: left=0, right=2 → sum = 2+11 = 13 > 9 → right--
//   Step3: left=0, right=1 → sum = 2+7  = 9 == 9 → 返回 [1, 2]
//
// ============================================================
class Solution3 {
public:
    vector<int> twoSum(vector<int>& numbers, int target) {
        int left = 0, right = numbers.size() - 1;

        // left < right 而不是 <=，因为两个下标不能相同
        while (left < right) {
            int sum = numbers[left] + numbers[right];
            if (sum == target) {
                return {left + 1, right + 1}; // 转换为 1-indexed
            } else if (sum < target) {
                left++;   // 和太小 → 左指针右移，增大较小值
            } else {
                right--;  // 和太大 → 右指针左移，减小较大值
            }
        }

        return {}; // 题目保证有解，实际不会执行到这里
    }
};

// ============================================================
// 【解法对比】
//
// | 解法       | 时间       | 空间 | 利用有序性 | 面试推荐 |
// |-----------|-----------|------|----------|---------|
// | 暴力枚举   | O(n²)     | O(1) | ❌       | 仅分析  |
// | 二分查找   | O(n logn) | O(1) | ✅ 加速查找 | 可提及  |
// | 双指针     | O(n)      | O(1) | ✅ 排除候选 | ⭐首选  |
//
// 双指针是最优解：时间 O(n)、空间 O(1)、代码最短。
// 二分查找的思维（固定一个找另一个）在三数之和等变体中也有用。
//
// 【易错点】
// 1. 下标忘记 +1:
//    ✗ return {left, right};         — 0-indexed，题目要求 1-indexed
//    ✓ return {left + 1, right + 1}; — 正确
//
// 2. 双指针循环条件用 <= 导致同一元素被用两次:
//    ✗ while (left <= right) — 当 left == right 时配对自己
//    ✓ while (left < right)  — 保证是两个不同位置
//
// 3. 二分查找的搜索起点:
//    ✗ int lo = 0;     — 可能找到 i 自身
//    ✓ int lo = i + 1; — 配对元素必须在 i 右侧
//
// 4. 二分中间值计算:
//    ✗ int mid = (lo + hi) / 2;       — lo+hi 可能溢出
//    ✓ int mid = lo + (hi - lo) / 2;  — 安全写法
//
// 【面试追问（递进链）】
//
// Q1: 为什么双指针保证不漏解？
// A1: 有序性保证：sum > target 时 right 与 left 右侧所有元素的和只会更大，
//     所以 right 和这些元素的配对都不可能是答案，right-- 安全排除。left++ 对称同理。
//
// Q2: 如果不限制空间，还有什么方法？
// A2: 哈希表，和 LeetCode 1 一样，边遍历边查 O(n)/O(n)。
//     但本题给了有序+O(1)空间约束，暗示双指针。
//
// Q3: 如果要找所有满足条件的配对（允许多个答案），怎么改？
// A3: 找到后不返回而是记录，然后 left++ 和 right-- 同时移动。
//     若有重复值，跳过相同元素去重（while(left<right && numbers[left]==numbers[left-1]) left++）。
//
// Q4: 如果扩展到三数之和呢？（LeetCode 15）
// A4: 排序后枚举第一个数 i，剩余部分用本题的双指针做两数之和。
//     外层 O(n) × 内层 O(n) = O(n²)。去重需要跳过重复的 i。
//
// 【相关题型】
// - 1. 两数之和: 无序数组，用哈希表 O(n)/O(n)。本题是有序+O(1)空间的 follow-up。
// - 15. 三数之和: 外层枚举 + 内层复用本题双指针。额外难点是去重。
// - 16. 最接近的三数之和: 同样的双指针框架，改为维护最小差值。
// - 653. 两数之和IV-BST: 中序遍历得有序数组 + 双指针，或双迭代器模拟。
// ============================================================
