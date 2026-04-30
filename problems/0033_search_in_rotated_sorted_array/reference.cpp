/*
 * LeetCode 33: 搜索旋转排序数组 (Search in Rotated Sorted Array)
 *
 * 【题目本质】
 * 在一个被"掰成两段"的有序数组中用 O(log n) 找 target。
 * 核心：旋转后 mid 将数组分成两半，至少一半有序，利用有序半边决定搜索方向。
 *
 * 【解法总览】
 * 解法1: 线性扫描        — O(n)     / O(1) — 基础理解，不满足要求
 * 解法2: 两次二分        — O(log n) / O(1) — 先找旋转点，再标准二分
 * 解法3: 一次二分(推荐)  — O(log n) / O(1) — 判断有序半边，一步到位
 */

// ============================================================
// 解法1: 线性扫描 — 最朴素的想法
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 直接遍历找 target，完全没利用"部分有序"的信息。
// 作为基线理解，但不满足题目 O(log n) 要求。
// ============================================================
class Solution1 {
public:
    int search(vector<int>& nums, int target) {
        for (int i = 0; i < (int)nums.size(); i++) {
            if (nums[i] == target) return i;
        }
        return -1;
    }
};

// ============================================================
// 解法2: 两次二分 — 先找旋转点，再标准二分
// 时间: O(log n)  空间: O(1)
//
// 【思路】
// 旋转数组 = 两段有序数组拼接。
// 第一步：二分找最小值位置（旋转点），把数组分成两段有序的。
// 第二步：判断 target 在左段还是右段。
// 第三步：对那一段做标准二分。
//
// 为什么找旋转点用 nums[mid] > nums[hi] 而不是 nums[mid] > nums[lo]?
// 因为未旋转时（如 [1,2,3,4,5]），nums[mid] 总是 > nums[lo]，
// 会导致 lo 一直右移，最终得到错误的旋转点。
// 而 nums[mid] > nums[hi] 在未旋转时不成立，hi 左移到 0，正确。
//
// 搜索空间收缩 (找旋转点):
//   [4, 5, 6, 7, 0, 1, 2]
//    L        M        R   nums[M]=7 > nums[R]=2 → lo=M+1=4
//                 L  M  R  nums[M]=1 < nums[R]=2 → hi=M=5
//                 L  R     nums[M]=0 < nums[R]=1 → hi=M=4
//                 LR       L==R → pivot=4, nums[4]=0 ✓
// ============================================================
class Solution2 {
public:
    int search(vector<int>& nums, int target) {
        int n = nums.size();

        // ---- 第一步：找旋转点（最小值索引） ----
        int lo = 0, hi = n - 1;
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (nums[mid] > nums[hi]) {
                // 最小值在 mid 右边（mid 不可能是最小值，因为 nums[hi] 更小）
                lo = mid + 1;
            } else {
                // 最小值在 mid 或 mid 左边
                hi = mid;
            }
        }
        int pivot = lo;

        // ---- 第二步：判断 target 在哪一段 ----
        // 右段范围: [pivot, n-1]，值域: [nums[pivot], nums[n-1]]
        // 左段范围: [0, pivot-1]，值域: [nums[0], nums[pivot-1]]
        if (target >= nums[pivot] && target <= nums[n - 1]) {
            lo = pivot;
            hi = n - 1;
        } else {
            lo = 0;
            hi = pivot - 1;
        }

        // ---- 第三步：标准二分查找 ----
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (nums[mid] == target) return mid;
            else if (nums[mid] < target) lo = mid + 1;
            else hi = mid - 1;
        }
        return -1;
    }
};

// ============================================================
// 解法3: 一次二分 — 判断有序半边，一步到位 ⭐ 面试首选
// 时间: O(log n)  空间: O(1)
//
// 【思路】
// 核心观察：mid 把旋转数组分成两半，至少一半是有序的。
//   - nums[left] <= nums[mid] → 左半 [left, mid] 有序
//   - 否则 → 右半 [mid, right] 有序
//
// 找到有序的那半边后，判断 target 是否落在其范围内：
//   - 是 → 在有序半边里继续二分
//   - 否 → 去另一半边
//
// 为什么 <= 而不是 <？
// 当 left == mid 时（区间只有 1-2 个元素），左半只有一个元素，
// 它当然是"有序"的。如果用 <，会错误地走到 else 分支。
//
// 搜索空间收缩过程：nums = [4,5,6,7,0,1,2], target = 0
//
// Round1: L=0, R=6, mid=3
//   [4, 5, 6, 7, 0, 1, 2]
//    L        M        R
//   nums[L]=4 <= nums[M]=7 → 左半有序 [4,5,6,7]
//   target=0 在 [4, 7) 内? No (4<=0 不成立)
//   → L = mid+1 = 4
//
// Round2: L=4, R=6, mid=5
//   [4, 5, 6, 7, 0, 1, 2]
//                 L  M  R
//   nums[L]=0 <= nums[M]=1 → 左半有序 [0,1]
//   target=0 在 [0, 1) 内? Yes (0<=0 且 0<1)
//   → R = mid-1 = 4
//
// Round3: L=4, R=4, mid=4
//   nums[mid]=0 == target=0 → 返回 4 ✓
// ============================================================
class Solution3 {
public:
    int search(vector<int>& nums, int target) {
        int left = 0, right = (int)nums.size() - 1;

        while (left <= right) {
            int mid = left + (right - left) / 2;

            // 命中，直接返回
            if (nums[mid] == target) return mid;

            if (nums[left] <= nums[mid]) {
                // 左半 [left, mid] 有序
                // 判断 target 是否在这个有序区间 [nums[left], nums[mid]) 内
                // 注意：nums[mid] 已经检查过不等于 target，所以用严格 <
                if (nums[left] <= target && target < nums[mid]) {
                    right = mid - 1;  // target 在左半，缩右边界
                } else {
                    left = mid + 1;   // target 不在左半，去右半找
                }
            } else {
                // 右半 [mid, right] 有序
                // 判断 target 是否在这个有序区间 (nums[mid], nums[right]] 内
                if (nums[mid] < target && target <= nums[right]) {
                    left = mid + 1;   // target 在右半，缩左边界
                } else {
                    right = mid - 1;  // target 不在右半，去左半找
                }
            }
        }

        return -1;
    }
};

// ============================================================
// 【解法对比】
//
// 解法1 线性扫描:  O(n)，不满足题目要求，但可以用来对拍验证。
// 解法2 两次二分:  O(log n)，逻辑分步清晰（找旋转点 → 定段 → 标准二分），
//                  适合思路不够清晰时使用，不容易出 bug。
// 解法3 一次二分:  O(log n)，代码最简洁，面试首选。
//                  需要仔细处理 <= 和 < 的取等方向。
//
// 面试建议：直接写解法3。如果写不出来，退到解法2也完全可以接受。
//
// 【易错点】
//
// 1. nums[left] <= nums[mid] 中忘记取等号
//    ✗ if (nums[left] < nums[mid])
//    ✓ if (nums[left] <= nums[mid])
//    当 left==mid（区间只剩1-2个元素）时，< 会错误判断左半"无序"。
//    例如 nums=[3,1], left=0, mid=0: nums[0]=3 < nums[0]=3 为 false，
//    走到 else 分支，把右半 [0,1] 当有序，但实际左半只有一个元素才是有序的。
//
// 2. target 范围判断只写了一个条件
//    ✗ if (target < nums[mid]) right = mid - 1;
//    ✓ if (nums[left] <= target && target < nums[mid]) right = mid - 1;
//    少了 nums[left] <= target 这个下界约束，会把不在左半范围内的 target
//    错误地留在左半边搜索。比如 nums=[4,5,6,7,0,1,2], target=1, mid=3(值7):
//    1 < 7 为 true，如果只判断这一个条件会让 right=2，但 target=1 在右半边。
//
// 3. 找旋转点时用 nums[mid] > nums[lo] 判断
//    ✗ if (nums[mid] > nums[lo]) lo = mid + 1;
//    ✓ if (nums[mid] > nums[hi]) lo = mid + 1;
//    未旋转数组 [1,2,3,4,5] 中，nums[mid] 总是 > nums[lo]，
//    lo 一直右移最终错误地认为旋转点在末尾。
//
// 4. 整数溢出：mid = (left + right) / 2 可能溢出
//    ✗ int mid = (left + right) / 2;
//    ✓ int mid = left + (right - left) / 2;
//    本题数据范围小不会出问题，但养成好习惯。
//
// 【面试追问】
//
// Q1: 两次二分 vs 一次二分的优劣？
//   两次二分逻辑更清晰，每步都是标准操作，适合紧张时使用。
//   一次二分代码更简洁，展示对问题的深刻理解，面试官更喜欢。
//
// Q2: 如果有重复元素怎么办？(→ LeetCode 81)
//   nums[left] == nums[mid] 时无法判断哪边有序。
//   例如 [1,3,1,1,1] 左半有序，[1,1,1,3,1] 右半有序，但 left 和 mid 值相同。
//   解法：遇到 nums[left] == nums[mid] 时 left++，最坏退化为 O(n)。
//   具体改动：在 if (nums[left] <= nums[mid]) 前加一个分支：
//     if (nums[left] == nums[mid]) { left++; continue; }
//
// Q3: 如果只需要找最小值呢？(→ LeetCode 153)
//   就是解法2的第一步。用 nums[mid] > nums[hi] 判断最小值在右半边。
//   比本题更简单，因为不需要搜索特定 target。
//
// Q4: 多次查询不同 target 怎么优化？
//   预处理一次找到旋转点 O(log n)，记录下来。
//   之后每次查询先 O(1) 判断 target 在哪段，再 O(log n) 标准二分。
// ============================================================
