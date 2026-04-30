/*
 * LeetCode 491: 非递减子序列 (Non-decreasing Subsequences)
 *
 * 【题目本质】
 * 在不改变原数组顺序的前提下，枚举所有长度 ≥ 2 的非递减子序列，不能有重复。
 * 核心难点：不能排序（排序会改变非递减关系），去重策略和子集 II 不同。
 *
 * 【解法总览】
 * 解法1: 回溯 + unordered_set 同层去重 — O(n·2^n) / O(n) — 通用做法
 * 解法2: 回溯 + 数组哈希同层去重     — O(n·2^n) / O(n) — 利用值域有限加速
 * 解法3: 位掩码枚举 + set 去重       — O(n²·2^n) / O(n·2^n) — 暴力基准
 */

// ============================================================
// 解法1: 回溯 + unordered_set 同层去重
// 时间: O(n·2^n)  空间: O(n) 递归深度
//
// 【思路】
// 子序列枚举 = 子集枚举框架：对每个位置决定选不选。
// 两个约束：
//   1) 非递减：新选的元素 >= path 的最后一个
//   2) 去重：在回溯树的同一层(同一个 for 循环)中，
//            相同值只选第一次出现的那个
//
// 不能排序！排序后原始数组的非递减关系被破坏了。
// 所以用每层的局部 unordered_set 记录"这层已经选过哪些值"。
//
// 决策树示意 (nums = [4, 6, 7, 7]):
//
//                  []
//            /      |       \        \
//         [4]      [6]     [7]     [7] ← 同层值=7已选过，跳过!
//       / | \      | \       |
//   [4,6][4,7][4,7] [6,7][6,7] [7,7]
//          ↑值7重复↑     ↑跳过↑
//    / \    |       |
// [4,6,7][4,6,7] [4,7,7] [6,7,7]
//     ↑跳过↑
//    |
// [4,6,7,7]
//
// 收集时机：path.size() >= 2 时就收集，但不 return（还可继续延长）
// ============================================================
class Solution1 {
public:
    vector<vector<int>> findSubsequences(vector<int>& nums) {
        vector<vector<int>> result;
        vector<int> path;
        backtrack(nums, 0, path, result);
        return result;
    }

private:
    void backtrack(vector<int>& nums, int start, vector<int>& path,
                   vector<vector<int>>& result) {
        // 长度 >= 2 就是一个合法的非递减子序列
        if (path.size() >= 2) {
            result.push_back(path);
            // 不 return！子序列还可以继续延长
        }

        // 同层去重：unordered_set 记录当前 for 循环中已选过的值
        // 这是局部变量，每层递归独立，退出时自动销毁
        unordered_set<int> usedInThisLevel;

        for (int i = start; i < (int)nums.size(); i++) {
            // 约束1: 非递减 — 当前值必须 >= path 末尾
            if (!path.empty() && nums[i] < path.back()) continue;

            // 约束2: 同层去重 — 这个值在当前层已经选过
            if (usedInThisLevel.count(nums[i])) continue;

            usedInThisLevel.insert(nums[i]);

            path.push_back(nums[i]);
            backtrack(nums, i + 1, path, result);  // i+1: 子序列不复用同一位置
            path.pop_back();  // 撤销选择
        }
    }
};

// ============================================================
// 解法2: 回溯 + 数组哈希同层去重（值域优化版）
// 时间: O(n·2^n)  空间: O(n)
//
// 【思路】
// 和解法1完全相同的回溯逻辑，唯一区别是去重的数据结构：
// 因为 nums[i] ∈ [-100, 100]，共 201 个可能值，
// 用 bool used[201] 替代 unordered_set，常数更小。
//
// 映射方式：nums[i] + 100 → 数组索引 [0, 200]
// 每层递归创建新的 bool 数组（栈上分配，开销很小）
// ============================================================
class Solution2 {
public:
    vector<vector<int>> findSubsequences(vector<int>& nums) {
        vector<vector<int>> result;
        vector<int> path;
        backtrack(nums, 0, path, result);
        return result;
    }

private:
    void backtrack(vector<int>& nums, int start, vector<int>& path,
                   vector<vector<int>>& result) {
        if (path.size() >= 2) {
            result.push_back(path);
        }

        // 值域 [-100, 100] 映射到 [0, 200]，用数组比 set 更快
        bool used[201] = {false};

        for (int i = start; i < (int)nums.size(); i++) {
            // 非递减约束
            if (!path.empty() && nums[i] < path.back()) continue;
            // 同层去重：+100 偏移避免负索引
            if (used[nums[i] + 100]) continue;

            used[nums[i] + 100] = true;
            path.push_back(nums[i]);
            backtrack(nums, i + 1, path, result);
            path.pop_back();
        }
    }
};

// ============================================================
// 解法3: 位掩码枚举 + set 去重
// 时间: O(n²·2^n)  空间: O(n·2^n)
//
// 【思路】
// n ≤ 15，直接枚举所有 2^n 个子集。
// 对每个子集：
//   1) 长度 < 2 → 跳过
//   2) 不是非递减 → 跳过
//   3) 加入 set<vector<int>> 自动去重
//
// 这是最暴力的做法，用来验证回溯解法的正确性。
// 缺点：set<vector<int>> 的插入和比较都是 O(n)，常数大。
//
// 位掩码示意 (n=4):
//   mask = 0b1011 → 选 nums[0], nums[1], nums[3]
//   mask = 0b1100 → 选 nums[2], nums[3]
// ============================================================
class Solution3 {
public:
    vector<vector<int>> findSubsequences(vector<int>& nums) {
        int n = nums.size();
        set<vector<int>> resultSet;

        // 枚举 2^n 个子集
        for (int mask = 0; mask < (1 << n); mask++) {
            vector<int> sub;
            for (int i = 0; i < n; i++) {
                if (mask & (1 << i)) {
                    sub.push_back(nums[i]);
                }
            }

            if ((int)sub.size() < 2) continue;

            // 检查是否非递减
            bool valid = true;
            for (int i = 1; i < (int)sub.size(); i++) {
                if (sub[i] < sub[i - 1]) {
                    valid = false;
                    break;
                }
            }

            if (valid) {
                resultSet.insert(sub);  // set 自动去重
            }
        }

        return vector<vector<int>>(resultSet.begin(), resultSet.end());
    }
};

// ============================================================
// 【解法对比】
//
// | 特性         | 解法1 (set去重)    | 解法2 (数组去重)  | 解法3 (位掩码)     |
// |-------------|-------------------|------------------|-------------------|
// | 去重方式     | unordered_set     | bool[201]        | set<vector<int>>  |
// | 时间复杂度   | O(n·2^n)          | O(n·2^n)         | O(n²·2^n)         |
// | 空间复杂度   | O(n)              | O(n)             | O(n·2^n)          |
// | 常数开销     | 哈希计算有开销     | 最小              | set 比较开销大     |
// | 适用场景     | 值域不确定时通用   | 值域有限时最优     | 验证正确性/暴力   |
//
// 面试推荐：解法2（值域有限时）或 解法1（通用）。
// 先说解法3暴力思路展示理解，再优化到回溯。
//
// ============================================================
// 【易错点】
//
// 1. ✗ 排序后用子集 II 的去重方法：
//      sort(nums.begin(), nums.end());
//      if (i > start && nums[i] == nums[i-1]) continue;
//    这会改变原数组顺序，[4,4,3,2,1] 排序后变 [1,2,3,4,4]，
//    原本只有 [4,4] 一个答案，排序后会多出 [1,2], [1,3] 等。
//    ✓ 不排序，用同层 set/数组去重。
//
// 2. ✗ 收集结果后 return：
//      if (path.size() >= 2) { result.push_back(path); return; }
//    这会导致只收集长度为 2 的子序列，漏掉 [4,6,7], [4,6,7,7] 等。
//    ✓ 收集后不 return，继续探索更长的子序列。
//
// 3. ✗ 把 used 定义在类成员或传参不重置：
//    不同层递归共享同一个 used，导致深层递归中的选择被浅层屏蔽。
//    ✓ used 必须是每层递归的局部变量。
//
// 4. ✗ 数组去重忘记 +100 偏移：
//      used[nums[i]] = true;  // nums[i]=-100 时数组越界！
//    ✓ used[nums[i] + 100] = true;
//
// 5. ✗ for 循环从 0 开始而非 start：
//      for (int i = 0; ...) // 会导致选已经跳过的位置，产生重复
//    ✓ for (int i = start; ...)
//
// ============================================================
// 【面试追问】
//
// Q1 (基础): 这题和 90.子集II 的区别是什么？为什么不能排序？
//    → 90 题只要求不重复子集，排序不影响子集的组成（无序）。
//      本题要求"非递减子序列"，依赖原数组的顺序关系，
//      排序后顺序变了，非递减子序列的集合完全不同。
//
// Q2 (去重深入): 同层去重用局部变量 vs 全局变量 + 回溯重置，有什么区别？
//    → 全局变量 + 回溯重置的方式是在 backtrack 返回后
//      把 used[nums[i]] 重置为 false。但这样做会使得
//      同层 for 循环后续迭代中，之前选过的值又"可选"了，
//      去重完全失效。
//      used 的生命周期必须覆盖整个 for 循环但不跨层，
//      局部变量天然满足这个要求。
//
// Q3 (变体): 如果要求严格递增（不允许相等），怎么改？
//    → 把 `nums[i] < path.back()` 改为 `nums[i] <= path.back()`，
//      即相等时也跳过。只改一个比较运算符。
//
// Q4 (优化): 如果 n 很大（比如 n=30），还能这样做吗？
//    → 2^30 ≈ 10^9，纯枚举不可行。需要考虑：
//      - 如果只求数量，可以用 DP
//      - 如果只求最长，用 LIS 的 O(n log n) 算法
//      - 如果必须枚举所有，结果本身可能指数级，无法避免
//
// ============================================================
