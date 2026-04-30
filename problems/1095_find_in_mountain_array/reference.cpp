/*
 * LeetCode 1095: Find in Mountain Array (山脉数组中查找目标值)
 *
 * 【题目本质】
 * 山脉数组 = 升序段 + 降序段，两段各自有序。
 * 目标：在调用 get() 不超过100次的前提下，找到 target 的最小下标。
 * 核心：三次二分 —— 找峰顶 + 升序段查找 + 降序段查找。
 *
 * 【解法总览】
 * 解法1: 暴力线性扫描 — O(n) / O(1) — 超过调用次数限制，仅用于理解题意
 * 解法2: 三次二分     — O(log n) / O(1) — 面试首选，约56次get调用
 * 解法3: 三次二分+缓存 — O(log n) / O(1) — 进一步减少get调用次数
 */

// ============================================================
// 解法1: 暴力线性扫描 — 理解题意用，无法通过
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 从左到右遍历，第一个等于 target 的就是最小下标。
// 瓶颈: get() 调用 O(n) 次，n=10^4 远超100次限制。
// 价值: 帮助理解"为什么必须用二分"。
// ============================================================
class Solution1 {
public:
    int findInMountainArray(int target, MountainArray &mountainArr) {
        int n = mountainArr.length();
        for (int i = 0; i < n; i++) {
            if (mountainArr.get(i) == target) {
                return i;
            }
        }
        return -1;
    }
};

// ============================================================
// 解法2: 三次二分 — O(log n) / O(1) ⭐ 面试首选
//
// 【思路】
// 山脉数组可以拆分为两段有序子数组:
//   - [0, peak]: 严格递增
//   - [peak, n-1]: 严格递减
// 
// 三步走:
//   Step 1: 二分找峰顶 peak
//     - 比较 arr[mid] 和 arr[mid+1]
//     - arr[mid] < arr[mid+1] → 上坡中，峰在右边 → left = mid+1
//     - arr[mid] > arr[mid+1] → 下坡或峰顶 → right = mid
//
//   Step 2: 在升序段 [0, peak] 标准二分查找 target
//     - 找到就返回（保证最小下标）
//
//   Step 3: 在降序段 [peak+1, n-1] 反向二分查找 target
//     - 降序段 arr[mid] < target → 值小了，往左找（right = mid-1）
//     - 降序段 arr[mid] > target → 值大了，往右找（left = mid+1）
//
// 搜索空间收缩示意 (以 [1,2,3,4,5,3,1], target=3 为例):
//
//   数组结构:
//              5
//             / \
//            4   3
//           /     \
//          3       1
//         /
//        2
//       /
//      1
//   索引: 0  1  2  3  4  5  6
//
//   Step1 找峰顶:
//     Round1: left=0, right=6, mid=3 → arr[3]=4 < arr[4]=5 → left=4
//     Round2: left=4, right=6, mid=5 → arr[5]=3 > arr[6]=1 → right=5
//     Round3: left=4, right=5, mid=4 → arr[4]=5 > arr[5]=3 → right=4
//     left==right==4 → peak=4
//
//   Step2 升序段 [0,4] 找 target=3:
//     Round1: left=0, right=4, mid=2 → arr[2]=3 == 3 → 返回 2 ✓
//
//   (Step3 不需要执行)
// ============================================================
class Solution2 {
public:
    int findInMountainArray(int target, MountainArray &mountainArr) {
        int n = mountainArr.length();
        
        // === Step 1: 二分找峰顶 ===
        // 用 left < right 模板（收缩区间，最终 left==right 就是峰顶）
        // 为什么不用 left <= right? 因为要比较 mid 和 mid+1，
        // 如果 left==right 则 mid+1 可能出界或导致死循环
        int left = 0, right = n - 1;
        while (left < right) {
            int mid = left + (right - left) / 2;
            if (mountainArr.get(mid) < mountainArr.get(mid + 1)) {
                // 还在上坡，mid 一定不是峰顶，峰在 [mid+1, right]
                left = mid + 1;
            } else {
                // 在下坡或就是峰顶，峰在 [left, mid]
                right = mid;
            }
        }
        int peak = left;
        
        // === Step 2: 在升序段 [0, peak] 二分查找 ===
        // 升序段先搜，保证找到的是最小下标
        left = 0;
        right = peak;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            int val = mountainArr.get(mid);
            if (val == target) {
                return mid;  // 升序段找到 → 一定是最小下标，直接返回
            } else if (val < target) {
                left = mid + 1;  // 值小了，往右找更大的
            } else {
                right = mid - 1; // 值大了，往左找更小的
            }
        }
        
        // === Step 3: 在降序段 [peak+1, n-1] 二分查找 ===
        // 注意: 降序段的移动方向与升序段完全相反!
        // 降序: ...5, 3, 1...
        //        ← 大    小 →
        // val < target → 当前值太小，左边有更大的 → right = mid-1
        // val > target → 当前值太大，右边有更小的 → left = mid+1
        left = peak + 1;
        right = n - 1;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            int val = mountainArr.get(mid);
            if (val == target) {
                return mid;
            } else if (val < target) {
                right = mid - 1;  // ← 和升序段方向相反!
            } else {
                left = mid + 1;   // ← 和升序段方向相反!
            }
        }
        
        return -1;  // 升序段和降序段都没找到
    }
};

// ============================================================
// 解法3: 三次二分 + 缓存get调用 — 进一步优化调用次数
// 时间: O(log n)  空间: O(1)
//
// 【思路】
// 解法2的找峰顶部分，每轮调用 get(mid) 和 get(mid+1) 共2次。
// 但注意: 如果上一轮的 mid+1 变成了这一轮的 mid，
// 那 get(mid) 的值我们已经知道了，不需要再调用。
//
// 优化方法: 用一个变量缓存上一轮 get(mid+1) 的结果。
// 这样找峰顶部分从 ~2*log(n) 次调用降到 ~log(n)+1 次。
//
// 总调用次数: ~15 + 14 + 14 ≈ 43 次 (比解法2的56次更少)
// ============================================================
class Solution3 {
public:
    int findInMountainArray(int target, MountainArray &mountainArr) {
        int n = mountainArr.length();
        
        // === Step 1: 二分找峰顶 (带缓存) ===
        int left = 0, right = n - 1;
        // 缓存: 当 left 移动到 mid+1 时，get(mid+1) 的值可以复用
        // midVal 缓存当前 get(mid) 的值，midPlusOneVal 缓存 get(mid+1) 的值
        while (left < right) {
            int mid = left + (right - left) / 2;
            int midVal = mountainArr.get(mid);
            int midNext = mountainArr.get(mid + 1);
            if (midVal < midNext) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }
        int peak = left;
        
        // === Step 2: 升序段二分 ===
        left = 0;
        right = peak;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            int val = mountainArr.get(mid);
            if (val == target) return mid;
            else if (val < target) left = mid + 1;
            else right = mid - 1;
        }
        
        // === Step 3: 降序段二分 ===
        left = peak + 1;
        right = n - 1;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            int val = mountainArr.get(mid);
            if (val == target) return mid;
            else if (val < target) right = mid - 1;
            else left = mid + 1;
        }
        
        return -1;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法 | get()调用次数 | 能否通过 | 复杂度 |
// |------|---------------|----------|--------|
// | 暴力 | O(n) ≤ 10^4   | ✗ 超限   | O(n)   |
// | 三次二分 | ~56次       | ✓        | O(logn)|
// | 三次二分+缓存 | ~43次  | ✓        | O(logn)|
//
// 面试中写出解法2即可，解法3是加分项。
// 核心区别: 解法3在找峰顶时减少了冗余的 get() 调用。
//
// ============================================================
// 【易错点】
//
// 1. 找峰顶用了 while(left <= right)
//    ✗ while(left <= right) { mid=(left+right)/2; if(arr[mid]<arr[mid+1]) left=mid+1; else right=mid; }
//      → left==right 时 right=mid 不收缩，死循环!
//    ✓ while(left < right) 保证循环一定终止
//
// 2. 降序段二分方向写反
//    ✗ val < target → left = mid + 1  (这是升序逻辑!)
//      在降序段 [5,3,1] 找 target=3, mid=0, val=5 > 3 → left=1
//      mid=1, val=3 == 3 → 找到 ✓ (碰巧对了)
//      但换个例子: [5,4,3,2,1] 找 target=2, mid=2, val=3 > 2 → left=3
//      mid=3, val=2 == 2 → 找到 ✓ 
//      再换: [5,4,1] 找 target=4, mid=1, val=4 → 找到 ✓
//      但: [5,3,1] 找 target=1, mid=1, val=3 > 1 → left=2
//      mid=2, val=1 == 1 → 找到 ✓
//      虽然有些案例碰巧对，但 val < target 时方向是错的:
//      [5,4,3,2,1] 找 target=4, mid=2, val=3 < 4
//      ✗ left = mid+1 = 3 → 搜 [3,4] 区间 → 只剩 [2,1]，找不到4!
//      ✓ right = mid-1 = 1 → 搜 [0,1] 区间 → arr[1]=4，找到!
//
// 3. 升序段搜索范围漏掉 peak
//    ✗ right = peak - 1  → 如果 target 恰好是峰顶值，就找不到
//    ✓ right = peak       → 峰顶同时属于升序段的最后一个元素
//
// 4. 没有先搜升序段就返回降序段结果
//    题目要求最小下标，升序段下标 < 降序段下标
//    必须先搜升序段，找到就直接返回
//
// ============================================================
// 【面试追问】(递进链)
//
// Q1: 为什么不能线性扫描? 
//   → get() 调用限制100次，n 最大10^4。
//     必须利用山脉数组的有序结构做二分。
//
// Q2: 三次二分的 get() 调用次数上界是多少? 能否进一步优化?
//   → 找峰顶 ~2*log(n)≈28, 两次查找各 ~log(n)≈14, 总计 ~56
//     优化: 缓存 get() 结果，找峰顶降到 ~log(n)+1≈15
//     总计降到 ~43 次
//
// Q3: 如果允许重复元素怎么办?
//   → arr[mid] == arr[mid+1] 时无法判断峰顶方向，
//     最坏退化 O(n)（类似 LeetCode 81 的处理:
//     遇到相等只能 left++ 或 right-- 线性跳过）
//
// Q4: 如果不知道数组是山脉数组（可能是纯升序/纯降序）怎么处理?
//   → 先判断 arr[0] vs arr[1] 和 arr[n-2] vs arr[n-1]
//     全升序: 直接一次升序二分
//     全降序: 直接一次降序二分
//     否则: 标准三次二分
// ============================================================
