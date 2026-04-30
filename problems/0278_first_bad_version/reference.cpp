/*
 * LeetCode 278: 第一个错误的版本 (First Bad Version)
 *
 * 【题目本质】
 * 在 [1, n] 这个单调序列中找第一个满足 isBadVersion(v)==true 的位置。
 * 序列结构: [false, false, ..., false, true, true, ..., true]
 * 这是经典的"找左边界"二分查找问题。
 *
 * 【解法总览】
 * 解法1: 线性扫描    — O(n) / O(1)     — 最直觉，但 TLE
 * 解法2: 二分查找    — O(log n) / O(1)  — 最优，面试必写
 */

// The API isBadVersion is defined for you.
// bool isBadVersion(int version);

// ============================================================
// 解法1: 线性扫描 — 从 1 到 n 逐个检查
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 最朴素的想法：从版本 1 开始一个个问，第一个返回 true 的就是答案。
// 这个解法的价值在于确认理解题意，但 n 最大 2^31-1，会超时。
//
// 搜索过程 (n=5, bad=4):
//   版本:  1     2     3     4     5
//   状态: good  good  good  BAD   BAD
//          ↑检查  ↑检查  ↑检查  ↑命中! → 返回 4
// ============================================================
class Solution1 {
public:
    int firstBadVersion(int n) {
        for (int i = 1; i <= n; i++) {
            if (isBadVersion(i)) {
                return i;
            }
        }
        return n; // 题目保证存在坏版本
    }
};

// ============================================================
// 解法2: 二分查找 — 利用单调性每次排除一半
// 时间: O(log n)  空间: O(1)
//
// 【思路】
// 解法1 的瓶颈：逐个检查，没有利用序列的结构。
// 关键观察：版本状态是 [false...false, true...true]，具有单调性。
// 我们要找的是"第一个 true"，即左边界。
//
// 二分逻辑：
//   - isBadVersion(mid) == true  → 答案在 [left, mid]（mid 可能是答案）
//   - isBadVersion(mid) == false → 答案在 [mid+1, right]（mid 一定不是答案）
//
// 使用 left < right 模板：退出时 left == right 就是答案。
//
// 搜索空间收缩过程 (n=5, bad=4):
//
//   版本:  1     2     3     4     5
//   状态: good  good  good  BAD   BAD
//
//   Round1: left=1, right=5, mid=3
//           isBadVersion(3)=false → left=4
//           搜索区间: [4, 5]
//
//   Round2: left=4, right=5, mid=4
//           isBadVersion(4)=true → right=4
//           搜索区间: [4, 4]
//
//   left==right==4 → 返回 4 ✓
//
// 为什么用 left + (right - left) / 2 而不是 (left + right) / 2？
// 因为 n 最大 2^31-1，left + right 可能溢出 int 范围。
// ============================================================
class Solution2 {
public:
    int firstBadVersion(int n) {
        int left = 1, right = n;
        
        while (left < right) {
            // 防溢出：等价于 (left + right) / 2
            int mid = left + (right - left) / 2;
            
            if (isBadVersion(mid)) {
                // mid 是坏版本 → 第一个坏版本 ≤ mid
                // mid 本身可能就是答案，所以不能 mid-1
                right = mid;
            } else {
                // mid 是好版本 → 第一个坏版本 > mid
                // mid 一定不是答案，安全跳过
                left = mid + 1;
            }
        }
        
        // 循环退出条件: left == right，即为第一个坏版本
        return left;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法       | 时间     | 空间  | API调用次数 | 能否AC |
// |-----------|----------|-------|------------|--------|
// | 线性扫描   | O(n)     | O(1)  | 最多 n 次   | ✗ TLE  |
// | 二分查找   | O(log n) | O(1)  | ~31 次     | ✓      |
//
// 面试直接写二分，但可以先口述线性解法展示理解。
//
// 【二分模板选择说明】
// 本题是"找左边界"问题，有三种常见写法：
//
// 模板A: left <= right (找确切值)
//   → 不适合本题，配合 right=mid 会死循环
//
// 模板B: left < right (找边界) ← 本题使用
//   → right=mid 不跳过可能的答案
//   → left=mid+1 跳过确定不是答案的
//   → 退出时 left==right 就是答案
//
// 模板C: left + 1 < right (安全模板)
//   → 退出时 left 和 right 相邻，需要后处理
//   → 可以用但多一步判断，不如模板B简洁
//
// 【易错点】
//
// 1. 整数溢出:
//    ✗ int mid = (left + right) / 2;
//      → left=2000000000, right=2100000000 时溢出为负数
//    ✓ int mid = left + (right - left) / 2;
//      → 先减后加，中间结果不超过 right
//
// 2. right 更新搞反:
//    ✗ right = mid - 1;
//      → 当 mid 恰好是第一个坏版本时，跳过了正确答案
//      → 例: [G, B, B]，mid=2 是坏版本，right=1，最终返回1(错)
//    ✓ right = mid;
//      → mid 是坏版本 → 答案 ≤ mid，不能排除 mid
//
// 3. 循环条件 left<=right 配 right=mid 导致死循环:
//    ✗ while(left <= right) { right = mid; }
//      → 当 left==right==mid 且 isBadVersion(mid)==true 时
//        right=mid 不改变任何值，永远不退出
//    ✓ while(left < right) → left==right 时自然退出
//
// 4. 搜索起点错误:
//    ✗ int left = 0;  → 版本号从1开始，不存在版本0
//    ✓ int left = 1;
//
// 【面试追问】
//
// Q1: 暴力 O(n) 能过吗？瓶颈在哪？
// → 不能，n 最大约 21 亿。瓶颈是逐个检查没利用单调结构。
//
// Q2: 为什么用 left < right 而不是 left <= right？
// → 因为当 isBadVersion(mid)==true 时 right=mid（不能跳过 mid），
//   如果用 left<=right，当 left==right==mid 时 right=mid 不改变值，死循环。
//   用 left<right，left==right 时直接退出，此时指向的就是答案。
//
// Q3: 能否进一步减少 API 调用次数？
// → 不能。这是信息论的下界：n 个版本需要 log₂(n) 次二值询问来定位。
//   二分每次获得 1 bit 信息，已经是最优的。
//
// Q4: 如果版本不是连续编号，而是给你一个有序数组，怎么改？
// → 把 left/right 从版本号改成数组索引，mid 取的是 arr[mid]，
//   其他逻辑完全一样。这就是 LeetCode 34/35 的思路。
//
// 【相关题型】
// - 34. 在排序数组中查找第一个和最后一个位置
//   → 复用同样的"找左边界"模板，找右边界时改成 left=mid（向右收缩）
// - 35. 搜索插入位置
//   → 完全相同的模板，找第一个 ≥ target 的位置
// - 69. x 的平方根
//   → 搜索空间从版本号换成 [0, x]，判断条件从 isBadVersion 换成 mid*mid > x
// - 374. 猜数字大小
//   → 几乎一模一样，API 从 isBadVersion 换成 guess，二分模板直接复用
// ============================================================
