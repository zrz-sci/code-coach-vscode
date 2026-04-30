/*
 * LeetCode 852: 山脉数组的峰顶索引 (Peak Index in a Mountain Array)
 *
 * 【题目本质】
 * 山脉数组先严格递增后严格递减，找到峰值的索引。
 * 本质是利用"二段性"做二分查找——站在 mid 比较 arr[mid] 与 arr[mid+1]
 * 就能判断在上升段还是下降段，从而每次排除一半搜索空间。
 *
 * 【解法总览】
 * 解法1: 线性扫描         — O(n)     / O(1) — 最直觉，不满足 O(log n) 要求
 * 解法2: 二分查找 ⭐      — O(log n) / O(1) — 面试首选
 * 解法3: 三分查找         — O(log n) / O(1) — 了解即可，更通用但本题无优势
 */

// ============================================================
// 解法1: 线性扫描 — 找第一个开始下降的位置
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 山脉数组先升后降，从左到右扫描，第一个满足 arr[i] > arr[i+1]
// 的位置 i 就是峰值。因为在峰值之前都是 arr[i] < arr[i+1]。
//
// 瓶颈：线性遍历没有利用山脉数组的结构信息，O(n) 不满足要求。
// ============================================================
class Solution1 {
public:
    int peakIndexInMountainArray(vector<int>& arr) {
        for (int i = 1; i < (int)arr.size() - 1; i++) {
            // 第一个"比右邻大"的位置就是峰值
            if (arr[i] > arr[i + 1]) {
                return i;
            }
        }
        return -1;  // 题目保证是山脉数组，不会到这里
    }
};

// ============================================================
// 解法2: 二分查找 — 利用山脉数组的"二段性" ⭐ 面试首选
// 时间: O(log n)  空间: O(1)
//
// 【思路】
// 从解法1的瓶颈出发：线性扫描逐个检查，能否跳着检查？
//
// 关键观察——山脉数组的"二段性"：
//   - 上升段: arr[mid] < arr[mid+1]  → 峰值在 mid 右边
//   - 下降段: arr[mid] > arr[mid+1]  → 峰值在 mid 或 mid 左边
//
// 站在 mid 只需看 arr[mid] 和 arr[mid+1] 的关系，就能判断在哪一侧，
// 从而每次排除一半搜索空间。
//
// 二分搜索空间收缩过程 (arr = [0, 2, 4, 7, 5, 3, 1]):
//
//  arr:  0  2  4  7  5  3  1
//  idx:  0  1  2  3  4  5  6
//                 ^
//              峰值 idx=3
//
//  Round1: left=0, right=6, mid=3
//          arr[3]=7 > arr[4]=5 → 下降段 → right=3
//                    搜索空间: [0..3]
//
//  Round2: left=0, right=3, mid=1
//          arr[1]=2 < arr[2]=4 → 上升段 → left=2
//                    搜索空间: [2..3]
//
//  Round3: left=2, right=3, mid=2
//          arr[2]=4 < arr[3]=7 → 上升段 → left=3
//                    搜索空间: [3..3]
//
//  left==right==3 → 返回 3 ✓
// ============================================================
class Solution2 {
public:
    int peakIndexInMountainArray(vector<int>& arr) {
        int left = 0, right = arr.size() - 1;
        
        // 使用 left < right 模板：找"边界"而非确切值
        // 循环结束时 left == right，收敛到峰值位置
        while (left < right) {
            int mid = left + (right - left) / 2;  // 防溢出写法
            
            if (arr[mid] < arr[mid + 1]) {
                // mid 在上升段：mid 不可能是峰值（右边更大）
                // 安全排除 mid 及其左边
                left = mid + 1;
            } else {
                // arr[mid] > arr[mid+1]
                // mid 在下降段或就是峰值
                // mid 有可能是答案，所以 right = mid（不能 mid-1）
                right = mid;
            }
        }
        
        return left;  // left == right，就是峰值索引
    }
};

// ============================================================
// 解法3: 三分查找 — 单峰函数的通用方法
// 时间: O(log n) (底数为 3/2，常数因子比二分大)  空间: O(1)
//
// 【思路】
// 三分查找把区间分成三段，取两个中间点 m1 和 m2：
//   - 若 arr[m1] < arr[m2]，峰值在 m1 右侧 → left = m1 + 1
//   - 若 arr[m1] > arr[m2]，峰值在 m2 左侧 → right = m2 - 1
//   - 每次排除约 1/3 的空间
//
// 三分查找更通用——对于任意单峰函数（甚至连续函数）都适用，
// 但本题有明确的二段性，二分就够了且更快。
// ============================================================
class Solution3 {
public:
    int peakIndexInMountainArray(vector<int>& arr) {
        int left = 0, right = arr.size() - 1;
        
        while (right - left > 2) {
            int m1 = left + (right - left) / 3;
            int m2 = right - (right - left) / 3;
            
            if (arr[m1] < arr[m2]) {
                // 峰值在 m1 右侧（m1 在上升段）
                left = m1 + 1;
            } else {
                // 峰值在 m2 左侧（m2 在下降段）
                right = m2 - 1;
            }
        }
        
        // 剩余不超过 3 个元素，线性找最大值
        int peak = left;
        for (int i = left + 1; i <= right; i++) {
            if (arr[i] > arr[peak]) peak = i;
        }
        return peak;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法     | 时间       | 空间  | 核心思想              | 适用场景               |
// |----------|-----------|-------|----------------------|----------------------|
// | 线性扫描  | O(n)      | O(1) | 找第一个下降点         | 理解题意的起步解         |
// | 二分查找⭐| O(log n)  | O(1) | 二段性：mid vs mid+1  | 面试首选，最简洁高效     |
// | 三分查找  | O(log n)  | O(1) | 单峰函数通用方法       | 无二段性的单峰问题       |
//
// 面试中直接写解法2。解法1作为思路铺垫口头说明即可。
//
// ============================================================
// 【易错点】
//
// 1. 循环条件 while(left <= right) + right = mid 会死循环
//    ✗ while (left <= right) { ... right = mid; }
//      → 当 left == right == mid 时，right = mid 不变，永远不退出
//    ✓ while (left < right) { ... right = mid; }
//      → left == right 时退出，正好指向峰值
//
// 2. right = mid - 1 跳过了峰值
//    ✗ if (arr[mid] > arr[mid+1]) right = mid - 1;
//      → mid 本身可能就是峰值，mid-1 跳过了它
//    ✓ if (arr[mid] > arr[mid+1]) right = mid;
//      → 保留 mid 作为候选
//
// 3. mid 计算溢出
//    ✗ int mid = (left + right) / 2;  // left+right 可能 > INT_MAX
//    ✓ int mid = left + (right - left) / 2;
//
// 4. left = mid 导致死循环（本题不会犯，但要理解）
//    如果写成 left = mid（而非 mid+1），当 left+1==right 时
//    mid = left，left = mid = left，不前进 → 死循环。
//    本题 arr[mid]<arr[mid+1] 时 left=mid+1，是安全的。
//
// ============================================================
// 【面试追问】
//
// Q1: 线性扫描能解吗？瓶颈在哪？
// → 可以 O(n) 找第一个下降点。瓶颈：没利用山脉数组的结构，逐个检查。
//
// Q2: 怎么优化到 O(log n)？为什么二分可行？
// → 山脉数组有"二段性"：比较 arr[mid] 和 arr[mid+1] 可以判断在
//   上升段还是下降段，每次排除一半。
//
// Q3: 如果不保证是山脉数组，只保证存在某个局部峰值，怎么办？
// → LeetCode 162 (Find Peak Element)。思路相同：arr[mid]<arr[mid+1]
//   说明右侧一定存在峰值（因为若一直递增到边界，边界就是峰值）。
//
// Q4: 找到峰值后，如何在山脉数组中查找某个目标值？
// → LeetCode 1095。先用本题方法找峰值 idx，然后在 arr[0..idx]（升序）
//   做标准二分，找不到再在 arr[idx..n-1]（降序）做反向二分。
//
// ============================================================
// 【相关题型】
//
// - 162. Find Peak Element
//   复用完全相同的二分模板（比较 arr[mid] 和 arr[mid+1]）。
//   区别：162 可能有多个峰值且不保证山脉结构，但二分仍能找到任意一个。
//
// - 1095. Find in Mountain Array
//   先用本题方法找峰值，再在两段各做一次二分。
//   复用：本题的峰值查找是第一步。
//
// - 34. Find First and Last Position of Element in Sorted Array
//   练习 left < right + right = mid 的"找左边界"二分模板。
//   本题找的是"上升→下降的分界点"，模板完全一样。
// ============================================================
