/*
 * LeetCode 704: 二分查找 (Binary Search)
 *
 * 【题目本质】
 * 在有序无重复数组中查找 target，返回下标或 -1。
 * 这是二分查找的"标准模板题"，所有二分变体的基础。
 *
 * 【解法总览】
 * 解法1: 线性扫描              — O(n) / O(1)    — 验证理解
 * 解法2: 二分查找（闭区间）      — O(log n) / O(1) — ⭐ 面试首选
 * 解法3: 二分查找（左闭右开）    — O(log n) / O(1) — STL 风格变体
 * 解法4: 递归二分              — O(log n) / O(log n) — 递归练习
 */

// ============================================================
// 解法1: 线性扫描 — 逐个比较，作为 baseline
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 最朴素的想法：从头到尾扫一遍，找到就返回。
// 这个解法没有利用"数组有序"的条件，所以是 O(n)。
// 它的价值是：(1) 确认题意理解正确 (2) 暴露瓶颈 → 引出二分。
//
// 瓶颈：每个元素都要看一遍。但数组有序啊！
// 如果我看中间的元素，就能一次排除一半 → 二分。
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
// 解法2: 二分查找（闭区间 [left, right]） — ⭐ 面试首选
// 时间: O(log n)  空间: O(1)
//
// 【思路】
// 有序 → 看中间值就能排除一半 → 二分。
//
// 关键是"搜索区间的定义"，这里用闭区间 [left, right]：
//   - left 和 right 都是"可能包含 target"的合法位置
//   - 循环条件：left <= right（[left, left] 仍有 1 个元素要检查）
//   - 更新规则：mid 已检查过，所以 left = mid+1 或 right = mid-1
//
// 搜索空间收缩过程 (nums = [-1,0,3,5,9,12], target = 9):
//
//   Round1: left=0, right=5
//           [-1, 0, 3, 5, 9, 12]
//                    ↑ mid=2, nums[2]=3 < 9 → left=3
//
//   Round2: left=3, right=5
//           [_, _, _, 5, 9, 12]
//                       ↑ mid=4, nums[4]=9 == 9 → return 4 ✓
//
// 搜索空间收缩过程 (target = 2, 不存在):
//
//   Round1: mid=2, nums[2]=3 > 2 → right=1   区间 [0,1]
//   Round2: mid=0, nums[0]=-1 < 2 → left=1   区间 [1,1]
//   Round3: mid=1, nums[1]=0 < 2  → left=2   区间 [2,1] → 空！
//   return -1 ✓
// ============================================================
class Solution2 {
public:
    int search(vector<int>& nums, int target) {
        int left = 0, right = (int)nums.size() - 1;  // 闭区间 [0, n-1]

        while (left <= right) {  // 区间 [left, right] 非空就继续
            // 防溢出：不用 (left + right) / 2
            int mid = left + (right - left) / 2;

            if (nums[mid] == target) {
                return mid;  // 找到了，直接返回
            } else if (nums[mid] < target) {
                // mid 太小，target 只可能在 mid 右边
                // mid 已经排除了，所以新区间从 mid+1 开始
                left = mid + 1;
            } else {
                // mid 太大，target 只可能在 mid 左边
                // mid 已经排除了，所以新区间到 mid-1 结束
                right = mid - 1;
            }
        }

        return -1;  // left > right → 搜索空间为空，没找到
    }
};

// ============================================================
// 解法3: 二分查找（左闭右开 [left, right)） — STL 风格
// 时间: O(log n)  空间: O(1)
//
// 【思路】
// 和解法2 逻辑完全一致，但搜索区间定义为 [left, right)：
//   - right 不在搜索范围内
//   - 初始化：right = n（而不是 n-1），因为 [0, n) 才覆盖所有索引
//   - 循环条件：left < right（当 left == right 时 [left, left) 为空）
//   - 右边界更新：right = mid（不是 mid-1！因为 right 本身不含在区间内）
//
// 为什么要学这个？
//   C++ STL 的 lower_bound/upper_bound 就是这种区间风格。
//   掌握两种写法，面试中能解释差异 = 加分。
//
// 对比解法2 的改动（标注 ← CHANGED）：
// ============================================================
class Solution3 {
public:
    int search(vector<int>& nums, int target) {
        int left = 0, right = (int)nums.size();  // ← CHANGED: n 而不是 n-1

        while (left < right) {  // ← CHANGED: < 而不是 <=
            int mid = left + (right - left) / 2;

            if (nums[mid] == target) {
                return mid;
            } else if (nums[mid] < target) {
                left = mid + 1;  // 和闭区间一样
            } else {
                right = mid;     // ← CHANGED: mid 而不是 mid-1
                                 // 因为 right 本身不含在 [left, right) 内
                                 // [left, mid) 已经正确排除了 mid
            }
        }

        return -1;
    }
};

// ============================================================
// 解法4: 递归二分 — 递归方式表达同样的逻辑
// 时间: O(log n)  空间: O(log n) — 递归栈深度
//
// 【思路】
// 二分本质是"缩小问题规模"：在 [left, right] 上找 target
// → 在 [left, mid-1] 或 [mid+1, right] 上找 target。
// 这天然是递归结构。
//
// 缺点：O(log n) 额外栈空间，且有函数调用开销。
// 实际面试和工程中优先迭代版。
// ============================================================
class Solution4 {
public:
    int search(vector<int>& nums, int target) {
        return binarySearch(nums, target, 0, (int)nums.size() - 1);
    }

private:
    int binarySearch(vector<int>& nums, int target, int left, int right) {
        // base case：搜索空间为空
        if (left > right) return -1;

        int mid = left + (right - left) / 2;

        if (nums[mid] == target) {
            return mid;
        } else if (nums[mid] < target) {
            // 递归搜索右半边
            return binarySearch(nums, target, mid + 1, right);
        } else {
            // 递归搜索左半边
            return binarySearch(nums, target, left, mid - 1);
        }
    }
};

// ============================================================
// 【解法对比】
//
// | 解法 | 时间 | 空间 | 区间定义 | 循环条件 | right更新 | 场景 |
// |------|------|------|----------|----------|-----------|------|
// | 线性扫描 | O(n) | O(1) | - | - | - | baseline |
// | 闭区间 ⭐ | O(logn)| O(1) | [l,r] | l<=r | r=mid-1 | 面试首选 |
// | 左闭右开 | O(logn)| O(1) | [l,r) | l<r | r=mid | STL风格 |
// | 递归 | O(logn)| O(logn)| [l,r] | 递归终止 | mid-1 | 教学用 |
//
// 闭区间 vs 左闭右开：
//   - 逻辑等价，差异全源于区间定义
//   - 闭区间更直觉（两端都包含），面试推荐
//   - 左闭右开与 STL 一致（begin/end 风格）
//
// 迭代 vs 递归：
//   - 逻辑完全一致
//   - 迭代省空间(O(1) vs O(logn))，无栈溢出风险
//   - 实际开发和面试中优先迭代
//
// ============================================================
// 【易错点】
//
// 1. mid 计算溢出:
//    ✗ int mid = (left + right) / 2;
//      → left=2^30, right=2^30 时 left+right 超过 INT_MAX
//    ✓ int mid = left + (right - left) / 2;
//      → right-left 不会溢出（都是非负数且 right >= left）
//
// 2. 闭区间写法用了 left < right（少了等号）:
//    ✗ while (left < right) 配 right = mid - 1
//      → 当 left == right 时直接退出，但 nums[left] 可能就是 target！
//      → 例如 nums=[5], target=5: left=0, right=0, 直接跳过循环返回-1
//    ✓ while (left <= right) 配 right = mid - 1
//
// 3. 左闭右开写法中 right = mid - 1:
//    ✗ right = mid - 1 在 [left, right) 中
//      → 新区间 [left, mid-1)，但 nums[mid-1] 还没检查过却被排除了！
//    ✓ right = mid → 新区间 [left, mid)，mid 被排除，mid-1 仍在范围内
//
// 4. 搞混两种写法的"套件":
//    闭区间套件: right=n-1, while(<=), right=mid-1
//    左闭右开套件: right=n, while(<), right=mid
//    三个细节必须配套，混用必出 bug。
//
// ============================================================
// 【面试追问】
//
// Q1: 时间复杂度为什么是 O(log n)？
// A1: 每次比较排除一半搜索空间。n 个元素经过 k 次后剩 n/2^k。
//     当 n/2^k = 1 时 k = log₂n，所以最多 log₂n 次比较。
//
// Q2: 如果数组有重复元素，怎么找 target 第一次/最后一次出现的位置？
// A2: 找到 nums[mid]==target 时不返回，而是继续缩小边界：
//     - 找第一个: right = mid (或 right = mid-1)，最终 left 指向第一个
//     - 找最后一个: left = mid + 1，最终 right 指向最后一个
//     → 这就是 lower_bound / upper_bound 的原理。参考 LeetCode 34。
//
// Q3: 如果搜索空间不是数组索引而是一个连续的值域呢？
// A3: 二分的本质是"在满足单调性的搜索空间上，每次排除一半"。
//     搜索空间可以是：数组索引、答案范围、甚至浮点数区间。
//     例如 LeetCode 69 (Sqrt(x))：在 [0, x] 上二分，
//     判断条件是 mid*mid <= x。
//
// Q4: 能否用 STL 一行解决？实际项目中你会怎么写？
// A4: auto it = lower_bound(nums.begin(), nums.end(), target);
//     return (it != nums.end() && *it == target) ? it - nums.begin() : -1;
//     面试中先手写展示基本功，再提 STL 展示工程素养。
// ============================================================
