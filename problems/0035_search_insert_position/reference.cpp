/*
 * LeetCode 35: 搜索插入位置 (Search Insert Position)
 *
 * 【题目本质】
 * 在有序无重复数组中，找到第一个 >= target 的下标（即 lower_bound）。
 * 如果 target 存在，该下标就是 target 的索引；
 * 如果 target 不存在，该下标就是它应该插入的位置。
 *
 * 【解法总览】
 * 解法1: 线性扫描        — O(n)     / O(1) — 最直觉，理解题意
 * 解法2: 二分(左闭右闭)   — O(log n) / O(1) — 面试标准答案 ⭐
 * 解法3: 二分(左闭右开)   — O(log n) / O(1) — 另一种常用模板
 * 解法4: STL lower_bound — O(log n) / O(1) — 工程写法
 */

// ============================================================
// 解法1: 线性扫描 — 从左到右找第一个 >= target 的位置
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 数组有序，从左往右扫，第一个 >= target 的元素位置就是答案。
// 如果所有元素都 < target，答案就是 n（插入到末尾）。
//
// 虽然不满足题目 O(log n) 的要求，但它帮我们理解了问题的本质：
// "搜索插入位置" == "找第一个 >= target 的下标" == lower_bound
// ============================================================
class Solution1 {
public:
    int searchInsert(vector<int>& nums, int target) {
        for (int i = 0; i < (int)nums.size(); i++) {
            if (nums[i] >= target) return i;
        }
        return nums.size();
    }
};

// ============================================================
// 解法2: 二分查找（左闭右闭模板 [left, right]） ⭐ 面试首选
// 时间: O(log n)  空间: O(1)
//
// 【思路】
// 解法1的瓶颈：线性扫描没有利用有序性。
// 有序 + O(log n) → 二分查找。
//
// 核心观察：不管 target 是否在数组中，答案都是"第一个 >= target 的下标"。
// 所以我们用二分找 lower_bound。
//
// 循环不变量：
//   - [0, left-1] 中的元素全部 < target（已排除）
//   - [right+1, n-1] 中的元素全部 >= target（已排除）
//   - [left, right] 是待搜索区间
//
// 循环结束时 left = right + 1，所以 left 恰好是分界点。
//
// 【搜索空间收缩过程示例】
//
// 例1: nums = [1,3,5,6], target = 5（存在）
// Round1: left=0, right=3, mid=1, nums[1]=3 < 5   → left=2
//                              ↑ 排除左半
// Round2: left=2, right=3, mid=2, nums[2]=5 >= 5  → right=1
//                              ↑ 可能是答案，收缩右边界
// left=2 > right=1 → 结束，返回 left=2 ✓ (nums[2]=5)
//
// 例2: nums = [1,3,5,6], target = 2（不存在，应插入位置1）
// Round1: left=0, right=3, mid=1, nums[1]=3 >= 2  → right=0
//                              ↑
// Round2: left=0, right=0, mid=0, nums[0]=1 < 2   → left=1
//                          ↑
// left=1 > right=0 → 结束，返回 left=1 ✓
//
// 例3: nums = [1,3,5,6], target = 7（大于所有元素，应返回4）
// Round1: left=0, right=3, mid=1, nums[1]=3 < 7   → left=2
// Round2: left=2, right=3, mid=2, nums[2]=5 < 7   → left=3
// Round3: left=3, right=3, mid=3, nums[3]=6 < 7   → left=4
// left=4 > right=3 → 结束，返回 left=4 ✓ (插入末尾)
// ============================================================
class Solution2 {
public:
    int searchInsert(vector<int>& nums, int target) {
        int left = 0, right = (int)nums.size() - 1;
        
        while (left <= right) {
            // 防溢出写法：不用 (left + right) / 2
            int mid = left + (right - left) / 2;
            
            if (nums[mid] < target) {
                // nums[mid] 比 target 小，mid 及其左边都不可能是答案
                left = mid + 1;
            } else {
                // nums[mid] >= target，mid 可能是答案
                // 但左边也许还有更小的 >= target 的元素，继续收缩
                right = mid - 1;
            }
        }
        
        // left 就是第一个 >= target 的下标
        // 如果 target 比所有元素都大，left == nums.size()
        return left;
    }
};

// ============================================================
// 解法3: 二分查找（左闭右开模板 [left, right)）
// 时间: O(log n)  空间: O(1)
//
// 【思路】
// 和解法2完全等价，只是用不同的区间约定。
// 左闭右开与 C++ STL 的 lower_bound 行为一致。
//
// 三个关键差异（vs 解法2）：
//   1. right 初始化为 n（不是 n-1），因为 right 本身不在搜索范围内
//   2. 循环条件 left < right（不是 <=），因为 left==right 时 [left, right) 为空
//   3. 收缩右边界 right = mid（不是 mid-1），因为开区间已经排除了 right
// ============================================================
class Solution3 {
public:
    int searchInsert(vector<int>& nums, int target) {
        int left = 0, right = (int)nums.size();  // 差异1: right = n
        
        while (left < right) {                    // 差异2: < 不是 <=
            int mid = left + (right - left) / 2;
            
            if (nums[mid] < target) {
                left = mid + 1;
            } else {
                right = mid;                      // 差异3: mid 不是 mid-1
            }
        }
        
        // left == right, 搜索区间为空
        return left;
    }
};

// ============================================================
// 解法4: STL lower_bound — 工程写法
// 时间: O(log n)  空间: O(1)
//
// 【思路】
// C++ 标准库已经封装了 lower_bound（第一个 >= target 的位置）。
// 面试中可以提及展示工程意识，但面试官通常要求手写。
// ============================================================
class Solution4 {
public:
    int searchInsert(vector<int>& nums, int target) {
        return lower_bound(nums.begin(), nums.end(), target) - nums.begin();
    }
};

// ============================================================
// 【解法对比】
//
// | 解法            | 时间     | 空间  | 特点                         |
// |----------------|----------|-------|------------------------------|
// | 线性扫描        | O(n)    | O(1)  | 最直觉，不满足题目要求         |
// | 二分(左闭右闭)  | O(log n)| O(1)  | 面试标准答案，直觉友好         |
// | 二分(左闭右开)  | O(log n)| O(1)  | 与 STL 风格一致              |
// | STL            | O(log n)| O(1)  | 一行搞定，展示工程意识         |
//
// 面试建议：选一种模板练熟，始终用同一种。
// 左闭右闭更直觉（两端都在搜索范围内），
// 左闭右开与 STL 一致（转换成库函数更自然）。
//
// ============================================================
// 【易错点】
//
// 1. ✗ int mid = (left + right) / 2;
//    ✓ int mid = left + (right - left) / 2;
//    原因：当 left + right 超过 INT_MAX 时会溢出。
//          本题数据量小不会出错，但在 n=2*10^9 的题目中会 bug。
//
// 2. ✗ 左闭右闭模板中写 right = mid（应该是 mid - 1）
//    原因：当 left == right == mid 且 nums[mid] >= target 时，
//          right = mid 不会改变任何值 → 死循环！
//          必须写 right = mid - 1 才能缩小搜索区间。
//
// 3. ✗ 左闭右开模板中写 right = mid - 1（应该是 mid）
//    原因：开区间 [left, right) 不包含 right，
//          如果写 right = mid - 1，会跳过 mid 这个候选答案。
//          例如 [1,3], target=3, mid=1, nums[1]=3 >= 3,
//          写 right=0 就漏掉了 index=1 这个正确答案。
//
// 4. ✗ 把 nums[mid] == target 单独判断并直接 return mid;
//    虽然本题（无重复元素）AC 没问题，但这不是 lower_bound。
//    一旦遇到有重复元素的变体（如 LeetCode 34），就会找不到"第一个"的位置。
//    养成找边界的习惯，别贪图"找到就返回"的快感。
//
// 5. ✗ 特判 target > nums[n-1] 后 return n;
//    不需要。二分自然能处理：所有元素 < target 时 left 会一路右移到 n。
//    多余的特判不仅不必要，还容易引入新 bug。
//
// ============================================================
// 【面试追问（递进链）】
//
// Q1: "你能用暴力解法做吗？瓶颈在哪？"
//  → 线性扫描 O(n)，瓶颈是没有利用数组有序的性质。
//
// Q2: "二分结束后为什么 left 就是答案？怎么证明？"
//  → 循环不变量：[0, left-1] 全部 < target，[right+1, n-1] 全部 >= target。
//    结束时 left = right + 1，所以 left 是"第一个 >= target"的位置。
//
// Q3: "如果数组有重复元素，找 target 的第一次出现和最后一次出现怎么做？"
//  → 第一次出现：本题的 lower_bound 不变，结束后检查 nums[left] == target。
//    最后一次出现：改为找 upper_bound（第一个 > target），然后减 1。
//    这就是 LeetCode 34 的做法。
//
// Q4: "lower_bound 和 upper_bound 的代码差异在哪？"
//  → 只有一行不同：nums[mid] < target（lower_bound）
//                  vs nums[mid] <= target（upper_bound）
//    即 == 的情况归到哪边：lower_bound 把 == 归右（继续收缩右边界），
//    upper_bound 把 == 归左（继续收缩左边界）。
//
// 【相关题型】
//
// - 704. 二分查找 — 最基础的"找精确值"二分。
//   复用：同样的二分框架，但 704 找到就返回，本题要找边界。
//
// - 34. 在排序数组中查找元素的第一个和最后一个位置
//   复用：本题的 lower_bound 模板调用两次。
//   代码差异：第二次把 `nums[mid] < target` 改成 `nums[mid] <= target` 就是 upper_bound。
//
// - 69. x 的平方根 — 搜索空间不是数组索引，而是答案范围 [0, x]。
//   复用："找边界"思想，找最大的 k 使得 k*k <= x。
//
// - 278. 第一个错误的版本 — 找左边界，模板与本题左闭右开版本几乎相同。
// ============================================================
