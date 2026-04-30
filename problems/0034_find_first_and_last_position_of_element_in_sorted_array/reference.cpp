/*
 * LeetCode 34: Find First and Last Position of Element in Sorted Array
 *
 * 【题目本质】
 * 在有序数组中找 target 的左边界和右边界。
 * 核心是"找到 target 后不停止，继续收缩区间到边界"。
 *
 * 【解法总览】
 * 解法1: 线性扫描          — O(n) / O(1)     — 暴力基准
 * 解法2: 两次独立二分       — O(log n) / O(1) — 面试首选（分别找左/右边界）
 * 解法3: 统一 lowerBound   — O(log n) / O(1) — 最优雅（只写一个函数复用两次）
 * 解法4: STL               — O(log n) / O(1) — 展示 STL 功底
 */

// ============================================================
// 解法1: 线性扫描 — 从两端各扫一遍
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 最直觉的做法：正向扫描找到第一个 target，反向扫描找到最后一个 target。
// 不满足题目 O(log n) 的要求，但可以作为正确性验证的基准。
//
// 搜索过程 (nums = [5,7,7,8,8,10], target = 8):
//   正向: 5→7→7→8(命中!) first=3
//   反向: 10→8(命中!) last=4
//   结果: [3, 4]
// ============================================================
class Solution1 {
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        int first = -1, last = -1;
        for (int i = 0; i < (int)nums.size(); i++) {
            if (nums[i] == target) {
                if (first == -1) first = i;  // 只记录第一次命中
                last = i;                      // 每次命中都更新 last
            }
        }
        return {first, last};
    }
};

// ============================================================
// 解法2: 两次独立二分 — 分别找左边界和右边界
// 时间: O(log n)  空间: O(1)
//
// 【思路】
// 解法1的瓶颈：有序数组的信息完全没利用，逐个扫描 O(n)。
// 有序 + O(log n) → 二分查找。
//
// 标准二分找到 target 就返回了，但我们需要的是"边界"。
// 关键改造：找到 target 后不停止，继续收缩。
//
// 找左边界 (findFirst):
//   nums[mid] < target  → mid一定不是答案 → left = mid + 1
//   nums[mid] >= target → mid可能是答案(或答案在更左) → right = mid
//   循环结束: left == right，检查是否等于 target
//
// 找右边界 (findLast):
//   nums[mid] > target  → mid一定不是答案 → right = mid - 1
//   nums[mid] <= target → mid可能是答案(或答案在更右) → left = mid
//   ⚠️ 此时 mid 必须向上取整，否则 left=mid 会死循环！
//
// 搜索空间收缩过程 (找左边界, target=8):
//   [5, 7, 7, 8, 8, 10]
//    L              R     mid=2, nums[2]=7 < 8 → L=3
//             [8, 8, 10]
//              L     R    mid=4, nums[4]=8 >= 8 → R=4
//             [8, 8]
//              L  R       mid=3, nums[3]=8 >= 8 → R=3
//             [8]
//              LR         L==R==3 → 检查 nums[3]=8==target ✓
//
// 搜索空间收缩过程 (找右边界, target=8):
//   [5, 7, 7, 8, 8, 10]
//    L              R     mid=3(向上取整), nums[3]=8 <= 8 → L=3
//             [8, 8, 10]
//              L     R    mid=4(向上取整), nums[4]=8 <= 8 → L=4
//                [8, 10]
//                 L   R   mid=5(向上取整), nums[5]=10 > 8 → R=4
//                [8]
//                 LR      L==R==4 → 检查 nums[4]=8==target ✓
// ============================================================
class Solution2 {
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        if (nums.empty()) return {-1, -1};

        int first = findFirst(nums, target);
        if (first == -1) return {-1, -1};  // target 不存在，无需找右边界
        int last = findLast(nums, target);

        return {first, last};
    }

private:
    // 找第一个等于 target 的位置（左边界）
    int findFirst(vector<int>& nums, int target) {
        int left = 0, right = (int)nums.size() - 1;
        while (left < right) {
            int mid = left + (right - left) / 2;  // 向下取整（默认）
            if (nums[mid] < target) {
                left = mid + 1;   // mid 一定不是答案
            } else {
                right = mid;      // mid 可能是答案，保留它
            }
        }
        // left == right，检查是否命中
        return nums[left] == target ? left : -1;
    }

    // 找最后一个等于 target 的位置（右边界）
    int findLast(vector<int>& nums, int target) {
        int left = 0, right = (int)nums.size() - 1;
        while (left < right) {
            // ⚠️ 向上取整！否则当 left+1==right 时 mid==left，left=mid 不变 → 死循环
            int mid = left + (right - left + 1) / 2;
            if (nums[mid] > target) {
                right = mid - 1;  // mid 一定不是答案
            } else {
                left = mid;       // mid 可能是答案，保留它
            }
        }
        return nums[left] == target ? left : -1;
    }
};

// ============================================================
// 解法3: 统一 lowerBound — 只写一个函数，复用两次
// 时间: O(log n)  空间: O(1)
//
// 【思路】
// 核心观察：
//   左边界 = 第一个 >= target 的位置 = lowerBound(target)
//   右边界 = 第一个 >= (target+1) 的位置 - 1 = lowerBound(target+1) - 1
//
// 这样只需要实现一个 lowerBound 函数即可解决问题。
// 这和 C++ STL 的 lower_bound / upper_bound 设计思路完全一致。
//
// lowerBound 的搜索空间是 [0, n]（注意包含 n）:
//   当所有元素都 < target 时，返回 n（表示不存在）
//
// 示例: nums = [5,7,7,8,8,10]
//   lowerBound(8)  → 3  (第一个 >= 8 的位置)
//   lowerBound(9)  → 5  (第一个 >= 9 的位置，即10所在位置)
//   左边界 = 3, 右边界 = lowerBound(9) - 1 = 5 - 1 = 4
//   结果: [3, 4] ✓
// ============================================================
class Solution3 {
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        int left = lowerBound(nums, target);

        // 检查 target 是否真的存在
        if (left == (int)nums.size() || nums[left] != target) {
            return {-1, -1};
        }

        // 右边界 = 第一个 >= (target+1) 的位置 - 1
        int right = lowerBound(nums, target + 1) - 1;

        return {left, right};
    }

private:
    // 返回第一个 >= target 的索引
    // 如果所有元素都 < target，返回 nums.size()
    int lowerBound(vector<int>& nums, int target) {
        int lo = 0, hi = (int)nums.size();  // 搜索空间 [0, n]
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (nums[mid] < target) {
                lo = mid + 1;  // mid 一定不满足 >= target
            } else {
                hi = mid;      // mid 满足 >= target，可能是答案
            }
        }
        return lo;  // lo == hi，收缩到一个点
    }
};

// ============================================================
// 解法4: 使用 STL lower_bound / upper_bound
// 时间: O(log n)  空间: O(1)
//
// 【思路】
// C++ STL 已经实现了我们解法3中的 lowerBound：
//   lower_bound: 第一个 >= target 的迭代器
//   upper_bound: 第一个 >  target 的迭代器
// 
// 左边界 = lower_bound 的位置
// 右边界 = upper_bound 的位置 - 1
//
// 面试中可以先写这个版本展示 STL 熟悉度，
// 面试官大概率追问"手写一个 lower_bound"。
// ============================================================
class Solution4 {
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        // lower_bound: 第一个 >= target
        auto left = lower_bound(nums.begin(), nums.end(), target);
        // upper_bound: 第一个 > target
        auto right = upper_bound(nums.begin(), nums.end(), target);

        // left == right 说明区间为空，即 target 不存在
        if (left == right) return {-1, -1};

        return {(int)(left - nums.begin()), (int)(right - nums.begin()) - 1};
    }
};

// ============================================================
// 【解法对比】
//
// | 解法     | 时间      | 空间 | 函数数量 | 易错程度 | 推荐场景     |
// |----------|-----------|------|----------|----------|-------------|
// | 线性扫描 | O(n)      | O(1) | 0        | 低       | 暖场/验证    |
// | 独立二分 | O(log n)  | O(1) | 2        | 中(取整) | 面试首选     |
// | 统一LB   | O(log n)  | O(1) | 1        | 低       | 最佳实践     |
// | STL      | O(log n)  | O(1) | 0        | 低       | 速度/展示STL |
//
// 解法2 vs 解法3:
//   - 解法2 写了两个函数 (findFirst/findLast)，findLast 需要向上取整容易出错
//   - 解法3 只写一个 lowerBound，通过 lowerBound(target+1)-1 得到右边界
//   - 解法3 更不容易出 bug，推荐优先掌握
//
// ============================================================
// 【易错点】
//
// 1. findLast 中 mid 不向上取整 → 死循环
//    ✗ int mid = left + (right - left) / 2;  // 配合 left = mid
//    ✓ int mid = left + (right - left + 1) / 2;
//    当 left=3, right=4 时，向下取整 mid=3，left=mid=3 不变 → 死循环
//
// 2. lowerBound 右边界初始化错误
//    ✗ int hi = nums.size() - 1;  // target 大于所有元素时无法返回 n
//    ✓ int hi = nums.size();       // 搜索空间 [0, n]，n 表示"都不满足"
//
// 3. 忘记验证 target 是否存在
//    ✗ return {lowerBound(nums, target), lowerBound(nums, target+1)-1};
//    ✓ 先检查 nums[left] == target，不等则返回 {-1, -1}
//    反例: nums=[5,7,9], target=6 → lowerBound(6)=1, nums[1]=7≠6
//
// 4. 空数组未特判
//    ✗ 直接调用 findFirst → nums[left] 越界
//    ✓ 开头 if (nums.empty()) return {-1, -1};
//    (解法3 的 lowerBound 在空数组时返回 0==nums.size()，自然走到不存在分支，
//     所以解法3 不需要额外特判——这也是它更优雅的原因之一)
//
// ============================================================
// 【面试追问】
//
// Q1(基础理解): "暴力怎么做？时间复杂度？为什么不够好？"
// → 线性扫描 O(n)。有序数组的"有序"信息完全没利用，应该用二分 O(log n)。
//
// Q2(核心追问): "标准二分找到 target 就停了，怎么改造找左边界？"
// → nums[mid]==target 时不返回，而是 right=mid 继续向左收缩。
//    循环结束时 left==right 就是左边界。
//
// Q3(统一思路): "能不能只写一个函数搞定两个边界？"
// → lowerBound(target) 找左边界，lowerBound(target+1)-1 找右边界。
//
// Q4(变体): "如果要统计 target 出现次数呢？"
// → count = right - left + 1 = lowerBound(target+1) - lowerBound(target)。
//    同样 O(log n)，比线性统计更快。
//
// Q5(深入): "left < right vs left <= right vs left + 1 < right，区别？"
// → left < right: 区间收缩到一个点退出，适合找边界（本题）
//   left <= right: 找到确切值就返回，区间可能为空退出
//   left + 1 < right: 保留两个候选，退出后检查两端，最安全但代码长
// ============================================================
